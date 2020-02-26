#ifndef __IBEX_EXPR_DATA_AVM_H__
#define __IBEX_EXPR_DATA_AVM_H__

#include "ibex_Function.h"
#include "ibex_ExprData.h"

namespace ibex {

struct AVMData {
    int begin_index;
    int size;
    int end_index;
    int lin_count;
};

class ExprDataAVMFactory : public ExprDataFactory<AVMData> {
public:
    virtual AVMData* init(const ExprNode& e) override;
    int current_index;
};

class ExprDataAVM : public ExprData<AVMData> {
public:
    ExprDataAVM(const Function& f);

    int nb_var = 0;

private:
    ExprDataAVMFactory& create_avm_f();

    ExprDataAVMFactory* avm_f_;
};

} // namespace ibex

#endif  // __IBEX_EXPR_DATA_AVM_H__