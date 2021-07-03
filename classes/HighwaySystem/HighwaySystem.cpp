#include "HighwaySystem.h"
#include "../Args/Args.h"
#include "../Route/Route.h"
#include "../../functions/split.h"
#include <cstring>
#include <fstream>

std::list<HighwaySystem*> HighwaySystem::syslist;
std::list<HighwaySystem*>::iterator HighwaySystem::it;
std::vector<HighwaySystem*> HighwaySystem::in_flight;

HighwaySystem::HighwaySystem(std::string &line)
{	std::ifstream file;
	// parse systems.csv line
	size_t NumFields = 6;
	std::string dummy;
	std::string* fields[6] = {&systemname, &dummy, &dummy, &dummy, &dummy, &dummy};
	split(line, fields, NumFields, ';');

	std::cout << systemname << '.' << std::flush;

	// read chopped routes CSV
	file.open(Args::highwaydatapath+"/hwy_data/_systems"+"/"+systemname+".csv");
		getline(file, line); // ignore header line
		while(getline(file, line))
		{	if (line.empty()) continue;
			// trim DOS newlines & trailing whitespace
			while ( strchr("\r\t ", line.back()) ) line.pop_back();
			route_list.push_back(new Route(line, this));
					     // deleted on termination of program
		}
	file.close();
}
