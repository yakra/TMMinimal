class Route;
#include <list>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class HighwaySystem
{	/* This class encapsulates the contents of one .csv file
	that represents the collection of highways within a system.

	See Route for information about the fields of a .csv file
	*/

	public:
	std::string systemname;
	std::vector<Route*> route_list;

	static std::list<HighwaySystem*> syslist;
	static std::list<HighwaySystem*>::iterator it;
	static std::vector<HighwaySystem*> in_flight;

	HighwaySystem(std::string &);
};
