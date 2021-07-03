#include "Args.h"
#include <cstring>

/* t */ int Args::numthreads = 4;
/* w */ std::string Args::highwaydatapath = "../../../HighwayData";
const char* Args::exec;

bool Args::init(int argc, char *argv[])
{	const char* slash = strrchr (argv[0], '/');
	exec = slash ? slash+1 : argv[0];

	// parsing
	for (unsigned int n = 1; n < argc; n++)
	#define ARG(N,S,L) ( n+N < argc && !strcmp(argv[n],S) || !strcmp(argv[n],L) )
	{	     if ARG(0, "-h", "--help")			{show_help(); return 1;}
		else if ARG(1, "-w", "--highwaydatapath")	{highwaydatapath  = argv[n+1]; n++;}
		else if ARG(1, "-t", "--numthreads")
		{	numthreads = strtol(argv[n+1], 0, 10);
			if (numthreads<1) numthreads=1;
			n++;
		}
	}
	#undef ARG
	return 0;
}

void Args::show_help()
{	std::string indent(strlen(exec), ' ');
	std::cout  <<  "usage: " << exec << " [-h] [-w HIGHWAYDATAPATH]\n";
	std::cout  <<  indent << "        [-t NUMTHREADS]\n";
	std::cout  <<  "\n";
	std::cout  <<  "optional arguments:\n";
	std::cout  <<  "  -h, --help            show this help message and exit\n";
	std::cout  <<  "  -w HIGHWAYDATAPATH, --highwaydatapath HIGHWAYDATAPATH\n";
	std::cout  <<  "		        path to the root of the highway data directory\n";
	std::cout  <<  "		        structure\n";
	std::cout  <<  "  -t NUMTHREADS, --numthreads NUMTHREADS\n";
	std::cout  <<  "		        Number of threads to use for concurrent tasks\n";
}
