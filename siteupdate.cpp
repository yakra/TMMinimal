// Tab Width = 8

// Travel Mapping Project, Jim Teresco and Eric Bryant, 2015-2021
/* Code to read .csv and .wpt files and prepare for
adding to the Travel Mapping Project database.

(c) 2015-2021, Jim Teresco and Eric Bryant
Original Python version by Jim Teresco, with contributions from Eric Bryant and the TravelMapping team
C++ translation by Eric Bryant
*/

#include <fstream>
#include <thread>
#include "classes/Args/Args.h"
#include "classes/HighwaySystem/HighwaySystem.h"
#include "classes/Route/Route.h"
#include "classes/Waypoint/Waypoint.h"
#include "classes/WaypointQuadtree/WaypointQuadtree.h"
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

	// Create a list of HighwaySystem objects, one per system in systems.csv file
	cout << "[No_Timestamp]" << "Reading systems list in " << Args::highwaydatapath << "/systems.csv." << endl;
	file.open(Args::highwaydatapath+"/systems.csv");
		getline(file, line); // ignore header line
		while(getline(file, line))
		{	if (line.back() == 0x0D) line.erase(line.end()-1);	// trim DOS newlines
			if (line.empty()) continue;
			if (line[0] == '#') continue; // ignore comments
			HighwaySystem::syslist.push_back(new HighwaySystem(line));
							 // deleted on termination of program
		}
		cout << endl;
	file.close();

	// For finding colocated Waypoints and concurrent segments, we have
	// quadtree of all Waypoints in existence to find them efficiently
	WaypointQuadtree all_waypoints(-90,-180,90,180);

	// Next, read all of the .wpt files for each HighwaySystem
	cout << "[No_Timestamp]" << "Reading waypoints for all routes." << endl;
      #ifdef threading_enabled
	HighwaySystem::in_flight.assign(Args::numthreads,0);
	std::vector<std::thread> thr(Args::numthreads);
	HighwaySystem::it = HighwaySystem::syslist.begin();
	#define THREADLOOP for (unsigned int t = 0; t < thr.size(); t++)
	THREADLOOP thr[t] = thread(ReadWptThread, t, &list_mtx, &all_waypoints);
	THREADLOOP thr[t].join();
	HighwaySystem::in_flight.clear();
      #else
	for (HighwaySystem* h : HighwaySystem::syslist)
	{	std::cout << h->systemname << std::flush;
		for (Route* r : h->route_list)
			r->read_wpt(0, &all_waypoints);
		std::cout << "!" << std::endl;
	}
      #endif

	cout << "Total run time: " << "[No_Timestamp]" << endl;
}
