#include "ibex_ExprDataAVM.h"


namespace ibex {

#define IMPL_FACT_0ARY(type, n) \
AVMData* ExprDataAVMFactory::init(const type& e) {\
    const Dim& d = e.dim; \
    return create_data(d.size(), n); \
}

#define IMPL_FACT_UNARY(type, n) \
AVMData* ExprDataAVMFactory::init(const type& e, AVMData&) {\
    const Dim& d = e.dim; \
    return create_data(d.size(), n); \
}

#define IMPL_FACT_BINARY(type, n) \
AVMData* ExprDataAVMFactory::init(const type& e, AVMData&, AVMData&) {\
    const Dim& d = e.dim; \
    return create_data(d.size(), n); \
}

IMPL_FACT_0ARY(ExprSymbol, 0)
IMPL_FACT_0ARY(ExprConstant, 0)

//IMPL_FACT_UNARY(ExprIndex, 0);
IMPL_FACT_UNARY(ExprMinus, 1)
IMPL_FACT_UNARY(ExprSqr, 6)
IMPL_FACT_UNARY(ExprSqrt, 4)
IMPL_FACT_UNARY(ExprExp, 4)
IMPL_FACT_UNARY(ExprLog, 4)
IMPL_FACT_UNARY(ExprCos, 6)
IMPL_FACT_UNARY(ExprSin, 6)
IMPL_FACT_UNARY(ExprTanh, 6);
IMPL_FACT_BINARY(ExprAdd, 1)
IMPL_FACT_BINARY(ExprSub, 1)
IMPL_FACT_BINARY(ExprMul, 6)

#undef IMPL_FACT_UNARY
#undef IMPL_FACT_BINARY
#undef IMPL_FACT_0ARY

AVMData* ExprDataAVMFactory::create_data(int var_count, int lin_count) {
    AVMData* new_data = new AVMData;
    new_data->begin_index = current_index;
    new_data->size = var_count;
    current_index += var_count;
    new_data->end_index = current_index;
    new_data->begin_lin_index = current_lin_index;
    new_data->lin_count = lin_count;
    current_lin_index += lin_count;
    new_data->end_lin_index = current_lin_index;
    return new_data;
}


ExprDataAVM::ExprDataAVM(const Function& f) : ExprData<AVMData>(f, create_avm_f()) {
    nb_var = avm_f_->current_index;
    nb_lin = avm_f_->current_lin_index;
    delete avm_f_;
}

ExprDataAVMFactory& ExprDataAVM::create_avm_f() {
    avm_f_ = new ExprDataAVMFactory();
    return *avm_f_;
}

} // namespace ibex
