//LEONARDO PINTON

#include "RailLine.h"

using namespace std;

RailLine_pice_standard::RailLine_pice_standard(int _from, int _to) {
	from = _from;
	to = _to;
	doesBlockTraffic = true;
	currentTrains = {};
	railSpeedLimit = { -1, -1 };
}

RailLine_pice_approach::RailLine_pice_approach(int _from, int _to, int type) {
	from = _from;
	to = _to;
	doesBlockTraffic = true;
	currentTrains = {};
	if(type == 0)
		railSpeedLimit = { 80, 80, 80, 80 };
	else
		railSpeedLimit = { 80, 80, 80, 80, -1, -1};
}

RailLine_pice_station::RailLine_pice_station(int _from, int _to, int type) {
	from = _from;
	to = _to;
	doesBlockTraffic = true;
	currentTrains = {};
	if (type == 0)
		railSpeedLimit = { 80, 80, 80, 80 };
	else
		railSpeedLimit = { 80, 80, 80, 80, -1, -1 };
}

RailLine_pice_parking::RailLine_pice_parking(int _from, int _to, int type) {
	from = _from;
	to = _to;
	doesBlockTraffic = false;
	currentTrains = {};
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


void RailLine::generateLine(list<int> stations) {
	
}

void RailLine::whosThere(int km_from, int km_to) {

}

const list<RailLine_pice> RailLine::getLine() const {

	return line;
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
