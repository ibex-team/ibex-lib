#include "ibex.h"

#ifndef _IBEX_WITH_ODE_
#error "You need to install the ODE plugin (--with-ode)."
#endif

#include "args.hxx"

#include <sstream>

using namespace ibex;
using namespace std;

#define CMAX 10

int main(int argc, char** argv) {

	stringstream _rel_eps_f;
	_rel_eps_f << "Relative precision on the objective. Default value is 1e" << round(::log10(OptimizerConfig::default_rel_eps_f)) << ".";

	args::ArgumentParser parser("********* IbexODE-attract-region *********.", "Finds an attracting sub-level set of a Lyapunov function v(x)<=c.");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::Positional<std::string> filename(parser, "filename", "The name of the MINIBEX file. The file must contain the declarations of:\n"
			"- a function f(x), or f(x,theta) if the system has an uncertain parameter theta\n"  \
			"- a function v(x)\n" \
			"- a quadratic function vminor(x) minoring v(x), if v(x) is not already quadratic\n"  \
			"- a declaration of a (vector) variable x with an approximation of the fixpoint of as domain\n"  \
			"- a declaration of a (vector) variable theta if the system has an uncertain parameter theta, with potentially an initial domain\n"  \
			"- constraints on theta (optional)" \
			);
	args::Flag version(parser, "version", "Display the version of this program (same as the version of the plugin).", {'v',"version"});
	args::ValueFlag<double> rel_eps_f(parser, "float", _rel_eps_f.str(), {'r', "rel-eps-f"});
	args::ValueFlag<double> timeout(parser, "float", "Timeout for optimization (time in seconds). Default value is +oo.", {'t', "timeout"});

	try
	{
		parser.ParseCLI(argc, argv);
	}
	catch (args::Help&)
	{
		std::cout << parser;
		return 0;
	}
	catch (args::ParseError& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}
	catch (args::ValidationError& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}

	if (version) {
		cout << "ibexode Release " << _IBEX_RELEASE_ << endl;
		exit(0);
	}

	if (filename.Get()=="") {
		ibex_error("no input file (try ibexode-attract-region --help)");
		exit(1);
	}

	cout << endl << "************************ setup ************************" << endl;
	cout << "  file loaded:\t\t" << filename.Get() << endl;

	double eps_f;

	if (rel_eps_f) {
		eps_f = rel_eps_f.Get();
		cout << "  rel-eps-f:\t\t" << rel_eps_f.Get() << "\t(relative precision on Lyapunov function level)" << endl;
	} else
		eps_f = OptimizerConfig::default_rel_eps_f;

	try {

		P_StructLyapunov struc(filename.Get().c_str());

		Timer timer;
		timer.start();

		AttractionRegion l(
				struc.f,
				struc.v,
				struc.vminor,
				struc.xhat,
				struc.theta,
				struc.theta_ctrs);

		timer.stop();
		double build_time = timer.get_time();

		// This option limits the search time
		if (timeout) {
			cout << "  timeout:\t\t" << timeout.Get() << "s" << endl;
			l.timeout = timeout.Get();
		}

		cout << "*******************************************************" << endl << endl;

		double time;

		Interval c1gd=l.find_level_set_dichotomy(AttractionRegion::DERIVATIVE, time, CMAX, eps_f);
		cout << "c_dichotomy_derivative= " << c1gd << " t=" << (1000.*(build_time+time)) << "ms" << endl;

		Interval c1gh=l.find_level_set_dichotomy(AttractionRegion::HANSEN, time, CMAX, eps_f);
		cout << "c_dichotomy_hansen=     " << c1gh << " t=" << (1000.*(build_time+time)) << "ms" << endl;

		Interval c1d=l.find_level_set_dichotomy(AttractionRegion::HESSIAN, time, CMAX, eps_f);
		cout << "c_dichotomy_hessian=    " << c1d << " t=" << (1000.*(build_time+time)) << "ms" << endl;

		Vector feasible(struc.xhat.size());
		int nb_cells;

		// TODO: if the problem is unfeasible, run with larger upper bound and so on.
		Interval c2=l.find_level_set_optim(c1gh.lb(), 1000, eps_f, feasible, time, nb_cells);

		cout << "c_optimizer=            " << c2 << " t=" << (1000.*time) << "ms" << endl << endl;

		cout << "  A point x such that v(x-x*)=c and v'(x-x*)>0:" << endl << "  " << feasible << endl << endl;

	} catch(UnknownFileException& e) {
		ibex_error("file not found");
	} catch(SyntaxError& e) {
		cout << e << endl;
	} catch(QuadraticFunction::NotQuadratic&) {
		ibex_error("could not detect ``v'' (or ``vminor'') as quadratic");
	}
}
