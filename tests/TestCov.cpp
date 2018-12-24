/* ============================================================================
 * I B E X - TestCov
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Nov 18, 2018
 * ---------------------------------------------------------------------------- */

#include "TestCov.h"
#include "ibex_Cov.h"
#include "utils.h"

#include <fstream>
#include <string.h>
#include <stdlib.h>

using namespace std;


uint32_t TestCov::inner[ni]             = { 0, 2, 3 };
bool TestCov::is_inner[N]               = { 1, 0, 1, 1, 0, 0, 0, 0, 0, 0 };

uint32_t TestCov::sol[nsol]             = { 1, 8 };
bool TestCov::is_sol[N]                 = { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0 };

uint32_t TestCov::unk[nunk]             = { 4 };
bool TestCov::is_unk[N]                 = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 };

uint32_t TestCov::pen[npen]             = { 5 };
bool TestCov::is_pen[N]                 = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 };

uint32_t TestCov::bnd[nbnd]             = { 6, 7, 9 };
bool TestCov::is_bnd[N]                 = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 1 };

uint32_t TestCov::not_inner[N-ni]       = { 1, 4, 5, 6, 7, 8, 9 };
uint32_t TestCov::bnd_or_sol[nbnd+nsol] = { 1, 6, 7, 8, 9 };
uint32_t TestCov::unk_or_pen[nunk+npen] = { 4, 5 };

uint32_t TestCov::varset[nsol][n-m]     = { {0, 1}, {1, 2} };

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

void TestCov::write_cov(ofstream& f, unsigned int level) {
	write(f,"IBEX COVERING FILE "); // signature
	write(f,(uint32_t) 1);          // format version
	write(f,(uint32_t) level);      // subformat level
	for (unsigned int i=0; i<=level; i++)
		write(f, (uint32_t) 0);
	write(f,(uint32_t) n);          // box dimension
}

void TestCov::write_covlist(ofstream& f, unsigned int level) {
	write_cov(f,level);
	write(f,(uint32_t) N);  // number of boxes

	vector<IntervalVector> b = boxes();

	for (size_t i=0; i<N; i++) {
		for (size_t j=0; j<n; j++) {
			write(f,b[i][j].lb());
			write(f,b[i][j].ub());
		}
	}
}

void TestCov::write_covIUlist(ofstream& f, unsigned int level) {
	write_covlist(f,level);
	write(f, (uint32_t) ni); // number of inner boxes
	for (size_t i=0; i<ni; i++) {
		write(f, (uint32_t) inner[i]); // ith inner box
	}
}

void TestCov::write_covIBUlist(ofstream& f, unsigned int level) {
	write_covIUlist(f,level);
	write(f, (uint32_t) (nbnd+nsol)); // total number of boundary boxes
	for (size_t i=0; i<nbnd; i++) {
		write(f, (uint32_t) bnd[i]);
	}
	for (size_t i=0; i<nsol; i++) {
		write(f, (uint32_t) sol[i]);
	}
}

void TestCov::write_covManifold(ofstream& f, unsigned int level) {
	write_covIBUlist(f,level);
	write(f, (uint32_t) m); // number of equalities
	write(f, (uint32_t) nb_ineq); // number of inequalities
	write(f, (uint32_t) nsol);


	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<nsol; i++) {
		write(f, (uint32_t) sol[i]);
		for (size_t j=0; j<n-m; j++) {
			write(f, varset[i][j]);
		}
		IntervalVector unicity=b[sol[i]];
		unicity.inflate(unicity_infl);
		for (size_t j=0; j<n; j++) {
			write(f,unicity[j].lb());
			write(f,unicity[j].ub());
		}
	}
}

void TestCov::write_covSolverData(ofstream& f, unsigned int level) {
	write_covManifold(f,level);

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

void TestCov::test_cov(Cov& cov) {
	CPPUNIT_ASSERT(cov.n==n);
}

void TestCov::test_covlist(CovList& cov) {
	test_cov(cov);

	CPPUNIT_ASSERT(cov.size()==N);

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		cout << cov[i] << endl;
		CPPUNIT_ASSERT(cov[i]==b[i]);
	}
}

void TestCov::test_covIUlist(CovIUList& cov) {
	test_covlist(cov);

	CPPUNIT_ASSERT(cov.nb_inner()==ni);
	CPPUNIT_ASSERT(cov.nb_unknown()==N-ni);

	for (size_t i=0; i<N; i++) {
		CPPUNIT_ASSERT((cov.status(i)==CovIUList::INNER && is_inner[i]) ||
		(cov.status(i)==CovIUList::UNKNOWN && (!is_inner[i])) );
		if (is_inner[i]) {
			CPPUNIT_ASSERT(cov.is_inner(i));
			CPPUNIT_ASSERT(!cov.is_unknown(i));
		} else {
			CPPUNIT_ASSERT(!cov.is_inner(i));
			CPPUNIT_ASSERT(cov.is_unknown(i));
		}
	}

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<ni; i++) {
		CPPUNIT_ASSERT(cov.inner(i)==b[inner[i]]);
	}

	for (size_t i=0; i<N-ni; i++) {
		CPPUNIT_ASSERT(cov.unknown(i)==b[not_inner[i]]);
	}

}

void TestCov::test_covIBUlist(CovIBUList& cov) {
	test_covIUlist(cov);

	CPPUNIT_ASSERT(cov.nb_boundary()==nbnd+nsol);
	CPPUNIT_ASSERT(cov.nb_unknown()==nunk+npen);

	for (size_t i=0; i<N; i++) {
		CPPUNIT_ASSERT(
		  (cov.status(i)==CovIBUList::INNER && is_inner[i]) ||
		  (cov.status(i)==CovIBUList::UNKNOWN && (is_unk[i] || is_pen[i])) ||
		  (cov.status(i)==CovIBUList::BOUNDARY && (is_bnd[i] || is_sol[i]))
		);

		if (is_bnd[i] || is_sol[i]) {
			CPPUNIT_ASSERT(cov.is_boundary(i));
			CPPUNIT_ASSERT(!cov.is_unknown(i));
		} else if (!is_inner[i]) {
			CPPUNIT_ASSERT(!cov.is_boundary(i));
			CPPUNIT_ASSERT(cov.is_unknown(i));
		}
	}

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<nbnd+nsol; i++) {
		CPPUNIT_ASSERT(cov.boundary(i)==b[bnd_or_sol[i]]);
	}

	for (size_t i=0; i<nunk+npen; i++) {
		CPPUNIT_ASSERT(cov.unknown(i)==b[unk_or_pen[i]]);
	}
}

void TestCov::test_covManifold(CovManifold& cov) {
	test_covIBUlist(cov);
	CPPUNIT_ASSERT(cov.m==m);
	CPPUNIT_ASSERT(cov.nb_ineq==nb_ineq);
	CPPUNIT_ASSERT(cov.nb_solution()==nsol);
	CPPUNIT_ASSERT(cov.nb_boundary()==nbnd);

	for (size_t i=0; i<N; i++) {
		CPPUNIT_ASSERT(
		  (cov.status(i)==CovManifold::INNER && is_inner[i]) ||
		  (cov.status(i)==CovManifold::UNKNOWN && (is_unk[i] || is_pen[i])) ||
		  (cov.status(i)==CovManifold::BOUNDARY && is_bnd[i])||
		  (cov.status(i)==CovManifold::SOLUTION && is_sol[i])
		  );
	}

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<nbnd; i++) {
		CPPUNIT_ASSERT(cov.boundary(i)==b[bnd[i]]);
	}

	for (size_t i=0; i<nsol; i++) {
		CPPUNIT_ASSERT(cov.solution(i)==b[sol[i]]);

		CPPUNIT_ASSERT(cov.varset(i).nb_param==n-m);

		for (size_t j=0; j<n-m; j++) {
			CPPUNIT_ASSERT(cov.varset(i).param(j)==(int) varset[i][j]);
		}

		IntervalVector unicity=b[sol[i]];
		unicity.inflate(unicity_infl);
		cout << cov.unicity(i) << " " << unicity << endl;
		CPPUNIT_ASSERT(cov.unicity(i)==unicity);
	}
}

void TestCov::test_covSolverData(CovSolverData& cov) {

	test_covManifold(cov);

	for (size_t i=0; i<n; i++) {
		//CPPUNIT_ASSERT(strcmp(cov.var_names[i].c_str(),solver_var_names[i])==0);
		CPPUNIT_ASSERT(cov.var_names[i] == solver_var_names[i]);
	}

	CPPUNIT_ASSERT(cov.solver_status == solver_status);
	CPPUNIT_ASSERT(cov.time == solver_time);
	CPPUNIT_ASSERT(cov.nb_cells == solver_nb_cells);
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

Cov* TestCov::build_cov() {
	return new Cov(n);
}

CovList* TestCov::build_covlist() {
	CovList* cov = new CovList(n);// box dimension

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		cov->add(b[i]);
	}

	return cov;
}

CovIUList* TestCov::build_covIUlist() {
	CovIUList* cov = new CovIUList(n); // box dimension

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		if (is_inner[i]) cov->add_inner(b[i]);
		else cov->add_unknown(b[i]);
	}

	return cov;
}

CovIBUList* TestCov::build_covIBUlist() {
	CovIBUList* cov = new CovIBUList(n); // box dimension

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		if (is_inner[i])
			cov->add_inner(b[i]);
		else if (is_bnd[i] || is_sol[i])
			cov->add_boundary(b[i]);
		else
			cov->add_unknown(b[i]);
	}

	return cov;
}

CovManifold* TestCov::build_covManifold() {
	CovManifold* cov = new CovManifold(n, m, nb_ineq); // box dimension

	vector<IntervalVector> b=boxes();

	int nsol=0;
	for (size_t i=0; i<N; i++) {
		if (is_inner[i])
			cov->add_inner(b[i]);
		else if (is_bnd[i])
			cov->add_boundary(b[i]);
		else if (is_sol[i]) {
			BitSet bitset(n);
			IntervalVector unicity=b[sol[nsol]];
			unicity.inflate(unicity_infl);

			for (size_t j=0; j<n-m; j++)
				bitset.add(varset[nsol][j]);
			cov->add_solution(b[i], unicity, VarSet(n,bitset,false));
			nsol++;;
		} else
			cov->add_unknown(b[i]);
	}

	return cov;
}

CovSolverData* TestCov::build_covSolverData() {
	CovSolverData* cov = new CovSolverData(n, m, nb_ineq); // box dimension

	vector<IntervalVector> b=boxes();

	int nsol=0;
	for (size_t i=0; i<N; i++) {
		if (is_inner[i])
			cov->add_inner(b[i]);
		else if (is_bnd[i])
			cov->add_boundary(b[i]);
		else if (is_sol[i]) {
			BitSet bitset(n);
			IntervalVector unicity=b[sol[nsol]];
			unicity.inflate(unicity_infl);

			for (size_t j=0; j<n-m; j++)
				bitset.add(varset[nsol][j]);
			cov->add_solution(b[i], unicity, VarSet(n,bitset,false));
			nsol++;;
		} else if (is_pen[i]) {
			cov->add_pending(b[i]);
		} else
			cov->add_unknown(b[i]);
	}

	vector<string> var_names;
	for (size_t i=0; i<n; i++)
		var_names.push_back(solver_var_names[i]);

	cov->var_names = var_names;
	cov->time = solver_time;
	cov->solver_status = solver_status;
	cov->nb_cells = solver_nb_cells;

	return cov;
}

/*=============================================================================================*/

void TestCov::covfac() {
	Cov* cov=build_cov();
	test_cov(*cov);
	delete cov;
}

void TestCov::read_covfile() {
	ofstream f;
	char* filename=open_file(f);
	write_cov(f,0);
	f.close();

	Cov cov(filename);
	test_cov(cov);
	free(filename);
}

void TestCov::write_covfile() {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	Cov* cov=build_cov();
	cov->save(tmpname);
	delete cov;

	Cov cov2(tmpname);
	test_cov(cov2);

	free(tmpname);
}

void TestCov::covlistfac() {
	CovList* cov=build_covlist();
	test_covlist(*cov);
	delete cov;
}

void TestCov::read_covlistfile1() {
	ofstream f;
	char* filename=open_file(f);
	write_covlist(f,1);
	f.close();

	CovList cov(filename);
	test_covlist(cov);
	free(filename);
}

void TestCov::read_covlistfile2() {
	ofstream f;
	char* filename=open_file(f);
	write_cov(f,0);
	f.close();

	CovList cov(filename);
	test_cov(cov);
	CPPUNIT_ASSERT(cov.size()==0);
	free(filename);
}

void TestCov::write_covlistfile() {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovList* cov=build_covlist();
	cov->save(tmpname);
	delete cov;

	CovList cov2(tmpname);
	test_cov(cov2);

	free(tmpname);
}

void TestCov::covIUlistfac() {
	CovIUList* cov=build_covIUlist();
	test_covIUlist(*cov);
	delete cov;
}

void TestCov::read_covIUlistfile1() {
	ofstream f;
	char* filename=open_file(f);
	write_covIUlist(f,2);
	f.close();

	CovIUList cov(filename);
	test_covIUlist(cov);
	free(filename);
}

void TestCov::read_covIUlistfile2() {
	ofstream f;
	char* filename=open_file(f);
	write_covlist(f,1);
	f.close();

	CovIUList cov(filename);
	test_covlist(cov);

	CPPUNIT_ASSERT(cov.nb_inner()==0);
	CPPUNIT_ASSERT(cov.nb_unknown()==N);

	free(filename);
}

void TestCov::write_covIUlistfile() {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovIUList* cov=build_covIUlist();
	cov->save(tmpname);
	delete cov;

	CovIUList cov2(tmpname);
	test_cov(cov2);

	free(tmpname);
}

void TestCov::covIBUlistfac() {
	CovIBUList* cov=build_covIBUlist();
	test_covIBUlist(*cov);
	delete cov;
}

void TestCov::read_covIBUlistfile1() {
	ofstream f;
	char* filename=open_file(f);
	write_covIBUlist(f,3);
	f.close();

	CovIBUList cov(filename);
	test_covIBUlist(cov);
	free(filename);
}

void TestCov::read_covIBUlistfile2() {
	ofstream f;
	char* filename=open_file(f);
	write_covIUlist(f,2);
	f.close();

	CovIBUList cov(filename);
	test_covIUlist(cov);
	CPPUNIT_ASSERT(cov.nb_boundary()==0);
	CPPUNIT_ASSERT(cov.nb_unknown()==cov.size() - cov.nb_inner());
	free(filename);
}


void TestCov::write_covIBUlistfile() {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovIBUList* cov=build_covIBUlist();
	cov->save(tmpname);
	delete cov;

	CovIBUList cov2(tmpname);
	test_cov(cov2);

	free(tmpname);
}

void TestCov::covManifoldfac() {
	CovManifold* cov=build_covManifold();
	test_covManifold(*cov);
	delete cov;
}

void TestCov::read_covManifoldfile1() {
	ofstream f;
	char* filename=open_file(f);
	write_covManifold(f,4);
	f.close();

	CovManifold cov(filename);
	test_covManifold(cov);
	free(filename);
}

void TestCov::read_covManifoldfile2() {
	ofstream f;
	char* filename=open_file(f);
	write_covIBUlist(f,3);
	f.close();

	CovManifold cov(filename);
	test_covIBUlist(cov);
	CPPUNIT_ASSERT(cov.m==0);
	CPPUNIT_ASSERT(cov.nb_ineq==0);
	CPPUNIT_ASSERT(cov.nb_solution()==0);
	CPPUNIT_ASSERT(cov.nb_boundary()==cov.CovIBUList::nb_boundary());

	free(filename);
}

void TestCov::write_covManifoldfile() {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovManifold* cov=build_covManifold();
	cov->save(tmpname);
	delete cov;

	CovManifold cov2(tmpname);
	test_cov(cov2);

	free(tmpname);
}

void TestCov::covSolverDatafac() {
	CovSolverData* cov=build_covSolverData();
	test_covSolverData(*cov);
	delete cov;
}

void TestCov::read_covSolverDatafile1() {
	ofstream f;
	char* filename=open_file(f);
	write_covSolverData(f,5);
	f.close();

	CovSolverData cov(filename);
	test_covSolverData(cov);
	free(filename);
}

void TestCov::read_covSolverDatafile2() {
	ofstream f;
	char* filename=open_file(f);
	write_covManifold(f,4);
	f.close();

	CovSolverData cov(filename);
	test_covManifold(cov);
	CPPUNIT_ASSERT(cov.var_names.empty());
	CPPUNIT_ASSERT(cov.solver_status == Solver::SUCCESS);
	CPPUNIT_ASSERT(cov.time == -1);
	CPPUNIT_ASSERT(cov.nb_cells == 0);
	CPPUNIT_ASSERT(cov.nb_pending() == 0);

	free(filename);
}

void TestCov::write_covSolverDatafile() {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovSolverData* cov=build_covSolverData();
	cov->save(tmpname);
	delete cov;

	CovSolverData cov2(tmpname);
	test_cov(cov2);

	free(tmpname);
}


