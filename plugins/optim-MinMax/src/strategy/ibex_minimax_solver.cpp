#include "ibex_minimax_solver.h"


minimax_solver::minimax_solver(Function *ofunc,Ctc *x_ctc,Ctc *x_ctc_inv,Ctc *xy_ctc,Ctc *xy_ctc_inv):objective_function(ofunc),x_ctc(x_ctc),x_ctc_inv(x_ctc_inv),lsolve(light_solver(xy_ctc,xy_ctc_inv))
{};

void minimax_solver::solve(IntervalVector x_box_ini, IntervalVector y_box_ini, double prec_x, double prec_y, double stop_prec) {

    // ***** y_heap initialization ********
    y_heap_costfub y_heap_costfunc; // element sorted w.r.t upper bound of objectif function evaluation
    Heap<y_heap_elem> y_heap_ini(y_heap_costfunc);  // y heap /!\ This heap will be copy, cannot used DoubleHeap until a copy function is available
    y_heap_elem* y_ini = new y_heap_elem(y_box_ini,Interval::ALL_REALS,0); // first cell of y heap
    y_heap_ini.push(y_ini); // push element in y_heap, y_heap is initialized

    //****** x_heap initialization ********
//    Heap<x_heap_elem> x_heap = init_x_heap(x_box_ini,y_heap_ini);
    x_heap_costflb x_heap_costfunc;
    Heap<x_heap_elem> x_heap(x_heap_costfunc);
    x_heap_elem *x_ini = new x_heap_elem(x_box_ini,y_heap_ini,Interval::ALL_REALS);
    x_heap.push(x_ini);

    //************** algo variables **************
    double uplo(NEG_INFINITY),loup(POS_INFINITY); // upper and lower bound enclosure of minimum value initialization
    LargestFirst lf;
    x_heap_elem *x_cell_tmp;
    x_heap_elem *x_subcells[2];
    pair<x_heap_elem*,x_heap_elem*> subcells_pair;
    unsigned nb_iter(10);
    double min_prec_light_solver(prec_y);
    IntervalVector midp(x_box_ini.size());
    Interval resmidp;
    IntervalVector best_sol(x_box_ini.size());
    IntervalVector box_mem(x_box_ini.size());
    double init_vol = x_box_ini.volume(),vol_rejected(0);
    bool min_prec_reached(false),midpoint_hit(true);
    IntervalVector max_y(y_box_ini.size());


    cout<<"initialization ok"<<endl;
    Timer::start();
    while(!x_heap.empty()) {
        if(loup-uplo<stop_prec) { // stop criterion reached
            break;
        }

        x_cell_tmp = x_heap.pop();
        if(!min_prec_reached)
            uplo = x_cell_tmp->fmax.lb();
        pair<IntervalVector,IntervalVector> subboxes = lf.bisect(x_cell_tmp->box);
        subcells_pair = x_cell_tmp->bisect(subboxes.first,subboxes.second);
        x_subcells[0] = subcells_pair.first;
        x_subcells[1] = subcells_pair.second;
        for(unsigned i=0;i<2;i++) { // run through the 2 subcells
            //***************** contraction w.r.t constraint on x ****************
            box_mem = x_subcells[i]->box;
            if(x_ctc != 0)
            {
                x_ctc->contract(x_subcells[i]->box);
                if(x_subcells[i]->box.is_empty()) {
                    vol_rejected += x_subcells[i]->box.volume();
                    x_subcells[i]->y_heap.flush();
                    delete x_subcells[i];
                    cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
                    continue;
                }
                else if(x_subcells[i]->box !=  box_mem) {
                    vol_rejected += box_mem.volume()-x_subcells[i]->box.volume();
                    cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
                }
            }
            //************ evaluation of f(x,y_heap) *****************
            min_prec_light_solver = compute_min_prec(x_box_ini,x_subcells[i]->box,y_box_ini,prec_y);
            nb_iter = choose_nbiter(false);
            x_subcells[i]->fmax = lsolve.optimize(&(x_subcells[i]->y_heap),&(x_subcells[i]->box),objective_function,nb_iter,loup,x_subcells[i]->fmax,min_prec_light_solver);
            if(x_subcells[i]->fmax == Interval::EMPTY_SET) { // certified that x box does not contains the solution
                vol_rejected += x_subcells[i]->box.volume();
                x_subcells[i]->y_heap.flush();
                delete x_subcells[i];
                cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
                continue;
            }
            //************* midpoint evaluation ****************
            midp = get_feasible_point(x_subcells[i]->box);
            if(!midp.is_empty())
            {
                Heap<y_heap_elem> heap_copy(x_subcells[i]->y_heap); // need to copy the heap for midpoint eval so the y_heap of x box is not modify
                nb_iter = choose_nbiter(true);   // need to be great enough so the minimum precision on y is reached
                resmidp = lsolve.optimize(&(heap_copy),&(midp),objective_function,nb_iter,loup,x_subcells[i]->fmax,prec_y); // eval maxf(midp,heap_copy), go to minimum prec on y to get a thin enclosure

                if(resmidp != Interval::EMPTY_SET && resmidp.ub()<loup) { // update best current solution
                    loup = resmidp.ub();
                    best_sol = midp;
                    max_y = heap_copy.top()->box;
                    cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
                }
                heap_copy.flush(); // delete copy of the heap, no more use after the computation of max f(midp,heap_copy)
            }
            if(x_subcells[i]->box.max_diam()<prec_x) {
                x_subcells[i]->y_heap.flush(); // may not be implemented in the destructor of Heap
                delete x_subcells[i];
                cout<<"minprec reached! "<<endl;
                cout<<"loup : "<<loup<<" get for point: x = "<<best_sol<<" y = "<<max_y<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
                min_prec_reached = true;
            }
            else
                x_heap.push(x_subcells[i]);
        }
    }
    Timer::stop();
    x_heap.flush();
    cout<<"loup : "<<loup<<" get for point: "<<best_sol<<" uplo: "<<uplo<< " volume rejected: "<<vol_rejected/init_vol*100<<endl;
    if(min_prec_reached)
        cout<<"minimum precision on x has been reached"<<endl;
    cout<<"result found in "<<Timer::VIRTUAL_TIMELAPSE()<<endl;
}

Heap<y_heap_elem> minimax_solver::init_y_heap(const IntervalVector& box) {
    y_heap_costfub y_heap_costfunc; // element sorted w.r.t upper bound of objectif function evaluation
    Heap<y_heap_elem> y_heap_ini(y_heap_costfunc);  // y heap /!\ This heap will be copy, cannot used DoubleHeap until a copy function is available
    y_heap_elem* y_ini = new y_heap_elem(box,Interval::ALL_REALS,0); // first cell of y heap
    y_heap_ini.push(y_ini); // push element in y_heap, y_heap is initialized
    return y_heap_ini;
}

Heap<x_heap_elem> minimax_solver::init_x_heap(const IntervalVector& box,Heap<y_heap_elem> y_heap_ini) {
    x_heap_costflb x_heap_costfunc;
    Heap<x_heap_elem> x_heap(x_heap_costfunc);
    x_heap_elem *x_ini = new x_heap_elem(box,y_heap_ini,Interval::ALL_REALS);
    x_heap.push(x_ini);
    return x_heap;
}

double minimax_solver::compute_min_prec(const IntervalVector& x_box_ini, const IntervalVector& x_box,const IntervalVector& y_box_ini,double prec_y) {
    double ratio(0);
    for(unsigned r=0;r<x_box_ini.size();r++)
        ratio += (x_box_ini[r]).diam()/(x_box[r]).diam();
    return ratio/y_box_ini.volume()>prec_y?ratio/(10*y_box_ini.volume()):prec_y;
}

double minimax_solver::choose_nbiter(bool midpoint_eval) {
    if(!midpoint_eval)
        return 10;
    else
        return 1000000000;
}

IntervalVector minimax_solver::get_feasible_point(const IntervalVector& x_box) {
    Vector midp = x_box.mid(); // get midpoint of x box,
    IntervalVector midp_int = IntervalVector(midp);
    if(x_ctc_inv != 0)
    {
        x_ctc_inv->contract(midp_int); // check if midpoint verity the constraint on x
        if(midp_int.is_empty()) { //midp does not verify x constraint
            return IntervalVector(midp);
        }
        else
           return IntervalVector(1,Interval::EMPTY_SET);
    }
    else
        return IntervalVector(midp);
}



