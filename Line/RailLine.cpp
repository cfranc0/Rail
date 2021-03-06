/**
 *@author Leonardo Pinton, 1230955
 *@code RailLine.cpp
 */

#include "RailLine.h"

using namespace std;

/* Constructors of the derived classes */
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

/* Definition of RailLine_piece member functions */
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


/* Definition of RailLine member functions*/
void RailLine::generateLine(list<Station> stations) {
	list<Station>::iterator currS = stations.begin();
	list<Station>::iterator prevS = stations.begin();
	++currS;
	for (; currS != stations.end(); ++currS, ++prevS){
		if(prevS == stations.begin())
			line.push_back(RailLine_piece_station(prevS->getKm(), prevS->getKm(), prevS->getType()));
		line.push_back(RailLine_piece_approach(prevS->getKm(), prevS->getKm() + 5, prevS->getType()));
		line.push_back(RailLine_piece_parking(prevS->getKm() + 5, prevS->getKm() + 5, prevS->getType()));
		line.push_back(RailLine_piece_standard(prevS->getKm() + 5, currS->getKm() - 5));
		line.push_back(RailLine_piece_parking(currS->getKm() - 5, currS->getKm() - 5, currS->getType()));
		line.push_back(RailLine_piece_approach(currS->getKm() - 5, currS->getKm(), currS->getType()));
		line.push_back(RailLine_piece_station(currS->getKm(), currS->getKm(), currS->getType()));
		lineLength = currS->getKm();
	}
}

int RailLine::nextParking(Train* t) {
	list<RailLine_piece>::iterator l = getRailLinePiece(t->getKm());
	list<RailLine_piece>::iterator l1 = l;
	list<RailLine_piece>::iterator l2 = l;
	if (t->getDirection() == 1) {

		switch (l->getPieceType()) {
		case 0:
			advance(l, 1);
			return l->getFrom();
			break;

		case 1:
			if ((++l1)->getPieceType() == 3) {
				advance(l, 1);
				return l->getFrom();
			}
			else {
				advance(l, 3);
				return l->getFrom();
			}
			break;

		case 2:
			advance(l, 2);
			return l->getFrom();
			break;

		case 3:
			return l->getFrom();
			break;
		}
	}
	else {

		switch (l->getPieceType()) {
		case 0:
			advance(l, -1);
			return l->getFrom();
			break;

		case 1:
			if ((++l1)->getPieceType() == 3) {
				--l1;
				advance(l, -3);
				return l->getFrom();
			}
			else {
				advance(l, -1);
				return l->getFrom();
			}
			break;

		case 2:
			advance(l, -2);
			return l->getFrom();
			break;

		case 3:
			return l->getFrom();
			break;
		}
	}

	return 0;
}

list<RailLine_piece>::iterator RailLine::getRailLinePiece(int km) {

	list<RailLine_piece>::iterator l = line.begin();
	list<RailLine_piece>::iterator pS;
	for (;  l != line.end(); ++l){
		pS = l;
		if (l->getFrom() <= km && l->getTo() >= km) {
			if ((++pS)->getFrom() == km && pS->getTo()) // check if the next RailLine_piece is a station or a parking
				return pS;
			break;
		}
	}
	return l;
}

const std::vector<int> RailLine::getSpeedLimit(int km) {

	list<RailLine_piece>::iterator l = getRailLinePiece(km);
	return l->getRailSpeedLimit();
}

int RailLine::getLineLength() const{

	return lineLength;
}

/* Definition of the overloading of the operator<< */
ostream& operator<<(ostream& os, const RailLine& _line) {
	list<RailLine_piece> _l = _line.getLine();
	list<RailLine_piece>::iterator lineIt = _l.begin();
	for(; lineIt != _l.end(); ++lineIt)
	{
		os << lineIt->getFrom() << " " << lineIt->getTo();
	}
	return os;
}
