#include "ibex.h"
#include <sstream>

using namespace std;
using namespace ibex;

#define MAX(a,b) ((a < b) ? b : a)
#define MIN(a,b) ((a < b) ? a : b)

double tot_time = 0.0;

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

unsigned int
uint_from_arg (const char *argname, const char *str)
{
	char *endptr = NULL;
	unsigned int val = (unsigned int) strtoul (str, &endptr, 10);
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
	  << "  --prec-ndigits-max <d>        " << std::endl
	  << "  --iter <i>        " << std::endl;
	ibex_error (s.str().c_str());
}

/* Return true if timeout was reached for at least one of the #iter run(s).
 * Return false otherwise.
 */
bool
do_benchs_iter (System &sys, double prec, double time_limit, unsigned int iter)
{
	bool timeout = false;

	for (unsigned int i = 0; i < iter; i++)
	{
		/* Build the default optimizer */
		double random_seed = DefaultOptimizer::default_random_seed + (double) i;
		DefaultOptimizer DefOpt (sys, prec, prec,
		       NormalizedSystem::default_eps_h, false, true, random_seed, 0.0);

		/* Set the time limit */
		DefOpt.timeout = time_limit;

		/* Do the actual computation */
		Optimizer::Status status = DefOpt.optimize (sys.box);

		/* Report some information (computation time, etc.) */
		std::cout << "BENCH: eps = " << prec
	            << " ; status = " << DefOpt.get_status()
	            << " ; time = " << DefOpt.get_time()
	            << " ; nb_cells = " << DefOpt.get_nb_cells()
	            << " ; uplo = " << DefOpt.get_uplo()
	            << " ; loup = " << DefOpt.get_loup()
	            << " ; random_seed = " << random_seed
	            << std::endl;

		tot_time += DefOpt.get_time();
		timeout |= status == Optimizer::TIME_OUT;
	}

	return timeout;
}

int
main (int argc, char *argv[])
{
	try
	{
		const char *benchfile = NULL;
		double prec_ndigits_max = NAN, prec_ndigits_min = NAN, time_limit = NAN;
		double prec_min = NAN, prec_max = NAN;
    unsigned int iter = 0;

		argc--; argv++; /* skip argv[0] = binary name */

		while (argc >= 2)
		{
			if (strcmp (argv[0], "--bench-file") == 0)
			{
				benchfile = argv[1];
				argc-=2; argv+=2;
			}
			else if (strcmp (argv[0], "--iter") == 0)
			{
				iter = uint_from_arg ("--iter", argv[1]);
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

		/* always bench prec_min */
		bool has_timeout = do_benchs_iter (sys, prec_min, time_limit, iter);
		if (!has_timeout)
		{
			double prec_ndigits = 0.;
			for ( ; prec_ndigits < MIN (8., prec_ndigits_max); prec_ndigits += 1.)
			{
				if (prec_ndigits_min < prec_ndigits)
				{
					double prec = pow (10, -prec_ndigits);
					has_timeout = do_benchs_iter (sys, prec, time_limit, iter);
					if (has_timeout)
						break;
				}
			}
			prec_ndigits -= 0.9;
			for (unsigned int i = 1; i < 10; i++, prec_ndigits += 0.1)
			{
				if (prec_ndigits <= prec_ndigits_min)
					continue;
				else if (prec_ndigits > prec_ndigits_max)
					break;
				else
				{
					double prec = pow (10, -prec_ndigits);
					has_timeout = do_benchs_iter (sys, prec, time_limit, iter);
					if (has_timeout)
						break;
				}
			}
			if (!has_timeout && prec_ndigits_max != prec_ndigits_min)
				do_benchs_iter (sys, prec_max, time_limit, iter);
		}
		std::cout << "# Total time: " << tot_time << std::endl;
		return EXIT_SUCCESS;
	}
	catch (ibex::SyntaxError& e)
	{
		cout << e << endl;
		return EXIT_FAILURE;
	}
}
