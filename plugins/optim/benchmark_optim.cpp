#include "ibex.h"
#include <sstream>

using namespace std;
using namespace ibex;


double double_from_arg (const char *argname, const char *str)
{
	char *endptr = NULL;
	double val = strtod (str, &endptr);
	if (endptr != str + strlen(str))
	{
		stringstream s;
		s << "\"" << argname << "\" must be a real number";
		ibex_error (s.str().c_str());
	}
	cout << "# " << argname << " = " << val << endl;
	return val;
}

void
usage (const char *argv0)
{
	stringstream s;
	s << "Usage: " << argv0 << " filename prec timelimit";
	ibex_error (s.str().c_str());
}

int
main (int argc, char *argv[])
{
	try
	{
		if (argc != 5) /* check the number of command-line arguments */
			usage (argv[0]);

		/* Load the file */
		const char *inputfile = argv[1];
		System sys (inputfile);
		cout << "# input file: " << inputfile << endl;
		
		/* Check that the file has a 'goal' */
		if (!sys.goal)
			ibex_error ("input file does not contains an optimization problem.");

		double prec       = double_from_arg ("prec", argv[2]);
		double goal_prec  = double_from_arg ("goal_prec", argv[3]);
		double time_limit = double_from_arg ("timelimit", argv[4]);

		/* Build the default optimizer */
		DefaultOptimizer DefOpt (sys, prec, goal_prec);

		/* Set the time limit */
		DefOpt.timeout = time_limit;

		/* Some options for the output */
		DefOpt.trace = 0; /* prints each better feasible point when it is found */
		cout.precision (12); /* display solutions with up to 12 decimals */

		/* Do the actual computation */
		DefOpt.optimize (sys.box);

		/* Report some information (computation time, etc.) */
		DefOpt.report();

		return EXIT_SUCCESS;
	}
	catch (ibex::SyntaxError& e)
	{
		cout << e << endl;
		return EXIT_FAILURE;
	}
}
