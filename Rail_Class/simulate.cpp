/**
 *@author Cescon Francesco - 1218667
 */

#include <math.h>
#include <iomanip>

#include "Rail.h"
using namespace std;

/**
 * Converts a "min" time to day+hh:mm
 */
string parseTime(int _m) {
  return (
    (_m >= 1440 ? to_string((int) floor(_m/1440))+"+" : "") +
    (floor(_m%1440/60) < 10 ? "0" + to_string((int) floor(_m%1440/60)) : to_string((int) floor(_m%1440/60))) + ":" +
    (floor(_m%60) < 10 ? "0" + to_string((int) floor(_m%60)) : to_string((int) floor(_m%60)))
  );
}
/**
 * [output description]
 * @param l   Importance level of the output
 * @param t   Time
 * @param tr  Train reference
 * @param an  Announcement text
 */
void output(int l, int t, Train* tr, string an) {
  // Select the output level you wish to receive as output
  // 0 is default, 1 is debug, 2 is detailed
  if ( l > 1) return;

  cout << parseTime(t) << " (";

  // Colored output for AV & AV+
  switch (tr->getType()) {
    case 1:
      cout << tr->getID();
      break;
    case 2:
      cout << "\033[;36m" << tr->getID() << "\033[;0m";
      break;
    case 3:
      cout << "\033[;34m" << tr->getID() << "\033[;0m";
      break;

  }

  cout << ") " << an << "\033[;0m\n";
}

/**
 * whosThere returns the trains that exist within a given km range
 */
list<Train*> Rail::whosThere(int _f, int _t) {

  // Always _f < _t
  if (_t < _f) {
    int tmp = _f;
    _f = _t;
    _t = tmp;
  }

  list<Train*> l;
  for (auto i = trainsList.begin(); i != trainsList.end(); ++i) {
    // For every train check if its within the range
    if ((*i)->getExists() && (*i)->getKm() >= _f && (*i)->getKm() <= _t) {
      // If it is, check if it's in a parking area (aka speed 0 and not taking space)
      if ((*i)->getSpeed() == 0 && !((*(line.getRailLinePiece((*i)->getKm()))).getBlock())) continue;

      l.push_back(*i);
    }
  }

  return l;
}

/**
 *
 */
int nextParking(list<RailLine_piece> rl, int k, int d) {

  if (d == 1) {
    for (auto i=rl.begin(); i != rl.end(); ++i)
      if (i->getFrom() >= k && i->getPieceType() == 3)
        return i->getFrom();
  }
  else {
    for (auto i=rl.end(); i != rl.begin(); --i)
      if (i->getFrom() <= k && i->getPieceType() == 3)
        return i->getFrom();
  }

  return -1;

}
/**
 *
 */
Station nextStation(list<Station> sl, int k, int d) {

  list<Station>::iterator c = sl.begin();
	list<Station>::iterator p = sl.begin();
	++c;
  for (; c != sl.end(); ++c, ++p) {
    if (c->getKm() >= k && d == 1) return *c;

    if (p->getKm() <= k && c->getKm() >= k && d == -1) return *p;
	}

  return *p;
}

/**
 * simulate is the function that performs the minute-by-minute calculation
 * of the line and the traffic on it.
 * It works by predicting the position of each train based on it's position
 * at the previous time and adjusting the future one based on the conditions
 * of the line and the trains around it. For this reason it can be described
 * as a post-updating simulation.
 */
void Rail::simulate() {

  const int MIN_TRAIN_SEPARATION = 10;

  // Numbers of trains still to arrive at their destination
  int deltaTrains = trainsList.size();
  // Direcitions for each railway line
  const int dir[6] = {1,-1,1,-1,1,-1};

  // Detect the time of the first departing train and use that as the time
  // for the beginning of the simulation
  int t = *((*(trainsList.begin()))->getTimeTable().begin());
  for (auto i = trainsList.begin(); i != trainsList.end(); ++i) {
    if (*((*i)->getTimeTable().begin()) < t)
      t = *((*i)->getTimeTable().begin());
  }

  cout << "\033[;33mSimulation starting at time : "<<t<<"\033[;0m\n";

  // Main simulation time cycle
  while (deltaTrains > 0 && t < 550) {

    for (auto tr = trainsList.begin(); tr != trainsList.end(); ++tr) {
      Train* thisTrain = *tr;
      /*
        Is the train already at destination? In case, skip
       */
      if (!thisTrain->getExists() && (thisTrain->getKm() == (line.getLineLength() * (thisTrain->getDirection() < 0))))
        continue;

      /*
        Advance the status of trains that already exist on the line and is moving
       */
      if (thisTrain->getExists() && thisTrain->getSpeed() != 0) {

        // Checking if the next MIN_TRAIN_SEPARATION km zone has trains inside
        int nextKm = thisTrain->getKm() + thisTrain->getDirection() * thisTrain->getSpeed() / 60;
        // And where is the next station and parking area in order to adjust the speed
        int nextPKm = nextParking(line.getLine(), thisTrain->getKm(), thisTrain->getDirection());
        Station nextSt = nextStation(stationsList, thisTrain->getKm(), thisTrain->getDirection());

        output(2, t, thisTrain, "proceeding at "+to_string(thisTrain->getSpeed())+"km/h on track " + to_string(thisTrain->getTrack()) + " at " + to_string(thisTrain->getKm()));

        /**
         * COMMUNICATIONS BETWEEN THE TRAIN AND THE STATION
         */

        // At 20km before the next station, a train contacts the station
        // with information about it's stop/pass.
        // The station responds with the state of the platform, the predicted
        // track on which the train will routed through or if it needs to stop
        // at a parking area.
        if (abs(thisTrain->getKm() - nextSt.getKm()) >= 20 && (abs(nextKm - nextSt.getKm()) < 20)) {

          output(1, t, thisTrain, "Contacted station : " + nextSt.getName());

          // Checking which platform or tracks are free to estimate the
          // one the train will take

          // Only counting the tracks that go in the direction of the train
          bool free[6] = {thisTrain->getDirection() == dir[0], thisTrain->getDirection() == dir[1], thisTrain->getDirection() == dir[2], thisTrain->getDirection() == dir[3], nextSt.getType() == 1 && thisTrain->getDirection() == dir[4], nextSt.getType() == 1 && thisTrain->getDirection() == dir[5]};

          // If the train needs to stop it cannot use platform 4 or 5
          if (thisTrain->getType() == 1) {
            free[4] = false;
            free[5] = false;
          }

          // Checking which train are inside the station at the moment
          int freeTrack = -1;
          list<Train*> near = whosThere(nextSt.getKm()-5, nextSt.getKm()+5);
          for (auto i = near.begin(); i != near.end(); ++i)
            if ((*i)->getID() != thisTrain->getID())
              free[(*i)->getTrack()] = false;
          // Finding the first free track
          for (int i = 0; i < 6; ++i) {
            if (free[i]) {
              freeTrack = i;

              // Or the transit tracks if they are free and the train does not
              // need to stop
              if (thisTrain->getType() != 1 && (free[4] || free[5]))
                freeTrack = (free[4] ? 4 : 5);

              break;
            }
          }


          cout << "┌─────────────────────────────────────────┐  ┌─────────────────────────────────────────┐\n";
          cout << "│ " << setw(18) << thisTrain->getID() << " » " << setw(18) << nextSt.getName().substr(0, 18) << " │  │ " << setw(18) << thisTrain->getID() << " « " << setw(18) << nextSt.getName().substr(0, 18) << " │\n";
          cout << "├─────────────────────────────────────────┤  ├─────────────────────────────────────────┤\n";


          cout << "│ Train approaching station.              │  ";
          if (freeTrack == -1)
            cout << "│ The station is currently full, the      │\n";
          else
            cout << "│ The station is currently able to        │\n";

          if (thisTrain->getType() != 1 && nextSt.getType() == 1)
            cout << "│ Planning NOT TO stop.                   │  ";
          else
            cout << "│ Planning TO stop.                       │  ";

          if (freeTrack == -1)
            cout << "│ train will be sent to parking.          │\n";
          else
            cout << "│ accept the train on platform " << freeTrack << ".         │\n";

          cout << "└─────────────────────────────────────────┘  └─────────────────────────────────────────┘\n";

        }

        /**
         * TRIP UPDATES
         */

        /*
          If within this minute the train passed the parking, let's check if the station
          it's approaching if
          - it needs to stop there
          - what platform it may stop/transit at
         */
        if ((nextKm > nextPKm && thisTrain->getDirection() == 1) || (nextKm < nextPKm && thisTrain->getDirection() == -1)) {

          output(1, t, thisTrain, "Parking passed : " + to_string(nextKm) + " <> " + to_string(nextPKm));

          // Only counting the tracks that go in the direction of the train
          bool free[6] = {thisTrain->getDirection() == dir[0], thisTrain->getDirection() == dir[1], thisTrain->getDirection() == dir[2], thisTrain->getDirection() == dir[3], nextSt.getType() == 1 && thisTrain->getDirection() == dir[4], nextSt.getType() == 1 && thisTrain->getDirection() == dir[5]};

          // If the train needs to stop it cannot use platform 4 or 5
          if (thisTrain->getType() == 1) {
            free[4] = false;
            free[5] = false;
          }
          // If the train is leaving a station
          if ((*(line.getRailLinePiece(nextKm))).getPieceType() == 0) {
            free[2] = false;
            free[3] = false;
            free[4] = false;
            free[5] = false;
          }

          // Checking which train are
          list<Train*> near;
          if ((*(line.getRailLinePiece(nextKm))).getPieceType() == 0) {
            near = whosThere(nextPKm, nextPKm+MIN_TRAIN_SEPARATION);
          }
          else {
            near = whosThere(nextSt.getKm()-5, nextSt.getKm()+5);
          }

          // Checking the train that are inside the station at the moment

          for (auto i = near.begin(); i != near.end(); ++i)
            if ((*i)->getID() != thisTrain->getID())
              free[(*i)->getTrack()] = false;

          bool trainRouted = false;
          for (int i = 0; i < 6; ++i) {
            if (free[i]) {

              // If train is AV/AV+ and track 4/5 are free use those instead
              if (thisTrain->getType() != 1 && (free[4] || free[5]))
                i = (free[4] ? 4 : 5);

              thisTrain->setTrack(i);
              thisTrain->setKm(nextKm);
              thisTrain->setSpeed(line.getSpeedLimit(nextKm)[i] == -1 ? thisTrain->getMaxSpeed() : line.getSpeedLimit(nextKm)[i]);

              output(1, t, thisTrain, "routed via track " + to_string(i) + " at km " + to_string(nextKm));

              trainRouted = true;
              break;
            }
          }
          // There are no free platform to be used, stop the train at the
          if (!trainRouted) {
            thisTrain->setKm(nextPKm);
            thisTrain->setSpeed(0);

            output(0, t, thisTrain, "\033[;33mparked at km " + to_string(nextPKm));
          }
        }

        /*
          If the train, over the past minute, passed over the station
          check if it needs to stop there
         */
        else if ((nextKm >= nextSt.getKm() && thisTrain->getDirection() == 1) || (nextKm <= nextSt.getKm() && thisTrain->getDirection() == -1)) {

          output(1, t, thisTrain, "Station passed : " + to_string(nextKm) + " <> " + to_string(nextSt.getKm()));

          // If the train is AV/AV+ and the station is secondary than
          // this staion is skipped
          if (thisTrain->getType() != 1 && nextSt.getType() == 1) {
            thisTrain->setKm(nextKm);
          }
          else {
            // The train is stopping at the station
            thisTrain->setKm(nextSt.getKm());
            thisTrain->setSpeed(0);
            thisTrain->setDelay(0);

            // Calculating the delay between the current time and the timetable
            int plannedArrival = -1;

            auto trainTT = thisTrain->getTimeTable();
            auto tt = trainTT.begin();
            if (thisTrain->getDirection() == 1) {
              for (auto c = stationsList.begin(); c != stationsList.end(); ++c, ++tt) {
                if (c->getKm() == nextSt.getKm()) {
                  plannedArrival = *tt;
                  break;
                }
              }
            } else {
              for (auto c = stationsList.end(); c != stationsList.begin(); --c, ++tt) {
                if (c->getKm() == nextSt.getKm()) {
                  plannedArrival = *tt;
                  break;
                }
              }
            }

            output(0, t, thisTrain, "\033[1;32mstopping at " + nextSt.getName() + (plannedArrival < t ? " |\033[21;31m Delay: "+to_string(t-plannedArrival) : ""));
          }
        }

        /*
          The train is just casully roaming around, check the train around it and, in case, adjust the speed to match
         */
        else {

          // Finding the closest train to the current one or up until the next parking
          // The ternary operator is to distinguish between +1/-1 direction
          list<Train*> near = whosThere(nextPKm, (thisTrain->getDirection() == 1 ?
            min(nextPKm + MIN_TRAIN_SEPARATION * thisTrain->getDirection(), nextPKm) :
            max(nextPKm + MIN_TRAIN_SEPARATION * thisTrain->getDirection(), nextPKm)
          ));
          Train* nextTrain = *near.begin();
          for (auto i = near.begin(); i != near.end(); ++i)
            if ((*i)->getDirection() == thisTrain->getDirection() && abs((*i)->getKm() - thisTrain->getKm()) < abs(nextTrain->getKm() - thisTrain->getKm()) && (*i)->getTrack() == thisTrain->getTrack())
              nextTrain = (*i);

          // Checking if the train is too close to the next train
          // In case pretend the train got as close as possible to the preceding
          // train and matched it's speed if it's moving slower.
          if (nextTrain != nullptr && abs(nextTrain->getKm() - thisTrain->getKm()) < 10) {
            thisTrain->setKm(nextTrain->getKm() - MIN_TRAIN_SEPARATION * thisTrain->getDirection());
            thisTrain->setSpeed(min(thisTrain->getMaxSpeed(), nextTrain->getSpeed()));
          }
          else {
            // Otherwise (the train is far behind) it just continues at its speed as usual.
            thisTrain->setKm(nextKm);
          }

        }

      }

      /*
        Spawn new trains
       */

      if (*(thisTrain->getTimeTable().begin()) <= t && !(thisTrain->getExists())) {

        // Check if there is space at its starting station
        list<Train*> near = whosThere((thisTrain->getDirection() < 0) * line.getLineLength(), (thisTrain->getDirection() < 0) * line.getLineLength());
        // Only counting the tracks that go in the direction of the train
        bool free[4] = {thisTrain->getDirection() == dir[0], thisTrain->getDirection() == dir[1], thisTrain->getDirection() == dir[2], thisTrain->getDirection() == dir[3]};

        // Settings the tracks where trains are as false
        for (auto i = near.begin(); i != near.end(); ++i)
          free[(*i)->getTrack()] = false;

        // If I can find a free track, spawn the train on there
        for (int i = 0; i < 4; ++i) {
          if (free[i]) {
            thisTrain->setTrack(i);
            thisTrain->setKm((thisTrain->getDirection() < 0) * line.getLineLength());
            thisTrain->setSpeed(line.getSpeedLimit(thisTrain->getKm())[i]);
            thisTrain->toggleExists();

            string stationName;

            for (auto j = stationsList.begin(); j != stationsList.end(); ++j)
              if ((*j).getKm() == thisTrain->getKm())
                stationName = (*j).getName().substr(0, (*j).getName().length()-1);

            output(0, t, thisTrain, "\033[;32mdeparting from " + stationName + " on track "+to_string(i) + (*(thisTrain->getTimeTable().begin()) < t ? " |\033[;31m Delay: "+to_string(t-*(thisTrain->getTimeTable().begin())) : ""));
            break;
          }
        }
      }
    }

    t ++;
  }

}
