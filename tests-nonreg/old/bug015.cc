


/* Fichier d'interface entre IBB et IBEX */




#include "IbexSystemFactory.h"
#include "IbexSystem.h"
#include "IbexMultiSystem.h"
#include "IbexBisection.h"
#include "IbexPropagation.h"
#include "IbexNewton.h"
#include "IbexBisector.h"
#include "IbexPaver.h"
#include "IbexShaving.h"

#include <fenv.h>
#include<math.h>
#define EPSILON 1e-7

using namespace ibex;

class BlockContract : public Composite {
public:
  BlockContract(System& sys, CompositeFactory& f, BlockSet& bs, REAL loc_prec) : 
    Operator(f), Composite(f), propag(sys), bs(bs), loc_prec(loc_prec) { }
 
 BlockContract(Propagation& propag, CompositeFactory& f, BlockSet& bs, REAL loc_prec) : 
    Operator(f), Composite(f), propag(propag), bs(bs), loc_prec(loc_prec) {  } 

  virtual bool contract(Cell& c, const Param&) throw(UnfeasibilityException) {
    int blk=c.root() ? 0 : bs.blk(c.last_split_var);  
    bool local_solution = true;
    
    while(blk<bs.nb_blk && local_solution) {

      ctr(blk).contract(space);
      local_solution = ctr(blk).space.box.max_diameter()<=loc_prec;
      
     propag.contract();
      
      blk++;
    }
    return true;
  }

  BlockContract(const BlockContract& bc) : Operator(bc.space), Composite(bc), propag(bc.propag), bs(bc.bs),
						   loc_prec(bc.loc_prec) { }
						   

  virtual BlockContract* copy() const {
    return new BlockContract(*this);
  }

  Propagation propag;
  BlockSet bs;
  REAL loc_prec;
};


int main(int argc, char* argv[]) {

  /* Loads a system from the file */
  System sysblock("bloc14");


  int  sol_n=0;


  //sysblock.set_output_flags (31);

//   cout << " sysblock " << sysblock << endl;
//   cout << "sysblock space" << sysblock.space << endl;
  
  // le bloc courant (seul a être bissecté)
  BlockSet ibb2(sysblock.nb_var(),1);  
  
  for (int j=0; j<14; j++)
    //  {cout << varsp[j]->name << " " << sysblock.space.var(varsp[j]->name) << endl;
    //    ibb2.set_blk(sysblock.space.var(varsp[j]->name),0);
    ibb2.set_blk(j,0);
    
//   cout << " avant multi " << endl;
//   cout << "ibb2 " << ibb2 << endl;

  try {
    MultiSystem multi(Decomposer(sysblock,ibb2));
      

  //  MultiSystem multi(Decomposer(sysblock,ibb2));
//   cout << " decomp " << multi.subsys(0) << endl;
//   cout << "multi.subsys(0).space" << multi.subsys(0).space << endl;  
  BasicCompositeFactory fac(multi.space);
  for (int k=0; k<ibb2.nb_blk; k++) {
    fac.add_ctr(Propagation(multi.subsys(k),0.3,true) & Newton(multi.subsys(k),0.01));
  }

double wbiss = 1.e-8;
/*
  SetIterator <int> Sit(parent_numbers) ; int* si1;
  
  
  while (si1=Sit.next())
    {// cout << wbiss << " " << *si1 << " " << D[*si1].intervalsize << endl;
         if (wbiss < D[*si1].intervalsize) wbiss = D[*si1].intervalsize;
    }
*/
  Propagation propag(sysblock,0.3,true);

  BlockContract bc(sysblock,fac,ibb2,wbiss);

 /* je crée le contracteur de précision adapté à 1 seul bloc
    (je précise simplement que son espace de travail est celui du
sous-système 0) */
  Precision prec(multi.subsys(0).space,wbiss);

/* J'instancie le paver en lui donnant une liste de contracteurs,
puisque je ne veux
   plus utiliser le contracteur de précision par défaut */
  vector<const Contractor*> ctc;
  ctc.push_back(&bc);
  ctc.push_back(&prec);

  BlockRoundRobin brr(sysblock.space, ibb2);

  Paver solver(sysblock.space, ctc, brr);




  solver.capacity=100000;



  solver.explore();


  
  solver.report();


  
     sysblock.space.box = solver.box(1,0);
    multi.subsys(0).space.sync(sysblock.space);
     try {
       prec.contract();
     cout << "[FAILED] Box larger than 1e-08\n";
  } catch(UnfeasibilityException) {
    cout << "[PASSED] Box smaller than 1e-08\n";
  }
 
  }  

  catch(NonRecoverableException e) {
	cout << e << endl;
     

  }
}



/** Doit afficher
[PASSED] Box smaller than 1e-08
*/
