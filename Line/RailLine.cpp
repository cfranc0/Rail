//LEONARDO PINTON

#include "RailLine.h"

using namespace std;

RailLine_piece_standard::RailLine_piece_standard(int _from, int _to) {
	from = _from;
	to = _to;
	pieceType = 0;
	doesBlockTraffic = true;
	railSpeedLimit = { -1, -1 };
}

RailLine_piece_approach::RailLine_piece_approach(int _from, int _to, int type) {
	from = _from;
	to = _to;
	pieceType = 1;
	doesBlockTraffic = true;
	if(type == 0)
		railSpeedLimit = { 80, 80, 80, 80 };
	else
		railSpeedLimit = { 80, 80, 80, 80, -1, -1};
}

RailLine_piece_station::RailLine_piece_station(int _from, int _to, int type) {
	from = _from;
	to = _to;
	pieceType = 2;
	doesBlockTraffic = true;
	if (type == 0)
		railSpeedLimit = { 80, 80, 80, 80 };
	else
		railSpeedLimit = { 80, 80, 80, 80, -1, -1 };
}

RailLine_piece_parking::RailLine_piece_parking(int _from, int _to, int type) {
	from = _from;
	to = _to;
	pieceType = 3;
	if (type == 0)
		railSpeedLimit = { 80, 80, 80, 80 };
	else
		railSpeedLimit = { 80, 80, 80, 80, -1, -1 };
}

int RailLine_piece::getFrom() const{
	return from;
}

int RailLine_piece::getTo() const{
	return to;
}

int RailLine_piece::getPieceType() const{
	return pieceType;
}

bool RailLine_piece::getBlock() const{
	return doesBlockTraffic;
}

const vector<int> RailLine_piece::getRailSpeedLimit() const{
	return railSpeedLimit;
}

const std::list<Train*> RailLine_piece::getCurrentTrains() const{
	return currentTrains;
}

const list<RailLine_piece> RailLine::getLine() const {

	return line;
}

void RailLine::generateLine(list<Station> stations) {
	list<Station>::iterator r = stations.begin();
	list<Station>::iterator prev = stations.begin();
	++r;
	for (r; r != stations.end(); ++r, ++prev){
		line.push_back(RailLine_piece_approach(prev->getKm(), prev->getKm() + 5, prev->getType()));
		line.push_back(RailLine_piece_parking(prev->getKm() + 5, prev->getKm() + 5, prev->getType()));
		line.push_back(RailLine_piece_standard(prev->getKm() + 5, r->getKm() - 5));
		line.push_back(RailLine_piece_parking(r->getKm() - 5, r->getKm() - 5, r->getType()));
		line.push_back(RailLine_piece_approach(r->getKm() - 5, r->getKm(), r->getType()));
		line.push_back(RailLine_piece_station(r->getKm(), r->getKm(), r->getType()));
	}
}

list<Train*> RailLine::whosThere(int km_from, int km_to) {
	list<Train*> trains;
	list<RailLine_piece>::iterator l = line.begin();
	for (l; l != line.end(); ++l) {
		if(l->getBlock() == true && l->getTo() >= km_from && l->getFrom() <= km_to){
				list<Train*> trainsTemp = l->getCurrentTrains();
				list<Train*>::iterator t = trainsTemp.begin();
				for (t; t != trainsTemp.end(); ++t)
					trains.push_back(*t);
			}
			else{
				list<Train*> trainsTemp = l->getCurrentTrains();
				list<Train*>::iterator t = trainsTemp.begin();
				for (t; t != trainsTemp.end(); ++t)
					trains.push_back(*t);
				break;
		}
	}
	return trains;
}

list<RailLine_piece>::iterator RailLine::getRailLinePiece(int km){

	list<RailLine_piece>::iterator t = line.begin();
	for (t;  t != line.end(); ++t){
		if(t->getFrom() <= km && t->getTo() >= km)
			break;
	}
	return t;
}

const std::vector<int> RailLine::getSpeedLimit(int km){

	list<RailLine_piece>::iterator t = getRailLinePiece(km);
	return t->getRailSpeedLimit();
}

ostream& operator<<(ostream& os, const RailLine& _line) {
	list<RailLine_piece> _l = _line.getLine();
	list<RailLine_piece>::iterator lineIt = _l.begin();
	for(lineIt; lineIt != _l.end(); ++lineIt)
	{
		os << lineIt->getFrom() << " " << lineIt->getTo();
	}
	return os;
}
