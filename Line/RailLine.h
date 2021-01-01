//LEONARDO PINTON

#pragma once
#include <iostream>
#include <vector>
#include <list>

#include "Station.h"
#include "Train.h"

class RailLine_piece {

public:
	int getFrom() const; // return the initial point of a pice of line
	int getTo() const; // return the ending point of a pice of line
	int getPieceType() const; // return the type of the piece of the line
	const std::vector<int> getRailSpeedLimit() const; // Return the vector with the speed limit of that pice of line
	const std::list<Train*> getCurrentTrains() const; // Return the list with the current trains on a pice of line

protected:
	RailLine_piece() {}; // The access to the default constructor is blocked

	std::list<Train*> currentTrains; // list of current trains in a pice of line
	std::vector<int> railSpeedLimit; // speed limit for the tracks
	int from = 0; // Initial point of the line
	int to = 0; // Ending point of the line
	int pieceType = 0;
	bool doesBlockTraffic = false; // check if a train on the line will take up space
};

class RailLine {

public:
	void generateLine(std::list<Station> stations); // Generate a line
	std::list<Train*> whosThere(int km_from, int km_to); // Return a list of train that are between the specified Kms
	std::list<RailLine_piece>::iterator getRailLinePiece(int km);
	const std::vector<int> getSpeedLimit(int km);
	const std::list<RailLine_piece> getLine() const; // Return the line

private:
	std::list<RailLine_piece> line; // Contains all the pices that form a line
};

class RailLine_piece_standard : public RailLine_piece { //Create a standard pice of line
public:
	RailLine_piece_standard(int from, int to);
};

class RailLine_piece_approach : public RailLine_piece { //Create an approach pice of line
public:
	RailLine_piece_approach(int from, int to, int type);
};

class RailLine_piece_station : public RailLine_piece { //Create a station pice of line
public:
	RailLine_piece_station(int from, int to, int type);
};

class RailLine_piece_parking : public RailLine_piece { //Create a parking pice of line
public:
	RailLine_piece_parking(int from, int to, int type);
};

std::ostream& operator<<(std::ostream& os, const RailLine& _line); // print the line
