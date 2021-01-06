#include <iostream>

#include <list>

#include "Rail_Class/Rail.h"
#include "Station/Station.h"

using std::cout;
using std::list;

int main() {

  Rail r;
  r.loadLine("line_description_1.txt");
  r.loadTimeTable("timetables_2.txt");
  r.simulate();

  /*list<Station> l = r.getStationsList();
  for (auto i = l.begin(); i != l.end(); ++i) {
    cout << *i << "\n";
  }*/

  cout << "End.\n";

  return 0;
}
