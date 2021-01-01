//LEONARDO PINTON

#pragma once
#include <iostream>
#include <vector>
#include <list>

#include "Station.h"
#include "Train.h"

class RailLine_pice {

public:
	int getFrom() const; // return the initial point of a pice of line
	int getTo() const; // return the ending point of a pice of line
	int getPiceType() const; // return the type of the pice of the line
	const std::vector<int> getRailSpeedLimit() const; // Return the vector with the speed limit of that pice of line
	const std::list<Train*> getCurrentTrains() const; // Return the list with the current trains on a pice of line

protected:
	RailLine_pice() {}; // The access to the default constructor is blocked

	std::list<Train*> currentTrains; // list of current trains in a pice of line
	std::vector<int> railSpeedLimit; // speed limit for the tracks
	int from = 0; // Initial point of the line
	int to = 0; // Ending point of the line
	int piceType = 0;
	bool doesBlockTraffic = false; // check if a train on the line will take up space

};

class RailLine {

public:
	void generateLine(std::list<Station> stations); // Generate a line
	std::list<Train*> whosThere(int km_from, int km_to); // Return a list of train that are between the specified Kms
	const std::vector<int> getSpeedLimit(int km);
	const std::list<RailLine_pice> getLine() const; // Return the line

private:
	std::list<RailLine_pice> line; // Contains all the pices that form a line
};

class RailLine_pice_standard : public RailLine_pice { //Create a standard pice of line
public:
	RailLine_pice_standard(int from, int to);
};

class RailLine_pice_approach : public RailLine_pice { //Create an approach pice of line
public:
	RailLine_pice_approach(int from, int to, int type);
};

class RailLine_pice_station : public RailLine_pice { //Create a station pice of line
public:
	RailLine_pice_station(int from, int to, int type);
};

class RailLine_pice_parking : public RailLine_pice { //Create a parking pice of line
public:
	RailLine_pice_parking(int from, int to, int type);
};

std::ostream& operator<<(std::ostream& os, const RailLine& _line); // print the line
