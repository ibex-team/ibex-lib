/* ============================================================================
 * I B E X - Template Domain Matrix implementation
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 28, 2014
 * ---------------------------------------------------------------------------- */


#ifndef __IBEX_TEMPLATE_MATRIX_H__
#define __IBEX_TEMPLATE_MATRIX_H__
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_IntervalMatrix.h"
#include "ibex_Random.h"

#include <cassert>

namespace ibex {

namespace { // to create anonymous functions

template<class M>
bool _is_unboundedM(const M& m) {
	if (m.is_empty()) return false;
	for (int i=0; i<m.nb_rows(); i++)
		if (m[i].is_unbounded()) return true;
	return false;
}

template<class V1,class V2>
bool _is_subsetM(const V1& v, const V2& x) {
	if (v.is_empty()) return true;
	for (int i=0; i<v.nb_rows(); i++)
		if (!v[i].is_subset(x[i])) return false;
	return true;
}

template<class V1,class V2>
bool _is_strict_subsetM(const V1& v, const V2& x) {
	if (v.is_empty() && !x.is_empty()) return true;
	if (x.is_empty()) return false;
	for (int i=0; i<v.nb_rows(); i++)
		if (!v[i].is_strict_subset(x[i])) return false;
	return true;
}

template<class M>
inline M _submatrix(const M& m, int row_start_index, int row_end_index, int col_start_index, int col_end_index) {
	assert(row_start_index>=0 && row_start_index<m.nb_rows());
	assert(row_end_index>=0 && row_end_index<m.nb_rows());
	assert(row_start_index<=row_end_index);
	assert(col_start_index>=0 && col_start_index<m.nb_cols());
	assert(col_end_index>=0 && col_end_index<m.nb_cols());
	assert(col_start_index<=col_end_index);
	//assert(!___is_empty(m));

	M sub(row_end_index-row_start_index+1, col_end_index-col_start_index+1);
	//cout << "m=" << (row_end_index-row_start_index+1) << "n=" << (col_end_index-col_start_index+1) << endl;
	//cout << sub << endl;
	int i2=0;
	for (int i=row_start_index; i<=row_end_index; i++, i2++) {
		int j2=0;
		for (int j=col_start_index; j<=col_end_index; j++,j2++)
			sub[i2][j2] = m[i][j];
	}
	return sub;
}


template<class M1, class M2>
inline void _put(M1& m, int row_start_index, int col_start_index, const M2& sub) {
	assert(row_start_index>=0);
	assert(row_start_index+sub.nb_rows()<=m.nb_rows());
	assert(col_start_index>=0);
	assert(col_start_index+sub.nb_cols()<=m.nb_cols());
	assert(!___is_empty(m));
	assert(!___is_empty(sub));

	int i=row_start_index;
	for (int i2=0; i2<sub.nb_rows(); i++,i2++) {
		int j=col_start_index;
		for (int j2=0; j2<sub.nb_cols(); j++,j2++)
			m[i][j] = sub[i2][j2];
	}
}

template<class M, class V>
inline void _put(M& m, int row_start_index, int col_start_index, const V& v, bool row_vec) {
	assert(row_start_index>=0);
	assert(row_vec || row_start_index+v.size()<=m.nb_rows());
	assert(col_start_index>=0);
	assert(!row_vec || col_start_index+v.size()<=m.nb_cols());
	assert(!___is_empty(m));
	//TODO: assert(!___is_empty(v))

	int i=row_start_index;
	int j=col_start_index;
	for (int k=0; k<v.size(); k++) {
		m[i][j]=v[k];
		if (row_vec) j++;
		else i++;
	}
}


template<class M>
inline M _transpose(const M& m) {
	M m2(m.nb_cols(), m.nb_rows());

	if (___is_empty(m))
		___set_empty(m2);
	else
		for (int i=0; i<m.nb_rows(); i++) {
			for (int j=0; j<m.nb_cols(); j++) {
				m2[j][i]=m[i][j];
			}
		}

	return m2;
}

template<class M,class V>
inline V _col(const M& m, int j) {
	assert(!___is_empty(m));
	assert(j>=0 && j<m.nb_cols());

	V res(m.nb_rows());
	for (int i=0; i<m.nb_rows(); i++)
		res[i]=m[i][j];
	return res;
}

template<class M,class V>
inline void _set_col(M& m, int col1, const V& v) {
	assert(!___is_empty(m));
	assert(col1>=0 && col1<m.nb_cols());
	assert(m.nb_rows()==v.size());

	for (int i=0; i<m.nb_rows(); i++)
		m[i][col1]=v[i];
}


template<class M1,class M2>
M1& _assignM(M1& m1, const M2& m2) {
	assert(m1.nb_rows()==m2.nb_rows() && m1.nb_cols()==m2.nb_cols());
	// or implement "resize"
	//resize(x.nb_rows(), x.nb_cols());
	if (___is_empty(m2))
		___set_empty(m1);
	else
		for (int i=0; i<m1.nb_rows(); i++)
			m1.set_row(i,m2[i]);

	return m1;
}

template<class M1,class M2>
bool _equalsM(M1& m1, const M2& m2) {
	if (m1.nb_rows()!=m2.nb_rows()) return false;
	if (m1.nb_cols()!=m2.nb_cols()) return false;

	if (___is_empty(m1)) return ___is_empty(m2);
	if (___is_empty(m2)) return ___is_empty(m1);

	for (int i=0; i<m1.nb_rows(); i++) {
		if (m1.row(i)!=m2.row(i)) return false;
	}
	return true;
}

template<class M>
inline std::ostream& _displayM(std::ostream& os, const M& m) {
	if (___is_empty(m)) { return os << "empty matrix"; }
	os << "(";
	for (int i=0; i<m.nb_rows(); i++) {
		os << "(";
		for (int j=0; j<m.nb_cols(); j++) {
			os << m[i][j];
			if (j<m.nb_cols()-1) os << " ; ";
		}
		os << ")";
		if (i<m.nb_rows()-1) os << std::endl;
	}
	os << ")";
	return os;
}

template<typename M>
inline M _absM(const M& m) {
	M res(m.nb_rows(),m.nb_cols());

	if (___is_empty(m)) { ___set_empty(res); return res; }

	for (int i=0; i<m.nb_rows(); i++)
		res[i]=abs(m[i]);

	return res;
}

template<typename M>
inline double _infinite_normM(const M& m) {
	assert(!___is_empty(m));
			
    double norm = 0;
    for (int j=0; j<m.nb_cols(); j++)
        norm += ___mag(m[0][j]);
    for (int i=1; i<m.nb_rows(); i++) {
        double sum = 0;
        for (int j=0; j<m.nb_cols(); j++)
            sum += ___mag(m[i][j]);
        if (sum > norm)
            norm = sum;
    }
    return norm;
}

} // end anonymous namespace

} // end namespace ibex

#endif //__IBEX_TEMPLATE_MATRIX_H__
