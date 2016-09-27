#include "ibex_LightOptimMinMax.h"
#include "ibex_OptimData.h"
#include "ibex_DataMinMax.h"
//#include <stdio.h>
#include "ibex_LargestFirst.h"


namespace ibex{

LightOptimMinMax::LightOptimMinMax(NormalizedSystem& y_sys, Ctc& ctc_xy):
	ctc_xy(ctc_xy),xy_sys(y_sys), bsc(new LargestFirst()) {

}

void LightOptimMinMax::add_backtrackable(Cell& root, const IntervalVector& y_init) {
	root.add<DataMinMax>();
	Cell * y_cell = new Cell(y_init);
	y_cell->add<OptimData>();
	bsc->add_backtrackable(*y_cell);
	root.get<DataMinMax>().y_heap.push(y_cell);
}

void LightOptimMinMax::optimize(Cell* x_cell, int nb_iter, double min_prec) {

	DataMinMax *data_x = &(x_cell->get<DataMinMax>());

	DoubleHeap<Cell> *y_heap = &(data_x->y_heap); // current cell

	// /!\ Does creating a contractor here cause memory leak ? If so, it must be recreated in the minimax_solver algo only when best_max change to limit the memory leak, and pass it as an argument of this function
	//    CtcFwdBwd max_ctc((*objective_function),Interval(NEG_INFINITY,best_max)); // contract w.r.t "the objective function must be lower than the current best solution best_max"
	//    cout<<"lsolve: contractor ok"<<endl;

	for(int i = 0; (!y_heap->empty()) && (i<nb_iter) ;i++) {

		Cell * tmp_cell = y_heap->pop1(); // we extract an element according to the first order
		std::pair<Cell*,Cell*> subcells_pair=bsc->bisect(*tmp_cell);// bisect tmp_cell into 2 subcells
		delete tmp_cell;

		handle_cell( x_cell, subcells_pair.first, min_prec);
		if (x_cell==NULL) return;

		handle_cell( x_cell, subcells_pair.second, min_prec);
		if (x_cell==NULL) return;

	}

	/*
??    if(is_midp && !midp_hit) // midpoint x eval case: if no y found such as xy constraint respected, cannot keep the result
??        return Interval::EMPTY_SET;
	 */
	// insert the y_box with a too small diameter (those which were stored in heap_save)
	fill_y_heap(*y_heap);

	if(y_heap->empty()){
		delete x_cell;
		return ;
	}

	// Update the lower and upper bound of of "max f(x,y_heap)"
	double new_fmax_ub = y_heap->top1()->get<DataMinMax>().fmax.ub(); // get the upper bound of max f(x,y_heap)
	double new_fmax_lb = y_heap->top2()->get<DataMinMax>().fmax.lb(); // get the lower bound of max f(x,y_heap)

	if( new_fmax_ub < data_x->fmax.lb()){
		delete x_cell;
		x_cell =NULL;
		return ;
	}

	if(data_x->fmax.lb() < new_fmax_lb ) // no midpoint verified the constraints, get a inferior bound from the second heap
		data_x->fmax = Interval(new_fmax_lb,new_fmax_ub);
	else
		data_x->fmax = Interval(data_x->fmax.lb(),new_fmax_ub);

	return;
}



void LightOptimMinMax::handle_cell( Cell* x_cell, Cell*  y_cell, double min_prec) {

	IntervalVector xy_box =init_xy_box(x_cell->box,y_cell->box);
	// recuperer les data
	DataMinMax * data_x= &(x_cell->get<DataMinMax>());
	double best_max =data_x->fmax.ub();
	double uplo = data_x->fmax.lb();

	OptimData *data_y = &(y_cell->get<OptimData>());

	// Check the constraints
	switch(check_constraints(xy_box)){
	case 2: { // all the constraints are satisfied
		data_y->pu=1;
		break;
	}
	case 0: { // One constraint is false
		// constraint on x and y not respected, move on.
		delete y_cell;
		y_cell =NULL;
		return ;
	}
	default: // nothing to do
		break;
	}

	//
	if(data_y->pu != 1)  {// there is a constraint on x and y
		ctc_xy.contract(xy_box);
		if (xy_box.is_empty()) { // constraint on x and y not respected, move on.
			delete y_cell;
			y_cell=NULL;
			return ;
		} else {
			// TODO to check normalement on peut propager la contraction sur le y
			for (int k=0; k<y_cell->box.size(); k++)
				y_cell->box[k] = xy_box[x_cell->box.size()+k];
		}
	}
	/********************************************************************************/
	//mid point test (TO DO: replace with local optim to find a better point than midpoint)
	IntervalVector mid_y_box = get_feasible_point(x_cell,y_cell);

	if( !mid_y_box.is_empty() || (data_y->pu == 1)) {
		// x y constraint respected for all x and mid(y), mid_y_box is a candidate for evaluation
		//midp_hit = true; <- JN:what is the variable?
		Interval midres = xy_sys.goal->eval(mid_y_box);
		if (midres.lb()> data_x->fmax.ub() ) {  // midres.lb()> best_max
			// there exists y such as constraint is respected and f(x,y)>best max, the max on x will be worst than the best known solution
			delete y_cell;
			y_cell = NULL;
			delete x_cell;
			x_cell =NULL;
			return ; // no need to go further, x_box does not contains the solution
		}
		else if(midres.lb()>data_y->pf.lb()) { // found y such as xy constraint is respected
			// TODO	 to check		// il faut faire un contract de y_heap
			data_x->y_heap.contract(midres.lb()); // to check
			data_y->pf = Interval(midres.lb(),data_y->pf.ub());
			data_x->fmax = Interval(midres.lb(),data_x->fmax.ub());; // yes we found a feasible solution for all x
		}
	}

	//************ part below add a contraction w.r.t f(x,y)<best_max, this part may not be efficient on every problem ******************************
	// TODO je n'ai pas regarde ceci, mais il me semble que ce n'est plus necessaire
	//            contract_best_max_cst(&max_ctc,&xy_box,&xy_box_ctc,subcells[j]);
	//            if(xy_box.is_empty()||xy_box_ctc.is_empty()) { // there are no x in x_box such as exists y in y_box, f(x,y)<best_max || no point (x,y) in xy_box such as both constraints are respected
	//                delete subcells[j];
	//                continue;
	//            }
	//********************************************

	// Update the lower and upper bound on y
	Interval res = xy_sys.goal->eval(xy_box); // objective function evaluation

	if(data_x->fmax.lb() < res.ub()) {  // y_box cannot contains max f(x,y)
		delete y_cell;
		y_cell =NULL;
		return ;
	}

	if (data_y->pf.lb() < res.lb()) { // because data->pf.lb() can be updates by an evaluation at the midpoint
		data_y->pf = res;
	}  else {
		data_y->pf = Interval(data_y->pf.lb(),res.ub()); // we keep the previous lower bound (perhaps found with a feasible solution)
	}

	// check if it is possible to find a better solution than those already found on x
	if((data_y->pf.lb()>best_max) && (data_y->pu == 1)){
		// box verified condition and eval is above best max, x box does not contains the solution
		// I think this case was already check with the mid-point.
		delete y_cell;
		y_cell = NULL;
		delete x_cell;
		x_cell =NULL;
		return ; // no need to go further, x_box does not contains the solution
	}


	if(y_cell->box.max_diam()<min_prec) { // if min prec reached, keep box in Heap_save
		heap_save.push_back(y_cell);
	} else {
		data_x->y_heap.push(y_cell); // add the box  in y_heap
	}
}




/*
void LightOptimMinMax::contract_best_max_cst(Ctc& max_ctc,IntervalVector& xy_box,IntervalVector& xy_box_ctc,y_heap_elem& elem) {
    if(ctc_xy == NULL || elem.pu == 1) { // no constraint or xy constraint respected for all xy
        max_ctc.contract(xy_box); // contract xy_box w.r.t lower than best max constraint, /!\ xy_box contracted! => x box of xy_box may have be contracted, must keep this result!
        if(xy_box.is_empty()) { // there are no x in x_box such as exists y in y_box, f(x,y)<best_max
            return;
        }
        else
            xy_box_ctc &= (xy_box); // result of contraction w.r.t the two constraints: best max and xy constraints
    }
    else
        max_ctc.contract(xy_box_ctc); // cannot keep x contraction as it is not certified that xy constraint is respected forall x and y in xy_box, xy_box_ctc contracted instead
    return;
}

 */

IntervalVector LightOptimMinMax::get_feasible_point(Cell * x_cell,Cell * y_cell) {
	IntervalVector mid_y_box = get_mid_y(y_cell->box,x_cell->box); // get the box (x,mid(y))
	if((y_cell->get<OptimData>().pu != 1)) { // constraint on xy exist and is not proved to be satisfied
		int res = check_constraints(mid_y_box);
		if(res == 0 ||res == 1)
			return IntervalVector(1,Interval::EMPTY_SET);
	}
	return mid_y_box;
}

int LightOptimMinMax::check_constraints(const IntervalVector& xy_box) {
	int res(2);

	for(int i=0;i<xy_sys.nb_ctr;i++) {
		Interval int_res =  xy_sys.ctrs[i].f.eval(xy_box);
		if(int_res.lb()>0)
			return 0;
		else if(int_res.ub()>=0)
			res = 1;
	}
	return res;

}



IntervalVector LightOptimMinMax::get_mid_y(const IntervalVector& x_box,const IntervalVector& y_box) { // returns the cast of box x and mid of box y

	IntervalVector res(y_box.size()+x_box.size());
	for(int i=0;i<x_box.size();i++) {
		res[i] = x_box[i];
	}
	for(int i=0;i<y_box.size();i++) {
		res[x_box.size()+i] = y_box[i].mid();
	}
	return res;
}

IntervalVector LightOptimMinMax::init_xy_box(const IntervalVector& x_box,const IntervalVector & y_box) {
	IntervalVector res(x_box.size()+y_box.size());
	for( int k=0 ; k<x_box.size();k++)
		res[k] = x_box[k];
	for(int k = 0;k<y_box.size();k++) // update current y box in the xy_box
		res[k+x_box.size()] = y_box[k];
	return res;
}

void LightOptimMinMax::fill_y_heap(DoubleHeap<Cell>& y_heap) {
	Cell* tmp_cell;
	while(!heap_save.empty()) { // push all boxes of heap_save in y_heap
		tmp_cell = heap_save.back();
		y_heap.push(tmp_cell);
		heap_save.pop_back();
	}
	heap_save.clear();
}

}
