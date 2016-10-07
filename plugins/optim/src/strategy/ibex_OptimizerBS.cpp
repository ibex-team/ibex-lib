//                                  I B E X                                   
// File        : ibex_OptimizerBS.cpp
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : September 3, 2015
//============================================================================

#include "ibex_OptimizerBS.h"


#include "ibex_Timer.h"

#include "ibex_NoBisectableVariableException.h"

#include <float.h>
#include <stdlib.h>


using namespace std;

namespace ibex {


OptimizerBS::~OptimizerBS() {
 
}


  double OptimizerBS::getminimum() const
  {return buffer.top()->box[ext_sys.goal_var()].lb();}


  OptimizerBS::OptimizerBS(System& user_sys, Ctc& ctc, Bsc& bsc, CellBuffer& buffer, double prec,
			   double goal_rel_prec, double goal_abs_prec, int sample_size, double equ_eps,
			 bool rigor, int selnode, double N,  double prob_bs, bool initset) : 
    Optimizer(user_sys, ctc, bsc, buffer, prec,goal_rel_prec, goal_abs_prec, sample_size, equ_eps,
	      rigor), N(N), selnode(selnode), prob_bs(prob_bs), initset(initset), factor_adaptive(1.0)
				 {
}




  void OptimizerBS::update_cell_data (Cell& c){

  c.get<CellBS>().lb=c.box[ext_sys.goal_var()].lb();
  c.get<CellBS>().ub=c.box[ext_sys.goal_var()].ub();
  c.get<CellBS>().id=nb_cells;

}

void OptimizerBS::handle_node(Cell* c, CellBuffer& L, const IntervalVector& init_box ){
	if (c->box[c->box.size()-1].lb() > compute_ymax(1.0)) {delete c; return;};  
	try{
		pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);
		pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);			
	    delete c;
		
		if(handle_cell_nopush(*new_cells.second,init_box))
			L.push(new_cells.second);
		if(handle_cell_nopush(*new_cells.first,init_box))
			L.push(new_cells.first);

	

	}catch (NoBisectableVariableException& ) {
		update_uplo_of_epsboxes ((c->box)[ext_sys.goal_var()].lb());
		delete c;
	}

	time_limit_check();

}

void move_nodes(CellBuffer& LL, CellBuffer& L, int M=-1){

	if(M==-1) M=LL.size();
	for(int i=0; !LL.empty() && i<M; i++)
		L.push(LL.pop());
}


//~ void OptimizerBS::diving(CellBuffer& L, const IntervalVector& init_box){
	//~ CellSet<maxID> LL;
	//~ LL.push(L.pop());
	//~ int fails=0;
	//~ while(!LL.empty()){
		//~ Cell* c2 = LL.pop();
		//~ int size = LL.size();
		//~ handle_node(c2, LL, init_box );
		//~ 
		//~ if (uplo_of_epsboxes == NEG_INFINITY) {
			//~ cout << " possible infinite minimum " << endl;
			//~ return;
		//~ }
//~ 
		//~ if (LL.size()==size) fails++;
		//~ if(fails >= M)
			//~ move_nodes(LL, L);
	//~ }
//~ }

//~ void OptimizerBS::diving_iter(CellBuffer& L, const IntervalVector& init_box){
	//~ CellSet<maxID> LL;
	//~ LL.push(L.pop());
	//~ int iters=0;
	//~ while(!LL.empty()){
		//~ Cell* c2 = LL.pop();
		//~ int size = LL.size();
		//~ handle_node(c2, LL, init_box );
		//~ 
		//~ if (uplo_of_epsboxes == NEG_INFINITY) {
			//~ cout << " possible infinite minimum " << endl;
			//~ return;
		//~ }
		//~ 
		//~ if(iters >= M)
			//~ move_nodes(LL, L);
		//~ iters++;
	//~ }
//~ }

//R. Bixby and E. Rothberg, Progress in computational mixed integer programming. A look back from the other side of the tipping point, Annals of Operations Research, 149 (2007), pp. 3741.
//~ void OptimizerBS::probed_diving(CellBuffer& L, const IntervalVector& init_box){
	//~ CellSet<maxID> LL;
	//~ LL.push(L.pop());
	//~ int fails=0;
	//~ while(!LL.empty()){
		//~ Cell* c = LL.pop();
		//~ int size = LL.size();
		//~ handle_node(c, LL, init_box );
		//~ 
		//~ if (uplo_of_epsboxes == NEG_INFINITY) {
			//~ cout << " possible infinite minimum " << endl;
			//~ return;
		//~ }
		//~ 
		//~ if (LL.size()==size) fails++;
		//~ if (LL.size()==size+2){//sort the last two leaves
			//~ Cell* c1=LL.pop();
			//~ Cell* c2=LL.pop();
			//~ if( minLB()(c2, c1) ){
				//~ c2->get<CellBS>().id++;
				//~ c1->get<CellBS>().id--;
			//~ }
			//~ LL.push(c1); LL.push(c2);
		//~ }
		//~ if(fails >= M)
			//~ move_nodes(LL, L);
	//~ }
//~ }

// with N=1 Beamsearch becomes FeasibleDiving, decribed in JOGO15 submitted paper              BN


void OptimizerBS::beam_search(CellBuffer& L, const IntervalVector& init_box){
	CellSet<minLB> LL,L_s;
	if(initset){
	  for(int i=0;i<N && !L.empty() ; i++) 	LL.push(L.pop());
	}else LL.push(L.pop());
	while(!LL.empty()){
		while(!LL.empty()){
			Cell* c2 = LL.pop();
			handle_node(c2, L_s, init_box );
				
			if (uplo_of_epsboxes == NEG_INFINITY) {
				cout << " possible infinite minimum " << endl;
				return;
			}

		}
		move_nodes(L_s, LL, N);
		move_nodes(L_s, L);
		
	}
}

void OptimizerBS::ibest_first(CellBuffer& L, const IntervalVector& init_box, int selnode){
	CellBuffer* LL, *Laux;
	if(selnode==IBEST_FIRST_MINLB){
		LL=new CellSet<minLB4ibf>;
		Laux=new CellSet<minLB4ibf>;
	}else if(selnode==IBEST_FIRST_MAXDEPTH){
		LL=new CellSet<maxDepth>;
		Laux=new CellSet<maxDepth>;	
	}else if(selnode==IBEST_FIRST_DEEPFIRST){
		LL=new CellSet<deepFirst>;
		Laux=new CellSet<deepFirst>;			
	}
	
	if(initset){
	  for(int i=0;i<N && !L.empty() ; i++) LL->push(L.pop());
    }else LL->push(L.pop());

	while(!LL->empty()){
		Cell* c2 = LL->pop();
		handle_node(c2, *LL, init_box );
			
		if (uplo_of_epsboxes == NEG_INFINITY) {
			cout << " possible infinite minimum " << endl;
			delete LL; delete Laux;
			return;
		}
		
			
		if(LL->size()>N){
		  move_nodes(*LL, *Laux, N);
		  move_nodes(*LL, L); //put size-N boxes from LL into L
		  move_nodes(*Laux, *LL);
	   }
	}
	delete LL; delete Laux;
}


Optimizer::Status OptimizerBS::optimize(const IntervalVector& init_box, double obj_init_bound) {

	Cell* root=new Cell(IntervalVector(n+1));
	init_search(*root,init_box,obj_init_bound);
	
	//data for the OptimizerBS
	root->add<CellBS>();

	double old_loup=loup;

        handle_cell(*root, init_box);
	
	double ymax= POS_INFINITY;
	
	try {	
		while (!buffer.empty()) {

		  //~ update_uplo();
  
			loup_changed=false;
			
	
			//Cell* c = buffer.pop();
			
			
			
			switch(selnode){
				case BEAM_SEARCH: 
					if((double) rand()/ (double) RAND_MAX < prob_bs)  
						beam_search( buffer, init_box);
					else 
						handle_node(buffer.pop(), buffer, init_box);
				break;
				case IBEST_FIRST_MINLB: 
				case IBEST_FIRST_MAXDEPTH: 
				case IBEST_FIRST_DEEPFIRST:
					if((double) rand()/ (double) RAND_MAX < prob_bs)  
						ibest_first( buffer, init_box, selnode);
					else 
						handle_node(buffer.pop(), buffer, init_box);
				break;

				case NORMAL: handle_node(buffer.pop(), buffer, init_box); break;
				//~ case DIVING: diving(buffer, init_box ); break;
				//~ case PROBED_DIVING: probed_diving(buffer, init_box ); break;
				//~ case DIVING_ITER: diving_iter(buffer, init_box);
			}	
			if(factor_adaptive==0.0) selnode=NORMAL;
			
			
			if (uplo_of_epsboxes == NEG_INFINITY) {
				cout << " possible infinite minimum " << endl;
				//	goto end;
				break;
			}
		
			if (loup<old_loup) {

			  // In case of a new upper bound (loup_changed == true), all the boxes
			  // with a lower bound greater than (loup - goal_prec) are removed and deleted.
			  // Note: if contraction was before bisection, we could have the problem
			  // that the current cell is removed by contract_heap. See comments in
			  // older version of the code (before revision 284).
			  ymax= compute_ymax(1.0);
					
			  if(factor_adaptive>1.0) N=1.0;
                   
			  buffer.contract(ymax,ext_sys.goal_var(), true);

                    
			  if (ymax <=NEG_INFINITY) {
			    cout << " infinite value for the minimum " << endl;
			    //			    goto end;
			    break;
			  }
			  if (trace) cout << setprecision(12) << "ymax=" << ymax << " uplo= " <<  uplo<< endl;
			  
			  old_loup=loup; 
			}	else
			  if(factor_adaptive>1.0) N*=factor_adaptive;
			
		
			if (uplo_of_epsboxes == NEG_INFINITY || ymax <=NEG_INFINITY) {cout << "uplo = neginfinity"<< endl; break;}
			update_uplo();

		}
	}
	catch (TimeOutException& ) {
	  return TIME_OUT;
	}
	
	//	end:

        return end_search();
}
       

} // end namespace ibex

