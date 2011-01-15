#include "ParserFunction.h"

namespace ibex { 
namespace parser {

P_Function::~P_Function() {
  free((char*) name);
  free((char*) output);

  for (vector<const char*>::const_iterator it=inputs->begin(); it!=inputs->end(); it++)
    free((char*) *it);  
  delete inputs;

  for (vector<const P_Assign*>::const_iterator it=vec->begin(); it!=vec->end(); it++)
    delete *it;  
  delete vec;
}

} // end namespace
} // end namespace
