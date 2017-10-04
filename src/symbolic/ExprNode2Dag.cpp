/*
 * ExprNode2Dag.cpp
 *
 *  Created on: 4 oct. 2017
 *      Author: iaraya
 */

#include "ExprNode2Dag.h"

namespace ibex {

void ExprNode2Dag::get_add_children(const ExprNode& nn, map<const ExprNode*,Interval,lt>& children, Interval sign){
	const ExprNode* n = &nn;
	while(dynamic_cast<const ExprMinus*>(n)){
		  sign*=-1;
		  n=&dynamic_cast<const ExprMinus*>(n)->expr;
	}

	if(lt::op(*n)==ADD){
	  get_add_children(*LEFT(n), children, sign);

	  get_add_children(*RIGHT(n), children, (CAST(ExprAdd,n))? sign:-sign);

	}else if(lt::op(*n)==MUL && lt::op(*LEFT(n))==CONSTANT)
		  add(children, sign* GET_VALUE(LEFT(n)), RIGHT(n));
	else if(lt::op(*n)==CONSTANT)
		  add(children, sign* GET_VALUE(n), 0);
	else
		  add(children, sign,  n);

}

void ExprNode2Dag::get_mul_children(const ExprBinaryOp& op2, map< const ExprNode*, Interval, lt> & children, Interval sig){
	for(int i=0; i<2; i++){
	  Interval sign(sig);
	  const ExprNode* n = (i==0)? &op2.left:&op2.right;
	  if(i==1) if(dynamic_cast<const ExprDiv*> (&op2)) sign*=-1;

	  if(lt::op(*n)==MUL)
		  get_mul_children(*CAST(ExprBinaryOp,n), children, sign);
	  else if(CAST(ExprPower,n))
		  add(children, sign * CAST(ExprPower, n)->expon, &CAST(ExprPower, n)->expr);
	  else if(CAST(ExprSqrt,n))
		  add(children, sign*0.5, &CAST(ExprSqrt, n)->expr);
	  else if(CAST(ExprSqr,n))
		  add(children, sign*2.0, &CAST(ExprSqr, n)->expr);
	  else if(lt::op(*n)==CONSTANT){
		  if(sign.lb()>0) mult(children, GET_VALUE(n), 0);
		  else mult(children, 1.0/GET_VALUE(n), 0);
	  }else
		  add(children, sign,  n);
	}
}

const ExprNode& ExprNode2Dag::generate_expression(const ExprNode& e){
	const ExprNode* node=NULL;
	if(lt::op(e)==ADD){
		map< const ExprNode*, Interval, lt>  children;
		get_add_children(e,children);
		for(map< const ExprNode*, Interval, lt>::reverse_iterator it=children.rbegin(); it!=children.rend(); it++){

			if(it->second == Interval(0.0)) continue;
			if(!it->first) node = add_term(node,&ExprConstant::new_scalar(it->second), true);
			else if(it->second == Interval(1.0)) node = add_term (node, it->first, true);
			else node = add_term (node, &(get_set_dagnode(it->second) * *it->first), true);

			if(node) node=&get_set_dagnode(*node);
		}
		if(!node) node = &ExprConstant::new_scalar(0.0);

	}else{
		map< const ExprNode*, Interval, lt>  children;
		get_mul_children(*CAST(ExprBinaryOp,&e),children);


		for(map< const ExprNode*, Interval, lt>::reverse_iterator it=children.rbegin(); it!=children.rend(); it++){
			if(it->second == Interval(0.0)) continue;
			if(!it->first && it->second == Interval(1.0)) continue;

			if(!it->first) { node = add_term (node, &ExprConstant::new_scalar(it->second), false); continue;}
			//cout << *it->first << endl;

	    	double intpart;
	    	if(it->second == Interval(0.0)) continue;
	    	else if(it->second == Interval(1.0)) node= add_term (node, it->first, false);
	    	else if(it->second == Interval(0.5)) node= add_term (node,  &(sqrt(*it->first)), false);
	    	else if(it->second == Interval(2.0)) node= add_term (node,  &(sqr(*it->first)), false);
	    	else if(it->second.is_degenerated() && modf(it->second.mid(), &intpart) == 0.0)
	    		node= add_term (node,  &pow((*it->first),(int)(it->second.mid())), false);
	    	else node= add_term (node,  &pow(*it->first,it->second.mid()), false);

	    	//cout << "in:" << *node << endl;
		    //if(node) node=&get_set_dagnode(*node);
		    //cout << "out:" << *node << endl;
		}
		if(!node) node = &ExprConstant::new_scalar(1.0);
	}

	return *node;
}

void ExprNode2Dag::visit(const ExprNode& e) {

	if (!clone.found(e)) {

		e.acceptVisitor(*this);
		//cout << "@in:" << e << " " << lt::op(e) << " " << e.fathers.size()  ; cout << endl;


		if((lt::op(e)==ADD && (e.fathers.size()==0 || lt::op(e.fathers[0])!=ADD)) ||
				(lt::op(e)==MUL && (e.fathers.size()==0 || lt::op(e.fathers[0])!=MUL))){
			// cout << "@in:" << e << " " << lt::op(e) << " " << lt::op(e.fathers[0])  ; cout << endl;

			const ExprNode* n=&generate_expression(*clone[e]);
			clone[e]= &get_set_dagnode(*n);

		//	cout << "@out:" << *clone[e]; cout  << endl;

		//si existe un nodo equivalente a n en el dag lo recupera, si no, lo agrega
		}else if(lt::op(e)!=ADD && lt::op(e)!=MUL){
			//cout << "@in:" << e   ; cout << endl;
			clone[e]= &get_set_dagnode(*clone[e]);
			//cout << "@out:" << *clone[e]; cout  << endl;
		}
	}

}

const ExprNode& ExprNode2Dag::generate(System& sys, Array<const ExprSymbol>& new_vars2){

	Array<const ExprSymbol> new_vars(sys.args.size());

	varcopy(sys.args,new_vars);
	const ExprNode& new_expr = ExprNode2Dag().transform(sys.args, new_vars, sys.f_ctrs.expr());
    //return new_expr;

	varcopy(new_vars,new_vars2);

	ExprCopy copy;
	const ExprNode& new_expr2 = copy.copy(new_vars, new_vars2, new_expr);

	return new_expr2;
}

} /* namespace ibex */
