/*---------------------------------------------------------------------------------
 * 
 * Linear algebra routines
 * -----------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/


#include <math.h>
#include "IbexLinear.h"
#include "IbexProjections.h"

#define TOO_LARGE 1e30
#define TOO_SMALL 1e-10

namespace ibex {

void RealLU(const MATRIX& A, MATRIX& LU, int* p) {

  int n = RowDimension(A);  
  if (n != ColDimension(A)) throw NotSquareMatrixException();
  if (n != RowDimension(LU) || n != ColDimension(LU)) throw NonRecoverableException("Bad argument dimension");

  LU = A;
  for (int i=1; i<=n; i++) p[i]=i;
  
  // LU computation
  REAL pivot;

  for (int i=1; i<=n; i++) {
    // pivot search
    int swap = i;
    pivot = LU(p[i],i);
    for (int j=i+1; j<=n; j++) {
      volatile REAL tmp=LU(p[j],i);
      if (fabs(tmp)>=TOO_LARGE) throw SingularMatrixException();
      if (fabs(tmp)>fabs(LU(p[swap],i))) swap=j;  
    }
    int tmp = p[i];
    p[i] = p[swap];
    p[swap] = tmp;
    // --------------------------------------------------

    pivot = LU(p[i],i);
    if (pivot==0.0) throw SingularMatrixException();
    if (fabs(1/pivot)>=TOO_LARGE) throw SingularMatrixException();

    for (int j=i+1; j<=n; j++) {
      for (int k=i+1; k<=n; k++) {
	LU(p[j],k)-=(LU(p[j],i)/pivot)*LU(p[i],k);
      }
      LU(p[j],i) = LU(p[j],i)/pivot;
    }
  }
}

static void RealLUSolve(const MATRIX& LU, const int* p, const VECTOR& b, VECTOR& x) {
  //cout << "LU=" << LU << " b=" << b << endl;

  int n = RowDimension(LU);  
  if (n != ColDimension(LU)) throw NotSquareMatrixException();
  if (n != Dimension(b) || n != Dimension(x)) throw NonRecoverableException("Bad argument dimension");
  
  // solve Lx=b
  x(1) = b(p[1]);  
  for (int i=2; i<=n; i++) {
    x(i) = b(p[i]);
    for (int j=1; j<i; j++) {
      x(i) -= LU(p[i],j)*x(j);
    }
  }
  //cout << " x1=" << x << endl;
  // solve Uy=x 
  if (fabs(LU(p[n],n))<=TOO_SMALL) throw SingularMatrixException();
  x(n) /= LU(p[n], n);
  for (int i=n-1; i>0; i--) {
    for (int j=i+1; j<=n; j++) {
      x(i) -= LU(p[i],j)*x(j);
    }
    if (fabs(LU(p[i],i))<=TOO_SMALL) throw SingularMatrixException();
    x(i) /= LU(p[i],i);
  } 
  //cout << " x2=" << x << endl;
}

void RealInverse(const MATRIX& A, MATRIX& invA) {
  int n = RowDimension(A);  

  MATRIX LU(n,n);
  int* p= new int[n+1];  

  try {
    RealLU(A, LU, p);

    VECTOR b(n);
    VECTOR x(n);
    
    for (int i=1; i<=n; i++) b(i)=0;

    for (int i=1; i<=n; i++) {
      b(i)=1;
      RealLUSolve(LU, p, b, x);
      SetCol(invA, i, x);
      b(i)=0;
    }
  } 
  catch(NotSquareMatrixException e) {
    delete[] p;
    throw e;
  }
  catch(SingularMatrixException e) {
    delete[] p;
    throw e;
  }
  delete[] p;
}

void Precond(INTERVAL_MATRIX& A, INTERVAL_VECTOR& b) {
  int n=RowDimension(A);
  if (n != ColDimension(A)) throw NotSquareMatrixException();  // not well-constraint problem 
  if (n != Dimension(b)) throw NonRecoverableException("Bad argument dimension");           // unvalid right-hand side dimension
  
  MATRIX C(n,n);
  try { RealInverse(Mid(A), C); }
  catch (SingularMatrixException) {
    try { RealInverse(Inf(A), C); }
    catch (SingularMatrixException) {
      RealInverse(Sup(A), C);
    }    
  }

//   cout << "A=" << ColDimension(A) << "x" << RowDimension(A) << "  " << "b=" << Dimension(b) << "  " << "C=" 
//        << ColDimension(C) << "x" << RowDimension(C) << endl;
  //cout << "C=" << C << endl;
  A = C*A;
  b = C*b;
}

// static void lu_interval(INTERVAL_MATRIX& A, int i, int n, INTERVAL& det) {

//   if (i==n) { 
//     if (A(n,n).contains(0)) throw NullPivotException();
//     det=A(n,n); 
//     return;
//   }
  
//   INTERVAL pivot = A(i,i);
  
//   if (pivot.contains(0)) throw NullPivotException();
  
//   for (int j=i+1; j<=n; j++) {
//     for (int k=i+1; k<=n; k++) A(j,k)-=(A(j,i)/pivot)*A(i,k);
//     A(j,i) = A(j,i)/pivot;
//   }
  
//   lu_interval(A, i+1, n, det);
//   det = A(i,i)*det;
// }

void GaussSeidel(const INTERVAL_MATRIX& A, const INTERVAL_VECTOR& b, INTERVAL_VECTOR& x, REAL ratio) {
  int n=RowDimension(A);
  if (n != ColDimension(A)) throw NotSquareMatrixException();
  if (n != Dimension(x) || n != Dimension(b)) throw NonRecoverableException("Bad argument dimension");
  
  REAL red;
  INTERVAL old, proj;

  do {
    red = 0;
    for (int i=1; i<=n; i++) {
      old = x(i);
      proj = b(i);
      for (int j=1; j<=n; j++)	if (j!=i) proj -= A(i,j)*x(j);      
      if (!proj.contains(0) || !A(i,i).contains(0))
	x(i).div_intersect(proj,A(i,i));
      if (x(i).empty()) { x.set_empty(); return; }
      REAL gain=old.ratiodelta(x(i));
      if (gain>red) red=gain;
    }    
  } while (red >= ratio);
}


void ContractMult(INTERVAL& A, INTERVAL_MATRIX& B, const INTERVAL_MATRIX& C, REAL ratio) {
  int n=RowDimension(C);
  int m=ColDimension(C);
  if (RowDimension(B)!=n || ColDimension(B)!=m) throw NonRecoverableException("Bad argument dimension");

//   cout << "A=" << A << endl;
//   cout << "B=" << B << endl;
//   cout << "C=" << C << endl;

  for (int i=1; i<=n; i++) {
      for (int j=1; j<=m; j++) {
	if (!proj_mult(C(i,j),A,B(i,j))) throw EmptyBoxException();
      }
    }
}

void ContractMult(INTERVAL_MATRIX& A, INTERVAL_MATRIX& B, const INTERVAL_MATRIX& C, REAL ratio) {
  int n=RowDimension(C);
  int m=ColDimension(C);
  int l=ColDimension(A);
  if (RowDimension(A)!=n || ColDimension(B)!=m || RowDimension(B)!=l) throw NonRecoverableException("Bad argument dimension");
  
  REAL red;
  INTERVAL oldA, oldB, proj;
  
  do {
    red = 0;
    for (int i=1; i<=n; i++) {
      for (int j=1; j<=m; j++) {
	for (int k=1; k<=l; k++) {
	  oldA = A(i,k);
	  oldB = B(k,j);

	  proj = C(i,j);
	  for (int p=1; p<=l; p++) if (p!=k) proj -= A(i,p)*B(p,j);

	  proj_mult(proj, A(i,k),B(k,j));

//   	  cout << "A(" << i << "," << k << ")=" << A(i,k) << endl;
//   	  cout << "B(" << k << "," << j << ")=" << B(k,j) << endl;
	  if (A(i,k).empty() || B(k,j).empty()) throw EmptyBoxException();
	  
	  REAL gain=oldA.ratiodelta(A(i,k));	 
	  if (gain>red) red=gain;
	  gain=oldB.ratiodelta(B(k,j));
	  if (gain>red) red=gain;
	} }
    }
  } while (red >= ratio);  
}

// void PrecGaussSeidel(INTERVAL_MATRIX& A, INTERVAL_VECTOR& b, INTERVAL_VECTOR& x) throw(LinearException) {
//   Precond(A, b);
//   GaussSeidel(A, b, x);
// }

void HansenBliek(const INTERVAL_MATRIX& A, const INTERVAL_VECTOR& B, INTERVAL_VECTOR& x) {
  int n=RowDimension(A);
  if (n != ColDimension(A)) throw NotSquareMatrixException();
  if (n != Dimension(x) || n != Dimension(B)) throw NonRecoverableException("Bad argument dimension");
  
  MATRIX Id(n,n); 
  MATRIX Zero(n,n);
  for (int i=1; i<=n; i++) for (int j=1; j<=n; j++) {
    Zero(i,j) = 0;
    Id(i,j)   = i==j;
  }
  
  INTERVAL_MATRIX radA(A-Id);
  MATRIX InfA(Id-Abs(radA));
  MATRIX M(n,n);

  RealInverse(InfA, M);  // may throw SingularMatrixException...
  
  if (! (M>=Zero)) throw NotInversePositivieMatrixException();
  
  VECTOR b(Mid(B));
  VECTOR delta = Sup(B)-b;

  VECTOR xstar = M * (Abs(b)+delta);

  REAL xtildek, xutildek, nuk, max, min;

  for (int k=1; k<=n; k++) {
    xtildek = (b(k)>=0) ? xstar(k) : xstar(k) + 2*M(k,k)*b(k);
    xutildek = (b(k)<=0) ? -xstar(k) : -xstar(k) + 2*M(k,k)*b(k);

    nuk = 1/(2*M(k,k)-1);
    max = nuk*xutildek;
    if (max < 0) max = 0;

    min = nuk*xtildek;
    if (min > 0) min = 0;

    /* compute bounds of x(k) */
    if (xtildek >= max) {
      if (xutildek <= min) x(k) = INTERVAL(xutildek,xtildek);
      else x(k) = INTERVAL(max,xtildek);
    } else {
      if (xutildek <= min) x(k) = INTERVAL(xutildek,min);
      else { x.set_empty(); return; }
    }
  }  
} 




} // end namespace

