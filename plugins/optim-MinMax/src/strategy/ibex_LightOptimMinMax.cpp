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
const double LightOptimMinMax::default_prec_y = 1.e-6;
const double LightOptimMinMax::default_ext_prob = 0;
const double LightOptimMinMax::default_list_elem_max = 1000;
const int LightOptimMinMax::default_nb_iter = 1000;

LightOptimMinMax::LightOptimMinMax(NormalizedSystem& y_sys, Ctc& ctc_xy):
    trace(false) , timeout(default_timeout),
        ctc_xy(ctc_xy),xy_sys(y_sys), bsc(new LargestFirst()), prec_y(default_prec_y), found_point(false), time(0),
        list_elem_max(default_list_elem_max),ext_crit_prob(default_ext_prob),nb_iter(default_nb_iter),monitor(false), csp_actif(false){


}

//LightOptimMinMax::LightOptimMinMax(NormalizedSystem& fa_y_sys):
//    trace(false) , timeout(default_timeout),
//        xy_sys(fa_y_sys), bsc(new LargestFirst()), prec_y(default_prec_y), found_point(false), time(0),
//        list_elem_max(default_list_elem_max),ext_crit_prob(default_ext_prob),nb_iter(default_nb_iter),monitor(false), csp_actif(true){


//}

LightOptimMinMax::~LightOptimMinMax() {
	delete bsc;
}

void LightOptimMinMax::add_backtrackable(Cell& root, const IntervalVector& y_init) {
//        if (csp_actif)
//            root.add<DataMinMaxCsp>();
//        else
//            root.add<DataMinMaxOpti>();
        root.add<DataMinMax>();
	Cell * y_cell = new Cell(y_init);
	y_cell->add<OptimData>();
	bsc->add_backtrackable(*y_cell);
        DataMinMax *data_x ;
//        DataMinMaxOpti *data_x ;

//        if (csp_actif)
//            data_x = &(root.get<DataMinMaxCsp>());
//        else
//            data_x = &(root.get<DataMinMaxOpti>());
        data_x = &(root.get<DataMinMax>());

	data_x->y_heap_costf1.add_backtrackable(*y_cell);
	data_x->y_heap_costf2.add_backtrackable(*y_cell);
	data_x->y_heap->push(y_cell);
}

bool LightOptimMinMax::optimize(Cell* x_cell) {
//    std::cout<<"run light optim"<<std::endl;

    std::cout<<std::endl<<std::endl<<"********************* Light optim res for box "<<x_cell->box<<" *************** "<<std::endl;
    std::cout<<"current optim parameters: "<<std::endl<<"list max elem: "<<list_elem_max<<std::endl<<"nb itex: "<<nb_iter<<std::endl;

        found_point  = false;
        DataMinMax *data_x;
//        DataMinMaxOpti *data_x;
//        if (csp_actif )
//            data_x = &(x_cell->get<DataMinMaxCsp>());
//        else
//            data_x = &(x_cell->get<DataMinMaxOpti>());
        data_x = &(x_cell->get<DataMinMax>());

	//std::cout <<"    DEB "<<data_x->fmax <<std::endl;

	DoubleHeap<Cell> *y_heap = data_x->y_heap; // current cell

	// /!\ Does creating a contractor here cause memory leak ? If so, it must be recreated in the minimax_solver algo only when best_max change to limit the memory leak, and pass it as an argument of this function
	//    CtcFwdBwd max_ctc((*objective_function),Interval(NEG_INFINITY,best_max)); // contract w.r.t "the objective function must be lower than the current best solution best_max"
	//    cout<<"lsolve: contractor ok"<<endl;

	// Define the TimeOut of to compute the bounds of x_cell
	time= Timer::get_time();

	// ********** contract x_box with ctc_xy***************
//        if(!csp_actif) // no constraint if dealing with fa cst, better not use the useless contractor
//        {
            IntervalVector xy_box = xy_box_hull(x_cell->box);
            ctc_xy.contract(xy_box);
            if(xy_box.is_empty()) {
                delete x_cell;
                return false;
            } else {
                // contract the result on x
                x_cell->box &= xy_box.subvector(0,x_cell->box.size()-1);
            }
//        }

        //monitoring variables, used to track upper bound, lower bound, number of elem in y_heap and heap_save at each iteration
        std::vector<double> ub,lb,nbel,nbel_save;


	// *********** loop ********************
	try {
            int current_iter(1);
                while(!stop_crit_reached(current_iter,y_heap->size())) {
			if (trace >= 3) std::cout<< *y_heap<<std::endl;

//                        if (csp_actif && (y_heap->top1()->get<OptimData>().pf.ub() < 0  )) {
//                                break;
//                        }
			Cell * y_cell = y_heap->pop1(); // we extract an element according to the first order
                        current_iter++;
//                        std::cout<<"current_iter: "<<current_iter<<std::endl;
			try {
                                std::pair<Cell*,Cell*> subcells_pair=bsc->bisect_cell(*y_cell);// bisect tmp_cell into 2 subcells
				delete y_cell;
//                                std::cout<<"handle first cell in light optim"<<std::endl;
                                bool res = handle_cell( x_cell, subcells_pair.first);
//                                std::cout<<"first cell handled"<<std::endl;
				if (!res) { // x_cell has been deleted
					delete subcells_pair.second;
					//std::cout <<"       OUT 1 "<<std::endl;
					return false;
				}
//                                std::cout<<"handle second cell in light optim"<<std::endl;
                                res = handle_cell( x_cell, subcells_pair.second);
//                                std::cout<<"second cell handled"<<std::endl;
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
                                bool res = handle_cell(x_cell,y_cell);

				if (res) heap_save.push_back(y_cell);
				else return false;

			}
			Timer::check(time+timeout);
                        if(monitor)
                        {
                            lb.push_back(data_x->fmax.lb());
                            ub.push_back(y_heap->top1()->get<OptimData>().pf.ub());
                            nbel.push_back(y_heap->size());
                            nbel_save.push_back(heap_save.size());
                        }
//                        std::cout<<"nb iter: "<<current_iter<<std::endl;

		}

	}
	catch (TimeOutException& ) { }
//        std::cout<<"light optim: out of loop"<<std::endl;
	/*
??    if(is_midp && !midp_hit) // midpoint x eval case: if no y found such as xy constraint respected, cannot keep the result
??        return Interval::EMPTY_SET;
	 */
	// insert the y_box with a too small diameter (those which were stored in heap_save)
//         std::cout<<"try to fill y_heap"<<std::endl;

        fill_y_heap(*y_heap);

        if (found_point) { // a feasible solution has been found
                y_heap->contract(-(data_x->fmax.lb())); // to check
        }
//        std::cout<<"y_heap filled"<<std::endl;

	// ** contract y_heap now

//        std::cout<<"found point pass"<<std::endl;
	if(y_heap->empty()){
//                std::cout <<"       OUT 3 "<<std::endl;
		delete x_cell;
		return false;
	}

	// Update the lower and upper bound of of "max f(x,y_heap)"
	double new_fmax_ub = y_heap->top1()->get<OptimData>().pf.ub(); // get the upper bound of max f(x,y_heap)
	double new_fmax_lb = y_heap->top2()->get<OptimData>().pf.lb(); // get the lower bound of max f(x,y_heap)

        std::cout<<"new_fmax_ub: "<<new_fmax_ub<<std::endl<<"new_fmax_lb: "<<new_fmax_lb<<std::endl<<"fmax_lb (from found point): "<<data_x->fmax.lb()<<std::endl;

	if (new_fmax_ub< new_fmax_lb) ibex_error("ibex_LightOptimMinMax: error, please report this bug.");

//        std::cout<<"fmax ini: "<<data_x->fmax<<std::endl;
	data_x->fmax &= Interval(new_fmax_lb, new_fmax_ub);
//        std::cout<<"new_fmax_lb: "<<new_fmax_lb<<" new_fmax_ub: "<<new_fmax_ub<<std::endl;
//        std::cout<<"fmax ini: "<<data_x->fmax<<std::endl;
//        if(data_x->fmax.lb()>100000){
//            std::cout<<"Issue: fmax_lb > 100,000"<<std::endl;

//        }

        if(monitor)
        {
            lb.push_back(data_x->fmax.lb());
            ub.push_back(data_x->fmax.ub());
            nbel.push_back(y_heap->size());
            nbel_save.push_back(heap_save.size());
            export_monitor(&ub,&lb,&nbel,&nbel_save,x_cell->box);
        }

	if(  data_x->fmax.is_empty()) {
//                std::cout <<"       OUT 4 "<<std::endl;
		delete x_cell;
		return false;
	}

//        std::cout <<"    FIN "<<data_x->fmax <<std::endl;
	return true;
}

bool LightOptimMinMax::stop_crit_reached(int current_iter,int heap_size) {
    if(nb_iter != 0 && current_iter>=nb_iter) // nb_iter ==  0 implies minimum precision required (may be mid point x case)
        return true;
    if(list_elem_max != 0 && heap_size>list_elem_max)
        return true;
    if(heap_size == 0)
        return true;
    return false;
}

bool LightOptimMinMax::handle_cell( Cell* x_cell, Cell*  y_cell) {

	IntervalVector xy_box =init_xy_box(x_cell->box,y_cell->box);
	// recuperer les data
        DataMinMax *data_x = &(x_cell->get<DataMinMax>());
//        DataMinMaxOpti *data_x = &(x_cell->get<DataMinMaxOpti>());
	OptimData  *data_y = &(y_cell->get<OptimData>());

        if(data_y->pu!=1) { // Check constraints
            if(handle_constraint(data_y,&xy_box,&(y_cell->box))) {
                delete y_cell;
                return true;
            }
        }
        else {
            handle_cstfree(&xy_box,y_cell);
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
                // TODO to check normalement on peut propager la contraction sur le y et sur le x
		for (int k=0; k<y_cell->box.size(); k++)
                        y_cell->box[k] &= xy_box[x_cell->box.size()+k];
//                if(data_y->pu==1) // constraint on xy is respected, contraction of x_box is possible
//                    x_cell->box &= xy_box.subvector(0,x_cell->box.size()-1);
	}
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
//            std::cout<<"y_cell pushed in heap_save, box: "<<y_cell->box<<" pf: "<<data_y->pf<<" pu: "<<data_y->pu<<std::endl;
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

bool LightOptimMinMax::handle_cstfree(IntervalVector * xy_box,Cell * y_cell) {
//    std::cout<<"init box: "<<*xy_box<<std::endl;
//    IntervalVector grad(xy_box->size());
//    xy_sys.goal->gradient(*xy_box,grad);
//    for(int i=xy_box->size()-y_cell->box.size();i<xy_box->size();i++) {
////        std::cout<<"i = "<<i<<std::endl;
//        if(grad[i].lb()>0) {
//            (*xy_box)[i] = Interval((*xy_box)[i].ub() -1.e-15,(*xy_box)[i].ub());
//        }
//        if(grad[i].ub()<0) {
//            (*xy_box)[i] = Interval((*xy_box)[i].lb(),(*xy_box)[i].lb()+1.e-15);
//        }
//    }
//    std::cout<<"final box: "<<*xy_box<<std::endl;
//    std::cout<<"free cst contraction done, contracted box: "<<*xy_box<<std::endl;
    return true;
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
        int res =2;

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
//        int nb_pt(0);
	while(!heap_save.empty()) { // push all boxes of heap_save in y_heap

//            std::cout<<"try to get last elem of heap_save"<<std::endl;
		tmp_cell = heap_save.back();
//                if(tmp_cell->box.max_diam()<1.e-14)
//                    nb_pt++;
//                OptimData  *data_y = &(tmp_cell->get<OptimData>());
//                std::cout<<""
//                std::cout<<"try to push last elem of heap_save,box: "<<tmp_cell->box<<std::endl;
                y_heap.push(tmp_cell);
//                std::cout<<"try to pop last elem of heap_save"<<std::endl;
		heap_save.pop_back();
	}
//        if(nb_pt>1) {
//            std::cout<<"CRITICAL ISSUE: more than 1 point in heap"<<std::endl;
//        }
	heap_save.clear();
}

void export_monitor(std::vector<double> * ub,std::vector<double> * lb,std::vector<double> * nbel,std::vector<double> * nbel_save,const IntervalVector& box) {
//    std::ofstream out;
    std::ofstream out("log.txt",std::ios::app);
//    std::string output_name = "log.txt";
//    out.open("log.txt");
//    out<<"x box: "<<box<<std::endl;
    for(int i = 0;i<ub->size();i++) {
        out<<ub->at(i)<<" "<<lb->at(i)<<" "<<nbel->at(i)<<" "<<nbel_save->at(i)<<std::endl;
    }
    out.close();

}
}
