//============================================================================
//                                  I B E X                                   
// File        : ibex_Linear.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 18, 2012
// Last Update : Aug 29, 2013
//============================================================================

#include "ibex_Linear.h"
#include "ibex_LinearException.h"

#include <math.h>
#include <float.h>
#include <stack>

#define TOO_LARGE 1e30
#define TOO_SMALL 1e-10

using namespace std;

namespace ibex {

namespace {
double _mig(double x)          { return fabs(x); }
double _mig(const Interval& x) { return x.mig(); }
bool _zero(double x)           { return x==0; }
bool _zero(const Interval& x)  { return x.contains(0); }

// S=scalar (double or Interval)
// M=matrix (Matrix or IntervalMatrix)
template<typename S, class M>
void LU(const M& A, M& LU, int* p) {

	int m = (A.nb_rows());
	int n = (A.nb_cols());
	assert(m == (LU.nb_rows()) && n == (LU.nb_cols()));

	// check the matrix has no "infinite" values
	for (int i=0; i<m; i++) {
		for (int j=0; j<n; j++) {
			if (_mig(A[i][j])>=TOO_LARGE) throw SingularMatrixException();
		}
	}

	LU = A;
	for (int i=0; i<m; i++) p[i]=i;

	// LU computation
	S pivot;

	int min_m_n=m<n? m : n;
	for (int i=0; i<min_m_n; i++) {
		// partial pivot search
		int swap = i;
		pivot = LU[p[i]][i];
		for (int j=i+1; j<m; j++) {
			if (_mig(LU[p[j]][i])>_mig(LU[p[swap]][i])) swap=j;
		}
		int tmp = p[i];
		p[i] = p[swap];
		p[swap] = tmp;
		// --------------------------------------------------

		pivot = LU[p[i]][i];

		if (_zero(pivot)) {
			if (i<min_m_n-1) throw SingularMatrixException();
			else // in this case, the matrix is not full-rank only if all the remaining columns are zero
				for (int k=i+1; k<n; k++) {
					if (!_zero(LU[p[i]][k])) return; // ok, full rank
				}
			throw SingularMatrixException();
		}
		if (_mig(1/pivot)>=TOO_LARGE) throw SingularMatrixException();

		for (int j=i+1; j<m; j++) {
			for (int k=i+1; k<n; k++) {
				//cout << LU[p[j]][k] << " " << LU[p[j]][i] << " " << pivot << endl;
				LU[p[j]][k]-=(LU[p[j]][i]/pivot)*LU[p[i]][k];
			}
			LU[p[j]][i] = LU[p[j]][i]/pivot;
		}
	}
}

template<typename S, class M>
void LU(const M& A, M& LU, int* pr, int* pc) {

	int m = (A.nb_rows());
	int n = (A.nb_cols());
	assert(m == (LU.nb_rows()) && n == (LU.nb_cols()));

	// check the matrix has no "infinite" values
	for (int i=0; i<m; i++) {
		for (int j=0; j<n; j++) {
			// TODO: change this exception (rectangular case)
			if (_mig(A[i][j])>=TOO_LARGE) throw SingularMatrixException();
		}
	}

	LU = A;
	for (int i=0; i<m; i++) pr[i]=i;
	for (int j=0; j<n; j++) pc[j]=j;

	// LU computation
	S pivot;
	int min_m_n=m<n? m : n;
	for (int i=0; i<min_m_n; i++) {
		//cout << "LU=" << LU << endl;
		// complete pivot search
		int swapR = i;
		int swapC = i;
		pivot = LU[pr[i]][i];
		for (int j=i; j<m; j++) {
			for (int k=i; k<n; k++) {
				if (_mig(LU[pr[j]][pc[k]])>_mig(LU[pr[swapR]][pc[swapC]])) { swapR=j; swapC=k; }
			}
		}
		int tmp = pr[i];
		pr[i] = pr[swapR];
		pr[swapR] = tmp;

		tmp = pc[i];
		pc[i] = pc[swapC];
		pc[swapC] = tmp;
		// --------------------------------------------------

		pivot = LU[pr[i]][pc[i]];
		//cout << "i=" << i << " pivot=" << pivot << endl;
		if (_zero(pivot)) {
			if (i<min_m_n-1) throw SingularMatrixException();
			else // in this case, the matrix is not full-rank only if all the remaining columns are zero
				for (int k=i+1; k<n; k++) {
					//cout << "lu[" << pr[i] << "," << pc[k] << "]=" << LU[pr[i]][pc[k]] << endl;
					if (!_zero(LU[pr[i]][pc[k]])) return; // ok, full rank
				}
			throw SingularMatrixException();
		}
		if (_mig(1/pivot)>=TOO_LARGE) throw SingularMatrixException();

		for (int j=i+1; j<m; j++) {
			for (int k=i+1; k<n; k++) {
				//cout << LU[p[j]][k] << " " << LU[p[j]][i] << " " << pivot << endl;
				LU[pr[j]][pc[k]]-=(LU[pr[j]][pc[i]]/pivot)*LU[pr[i]][pc[k]];
			}
			LU[pr[j]][pc[i]] = LU[pr[j]][pc[i]]/pivot;
		}
	}
}

template<class M, class V>
void LU_solve(const M& LU, const int* p, const V& b, V& x) {
    int n = LU.nb_rows();
    assert(n == LU.nb_cols());
    assert(n == b.size() && n == x.size());

    // solve LX=b
    x[0] = b[p[0]];
    for (int i = 1; i < n; ++i) {
        x[i] = b[p[i]];
        for (int j = 0; j < i; ++j) {
            x[i] -= LU[p[i]][j] * x[j];
        }
    }

    // solve Uy=x
    if (_mig(LU[p[n - 1]][n - 1]) <= TOO_SMALL)
        throw SingularMatrixException();
    x[n - 1] /= LU[p[n - 1]][n - 1];
    for (int i = n - 2; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            x[i] -= LU[p[i]][j] * x[j];
        }
        if (_mig(LU[p[i]][i]) <= TOO_SMALL)
            throw SingularMatrixException();
        x[i] /= LU[p[i]][i];
    }
}

} // end anonymous namespace

void real_LU(const Matrix& A, Matrix& _LU, int* p) {
	LU<double,Matrix>(A,_LU,p);
}

void real_LU(const Matrix& A, Matrix& _LU, int* pr, int* pc) {
	LU<double,Matrix>(A,_LU,pr,pc);
}

void interval_LU(const IntervalMatrix& A, IntervalMatrix& _LU, int* p) {
	LU<Interval,IntervalMatrix>(A,_LU,p);
}

void interval_LU(const IntervalMatrix& A, IntervalMatrix& _LU, int* pr, int* pc) {
	LU<Interval,IntervalMatrix>(A,_LU,pr,pc);
}

void real_LU_solve(const Matrix &LU, const int* p, const Vector& b, Vector& x) {
    LU_solve<Matrix, Vector>(LU, p, b, x);
}

void interval_LU_solve(const IntervalMatrix& LU, const int *p, const IntervalVector& b, IntervalVector& x) {
    LU_solve<IntervalMatrix, IntervalVector>(LU, p, b, x);
}

void real_inverse(const Matrix& A, Matrix& invA) {
	int n = (A.nb_rows());

	Matrix LU(n,n);
	int* p= new int[n];

	try {
		real_LU(A, LU, p);

		Vector b(n,0.0);
		Vector x(n);

		for (int i=0; i<n; i++) {
			b[i]=1;
			real_LU_solve(LU, p, b, x);
			invA.set_col(i, x);
			b[i]=0;
		}
	}
	catch(SingularMatrixException& e) {
		delete[] p;
		throw e;
	}
	delete[] p;
}

Matrix real_inverse(const Matrix& A) {
	Matrix Minv(A.nb_rows(), A.nb_cols());
	real_inverse(A,Minv);
	return Minv;
}

void neumaier_inverse(const IntervalMatrix& A, IntervalMatrix& invA) {
    int n = A.nb_rows();
    if (n != A.nb_cols()) throw NotSquareMatrixException();

    Vector u(n, 1);
    Matrix C(n, n);
    real_inverse(A.mid(), C); // throw SingularMatrixException
    double beta = infinite_norm(C * A - Matrix::eye(n));
    if (beta >= 1)
        throw SingularMatrixException();
    Vector w(n);
    for (int j = 0; j < n; ++j) {
        w[j] = infinite_norm(C.col(j));
    }
    Matrix uw = outer_product(u, w);
    IntervalMatrix E(n, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            E[i][j] = Interval(-uw[i][j], uw[i][j]);
    invA = C + beta / (1 - beta) * E;
}

void precond_rohn_inverse(const IntervalMatrix& A, IntervalMatrix& invA) {
    //Th. 10.2, not the best
    int n = A.nb_rows();
    assert(n == A.nb_cols());
    Matrix delta = (A - Matrix::eye(n)).ub();
    if (infinite_norm(delta) >= 1) {
        throw SingularMatrixException();
    }
    Matrix invM = Matrix::eye(n) - delta;
    Matrix M(n, n);
    real_inverse(invM, M);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                invA[i][j] = Interval(-M[i][j], M[i][j]);
            } else {
                invA[i][j] = Interval(M[i][j] / (2 * M[i][j] - 1), M[i][j]);
            }
        }
    }
}

bool full_rank(const IntervalMatrix& A) {
	int *pr = new int[A.nb_rows()]; // ignored
	int *pc = new int[A.nb_cols()]; // ignored
	 try {
		 IntervalMatrix LU(A);

		 interval_LU(A,LU,pr,pc);

		 delete [] pr;
		 delete [] pc;
		 return true;
	 } catch(SingularMatrixException& e) {
		 // means in particular that we could not extract an
		 // invertible m*m submatrix
		 delete [] pr;
		 delete [] pc;
		 return false;
	 }
}

void precond(IntervalMatrix& A) {
	int n=(A.nb_rows());
	assert(n == A.nb_cols()); //throw NotSquareMatrixException();  // not well-constraint problem

	Matrix C(n,n);
	try { real_inverse(A.mid(), C); }
	catch (SingularMatrixException&) {
		try { real_inverse(A.lb(), C); }
		catch (SingularMatrixException&) {
			real_inverse(A.ub(), C);
		}
	}

	A = C*A;
}

void precond(IntervalMatrix& A, IntervalVector& b) {
	int n=(A.nb_rows());
	assert(n == A.nb_cols()); //throw NotSquareMatrixException();  // not well-constraint problem
	assert(n == b.size());

	Matrix C(n,n);
	try { real_inverse(A.mid(), C); }
	catch (SingularMatrixException&) {
		try { real_inverse(A.lb(), C); }
		catch (SingularMatrixException&) {
			real_inverse(A.ub(), C);
		}
	}

	//   cout << "A=" << (A.nb_cols()) << "x" << (A.nb_rows()) << "  " << "b=" << (b.size()) << "  " << "C="
	//        << (C.nb_cols()) << "x" << (C.nb_rows()) << endl;
	//cout << "C=" << C << endl;
	A = C*A;
	b = C*b;
}

void gauss_seidel(const IntervalMatrix& A, const IntervalVector& b, IntervalVector& x, double ratio) {
	int m=(A.nb_rows());
	int n=(A.nb_cols());
	assert(x.size()==n);
	assert(b.size()==m);

	double red;
	Interval old, proj, tmp;
	int i;

	do {
		red = 0;
		for (int r=0; r<m; r++) {
			i=r % n; // in case m>n
			old = x[i];
			proj = b[r];

			for (int j=0; j<n; j++)	if (j!=i) proj -= A[r][j]*x[j];
			tmp=A[r][i];

			bwd_mul(proj,tmp,x[i]);

			if (x[i].is_empty()) { x.set_empty(); return; }

			double gain=old.rel_distance(x[i]);
			if (gain>red) red=gain;
		}
	} while (red >= ratio);
}

bool inflating_gauss_seidel(const IntervalMatrix& A, const IntervalVector& b, IntervalVector& x, double min_dist, double mu_max) {
	int n=(A.nb_rows());
	assert(n == (A.nb_cols()));
	assert(n == (x.size()) && n == (b.size()));
	assert(min_dist>0);
	//cout << " ====== inflating Gauss-Seidel ========= " << endl;
	//double red;
	IntervalVector xold(n);
	Interval proj;
	double d=DBL_MAX; // Hausdorff distances between 2 iterations
	double dold;
	double mu; // ratio of dist(x_k,x_{k-1)) / dist(x_{k-1},x_{k-2}).
	do {
		dold = d;
		xold = x;
		for (int i=0; i<n; i++) {
			proj = b[i];
			for (int j=0; j<n; j++)	if (j!=i) proj -= A[i][j]*x[j];
			x[i] = proj/A[i][i];
		}
		d=distance(xold,x);
		mu=d/dold;
		//cout << "  x=" << x << " d=" << d << " mu=" << mu << endl;
	} while (mu<mu_max && d>min_dist);
	//cout << " ======================================= " << endl;
	return (mu<mu_max);

}

void hansen_bliek(const IntervalMatrix& A, const IntervalVector& B, IntervalVector& x) {
	int n=A.nb_rows();
	assert(n == A.nb_cols()); // throw NotSquareMatrixException();
	assert(n == x.size() && n == B.size());

	Matrix Id(n,n);
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++) {
			Id[i][j]   = i==j;
		}

	IntervalMatrix radA(A-Id);
	Matrix InfA(Id-abs(radA.lb()));
	Matrix M(n,n);

	real_inverse(InfA, M);  // may throw SingularMatrixException...

	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
			if (! (M[i][j]>=0.0)) throw NotInversePositiveMatrixException();

	Vector b(B.mid());
	Vector delta = (B.ub())-b;

	Vector xstar = M * (abs(b)+delta);

	double xtildek, xutildek, nuk, max, min;

	for (int k=0; k<n; k++) {
		xtildek = (b[k]>=0) ? xstar[k] : xstar[k] + 2*M[k][k]*b[k];
		xutildek = (b[k]<=0) ? -xstar[k] : -xstar[k] + 2*M[k][k]*b[k];

		nuk = 1/(2*M[k][k]-1);
		max = nuk*xutildek;
		if (max < 0) max = 0;

		min = nuk*xtildek;
		if (min > 0) min = 0;

		/* compute bounds of x(k) */
		if (xtildek >= max) {
			if (xutildek <= min) x[k] = Interval(xutildek,xtildek);
			else x[k] = Interval(max,xtildek);
		} else {
			if (xutildek <= min) x[k] = Interval(xutildek,min);
			else { x.set_empty(); return; }
		}
	}
}

Interval det(const IntervalMatrix& A) {
	int n=A.nb_cols();
	if(n!=A.nb_rows())
		throw NotSquareMatrixException();

	IntervalMatrix LU(A);
	int *p = new int[n];
	interval_LU(A,LU,p);
	Interval res=LU[p[0]][0];
	for (int i=1; i<n; i++) {
		res*=LU[p[i]][i];
	}
	// we need now to multiply by the sign
	// of the permutation. The sign is +1
	// iff the permutation is decomposable
	// into an even number of transpositions:
	int i=0;
	double sign=1;
	while (i<n) {
		if (p[i]==i) { i++; continue; }
		// warning: we modify p (but OK since it
		// is not used anymore)
		sign=-sign;
		int tmp=p[i];
		p[i]=p[tmp];
		p[tmp]=tmp;
	}

	return sign*res;
}

bool is_posdef_sylvester(const IntervalMatrix& A) {
    int n = A.nb_cols();

    try {
    	for (int i=0; i<n-1; i++) {
    		if (det(A.submatrix(0, i, 0, i)).lb()<0) return false;
    	}

    	if (det(A).lb()<0) return false;

    	return true;
    } catch(SingularMatrixException&) {
    	return false;
    }
}

bool is_posdef_rohn(const IntervalMatrix& A) {
	int n=A.nb_rows();
	if (A.nb_cols()!=n) throw NotSquareMatrixException();

	Matrix midA=A.mid();
	Matrix radA=A.rad();
	Matrix Tz(n,n);

	// the integer is the position in the vector where the sign
	// has to be changed.
	stack<pair<Vector,int> > s;

	// The first index is 1 to break symmetry (the sign of index 0
	// is always 1).
	s.push(make_pair(Vector::ones(n),1));

	// initial test with z=(1,...,1)
	if (!is_posdef_sylvester(midA - radA)) return false;

	while (!s.empty()) {
		pair<Vector,int> p=s.top();
		s.pop();

		Vector& v=p.first;
		int i=p.second;

		if (i<n-1) s.push(make_pair(v,i+1));

		v[i]=-1;
		Tz=Matrix::diag(v);
		if (!is_posdef_sylvester(midA - Tz*radA*Tz)) return false;

		if (i<n-1) s.push(make_pair(v,i+1));
	}

	return true;
}

bool is_diagonal_dominant(const IntervalMatrix& A) {
	double s;
	for (int i=0; i<A.nb_rows(); i++) {
    	s=0;
    	for (int j=0; j<A.nb_cols(); j++) {
    		if (j!=i) s+=A[i][j].mag();
    	}
    	if (s>=A[i][i].mig()) return false;
    }
    return true;
}

} // end namespace

