#define debug
#include "Waypoint.h"
#include "../HighwaySystem/HighwaySystem.h"
#include "../Route/Route.h"
#include <cmath>
#include <cstring>

#ifdef debug
//#include <iostream>
#endif

Waypoint::Waypoint(char *line, Route *rte)
{	/* initialize object from a .wpt file line */
	route = rte;
	colocated = 0;

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
	// TMMinimal is intended for use with known good HighwayData commits with >=2 tokens per line
	label = alt_labels.front();		// first token is the primary label...
	alt_labels.pop_front();			// ...and not an alternate.

	// parse URL
	size_t latBeg = URL.find("lat=")+4;
	size_t lonBeg = URL.find("lon=")+4;
	lat = strtod(&URL[latBeg], 0);
	lng = strtod(&URL[lonBeg], 0);
}

std::string Waypoint::root_at_label()
{	return route->root + "@" + label;
}//*/
