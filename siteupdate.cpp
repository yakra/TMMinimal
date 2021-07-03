// Tab Width = 8

// Travel Mapping Project, Jim Teresco and Eric Bryant, 2015-2021
/* Code to read .csv and .wpt files and prepare for
adding to the Travel Mapping Project database.

(c) 2015-2021, Jim Teresco and Eric Bryant
Original Python version by Jim Teresco, with contributions from Eric Bryant and the TravelMapping team
C++ translation by Eric Bryant

This module defines classes to represent the contents of a
.csv file that lists the highways within a system, and a
.wpt file that lists the waypoints for a given highway.
*/

#include <cstring>
#include <dirent.h>
#include <thread>
#include "classes/Args/Args.h"
#include "classes/ElapsedTime/ElapsedTime.h"
#include "classes/ErrorList/ErrorList.h"
#include "classes/HighwaySystem/HighwaySystem.h"
#include "classes/Route/Route.h"
#include "classes/Waypoint/Waypoint.h"
#include "classes/WaypointQuadtree/WaypointQuadtree.h"
#include "functions/crawl_hwy_data.h"
#include "functions/split.h"
#ifdef threading_enabled
#include "functions/threads.h"
#endif
using namespace std;
mutex terminal_mtx;

int main(int argc, char *argv[])
{	ifstream file;
	string line;
	mutex list_mtx;

	// argument parsing
	if (Args::init(argc, argv)) return 1;
      #ifndef threading_enabled
	Args::numthreads = 1;
      #endif

	// start a timer for including elapsed time reports in messages
	ElapsedTime et;
	time_t timestamp = time(0);
	cout << "Start: " << ctime(&timestamp);

	// create ErrorList
	ErrorList el;

	// Create a list of HighwaySystem objects, one per system in systems.csv file
	cout << et.et() << "Reading systems list in " << Args::highwaydatapath << "/systems.csv." << endl;
	file.open(Args::highwaydatapath+"/systems.csv");
	if (!file) el.add_error("Could not open "+Args::highwaydatapath+"/systems.csv");
	else {	getline(file, line); // ignore header line
		list<string> ignoring;
		while(getline(file, line))
		{	if (line.back() == 0x0D) line.erase(line.end()-1);	// trim DOS newlines
			if (line.empty()) continue;
			if (line[0] == '#')
			{	ignoring.push_back("Ignored comment in systems.csv: " + line);
				continue;
			}
			HighwaySystem *hs = new HighwaySystem(line, el);
					    // deleted on termination of program
			if (!hs->is_valid) delete hs;
			else {	HighwaySystem::syslist.push_back(hs);
			     }
		}
		cout << endl;
		// at the end, print the lines ignored
		for (string& l : ignoring) cout << l << endl;
		ignoring.clear();
	     }
	file.close();

	// For tracking whether any .wpt files are in the directory tree
	// that do not have a .csv file entry that causes them to be
	// read into the data
	cout << et.et() << "Finding all .wpt files. " << flush;
	crawl_hwy_data(Args::highwaydatapath+"/hwy_data", Route::all_wpt_files);
	cout << Route::all_wpt_files.size() << " files found." << endl;

	// For finding colocated Waypoints and concurrent segments, we have
	// quadtree of all Waypoints in existence to find them efficiently
	WaypointQuadtree all_waypoints(-90,-180,90,180);

	// Next, read all of the .wpt files for each HighwaySystem
	cout << et.et() << "Reading waypoints for all routes." << endl;
      #ifdef threading_enabled
	HighwaySystem::in_flight.assign(Args::numthreads,0);
	std::vector<std::thread> thr(Args::numthreads);
	HighwaySystem::it = HighwaySystem::syslist.begin();
	#define THREADLOOP for (unsigned int t = 0; t < thr.size(); t++)
	THREADLOOP thr[t] = thread(ReadWptThread, t, &list_mtx, &el, &all_waypoints);
	THREADLOOP thr[t].join();
	HighwaySystem::in_flight.clear();
      #else
	for (HighwaySystem* h : HighwaySystem::syslist)
	{	std::cout << h->systemname << std::flush;
		for (Route* r : h->route_list)
			r->read_wpt(0, &all_waypoints, &el);
		std::cout << "!" << std::endl;
	}
      #endif

	timestamp = time(0);
	cout << "Finish: " << ctime(&timestamp);
	cout << "Total run time: " << et.et() << endl;

}
