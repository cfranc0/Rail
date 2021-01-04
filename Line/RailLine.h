/**
 *@author Leonardo Pinton, 1230955
 *@code RailLine.h
 */

#ifndef RAILLINE_H
#define RAILLINE_H

#include <iostream>
#include <vector>
#include <list>

#include "Station.h"
#include "Train.h"

/* Base class */
class RailLine_piece {

public:

	/**
	* getFrom
	* @return the initial point of a piece of line
	*/
	int getFrom() const;

	/**
	* getTo
	* @return the ending point of a piece of line
	*/
	int getTo() const;

	/**
	* getPieceType
	* @return the type of a piece of line
	*/
	int getPieceType() const;

	/**
	* getBlock
	* @return if a train on a piece of line would block over trains
	*/
	bool getBlock() const;

	/**
	* getRailSpeedLimit
	* @return the vector with the speed limit applied on that piece of line
	*/
	const std::vector<int> getRailSpeedLimit() const;

	/**
	* getCurrentTrains
	* @return the list with the current trains on a piece of line
	*/
	const std::list<Train*> getCurrentTrains() const;

protected:
	RailLine_piece() {};

	std::list<Train*> currentTrains; // List of current trains on a piece of line
	std::vector<int> railSpeedLimit; // Speed limit for the tracks
	int from = 0; // Initial point of a piece of line
	int to = 0; // Ending point of a piece of line
	int pieceType = 0; // Type of a piece of line
	bool doesBlockTraffic = false; // Check if a train on the line will take up space
};

/* Class RailLine */
class RailLine {

public:

	/**
	* generateLine
	* @param list<Station> : list of stations
	* Create a new line
	*/
	void generateLine(std::list<Station> stations);

	/**
	* whosThere
	* @param km_from : first km of the range
	* @param km_to : Last km of the range
	* @return a list of train that are between the specified Kms
	*/
	std::list<Train*> whosThere(int km_from, int km_to);

	/**
	* getRailLinePiece
	* @param km : Specified km
	* @return the iterator that points to the piece of line that contains the specified km
	*/
	std::list<RailLine_piece>::iterator getRailLinePiece(int km);

	/**
	* getSpeedLimit
	* @param km : Specified km
	* @return the vector with the speed limit for the piece of line that contains the specified km
	*/
	const std::vector<int> getSpeedLimit(int km);

	/**
	* getLine
	* @return the line
	*/
	const std::list<RailLine_piece> getLine() const;


	/**
	* getLineSize
	* @return the size of the line
	*/
	int getLineSize() const;

private:
	std::list<RailLine_piece> line; // Contains all the pieces that form a line
	int lineSize;
};

/* Derived classes */
class RailLine_piece_standard : public RailLine_piece {
public:

	/**
	* RailLine_piece_standard constructor
	* @param from : Fist km of the standard piece
	* @param to : Last km of the standard piece
	* Create a new standard piece of line
	*/
	RailLine_piece_standard(int from, int to);
};

class RailLine_piece_approach : public RailLine_piece {
public:

	/**
	* RailLine_piece_approach constructor
	* @param from : Fist km of the standard piece
	* @param to : Last km of the standard piece
	* @param type : Type of station, useful to define the speed limit of the piece of line
	* Create a new approach piece of line
	*/
	RailLine_piece_approach(int from, int to, int type);
};

class RailLine_piece_station : public RailLine_piece {
public:

	/**
	* RailLine_piece_station constructor
	* @param from : Fist km of the standard piece
	* @param to : Last km of the standard piece
	* @param type : Type of station, useful to define the speed limit of the piece of line
	* Create a new station piece of line
	*/
	RailLine_piece_station(int from, int to, int type);
};

class RailLine_piece_parking : public RailLine_piece {
public:

	/**
	* RailLine_piece_station constructor
	* @param from : Fist km of the standard piece
	* @param to : Last km of the standard piece
	* @param type : Type of station, useful to define the speed limit of the piece of line
	* Create a new station piece of line
	*/
	RailLine_piece_parking(int from, int to, int type);
};


/**
* Overloading of the operator<<
* Print the line created with generateLine
*/
std::ostream& operator<<(std::ostream& os, const RailLine& _line);
#endif //!RAILLINE_H
