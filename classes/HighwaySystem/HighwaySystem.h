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

	With the implementation of three levels of systems (active,
	preview, devel), added parameter and field here, to be stored in
	DB
	*/

	public:
	std::string systemname;
	std::string fullname;
	std::string color;
	short tier;
	char level; // 'a' for active, 'p' for preview, 'd' for devel

	std::vector<Route*> route_list;
	std::unordered_set<std::string>listnamesinuse, unusedaltroutenames;
	std::mutex lniu_mtx, uarn_mtx;
	bool is_valid;

	static std::list<HighwaySystem*> syslist;
	static std::list<HighwaySystem*>::iterator it;
	static std::vector<HighwaySystem*> in_flight;

	HighwaySystem(std::string &);
};
