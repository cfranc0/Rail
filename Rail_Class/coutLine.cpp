/**
 *@author Cescon Francesco - 1218667
 */

#include "Rail.h"
#include <iomanip>
using namespace std;

std::ostream& operator<<(std::ostream& os, const Rail& _rail) {
	list<RailLine_piece> _l = _rail.getLine().getLine();
	list<Station> _s = _rail.getStationsList();
	auto si = _s.begin();

	bool reverseApproach = true;

	for(auto i = _l.begin(); i != _l.end(); ++i)
	{

		if ((*i).getPieceType() != 3)
			os << setw(4) << (*i).getFrom();

		switch ((*i).getPieceType()) {

			case 0:
				os << "   ││  \n";
				for (size_t j = 0; j < ((*i).getTo() - (*i).getFrom() - 10)/10 + (((*i).getTo() - (*i).getFrom() - 10)%10 > 0 ? 1 : 0); j++)
					os << "       ││  \n";
				break;
			case 1:
				if ((*i).getRailSpeedLimit().size() == 4)
					os << " \033[;33m" << (reverseApproach ? " ││││ \n      └┼┼┘ " : " ┌┼┼┐ \n      ││││ ") << "\033[0m\n";
				else
					os << " \033[;33m" << (reverseApproach ? "││││││\n     └┴┼┼┴┘" : "┌┬┼┼┬┐\n     ││││││") << "\033[0m\n";

				reverseApproach = !reverseApproach;
				break;
			case 2:
				if ((*i).getRailSpeedLimit().size() == 4)
					os << " \033[;31m ┝┿┿┥ \033[0m";
				else
					os << " \033[;31m┝┿┿┿┿┥\033[0m";

				os << " " << (*si).getName() << "\n";
				++si;
				break;
		}
	}
	return os;
}
