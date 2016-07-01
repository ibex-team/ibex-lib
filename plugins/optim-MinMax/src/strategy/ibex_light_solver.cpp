#include "ibex_light_solver.h"




light_solver::light_solver(Ctc * ctc_xy,NormalizedSystem *y_sys): ctc_xy(ctc_xy),y_sys(y_sys)
{}


Interval light_solver::optimize(DoubleHeap<y_heap_elem> * y_heap,IntervalVector* x_box,Function * objective_function, unsigned nb_iter,double best_max,Interval fmax, double min_prec,bool is_midp) {

//    if(y_heap->empty())
//        return Interval::EMPTY_SET;
//    cout<<"lsolve: start function optimize"<<endl;
    IntervalVector xy_box(x_box->size()+y_heap->top1()->box.size());
    IntervalVector xy_box_ctc(xy_box); // box that will be contracted when dealing with best max constraint and xy constraint, needed to get back the original x box when dealing with another y box
    IntervalVector xy_box_cp(xy_box); // use for contraction w.r.t ctc_xy_inv, check is constraint respected for all x y without loosing the original box
    for(unsigned i =0;i<x_box->size();i++) // initialize box containing x and y
        xy_box[i] = (*x_box)[i];


//    cout<<"lsolve: interval vector ok"<<endl;

    // /!\ Does creating a contractor here cause memory leak ? If so, it must be recreated in the minimax_solver algo only when best_max change to limit the memory leak, and pass it as an argument of this function
    CtcFwdBwd max_ctc((*objective_function),Interval(NEG_INFINITY,best_max)); // contract w.r.t "the objective function must be lower than the current best solution best_max"
//    cout<<"lsolve: contractor ok"<<endl;
    y_heap_elem *tmp_cell; // current cell

    LargestFirst lf; // bissector
    pair<y_heap_elem*,y_heap_elem*> subcells_pair;
    y_heap_elem *subcells[2];
    IntervalVector mid_y_box(xy_box.size());
    Interval midres;
    double uplo(fmax.lb()),loup(fmax.ub());
    bool midp_hit(false);// true if at least 1 y found such as constraint xy is respected

    std::vector<y_heap_elem*> Heap_save; // element of y_heap reaching min_prec are removed from y_heap and pushed in a save heap

    for(unsigned i = 0;i<nb_iter;i++) {
        if(y_heap->empty()){ // no more element in the list, happen if all elements of y_heap have reached min_prec
            break;
        }
        tmp_cell = y_heap->pop1();
        pair<IntervalVector,IntervalVector> subboxes = lf.bisect(tmp_cell->box);
        subcells_pair = tmp_cell->bisect(subboxes.first,subboxes.second);// bisect tmp_cell into 2 subcells
        subcells[0] = subcells_pair.first;
        subcells[1] = subcells_pair.second;

        for(unsigned j=0;j<=1;j++) { // deals with the two subcells
            init_xy_box(&xy_box,subcells[j]->box);
            xy_box_ctc = xy_box;
            if(check_constraints(xy_box)==2) {
                subcells[j]->pu=1;
            }
            if(ctc_xy != 0 && subcells[j]->pu != 1) // there is a constraint on x and y
            {
                ctc_xy->contract(xy_box_ctc);
                if(xy_box_ctc.is_empty()) { // constraint on x and y not respected, move on.
                    delete subcells[j];
                    continue;
                }
            }
            //mid point test (TO DO: replace with local optim to find a better point than midpoint)

            mid_y_box = get_feasible_point(xy_box,subcells[j]);
            if(ctc_xy == NULL || !mid_y_box.is_empty() || (subcells[j]->pu == 1)) {// x y constraint respected for all x and mid(y), mid_y_box is a candidate for evaluation
                midp_hit = true;
                midres = objective_function->eval(mid_y_box);
                if (midres.lb()>best_max ) { // exists y such as constraint is respected and f(x,y)>best max, x does not contains the solution
                    delete subcells[j];
                    if(j==0)
                        delete subcells[j+1]; // have to delete the other subcell, it is not in y_heap hence will not be deleted when flushing y_heap in minimax_solver
                    return Interval::EMPTY_SET; // no need to go further, x_box does not contains the solution

                }
                else if(midres.lb()>uplo) { // found y such as xy constraint is respected
                    uplo = midres.lb();
                }
            }

            //************ part below add a contraction w.r.t f(x,y)<best_max, this part may not be efficient on every problem ******************************

//            contract_best_max_cst(&max_ctc,&xy_box,&xy_box_ctc,subcells[j]);
//            if(xy_box.is_empty()||xy_box_ctc.is_empty()) { // there are no x in x_box such as exists y in y_box, f(x,y)<best_max || no point (x,y) in xy_box such as both constraints are respected
//                delete subcells[j];
//                continue;
//            }
            //********************************************

            subcells[j]->pf = objective_function->eval(xy_box_ctc); // objective function evaluation
            if(subcells[j]->pf.ub()<uplo) {  // y_box cannot contains max f(x,y)
                delete subcells[j];
                continue;
            }
            if(subcells[j]->box.max_diam()<min_prec) { // if min prec reached, keep box in Heap_save
                Heap_save.push_back(subcells[j]);
            }
            else
                y_heap->push(subcells[j]); // add the box  in y_heap
        }
    }
    if(is_midp && !midp_hit) // midpoint x eval case: if no y found such as xy constraint respected, cannot keep the result
        return Interval::EMPTY_SET;

    fill_y_heap(y_heap,&Heap_save);
    if(y_heap->empty())
        return Interval::EMPTY_SET;


    loup = y_heap->top1()->pf.ub(); // get the upper bound of max f(x,y_heap)
    if(uplo<y_heap->top2()->pf.lb())
        uplo = y_heap->top2()->pf.lb();

    return Interval(uplo,loup);

}

void light_solver::contract_best_max_cst(Ctc* max_ctc,IntervalVector* xy_box,IntervalVector* xy_box_ctc,y_heap_elem* elem) {
    if(ctc_xy == NULL || elem->pu == 1) { // no constraint or xy constraint respected for all xy
        max_ctc->contract(*xy_box); // contract xy_box w.r.t lower than best max constraint, /!\ xy_box contracted! => x box of xy_box may have be contracted, must keep this result!
        if(xy_box->is_empty()) { // there are no x in x_box such as exists y in y_box, f(x,y)<best_max
            return;
        }
        else
            xy_box_ctc->operator &=(*xy_box); // result of contraction w.r.t the two constraints: best max and xy constraints
    }
    else
        max_ctc->contract(*xy_box_ctc); // cannot keep x contraction as it is not certified that xy constraint is respected forall x and y in xy_box, xy_box_ctc contracted instead
    return;
}

IntervalVector light_solver::get_feasible_point(const IntervalVector& xy_box,y_heap_elem * elem) {
    IntervalVector mid_y_box = get_mid_y(xy_box,elem->box); // get the box (x,mid(y))
    if( (y_sys!=0) && (elem->pu != 1)) { // constraint on xy exist and is not proved to be satisfied
        int res = check_constraints(mid_y_box);
        if(res == 0 ||res == 1)
            return IntervalVector(1,Interval::EMPTY_SET);
    }
    return mid_y_box;
}

int light_solver::check_constraints(const IntervalVector& box) {
    int res(2);
    Interval int_res;
    for(unsigned i=0;i<y_sys->ctrs.size();i++) {
        int_res =  y_sys->ctrs[i].f.eval(box);
        if(int_res.lb()>=0)
            return 0;
        else if(int_res.ub()>=0)
            res = 1;
    }
    return res;

}



IntervalVector get_mid_y(const IntervalVector& xy_box,const IntervalVector& y_box) { // returns the cast of box x and mid of box y
    Vector y_mid = y_box.mid();
    IntervalVector res(xy_box);
    for(unsigned i=0;i<y_box.size();i++) {
        res[xy_box.size()-y_box.size()+i] = Interval(y_mid[i]);
    }
    return res;
}

void init_xy_box(IntervalVector* xy_box,const IntervalVector & y_box) {
    for(unsigned k = 0;k<y_box.size();k++) // update current y box in the xy_box
        (*xy_box)[k+(xy_box->size()-y_box.size())] = y_box[k];
}

void fill_y_heap(DoubleHeap<y_heap_elem>* y_heap,vector<y_heap_elem*>* Heap_save) {
    y_heap_elem* tmp_cell;
    while(!Heap_save->empty()) { // push all boxes of heap_save in y_heap
        tmp_cell = Heap_save->back();
        y_heap->push(tmp_cell);
        Heap_save->pop_back();
    }
}


