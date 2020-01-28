#ifndef _IBEX_LPLIBWRAPPER_H_
#define _IBEX_LPLIBWRAPPER_H_

#include "ilcplex/ilocplex.h"
#include <unordered_map>

#define IBEX_LPSOLVER_WRAPPER_ATTRIBUTES IloEnv env; \
                                         IloModel model; \
                                         IloCplex cplex; \
                                         IloNumVarArray x; \
                                         IloObjective cost_; \
                                         IloRangeArray constraints; \
                                         std::unordered_map<IloInt, int> variableIdToIndex; \
                                         ibex::Vector linearit2ivec(int n, IloExpr::LinearIterator it) const;

#endif /* _IBEX_LPLIBWRAPPER_H_ */
