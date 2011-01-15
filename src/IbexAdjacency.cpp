/*---------------------------------------------------------------------------------
 * 
 * Adjacency of contractors
 * ------------------------
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

#include "IbexAdjacency.h"
#include "IbexEnv.h"

namespace ibex {

Adjacency::Adjacency(const ContractorList& csp, const Space& space) {

  _nb_ctr = csp.size();

  int* ctr_count[4]; // count the number of constraints in which an entity is involved

  for (int type=0; type<4; type++) {
    nb_ent[type]     = space.nb((EntityType) type);
    adj_matrix[type] = new int*[_nb_ctr];
    ctr_adj[type]    = new int*[_nb_ctr];
    ent_adj[type]    = new int*[nb_ent[type]];
    ctr_count[type]    = new int[nb_ent[type]];
    for (int e=0; e<nb_ent[type]; e++) ctr_count[type][e]=0;
  }


  /*--------------------------- build adjacency matrix --------------------------------*/
  for (int c=0; c<_nb_ctr; c++) {
    
    int ent_count[4]; // count the number of entities in the current constraint

    for (int type=0; type<4; type++) {
      adj_matrix[type][c] = new int[nb_ent[type]];      
      for (int e=0; e<nb_ent[type]; e++) adj_matrix[type][c][e]=0;
      ent_count[type] = 0;
    }

    for (int i=0; i<space.env.nb_keys(); i++) {
      if (space.used(i) && csp(c).involves(i)) {
	EntityType type = space.entity(i).type;
	int dom_num = space.component(i);
	adj_matrix[type][c][dom_num]=1;              // [rem] 1 could be replaced by the number of occurrences (if built with System)
	ctr_count[type][dom_num]++;	             // increase number of constraints involving e
	ent_count[type]++;                           // increase number of entities involved in c
      }
    }

    for (int type=0; type<4; type++) {
      ctr_adj[type][c] = new int[ent_count[type]+1];
      ctr_adj[type][c][0] = 0;   // this integer is used as a counter for the number of entities
                                 // in c and will eventually contain ent_count[type].
    }

  }

  /*--------------------------- build adjacency lists --------------------------------*/
  for (int type=0; type<4; type++) {
    for (int e=0; e<nb_ent[type]; e++) {
      ent_adj[type][e] = new int[ctr_count[type][e]+1];
      ent_adj[type][e][0] = 0;   // this integer is used as a counter for the number of constraints 
      for (int c=0; c<_nb_ctr; c++) {
	if (adj_matrix[type][c][e]) {
	  ctr_adj[type][c][++ctr_adj[type][c][0]] = e;
	  ent_adj[type][e][++ent_adj[type][e][0]] = c;
	}
      } 
    }
  }

  for (int type=0; type<4; type++) delete[] ctr_count[type];
}


Adjacency::Adjacency(const Adjacency& a) {
  _nb_ctr = a._nb_ctr;

  for (int type=0; type<4; type++) {
    nb_ent[type] = a.nb_ent[type];
    adj_matrix[type] = new int*[_nb_ctr];
    ctr_adj[type]    = new int*[_nb_ctr];
    ent_adj[type]    = new int*[nb_ent[type]];

    for (int c=0; c<_nb_ctr; c++) {
      adj_matrix[type][c] = new int[a.nb_ent[type]];      

      for (int e=0; e<nb_ent[type]; e++) {
	adj_matrix[type][c][e]= a.adj_matrix[type][c][e];
      }

      int n = a.ctr_adj[type][c][0];
      ctr_adj[type][c] = new int[n+1];

      for (int i=0; i<n+1; i++) {
	ctr_adj[type][c][i] = a.ctr_adj[type][c][i];
      }
    }

    for (int e=0; e<nb_ent[type]; e++) {
      int n = a.ent_adj[type][e][0];
      ent_adj[type][e] = new int[n+1];
      for (int i=0; i<n+1; i++) {
	ent_adj[type][e][i] = a.ent_adj[type][e][i];
      }
    }
  }
}

Adjacency::~Adjacency() {
  if (_nb_ctr>0)
    for (int type=0; type<4; type++) {
      for (int c=0; c<nb_ctr(); c++) {
	delete[] adj_matrix[type][c];
	delete[] ctr_adj[type][c];
      }
      for (int e=0; e<nb_ent[type]; e++) {
	delete[] ent_adj[type][e];
      }
      delete[] adj_matrix[type];
      delete[] ctr_adj[type];
      delete[] ent_adj[type];
    }
}

void Adjacency::init(CoupleQueue& q) const {
  for (int c=0; c<nb_ctr(); c++)
    for (int i=0; i<ctr_nb_vars(c); i++)
      q.push(c,ctr_ith_var(c,i));
}

void Adjacency::propagate(CoupleQueue& q, int c, int v) const {

  for (int i=0; i<var_nb_ctrs(v); i++) {
    int c2 = var_ith_ctr(v,i);
    if (c!=c2)
      for (int j=0; j<ctr_nb_vars(c2); j++) {
	int v2 = ctr_ith_var(c2,j);
 	//if (v!=v2 || ctr_nb_occ_var(c2,v)>1)
	q.push(c2,v2);
      }
  }
}

std::ostream& operator<<(std::ostream& os, const Adjacency& a) {

//   cout << "adj matrix\n";
//   for (int i=0; i<a._nb_ctr; i++) {
//     for (int j=0; j<a._nb_ctr; j++)
//       cout << a.adj_matrix[i][j] << " ";
//     cout << endl;
//   }
//   cout << endl;

  for (int c=0; c<a.nb_ctr(); c++) {
    
    os <<"var adj of constraint " << c << ": [";
    for (int v=0; v<a.ctr_nb_vars(c); v++) {
      os <<a.ctr_ith_var(c,v) << " ";
    }
    os <<"]" <<endl; 

    os <<"syb adj of constraint " << c << ": [";
    for (int s=0; s<a.ctr_nb_sybs(c); s++) {
      os <<a.ctr_ith_syb(c,s) << " ";
    }
    os <<"]" <<endl; 

    os <<"epr adj of constraint " << c << ": [";
    for (int u=0; u<a.ctr_nb_eprs(c); u++) {
      os <<a.ctr_ith_epr(c,u) << " ";
    }
    os <<"]" <<endl; 

    os <<"upr adj of constraint " << c << ": [";
    for (int u=0; u<a.ctr_nb_uprs(c); u++) {
      os <<a.ctr_ith_upr(c,u) << " ";
    }
    os <<"]" <<endl; 

  }

  for (int v=0; v<a.nb_var(); v++) {
    os <<"adj of variables   " << v << ": [";
    for (int c=0; c<a.var_nb_ctrs(v); c++) {
      os <<a.var_ith_ctr(v,c) << " ";
    }
    os <<"]"<<endl;
  }
  for (int s=0; s<a.nb_syb(); s++) {
    os <<"adj of sybs   " << s << ": [";
    for (int c=0; c<a.syb_nb_ctrs(s); c++) {
      os <<a.syb_ith_ctr(s,c) << " ";
    }
    os <<"]"<<endl;
  }

  for (int u=0; u<a.nb_epr(); u++) {
    os <<"adj of eprs   " << u << ": [";
    for (int c=0; c<a.epr_nb_ctrs(u); c++) {
      os <<a.epr_ith_ctr(u,c) << " ";
    }
    os <<"]"<<endl;
  }

  for (int u=0; u<a.nb_upr(); u++) {
    os <<"adj of uprs   " << u << ": [";
    for (int c=0; c<a.upr_nb_ctrs(u); c++) {
      os <<a.upr_ith_ctr(u,c) << " ";
    }
    os <<"]"<<endl;
  }

  return os;
}

} // end namespace
