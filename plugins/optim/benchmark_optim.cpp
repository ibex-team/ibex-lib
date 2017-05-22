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
	return val;
}

void
usage (const char *errmsg)
{
	stringstream s;
	s << errmsg << std::endl
	  << "Usage: benchmark_optim ARGS" << std::endl
	  << "Mandatory parameter are:" << std::endl
	  << "  --bench-file <file>   file containing the problem" << std::endl
	  << "  --time-limit <t>      optimizer will stop after <t> seconds" << std::endl
	  << "  --min-prec <d>        " << std::endl
	  << "  --max-prec <d>        " << std::endl;
	ibex_error (s.str().c_str());
}

int
main (int argc, char *argv[])
{
	try
	{
		const char *benchfile = NULL;
		double max_prec = NAN, min_prec = NAN, time_limit = NAN;

		argc--; argv++; /* skip argv[0] = binary name */

		while (argc >= 2)
		{
			if (strcmp (argv[0], "--bench-file") == 0)
			{
				benchfile = argv[1];
				argc-=2; argv+=2;
			}
			else if (strcmp (argv[0], "--time-limit") == 0)
			{
				time_limit = double_from_arg ("--time-limit", argv[1]);
				argc-=2; argv+=2;
			}
			else if (strcmp (argv[0], "--min-prec") == 0)
			{
				min_prec = double_from_arg ("--min-prec", argv[1]);
				argc-=2; argv+=2;
			}
			else if (strcmp (argv[0], "--max-prec") == 0)
			{
				max_prec = double_from_arg ("--max-prec", argv[1]);
				argc-=2; argv+=2;
			}
			else
				usage ("unrecognized command-line parameter");
		}
		if (argc != 0)
			usage ("too many command-line parameter");

		cout << "# INPUT: bench file: " << benchfile << endl;
		cout << "# INPUT: time limit: " << time_limit << "s" << endl;
		cout << "# INPUT: max prec: " << max_prec << endl;
		cout << "# INPUT: min_prec: " << min_prec << endl;

		/* Load the file */
		System sys (benchfile);
		
		/* Check that the file has a 'goal' */
		if (!sys.goal)
			ibex_error ("input file does not contains an optimization problem.");

		for (double prec = 1; prec > 10e-12; prec /= 10)
		{
			if (min_prec <= prec && prec <= max_prec)
			{
				/* Build the default optimizer */
				DefaultOptimizer DefOpt (sys, prec, prec);

				/* Set the time limit */
				DefOpt.timeout = time_limit;

				/* Some options for the output */
				DefOpt.trace = 0; /* prints each better feasible point when it is found */
				/* Do the actual computation */
				DefOpt.optimize (sys.box);

				/* Report some information (computation time, etc.) */
				//DefOpt.report();
				std::cout << "BENCH: eps = " << prec << " ; time = " << DefOpt.time << std::endl;
			}
		}
		return EXIT_SUCCESS;
	}
	catch (ibex::SyntaxError& e)
	{
		cout << e << endl;
		return EXIT_FAILURE;
	}
}
