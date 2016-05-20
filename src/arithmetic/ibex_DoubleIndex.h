/* ============================================================================
 * I B E X - DoubleIndex of an expression
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 1, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_INDEX_H__
#define __IBEX_INDEX_H__

#include <math.h>
#include "ibex_Dim.h"

namespace ibex {

class DoubleIndex {
public:
	static DoubleIndex one_elt(const Dim& d, int i, int j);
	static DoubleIndex one_row(const Dim& d, int i);
	static DoubleIndex one_col(const Dim& d, int j);
	static DoubleIndex rows(const Dim& d, int first_row, int last_row);
	static DoubleIndex cols(const Dim& d, int first_col, int last_col);
	static DoubleIndex subrow(const Dim& d, int i, int first_col, int last_col);
	static DoubleIndex subcol(const Dim& d, int first_row, int last_row, int j);
	static DoubleIndex submatrix(const Dim& d, int first_row, int last_row, int first_col, int last_col);
	static DoubleIndex all(const Dim& d);

	/** Basic index system "expr[i]" */
	static DoubleIndex one_index(const Dim& d, int i);

	/** \brief Uninitialized object */
	DoubleIndex();

	DoubleIndex(const DoubleIndex&);
	long long hash() const;
	int row() const;
	int col() const;
	int first_row() const;
	int first_col() const;
	int last_row() const;
	int last_col() const;
	int nb_rows() const;
	int nb_cols() const;
	bool one_row() const;
	bool one_col() const;
	bool all_rows() const;
	bool all_cols() const;
	bool all() const;
	bool operator==(const DoubleIndex& idx) const;
	bool operator!=(const DoubleIndex& idx) const;

private:
	DoubleIndex(const Dim& d, int first_row, int last_row, int first_col, int last_col);
	Dim dim;
	int r1, r2, c1, c2;
};

/**
 * \brief Display the index, ex: "(:,1)".
 */
std::ostream& operator<<(std::ostream& os, const DoubleIndex idx);

/*============================================ inline implementation ============================================ */
inline DoubleIndex DoubleIndex::one_elt(const Dim& d, int i, int j) {
	return submatrix(d,i,i,j,j);
}

inline DoubleIndex DoubleIndex::one_row(const Dim& d, int i) {
	return rows(d,i,i);
}

inline DoubleIndex DoubleIndex::one_col(const Dim& d, int j) {
	return cols(d,j,j);
}

inline DoubleIndex DoubleIndex::rows(const Dim& d, int i1, int i2) {
	return submatrix(d,i1,i2,0,d.nb_cols()-1);
}

inline DoubleIndex DoubleIndex::cols(const Dim& d, int j1, int j2) {
	return submatrix(d,0,d.nb_rows()-1,j1,j2);
}

inline DoubleIndex DoubleIndex::subrow(const Dim& d, int i, int first_col, int last_col) {
	return submatrix(d,i,i,first_col,last_col);
}

inline DoubleIndex DoubleIndex::subcol(const Dim& d, int first_row, int last_row, int j) {
	return submatrix(d,first_row,last_row,j,j);
}

inline DoubleIndex DoubleIndex::submatrix(const Dim& d, int i1, int i2, int j1, int j2) {
	return DoubleIndex(d,i1,i2,j1,j2);
}

inline DoubleIndex DoubleIndex::all(const Dim& d) {
	return rows(d,0,d.nb_rows()-1);
}

inline DoubleIndex DoubleIndex::one_index(const Dim& dim, int i) {
	if (dim.is_matrix())
		return one_row(dim,i);
	else
		// A single index i with a row vector
		// gives the ith column.
		return one_col(dim,i);
}

inline DoubleIndex::DoubleIndex() : dim(Dim::scalar()), r1(-1), r2(-1), c1(-1), c2(-1) {
}

inline DoubleIndex::DoubleIndex(const Dim& d, int i1, int i2, int j1, int j2) :
	dim(d), r1(i2), r2(i2), c1(j1), c2(j2) {

	if (i2>=d.nb_rows() || j2>=d.nb_cols())
		throw DimException("DoubleIndex: index out of bounds");
	if (i1<0 || i1>i2 || j1<0 || j1>j2)
		ibex_error("DoubleIndex: malformed indices");
}

inline DoubleIndex::DoubleIndex(const DoubleIndex& idx) :
		// note: C++ would do it implicitly.
		dim(idx.dim), r1(idx.r1), r2(idx.r2), c1(idx.c1), c2(idx.c2) {
}

inline long long DoubleIndex::hash() const {
	return ::pow(2,r1)+::pow(3,r2)+::pow(5,c1)+::pow(7,c2);
}

inline int DoubleIndex::row() const       { return r1; }
inline int DoubleIndex::col() const       { return c1; }
inline int DoubleIndex::first_row() const { return r1; }
inline int DoubleIndex::first_col() const { return c1; }
inline int DoubleIndex::last_row() const  { return r2; }
inline int DoubleIndex::last_col() const  { return c2; }
inline int DoubleIndex::nb_rows() const   { return r2-r1+1; }
inline int DoubleIndex::nb_cols() const   { return c2-c1+1; }
inline bool DoubleIndex::one_row() const  { return r1==r2; }
inline bool DoubleIndex::one_col() const  { return c1==c2; }
inline bool DoubleIndex::all_rows() const { return r1==0 && r2==dim.nb_rows()-1; }
inline bool DoubleIndex::all_cols() const { return c1==0 && c2==dim.nb_cols()-1; }
inline bool DoubleIndex::all() const { return all_rows() && all_cols(); }
inline bool DoubleIndex::operator==(const DoubleIndex& idx) const {
	return dim==idx.dim && r1==idx.r1 && r2==idx.r2 && c1==idx.c1 && c2==idx.c2;
}
inline bool DoubleIndex::operator!=(const DoubleIndex& idx) const { return !(*this==idx); }

inline std::ostream& operator<<(std::ostream& os, const DoubleIndex idx) {
	if (idx.all()) return os;

	os << "(";
	switch (idx.dim.type) {
	case Dim::ROW_VECTOR:
		if (idx.one_col()) return os << idx.first_col();
		else return os << idx.first_col() << ":" << idx.last_col();
	case Dim::COL_VECTOR:
		if (idx.one_row()) return os << idx.first_row();
		else return os << idx.first_row() << ":" << idx.last_row();
	default:
		assert(idx.dim.is_matrix());
		if (idx.all_rows()) os << ":";
		else if (idx.one_row()) os << idx.first_row();
		else os << idx.first_row() << ":" << idx.last_row();
		os << ",";
		if (idx.all_cols()) os << ":";
		else if (idx.one_col()) os << idx.first_col();
		else os << idx.first_col() << ":" << idx.last_col();
	}
	return os << ")";
}

} /* namespace ibex */

#endif /* __IBEX_INDEX_H__ */
