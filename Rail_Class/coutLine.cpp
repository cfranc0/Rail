/**
 *@author Cescon Francesco - 1218667
 */

#include "Rail.h"
#include <iomanip>
using namespace std;

/**
 * Rail: operator<<
 * Overloading the operator << for istances of class Rail
 *
 * This function will ouput to the provided ostream a graphical
 * rapresentation of the line.
 */
std::ostream& operator<<(std::ostream& os, const Rail& _rail) {
	list<RailLine_piece> _l = _rail.getLine().getLine();
	list<Station> _s = _rail.getStationsList();
	auto si = _s.begin();

	bool reverseApproach = true;

	// Looping every line piece
	for(auto i = _l.begin(); i != _l.end(); ++i)
	{

		// Parkings are not displayed
		if ((*i).getPieceType() != 3)
			os << setw(4) << (*i).getFrom();

		switch ((*i).getPieceType()) {

			// Just a regular piece of standard line
			// The length of this piece is dynamic
			case 0:
				os << "   ││  \n";
				for (size_t j = 0; j < ((*i).getTo() - (*i).getFrom() - 10)/10 + (((*i).getTo() - (*i).getFrom() - 10)%10 > 0 ? 1 : 0); j++)
					os << "       ││  \n";
				break;

			// Approach section (or departure)
			case 1:
				if ((*i).getRailSpeedLimit().size() == 4)
					os << " \033[;33m" << (reverseApproach ? " ┆┆┆┆ \n      └┼┼┘ " : " ┌┼┼┐ \n      ┆┆┆┆ ") << "\033[0m\n";
				else
					os << " \033[;33m" << (reverseApproach ? "┆┆┆┆┆┆\n     └┴┼┼┴┘" : "┌┬┼┼┬┐\n     ┆┆┆┆┆┆") << "\033[0m\n";

				reverseApproach = !reverseApproach;
				break;

			// Station: also display the name of the station
			case 2:
				if ((*i).getRailSpeedLimit().size() == 4)
					os << " \033[;31m ┝┿┿┥ \033[0m";
				else
					os << " \033[;31m┝┿┿┥┊┊\033[0m";

				os << " ⬦ " << (*si).getName() << "\n";
				++si;
				break;
		}
	}
	return os;
}
