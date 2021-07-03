#define debug
#include "Waypoint.h"
#include "../HighwaySystem/HighwaySystem.h"
#include "../Route/Route.h"
#include <cmath>
#include <cstring>

#ifdef debug
//#include <iostream>
#endif

#define pi 3.141592653589793238

Waypoint::Waypoint(char *line, Route *rte)
{	/* initialize object from a .wpt file line */
	route = rte;

	// parse WPT line
	size_t spn = 0;
	for (char* c = line; *c; c += spn)
	{	for (spn = strcspn(c, " "); c[spn] == ' '; spn++) c[spn] = 0;
		alt_labels.emplace_back(c);
	}

	// We know alt_labels will have at least one element, because if the WPT line is
	// blank or contains only spaces, Route::read_wpt will not call this constructor.
	std::string URL = alt_labels.back();	// last token is actually the URL...
	alt_labels.pop_back();			// ...and not a label.
	if (alt_labels.empty()) label = "NULL";
	else {	label = alt_labels.front();	// first token is the primary label...
		alt_labels.pop_front();		// ...and not an alternate.
	     }
	is_hidden = label[0] == '+';
	colocated = 0;

	// parse URL
	size_t latBeg = URL.find("lat=")+4;
	size_t lonBeg = URL.find("lon=")+4;
	if (latBeg == 3 || lonBeg == 3)
	{	lat = 0;	lng = 0;	return;
	}
	lat = strtod(&URL[latBeg], 0);
	lng = strtod(&URL[lonBeg], 0);
}

bool Waypoint::same_coords(Waypoint *other)
{	// return if this waypoint is colocated with the other,
	// using exact lat,lng match
	return lat == other->lat && lng == other->lng;
}//*/

std::string Waypoint::root_at_label()
{	return route->root + "@" + label;
}//*/

#undef pi
