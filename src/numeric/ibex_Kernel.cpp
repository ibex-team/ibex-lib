//============================================================================
//                                  I B E X
// File        : ibex_Kernel.cpp
// Author      : Alexandre Goldsztejn
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 5, 2018
// Last Update : Mar 5, 2018
//============================================================================

#include "ibex_Kernel.h"
#include "ibex_Linear.h"

namespace ibex {

unsigned int gram_schmidt(Matrix& A, double eps) {
	unsigned int n=A.nb_rows();
	unsigned int m=A.nb_cols();

	unsigned int current_row;
	for(current_row=0; current_row<n; current_row++) {
		// Select among the remaining rows, which are orthogonalized wrt the firsts,
		// with greatest norm
		unsigned int best_row=current_row;
		double best_norm=norm(A.row(current_row));
		for(unsigned int i=current_row+1; i<n; i++) {
			double new_norm=norm(A.row(i));
			if(new_norm>best_norm) {
				best_norm=new_norm;
				best_row=i;
			}
		}
		if(best_norm<eps) {
			break; // remaining vectors will be set to zero
		}
		else {
			A.row(best_row)*=1/best_norm;
			if(best_row!=current_row) {
				// Exchange the selected row with the current one
				Vector temp=A.row(current_row);
				A.row(current_row)=A.row(best_row);
				A.row(best_row)=temp;
			}
			// Orthogonalize the remaining rows
			for(unsigned int i=current_row+1; i<n; i++) {
				A.row(i) -= ((A.row(i)*A.row(current_row)) /*/(A.row(current_row)*A.row(current_row))*/ ) *A.row(current_row); // A.row(current_row) is already normalized
			}
		}
	}
	unsigned int n_lin_ind=current_row;
	// Set to zero the vectors remaining after the break, if any
	for(; current_row<n; current_row++)
		A.row(current_row)=Vector::zeros(m);
	return n_lin_ind;
}

Matrix kernel(const Matrix& A) {
	Matrix P=Matrix::eye(A.nb_cols())-A.transpose()*real_inverse(A*A.transpose())*A;
	unsigned int dim_kernel=gram_schmidt(P);
	Matrix U(dim_kernel,A.nb_cols());
	for(unsigned int i=0; i<dim_kernel; i++) {
		U.row(i)=P.row(i);
	}
	return U;
}

} /* namespace ibex */
