//============================================================================
//                                  I B E X
// File        : ibex_AmplInterface.cpp Adapted from CouenneAmplInterface
//				by Pietro Belotti, Leo Liberti and Sonia Caffieri
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Nov 5, 2013
// Last Update : Nov 5, 2013
//============================================================================

#include "ibex_AmplInterface.h"
#include "ibex_Exception.h"

#include "amplsolvers/asl.h"
#include "amplsolvers/nlp.h"
#include "amplsolvers/getstub.h"
#include "amplsolvers/opcode.hd"
#include <stdint.h>


#define OBJ_DE    ((const ASL_fg *) asl) -> I.obj_de_
#define VAR_E     ((const ASL_fg *) asl) -> I.var_e_
#define CON_DE    ((const ASL_fg *) asl) -> I.con_de_
#define OBJ_sense ((const ASL_fg *) asl) -> i.objtype_
#define CEXPS1 ((const ASL_fg *) asl) -> I.cexps1_
#define CEXPS ((const ASL_fg *) asl) -> I.cexps_

#include "amplsolvers/r_opn.hd" /* for N_OPS */

static fint timing = 0;

static
keyword keywds[] = { // must be alphabetical
		KW(const_cast<char*>("timing"), L_val, &timing, const_cast<char*>("display timings for the run")),
};

static
Option_Info Oinfo = { const_cast<char*>("testampl"), const_cast<char*>("ANALYSIS TEST"),
		const_cast<char*>("concert_options"), keywds, nkeywds, 0, const_cast<char*>("ANALYSIS TEST") };


/////////////////////////////////////////////////////////////////////////////////////////
/* $Id: invmap.cpp 577 2011-05-21 20:38:48Z pbelotti $
 *
 * Name:    invmap.cpp
 * Author:  Pietro Belotti
 * Purpose: create a bijection between ASL's efunc and integer to
 *          inversely map e->op fields into constant operators
 *
 * (C) Carnegie-Mellon University, 2006-11.
 * This file is licensed under the Eclipse Public License (EPL)
 */

/* couples an ASL function pointer with the relative operator constant */

typedef struct {
	efunc *fp;
	int    op;
} AslCouPair;


/* compare two AslCoupair's, used in qsort and bsearch below */

/* AW: 2007-06-11: changed b/c of problems with MSVC++ */
/* inline int pair_compare (const void *p1, const void *p2) { */
static int pair_compare (const void *p1, const void *p2) {

	/* FIX! weak cast for 64 bit machines */

	register size_t f1 = Intcast (((AslCouPair *) p1) -> fp);
	register size_t f2 = Intcast (((AslCouPair *) p2) -> fp);

	if      (f1 < f2) return -1;
	else if (f1 > f2) return  1;
	else return 0;
}


/* array of pairs (efunc2*, int) that relates all operators */

AslCouPair opmap [N_OPS];


/* binary search to get operator number from its efunc2* (the type of e->op) */

size_t getOperator (efunc *f) {

	static char first_call = 1;
	AslCouPair key, *res;

	/* FIX cast for 64 bit machines */

	if ((Intcast f <  N_OPS) &&	(Intcast f > -N_OPS))
		return Intcast f;

	key.fp = f;

	if (first_call) { /* opmap is still empty, fill it using values from r_ops [] */

		register int i=0;
		register AslCouPair *ops = opmap;

		/* fill opmap vector with inverse correspondence pairs efunc -> int */
		while (i<N_OPS) {
			ops -> fp = r_ops [ops -> op = i++];
			ops++;
		}

		/* sort opmap for later use with bsearch */
		qsort (opmap, N_OPS, sizeof (AslCouPair), pair_compare);
		first_call = 0;
	}

	/* find int operator through binary search */
	res = (AslCouPair *) bsearch (&key, opmap, N_OPS, sizeof (AslCouPair), pair_compare);

	if (!res)
		return -1;

	return res -> op;
}



// (C++) code starts here ///////////////////////////////////////////////////////////////////////////

namespace ibex {

//const double AmplInterface::default_max_bound= 1.e20;


AmplInterface::AmplInterface(std::string nlfile) : asl(NULL), _nlfile(nlfile), _x(NULL){

	if (!readASLfg()) {
		ibex_error("Fail to read the ampl file.\n");
	}

	if (!readnl()) {
		ibex_error("Fail to read the nl file.\n");
	}

}

AmplInterface::~AmplInterface() {
	if (_x) delete _x;

	var_data.clear();

	if (asl) {
		ASL_free(&asl);
	}
}

bool AmplInterface::writeSolution(double * sol, bool found) {
	const char* message;

	//TODO setup a nicer message
	if (found) {
		message = "IBEX found a solution.\n";
	} else {
		message = "IBEX could not found a solution.\n";
	}

	write_sol(const_cast<char*>(message), sol, NULL, NULL);

	return true;
}





// Reads a NLP from an AMPL .nl file through the ASL methods
bool AmplInterface::readASLfg() {
	assert(asl == NULL);

	if (_nlfile == "")
		return false;

	char** argv = new char*[3];
	argv[0] = const_cast<char*>("dummy");
	argv[1] = strdup(_nlfile.c_str());
	argv[2] = NULL;

	// Create the ASL structure
	asl = (ASL*) ASL_alloc (ASL_read_fg);

	char* stub = getstub (&argv, &Oinfo);

	// Although very intuitive, we shall explain why the second argument
	// is passed with a minus sign: it is to tell the ASL to retrieve
	// the nonlinear information too.
	FILE* nl = jac0dim (stub, - (fint) strlen (stub));

	// Set options in the asl structure
	want_xpi0 = 1 | 2;  // allocate initial values for primal and dual if available
	obj_no = 0;         // always want to work with the first (and only?) objective

	// read the rest of the nl file
	fg_read (nl, ASL_return_read_err | ASL_findgroups);

	//FIXME freeing argv and argv[1] gives segfault !!!
	//  free(argv[1]);
	//  delete[] argv;

	return true;
}




// Reads a NLP from an AMPL .nl file through the ASL methods
bool AmplInterface::readnl() {


	// the variable /////////////////////////////////////////////////////////////
	// TODO only continuous variables for the moment
	_x =new Variable(n_var,"x");
	IntervalVector bound(n_var);

		// Each has a linear and a nonlinear part
		// thanks to Dominique Orban:
		//        http://www.gerad.ca/~orban/drampl/def-vars.html
		//        http://www.gerad.ca/~orban/drampl/dag.html

	try {

	// lower and upper bounds of the variables ///////////////////////////////////////////////////////////////
		if (LUv) {
			real *Uvx_copy = Uvx;
			// LUv is the variable lower bound if Uvx!=0, and the variable lower and upper bound if Uvx == 0
			if (!Uvx_copy)
				for (int i=0; i<n_var; i++) {
					//bound[i] = Interval(  ((LUv[2*i] <= -default_max_bound) ? -default_max_bound : LUv[2*i] ),
						//						((LUv[2*i+1] >= default_max_bound) ? default_max_bound : LUv[2*i+1]) );
					bound[i] = Interval( LUv[2*i], LUv[2*i+1]);
				}
			else
				for (int i=n_var; i--;) {
					//bound[i] = Interval(	(LUv [i] <= -default_max_bound ? -default_max_bound : LUv[i] ),
						//						(Uvx_copy [i] >= default_max_bound ? default_max_bound : Uvx_copy[i]) );
					bound[i] = Interval( LUv[i], Uvx_copy[i]);
				}
		} // else it is [-oo,+oo]
		add_var(*_x, bound);

	// objective functions /////////////////////////////////////////////////////////////
		if (n_obj>1) {ibex_error("Error AmplInterface: too much objective function in the ampl model."); return false;}

		for (int i = 0; i < n_obj; i++) {
			///////////////////////////////////////////////////
			//  the nonlinear part
			const ExprNode *body = &(nl2expr (OBJ_DE [i] . e));

			////////////////////////////////////////////////
			// The linear part
			// count nonzero terms in linear part
			double coeff;
			int index;
			for (ograd *objgrad = Ograd [i]; objgrad; objgrad = objgrad -> next) {
				if ((dynamic_cast<const ExprConstant*>(body))&&(((ExprConstant*)(body))->is_zero())) {
					delete body;
					if (fabs (objgrad -> coef) != 0.0) {
						coeff = objgrad -> coef;
						index = objgrad -> varno;
						if (coeff==1) {
							body = &((*_x)[index]);
						} else if (coeff==-1) {
							body = &( - (*_x)[index]);
						} else {
							body = &(coeff * (*_x)[index]);
						}
					}
				} else {
					if (fabs (objgrad -> coef) != 0.0) {
						coeff = objgrad -> coef;
						index = objgrad -> varno;
						if (coeff==1) {
							body = &(*body + (*_x)[index]);
						} else if (coeff==-1) {
							body = &(*body - (*_x)[index]);
						} else {
							body = &(*body +coeff * (*_x)[index]);
						}
					}
				}
			}

			////////////////////////////////////////////////
			// Max or Min
			// 3rd/ASL/solvers/asl.h, line 336: 0 is minimization, 1 is maximization
			if (OBJ_sense [i] == 0) {
				add_goal(*body);
			} else {
				add_goal(-(*body));
			}
		}

	// constraints ///////////////////////////////////////////////////////////////////
		// allocate space for argument list of all constraints' summations
		// of linear and nonlinear terms
		const ExprNode **body_con = new const ExprNode*[n_con];
		///////////////////////////////////////////////////
		// The nonlinear part :
		//init array of each constraint with the nonlinear part
		for (int i = 0; i<n_con;i++)
			body_con[i] = &(nl2expr (CON_DE [i] . e));

		///////////////////////////////////////////////////
		// count all linear terms
		if (A_colstarts && A_vals)    {      // Constraints' linear info is stored in A_vals
			for (int j = 0; j < n_var; j++){
				for (int i = A_colstarts [j], k = A_colstarts [j+1] - i; k--; i++) {
					if ((dynamic_cast<const ExprConstant*>(body_con[A_rownos[i]]))&&(((ExprConstant*)(body_con[A_rownos[i]]))->is_zero())) {
						delete body_con[A_rownos[i]];
						if (A_vals[i]==1) {
							body_con[A_rownos[i]] = &((*_x)[j]);
						} else if (A_vals[i]==-1) {
							body_con[A_rownos[i]] = &(- (*_x)[j]);
						} else {
							body_con[A_rownos[i]] = &((A_vals[i]) * (*_x)[j]);
						}
					} else {
						if (A_vals[i]==1) {
							body_con[A_rownos[i]] = &(*(body_con[A_rownos[i]]) + (*_x)[j]);
						} else if (A_vals[i]==-1) {
							body_con[A_rownos[i]] = &(*(body_con[A_rownos[i]]) - (*_x)[j]);
						} else {
							body_con[A_rownos[i]] = &(*(body_con[A_rownos[i]]) + (A_vals[i]) * (*_x)[j]);
						}
					}

				}
			}
		} else {		// Constraints' linear info is stored in Cgrad
			cgrad *congrad;
			double coeff;
			int index;
			for ( int i = 0; i < n_con; i++)
				for (congrad = Cgrad [i]; congrad; congrad = congrad -> next) {
					if ((dynamic_cast<const ExprConstant*>(body_con[i]))&&(((ExprConstant*)(body_con[i]))->is_zero())) {
						delete body_con[i];
						coeff = congrad -> coef;
						if (fabs (coeff) != 0.0) {
							index = congrad -> varno;
							if (coeff==1) {
								body_con[i] = &( (*_x)[index]);
							} else if (coeff==-1) {
								body_con[i] = &(-(*_x)[index]);
							} else {
								body_con[i] = &((coeff) * (*_x)[index]);
							}
						}

					} else {
						coeff = congrad -> coef;
						if (fabs (coeff) != 0.0) {
							index = congrad -> varno;
							if (coeff==1) {
								body_con[i] = &(*(body_con[i]) + (*_x)[index]);
							} else if (coeff==-1) {
								body_con[i] = &(*(body_con[i]) - (*_x)[index]);
							} else {
								body_con[i] = &(*(body_con[i]) + (coeff) * (*_x)[index]);
							}
						}
					}
				}
		}
		///////////////////////////////////////////////////
		// Kind of constraints : equality, inequality
		for (int i = 0; i < n_con; i++) {
			int sig;
			double lb, ub;

			/* LUrhs is the constraint lower bound if Urhsx!=0, and the constraint lower and upper bound if Uvx == 0 */
			if (Urhsx) {
				lb = LUrhs [i];
				ub = Urhsx [i];
			} else {
				lb = LUrhs [2*i];
				ub = LUrhs [2*i+1];
			}

			// set constraint sign
			if (negInfinity < lb)
				if (ub < Infinity)  sig =1; // EQ;
				else                sig =2; // GEQ;
			else                    sig =3; // LEQ;

			// add them (and set lower-upper bound)
			switch (sig) {

			case  1:  {
				if (lb==ub) {
					if (lb==0) {
						add_ctr_eq((*(body_con[i])));
					} else if (lb<0) {
						add_ctr_eq((*(body_con[i])+(-lb)));
					} else {
						add_ctr_eq((*(body_con[i])-lb));
					}
				} else  {
					 add_ctr_eq((*(body_con[i])-Interval(lb,ub)));
				}
				break;
			}
			case  2:  {
				if (lb==0) {
					add_ctr(ExprCtr(*(body_con[i]),GEQ));
				} else if (lb<0) {
					add_ctr(ExprCtr(*(body_con[i])+(-lb),GEQ));
				} else {
					add_ctr(ExprCtr(*(body_con[i])-lb,GEQ));
				}
				break;
			}
			case  3: {
				if (ub==0) {
					add_ctr(ExprCtr(*(body_con[i]),LEQ));
				} else if (ub<0) {
					add_ctr(ExprCtr(*(body_con[i])+(-ub),LEQ));
				} else {
					add_ctr(ExprCtr(*(body_con[i])-ub,LEQ));
				}
				break;
			}
			default: ibex_error("Error: could not recognize a constraint\n"); return false;
			}
		}


		delete[] body_con;

	} catch (...) {
		return false;
	}

	return true;
}

// converts an AMPL expression (sub)tree into an expression* (sub)tree
// thank to Dominique Orban for the explication of the DAG inside AMPL:
// http://www.gerad.ca/~orban/drampl/dag.html
const ExprNode& AmplInterface::nl2expr(expr *e) {

	switch (getOperator (e -> op)) {

	case OPNUM:    return  (ExprConstant::new_scalar(((expr_n *)e)->v));
	case OPPLUS:   {
		if (getOperator(e->R.e->op)==OPUMINUS) {
			return  (( nl2expr(e -> L.e)) - nl2expr((e->R.e)->L.e) );
		} else {
			if (getOperator(e->L.e->op)==OPUMINUS) {
				return  (nl2expr(e->R.e) - nl2expr((e->L.e)->L.e) );
			} else {
				return  (nl2expr(e->R.e) + nl2expr(e->L.e) );
			}
		}
	}
	case OPMINUS:  {
		if (getOperator(e->R.e->op)==OPUMINUS)  {
			return (( nl2expr(e->L.e)) + nl2expr((e->R.e)->L.e) );
		} else {
			return  (( nl2expr(e->L.e)) - nl2expr(e->R.e));
		}
	}
	case OPDIV:    return  (( nl2expr(e -> L.e)) / (nl2expr(e -> R.e)));
	case OPMULT:   return  (operator*((nl2expr (e -> L.e)) , (nl2expr(e -> R.e))));
	case OPPOW:    return  pow( nl2expr(e -> L.e), nl2expr(e -> R.e));
	case OP1POW:   {
		if (((int) (((expr_n *)e->R.e)->v) )==(((expr_n *)e->R.e)->v)) {
			return pow( nl2expr(e -> L.e), (int) (((expr_n *)e->R.e)->v));
		} else
			return pow( nl2expr(e -> L.e), ExprConstant::new_scalar(((expr_n *)e->R.e)->v));
	}
	case OP2POW:   return sqr( nl2expr(e -> L.e));
	case OPCPOW:   return pow(ExprConstant::new_scalar(((expr_n *)e->L.e)->v), nl2expr(e -> R.e));
	case MINLIST: {
		expr **ep = e->L.ep;
		const ExprNode* ee = &(nl2expr(*ep));
		ep++;
		while (ep < e->R.ep) {
			ee = &(min(*ee , nl2expr(*ep)));
			ep++;
		}
		return *ee;
	}
	case MAXLIST:  {
		expr **ep = e->L.ep;
		const ExprNode* ee = &(nl2expr(*ep));
		ep++;
		while (ep < e->R.ep) {
			ee = &(max(*ee , nl2expr(*ep)));
			ep++;
		}
		return *ee;
	}
	case OPSUMLIST: {
		expr **ep = e->L.ep;
		const ExprNode* ee = &(nl2expr(*ep));
		ep++;
		while (ep < e->R.ep) {
			if (getOperator((*ep)->op)==OPUMINUS) {
				ee = &(*ee - nl2expr((*ep)->L.e) );
			} else {
				ee = &(*ee + nl2expr(*ep) );
			}
			ep++;
		}
		return *ee;
	}
	case ABS:      return abs( nl2expr(e -> L.e));
	case OPUMINUS: {
		if (getOperator(e->L.e->op)==OPUMINUS)  {
			return  (nl2expr((e -> L.e)->L.e));
		} else {
			return  (operator-(nl2expr(e -> L.e)));
		}
	}
	case OP_sqrt:  return sqrt(nl2expr(e -> L.e));
	case OP_exp:   return exp( nl2expr(e -> L.e));
	case OP_log:   return log( nl2expr(e -> L.e));
	case OP_log10: return ((ExprConstant::new_scalar(1.0/log(Interval(10.0)))) * log(nl2expr (e -> L.e)));
	case OP_cos:   return cos( nl2expr(e -> L.e));
	case OP_sin:   return sin( nl2expr(e -> L.e));
	case OP_tan:   return tan( nl2expr(e -> L.e));
	case OP_cosh:  return cosh(nl2expr(e -> L.e));
	case OP_sinh:  return sinh(nl2expr(e -> L.e));
	case OP_tanh:  return tanh(nl2expr(e -> L.e));
	case OP_acos:  return acos(nl2expr(e -> L.e));
	case OP_asin:  return asin(nl2expr(e -> L.e));
	case OP_atan:  return atan(nl2expr(e -> L.e));
	//case OP_asinh: notimpl ("asinh");
	//case OP_acosh: notimpl ("acosh");
	//case OP_atanh: notimpl ("atanh");
	case OP_atan2: return atan2(nl2expr(e -> L.e), nl2expr(e -> R.e));
	//case OPintDIV: notimpl ("intdiv");
	//case OPprecision: notimpl ("precision");
	//case OPround:  notimpl ("round");
	//case OPtrunc:  notimpl ("trunc");
	//case FLOOR:   notimpl ("floor");
	//case CEIL:    notimpl ("ceil");
	//case OPFUNCALL: notimpl ("function call");
	//case OPPLTERM:  notimpl ("plterm");
	//case OPIFSYM:   notimpl ("ifsym");
	//case OPHOL:     notimpl ("hol");
	//case OPREM:   notimpl ("remainder");
	//case OPLESS:  notimpl ("less");
	//case OPIFnl:  // TODO return (chi(nl2expr(????))) BoolInterval??
	                // see ASL/solvers/rops.c, see f_OPIFnl and  expr_if
	case OPVARVAL:  {
		int j = ((expr_v *) e) -> a;
		if (j<n_var) {
			return (*_x)[j];
		}
		else {
			// http://www.gerad.ca/~orban/drampl/def-vars.html
			// common expression | defined variable
			int k = (expr_v *)e - VAR_E;
			double coeff;
			int index;

			if( k >= n_var ) {
				const ExprNode* body;
				// This is a common expression. Find pointer to its root.

				// Check if the common expression are already construct
				if (var_data.find(k)!=var_data.end()) {
					body = var_data[k];
				}
				else {

					// Constract the common expression
					j = k - n_var;
					if( j < ncom0 ) 	{
						cexp *common = CEXPS +j;
						// init with the nonlinear part
						body = &(nl2expr(common->e));

						int nlin = common->nlin; // Number of linear terms
						if( nlin > 0 ) {
							linpart * L = common->L;
							for(int i = 0; i < nlin; i++ ) {
								if ((dynamic_cast<const ExprConstant*>(body))&&(((ExprConstant*)(body))->is_zero())) {
									coeff = (L[i]).fac;
									index = ((uintptr_t) (L[i].v.rp) - (uintptr_t) VAR_E) / sizeof (expr_v);
									if (coeff==1) {
										body = &((*_x)[index]);
									} else if (coeff==-1) {
										body = &( - (*_x)[index]);
									} else if (coeff != 0) {
										body = &(coeff * (*_x)[index]);
									}
								} else {
									coeff = (L[i]).fac;
									index = ((uintptr_t) (L[i].v.rp) - (uintptr_t) VAR_E) / sizeof (expr_v);
									if (coeff==1) {
										body = &(*body + (*_x)[index]);
									} else if (coeff==-1) {
										body = &(*body - (*_x)[index]);
									} else if (coeff != 0) {
										body = &(*body +coeff * (*_x)[index]);
									}
								}
							}
						}
					}
					else {
						cexp1 *common = (CEXPS1 - ncom0) +j ;
						// init with the nonlinear part
						body = &(nl2expr(common->e));

						int nlin = common->nlin; // Number of linear terms
						if( nlin > 0 ) {
							linpart * L = common->L;
							for(int i = 0; i < nlin; i++ ) {
								if ((dynamic_cast<const ExprConstant*>(body))&&(((ExprConstant*)(body))->is_zero())) {
									coeff = (L[i]).fac;
									index = ((uintptr_t) (L[i].v.rp) - (uintptr_t) VAR_E) / sizeof (expr_v);
									if (coeff==1) {
										body = &( (*_x)[index]);
									} else if (coeff==-1) {
										body = &( - (*_x)[index]);
									} else if (coeff != 0) {
										body = &(coeff * (*_x)[index]);
									}
								} else {
									coeff = (L[i]).fac;
									index = ((uintptr_t) (L[i].v.rp) - (uintptr_t) VAR_E) / sizeof (expr_v);
									if (coeff==1) {
										body = &(*body + (*_x)[index]);
									} else if (coeff==-1) {
										body = &(*body - (*_x)[index]);
									} else if (coeff != 0) {
										body = &(*body +coeff * (*_x)[index]);
									}

								}
							}
						}
					}
					var_data[k] = body;;
				}
				return *body;

			} else {
				ibex_error("Error AmplInterface: unknown defined variable \n");
				throw -1;
			}
		}
	}

	default: {
		ibex_error( "Error AmplInterface: unknown operator or not implemented \n");
		throw -2;
	}
	}
	return ExprConstant::new_scalar(0.);
}


}

