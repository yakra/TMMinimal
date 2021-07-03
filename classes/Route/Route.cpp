#include "Route.h"
#include "../../functions/split.h"

std::unordered_set<std::string> Route::all_wpt_files;
std::mutex Route::awf_mtx;

Route::Route(std::string &line, HighwaySystem *sys, ErrorList &el)
{	/* initialize object from a .csv file line,
	but do not yet read in waypoint file */

	// parse chopped routes csv line
	size_t NumFields = 8;
	std::string sys_str, arn_str;
	std::string* fields[8] = {&sys_str, &rg_str, &route, &banner, &abbrev, &city, &root, &arn_str};
	split(line, fields, NumFields, ';');

	// Removing sanity checks, as TMMinimal is to be tested
	// with a HighwayData commit known to have valid data

	system = sys;
}
