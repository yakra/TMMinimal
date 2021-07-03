#include "Route.h"
#include "../../functions/split.h"

Route::Route(std::string &line, HighwaySystem *sys)
{	/* initialize object from a .csv file line,
	but do not yet read in waypoint file */
	size_t NumFields = 8;
	std::string dummy;
	std::string* fields[8] = {&dummy, &rg_str, &dummy, &dummy, &dummy, &dummy, &root, &dummy};
	split(line, fields, NumFields, ';');
	system = sys;
}
