//============================================================================
//                                  I B E X
// File        : ibex_OptimMinMax.cpp
// Author      : Dominique Monnet, Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 1, 2016
//============================================================================

#include "ibex_OptimMinMax.h"
#include "ibex_LargestFirst.h"
#include "ibex_Timer.h"
#include <stdio.h>
#include "ibex_DataMinMax.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;

namespace ibex {

//Objectif function default parameters for light solver
const int OptimMinMax::default_iter = 10;
const int OptimMinMax::default_list_rate = 0;
const double OptimMinMax::default_min_perc_coef = 1;
const int OptimMinMax::default_list_elem_absolute_max = 5000;
const int OptimMinMax::default_prob_heap = 10; //10% to pop second heap in light_solver
const int OptimMinMax::default_local_iter = 0; //10% to pop second heap in light_solver

// Csp default parameters for light solver
const int OptimMinMax::default_iter_csp = 10;
const int OptimMinMax::default_list_rate_csp = 0;
const double OptimMinMax::default_min_perc_coef_csp = 0;
const int OptimMinMax::default_list_elem_absolute_max_csp = 100;
const int OptimMinMax::default_prob_heap_csp = 0; //0% to pop second heap in light_solver
const int OptimMinMax::default_local_iter_csp = 0; //10% to pop second heap in light_solver


OptimMinMax::OptimMinMax(NormalizedSystem& x_sys,NormalizedSystem& xy_sys, Ctc& x_ctc,Ctc& xy_ctc,double prec_x,double prec_y,double goal_rel_prec):
                                                                Optim(x_sys.nb_var, new CellDoubleHeap(*new CellCostFmaxlb_opt(), *new CellCostFmaxub_opt()),
                                                                                prec_x, goal_rel_prec, goal_rel_prec, 1), // attention meme precision en relatif et en absolue
                                                                                x_box_init(x_sys.box), y_box_init(xy_sys.box.subvector(x_sys.nb_var, xy_sys.nb_var-1)), trace_freq(10000),
                                                                                lsolve(xy_sys,xy_ctc,NULL),
                                                                                x_ctc(x_ctc),x_sys(x_sys),
                                                                                bsc(new LargestFirst()),
                                                                                prec_y(prec_y),
                                                                                iter(default_iter),
                                                                                list_rate(default_list_rate),
                                                                                min_perc_coef(default_min_perc_coef),
                                                                                critpr(default_prob_heap),
                                                                                list_elem_absolute_max(default_list_elem_absolute_max),
                                                                                local_iter(0),
                                                                                fa_lsolve(xy_sys,xy_ctc,NULL,true), // useless if no fa cst but need to construct it...
                                                                                fa_y_cst(false),
                                                                                y_box_init_fa(IntervalVector(1)),
                                                                                monitor(false),
                                                                                only_csp(false),
                                                                                monitor_csp(false)
{
        Array<const ExprNode> args(xy_sys.goal->nb_arg());
        Array<const ExprSymbol> var;
        for(int i = 0;i<xy_sys.goal->nb_arg();i++) {
            const ExprSymbol& a = ExprSymbol::new_(xy_sys.goal->arg(i).dim);
            var.add(a);
            args.set_ref(i,a);
        }
        minus_goal = new Function(var,-(*xy_sys.goal)(args));

//        minus_goal_csp = minus_goal;// does not matter, fa solver not used

        local_search = new UnconstrainedLocalSearch(*minus_goal,IntervalVector(1));
        lsolve.local_solver = local_search;
    //    fa_lsolve(xy_sys,xy_ctc,UnconstrainedLocalSearch(minus_goal_csp,IntervalVector(1)),true); // useless if no fa cst but need to construct it...

};

OptimMinMax::OptimMinMax(NormalizedSystem& x_sys,NormalizedSystem& xy_sys,NormalizedSystem& max_fa_y_cst_sys, Ctc& x_ctc,Ctc& xy_ctc,
                double prec_x,double prec_y,double goal_rel_prec,double fa_cst_prec):
                                                Optim(x_sys.nb_var, new CellDoubleHeap(*new CellCostFmaxlb_opt(), *new CellCostFmaxub_opt()),
                                                                prec_x, goal_rel_prec, goal_rel_prec, 1), // attention meme precision en relatif et en absolue
                                                                x_box_init(x_sys.box), y_box_init(xy_sys.box.subvector(x_sys.nb_var, xy_sys.nb_var-1)), trace_freq(10000),
                                                                x_ctc(x_ctc),x_sys(x_sys),
                                                                lsolve(xy_sys,xy_ctc,NULL),
                                                                bsc(new LargestFirst()),
                                                                prec_y(prec_y),
                                                                iter(default_iter),
                                                                list_rate(default_list_rate),
                                                                min_perc_coef(default_min_perc_coef),
                                                                critpr(default_prob_heap),
                                                                list_elem_absolute_max(default_list_elem_absolute_max),
                                                                local_iter(default_local_iter),
    								prec_fa_y(fa_cst_prec),
                                                                fa_lsolve(max_fa_y_cst_sys,xy_ctc,NULL,true), // construct light solver for for all y cst with fake contractor, contractor useless since no constraint on xy only the objective function matter
                                                                fa_y_cst(true),
                                                                y_box_init_fa(max_fa_y_cst_sys.box.subvector(x_sys.nb_var, max_fa_y_cst_sys.nb_var-1)),
                                                                iter_csp(default_iter_csp),
                                                                list_rate_csp(default_list_rate_csp),
                                                                min_perc_coef_csp(default_min_perc_coef_csp),
                                                                critpr_csp(default_prob_heap_csp),
                                                                list_elem_absolute_max_csp(default_list_elem_absolute_max_csp),
                                                                local_iter_csp(default_local_iter_csp),
                                                                monitor(false),
                                                                only_csp(false),
                                                                monitor_csp(false)
{
        Array<const ExprNode> args(xy_sys.goal->nb_arg());
        Array<const ExprSymbol> var;
        for(int i = 0;i<xy_sys.goal->nb_arg();i++) {
            const ExprSymbol& a = ExprSymbol::new_(xy_sys.goal->arg(i).dim);
            var.add(a);
            args.set_ref(i,a);
        }
        minus_goal = new Function(var,-(*xy_sys.goal)(args));

        Array<const ExprNode> args_csp(max_fa_y_cst_sys.goal->nb_arg());
        Array<const ExprSymbol> var_csp;
        for(int i = 0;i<max_fa_y_cst_sys.goal->nb_arg();i++) {
            const ExprSymbol& a = ExprSymbol::new_(max_fa_y_cst_sys.goal->arg(i).dim);
            var_csp.add(a);
            args_csp.set_ref(i,a);
        }

        minus_goal_csp = new Function(var_csp,-(*max_fa_y_cst_sys.goal)(args_csp));

        local_search = new UnconstrainedLocalSearch(*minus_goal,IntervalVector(1));
        lsolve.local_solver = local_search;

        local_search_csp = new UnconstrainedLocalSearch(*minus_goal_csp,IntervalVector(1));
        fa_lsolve.local_solver = local_search_csp;

};


OptimMinMax::~OptimMinMax() {
	buffer->flush();
	delete &(buffer->cost1());
	delete &(buffer->cost2());
	delete buffer;
	delete bsc;
        delete minus_goal;
        delete minus_goal_csp;
}

Optim::Status OptimMinMax::optimize() {
	return optimize(x_sys.box,POS_INFINITY);
}


Optim::Status OptimMinMax::optimize(const IntervalVector& x_box_ini1, double obj_init_bound) {
//	bool monitor = false;
	if (trace) lsolve.trace =trace;
	// we estimate that one iteration is at most 1% of the total time
	lsolve.timeout=timeout/100;
        fa_lsolve.timeout = timeout/100;

	loup=obj_init_bound;

	uplo=NEG_INFINITY;
	uplo_of_epsboxes=POS_INFINITY;

	x_box_init = x_box_ini1;

	//****** initialization of the first Cell********
	Cell * root = new Cell(x_box_init);
	bsc->add_backtrackable(*root);
        lsolve.add_backtrackable(*root,y_box_init,critpr);
        buffer->cost1().add_backtrackable(*root);
        buffer->cost2().add_backtrackable(*root);

        if(fa_y_cst) {
            fa_lsolve.add_backtrackable(*root,y_box_init_fa,critpr_csp);
        }

        //************* set local search solver param *********
        lsolve.local_search_iter = local_iter;
        cout<<"local_search_iter: "<<local_iter<<endl;
        fa_lsolve.local_search_iter = local_iter_csp;


	//****** x_heap initialization ********
	nb_cells=0;
        buffer->flush();

	// *** initialisation Algo ***
	loup_changed=false;
	double ymax;
        initial_loup=obj_init_bound;
	loup_point=x_box_init.mid();
        time=0;
        Timer::reset_time();
        Timer::start();

        handle_cell(root);

        update_uplo();

	//monitoring variables, used to track upper bound, lower bound, number of elem in y_heap and heap_save at each iteration
	std::vector<double> ub,lb,nbel,nbyel;
	long long int nbel_count(0);
        bool handle_res1,handle_res2;

        try {
                while (!buffer->empty() && (loup-uplo)>goal_rel_prec) {
			//			if (trace >= 2) cout << " buffer " << buffer << endl;
			if (trace >= 2) buffer->print(cout);


			loup_changed=false;

			Cell *c = buffer->pop();
                        if(monitor) {
                            DataMinMax * data_x = &(c->get<DataMinMaxOpti>());
                            nbel_count -= data_x->y_heap->size();
                        }


			try {
				pair<Cell*,Cell*> new_cells=bsc->bisect_cell(*c);
				delete c; // deletes the cell.

                                handle_res1 = handle_cell(new_cells.first);
				if(handle_res1 && monitor) {
//					cout<<"try to get backtrack 1"<<endl;
                                        DataMinMax * data_x1 = &((new_cells.first)->get<DataMinMaxOpti>());
//					cout<<"get backtrack 1"<<endl;
					nbel_count += data_x1->y_heap->size();
				}

                                handle_res2 = handle_cell(new_cells.second);

				if(handle_res2 && monitor) {
//					cout<<"try to get backtrack 2"<<endl;
                                        DataMinMax * data_x2 = &((new_cells.second)->get<DataMinMaxOpti>());
//					cout<<"get backtrack 2"<<endl;
					nbel_count += data_x2->y_heap->size();
				}


				if (uplo_of_epsboxes == NEG_INFINITY) {
					cout << " possible infinite minimum " << endl;
					break;
				}
				if (loup_changed) {
					// In case of a new upper bound (loup_changed == true), all the boxes
					// with a lower bound greater than (loup - goal_prec) are removed and deleted.
					// Note: if contraction was before bisection, we could have the problem
					// that the current cell is removed by contractHeap. See comments in
					// older version of the code (before revision 284).

					ymax=compute_ymax();

					buffer->contract(ymax);
					//cout << " now buffer is contracted and min=" << buffer.minimum() << endl;


					if (ymax <= NEG_INFINITY) {
						if (trace) cout << " infinite value for the minimum " << endl;
						break;
					}
					if (trace) cout <<  "iter="<< nb_cells <<",  size_heap="<< buffer->size()<< ",  ymax=" << ymax << ",  uplo= " <<  uplo<< endl;
				}
				update_uplo();

				if(monitor)
				{

					lb.push_back(uplo);
                                        ub.push_back(loup);
					nbel.push_back(buffer->size());
					nbyel.push_back(nbel_count);
				}


				Timer::check(timeout);

			}
			catch (NoBisectableVariableException& ) {
                                bool res=handle_cell(c);
                                if (res) update_uplo_of_epsboxes(c->get<DataMinMaxOpti>().fmax.lb());
				//if (trace>=1) cout << "epsilon-box found: uplo cannot exceed " << uplo_of_epsboxes << endl;
				update_uplo(); // the heap has changed -> recalculate the uplo

			}
		}
		if(monitor)
			export_monitor(&ub,&lb,&nbel,&nbyel);
	}
	catch (TimeOutException& ) {
		Timer::stop();
		time = Timer::get_time();
		return TIME_OUT;
	}

	Timer::stop();
	time = Timer::get_time();

	if (uplo_of_epsboxes == POS_INFINITY && (loup==POS_INFINITY || (loup==initial_loup && goal_abs_prec==0 && goal_rel_prec==0)))
		return INFEASIBLE;
	else if (loup==initial_loup)
		return NO_FEASIBLE_FOUND;
	else if (uplo_of_epsboxes == NEG_INFINITY)
		return UNBOUNDED_OBJ;
	else
		return SUCCESS;
}





bool  OptimMinMax::handle_cell(Cell * x_cell) {

        DataMinMaxOpti * data_x = &(x_cell->get<DataMinMaxOpti>());
//        cout<<"handle cell of box: "<<x_cell->box<<endl<<"and of fmax: "<<data_x->fmax<<endl;


        ofstream out;
        if(monitor_csp) {
            out.open("paver.txt",std::ios_base::app);
            if(!out.is_open())
                cout<<"ERROR: cannot open paver.txt"<<endl;
        }
	// ATTENTION CECI EST FAUX !!!!!
	//double ymax;
	//if (loup==POS_INFINITY) ymax=POS_INFINITY;
	//else ymax= compute_ymax()+1.e-15;
	// update the upper bound with the loup
	//data_x->fmax &= Interval(NEG_INFINITY,loup);
	//std::cout <<" debut "<<data_x->fmax <<std::endl;
	//if (data_x->fmax.is_empty()) {
	//	delete x_cell;
	//	return false;
	//}
	// ATTENTION CECI EST FAUX !!!!!

	//***************** contraction w.r.t constraint on x ****************
//        cout<<"checking ctr.... "<<endl;
        IntervalVector tmpbox(x_cell->box);
        int res_cst = check_constraints(x_cell,false);
        if (res_cst == 0) {
            if(monitor_csp) {
                for(int i=0; i<tmpbox.size();i++) {
                    out<<tmpbox[i].lb()<<" "<<tmpbox[i].ub()<<" ";
                }
            out<<res_cst<<endl;
            }
            out.close();
            return false;
        }
        else if (res_cst==2 && only_csp) {
            if(monitor_csp) {
                for(int i=0; i<x_cell->box.size();i++)
                    out<<x_cell->box[i].lb()<<" "<<x_cell->box[i].ub()<<" ";
            }
            out<<res_cst<<endl;
            delete x_cell; // need to delete x_cell because not deleted in check cst.
            out.close();
            return false;
        }
        else if (data_x->pu != 1)     {
//                x_ctc.contract(x_cell->box);
                if(x_cell->box.is_empty()) {
                        //vol_rejected += x_cell->box.volume();
                        delete x_cell;
                        //cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
                        return false;
                }
                /*  TODO faire des options de TRACE
            else if(x_cell->box !=  box_mem) {
                vol_rejected += box_mem.volume()-x_cell->box.volume();
                cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
            }
                 */
        }
//        cout<<"done"<<endl;
//        cout<<" evaluation of objectif... "<<endl;
        if(!only_csp) {

            //************ evaluation of f(x,y_heap) *****************
            lsolve.prec_y = compute_min_prec(x_cell->box,false);
            lsolve.nb_iter = choose_nbiter(false,false);
            lsolve.list_elem_max = compute_heap_max_size(data_x->y_heap->size(),false);
    //        CellCostmaxFmaxub cost;
    //        cout<<"cost after eval: "<<cost.cost(*x_cell)<<endl;
    //        DataMinMax *data_opt_x = &(x_cell->get<DataMinMax>());
    //        cout<<"fmax from DataMinMax cast: "<<data_opt_x->fmax<<endl;
            // compute
    //                cout<<"run light optim with box: "<<x_cell->box<<endl;
            bool res =lsolve.optimize(x_cell,loup);
    //                cout<<"light optim result: "<<data_x->fmax<< endl;
            //std::cout<<" apres res="<<res<<" bound= "<<data_x->fmax <<std::endl;

    //        cout<<"done"<<endl;
    //        cout<<"midpoint evaluation... "<<endl;
            if(!res) { // certified that x box does not contains the solution
                    // TODO Trace
                    //vol_rejected += x_cell->box.volume();
                    //x_cell->y_heap.flush();
                    //                delete x_cell;
                    //cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
                    return false;
            }

            //************* midpoint evaluation ****************
            Cell *x_copy = new Cell(*x_cell); // copy of the Cell and the y_heap
            bool found_feas_pt = get_feasible_point(x_copy);
            if(found_feas_pt)    { // we found a feasible point

    //                cout<<endl<<"**************"<<endl;
    //                                cout<<"try mid point "<<x_cell->box.mid()<<", init fmax: "<<x_copy->get<DataMinMaxOpti>().fmax<<endl;
                    lsolve.nb_iter = choose_nbiter(true,false);
                    lsolve.prec_y = prec_y;
                    lsolve.list_elem_max = 0; // no limit on heap size
                    bool res1 = lsolve.optimize(x_copy,loup); // eval maxf(midp,heap_copy), go to minimum prec on y to get a thin enclosure
    //                cout<<"res1: "<<res1<<endl;


                    if (res1) {
    //                    cout<<"res midp eval: "<<x_copy->get<DataMinMaxOpti>().fmax<<endl;
                            double new_loup = x_copy->get<DataMinMaxOpti>().fmax.ub();

                            //data_x->fmax &= Interval(NEG_INFINITY,new_loup);

                            if(new_loup<loup) { // update best current solution
                                    //cout<<"worst case over Y: "<<x_copy->get<DataMinMax>().y_heap->top1()->box<<endl;
                                    loup = new_loup;
                                    loup_changed = true;
                                    loup_point = (x_copy->box.mid());

                                    if (trace) cout << "[mid]"  << " loup update " << loup  << " loup point  " << loup_point << endl;
                                    //cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
                            }
                            delete x_copy; // delete copy of the heap, no more use and it was not delete in light optim since res1 = 1
                    }
            }
    //        else // need to delete x_copy
    //            delete x_copy;
    //        cout<<"done"<<endl;
    //        cout<<"check min prec..."<<endl;
        }

        //***** if x_cell is too small ******************
        if(x_cell->box.max_diam()<prec) {
//            cout<<"Min prec, box: "<<x_cell->box<<endl;
            int ctr_ok = 2;
            tmpbox = x_cell->box;
            if(fa_y_cst) { // go min precision on csp problem, else suppose the ctr on x is respect because x_cell not deleted when first check constraints at the begining of handle_cell()
                ctr_ok= check_constraints(x_cell, true);
            }
            if(only_csp && monitor_csp) {
                for(int i=0; i<x_cell->box.size();i++) {
                    out<<tmpbox[i].lb()<<" "<<tmpbox[i].ub()<<" ";
                }
                out<<ctr_ok<<endl;
                if(ctr_ok!=0)
                    delete x_cell;
                out.close();
                return false;
            }
            if (ctr_ok !=0 && !only_csp) {
                lsolve.nb_iter = choose_nbiter(true,false);   // need to be great enough so the minimum precision on y is reached
                lsolve.prec_y = prec_y;
                lsolve.list_elem_max = 0; // no limit on heap size
//                cout<<"fmax ini: "<<data_x->fmax<<endl;
                //cout<<"for box: "<<x_cell->box<<endl;
                bool res =lsolve.optimize(x_cell,loup); // eval maxf(midp,heap_copy), go to minimum prec on y to get a thin enclosure
//                cout<<"fmax min prec: "<<data_x->fmax<<endl;

                if(res){
                        update_uplo_of_epsboxes(data_x->fmax.lb());
                        delete x_cell;
                }
            }
                //cout<<"minprec reached! "<<" box: "<<x_cell->box<<" eval full prec: "<<x_cell->fmax <<endl;
                //cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
                //min_prec_reached = true;
                return false;
        }
//        cout<<"done"<<endl;
//        cout<<"update cell data..."<<endl;


        /*	if (data_x->fmax.is_empty()) {
                delete x_cell;
                return false;
        }*/

        // update optim data of the cell
        buffer->cost2().set_optim_data(*x_cell,x_sys);
        buffer->push(x_cell);
        nb_cells++;

        if (trace && (nb_cells%trace_freq==0)) cout <<  "iter="<< nb_cells <<",  size_heap="<< buffer->size()<< ",  loup=" << loup << ",  uplo= " <<  uplo<< endl;

//        std::cout<<" fin "<<data_x->fmax <<std::endl;
//        cout<<"done, exit handle_cell()"<<endl;
	return true;
}



double OptimMinMax::compute_min_prec( const IntervalVector& x_box,bool csp) {

    if(!csp) {
        if(min_perc_coef == 0)
                return prec_y;

        double ratio = 0;
        for(int r=0;r<x_box_init.size();r++)
                ratio += (x_box_init[r]).diam()/(x_box[r]).diam();
        ratio/=x_box.size();
        double new_prec = y_box_init.max_diam()*ratio/min_perc_coef;
        return new_prec>prec_y?new_prec:prec_y;
    }
    else {
        if(min_perc_coef_csp == 0)
                return prec_y;

        double ratio = 0;
        for(int r=0;r<x_box_init.size();r++)
                ratio += (x_box_init[r]).diam()/(x_box[r]).diam();
        ratio/=x_box.size();
        double new_prec = y_box_init_fa.max_diam()*ratio/min_perc_coef_csp;
        return new_prec>prec_fa_y?new_prec:prec_fa_y;
    }
}

int OptimMinMax::choose_nbiter(bool midpoint_eval,bool csp) {

        if(!midpoint_eval) {
            if(!csp)
                return iter;
            else
                return iter_csp;
        }
        else
            return 0; // go to min prec
}

int OptimMinMax::compute_heap_max_size(int y_heap_size,bool csp) {
    if(!csp) {
        if(list_rate == 0 || y_heap_size>= list_elem_absolute_max) { // no constraint on list growth rate or max size already reached
                return list_elem_absolute_max;
        }
        return  y_heap_size+list_rate;
    }
    else {
        if(list_rate_csp == 0 || y_heap_size>= list_elem_absolute_max_csp) { // no constraint on list growth rate or max size already reached
                return list_elem_absolute_max_csp;
        }
        return  y_heap_size+list_rate_csp;
    }
}

bool OptimMinMax::get_feasible_point(Cell * elem) {
//        cout<<endl<<"     get_feasible_point"<<endl;
        elem->box = elem->box.mid(); // get the box (x,mid(y))
        int res = check_constraints(elem,true);
//        cout<<"done, res = "<<res<<endl;
        if(res == 2)
            return true;
        return false;
}

int OptimMinMax::check_regular_ctr(const IntervalVector& box) {
        int res =2;
        for(int i=0;i<x_sys.nb_ctr;i++) {
                Interval int_res = x_sys.ctrs[i].f.eval(box);
                if(int_res.lb()>=0)
                        return 0;
                else if(int_res.ub()>=0)
                        res = 1;
        }
        return res;
}

int OptimMinMax::check_fa_ctr(Cell* x_cell,bool midp) {
    DataMinMax * data_csp = &(x_cell->get<DataMinMaxCsp>());
//    cout<<"data_csp->pu: "<<data_csp->pu<<endl;
    if(data_csp->pu != 1) {
        fa_lsolve.nb_iter = choose_nbiter(midp,true);
        if(midp) {
            fa_lsolve.list_elem_max = 0;
            fa_lsolve.prec_y = prec_fa_y;
        }
        else {
            fa_lsolve.list_elem_max = compute_heap_max_size(data_csp->y_heap->size(),true);
            fa_lsolve.prec_y = compute_min_prec(x_cell->box,true);
        }
        bool ok= fa_lsolve.optimize(x_cell,0);
        if(!ok) {
//            cout<<"     fa ctr not satisfied for box: "<<x_cell->box<<endl;
            return 0;
        }
        else if (data_csp->y_heap->top1()->get<OptimData>().pf.ub()<0) {
            data_csp->pu = 1;
            data_csp->y_heap->flush();
            return 2;
        }
        else return 1;
    }
    return 2;
}

int OptimMinMax::check_constraints(Cell * x_cell,bool midp) {
//        cout<<"     check constraints for box: "<<x_cell->box<<endl;
        int res_rctr = 2;
        int res_factr = 2;
        DataMinMaxOpti * data_opt = &(x_cell->get<DataMinMaxOpti>());
//        cout<<"     check regular ctr..."<<endl;
        if(data_opt->pu != 1)
            res_rctr = check_regular_ctr(x_cell->box);
        if(res_rctr == 2)
            data_opt->pu = 1;
        else if(res_rctr == 0) {
            delete x_cell;
            return 0;
        }
//        cout<<"     done, res_rctr = "<<res_rctr<<endl;
//        cout<<"     check fa ctr..."<<endl;
        if(fa_y_cst)
            res_factr = check_fa_ctr(x_cell,midp);
//        cout<<"     done, res_factr = "<<res_factr<<endl;
//        cout<<"res_factr: "<<res_factr<<endl;
        if(res_rctr == 2 &&  res_factr == 2) // all ctr satisfied
            return 2;
        else if(res_rctr == 0 || res_factr == 0)
            return 0;
        else
            return 1;
}

void export_monitor(std::vector<double> * ub,std::vector<double> * lb,std::vector<double> * nbxel,std::vector<double> * nbyel) {
        std::ofstream out;
        //    std::string output_name = "log.txt";
        out.open("log.txt");
        //    out<<"x box: "<<box<<std::endl;
        for(int i = 0;i<ub->size();i++) {
                out<<ub->at(i)<<" "<<lb->at(i)<<" "<<nbxel->at(i)<<" "<<nbyel->at(i)<<std::endl;
        }
        out.close();

}
}

