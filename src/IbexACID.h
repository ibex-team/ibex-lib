 #ifndef _ACID_CID
#define _ACID_CID

#include "IbexContractor.h"
#include "IbexSystem.h"
#include "Ibex3BCID.h"

namespace ibex {
/** \ingroup ctcgroup
 * \brief ACID contractor (Adpative version of 3BCID)
 *
 * This class is an implementation of the ACID algorithm, an adaptive version of the 3BCID  contractor defined by Gilles Trombettoni and Gilles Chabert in Proc. of CP, Constraint Programming, 2007.
The "handled" number of variables for which a shaving will be performed is adaptively tuned.
The ACID algorithm alternates 
 - small tuning phases (during e.g 50 nodes)  where the shaving is called on a number of variables 
double of the last tuned value (all variables in the first tuning phase);  Statistics are made in order to determine 
an average number of interesting calls,  The number of variables to be handled in the next running phase
is computed at the end of the tuning phase.

-  large running phases (during e.g. 950 nodes) 
where 3BCID is called with the  number of variables determined during the last tunig phase.
 * \author Bertrand Neveu , Gilles Trombettoni
 * \date June 2012
 */


class Adapt3BCID : public _3BCID {

  public :
    /** the handled constraint system */
    System system;

    /** the Adapt3BCID constructor : 
     *      the parameters are the same as for 3BCID . ctc, s3b, scid, var_min_width  except  vhandled which is now computed and no more a parameter 
     * \param  ct_ratio (for contracting ratio), a new parameter, at the kernel of the ACID algorithm : it indicates that variables are useful
     *to be shaved until all variable shavings yield average gains smaller than ct-ratio. the default value is 0.005, 
     * \param sys the constraint system useful to compute the order of the variables to be shaved.using a smearsumrel like (cf bisector) criterion

     */
    
  Adapt3BCID(const System& sys, Space& cid_space, Contractor& ctc, int s3b=default_s3b, int scid=default_scid,
	     REAL var_min_width=default_var_min_width, REAL ct_ratio=default_ctratio): Operator(ctc.space), _3BCID (cid_space,ctc,s3b,scid,sys.nb_var(),var_min_width), system(sys), nbcalls(0), nbctvar(0), nbcidvar(0) ,  nbtuning(0), ctratio(ct_ratio){ctc.contract_floor=-1;}
  Adapt3BCID(const Adapt3BCID& s) : Operator(s.space), _3BCID (s.cid_space, *s.ctc.copy(), s.s3b,s.scid,s.vhandled,s.var_min_width), system(s.system), nbcalls(s.nbcalls), nbctvar(s.nbctvar), ctratio(s.ctratio), nbcidvar(0) , nbtuning(0) {ctc.contract_floor=-1;}


  virtual Adapt3BCID* copy() const {
    return new Adapt3BCID(*this);
    
  }

  /** the contraction function */
  virtual void contract();
  REAL nbvar_stat();
  /** the average (on all tunings) of the  number of variables to be shaved  : result given at the end of the search*/
  static REAL nbvarstat;
  /** default ctratio value, set to 0.005 */
  static const REAL default_ctratio;
  protected :
    vector<int> smearorder;
    void compute_smearorder();
    int nbcalls;
    REAL nbctvar;
    REAL ctratio;
    int nbcidvar;
    int nbtuning;
  };
    

}
#endif
