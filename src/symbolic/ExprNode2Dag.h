/*
 * ExprNode2Dag.h
 *
 *  Created on: 4 oct. 2017
 *      Author: iaraya
 */

#ifndef SYMBOLIC_EXPRNODE2DAG_H_
#define SYMBOLIC_EXPRNODE2DAG_H_

#define CAST(m,n) dynamic_cast<const m*>(n)
#define LEFT(n) &CAST(ExprBinaryOp,n)->left
#define RIGHT(n) &CAST(ExprBinaryOp,n)->right
#define GET_VALUE(n) CAST(ExprConstant,n)->get_value()
#define GET_EXPR(n) &dynamic_cast<const ExprUnaryOp*> (n)->expr
#define NULL 0

#include <map>

using namespace std;

namespace ibex {


enum{CONSTANT=-1, SYM, EXP, LOG, COS, SIN, TAN, COSH, SINH, TANH, ACOS, ASIN, ATAN, ACOSH, ASINH, ATANH, ABS, POWER, SIGN, SQR,
SQRT, TRANS, ADD, ATAN2, MUL, MAX, MIN, INDEX};

struct lt{
	/*
	* \brief This function returns the type of the node (e.g. sin=4, cos=3 , etc)
	*/
	static int op(const ExprNode& node_expr){

	    int subclass=-3;

	    if (dynamic_cast<const ExprExp*> (&node_expr))
	        subclass = EXP;
	    else if (dynamic_cast<const ExprLog*> (&node_expr))
	        subclass = 2;
	    else if (dynamic_cast<const ExprCos*> (&node_expr))
	        subclass = 3;
	    else if (dynamic_cast<const ExprSin*> (&node_expr))
	        subclass = 4;
	    else if (dynamic_cast<const ExprTan*> (&node_expr))
	        subclass = 5;
	    else if (dynamic_cast<const ExprCosh*> (&node_expr))
	        subclass = 6;
	    else if (dynamic_cast<const ExprSinh*> (&node_expr))
	        subclass = 7;
	    else if (dynamic_cast<const ExprTanh*> (&node_expr))
	        subclass = 8;
	    else if (dynamic_cast<const ExprAcos*> (&node_expr))
	        subclass = 9;
	    else if (dynamic_cast<const ExprAsin*> (&node_expr))
	        subclass = 10;
	    else if (dynamic_cast<const ExprAtan*> (&node_expr))
	        subclass = 11;
	    else if (dynamic_cast<const ExprAcosh*> (&node_expr))
	        subclass = 12;
	    else if (dynamic_cast<const ExprAsinh*> (&node_expr))
	        subclass = 13;
	    else if (dynamic_cast<const ExprAtanh*> (&node_expr))
	        subclass = 14;
	    else if (dynamic_cast<const ExprAbs*> (&node_expr))
	        subclass = 15;
	    else if (dynamic_cast<const ExprPower*> (&node_expr))
	        subclass = 16;
	    else if (dynamic_cast<const ExprMinus*> (&node_expr))
	        subclass = ADD;
	    else if (dynamic_cast<const ExprSign*> (&node_expr))
	        subclass = 17;
	    else if (dynamic_cast<const ExprSqr*> (&node_expr))
	        subclass = 18;
	    else if (dynamic_cast<const ExprSqrt*> (&node_expr))
	        subclass = SQRT;
	    else if (dynamic_cast<const ExprTrans*> (&node_expr))
	        subclass = TRANS;
	    else if (dynamic_cast<const ExprAdd*> (&node_expr))
	        subclass = ADD;
	    else if (dynamic_cast<const ExprAtan2*> (&node_expr))
	        subclass = ATAN2;
	    else if (dynamic_cast<const ExprDiv*> (&node_expr))
	        subclass = MUL;
	    else if (dynamic_cast<const ExprMax*> (&node_expr))
	        subclass = MAX;
	    else if (dynamic_cast<const ExprMin*> (&node_expr))
	        subclass = MIN;
	    else if (dynamic_cast<const ExprMul*> (&node_expr))
	        subclass = MUL;
	    else if (dynamic_cast<const ExprSub*> (&node_expr))
	        subclass = ADD;
	    else if (dynamic_cast<const ExprConstant*> (&node_expr))
	        subclass = CONSTANT;
	    else if (dynamic_cast<const ExprSymbol*> (&node_expr))
	        subclass = SYM;
	    else if(dynamic_cast<const ExprIndex*> (&node_expr))
	    	subclass = INDEX;
	    return subclass;
	}

    bool operator()(const ExprNode* expr1, const ExprNode* expr2){


		if(expr1 == expr2) return false;

		if(!expr1) return true;
		if(!expr2) return false;

		if(op(*expr1)< op(*expr2)) return true;
		if(op(*expr2)< op(*expr1)) return false;

		//constant
		if(op(*expr1)==CONSTANT) {
			if(GET_VALUE(expr1).lb()<(GET_VALUE(expr2)).lb()) return true;
			if(GET_VALUE(expr1).lb()>(GET_VALUE(expr2)).lb()) return false;
			if(GET_VALUE(expr1).ub()<(GET_VALUE(expr2)).ub()) return true;
			return false;
		}

		if(op(*expr1)==SYM){
			if( strcmp(CAST(ExprSymbol,expr1)->name,  CAST(ExprSymbol,expr2)->name)<0) return true;
			return false;
		}

		if(op(*expr1)==INDEX){
			if( strcmp(CAST(ExprSymbol,&CAST(ExprIndex,expr1)->expr)->name,  CAST(ExprSymbol,&CAST(ExprIndex,expr2)->expr)->name)<0) return true;
			if( strcmp(CAST(ExprSymbol,&CAST(ExprIndex,expr1)->expr)->name,  CAST(ExprSymbol,&CAST(ExprIndex,expr2)->expr)->name)>0) return false;

			if( CAST(ExprIndex,expr1)->index.last_row()< CAST(ExprIndex,expr2)->index.last_row()) return true;
			return false;
		}



		if(op(*expr1)==POWER){
			if(lt()(GET_EXPR(expr1), GET_EXPR(expr2))) return true;
			if(lt()(GET_EXPR(expr2), GET_EXPR(expr1))) return false;

			if(CAST(ExprPower,expr1)->expon <CAST(ExprPower,expr2)->expon) return true;
			return false;
		}

		if(CAST(ExprUnaryOp,expr1))
			return lt()(GET_EXPR(expr1), GET_EXPR(expr2));

		if(CAST(ExprBinaryOp,expr1)){

			if(lt()(LEFT(expr1), LEFT(expr2))) return true;
			if(lt()(LEFT(expr2), LEFT(expr1))) return false;

			if(lt()(RIGHT(expr1), RIGHT(expr2))) return true;
			return false;
		}




		//both expressions are considered equal

		return false;
    }
};


class ExprNode2Dag : public ExprCopy {

public:
	/*
	 * \brief Duplicate an expression (with new symbols) and replace a set of common sub-expression
	 *        by a single node.
	 *
	 * \param old_vars - symbols in the input expression
	 * \param new_vars - symbols in the result expression. Symbols in \a old_vars are matched to symbols in \a new_vars
	 *                   with respect to their order.
	 * \param expr     - the expression
	 */
	const ExprNode& transform(const Array<const ExprSymbol>& old_vars,
			const Array<const ExprSymbol>& new_vars, const ExprNode& expr) {

		old2new.clear();
		const ExprNode* a= & ExprCopy::copy(old_vars,new_vars,expr);

		/*for(map< const ExprNode*, const ExprNode*, lt >::iterator it=old2new.begin(); it!=old2new.end();it++){
			cout << *it->first << ":" << it->first->id ; cout << endl;;
		}*/

		return *a;

	}

	static const ExprNode& generate(System& sys, Array<const ExprSymbol>& new_vars2);

	/*
	 * \brief
	 */
	static void get_add_children(const ExprNode& nn, map<const ExprNode*,Interval,lt>& children, Interval sign=Interval(1.0));

	static void get_mul_children(const ExprBinaryOp& op2, map< const ExprNode*, Interval, lt> & children, Interval sig=Interval(1.0));


	protected:

	/*
	*  \brief updates children[e] <-- children[e]*a
	*/
	static void mult(map<const ExprNode*,Interval,lt>& children, Interval a, const ExprNode* e){
		if(children.find(e)==children.end()) children[e]=a;
		else children[e]*=a;
	}

	/*
	*  \brief updates children[e] <-- children[e]+a
	*/
	static void add(map<const ExprNode*,Interval,lt>& children, Interval a, const ExprNode* e){

		if(children.find(e)==children.end()) children[e]=a;
		else children[e]+=a;
	}

	const ExprNode* add_term(const ExprNode* n, const ExprNode* e, bool add){
		e=&get_set_dagnode(*e);

		if(n){
			n=&get_set_dagnode(*n);
			if(add) n= &(*e + *n);
			else n= &(*e * *n);
		}
		else n=e;
		return n;
	}


	const ExprNode& generate_expression(const ExprNode& e);

    const ExprNode& get_set_dagnode(const ExprNode& e){
		if(old2new.find(&e)==old2new.end()){
			//cout << "new_node:" << e <<endl;
			old2new[&e] = &e;
			return e;
		}
		return *old2new[&e];
    }

	void visit(const ExprNode& e);


	map< const ExprNode*, const ExprNode*, lt > old2new;

};



} /* namespace ibex */

#endif /* SYMBOLIC_EXPRNODE2DAG_H_ */
