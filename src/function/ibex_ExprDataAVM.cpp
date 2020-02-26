#include "ibex_ExprDataAVM.h"

namespace ibex {

AVMData* ExprDataAVMFactory::init(const ExprNode& e) {
    const Dim& d = e.dim;
    AVMData* new_data = new AVMData;
    new_data->begin_index = current_index;
    new_data->size = d.size();
    new_data->end_index = current_index + d.size();
    new_data->lin_count = 1;
    current_index += d.size();
    return new_data;
}

ExprDataAVM::ExprDataAVM(const Function& f) : ExprData<AVMData>(f, create_avm_f()) {
    nb_var = avm_f_->current_index;
    delete avm_f_;
}

ExprDataAVMFactory& ExprDataAVM::create_avm_f() {
    avm_f_ = new ExprDataAVMFactory();
    return *avm_f_;
}

} // namespace ibex