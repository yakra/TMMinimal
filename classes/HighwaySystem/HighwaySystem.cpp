#include "HighwaySystem.h"
#include "../Args/Args.h"
#include "../DBFieldLength/DBFieldLength.h"
#include "../ErrorList/ErrorList.h"
#include "../Route/Route.h"
#include "../../functions/split.h"
#include <cstring>
#include <fstream>

std::list<HighwaySystem*> HighwaySystem::syslist;
std::list<HighwaySystem*>::iterator HighwaySystem::it;
std::vector<HighwaySystem*> HighwaySystem::in_flight;

HighwaySystem::HighwaySystem(std::string &line, ErrorList &el)
{	std::ifstream file;
	// parse systems.csv line
	size_t NumFields = 6;
	std::string country_str, tier_str, level_str;
	std::string* fields[6] = {&systemname, &country_str, &fullname, &color, &tier_str, &level_str};
	split(line, fields, NumFields, ';');
	is_valid = 1;

	// Removing sanity checks, as TMMinimal is to be tested
	// with a HighwayData commit known to have valid data

	// Tier
	char *endptr;
	tier = strtol(tier_str.data(), &endptr, 10);
	// Level
	level = level_str[0];

	std::cout << systemname << '.' << std::flush;

	// read chopped routes CSV
	file.open(Args::highwaydatapath+"/hwy_data/_systems"+"/"+systemname+".csv");
	if (!file) el.add_error("Could not open "+Args::highwaydatapath+"/hwy_data/_systems"+"/"+systemname+".csv");
	else {	getline(file, line); // ignore header line
		while(getline(file, line))
		{	if (line.empty()) continue;
			// trim DOS newlines & trailing whitespace
			while ( strchr("\r\t ", line.back()) ) line.pop_back();
			Route* r = new Route(line, this, el);
				   // deleted on termination of program
			if (r->root.size()) route_list.push_back(r);
			else {	el.add_error("Unable to find root in " + systemname +".csv line: ["+line+"]");
				delete r;
			     }
		}
	     }
	file.close();
}
