class ErrorList;
class HighwaySegment;
class HighwaySystem;
class Waypoint;
class WaypointQuadtree;
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Route
{   /* This class encapsulates the contents of one .csv file line
    that represents a highway within a system and the corresponding
    information from the route's .wpt.

    The format of the .csv file for a highway system is a set of
    semicolon-separated lines, each of which has 8 fields:

    System;Region;Route;Banner;Abbrev;City;Route;AltRouteNames

    The first line names these fields, subsequent lines exist,
    one per highway, with values for each field.

    System: the name of the highway system this route belongs to,
    an instance of HighwaySystem

    Region: the project region or subdivision in which the
    route belongs.

    Route: the route name as would be specified in user lists

    Banner: the (optional) banner on the route such as 'Alt',
    'Bus', or 'Trk'.  Now allowed up to 6 characters

    Abbrev: (optional) for bannered routes or routes in multiple
    sections, the 3-letter abbrevation for the city or other place
    that is used to identify the segment.

    City: (optional) the full name to be displayed for the Abbrev
    above.

    Root: the name of the .wpt file that lists the waypoints of the
    route, without the .wpt extension.

    AltRouteNames: (optional) comma-separated list of former or
    other alternate route names that may appear in user list files.
    */

	public:
	HighwaySystem *system;
	std::string rg_str;	// region code string
	std::string route;
	std::string banner;
	std::string abbrev;
	std::string city;
	std::string root;

	std::vector<Waypoint*> point_list;
	std::vector<HighwaySegment*> segment_list;

	static std::unordered_set<std::string>	all_wpt_files;
	static std::mutex awf_mtx;		// for locking the all_wpt_files set when erasing processed WPTs

	Route(std::string &, HighwaySystem *, ErrorList &);

	void read_wpt(unsigned int, WaypointQuadtree *, ErrorList *);
};
