/* ============================================================================
 * I B E X - TestCov
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Nov 18, 2018
 * Last Update : Feb 14, 2019
 * ---------------------------------------------------------------------------- */

#include "TestCov.h"
#include "ibex_Cov.h"
#include "utils.h"

#include <fstream>
#include <string.h>
#include <stdlib.h>

using namespace std;

uint32_t TestCov::sol[nsol]             = { 1, 5, 8 };
bool TestCov::is_sol[N]                 = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 };

uint32_t TestCov::unk[nunk]             = { 0, 4 };
bool TestCov::is_unk[N]                 = { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0 };

uint32_t TestCov::pen[npen]             = { 2, 3 };
bool TestCov::is_pen[N]                 = { 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 };

uint32_t TestCov::bnd[nbnd]             = { 6, 7, 9 };
bool TestCov::is_bnd[N]                 = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 1 };

uint32_t TestCov::not_sol[N-nsol]       = { 0, 2, 3, 4, 6, 7, 9 };

uint32_t TestCov::sol_or_bnd[nsol+nbnd] = { 1, 5, 6, 7, 8, 9 };

uint32_t TestCov::bnd_or_unk_or_pen
                       [nbnd+nunk+npen] = { 0, 2, 3, 4, 6, 7, 9 };

uint32_t TestCov::unk_or_pen[nunk+npen] = { 0, 2, 3, 4 };

uint32_t TestCov::varset_sol[nsol][n-m]     = { {0, 1}, {1, 2}, {1, 2} };

uint32_t TestCov::varset_bnd[nbnd][n-m]     = { {0, 1}, {1, 2}, {0, 2} };

double TestCov::unicity_infl = 0.5;

string TestCov::solver_var_names[n]      = { "x1", "x2", "x3" };


namespace {

char* open_file(ofstream& f) { // return char* must be freed
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);
	f.open(tmpname, ios::out | ios::trunc | ios::binary);
	return tmpname;
}

void write(ofstream& of, const char* x) {
	of.write(x, (strlen(x)+1)*sizeof(char));
}

void write(ofstream& of, uint32_t x) {
	of.write((const char*) &x, sizeof(uint32_t));
}

void write(ofstream& of, double x) {
	of.write((const char*) &x, sizeof(double));
}


}


/*=============================================================================================*/

vector<IntervalVector> TestCov::boxes() {
	vector<IntervalVector> res;
	for (size_t i=0; i<N; i++) {
		res.push_back(IntervalVector(n,Interval(0,i)));
	}
	return res;
}

/*=============================================================================================*/

void TestCov::write_cov(ofstream& f, ScenarioType scenario, unsigned int level, bool right_version) {
	write(f,"IBEX COVERING FILE "); // signature
	write(f,(uint32_t) level);      // subformat level
	for (unsigned int i=0; i<=level; i++) // format sequence
		write(f, (uint32_t) 0);
	for (unsigned int i=0; i<=level; i++) // format version
		if (!right_version && i==level)
			write(f, (uint32_t) 2); // test a wrong version at last level
		else
			write(f, (uint32_t) 1);

	write(f,(uint32_t) n);          // box dimension
}

void TestCov::write_covlist(ofstream& f, ScenarioType scenario, unsigned int level, bool right_version) {
	write_cov(f,scenario,level,right_version);
	write(f,(uint32_t) N);  // number of boxes

	vector<IntervalVector> b = boxes();

	for (size_t i=0; i<N; i++) {
		for (size_t j=0; j<n; j++) {
			write(f,b[i][j].lb());
			write(f,b[i][j].ub());
		}
	}
}

void TestCov::write_covIUlist(ofstream& f, ScenarioType scenario, unsigned int level, bool right_version) {
	write_covlist(f,scenario,level,right_version);

	if (scenario==INEQ_EQ_ONLY || scenario==INEQ_HALF_BALL) {
		write(f, (uint32_t) nsol); // number of inner boxes
		for (size_t i=0; i<nsol; i++) {
			write(f, (uint32_t) sol[i]); // ith inner box
		}
	} else {
		write(f, (uint32_t) 0);
	}
}

void TestCov::write_covIBUlist(ofstream& f, ScenarioType scenario, unsigned int level, bool right_version) {
	write_covIUlist(f,scenario,level,right_version);
	write(f, (uint32_t) 0);           // type of boundary boxes = INNER_PT

	switch(scenario) {
	case INEQ_EQ_ONLY:
		write(f, (uint32_t) 0); // no boundary
		break;
	case INEQ_HALF_BALL:
		write(f, (uint32_t) nbnd); // total number of boundary boxes
		for (size_t i=0; i<nbnd; i++) {
			write(f, (uint32_t) bnd[i]);
		}
		break;
	case EQ_ONLY:
		write(f, (uint32_t) nsol); // total number of boundary boxes
		for (size_t i=0; i<nsol; i++) {
			write(f, (uint32_t) sol[i]);
		}
		break;
	case HALF_BALL:
		write(f, (uint32_t) (nbnd+nsol)); // total number of boundary boxes
		for (size_t i=0; i<nbnd+nsol; i++) {
			write(f, (uint32_t) sol_or_bnd[i]);
		}
		break;
	default:
		assert(false);
	}
}

void TestCov::write_covManifold(ofstream& f, ScenarioType scenario, unsigned int level, bool right_version) {
	write_covIBUlist(f,scenario, level,right_version);

	size_t nb_eq = (scenario==INEQ_EQ_ONLY || scenario==INEQ_HALF_BALL) ? 0 : m;

	write(f, (uint32_t) nb_eq);   // number of equalities
	write(f, (uint32_t) nb_ineq); // number of inequalities

	vector<IntervalVector> b=boxes();

	switch(scenario) {
	case INEQ_EQ_ONLY:
		write(f, (uint32_t) 0);       // type of boundary boxes = EQU_ONLY
		// no solution
		write(f, (uint32_t) nbnd);
		for (size_t i=0; i<nbnd; i++) {
			write(f, (uint32_t) bnd[i]);
		}
		break;
	case INEQ_HALF_BALL:
		write(f, (uint32_t) 2);       // type of boundary boxes = HALF_BALL
		// no solution
		write(f, (uint32_t) nbnd);
		for (size_t i=0; i<nbnd; i++) {
			write(f, (uint32_t) bnd[i]);
		}
		break;
	case EQ_ONLY:
		write(f, (uint32_t) 0);       // type of boundary boxes = EQU_ONLY
		write(f, (uint32_t) nsol);
		for (size_t i=0; i<nsol; i++) {
			write(f, (uint32_t) sol[i]);
			for (size_t j=0; j<n-m; j++) {
				write(f, varset_sol[i][j]);
			}
			IntervalVector unicity=b[sol[i]];
			unicity.inflate(unicity_infl);
			for (size_t j=0; j<n; j++) {
				write(f,unicity[j].lb());
				write(f,unicity[j].ub());
			}
		}
		write(f, (uint32_t) nbnd);
		for (size_t i=0; i<nbnd; i++) {
			write(f, (uint32_t) bnd[i]);
			for (size_t j=0; j<n-m; j++) {
				write(f, varset_bnd[i][j]);
			}
		}
		break;
	case HALF_BALL:
		write(f, (uint32_t) 2);       // type of boundary boxes = HALF_BALL
		write(f, (uint32_t) nsol);
		for (size_t i=0; i<nsol; i++) {
			write(f, (uint32_t) sol[i]);
			for (size_t j=0; j<n-m; j++) {
				write(f, varset_sol[i][j]);
			}
			IntervalVector unicity=b[sol[i]];
			unicity.inflate(unicity_infl);
			for (size_t j=0; j<n; j++) {
				write(f,unicity[j].lb());
				write(f,unicity[j].ub());
			}
		}
		write(f, (uint32_t) nbnd);
		for (size_t i=0; i<nbnd; i++) {
			write(f, (uint32_t) bnd[i]);
			for (size_t j=0; j<n-m; j++) {
				write(f, varset_bnd[i][j]);
			}
		}
		break;
	default:
		assert(false);
	}
}

void TestCov::write_covSolverData(ofstream& f, ScenarioType scenario, unsigned int level, bool right_version) {
	write_covManifold(f,scenario,level,right_version);

	for (size_t i=0; i<n; i++)
		write(f,solver_var_names[i].c_str());

	switch (solver_status) {
	case Solver::SUCCESS:           write(f,(uint32_t) 0); break;
	case Solver::INFEASIBLE:        write(f,(uint32_t) 1); break;
	case Solver::NOT_ALL_VALIDATED: write(f,(uint32_t) 2); break;
	case Solver::TIME_OUT:          write(f,(uint32_t) 3); break;
	default:                        write(f,(uint32_t) 4); break;
	}
	write(f,solver_time);
	write(f,(uint32_t) solver_nb_cells);
	write(f,(uint32_t) npen);

	for (size_t i=0; i<npen; i++) {
		write(f, (uint32_t) pen[i]);
	}
}

/*=============================================================================================*/

void TestCov::test_cov(ScenarioType scenario, Cov& cov) {
	CPPUNIT_ASSERT(cov.n==n);
}

void TestCov::test_covlist(ScenarioType scenario, CovList& cov) {
	test_cov(scenario, cov);

	CPPUNIT_ASSERT(cov.size()==N);

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		//cout << cov[i] << endl;
		CPPUNIT_ASSERT(cov[i]==b[i]);
	}
}

void TestCov::test_covIUlist(ScenarioType scenario, CovIUList& cov) {
	test_covlist(scenario, cov);

	if (scenario==INEQ_EQ_ONLY || scenario==INEQ_HALF_BALL) {
		CPPUNIT_ASSERT(cov.nb_inner()==nsol);
		CPPUNIT_ASSERT(cov.nb_unknown()==N-nsol);
		for (size_t i=0; i<N; i++) {
			CPPUNIT_ASSERT((cov.status(i)==CovIUList::INNER && is_sol[i]) ||
			(cov.status(i)==CovIUList::UNKNOWN && (!is_sol[i])) );
			if (is_sol[i]) {
				CPPUNIT_ASSERT(cov.is_inner(i));
				CPPUNIT_ASSERT(!cov.is_unknown(i));
			} else {
				CPPUNIT_ASSERT(!cov.is_inner(i));
				CPPUNIT_ASSERT(cov.is_unknown(i));
			}
		}

		vector<IntervalVector> b=boxes();

		for (size_t i=0; i<nsol; i++) {
			CPPUNIT_ASSERT(cov.inner(i)==b[sol[i]]);
		}

		for (size_t i=0; i<N-nsol; i++) {
			CPPUNIT_ASSERT(cov.unknown(i)==b[not_sol[i]]);
		}
	} else {
		CPPUNIT_ASSERT(cov.nb_inner()==0);
		CPPUNIT_ASSERT(cov.nb_unknown()==N);
	}
}

void TestCov::test_covIBUlist(ScenarioType scenario, CovIBUList& cov) {
	test_covIUlist(scenario, cov);

	vector<IntervalVector> b=boxes();

	switch(scenario) {
	case INEQ_EQ_ONLY:
		CPPUNIT_ASSERT(cov.nb_boundary()==0);
		CPPUNIT_ASSERT(cov.nb_unknown()==nbnd+nunk+npen);

		for (size_t i=0; i<N; i++) {
			CPPUNIT_ASSERT(
					(cov.status(i)==CovIBUList::INNER && is_sol[i]) ||
					(cov.status(i)==CovIBUList::UNKNOWN && (!is_sol[i]))
			);

			if (is_sol[i]) {
				CPPUNIT_ASSERT(!cov.is_boundary(i));
				CPPUNIT_ASSERT(!cov.is_unknown(i));
			}
			else {
				CPPUNIT_ASSERT(!cov.is_boundary(i));
				CPPUNIT_ASSERT(cov.is_unknown(i));
			}
		}

		for (size_t i=0; i<nbnd+nunk+npen; i++) {
			CPPUNIT_ASSERT(cov.unknown(i)==b[bnd_or_unk_or_pen[i]]);
		}
		break;
	case INEQ_HALF_BALL:
		CPPUNIT_ASSERT(cov.nb_inner()==nsol);
		CPPUNIT_ASSERT(cov.nb_boundary()==nbnd);
		CPPUNIT_ASSERT(cov.nb_unknown()==nunk+npen);

		for (size_t i=0; i<N; i++) {
			CPPUNIT_ASSERT(
					(cov.status(i)==CovIBUList::INNER && is_sol[i]) ||
					(cov.status(i)==CovIBUList::BOUNDARY && is_bnd[i]) ||
					(cov.status(i)==CovIBUList::UNKNOWN && (is_unk[i] || is_pen[i]))
			);

			if (is_sol[i]) {
				CPPUNIT_ASSERT(!cov.is_boundary(i));
				CPPUNIT_ASSERT(!cov.is_unknown(i));
			}
			else if (is_bnd[i]) {
				CPPUNIT_ASSERT(cov.is_boundary(i));
				CPPUNIT_ASSERT(!cov.is_unknown(i));
			} else {
				CPPUNIT_ASSERT(!cov.is_boundary(i));
				CPPUNIT_ASSERT(cov.is_unknown(i));
			}
		}

		for (size_t i=0; i<nbnd; i++) {
			CPPUNIT_ASSERT(cov.boundary(i)==b[bnd[i]]);
		}

		for (size_t i=0; i<nunk+npen; i++) {
			CPPUNIT_ASSERT(cov.unknown(i)==b[unk_or_pen[i]]);
		}
		break;
	case EQ_ONLY:
		CPPUNIT_ASSERT(cov.nb_inner()==0);
		CPPUNIT_ASSERT(cov.nb_boundary()==nsol);
		CPPUNIT_ASSERT(cov.nb_unknown()==nbnd+nunk+npen);

		for (size_t i=0; i<N; i++) {
			CPPUNIT_ASSERT(
					(cov.status(i)==CovIBUList::BOUNDARY && is_sol[i]) ||
					(cov.status(i)==CovIBUList::UNKNOWN && (is_bnd[i] || is_unk[i] || is_pen[i]))
			);

			if (is_sol[i]) {
				CPPUNIT_ASSERT(cov.is_boundary(i));
				CPPUNIT_ASSERT(!cov.is_unknown(i));
			} else {
				CPPUNIT_ASSERT(!cov.is_boundary(i));
				CPPUNIT_ASSERT(cov.is_unknown(i));
			}
		}

		for (size_t i=0; i<nsol; i++) {
			CPPUNIT_ASSERT(cov.boundary(i)==b[sol[i]]);
		}

		for (size_t i=0; i<nbnd+nunk+npen; i++) {
			CPPUNIT_ASSERT(cov.unknown(i)==b[bnd_or_unk_or_pen[i]]);
		}
		break;
	case HALF_BALL:
		CPPUNIT_ASSERT(cov.nb_inner()==0);
		CPPUNIT_ASSERT(cov.nb_boundary()==nbnd+nsol);
		CPPUNIT_ASSERT(cov.nb_unknown()==nunk+npen);

		for (size_t i=0; i<N; i++) {
			CPPUNIT_ASSERT(
					(cov.status(i)==CovIBUList::BOUNDARY && (is_sol[i] || is_bnd[i])) ||
					(cov.status(i)==CovIBUList::UNKNOWN && (is_unk[i] || is_pen[i]))
			);

			if (is_sol[i] || is_bnd[i]) {
				CPPUNIT_ASSERT(cov.is_boundary(i));
				CPPUNIT_ASSERT(!cov.is_unknown(i));
			} else {
				CPPUNIT_ASSERT(!cov.is_boundary(i));
				CPPUNIT_ASSERT(cov.is_unknown(i));
			}
		}

		for (size_t i=0; i<nsol+nbnd; i++) {
			CPPUNIT_ASSERT(cov.boundary(i)==b[sol_or_bnd[i]]);
		}

		for (size_t i=0; i<nunk+npen; i++) {
			CPPUNIT_ASSERT(cov.unknown(i)==b[unk_or_pen[i]]);
		}
		break;
	default:
		assert(false);
	}
}

void TestCov::test_covManifold(ScenarioType scenario, CovManifold& cov) {
	test_covIBUlist(scenario, cov);

	size_t nb_eq = (scenario==INEQ_EQ_ONLY || scenario==INEQ_HALF_BALL) ? 0 : m;

	CPPUNIT_ASSERT(cov.nb_eq()==nb_eq);
	CPPUNIT_ASSERT(cov.nb_ineq()==nb_ineq);
	CPPUNIT_ASSERT(cov.nb_solution()==nsol);
	CPPUNIT_ASSERT(cov.nb_boundary()==nbnd);
	CPPUNIT_ASSERT(cov.nb_unknown()==nunk+npen);

	for (size_t i=0; i<N; i++) {
		CPPUNIT_ASSERT(
		  (cov.status(i)==CovManifold::SOLUTION && is_sol[i]) ||
		  (cov.status(i)==CovManifold::BOUNDARY && is_bnd[i])||
		  (cov.status(i)==CovManifold::UNKNOWN && (is_unk[i] || is_pen[i]))
		  );
	}

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<nsol; i++) {
		CPPUNIT_ASSERT(cov.solution(i)==b[sol[i]]);

		CPPUNIT_ASSERT((size_t) cov.solution_varset(i).nb_param==n-nb_eq);

		if (scenario==EQ_ONLY || scenario==HALF_BALL) {
			for (size_t j=0; j<n-nb_eq; j++) {
				CPPUNIT_ASSERT(cov.solution_varset(i).param(j)==(int) varset_sol[i][j]);
			}

			IntervalVector unicity=b[sol[i]];
			unicity.inflate(unicity_infl);
			//cout << cov.unicity(i) << " " << unicity << endl;
			CPPUNIT_ASSERT(cov.unicity(i)==unicity);
		}
	}

	for (size_t i=0; i<nbnd; i++) {
		CPPUNIT_ASSERT(cov.boundary(i)==b[bnd[i]]);

		CPPUNIT_ASSERT((size_t) cov.boundary_varset(i).nb_param==n-nb_eq);

		if (scenario==EQ_ONLY || scenario==HALF_BALL) {
			for (size_t j=0; j<n-nb_eq; j++) {
				CPPUNIT_ASSERT(cov.boundary_varset(i).param(j)==(int) varset_bnd[i][j]);
			}
		}
	}

	for (size_t i=0; i<nunk+npen; i++) {
		CPPUNIT_ASSERT(cov.unknown(i)==b[unk_or_pen[i]]);
	}

}

void TestCov::test_covSolverData(ScenarioType scenario, CovSolverData& cov) {


	//	switch(scenario) {
	//	case INEQ_EQ_ONLY:
	//		break;
	//	case INEQ_HALF_BALL:
	//		break;
	//	case EQ_ONLY:
	//		break;
	//	case HALF_BALL:
	//		break;
	//	default:
	//		assert(false);
	//	}
	//

	test_covManifold(scenario, cov);

	for (size_t i=0; i<n; i++) {
		//CPPUNIT_ASSERT(strcmp(cov.var_names[i].c_str(),solver_var_names[i])==0);
		CPPUNIT_ASSERT(cov.var_names()[i] == solver_var_names[i]);
	}

	CPPUNIT_ASSERT(cov.solver_status() == solver_status);
	CPPUNIT_ASSERT(cov.time() == solver_time);
	CPPUNIT_ASSERT(cov.nb_cells() == solver_nb_cells);
	CPPUNIT_ASSERT(cov.nb_pending() == npen);

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<npen; i++) {
		CPPUNIT_ASSERT(cov.pending(i)==b[pen[i]]);
	}

	for (size_t i=0; i<nunk; i++) {
		CPPUNIT_ASSERT(cov.unknown(i)==b[unk[i]]);
	}
}

/*=============================================================================================*/

Cov* TestCov::build_cov(ScenarioType scenario) {
	return new Cov(n);
}

CovList* TestCov::build_covlist(ScenarioType scenario) {
	CovList* cov = new CovList(n);// box dimension

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		cov->add(b[i]);
	}

	return cov;
}

CovIUList* TestCov::build_covIUlist(ScenarioType scenario) {
	CovIUList* cov = new CovIUList(n); // box dimension

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		if ((scenario==INEQ_EQ_ONLY || scenario==INEQ_HALF_BALL) && is_sol[i])
			cov->add_inner(b[i]);
		else
			cov->add_unknown(b[i]);
	}

	return cov;
}

CovIBUList* TestCov::build_covIBUlist(ScenarioType scenario) {
	CovIBUList* cov = new CovIBUList(n); // box dimension

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		if ((scenario==INEQ_EQ_ONLY || scenario==INEQ_HALF_BALL) && is_sol[i])
			cov->add_inner(b[i]);
		else if ((scenario==INEQ_HALF_BALL || scenario==HALF_BALL) && is_bnd[i])
			cov->add_boundary(b[i]);
		else if ((scenario==EQ_ONLY || scenario==HALF_BALL) && is_sol[i])
			cov->add_boundary(b[i]);
		else
			cov->add_unknown(b[i]);
	}

	return cov;
}

CovManifold* TestCov::build_covManifold(ScenarioType scenario) {

	size_t nb_eq = (scenario==INEQ_EQ_ONLY || scenario==INEQ_HALF_BALL) ? 0 : m;

	CovManifold::BoundaryType boundary_type = (scenario==INEQ_EQ_ONLY || scenario==EQ_ONLY) ? CovManifold::EQU_ONLY : CovManifold::HALF_BALL;

	CovManifold* cov = new CovManifold(n, nb_eq, nb_ineq, boundary_type); // box dimension

	vector<IntervalVector> b=boxes();

	int isol=0;
	int ibnd=0;
	for (size_t i=0; i<N; i++) {
		if (is_sol[i]) {
			if (scenario==INEQ_EQ_ONLY || scenario==INEQ_HALF_BALL)
				cov->add_inner(b[i]);
			else {
				BitSet bitset(n);
				IntervalVector unicity=b[sol[isol]];
				unicity.inflate(unicity_infl);

				for (size_t j=0; j<n-m; j++)
					bitset.add(varset_sol[isol][j]);
				cov->add_solution(b[i], unicity, VarSet(n,bitset,false));
				isol++;;
			}
		} else if (is_bnd[i]) {
			if (nb_eq>0 && m<n) {
				BitSet bitset(n);
				for (size_t j=0; j<n-m; j++)
					bitset.add(varset_bnd[ibnd][j]);
				cov->add_boundary(b[i], VarSet(n,bitset,false));
				ibnd++;
			} else
				cov->add_boundary(b[i]);
		}
		else
			cov->add_unknown(b[i]);
	}

	return cov;
}

CovSolverData* TestCov::build_covSolverData(ScenarioType scenario) {
	size_t nb_eq = (scenario==INEQ_EQ_ONLY || scenario==INEQ_HALF_BALL) ? 0 : m;

	CovManifold::BoundaryType boundary_type = (scenario==INEQ_EQ_ONLY || scenario==EQ_ONLY) ? CovManifold::EQU_ONLY : CovManifold::HALF_BALL;

	CovSolverData* cov = new CovSolverData(n, nb_eq, nb_ineq, boundary_type); // box dimension

	vector<IntervalVector> b=boxes();

	int isol=0;
		int ibnd=0;
	for (size_t i=0; i<N; i++) {
		if (is_sol[i]) {
			if (scenario==INEQ_EQ_ONLY || scenario==INEQ_HALF_BALL)
				cov->add_inner(b[i]);
			else {
				BitSet bitset(n);
				IntervalVector unicity=b[sol[isol]];
				unicity.inflate(unicity_infl);

				for (size_t j=0; j<n-m; j++)
					bitset.add(varset_sol[isol][j]);
				cov->add_solution(b[i], unicity, VarSet(n,bitset,false));
				isol++;;
			}
		} else if (is_bnd[i]) {
			if (nb_eq>0 && m<n) {
				BitSet bitset(n);
				for (size_t j=0; j<n-m; j++)
					bitset.add(varset_bnd[ibnd][j]);
				cov->add_boundary(b[i], VarSet(n,bitset,false));
				ibnd++;
			} else
				cov->add_boundary(b[i]);
		} else if (is_pen[i]) {
			cov->add_pending(b[i]);
		} else
			cov->add_unknown(b[i]);
	}

	vector<string> var_names;
	for (size_t i=0; i<n; i++)
		var_names.push_back(solver_var_names[i]);

	cov->var_names() = var_names;
	cov->set_time(solver_time);
	cov->set_solver_status(solver_status);
	cov->set_nb_cells(solver_nb_cells);

	return cov;
}

/*=============================================================================================*/

void TestCov::covfac(ScenarioType scenario) {
	Cov* cov=build_cov(scenario);
	test_cov(scenario, *cov);
	delete cov;
}

void TestCov::read_covfile(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_cov(f,scenario,0);
	f.close();

	Cov cov(filename);
	test_cov(scenario, cov);
	free(filename);
}

void TestCov::write_covfile(ScenarioType scenario) {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	Cov* cov=build_cov(scenario);
	cov->save(tmpname);
	delete cov;

	Cov cov2(tmpname);
	test_cov(scenario, cov2);

	free(tmpname);
}

void TestCov::covlistfac1(ScenarioType scenario) {
	CovList* cov=build_covlist(scenario);
	test_covlist(scenario, *cov);
	delete cov;
}

void TestCov::covlistfac2(ScenarioType scenario) {
	Cov* _cov=build_cov(scenario);
	CovList cov(*_cov);
	test_cov(scenario, cov);
	CPPUNIT_ASSERT(cov.size()==0);
	delete _cov;
}

void TestCov::read_covlistfile1(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_covlist(f,scenario,1);
	f.close();

	CovList cov(filename);
	test_covlist(scenario, cov);
	free(filename);
}

void TestCov::read_covlistfile2(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_cov(f,scenario,0);
	f.close();

	CovList cov(filename);
	test_cov(scenario, cov);
	CPPUNIT_ASSERT(cov.size()==0);
	free(filename);
}

void TestCov::write_covlistfile(ScenarioType scenario) {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovList* cov=build_covlist(scenario);
	cov->save(tmpname);
	delete cov;

	CovList cov2(tmpname);
	test_cov(scenario, cov2);

	free(tmpname);
}

void TestCov::covIUlistfac1(ScenarioType scenario) {
	CovIUList* cov=build_covIUlist(scenario);
	test_covIUlist(scenario, *cov);
	delete cov;
}

void TestCov::covIUlistfac2(ScenarioType scenario) {
	CovList* _cov=build_covlist(scenario);
	CovIUList cov(*_cov);
	test_covlist(scenario, cov);
	CPPUNIT_ASSERT(cov.nb_inner()==0);
	CPPUNIT_ASSERT(cov.nb_unknown()==N);
	delete _cov;
}

void TestCov::read_covIUlistfile1(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_covIUlist(f,scenario,2);
	f.close();

	CovIUList cov(filename);
	test_covIUlist(scenario, cov);
	free(filename);
}

void TestCov::read_covIUlistfile2(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_covlist(f,scenario,1);
	f.close();

	CovIUList cov(filename);
	CPPUNIT_ASSERT(cov.nb_inner()==0);
	CPPUNIT_ASSERT(cov.nb_unknown()==N);

	free(filename);
}

void TestCov::write_covIUlistfile(ScenarioType scenario) {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovIUList* cov=build_covIUlist(scenario);
	cov->save(tmpname);
	delete cov;

	CovIUList cov2(tmpname);
	test_cov(scenario, cov2);

	free(tmpname);
}

void TestCov::covIBUlistfac1(ScenarioType scenario) {
	CovIBUList* cov=build_covIBUlist(scenario);
	test_covIBUlist(scenario, *cov);
	delete cov;
}

void TestCov::covIBUlistfac2(ScenarioType scenario) {
	CovIUList* _cov=build_covIUlist(scenario);
	CovIBUList cov(*_cov);
	test_covIUlist(scenario, cov);
	CPPUNIT_ASSERT(cov.nb_boundary()==0);
	CPPUNIT_ASSERT(cov.nb_unknown()==cov.size() - cov.nb_inner());
	delete _cov;
}

void TestCov::read_covIBUlistfile1(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_covIBUlist(f,scenario,3);
	f.close();

	CovIBUList cov(filename);
	test_covIBUlist(scenario, cov);
	free(filename);
}

void TestCov::read_covIBUlistfile2(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_covIUlist(f,scenario,2);
	f.close();

	CovIBUList cov(filename);
	test_covIUlist(scenario, cov);
	CPPUNIT_ASSERT(cov.nb_boundary()==0);
	CPPUNIT_ASSERT(cov.nb_unknown()==cov.size() - cov.nb_inner());
	free(filename);
}

void TestCov::read_covIBUlistfile3(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_covIBUlist(f,scenario,3,false);
	f.close();

   // check that we still managed to read a CovIUlist...
	CovIBUList cov(filename);
	test_covIUlist(scenario, cov);
	CPPUNIT_ASSERT(cov.nb_boundary()==0);
	CPPUNIT_ASSERT(cov.nb_unknown()==cov.size() - cov.nb_inner());
	free(filename);
}

void TestCov::write_covIBUlistfile(ScenarioType scenario) {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovIBUList* cov=build_covIBUlist(scenario);
	cov->save(tmpname);
	delete cov;

	CovIBUList cov2(tmpname);
	test_cov(scenario, cov2);

	free(tmpname);
}

void TestCov::covManifoldfac1(ScenarioType scenario) {
	CovManifold* cov=build_covManifold(scenario);
	test_covManifold(scenario, *cov);
	delete cov;
}

void TestCov::covManifoldfac2(ScenarioType scenario) {
	CovIBUList* _cov=build_covIBUlist(scenario);
	CovManifold cov(*_cov);

	test_covIBUlist(scenario, cov);
	CPPUNIT_ASSERT(cov.nb_eq()==0);
	CPPUNIT_ASSERT(cov.nb_ineq()==0);

	switch(scenario) {
	case INEQ_EQ_ONLY:
		CPPUNIT_ASSERT(cov.nb_solution()==nsol);
		CPPUNIT_ASSERT(cov.nb_boundary()==0);
	break;
	case INEQ_HALF_BALL:
		CPPUNIT_ASSERT(cov.nb_solution()==nsol);
		CPPUNIT_ASSERT(cov.nb_boundary()==0);
	break;
	case EQ_ONLY:
		CPPUNIT_ASSERT(cov.nb_solution()==0);
		CPPUNIT_ASSERT(cov.nb_boundary()==0);
	break;
	case HALF_BALL:
		CPPUNIT_ASSERT(cov.nb_solution()==0);
		CPPUNIT_ASSERT(cov.nb_boundary()==0);
	break;
	default:
		assert(false);
	}

	delete _cov;
}

void TestCov::read_covManifoldfile1(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_covManifold(f,scenario,4);
	f.close();

	CovManifold cov(filename);
	test_covManifold(scenario, cov);
	free(filename);
}

void TestCov::read_covManifoldfile2(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_covIBUlist(f,scenario,3);
	f.close();

	CovManifold cov(filename);
	test_covIBUlist(scenario, cov);
	CPPUNIT_ASSERT(cov.nb_eq()==0);
	CPPUNIT_ASSERT(cov.nb_ineq()==0);

	switch(scenario) {
	case INEQ_EQ_ONLY:
		CPPUNIT_ASSERT(cov.nb_solution()==nsol);
		CPPUNIT_ASSERT(cov.nb_boundary()==0);
	break;
	case INEQ_HALF_BALL:
		CPPUNIT_ASSERT(cov.nb_solution()==nsol);
		CPPUNIT_ASSERT(cov.nb_boundary()==0);
	break;
	case EQ_ONLY:
		CPPUNIT_ASSERT(cov.nb_solution()==0);
		CPPUNIT_ASSERT(cov.nb_boundary()==0);
	break;
	case HALF_BALL:
		CPPUNIT_ASSERT(cov.nb_solution()==0);
		CPPUNIT_ASSERT(cov.nb_boundary()==0);
	break;
	default:
		assert(false);
	}

	free(filename);
}

void TestCov::write_covManifoldfile(ScenarioType scenario) {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovManifold* cov=build_covManifold(scenario);
	cov->save(tmpname);
	delete cov;

	CovManifold cov2(tmpname);
	test_cov(scenario, cov2);

	free(tmpname);
}

void TestCov::covSolverDatafac1(ScenarioType scenario) {
	CovSolverData* cov=build_covSolverData(scenario);
	test_covSolverData(scenario, *cov);
	delete cov;
}

void TestCov::covSolverDatafac2(ScenarioType scenario) {
	CovManifold* _cov=build_covManifold(scenario);
	CovSolverData cov(*_cov);
	test_covManifold(scenario, cov);
	CPPUNIT_ASSERT(cov.var_names().empty());
	CPPUNIT_ASSERT(cov.solver_status() == Solver::SUCCESS);
	CPPUNIT_ASSERT(cov.time() == -1);
	CPPUNIT_ASSERT(cov.nb_cells() == 0);
	CPPUNIT_ASSERT(cov.nb_pending() == 0);

	delete _cov;
}


void TestCov::read_covSolverDatafile1(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_covSolverData(f,scenario,5);
	f.close();

	CovSolverData cov(filename);
	test_covSolverData(scenario, cov);
	free(filename);
}

void TestCov::read_covSolverDatafile2(ScenarioType scenario) {
	ofstream f;
	char* filename=open_file(f);
	write_covManifold(f,scenario,4);
	f.close();

	CovSolverData cov(filename);
	test_covManifold(scenario, cov);
	CPPUNIT_ASSERT(cov.var_names().empty());
	CPPUNIT_ASSERT(cov.solver_status() == Solver::SUCCESS);
	CPPUNIT_ASSERT(cov.time() == -1);
	CPPUNIT_ASSERT(cov.nb_cells() == 0);
	CPPUNIT_ASSERT(cov.nb_pending() == 0);

	free(filename);
}

void TestCov::write_covSolverDatafile(ScenarioType scenario) {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovSolverData* cov=build_covSolverData(scenario);
	cov->save(tmpname);
	delete cov;

	CovSolverData cov2(tmpname);
	test_cov(scenario, cov2);

	free(tmpname);
}
