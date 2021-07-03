class HGVertex;
class HighwayGraph;
class Route;
#include <deque>
#include <forward_list>
#include <fstream>
#include <list>
#include <unordered_set>
#include <vector>

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
	HGVertex *vertex;
	double lat, lng;
	std::string label;
	std::deque<std::string> alt_labels;
	std::vector<Waypoint*> ap_coloc;
	std::forward_list<Waypoint*> near_miss_points;
	unsigned int point_num;
	bool is_hidden;

	Waypoint(char *, Route *);

	std::string root_at_label();
};
