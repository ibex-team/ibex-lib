/*---------------------------------------------------------------------------------
 * 
 * Set of variables block (partition)
 * ----------------------------------
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

#ifndef _IBEX_BLOCK_SET_H
#define _IBEX_BLOCK_SET_H

#include "IbexException.h"

namespace ibex {

/** \ingroup exceptgroup
 * \brief Exception related to sets of variable blocks
 *
 * This exception is used for all the problems that can arise while creating/using an #ibex::BlockSet instance.
 * The #type member contains a more precise description of the problem (this constant
 * might evolve to subclasses in the future). 
 *
 * <table border="0">
 *  <tr><th align="left">\c BAD_NB_BLK </th><td> the number of blocks exceeds the number of variables,</td></tr>
 *  <tr><th align="left">\c BAD_VAR    </th><td> the variable specified exceeds the number of variables,</td></tr>
 *  <tr><th align="left">\c BAD_BLK    </th><td> the block specified exceeds the number of blocks,</td></tr>
 *  <tr><th align="left">\c EMPTY_BLK  </th><td> a block is empty,</td></tr>
 *  <tr><th align="left">\c NO_BLK     </th><td> the variable does not belong to any block,</td></tr>
 *  <tr><th align="left">\c ALL_EMPTY  </th><td> all the blocs are empty.</td></tr>
 * </table> 
 *
 * You can directly put an instance of this class
 * into the standard output, this will show the associated message, e.g.:
 * \code 
 * try {
 * ....
 * } catch (BlockSetException e) {
 *   cout << e << endl; // will display a message such as "the number of blocks exceeds etc."
 * }
 * \endcode  
 *
 * \author Gilles Chabert
 * \date March 2007 
 */
class BlockSetException : public IbexException { 
 public:
  /** \typedef exceptype
  See comments above. */
  typedef enum { BAD_NB_BLK, BAD_VAR, BAD_BLK, EMPTY_BLK, NO_BLK, ALL_EMPTY } exceptype;

  /** Create a block set exception. */
  BlockSetException(exceptype type) : type(type) { }
    
  /** Identification of the problem. */
  const exceptype type;
};

/** Stream out a block set exception with details */
std::ostream& operator<<(std::ostream&, const BlockSetException&);

/** \ingroup maingroup
 * \brief Set of variables blocks. 
 *
 * This class allows to create a parition of variables, i.e., divide variables up into various <i>blocks</i>.
 * Some algorithms (e.g., see #ibex::BlockRoundRobin or #ibex::MultiSystem::MultiSystem(const System&, const BlockSet&)) requires variables to be organized 
 * by blocks. E.g., instead of taking a usual set of variables such as {0,1,2,3,4,5}, it takes a collection of 
 * subsets such as { {0,2,5}, {3}, {1,4} }.
 *
 * \note Some variables may not belong to a block and a block can be empty. However, such situations
 * may not be valid for algorithms using instances of this class. 
 * \sa #ibex::BlockRoundRobin 
 *  and #ibex::MultiSystem::MultiSystem(const System&, const BlockSet&)
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class BlockSet {
 public:
  /** Create a set of \a nb_blk empty blocks.
   * \param nb_var - the number of variables to be divided. This number if fixed.
   * \param nb_blk - the number of blocks. This number is fixed.
   * \throw BlockSetException - if \a nb_blk exceeds \a nb_var. */
  BlockSet(int nb_var, int nb_blk) throw(BlockSetException);

  /** Duplicate the set of variable blocks.*/
  BlockSet(const BlockSet&);

  /** Delete the set of variable blocks. */
  ~BlockSet();

  /** Remove a variable from its block.
   *
   * \throw BlockSetException - if the variable is not in a block.*/  
  void rem_var(int var) throw(BlockSetException);

  /** Add a variable into a block.
   *
   * If the variable is already within a block, it is first removed from the latter.
   *
   * \param var - the variable to add.
   * \param blk - the block.
   * \throw BlockSetException - if \a var or \a blk exceeds bounds. */
  void set_blk(int var, int blk) throw(BlockSetException);

  /** Return the block of a variable. */
  inline int blk(int var) const throw(BlockSetException) { 
    if (var>=nb_var) throw BlockSetException(BlockSetException::BAD_VAR);
    if (noblk(var)) throw BlockSetException(BlockSetException::NO_BLK);  
    else return var2blk[var]; 
  }

  /** Return the variable following \a var in a block, in a circular way (if \a var
   * is the last variable of the block, then the method returns the first variable
   * of the block). 
   * \throw BlockSetException - if \a var exceeds the number of variables or
   * if the variable is in no block. */
  inline int next(int var) const throw(BlockSetException) { 
    if (var>=nb_var) throw BlockSetException(BlockSetException::BAD_VAR); 
    if (noblk(var)) throw BlockSetException(BlockSetException::NO_BLK); 
    else return nxt[var]; 
  }

  /** Return the first variable of the block \a blk. */
  inline int first(int blk) const throw(BlockSetException) { 
    if (blk>=nb_blk) throw BlockSetException(BlockSetException::BAD_BLK); 
    if (empty(blk)) throw BlockSetException(BlockSetException::EMPTY_BLK);
    else return blk2var[blk];
  }

  /** Return the last variable of the block \a blk. */
  inline int last(int blk) const throw(BlockSetException) { 
    return prv[first(blk)];
  }

  /** Return true if the variable \a var is not in a block. */
  inline bool noblk(int var) const throw(BlockSetException) {    
    if (var>=nb_var) throw BlockSetException(BlockSetException::BAD_VAR); 
    else return var2blk[var]==-1;
  }  

  /** Return true if the block \a blk is empty. */
  inline bool empty(int blk) const throw(BlockSetException) {    
    if (blk>=nb_blk) throw BlockSetException(BlockSetException::BAD_BLK); 
    else return blk2var[blk]==-1;
  }

 /** Return true if the block \a blk is empty. */
  inline int size(int blk) const throw(BlockSetException) {
    if (blk>=nb_blk) throw BlockSetException(BlockSetException::BAD_BLK); 
    else return blksize[blk];
  }

  /** The number of variables to divide up into blocks. */
  const int nb_var;

  /** The number of blocks. */
  const int nb_blk;

 private:
  /* Insert var before next. */
  void insert(int var, int next);

  int *var2blk;
  int *nxt;
  int *prv;
  int *blk2var;
  int *blksize;
};

/** Stream out a set of variable blocks. */
std::ostream& operator<<(std::ostream&, const BlockSet&);

} // end namespace

#endif
