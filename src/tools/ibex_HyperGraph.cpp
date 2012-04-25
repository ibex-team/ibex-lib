/* ============================================================================
 * I B E X - ibex_HyperGraph.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_HyperGraph.h"

namespace ibex {

HyperGraph::HyperGraph(int nb_ctr, int nb_var) : m(nb_ctr), n(nb_var) {
	matrix = new int*[m];
	ctr_adj = new int*[m];
	var_adj = new int*[n];

	for (int i=0; i<m; i++) {
		matrix[i]=new int[n];
		ctr_adj[i]=new int[n+1]; // the first place is kept for storing the size (arity) of ctr i
		for (int j=0; j<n; j++) matrix[i][j]=0;
		for (int j=0; j<n+1; j++) ctr_adj[i][j]=0;
	}
	for (int j=0; j<n; j++) {
		var_adj[j]=new int[m+1]; // the first place is kept for storing the size (arity) of var j
		for (int i=0; i<m+1; i++) var_adj[j][i]=0;
	}
}

void HyperGraph::add_arc(int ctr, int var, int value) {
	if (matrix[ctr][var]>0) return; // already present

	matrix[ctr][var]=value;

	ctr_adj[ctr][++ctr_adj[ctr][0]]=var;

	var_adj[var][++var_adj[var][0]]=ctr;
}


HyperGraph::HyperGraph(const HyperGraph& g) : m(g.m), n(g.n) {
	matrix = new int*[m];
	ctr_adj = new int*[m];
	var_adj = new int*[n];

	for (int i=0; i<m; i++) {
		matrix[i]=new int[n];
		ctr_adj[i]=new int[n+1];
		for (int j=0; j<n; j++) matrix[i][j]=g.matrix[i][j];
		for (int j=0; j<n+1; j++) ctr_adj[i][j]=g.ctr_adj[i][j];
	}
	for (int j=0; j<n; j++) {
		var_adj[j]=new int[m+1];
		for (int i=0; i<m+1; i++) var_adj[j][i]=g.var_adj[j][i];
	}
}

HyperGraph::~HyperGraph() {
	for (int i=0; i<m; i++) {
		delete[] matrix[i];
		delete[] ctr_adj[i];
	}
	for (int j=0; j<n; j++) {
		delete[] var_adj[j];
	}
	delete[] matrix;
	delete[] ctr_adj;
	delete[] var_adj;
}

std::ostream& operator<<(std::ostream& os, const HyperGraph& g) {

   os << "adj matrix\n";
   for (int i=0; i<g.nb_ctr(); i++) {
     for (int j=0; j<g.nb_var(); j++)
       os << g.arc(i,j) << " ";
     os << std::endl;
  }
  os << std::endl;

  for (int c=0; c<g.nb_ctr(); c++) {

    os <<"var adj of constraint " << c << ": [";
    for (int v=0; v<g.ctr_nb_vars(c); v++) {
      os <<g.ctr_ith_var(c,v) << " ";
    }
    os <<"]" <<std::endl;
  }

  for (int v=0; v<g.nb_var(); v++) {
    os <<"adj of variables   " << v << ": [";
    for (int c=0; c<g.var_nb_ctrs(v); c++) {
      os <<g.var_ith_ctr(v,c) << " ";
    }
    os <<"]"<<std::endl;
  }
  return os;
}

} // namespace ibex
