#include "TravelerList.h"

// Return mileage across all regions for a specified system
double TravelerList::system_region_miles(HighwaySystem *h)
{	double mi = 0;
	for (std::pair<Region* const, double>& rm : system_region_mileages.at(h)) mi += rm.second;
	return mi;
}//*/

std::mutex TravelerList::mtx;
std::list<std::string> TravelerList::ids;
std::list<TravelerList*> TravelerList::allusers;
