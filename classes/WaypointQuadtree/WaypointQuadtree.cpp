#define debug
#ifdef debug
  #include "../TravelerList/TravelerList.h"
  #define COND if (w->route->system->systemname == "usaca" || w->route->system->systemname == "usaal" || w->route->system->systemname == "usafl")
  #define DEBUG(WHAT) WHAT
  #define   LOCK TravelerList::mtx.lock() /* repurpose a mutex not doing anything */
  #define UNLOCK TravelerList::mtx.unlock() /* else ATM for locking terminal */
#else
  #define DEBUG(WHAT)
#endif

#include "WaypointQuadtree.h"
#include "../HighwaySystem/HighwaySystem.h"
#include "../Route/Route.h"
#include "../Waypoint/Waypoint.h"
#ifdef threading_enabled
#endif
inline bool WaypointQuadtree::WaypointQuadtree::refined()
{	return nw_child;
}

WaypointQuadtree::WaypointQuadtree(double MinLat, double MinLng, double MaxLat, double MaxLng)
{	// initialize an empty quadtree node on a given space"""
	min_lat = MinLat;
	min_lng = MinLng;
	max_lat = MaxLat;
	max_lng = MaxLng;
	mid_lat = (min_lat + max_lat) / 2;
	mid_lng = (min_lng + max_lng) / 2;
	nw_child = 0;
	ne_child = 0;
	sw_child = 0;
	se_child = 0;
	unique_locations = 0;
}
