#ifndef __IBEX_CTC_AVM_H__
#define __IBEX_CTC_AVM_H__

#include "ibex_Ctc.h"
#include "ibex_AVM.h"

namespace ibex {

class CtcAVM : public Ctc {
public:

    CtcAVM(const Function& f);
    virtual ~CtcAVM();


    /**
	 * \brief Contraction.
	 */
	void contract(IntervalVector& box);
    void contract(IntervalVector& box, ContractContext& context);
private:
    AVM* avm_;

};

} // end namespace ibex

#endif  // __IBEX_CTC_AVM_H__