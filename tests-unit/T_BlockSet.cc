
/* CHABERT March 16th 2007 */

#include "T_BlockSet.h"
#include "IbexBisection.h"
#include "IbexEnv.h"
#include "IbexSpace.h"
#include "IbexBisector.h"
#include "IbexBisection.h"
#include "IbexBlockSet.h"
#include "IntervalVector.h"

using namespace ibex;

void T_BlockSet() {

  cout << "-------------- check block sets --------------\n";
  _class="BlockSet";
  _meth="constructor";
  _nber = 0;
  try {
    BlockSet(30,31);
    _test(0);
  } catch(...) { _test(1); }

  try {
    BlockSet(30,0);
    _test(0);
  } catch(...) { _test(1); }

  BlockSet bs(6,3);

  bs.set_blk(3,0);
  bs.set_blk(0,0);
  bs.set_blk(1,1);
  bs.set_blk(4,2);
  bs.set_blk(5,0);
  bs.set_blk(2,2);

  /***************************************************/
  _meth="copy constructor";
  _nber = 0;

  BlockSet bs2(bs);
  _test(bs2.blk(3)==0);
  _test(bs2.first(2)==2);
  _test(bs2.last(1)==1);

  /***************************************************/
  _meth="blc";
  _nber = 0;

  _test(bs.blk(0)==0);
  _test(bs.blk(1)==1);
  _test(bs.blk(2)==2);
  _test(bs.blk(3)==0);
  _test(bs.blk(4)==2);
  _test(bs.blk(5)==0);

  /***************************************************/
  _meth="first/next";
  _nber = 0;

  _test(bs.first(0)==0);
  _test(bs.next(0)==3);
  _test(bs.next(3)==5);
  _test(bs.next(5)==0);

  _test(bs.first(1)==1);
  _test(bs.next(1)==1);

  _test(bs.first(2)==2);
  _test(bs.next(2)==4);
  _test(bs.next(4)==2);

  /***************************************************/
  _meth="rem_var";
  _nber = 0;

  bs.rem_var(1);
  _test(bs.empty(1));

  bs.rem_var(3);
  _test(bs.first(0)==0);
  _test(bs.next(0)==5);
  _test(bs.next(5)==0);
 
  bs.rem_var(0);
  _test(bs.first(0)==5);
  _test(bs.next(5)==5);

  bs.rem_var(5);
  _test(bs.empty(0));

  /***************************************************/
  _meth="set_blc";
  _nber = 0;

  bs.set_blk(0,0);

  _test(bs.first(0)==0);
  _test(bs.next(0)==0);

  // change the block of a var such that the old block becomes empty and the new block becomes nonempty.
  bs.set_blk(0,1);
  _test(bs.empty(0));
  _test(bs.first(1)==0);
  _test(bs.next(0)==0);

  // change the block of a var with old & new blocks with multiple variables.
  bs.set_blk(2,1);
  _test(bs.next(0)==2);
  _test(bs.next(4)==4);

}

void T_block_round_robin() {
  /***************************************************/
  _class = "BlockRoundRobin";
  _meth="bisect";
  _nber = 0;

  Env env;
  env.add_symbol("x",6);
  SpaceFactory spacef(env);
  spacef.set_entity("x",IBEX_VAR);
  Space space(spacef);
  INTERVAL_VECTOR& box = space.box;

  for (int i=0; i<6; i++) box(i+1)=INTERVAL(0,0);

  BlockSet bs(6,4);

  try {
    BlockRoundRobin(space, bs, 1.0).bisect(-1);
    _test(0);
  } catch(BlockSetException e) {
    _test(e.type==BlockSetException::ALL_EMPTY);
  }

  bs.set_blk(0,0);
  bs.set_blk(3,0);
  bs.set_blk(5,0);

  bs.set_blk(1,1);

  bs.set_blk(2,3);
  bs.set_blk(4,3);

//   try {
//     BlockRoundRobin(space, bs, 1.0).bisect(-1);
//     _test(0);
//   } catch(BoxTooSmallException e) {
//     _test(1);
//   }
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(-1).var==0);

  box(0+1)=INTERVAL(0,2);
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(-1).var==0);
  box(0+1)=INTERVAL(0,0);
 
  // does it stay within the first block (must choose the second) ?
  box(1+1)=INTERVAL(0,2);
  box(3+1)=INTERVAL(0,2);
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(-1).var==3);  
  box(3+1)=INTERVAL(0,0);

  // does it stay within the first block (must choose the last) ?
  box(5+1)=INTERVAL(0,2);
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(-1).var==5);
  box(5+1)=INTERVAL(0,0);

  // does it roll back in the first block?
  box(0+1)=INTERVAL(0,2);
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(3).var==0);
  box(0+1)=INTERVAL(0,0);

  // does it proceed to the second block?
  box(1+1)=INTERVAL(0,2);
  // at start?
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(-1).var==1);
  // from block 1?
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(3).var==1);
  // does it rool back in the second block ?
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(1).var==1);
  box(1+1)=INTERVAL(0,0);

  // does it proceed to the fourth block?
  box(2+1)=INTERVAL(0,2);
  // at start?
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(-1).var==2);
  // from block 1?
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(3).var==2);
  // from block 2?
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(1).var==2);
  // does it roll back in the fourth block?
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(4).var==2);

  // can it jump two empty blocks?
  bs.rem_var(1);
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(-1).var==2);
  _test ( BlockRoundRobin(space, bs, 1.0).bisect(0).var==2);

  // can it detect a "last variable" which is not in a block?
  try {
    BlockRoundRobin(space, bs, 1.0).bisect(1);
    _test(0);
  } catch(BlockSetException e) {
    _test(e.type==BlockSetException::NO_BLK);
  }

  // can it jump empty blocks in the begining ?
  bs.rem_var(0);
  bs.rem_var(3);
  bs.rem_var(5);
  _test (BlockRoundRobin(space, bs, 1.0).bisect(-1).var==2);

  // can it jump empty blocks in the end ?
  bs.set_blk(0,0);
  bs.set_blk(3,0);
  bs.set_blk(5,0);

  bs.rem_var(2);
  bs.rem_var(4);
//   try {
//     BlockRoundRobin(space, bs, 1.0).bisect(-1);
//     _test(0);
//   } catch(BoxTooSmallException e) {
//     _test(1);
//   }
  _test(BlockRoundRobin(space, bs, 1.0).bisect(-1).var==0);


}
