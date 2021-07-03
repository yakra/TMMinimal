#include <iostream>
#include <list>

class Args
{	public:
	/* w */ static std::string highwaydatapath;
	/* t */ static int numthreads;
	/* T */ static int timeprecision;
		static const char* exec;

	static bool init(int argc, char *argv[]);
	static void show_help();
};
