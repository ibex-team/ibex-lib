//============================================================================
//                                  I B E X
// File        : ibex_LightOptimMinMax.cpp
// Author      : Dominique Monnet, Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 1, 2016
//============================================================================



#include "ibex_LightOptimMinMax.h"
#include "ibex_DataMinMax.h"
//#include <stdio.h>
#include "ibex_LargestFirst.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_Timer.h"

namespace ibex{

const double LightOptimMinMax::default_timeout = 60;

LightOptimMinMax::LightOptimMinMax(NormalizedSystem& y_sys, Ctc& ctc_xy):  trace(false) , timeout(default_timeout),
	ctc_xy(ctc_xy),xy_sys(y_sys), bsc(new LargestFirst()), prec_y(1.e-6), found_point(false), time(0)  {

}

LightOptimMinMax::~LightOptimMinMax() {
	delete bsc;
}

void LightOptimMinMax::add_backtrackable(Cell& root, const IntervalVector& y_init) {
	root.add<DataMinMax>();
	Cell * y_cell = new Cell(y_init);
	y_cell->add<OptimData>();
	bsc->add_backtrackable(*y_cell);
	DataMinMax *data_x = &(root.get<DataMinMax>());
	data_x->y_heap_costf1.add_backtrackable(*y_cell);
	data_x->y_heap_costf2.add_backtrackable(*y_cell);
	data_x->y_heap->push(y_cell);
}

bool LightOptimMinMax::optimize(Cell* x_cell, int nb_iter, double prec_y1) {

        bool cst = xy_sys.nb_ctr>0;
	found_point  = false;
	prec_y = prec_y1;
	DataMinMax *data_x = &(x_cell->get<DataMinMax>());

	//std::cout <<"    DEB "<<data_x->fmax <<std::endl;

	DoubleHeap<Cell> *y_heap = data_x->y_heap; // current cell

	// /!\ Does creating a contractor here cause memory leak ? If so, it must be recreated in the minimax_solver algo only when best_max change to limit the memory leak, and pass it as an argument of this function
	//    CtcFwdBwd max_ctc((*objective_function),Interval(NEG_INFINITY,best_max)); // contract w.r.t "the objective function must be lower than the current best solution best_max"
	//    cout<<"lsolve: contractor ok"<<endl;

	// Define the TimeOut of to compute the bounds of x_cell
	time= Timer::get_time();

	// ********** contract x_box with ctc_xy***************
        if(cst) // contraction needed only if constraints on xy
        {
            IntervalVector xy_box = xy_box_hull(x_cell->box);
            ctc_xy.contract(xy_box);
            if(xy_box.is_empty()) {
                delete x_cell;
                return false;
            } else {
                // contract the result on x
                x_cell->box &= xy_box.subvector(0,x_cell->box.size()-1);
            }
        }

	// *********** loop ********************
	try {
		for(int i = 0; (!y_heap->empty()) && (i<nb_iter) ;i++) {

			if (trace >= 3) std::cout<< *y_heap<<std::endl;

			Cell * y_cell = y_heap->pop1(); // we extract an element according to the first order
			try {
				std::pair<Cell*,Cell*> subcells_pair=bsc->bisect_cell(*y_cell);// bisect tmp_cell into 2 subcells
				delete y_cell;

                                bool res = handle_cell( x_cell, subcells_pair.first,cst);
				if (!res) { // x_cell has been deleted
					delete subcells_pair.second;
					//std::cout <<"       OUT 1 "<<std::endl;
					return false;
				}

                                res = handle_cell( x_cell, subcells_pair.second,cst);
				if (!res) { // x_cell has been deleted
					//std::cout <<"       OUT 2 "<<std::endl;
					return false;
				}

				// ** contract not yet
				//if (found_point) { // a feasible solution has been found
				//	y_heap->contract(-(data_x->fmax.lb())); // to check
				//}
			}
			catch (NoBisectableVariableException& ) {
                                bool res = handle_cell(x_cell,y_cell,cst);

				if (res) heap_save.push_back(y_cell);
				else return false;

			}
			Timer::check(time+timeout);

		}

	}
	catch (TimeOutException& ) { }
	/*
??    if(is_midp && !midp_hit) // midpoint x eval case: if no y found such as xy constraint respected, cannot keep the result
??        return Interval::EMPTY_SET;
	 */
	// insert the y_box with a too small diameter (those which were stored in heap_save)
	fill_y_heap(*y_heap);

	// ** contract y_heap now
	if (found_point) { // a feasible solution has been found
		y_heap->contract(-(data_x->fmax.lb())); // to check
	}

	if(y_heap->empty()){
		//std::cout <<"       OUT 3 "<<std::endl;
		delete x_cell;
		return false;
	}

	// Update the lower and upper bound of of "max f(x,y_heap)"
	double new_fmax_ub = y_heap->top1()->get<OptimData>().pf.ub(); // get the upper bound of max f(x,y_heap)
	double new_fmax_lb = y_heap->top2()->get<OptimData>().pf.lb(); // get the lower bound of max f(x,y_heap)

	if (new_fmax_ub< new_fmax_lb) ibex_error("ibex_LightOptimMinMax: error, please report this bug.");

	data_x->fmax &= Interval(new_fmax_lb, new_fmax_ub);

	if(  data_x->fmax.is_empty()) {
		//std::cout <<"       OUT 4 "<<std::endl;
		delete x_cell;
		return false;
	}


	//std::cout <<"    FIN "<<data_x->fmax <<std::endl;
	return true;
}



bool LightOptimMinMax::handle_cell( Cell* x_cell, Cell*  y_cell,bool cst) {

	IntervalVector xy_box =init_xy_box(x_cell->box,y_cell->box);
	// recuperer les data
	DataMinMax *data_x = &(x_cell->get<DataMinMax>());
	OptimData  *data_y = &(y_cell->get<OptimData>());

        if(cst) { // Check constraints
            if(handle_constraint(data_y,&xy_box,&(y_cell->box))) {
                delete y_cell;
                return true;
            }
        }
        else {
            if(handle_cstfree(&xy_box)) {
                delete y_cell;
                return true;
            }
        }
	/********************************************************************************/
	//mid point test (TO DO: replace with local optim to find a better point than midpoint)
	IntervalVector mid_y_box = get_feasible_point(x_cell,y_cell);

	if (!(mid_y_box.is_empty())) {
		// x y constraint respected for all x and mid(y), mid_y_box is a candidate for evaluation
		//midp_hit = true; <- JN:what is the variable?
		Interval midres = xy_sys.goal->eval(mid_y_box);
		if ( data_x->fmax.ub() < midres.lb() ) {  // midres.lb()> best_max
			// there exists y such as constraint is respected and f(x,y)>best max, the max on x will be worst than the best known solution
			delete y_cell;
			delete x_cell;
			//std::cout <<"           OUT 6 mid="<<y_cell->box<<std::endl;
			return false; // no need to go further, x_box does not contains the solution
		}
		else if(midres.lb()>data_y->pf.lb()) { // found y such as xy constraint is respected
			// TODO	 to check		// il faut faire un contract de y_heap
			data_y->pf   &= Interval(midres.lb(),POS_INFINITY);
			if (data_x->fmax.lb() < midres.lb() ) {
				found_point = true;
				data_x->fmax &= Interval(midres.lb(),POS_INFINITY);; // yes we found a feasible solution for all x
			}
		}
	}

	//************ part below add a contraction w.r.t f(x,y)<best_max, this part may not be efficient on every problem ******************************
	xy_sys.goal->backward(data_x->fmax,xy_box);
	if (xy_box.is_empty()) { // constraint on x and y not respected, move on.
		delete y_cell;
		return true;
	} else {
		// TODO to check normalement on peut propager la contraction sur le y
		for (int k=0; k<y_cell->box.size(); k++)
			y_cell->box[k] = xy_box[x_cell->box.size()+k];
	}
	// TODO je n'ai pas regarde ceci, mais il me semble que ce n'est plus necessaire
	//            contract_best_max_cst(&max_ctc,&xy_box,&xy_box_ctc,subcells[j]);
	//            if(xy_box.is_empty()||xy_box_ctc.is_empty()) { // there are no x in x_box such as exists y in y_box, f(x,y)<best_max || no point (x,y) in xy_box such as both constraints are respected
	//                delete subcells[j];
	//                continue;
	//            }
	//********************************************

	// Update the lower and upper bound on y
	data_y->pf &= xy_sys.goal->eval(xy_box); // objective function evaluation
	if( data_y->pf.is_empty() || data_x->fmax.lb() > data_y->pf.ub()) {  // y_box cannot contains max f(x,y)
		delete y_cell;
		return true;
	}

	// check if it is possible to find a better solution than those already found on x
	if((data_y->pf.lb() > data_x->fmax.ub()) && (data_y->pu == 1)){
		// box verified condition and eval is above best max, x box does not contains the solution
		// I think this case was already check with the mid-point.
		delete y_cell;
		delete x_cell;
		//std::cout <<"           OUT 7 "<<std::endl;
		return false; // no need to go further, x_box does not contains the solution
	}

	//*************************************************
	// store y_cell
	if (y_cell->box.max_diam()<prec_y) {
		heap_save.push_back(y_cell);
	} else {
		data_x->y_heap->push(y_cell);
	}

	return true;
}


bool LightOptimMinMax::handle_constraint(OptimData  *data_y, IntervalVector * xy_box,IntervalVector * y_box) {
    switch(check_constraints(*xy_box)){
    case 2: { // all the constraints are satisfied
            data_y->pu=1;
            break;
    }
    case 0: { // One constraint is false
            // constraint on x and y not respected, move on.
            return true;
    }
    default: // nothing to do
            break;
    }

    //
    if(data_y->pu != 1)  {// there is a constraint on x and y
            ctc_xy.contract(*xy_box);
            if (xy_box->is_empty()) { // constraint on x and y not respected, move on.
                    return true;
            } else {
                    // TODO to check normalement on peut propager la contraction sur le y
                    for (int k=0; k<y_box->size(); k++)
                            (*y_box)[k] = (*xy_box)[xy_box->size()-y_box->size()-1+k];
            }
    }
    return false;
}

bool LightOptimMinMax::handle_cstfree(IntervalVector * xy_box) {
    //TO DO
    return false;
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
	IntervalVector mid_y_box = get_mid_y(x_cell->box,y_cell->box); // get the box (x,mid(y))
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

IntervalVector LightOptimMinMax::xy_box_hull(const IntervalVector& x_box) {
	IntervalVector res(xy_sys.nb_var);
	for( int k=0 ; k<x_box.size();k++)
		res[k] = x_box[k];
	for(int k = x_box.size();k<xy_sys.nb_var;k++) // update current y box in the xy_box
		res[k] = xy_sys.box[k];
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
