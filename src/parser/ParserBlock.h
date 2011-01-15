/*---------------------------------------------------------------------------------
 * 
 * Lists of constraint/contractor blocks
 * -------------------------------------
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

#ifndef __PARSER_BLOCK_H
#define __PARSER_BLOCK_H

#include <vector>
#include "Scope.h"

extern int ibex_lineno;

namespace ibex {

namespace parser {

/*---------------------------------------------------------------------------------*/
/* list of blocks */

template<class T> class P_ListVisitor;

template<class T>
class P_List {
 public:

  virtual ~P_List() { }

  virtual void accept(P_ListVisitor<T>&) const=0;
};

template<class T> class P_BlkList;
template<class T> class P_BlkListRef;

template<class T>
class P_ListVisitor {
 public:
  virtual void visit(const P_List<T>&)=0;
  virtual void visit(const P_BlkList<T>&)=0;
  virtual void visit(const P_BlkListRef<T>&)=0;
};


/*---------------------------------------------------------------------------------*/
/* blocks */

template<class T> class P_BlkVisitor;

template<class T>
class P_Blk {
 public:
  P_Blk() : line(ibex_lineno) { }
  
  virtual ~P_Blk() { }

  virtual void accept(P_BlkVisitor<T>&) const=0;

  int line;
};

template<class T> class P_Loop;

template<class T>
class P_BlkVisitor {
 public:
  virtual void visit(const P_Blk<T>&)=0;
  virtual void visit(const P_Loop<T>&)=0;
  virtual void visit(const T&)=0;
};

/*---------------------------------------------------------------------------------*/

template<class T>
class P_BlkList : public P_List<T> {
 public:

  /* create an empty list with a given scope.
   * A copy of the scope is made. */
  //  P_BlkList(const Scope& scope) : scope(scope) { }

  /* Create an empty list with. */
  P_BlkList() { }

  /* create a list filled with a first block. */
  P_BlkList(const P_Blk<T>& b) { //: scope(scope) { 
    list.push_back(&b);
  }

  ~P_BlkList() {
    for (typename vector<const P_Blk<T>*>::iterator it=list.begin(); it!=list.end(); it++) {
      delete *it; 
    }
  }

  virtual void accept(P_ListVisitor<T>& v) const { v.visit(*this); }
    
  /* add a constraint to the list */
  void add(const P_Blk<T>& b) { 
    list.push_back(&b);
  }

  int size() const { return list.size(); }

  const P_Blk<T>& operator()(int i) const { return *list[i]; }

  vector <const P_Blk<T>*> list;

  //  const Scope scope;
};

/* A reference to a list */
template<class T>
class P_BlkListRef : public P_List<T> {
 public:
  P_BlkListRef(const char* name) : name(strdup(name)) { }

  ~P_BlkListRef() { 
    free((char*) name); 
  }

  virtual void accept(P_ListVisitor<T>& v) const { v.visit(*this); }

  const char* name;
};


/*---------------------------------------------------------------------------------*/

/* "For" loop around an iterator-dependant block */
template<class T>
class P_Loop : public P_Blk<T> {
 public:
  P_Loop(const char* iter, int first_value, int last_value, const P_List<T>& iter_list) : 
    iter(strdup(iter)), first_value(first_value), last_value(last_value), iter_list(iter_list) { }

  ~P_Loop() { 
    free((char*) iter);
    delete &iter_list;
  }

  virtual void accept(P_BlkVisitor<T>& v) const { v.visit(*this); } 

  const char* iter;
  int first_value;
  int last_value;
  const P_List<T>& iter_list; // iterators-dependent block
};

} // end namespace parser

} // end namespace ibex

#endif
