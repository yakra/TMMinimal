#include "Route.h"
#include "../Datacheck/Datacheck.h"
#include "../DBFieldLength/DBFieldLength.h"
#include "../ErrorList/ErrorList.h"
#include "../HighwaySystem/HighwaySystem.h"
#include "../Region/Region.h"
#include "../../functions/lower.h"
#include "../../functions/split.h"
#include "../../functions/upper.h"
#include <cstring>

std::unordered_map<std::string, Route*> Route::root_hash, Route::pri_list_hash, Route::alt_list_hash;
std::unordered_set<std::string> Route::all_wpt_files;
std::mutex Route::awf_mtx;

Route::Route(std::string &line, HighwaySystem *sys, ErrorList &el)
{	/* initialize object from a .csv file line,
	but do not yet read in waypoint file */
	con_route = 0;
	mileage = 0;
	rootOrder = -1; // order within connected route
	region = 0;	// if this stays 0, setup has failed due to bad .csv data
	is_reversed = 0;
	last_update = 0;

	// parse chopped routes csv line
	size_t NumFields = 8;
	std::string sys_str, arn_str;
	std::string* fields[8] = {&sys_str, &rg_str, &route, &banner, &abbrev, &city, &root, &arn_str};
	split(line, fields, NumFields, ';');

	// Removing sanity checks, as TMMinimal is to be tested
	// with a HighwayData commit known to have valid data

	// system
	system = sys;
	// region
	region = Region::code_hash.at(rg_str);
	lower(root.data());
	// alt_route_names
	upper(arn_str.data());
	size_t len;
	for (size_t pos = 0; pos < arn_str.size(); pos += len+1)
	{	len = strcspn(arn_str.data()+pos, ",");
		alt_route_names.emplace_back(arn_str, pos, len);
	}

	// insert into root_hash, checking for duplicate root entries
	if (!root_hash.insert(std::pair<std::string, Route*>(root, this)).second)
		el.add_error("Duplicate root in " + system->systemname + ".csv: " + root +
			     " already in " + root_hash.at(root)->system->systemname + ".csv");
	// insert list name into pri_list_hash, checking for duplicate .list names
	std::string list_name(readable_name());
	upper(list_name.data());
	if (alt_list_hash.find(list_name) != alt_list_hash.end())
		el.add_error("Duplicate main list name in " + root + ": '" + readable_name() +
			     "' already points to " + alt_list_hash.at(list_name)->root);
	else if (!pri_list_hash.insert(std::pair<std::string,Route*>(list_name, this)).second)
		el.add_error("Duplicate main list name in " + root + ": '" + readable_name() +
			     "' already points to " + pri_list_hash.at(list_name)->root);
	// insert alt names into alt_list_hash, checking for duplicate .list names
	for (std::string& a : alt_route_names)
	{   list_name = rg_str + ' ' + a;
	    upper(list_name.data());
	    if (pri_list_hash.find(list_name) != pri_list_hash.end())
		el.add_error("Duplicate alt route name in " + root + ": '" + region->code + ' ' + a +
			     "' already points to " + pri_list_hash.at(list_name)->root);
	    else if (!alt_list_hash.insert(std::pair<std::string, Route*>(list_name, this)).second)
		el.add_error("Duplicate alt route name in " + root + ": '" + region->code + ' ' + a +
			     "' already points to " + alt_list_hash.at(list_name)->root);
	    // populate unused set
	    system->unusedaltroutenames.insert(list_name);
	}
}

std::string Route::readable_name()
{	// return a string for a human-readable route name
	return rg_str + " " + route + banner + abbrev;
}//*/
