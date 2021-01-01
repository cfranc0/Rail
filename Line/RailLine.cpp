//LEONARDO PINTON

#include "RailLine.h"
#include "Station.h"
#include "Train.h"

using namespace std;

RailLine_pice_standard::RailLine_pice_standard(int _from, int _to) {
	from = _from;
	to = _to;
	doesBlockTraffic = true;
	piceType = 0;
	railSpeedLimit = { -1, -1 };
}

RailLine_pice_approach::RailLine_pice_approach(int _from, int _to, int type) {
	from = _from;
	to = _to;
	doesBlockTraffic = true;
	piceType = 1;
	if(type == 0)
		railSpeedLimit = { 80, 80, 80, 80 };
	else
		railSpeedLimit = { 80, 80, 80, 80, -1, -1};
}

RailLine_pice_station::RailLine_pice_station(int _from, int _to, int type) {
	from = _from;
	to = _to;
	doesBlockTraffic = true;
	piceType = 2;
	if (type == 0)
		railSpeedLimit = { 80, 80, 80, 80 };
	else
		railSpeedLimit = { 80, 80, 80, 80, -1, -1 };
}

RailLine_pice_parking::RailLine_pice_parking(int _from, int _to, int type) {
	from = _from;
	to = _to;
	doesBlockTraffic = false;
	piceType = 3;
	if (type == 0)
		railSpeedLimit = { 80, 80, 80, 80 };
	else
		railSpeedLimit = { 80, 80, 80, 80, -1, -1 };
}

int RailLine_pice::getFrom() const{
	return from;
}

int RailLine_pice::getTo() const{
	return to;
}

int RailLine_pice::getPiceType() const{
	return piceType;
}

const vector<int> RailLine_pice::getRailSpeedLimit() const{
	return railSpeedLimit;
}

const std::list<Train*> RailLine_pice::getCurrentTrains() const{
	return currentTrains;
}

const list<RailLine_pice> RailLine::getLine() const {

	return line;
}

void RailLine::generateLine(list<Station> stations) {
	list<Station>::iterator r = stations.begin();
	list<Station>::iterator prev;
	++r;
	for (r; r != stations.end(); ++r){
		prev = --r;
		line.push_back(RailLine_pice_approach(prev->getKm(), prev->getKm() + 5, prev->getType()));
		line.push_back(RailLine_pice_parking(prev->getKm() + 5, prev->getKm() + 5, prev->getType()));
		line.push_back(RailLine_pice_standard(prev->getKm() + 5, prev->getKm() + 5 + (r->getKm() - 5)));
		line.push_back(RailLine_pice_parking(r->getKm() - 5, r->getKm() - 5, r->getType()));
		line.push_back(RailLine_pice_approach(r->getKm() - 5, r->getKm(), r->getType()));
		line.push_back(RailLine_pice_station(r->getKm(), r->getKm(), r->getType()));
	}
}

list<Train*> RailLine::whosThere(int km_from, int km_to) {
	list<Train*> t;
	return t;
}

const vector<int> RailLine::getSpeedLimit(int km){
	vector<int> speedLimit;
	list<RailLine_pice>::iterator t = line.begin();
	for (t;  t != line.end(); ++t){
		if(t->getFrom() == km || t->getTo() == km){
			speedLimit = t->getRailSpeedLimit();
			break;
		}
		if(t->getPiceType() == 1){
			for (int i = 1; i < 6; ++i) {
				if(t->getFrom() + i == km){
					speedLimit = t->getRailSpeedLimit();
					break;
				}
			}
		}
		else
			speedLimit = t->getRailSpeedLimit();
	}
	return speedLimit;
}

ostream& operator<<(ostream& os, const RailLine& _line) {
	list<RailLine_pice> _l = _line.getLine();
	list<RailLine_pice>::iterator lineIt = _l.begin();
	for(lineIt; lineIt != _l.end(); ++lineIt)
	{
		os << lineIt->getFrom() << " " << lineIt->getTo();
	}
	return os;
}
