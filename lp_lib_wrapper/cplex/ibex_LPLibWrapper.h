#ifndef _IBEX_LPLIBWRAPPER_H_
#define _IBEX_LPLIBWRAPPER_H_

#include "ilcplex/cplex.h"

#define IBEX_LPSOLVER_WRAPPER_ATTRIBUTES CPXENVptr  envcplex;\
                                         CPXLPptr lpcplex;\
                                         int * indice;\
                                         double * tmp;\
                                         int * r_matbeg;\
                                         double * r_matval;\
                                         int * r_matind 

#endif /* _IBEX_LPLIBWRAPPER_H_ */
