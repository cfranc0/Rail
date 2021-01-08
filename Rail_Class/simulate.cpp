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
 * @param l   Importance level of the output
 * @param t   Time
 * @param tr  Train reference
 * @param an  Announcement text
 */
void output(int l, int t, Train* tr, string an) {
  // Select the output level you wish to receive as output
  // 0 is default, 1 is debug, 2 is detailed
  if (l > 0) return;

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
      // If it is, check if it's in a parking area (not taking space)
      if (!((*(line.getRailLinePiece((*i)->getKm()))).getBlock())) continue;

      l.push_back(*i);
    }
  }

  return l;
}

/**
 * routeThrough returns the number of the first available track
 * to route a train.
 * The range [_f, _t] is taken as a single piece, meaning that
 * the possibility to switch track down the line is not allowed.
 * Only if a track is fully free between _f and _t, it will be returned.
 */
int Rail::routeThrough(Train* thisTrain, int _f, int _t) {
  // Direcitions for each railway line
  const int dir[6] = {1,-1,1,-1,1,-1};

  // Always _f < _t
  if (_t < _f) {
    int tmp = _f;
    _f = _t;
    _t = tmp;
  }

  // Only counting the tracks that go in the direction of the train
  bool free[6];
  for (int i = 0; i < 6; i++) free[i] = (thisTrain->getDirection() == dir[i] );

  // Finding the number of continuos tracks within _f and _t
  list<RailLine_piece> rl = line.getLine();
  auto rlp = rl.begin();
  while (rlp->getFrom() <= _t && rlp != rl.end()) {
    if (rlp->getTo() <= _f && _t != _f) {++rlp; continue;}
    /*cout << "Checking ["<<rlp->getFrom()<<"."<<rlp->getTo()<<"] ="<<rlp->getRailSpeedLimit().size();*/
    if (rlp->getTo() == rlp->getFrom() && (_t != _f)) {/*cout<<" skipped\n";*/++rlp; continue;};
    if (rlp->getFrom() == _t && rlp->getTo() > _t) {/*cout<<" skipped\n";*/++rlp; continue;};
    /*cout<<"\n";*/

    if (rlp->getRailSpeedLimit().size() == 2) {
      free[2] = false; free[3] = false; free[4] = false; free[5] = false;
      break;
    }
    else if (rlp->getRailSpeedLimit().size() == 4) {
      free[4] = false; free[5] = false;
    }

    ++rlp;
  }

  // If the train needs to stop it cannot use platform 4 or 5
  if (thisTrain->getType() == 1) {
    free[4] = false;
    free[5] = false;
  }
  //cout << _f << "->" << _t<< "   "; for (int i = 0; i < 6; i++) cout << free[i] << "-"; cout << "\n";

  // Checking which train are
  list<Train*> near = whosThere(_f, _t);

  // Checking the train that are inside the station at the moment

  for (auto i = near.begin(); i != near.end(); ++i)
    if ((*i)->getID() != thisTrain->getID())
      free[(*i)->getTrack()] = false;

  for (int i = 0; i < 6; ++i) {
    if (free[i]) {

      // If train is AV/AV+ and track 4/5 are free use those instead
      if (thisTrain->getType() != 1 && (free[4] || free[5])) {
        i = (free[4] ? 4 : 5);
      }

      return i;
    }
  }
  return -1;
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

  constexpr int MIN_TRAIN_SEPARATION = 10; // km
  constexpr int MIN_TRAIN_STOP = 5;        // min

  // Numbers of trains still to arrive at their destination
  int deltaTrains = trainsList.size();

  // Detect the time of the first departing train and use that as the time
  // for the beginning of the simulation
  int t = *((*(trainsList.begin()))->getTimeTable().begin());
  for (auto i = trainsList.begin(); i != trainsList.end(); ++i) {
    if (*((*i)->getTimeTable().begin()) < t)
      t = *((*i)->getTimeTable().begin());
  }

  cout << "\033[;33mSimulation starting at time : "<<t<<"\033[;0m\n";

  // Main simulation time cycle
  while (deltaTrains > 0) {

    for (auto tr = trainsList.begin(); tr != trainsList.end(); ++tr) {
      Train* thisTrain = *tr;

      /*
        Advance the status of trains that already exist on the line and are moving
       */
      if (thisTrain->getExists() && thisTrain->getSpeed() != 0) {

        // Checking if the next MIN_TRAIN_SEPARATION km zone has trains inside
        int nextKm = thisTrain->getKm() + thisTrain->getDirection() * thisTrain->getSpeed() / 60;
        // And where is the next station and parking area in order to adjust the speed
        int nextPKm = line.nextParking(thisTrain);

        Station nextSt = nextStop(thisTrain);

        output(2, t, thisTrain, "proceeding at "+to_string(thisTrain->getSpeed())+"km/h on track " + to_string(thisTrain->getTrack()) + " at km " + to_string(thisTrain->getKm()));

        /**
         * COMMUNICATIONS BETWEEN THE TRAIN AND THE STATION
         */

        // At 20km before the next station, a train contacts the station
        // with information about it's stop/pass.
        // The station responds with the state of the platform, the predicted
        // track on which the train will routed through or if it needs to stop
        // at a parking area.
        if (abs(thisTrain->getKm() - nextSt.getKm()) >= 20 && (abs(nextKm - nextSt.getKm()) < 20) && true) {

          output(1, t, thisTrain, "Contacted station : " + nextSt.getName());

          // Checking which platform or tracks are free to estimate the
          // one the train will take
          int freeTrack = routeThrough(thisTrain, nextSt.getKm()-5, nextSt.getKm()+5);

          cout << "┌─────────────────────────────────────────┐  ┌─────────────────────────────────────────┐\n";
          cout << "│ " << setw(4) << thisTrain->getID() << " » " << setw(32) << nextSt.getName().substr(0, 18) << " │  │ " << setw(4) << thisTrain->getID() << " « " << setw(32) << nextSt.getName().substr(0, 32) << " │\n";
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
          If the train, over the past minute, passed over the station
          check if it needs to stop there
         */
        if ((nextKm >= nextSt.getKm() && thisTrain->getDirection() == 1) || (nextKm <= nextSt.getKm() && thisTrain->getDirection() == -1)) {

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

            // Calculating the delay between the current time and the timetable
            int plannedArrival = -1;
            auto trainTT = thisTrain->getTimeTable();
            auto tt = trainTT.begin();
            if (thisTrain->getDirection() == 1) {
              /*for (auto ttt = trainTT.begin(); ttt != trainTT.end(); ++ttt) cout << *ttt << ", "; cout << "\n";
              for (auto ttt = stationsList.begin(); ttt != stationsList.end(); ++ttt) cout << ttt->getKm() << ", "; cout << "\n";*/

              for (auto c = stationsList.begin(); c != stationsList.end(); ++c, ++tt) {
                if (c->getKm() == nextSt.getKm()) {
                  plannedArrival = *tt;
                  break;
                }
              }
            } else {
              /*for (auto ttt = trainTT.begin(); ttt != trainTT.end(); ++ttt) cout << *ttt << ", "; cout << "\n";
              for (auto ttt = stationsList.rbegin(); ttt != stationsList.rend(); ++ttt) cout << ttt->getKm() << ", "; cout << "\n";*/

              for (auto c = stationsList.rbegin(); c != stationsList.rend(); ++c, ++tt) {
                if (c->getKm() == nextSt.getKm()) {
                  plannedArrival = *tt;
                  break;
                }
              }
            }

            thisTrain->setDelay(t-plannedArrival);

            output(0, t, thisTrain, "\033[;35mstopping at " + nextSt.getName() + " on platform " + to_string(thisTrain->getTrack()) + (plannedArrival < t ? " |\033[21;31m Delay: "+to_string(t-plannedArrival) : ""));
          }

          // Check if this is the destination for this train, in case remove
          // it from the trains still to arrive and change exists
          if (nextSt.getKm() == 0 || nextSt.getKm() == line.getLineLength()) {

            --deltaTrains;
            thisTrain->toggleExists();

            output(0, t, thisTrain, "\033[1;33mreached its destination " + nextSt.getName() + (thisTrain->getDelay() > 0 ? " with a delay of " + to_string(thisTrain->getDelay()) + " minutes." : ""));
          }
        }

        /*
          If within this minute the train passed the parking, let's check if the station
          it's approaching if
          - it needs to stop there
          - what platform it may stop/transit at
         */
        else if ((((nextKm > nextPKm && thisTrain->getDirection() == 1) || (nextKm < nextPKm && thisTrain->getDirection() == -1))) && !(nextPKm == 0 || nextPKm == line.getLineLength())) {

          output(1, t, thisTrain, "Parking passed : " + to_string(nextKm) + " <> " + to_string(nextPKm));

          // Check if there is a free track to route the train
          // Based if we are approaching or leaving a station, the
          // checked range changes.
          int route;
          if ((*(line.getRailLinePiece(nextKm))).getPieceType() == 0) {
            route = routeThrough(thisTrain, nextPKm, nextPKm+MIN_TRAIN_SEPARATION*thisTrain->getDirection());
          }
          else {
            route = routeThrough(thisTrain, nextSt.getKm()-5, nextSt.getKm());
          }

          if (route != -1) {
            thisTrain->setTrack(route);
            thisTrain->setKm(nextKm);
            thisTrain->setSpeed(line.getSpeedLimit(nextKm)[route]);

            output(1, t, thisTrain, "routed via track " + to_string(route) + " at km " + to_string(nextKm));
          } else {
            thisTrain->setKm(nextPKm);
            thisTrain->setSpeed(0);

            output(0, t, thisTrain, "\033[;33mparked at km " + to_string(nextPKm));
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
        Handle trains that are currently stopped at a station or in a parking area
       */
      else if (thisTrain->getExists() && thisTrain->getSpeed() == 0) {

        // Train is at a parking area check if there is space in the next track
        // section
        if (line.getRailLinePiece(thisTrain->getKm())->getPieceType() == 3) {

          int freeTrack = routeThrough(thisTrain, thisTrain->getKm(), thisTrain->getKm()+MIN_TRAIN_SEPARATION*thisTrain->getDirection());
          if (freeTrack == -1) continue;

          // Track found, route the train there
          thisTrain->setTrack(freeTrack);
          thisTrain->setSpeed(line.getSpeedLimit(thisTrain->getKm())[freeTrack]);

          output(0, t, thisTrain, "\033[7;33mleft parking at km" + to_string(thisTrain->getKm()));

        }

        // Train is at a station
        else if (line.getRailLinePiece(thisTrain->getKm())->getPieceType() == 2) {

          // Check if 5 minutes have passed
          // Since there is no condition on the departure time of a train, I will
          // make train depart just after the MIN_TRAIN_STOP value, and put them
          // in parking in case the line is full. This is done to allow other
          // trains to use the platform.
          int plannedArrival = -1;

          auto trainTT = thisTrain->getTimeTable();
          //for (auto ttt = trainTT.begin(); ttt != trainTT.end(); ++ttt) cout << *ttt << ", "; cout << "\n";
          auto tt = trainTT.begin();
          string stationName;
          if (thisTrain->getDirection() == 1) {
            //for (auto ttt = stationsList.begin(); ttt != stationsList.end(); ++ttt) cout << ttt->getKm() << ", "; cout << "\n";
            for (auto c = stationsList.begin(); c != stationsList.end(); ++c, ++tt) {
              if (c->getKm() == thisTrain->getKm()) {
                plannedArrival = *tt;
                stationName = c->getName();
                break;
              }
            }
          } else {
            //for (auto ttt = stationsList.rbegin(); ttt != stationsList.rend(); ++ttt) cout << ttt->getKm() << ", "; cout << "\n";
            for (auto c = stationsList.rbegin(); c != stationsList.rend(); ++c, ++tt) {
              if (c->getKm() == thisTrain->getKm()) {
                plannedArrival = *tt;
                stationName = c->getName();
                break;
              }
            }
          }

          // The train has been at the platform less than MIN_TRAIN_STOP minutes
          if (t < plannedArrival + thisTrain->getDelay() + MIN_TRAIN_STOP) continue;

          // There is no need to check if the track is free as a train leaving a station
          // stayed enough time to le the previous one clear the line
          thisTrain->setSpeed(line.getSpeedLimit(thisTrain->getKm())[thisTrain->getTrack()]);

          output(0, t, thisTrain, "\033[1;32mleaving "+stationName+" station after "+to_string(t-(plannedArrival + thisTrain->getDelay()))+" minutes");

        }

      }

      /*
        Spawn new trains
       */

      if (*(thisTrain->getTimeTable().begin()) <= t && !(thisTrain->getExists()) && thisTrain->getKm() == -1) {

        // Check if there is space at its starting station
        int freePlatform = routeThrough(thisTrain, (thisTrain->getDirection() < 0) * line.getLineLength(), (thisTrain->getDirection() < 0) * line.getLineLength());

        if (freePlatform != -1) {
          thisTrain->setTrack(freePlatform);
          thisTrain->setKm((thisTrain->getDirection() < 0) * line.getLineLength());
          thisTrain->setSpeed(line.getSpeedLimit(thisTrain->getKm())[freePlatform]);
          thisTrain->toggleExists();

          string stationName;

          for (auto j = stationsList.begin(); j != stationsList.end(); ++j)
            if ((*j).getKm() == thisTrain->getKm())
              stationName = (*j).getName().substr(0, (*j).getName().length()-1);

          output(0, t, thisTrain, "\033[;32mdeparting from " + stationName + " on platform "+to_string(freePlatform) + (*(thisTrain->getTimeTable().begin()) < t ? " |\033[;31m Delay: "+to_string(t-*(thisTrain->getTimeTable().begin())) : ""));
          break;
        }
      }
    }

    t ++;
  }

}
