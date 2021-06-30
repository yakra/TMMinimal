class Waypoint;
#include <list>
#include <mutex>

class WaypointQuadtree
{	// This class defines a recursive quadtree structure to store
	// Waypoint objects for efficient geometric searching.

	public:
	double min_lat, min_lng, max_lat, max_lng, mid_lat, mid_lng;
	WaypointQuadtree *nw_child, *ne_child, *sw_child, *se_child;
	std::list<Waypoint*> points;
	unsigned int unique_locations;
	std::recursive_mutex mtx;

	bool refined();
	WaypointQuadtree(double, double, double, double);
	void refine(unsigned int);
	void insert(unsigned int, Waypoint*, bool);
};
