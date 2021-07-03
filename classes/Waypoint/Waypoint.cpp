#define debug
#include "Waypoint.h"
#include "../HighwaySystem/HighwaySystem.h"
#include "../Route/Route.h"
#include "../../functions/valid_num_str.h"
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
	bool valid_coords = 1;
	if (!valid_num_str(URL.data()+latBeg, '&'))
	{	size_t ampersand = URL.find('&', latBeg);
		std::string lat_string = (ampersand == -1) ? URL.data()+latBeg : URL.substr(latBeg, ampersand-latBeg);
		if (lat_string.size() > 59)
		{	lat_string = lat_string.substr(0, 59-3);
			while (lat_string.back() < 0)	lat_string.erase(lat_string.end()-1);
			lat_string += "...";
		}
		valid_coords = 0;
	}
	if (!valid_num_str(URL.data()+lonBeg, '&'))
	{	size_t ampersand = URL.find('&', lonBeg);
		std::string lng_string = (ampersand == -1) ? URL.data()+lonBeg : URL.substr(lonBeg, ampersand-lonBeg);
		if (lng_string.size() > 59)
		{	lng_string = lng_string.substr(0, 59-3);
			while (lng_string.back() < 0)	lng_string.erase(lng_string.end()-1);
			lng_string += "...";
		}
		valid_coords = 0;
	}
	if (valid_coords)
	     {	lat = strtod(&URL[latBeg], 0);
		lng = strtod(&URL[lonBeg], 0);
	     }
	else {	lat = 0;
		lng = 0;
	     }
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
