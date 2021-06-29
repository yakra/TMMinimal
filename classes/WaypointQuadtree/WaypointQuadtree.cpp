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

void WaypointQuadtree::refine(unsigned int threadnum)
{	// refine a quadtree into 4 sub-quadrants"""
	//std::cout << "QTDEBUG: " << str() << " being refined" << std::endl;
	nw_child = new WaypointQuadtree(mid_lat, min_lng, max_lat, mid_lng);
	ne_child = new WaypointQuadtree(mid_lat, mid_lng, max_lat, max_lng);
	sw_child = new WaypointQuadtree(min_lat, min_lng, mid_lat, mid_lng);
	se_child = new WaypointQuadtree(min_lat, mid_lng, mid_lat, max_lng);
		   // deleted on termination of program
	for (Waypoint *p : points) insert(threadnum, p, 0);
	points.clear();
}

void WaypointQuadtree::insert(unsigned int threadnum, Waypoint *w, bool init)
{	// insert Waypoint *w into this quadtree node
	//std::cout << "QTDEBUG: " << str() << " insert " << w->str() << std::endl;
	mtx.lock();
	if (!refined())
	     {	DEBUG (	LOCK;
			COND {	std::cout << "ReadWptThread " << threadnum <<
				" BEG " << (init ? "init" : "refi") << " insert " << std::flush <<
				w->root_at_label() << "\t#" << w <<
				" into Quadtree node #" << this << std::endl;
			     }
			UNLOCK;
		      )//*/
		if (init)
		{	DEBUG (	LOCK;
				COND {	std::cout << "ReadWptThread " << threadnum <<
					"   look for colocated points during initial insertion..." << std::endl;
				     }
				UNLOCK;
			      )//*/
			Waypoint *other_w = 0;
			for (Waypoint *p : points)
			{	DEBUG (	LOCK; COND
				{	std::cout << "ReadWptThread " << threadnum << "     " <<
					p << std::flush << '\t' << p->root_at_label() << std::flush;
				}     )//*/
				if (p->same_coords(w))
				{	other_w = p;
					DEBUG (	COND std::cout << '!' << std::endl;
						UNLOCK;
					      )
					break;
				}
				DEBUG (	else COND std::cout << '?' << std::endl;
					UNLOCK;
				      )
			}
			if (other_w)
			{	// see if this is the first point colocated with other_w
				DEBUG(LOCK; COND{std::cout << "ReadWptThread " << threadnum << "       dereference other_w" << std::endl;} UNLOCK;)
				if (!other_w->colocated)
				{	other_w->colocated = new std::list<Waypoint*>;
							     // deleted on termination of program
					DEBUG(LOCK; COND{std::cout << "ReadWptThread " << threadnum << "         dereference other_w->colocated" << std::endl;} UNLOCK;)
					other_w->colocated->push_back(other_w);
				}
				DEBUG(LOCK; COND{std::cout << "ReadWptThread " << threadnum << "       dereference other_w->colocated" << std::endl;} UNLOCK;)
				other_w->colocated->push_back(w);
				DEBUG(LOCK; COND{std::cout << "ReadWptThread " << threadnum << "       dereference w->colocated" << std::endl;} UNLOCK;)
				w->colocated = other_w->colocated;
			}
			DEBUG(else COND
			{	LOCK;
				std::cout << "ReadWptThread " << threadnum << "   ...none found." << std::endl;
				UNLOCK;
			}    )//*/
		}
		DEBUG (	LOCK;
			COND {	std::cout << "ReadWptThread " << threadnum <<
				"   update unique location count" << std::endl;
			     }
			UNLOCK;
		      )//*/
		if (!w->colocated || w == w->colocated->front())
		{	//std::cout << "QTDEBUG: " << str() << " at " << unique_locations << " unique locations" << std::endl;
			unique_locations++;
		}
		points.push_back(w);
		if (unique_locations > 50)  // 50 unique points max per quadtree node
			refine(threadnum);
		DEBUG (	LOCK;
			COND {	std::cout << "ReadWptThread " << threadnum <<
				" END " << (init ? "init" : "refi") << " insert " << std::flush <<
				w->root_at_label() << "\t#" << w <<
				" into Quadtree node #" << this << std::endl;
			     }
			UNLOCK;
		      )//*/
		mtx.unlock();
	     }
	else {	mtx.unlock();
		if (w->lat < mid_lat)
			if (w->lng < mid_lng)
				sw_child->insert(threadnum, w, init);
			else	se_child->insert(threadnum, w, init);
		else	if (w->lng < mid_lng)
				nw_child->insert(threadnum, w, init);
			else	ne_child->insert(threadnum, w, init);
	     }
}
