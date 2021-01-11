/**
 *@author Leonardo Pinton, 1230955
 *@code RailLine.h
 */

#ifndef RAILLINE_H
#define RAILLINE_H

#include <iostream>
#include <vector>
#include <list>
#include <iterator>

#include "../Station/Station.h"
#include "../Train/Train.h"

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
	* I didn't make the copy constructor of RailLine
	* because I think that every line is unique
	*/

	/**
	* generateLine
	* @param list<Station> : list of stations
	* Create a new line
	*/
	void generateLine(std::list<Station> stations);

	/**
	* nextParking
	* @param t : Train from where I want to find the next parking area
	* @return the initial km of the next parking area
	*/
	int nextParking(Train* t);

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
	int getLineLength() const;

private:
	std::list<RailLine_piece> line; // Contains all the pieces that form a line
	int lineLength; // Length of the line
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

	/**
	* Block the possibility of making a copy of a RailLine_piece_standard();
	*/
	RailLine_piece_standard(const RailLine_piece_standard& l) = delete;
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

	/**
	* Block the possibility of making a copy of a RailLine_piece_approach();
	*/
	RailLine_piece_approach(const RailLine_piece_approach& l) = delete;
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

	/**
	* Block the possibility of making a copy of a RailLine_piece_station();
	*/
	RailLine_piece_station(const RailLine_piece_station& l) = delete;
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

	/**
	* Block the possibility of making a copy of a RailLine_piece_parking();
	*/
	RailLine_piece_parking(const RailLine_piece_parking& l) = delete;
};


/**
* Overloading of the operator<<
* Print the line created with generateLine
*/
std::ostream& operator<<(std::ostream& os, const RailLine& _line);
#endif //!RAILLINE_H
