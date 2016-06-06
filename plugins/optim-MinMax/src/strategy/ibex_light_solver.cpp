#include "ibex_light_solver.h"



light_solver::light_solver(Ctc * ctc_xy,Ctc * ctc_xy_inv): ctc_xy(ctc_xy),ctc_xy_inv(ctc_xy_inv)
{}


Interval light_solver::optimize(Heap<y_heap_elem> * y_heap,IntervalVector* x_box,Function * objective_function, unsigned nb_iter,double best_max,Interval fmax, double min_prec) {

//    if(y_heap->empty())
//        return Interval::EMPTY_SET;
//    cout<<"lsolve: start function optimize"<<endl;
    IntervalVector xy_box(x_box->size()+y_heap->top()->box.size());
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
    IntervalVector mid_y_box(xy_box.size()),mid_y_box_cp(xy_box.size());
    Interval midres;
    double uplo(fmax.lb()),loup(fmax.ub());
    bool midp_hit(false);// true if at least 1 y found such as constraint xy is respected

    std::vector<y_heap_elem*> Heap_save; // element of y_heap reaching min_prec are removed from y_heap and pushed in a save heap

//    cout<<"lsolve: init ok"<<endl;



    for(unsigned i = 0;i<nb_iter;i++) {
//        cout<<"iteration: "<<i<<endl;
        if(y_heap->empty()){ // no more element in the list, happen if all elements of y_heap have reached min_prec
//            cout<<"lsove: y_heap empty"<<endl;
            break;
        }
        tmp_cell = y_heap->pop();
//        cout<<"lsolve: tmp_cell eval: "<<tmp_cell->pf<<endl;
        pair<IntervalVector,IntervalVector> subboxes = lf.bisect(tmp_cell->box);
        subcells_pair = tmp_cell->bisect(subboxes.first,subboxes.second);// bisect tmp_cell into 2 subcells
        subcells[0] = subcells_pair.first;
        subcells[1] = subcells_pair.second;
//        cout<<"lsolve: boxes after bissection: "<<subcells[0]->box<<" and "<<subcells[1]->box<<endl;

//        cout<<"lsolve: bisection ok"<<endl;

        for(unsigned j=0;j<=1;j++) { // deals with the two subcells
            for(unsigned k = 0;k<subcells[j]->box.size();k++) // update current y box in the xy_box
                xy_box[k+x_box->size()] = subcells[j]->box[k];
//            cout<<"lsolve: xy_box after init: "<<xy_box<<endl;
            xy_box_ctc = xy_box;
            if(ctc_xy != 0) // there is a constraint on x and y
            {
//                cout<<"lsolve: contraction"<<endl;
                ctc_xy->contract(xy_box_ctc);
//                cout<<"lsolve: contraction pass"<<endl;
                if(xy_box_ctc.is_empty()) { // constraint on x and y not respected, move on.
                    delete subcells[j];
                    continue;
                }
            }
            //mid point test (TO DO: replace with local optim to find a better point than midpoint)
            mid_y_box = get_mid_y(xy_box,subcells[j]->box); // get the box (x,mid(y))
            if( (ctc_xy_inv!=NULL) && (subcells[j]->pu != 1)) { // constraint on xy exist and is not proved to be satisfied
                 mid_y_box_cp = mid_y_box;
//                 cout<<"lsolve: midp contraction"<<endl;
                ctc_xy_inv->contract(mid_y_box_cp);
//                cout<<"lsolve: midp contraction pass"<<endl;
            }
            if(ctc_xy_inv == NULL || mid_y_box_cp.is_empty() || (subcells[j]->pu == 1)) {// x y constraint respected for all x and mid(y), mid_y_box is a candidate for evaluation
                midp_hit = true;
                midres = objective_function->eval(mid_y_box);
//                cout<<"lsove: res of midpoint"<<mid_y_box<<" evaluation: "<<midres<<endl;
                if (midres.lb()>best_max ) { // exists y such as constraint is respected and f(x,y)>best max, x does not contains the solution
//                    cout<<"lsolve: midpoint rejection*******************"<<endl;
                    delete subcells[j];
                    if(j==0)
                        delete subcells[j+1]; // have to delete the other subcell, it is not in y_heap hence will not be deleted when flushing y_heap in minimax_solver
                    return Interval::EMPTY_SET; // no need to go further, x_box does not contains the solution

                }
                else if(midres.lb()>uplo) { // found y such as xy constraint is respected

                    uplo = midres.lb();
//                    if(x_box->is_flat())
//                        cout<<"lb updated to "<<uplo<<endl;
                }
            }

            // eval entire box

            //*********** this part may be not needed, check if all xy box respect constraint, if so no need to contract subboxes of xy later *********
            if(ctc_xy_inv!=NULL && (subcells[j]->pu != 1)) { // constraint on xy exist and is not proved to be satisfied
                xy_box_cp = xy_box;
                ctc_xy_inv->contract(xy_box_cp);
                if(xy_box_cp.is_empty())
                    subcells[j]->pu =1;

            }
            //*****************************************

            //************ part below add a contraction w.r.t f(x,y)<best_max, this part may not be efficient on every problem ******************************
//            if(ctc_xy_inv == NULL || subcells[j]->pu == 1) { // no constraint or xy constraint respected for all xy
////                cout<<"xy_box before max_ctc contraction: "<<xy_box<<endl;
//                max_ctc.contract(xy_box); // contract xy_box w.r.t lower than best max constraint, /!\ xy_box contracted! => x box of xy_box may have be contracted, must keep this result!
////                cout<<"xy_box after max_ctc contraction: "<<xy_box<<endl;
//                if(xy_box.subvector(0,x_box->size()-1).is_empty()) { // there are no x in x_box such as exists y in y_box, f(x,y)<best_max
////                    cout<<"lsolve: null contraction rejection *********************"<<endl;
//                    delete subcells[j];
//                    if(j==0)
//                        delete subcells[j+1];
//                    return Interval::EMPTY_SET;

//                }
//                else
//                    xy_box_ctc.operator &=(xy_box); // result of contraction w.r.t the two constraints: best max and xy constraints
//            }
//            else
//                if(ctc_xy_inv == NULL)
//                    max_ctc.contract(xy_box_ctc); // cannot keep x contraction as it is not certified that xy constraint is respected forall x and y in xy_box, xy_box_ctc contracted instead
//            if(xy_box_ctc.is_empty()) { // not point (x,y) in xy_box such as both constraints are respected
////                cout<<"lsolve: xy_box_ctc empty"<<endl;
//                delete subcells[j];
//                continue;
//            }
            //********************************************

            subcells[j]->pf = objective_function->eval(xy_box_ctc); // objective function evaluation
//            if(x_box->is_flat())
//                cout<<"evaluation of box "<<xy_box_ctc<<" : "<<subcells[j]->pf<<endl;
            if(subcells[j]->pf.ub()<uplo) {  // y_box cannot contains max f(x,y)
//                cout<<"cell deleted"<<endl;
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
    if(x_box->is_flat() && !midp_hit) // midpoint x eval case: if no y found such as xy constraint respected, cannot keep the result
        return Interval::EMPTY_SET;
    while(!Heap_save.empty()) { // push all boxes of heap_save in y_heap
        tmp_cell = Heap_save.back();
        y_heap->push(tmp_cell);
        Heap_save.pop_back();
    }

    if(y_heap->empty())
        return Interval::EMPTY_SET;


    loup = y_heap->top()->pf.ub(); // get the upper bound of max f(x,y_heap)
//    cout<<"y_heap top ub: "<<loup<<endl;
//    if(*x_box != xy_box.subvector(0,x_box->size()-1))
//        cout<<"contraction usefull: "<<(*x_box)<<" -> "<< xy_box.subvector(0,x_box->size()-1)<<endl;
//    if(!x_box->is_flat()) // no midpoint eval on x
//        *x_box = xy_box.subvector(0,x_box->size()-1); // update x_box
    return Interval(uplo,loup);

}

IntervalVector get_mid_y(const IntervalVector& xy_box,const IntervalVector& y_box) { // returns the cast of box x and mid of box y
    Vector y_mid = y_box.mid();
    IntervalVector res(xy_box);
    for(unsigned i=0;i<y_box.size();i++) {
        res[xy_box.size()-y_box.size()+i] = Interval(y_mid[i]);
    }
    return res;
}
