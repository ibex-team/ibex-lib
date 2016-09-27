#include "ibex_OptimMinMax.h"
#include "ibex_LargestFirst.h"
#include "ibex_Timer.h"
#include <stdio.h>

namespace ibex {

OptimMiniMax::OptimMiniMax(NormalizedSystem& x_sys,NormalizedSystem& xy_sys, Ctc& x_ctc,Ctc& xy_ctc):
	loup(POS_INFINITY), uplo(NEG_INFINITY), minprec_uplo(POS_INFINITY), best_sol(x_sys.nb_var), x_heap(x_heap_costfunc),
	x_ctc(x_ctc),x_sys(x_sys),lsolve(xy_sys,xy_ctc), bsc(new LargestFirst()),
	x_box_init(x_sys.nb_var), y_box_init(xy_sys.nb_var-x_sys.nb_var) {
};

OptimMiniMax::~OptimMiniMax() {
	delete bsc;
}

void OptimMiniMax::solve(const IntervalVector& x_box_ini1, const IntervalVector& y_box_ini1, double prec_x, double prec_y, double stop_prec) {

	std::cout<<"start init"<<std::endl;

	x_box_init = x_box_ini1;
	y_box_init = y_box_ini1;

	//****** initialization of the first Cell********
	Cell * root = new Cell(x_box_init);
	bsc->add_backtrackable(*root);
	lsolve.add_backtrackable(*root,y_box_init);
	x_heap_costfunc.add_backtrackable(*root);


	//****** x_heap initialization ********
	x_heap.flush();
	x_heap.push(root);


	//************** algo variables **************

	uplo=NEG_INFINITY;
	loup= POS_INFINITY; // upper and lower bound enclosure of minimum value initialization
	minprec_uplo= POS_INFINITY;

	Cell *x_cell;
	int nb_iter= 10;
	double min_prec_light_solver=prec_y;


	std::cout<<"initialization ok"<<std::endl;
	Timer::start();
	while((!x_heap.empty()) &&(loup-uplo<stop_prec)) { // stop criterion reached

		x_cell = x_heap.pop();

		// TODO je ne sais pas trop ce que ca fait
		//if(!min_prec_reached || ((minprec_uplo < x_cell->get<DataMinMax>().fmax.lb()) && min_prec_reached))
		//    uplo = x_cell->get<DataMinMax>().fmax.lb();

		std::pair<Cell*,Cell*> subcells_pair = bsc->bisect(*x_cell);

		delete x_cell;


		handle_cell(subcells_pair.first,prec_x,prec_y);
		if (subcells_pair.first) x_heap.push(subcells_pair.first);

		handle_cell(subcells_pair.second,prec_x,prec_y);
		if (subcells_pair.second) x_heap.push(subcells_pair.second);
	}
	Timer::stop();
	x_heap.flush();
	std::cout<<"loup : "<<loup<<" get for point: "<<best_sol<<" uplo: "<<uplo<<std::endl; // " volume rejected: "<<vol_rejected/init_vol*100<<endl;
	//if(min_prec_reached)
	//    cout<<"minimum precision on x has been reached"<<endl;
	std::cout<<"result found in "<<Timer::VIRTUAL_TIMELAPSE()<<std::endl;

	// TODO faire des vrais flag de retour
}


void  OptimMiniMax::handle_cell(Cell * x_cell, double prec_x, double prec_y) {

	DataMinMax * data_x = &(x_cell->get<DataMinMax>());

	//***************** contraction w.r.t constraint on x ****************
	if(check_constraints(x_cell->box)==2)
		data_x->pu=1;
	if (data_x->pu != 1)     {
		x_ctc.contract(x_cell->box);
		if(x_cell->box.is_empty()) {
			//vol_rejected += x_cell->box.volume();
			delete x_cell;
			x_cell =NULL;
			//cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
			return ;
		}
		/*  TODO faire des options de TRACE
            else if(x_cell->box !=  box_mem) {
                vol_rejected += box_mem.volume()-x_cell->box.volume();
                cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
            }
		 */
	}
	//************ evaluation of f(x,y_heap) *****************
	double min_prec_light_solver = compute_min_prec(x_cell->box,prec_y);
	int nb_iter = choose_nbiter(false);
	// compute
	lsolve.optimize(x_cell,nb_iter,min_prec_light_solver);

	if(x_cell==NULL) { // certified that x box does not contains the solution
		//vol_rejected += x_cell->box.volume();
		//x_cell->y_heap.flush();
		//delete x_cell;
		//cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
		return;
	}

	//************* midpoint evaluation ****************
	IntervalVector  midp = get_feasible_point(x_cell);
	if(!midp.is_empty())    { // we found a feasible point
		Cell *x_copy = new Cell(*x_cell); // copy of the Cell and the y_heap
		x_copy->box=midp;

		nb_iter = choose_nbiter(true);   // need to be great enough so the minimum precision on y is reached
		lsolve.optimize(x_copy,nb_iter,min_prec_light_solver); // eval maxf(midp,heap_copy), go to minimum prec on y to get a thin enclosure
		Interval resmidp = x_copy->get<DataMinMax>().fmax;

		if(x_copy && resmidp.ub()<loup) { // update best current solution
			loup = resmidp.ub();
			best_sol = midp;
			//max_y = heap_copy.top1()->box;
			//cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
		}
		delete x_copy; // delete copy of the heap, no more use after the computation of max f(midp,heap_copy)
	}


	//***** if x_cell is too small ******************
	if(x_cell->box.max_diam()<prec_x) {
		nb_iter = choose_nbiter(true);   // need to be great enough so the minimum precision on y is reached

		//cout<<"fmax ini: "<<x_cell->fmax<<endl;
		lsolve.optimize(x_cell,nb_iter,prec_y); // eval maxf(midp,heap_copy), go to minimum prec on y to get a thin enclosure
		//cout<<"fmax min prec: "<<x_cell->fmax<<endl;

		if(x_cell){
			if(minprec_uplo>x_cell->get<DataMinMax>().fmax.lb())
				minprec_uplo = x_cell->get<DataMinMax>().fmax.lb();
		}

		delete x_cell;
		x_cell =NULL;
		//cout<<"minprec reached! "<<" box: "<<x_cell->box<<" eval full prec: "<<x_cell->fmax <<endl;
		//cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
		//min_prec_reached = true;
	}
	return;
}



double OptimMiniMax::compute_min_prec( const IntervalVector& x_box,double prec_y) {
	double ratio = 0;
	for(int r=0;r<x_box_init.size();r++)
		ratio += (x_box_init[r]).diam()/(x_box[r]).diam();
	return ratio/y_box_init.volume()>prec_y?ratio/(10*y_box_init.volume()):prec_y;
}

double OptimMiniMax::choose_nbiter(bool midpoint_eval) {
	if(!midpoint_eval)
		return 10;
	else
		return 1000000000;
}

IntervalVector OptimMiniMax::get_feasible_point(Cell * elem) {
	Vector mid_x = elem->box.mid(); // get the box (x,mid(y))
	if( (!x_sys.ctrs.is_empty()) && (elem->get<DataMinMax>().pu != 1)) { // constraint on xy exist and is not proved to be satisfied
		int res = check_constraints(mid_x);
		if(res==0 || res==1)
			return IntervalVector(1,Interval::EMPTY_SET);
	}
	return mid_x;
}

int OptimMiniMax::check_constraints(const IntervalVector& box) {
	int res =2;
	for(int i=0;i<x_sys.ctrs.size();i++) {
		Interval int_res = x_sys.ctrs[i].f.eval(box);
		if(int_res.lb()>=0)
			return 0;
		else if(int_res.ub()>=0)
			res = 1;
	}
	return res;
}


}

