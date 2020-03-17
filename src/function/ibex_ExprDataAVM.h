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
    int begin_lin_index;
    int end_lin_index;
};

class ExprDataAVMFactory : public ExprDataFactory<AVMData> {
public:
    //virtual AVMData* init(const ExprIndex& e) override;
    virtual AVMData* init(const ExprSymbol& e) override;
    virtual AVMData* init(const ExprConstant& e) override;
    virtual AVMData* init(const ExprMinus& e, AVMData&) override;
    virtual AVMData* init(const ExprSqr& e, AVMData&) override;
    virtual AVMData* init(const ExprDiv& e, AVMData&, AVMData&) override;
    virtual AVMData* init(const ExprSqrt& e, AVMData&) override;
    virtual AVMData* init(const ExprExp& e, AVMData&) override;
    virtual AVMData* init(const ExprLog& e, AVMData&) override;
    virtual AVMData* init(const ExprCos& e, AVMData&) override;
    virtual AVMData* init(const ExprAcos& e, AVMData&) override;
    virtual AVMData* init(const ExprSin& e, AVMData&) override;
    virtual AVMData* init(const ExprAsin& e, AVMData&) override;
    virtual AVMData* init(const ExprSinh& e, AVMData&) override;
    virtual AVMData* init(const ExprAsinh& e, AVMData&) override;
    virtual AVMData* init(const ExprCosh& e, AVMData&) override;
    virtual AVMData* init(const ExprAcosh& e, AVMData&) override;
    virtual AVMData* init(const ExprTan& e, AVMData&) override;
    virtual AVMData* init(const ExprAtan& e, AVMData&) override;
    virtual AVMData* init(const ExprTanh& e, AVMData&) override;
    virtual AVMData* init(const ExprAtanh& e, AVMData&) override;
    virtual AVMData* init(const ExprPower& e, AVMData&) override;
    virtual AVMData* init(const ExprAbs& e, AVMData&) override;
    virtual AVMData* init(const ExprSign& e, AVMData&) override;
    virtual AVMData* init(const ExprAdd&, AVMData&, AVMData&) override;
    virtual AVMData* init(const ExprSub&, AVMData&, AVMData&) override;
    virtual AVMData* init(const ExprMul&, AVMData&, AVMData&) override;
    int current_index;
    int current_lin_index;

    AVMData* create_data(int var_count, int lin_count);
};

class ExprDataAVM : public ExprData<AVMData> {
public:
    ExprDataAVM(const Function& f);

    int nb_var = 0;
    int nb_lin = 0;

private:
    ExprDataAVMFactory& create_avm_f();

    ExprDataAVMFactory* avm_f_;
};

} // namespace ibex

#endif  // __IBEX_EXPR_DATA_AVM_H__