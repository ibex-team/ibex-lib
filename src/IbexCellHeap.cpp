/*---------------------------------------------------------------------------------
 * 
 * Heap of Cells - To order boxes wrt to some criterion
 * ---------------------------------------------------
 *
 * Copyright (C) 2007-2010 Gilles Chabert
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

#include "IbexCellHeap.h"
#include <algorithm>

namespace ibex {


namespace {
  /* we need this dummy class just because
   * the xxx_heap functions take the comparator
   * argument by copy (hence, we cannot give
   * "*this" since the class is abstract) */
  struct CellComparator {
    bool operator()(const pair<Cell*,REAL>& c1, const pair<Cell*,REAL>& c2) {
      /* if (c1.second == c2.second)
	if (random()%2)
	  return true;
	else
	  return false;
      else
      */
	return c1.second >= c2.second;
    }
  };
}

bool CellHeap::operator()(const pair<Cell*,REAL>& c1, const pair<Cell*,REAL>& c2) const {
  return c1.second >= c2.second;
}

void CellHeap::flush() { 
  for (vector<pair<Cell*,REAL> >::iterator it=l.begin(); it!=l.end(); it++)
    delete it->first;

  l.clear();
}

int CellHeap::size() const {
  return l.size();
}



// E.g.: called by manage_buffer in Optimizer in case of a new upper bound
// on the objective ("loup"). This function then removes (and deletes) from 
// the heap all the cells with a cost greater than loup.
void CellHeap::contract_heap(REAL loup)
  {  
    //    cout << " before contract heap  " << l.size() << endl;

    sort_heap(l.begin(),l.end(),CellComparator());
    vector<pair<Cell*,REAL> >::iterator it0=l.begin();
    int k=0;
    for (vector<pair<Cell*,REAL> >::iterator it=l.begin(); it!=l.end(); it++)
      { 
	it0=it;       	
        if (it->second < loup) break;
	k++;
      }
    
    for (int i=0;i<k;i++)
      {delete l[i].first;}
    
    if (k>0) l.erase(l.begin(),it0);
      
    make_heap(l.begin(), l.end() ,CellComparator());
    //    cout << " after contract heap " << l.size() << endl;

  }


bool CellHeap::empty() const {
  return l.empty();
}

void CellHeap::push(Cell* cell) { 
  l.push_back(pair<Cell*,REAL>(cell,cost(*cell))); 
  push_heap(l.begin(), l.end(), CellComparator());
}

Cell* CellHeap::pop() { 
  Cell* c = l.front().first;
  pop_heap(l.begin(),l.end(), CellComparator()); // put the "best" at the end
  l.pop_back(); // removes the "best"
  return c;     // and return it
}

Cell* CellHeap::top() const { 
  return l.front().first;
}

ostream& operator<<(ostream& os, const CellHeap& heap) {
  os << "[ ";
  for (vector<pair<Cell*,REAL> >::const_iterator it=heap.l.begin(); it!=heap.l.end(); it++)
    os << it->first->get_box() << " ";
  return os << "]";
}
  

} // end namespace


