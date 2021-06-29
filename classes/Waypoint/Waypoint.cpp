#define debug
#include "Waypoint.h"
#include "../Datacheck/Datacheck.h"
#include "../DBFieldLength/DBFieldLength.h"
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
	{	Datacheck::add(route, label, "", "", "MALFORMED_URL", "MISSING_ARG(S)");
		lat = 0;	lng = 0;	return;
	}
	bool valid_coords = 1;
	if (!valid_num_str(URL.data()+latBeg, '&'))
	{	size_t ampersand = URL.find('&', latBeg);
		std::string lat_string = (ampersand == -1) ? URL.data()+latBeg : URL.substr(latBeg, ampersand-latBeg);
		if (lat_string.size() > DBFieldLength::dcErrValue)
		{	lat_string = lat_string.substr(0, DBFieldLength::dcErrValue-3);
			while (lat_string.back() < 0)	lat_string.erase(lat_string.end()-1);
			lat_string += "...";
		}
		Datacheck::add(route, label, "", "", "MALFORMED_LAT", lat_string);
		valid_coords = 0;
	}
	if (!valid_num_str(URL.data()+lonBeg, '&'))
	{	size_t ampersand = URL.find('&', lonBeg);
		std::string lng_string = (ampersand == -1) ? URL.data()+lonBeg : URL.substr(lonBeg, ampersand-lonBeg);
		if (lng_string.size() > DBFieldLength::dcErrValue)
		{	lng_string = lng_string.substr(0, DBFieldLength::dcErrValue-3);
			while (lng_string.back() < 0)	lng_string.erase(lng_string.end()-1);
			lng_string += "...";
		}
		Datacheck::add(route, label, "", "", "MALFORMED_LON", lng_string);
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

std::string Waypoint::str()
{	std::string ans = route->root + " " + label;
	char coordstr[51];
	sprintf(coordstr, "%.15g", lat);
	if (!strchr(coordstr, '.')) strcat(coordstr, ".0"); // add single trailing zero to ints for compatibility with Python
	ans += " (";
	ans += coordstr;
	ans += ',';
	sprintf(coordstr, "%.15g", lng);
	if (!strchr(coordstr, '.')) strcat(coordstr, ".0"); // add single trailing zero to ints for compatibility with Python
	ans += coordstr;
	return ans + ')';
}//*/

bool Waypoint::same_coords(Waypoint *other)
{	// return if this waypoint is colocated with the other,
	// using exact lat,lng match
	return lat == other->lat && lng == other->lng;
}//*/

double Waypoint::distance_to(Waypoint *other)
{	// return the distance in miles between this waypoint and another
	// including the factor defined by the CHM project to adjust for
	// unplotted curves in routes
	// convert to radians
	double rlat1 = lat * (pi/180);
	double rlng1 = lng * (pi/180);
	double rlat2 = other->lat * (pi/180);
	double rlng2 = other->lng * (pi/180);

	// haversine formula
	double ans = asin(sqrt(pow(sin((rlat2-rlat1)/2),2) + cos(rlat1) * cos(rlat2) * pow(sin((rlng2-rlng1)/2),2))) * 7926.2; // EARTH_DIAMETER

	return ans * 1.02112; // CHM/TM distance fudge factor to compensate for imprecision of mapping
}//*/

double Waypoint::angle(Waypoint *pred, Waypoint *succ)
{	// return the angle in degrees formed by the waypoints between the
	// line from pred to self and self to succ
	// convert to radians
	double rlatself = lat * (pi/180);
	double rlngself = lng * (pi/180);
	double rlatpred = pred->lat * (pi/180);
	double rlngpred = pred->lng * (pi/180);
	double rlatsucc = succ->lat * (pi/180);
	double rlngsucc = succ->lng * (pi/180);

	double x0 = cos(rlngpred)*cos(rlatpred);
	double x1 = cos(rlngself)*cos(rlatself);
	double x2 = cos(rlngsucc)*cos(rlatsucc);

	double y0 = sin(rlngpred)*cos(rlatpred);
	double y1 = sin(rlngself)*cos(rlatself);
	double y2 = sin(rlngsucc)*cos(rlatsucc);

	double z0 = sin(rlatpred);
	double z1 = sin(rlatself);
	double z2 = sin(rlatsucc);

	return acos
	(	( (x2-x1)*(x1-x0) + (y2-y1)*(y1-y0) + (z2-z1)*(z1-z0) )
	/ sqrt	(	( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1) )
		*	( (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) + (z1-z0)*(z1-z0) )
		)
	)
	*180/pi;
}//*/

std::string Waypoint::root_at_label()
{	return route->root + "@" + label;
}//*/

/* Datacheck */

bool Waypoint::label_too_long()
{	// label longer than the DB can store
	if (label.size() > DBFieldLength::label)
	{	// save the excess beyond what can fit in a DB field, to put in the info/value column
		std::string excess = label.data()+DBFieldLength::label-3;
		// strip any partial multi-byte characters off the beginning
		while (excess.front() < 0)	excess.erase(excess.begin());
		// if it's too long for the info/value column,
		if (excess.size() > DBFieldLength::dcErrValue-3)
		{	// cut it down to what will fit,
			excess = excess.substr(0, DBFieldLength::dcErrValue-6);
			// strip any partial multi-byte characters off the end,
			while (excess.back() < 0)	excess.erase(excess.end()-1);
			// and append "..."
			excess += "...";
		}
		// now truncate the label itself
		label = label.substr(0, DBFieldLength::label-3);
		// and strip any partial multi-byte characters off the end
		while (label.back() < 0)	label.erase(label.end()-1);
		Datacheck::add(route, label+"...", "", "", "LABEL_TOO_LONG", "..."+excess);
		return 1;
	}
	return 0;
}

#undef pi
