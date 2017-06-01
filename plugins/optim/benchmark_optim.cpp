#include "ibex.h"
#include <sstream>

using namespace std;
using namespace ibex;

#define MAX(a,b) ((a < b) ? b : a)
#define MIN(a,b) ((a < b) ? a : b)

double
double_from_arg (const char *argname, const char *str)
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
	  << "  --prec-ndigits-min <d>        " << std::endl
	  << "  --prec-ndigits-max <d>        " << std::endl;
	ibex_error (s.str().c_str());
}

/* return true if timeout was reached, false */
bool
do_one_bench (System &sys, double prec, double time_limit)
{
	/* Build the default optimizer */
	DefaultOptimizer DefOpt (sys, prec, prec);

	/* Set the time limit */
	DefOpt.timeout = time_limit;

	/* Do the actual computation */
	DefOpt.optimize (sys.box);

	/* Report some information (computation time, etc.) */
	std::cout << "BENCH: eps = " << prec << " ; time = " << DefOpt.time
						<< std::endl;

	return DefOpt.time >= time_limit;
}

int
main (int argc, char *argv[])
{
	try
	{
		const char *benchfile = NULL;
		double prec_ndigits_max = NAN, prec_ndigits_min = NAN, time_limit = NAN;
		double prec_min = NAN, prec_max = NAN;

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
			else if (strcmp (argv[0], "--prec-ndigits-min") == 0)
			{
				prec_ndigits_min = double_from_arg ("--prec-ndigits-min", argv[1]);
				argc-=2; argv+=2;
			}
			else if (strcmp (argv[0], "--prec-ndigits-max") == 0)
			{
				prec_ndigits_max = double_from_arg ("--prec-ndigits-max", argv[1]);
				argc-=2; argv+=2;
			}
			else
				usage ("unrecognized command-line parameter");
		}
		if (argc != 0)
			usage ("too many command-line parameter");

		cout << "# INPUT: bench file: " << benchfile << endl;
		cout << "# INPUT: time limit: " << time_limit << "s" << endl;
		cout << "# INPUT: prec ndigits max: " << prec_ndigits_max << endl;
		cout << "# INPUT: prec ndigits min: " << prec_ndigits_min << endl;

		/* Check for missing command-line parameter */
		if (isnan (prec_ndigits_max))
			usage ("missing --prec-ndigits-max command-line parameter");
		else
			prec_max = pow (10, -prec_ndigits_max);

		if (isnan (prec_ndigits_min))
			usage ("missing --prec-ndigits-min command-line parameter");
		else
			prec_min = pow (10, -prec_ndigits_min);

		if (prec_ndigits_min > prec_ndigits_max)
			usage ("--prec-ndigits-min should not be larger than --prec-ndigits-max");

		cout << "# INFO: prec max: " << prec_max << endl;
		cout << "# INFO: prec min: " << prec_min << endl;

		/* Load the file */
		System sys (benchfile);
		
		/* Check that the file has a 'goal' */
		if (!sys.goal)
			ibex_error ("input file does not contains an optimization problem.");

		/* always bench prec_min and prec_max */
		do_one_bench (sys, prec_min, time_limit);
		do_one_bench (sys, prec_max, time_limit);

		double prec_ndigits = 0.;
		for ( ; prec_ndigits < MIN (10., prec_ndigits_max); prec_ndigits += 1.)
		{
			if (prec_ndigits_min < prec_ndigits)
			{
				double prec = pow (10, -prec_ndigits);
				bool has_timeout = do_one_bench (sys, prec, time_limit);
				if (has_timeout)
					break;
			}
		}
		prec_ndigits -= 0.9;
		for (unsigned int i = 1; i < 10; i++, prec_ndigits += 0.1)
		{
			if (prec_ndigits_min < prec_ndigits)
			{
				double prec = pow (10, -prec_ndigits);
				bool has_timeout = do_one_bench (sys, prec, time_limit);
				if (has_timeout)
					break;
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
