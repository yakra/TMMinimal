class Route;
#include <deque>
#include <list>
#include <string>

class Waypoint
{   /* This class encapsulates the information about a single waypoint
    from a .wpt file.

    The line consists of one or more labels, at most one of which can
    be a "regular" label.  Others are "hidden" labels and must begin with
    a '+'.  Then an OSM URL which encodes the latitude and longitude.

    root is the unique identifier for the route in which this waypoint
    is defined
    */

	public:
	Route *route;
	std::list<Waypoint*> *colocated;
	double lat, lng;
	std::string label;
	std::deque<std::string> alt_labels;

	Waypoint(char *, Route *);

	std::string root_at_label();
};
