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

#define DECIMALFP_NDECIMAL 1
#define DECIMALFP_MUL_UINT 10 /* 10^DECIMALFP_NDECIMAL */
#define DECIMALFP_MUL_DOUBLE 10. /* 10^DECIMALFP_NDECIMAL */

class DecimalFP
{
	private:
		unsigned int _v;

	public:
		DecimalFP () : _v(UINT_MAX) {} ;
		DecimalFP (const unsigned int u)
		{
			_v = u * DECIMALFP_MUL_UINT;
		}
		DecimalFP (const double d)
		{
			_v = round (d * DECIMALFP_MUL_DOUBLE);
		}
		bool is_nan() const
		{
			return _v == UINT_MAX;
		}
		double get_double () const
		{
			return (this->is_nan()) ? NAN : _v/DECIMALFP_MUL_DOUBLE;
		}
		DecimalFP& operator= (const unsigned int u)
		{
			_v = u * DECIMALFP_MUL_UINT;
			return *this;
		}
		DecimalFP& operator= (const double d)
		{
			_v = round (d * DECIMALFP_MUL_DOUBLE);
			return *this;
		}
		DecimalFP& operator+= (const DecimalFP &o)
		{
			this->_v += o._v;
			return *this;
		}
		DecimalFP& operator-= (const DecimalFP &o)
		{
			this->_v -= o._v;
			return *this;
		}
		bool operator< (DecimalFP const & o)
		{
			return (this->_v < o._v);
		}
		bool operator<= (DecimalFP const & o)
		{
			return (this->_v <= o._v);
		}
		bool operator!= (DecimalFP const & o)
		{
			return (this->_v != o._v);
		}
		friend ostream& operator<< (ostream &, DecimalFP const &);
};



ostream& operator<< (ostream &os, DecimalFP const &v)
{
	if (v.is_nan())
		return os << "nan";
	else
		return os << (v._v/DECIMALFP_MUL_UINT) << "." << (v._v%DECIMALFP_MUL_UINT);
}

/* Return true if timeout was reached for at least one of the #iter run(s).
 * Return false otherwise.
 */
bool
do_benchs_iter (System &sys, DecimalFP prec, double time_limit,
                unsigned int iter)
{
	bool timeout = false;
	double eps = pow (10, -prec.get_double());

	for (unsigned int i = 0; i < iter; i++)
	{
		/* Build the default optimizer */
		double random_seed = DefaultOptimizer::default_random_seed + (double) i;
		DefaultOptimizer DefOpt (sys, eps, eps, NormalizedSystem::default_eps_h,
		                         false, true, random_seed, 0.0);

		/* Set the time limit */
		DefOpt.timeout = time_limit;

		/* Do the actual computation */
		Optimizer::Status status = DefOpt.optimize (sys.box);

		/* Report some information (computation time, etc.) */
		std::cout << "BENCH: eps = 10^-" << prec
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
		double time_limit = NAN;
		DecimalFP prec_max, prec_min;
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
				prec_min = double_from_arg ("--prec-ndigits-min", argv[1]);
				argc-=2; argv+=2;
			}
			else if (strcmp (argv[0], "--prec-ndigits-max") == 0)
			{
				prec_max = double_from_arg ("--prec-ndigits-max", argv[1]);
				argc-=2; argv+=2;
			}
			else
				usage ("unrecognized command-line parameter");
		}
		if (argc != 0)
			usage ("too many command-line parameter");

		cout << "# INPUT: bench file: " << benchfile << endl;
		cout << "# INPUT: time limit: " << time_limit << "s" << endl;
		cout << "# INPUT: prec ndigits max: " << prec_max << endl;
		cout << "# INPUT: prec ndigits min: " << prec_min << endl;

		/* Check for missing command-line parameter */
		if (prec_max.is_nan())
			usage ("missing --prec-ndigits-max command-line parameter");

		if (prec_min.is_nan())
			usage ("missing --prec-ndigits-min command-line parameter");

		if (prec_max < prec_min)
			usage ("--prec-ndigits-min should not be larger than --prec-ndigits-max");

		if (DecimalFP (8.) < prec_max)
			usage ("--prec-ndigits-max cannot be larger than 8");

		/* Load the file */
		System sys (benchfile);

		/* Check that the file has a 'goal' */
		if (!sys.goal)
			ibex_error ("input file does not contains an optimization problem.");

		/* always bench prec_min */
		bool has_timeout = do_benchs_iter (sys, prec_min, time_limit, iter);
		if (!has_timeout)
		{
			DecimalFP prec = 0., incr = 1.;
			for ( ; prec < prec_max; prec += incr)
			{
				if (prec_min < prec)
				{
					has_timeout = do_benchs_iter (sys, prec, time_limit, iter);
					if (has_timeout)
						break;
				}
			}
			prec -= incr; incr = 0.1; prec += incr;
			for (unsigned int i = 1; i < 10; i++, prec += incr)
			{
				if (prec <= prec_min)
					continue;
				else if (prec_max < prec)
					break;
				else
				{
					has_timeout = do_benchs_iter (sys, prec, time_limit, iter);
					if (has_timeout)
						break;
				}
			}
			if (!has_timeout && prec_max != prec_min)
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
