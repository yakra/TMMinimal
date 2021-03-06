#define debug
#ifdef debug
  #define   DEBUG(WHAT) WHAT
#else
  #define   DEBUG(WHAT)
#endif

#include "Route.h"
#include "../Args/Args.h"
#include "../HighwaySystem/HighwaySystem.h"
#include "../Waypoint/Waypoint.h"
#include "../WaypointQuadtree/WaypointQuadtree.h"
#include <cstring>
#include <fstream>
#include <unordered_set>

void Route::read_wpt(unsigned int threadnum, WaypointQuadtree *all_waypoints)
{	/* read data into the Route's waypoint list from a .wpt file */
	extern std::mutex terminal_mtx;
	//cout << "read_wpt on " << str() << endl;
	std::string filename = Args::highwaydatapath + "/hwy_data" + "/" + rg_str + "/" + system->systemname + "/" + root + ".wpt";
	// remove full path from all_wpt_files list
	awf_mtx.lock();
	all_wpt_files.erase(filename);
	awf_mtx.unlock();
	std::vector<char*> lines;
	std::ifstream file(filename);
	if (!file)
	{	std::cout << "ERROR: No such file or directory: '" + filename + '\'' << std::endl;
		file.close();
		return;
	}
	file.seekg(0, std::ios::end);
	unsigned long wptdatasize = file.tellg();
	file.seekg(0, std::ios::beg);
	char *wptdata = new char[wptdatasize+1];
	file.read(wptdata, wptdatasize);
	wptdata[wptdatasize] = 0; // add null terminator
	file.close();

	// split file into lines
	size_t spn = 0;
	for (char* c = wptdata; *c; c += spn)
	{	for (spn = strcspn(c, "\n\r"); c[spn] == '\n' || c[spn] == '\r'; spn++) c[spn] = 0;
		lines.emplace_back(c);
	}

	lines.push_back(wptdata+wptdatasize+1); // add a dummy "past-the-end" element to make lines[l+1]-2 work
	// set to be used for finding duplicate coordinates
	std::unordered_set<Waypoint*> coords_used;
	double vis_dist = 0;
	Waypoint *last_visible = 0;
	char fstr[112];

	for (unsigned int l = 0; l < lines.size()-1; l++)
	{	// strip whitespace
		while (lines[l][0] == ' ' || lines[l][0] == '\t') lines[l]++;
		char * endchar = lines[l+1]-2; // -2 skips over the 0 inserted while splitting wptdata into lines
		while (*endchar == 0) endchar--;  // skip back more for CRLF cases, and lines followed by blank lines
		while (*endchar == ' ' || *endchar == '\t')
		{	*endchar = 0;
			endchar--;
		}
		if (lines[l][0] == 0) continue;
		Waypoint *w = new Waypoint(lines[l], this);
			      // deleted on termination of program, or immediately below if invalid
		bool malformed_url = w->lat == 0 && w->lng == 0;
		if (malformed_url)
		{	delete w;
			continue;
		}
		point_list.push_back(w);
		all_waypoints->insert(threadnum, w, 1);
	}
	delete[] wptdata;

	DEBUG(terminal_mtx.lock();)
	std::cout << '.' << std::flush;
	DEBUG(terminal_mtx.unlock();)
	//std::cout << str() << std::flush;
	//print_route();
}
