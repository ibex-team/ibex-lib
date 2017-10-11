#include "ibex_LightLocalSolver.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_LinearException.h"


namespace ibex {



CellCostPFlb LightLocalSolver::x_heap_costf;

LightLocalSolver::LightLocalSolver(NormalizedSystem& y_sys,UnconstrainedLocalSearch* local_solver_over_x,UnconstrainedLocalSearch* local_solver_over_y,Ctc& x_ctc,int x_dim, int y_dim,bool csp_actif) :
                        xy_sys(y_sys),local_solver_over_x(local_solver_over_x),local_solver_over_y(local_solver_over_y),nb_sols(0),x_ctc(x_ctc),
                        min_acpt_diam(0),x_dim(x_dim),y_dim(y_dim),csp_actif(csp_actif),
                        nb_sivia_iter(0),nb_optim_iter(0),y_sol_radius(0),
                        reg_acpt_error(0),x_heap(new Heap<Cell>(x_heap_costf)),perf_ctc_coef(1),nb_rdm_candidates(10)
{
//    Cell *x_cell = new Cell(IntervalVector(x_dim));
//    x_cell->add<OptimData>();
//    x_heap_costf.add_backtrackable(*x_cell);
//    delete x_cell;
}

LightLocalSolver::~LightLocalSolver() {
}


bool LightLocalSolver::compute_supy_lb(Cell* x_cell,double uplo,double loup,Function* minus_goal = NULL) {
    perf_ctc_coef = 1;
//    local_sols.clear();
//    std::cout<<"nb sols: "<<nb_sols<<std::endl;
    DataMinMax *data_x;
    if (csp_actif )
        data_x = &(x_cell->get<DataMinMaxCsp>());
    else
        data_x = &(x_cell->get<DataMinMaxOpti>());
//    std::cout<<"      LghtLoc: data_x get"<<std::endl;

//    std::cout<<"      LghtLoc: number of global sols: "<<nbs<<std::endl;
    std::vector<Vector> candidates = find_max_candidates(uplo,x_cell);

//    std::cout<<"      LghtLoc: searching for local sol around "<<global_xy_sol<<std::endl;

    std::pair<std::vector<Matrix>,std::vector<Vector> > local_solutions =  get_local_solutions(candidates,loup,x_cell); // get local solutions as regressions of unique points



    if(!local_solutions.first.empty()) { // check if one point_solutions gave a lower bound greater than the loup, meaning that the global solution is not in x_box
        if(local_solutions.first.back().nb_cols() == 0)
            return true;
    }
    else if(local_solutions.second.empty()) // no local solution found
        return true;

//        std::cout<<"       LghtLoc: start sivia"<<std::endl;
//    if(regress)
//    {
    std::queue< std::pair<IntervalVector,Interval> > csp_lower_loup_boxes;
    if(nb_sivia_iter>0 ) // if iterations and at least one local solutions
    {
        csp_lower_loup_boxes = sivia_x_bsct( x_cell,local_solutions,loup);
        if(csp_lower_loup_boxes.empty()) {
            std::cout<<"    csp empty, return false "<<std::endl;
            return false;
        }
//                    std::cout<<"      LghtLoc: compute volume of list..."<<std::endl;
        double list_vol = list_volume(csp_lower_loup_boxes);
        std::cout<<"      LghtLoc: init x_cell box volume: "<<x_cell->box.volume()<<std::endl
                <<"            volume after sivia: "<<list_vol<<std::endl<<"              size of csp list: "<<csp_lower_loup_boxes.size()<<std::endl;
        IntervalVector ctc_x_box = list_hull_box(csp_lower_loup_boxes);
        std::cout<<"      LghtLoc: vol of hullbox: "<<ctc_x_box.volume()<<std::endl;
        x_cell->box = ctc_x_box;
        csp_queue = csp_lower_loup_boxes;
        perf_ctc_coef = list_vol/x_cell->box.volume()*csp_queue.size()/(2*nb_sivia_iter); // contraction volume gain times ratio number of elements upon max list elem
    }
    else {
        DataMinMax *data_x;
        if (csp_actif )
            data_x = &(x_cell->get<DataMinMaxCsp>());
        else
            data_x = &(x_cell->get<DataMinMaxOpti>());
        csp_lower_loup_boxes.push(std::pair<IntervalVector,Interval>(x_cell->box,data_x->fmax));
    }

    // run global optim over x at the y of global_xy_sol to upper the lower bound lb:  fy(x)> lb for all x in [x]
    if(nb_optim_iter>0)
    {
        std::cout<<"      LghtLoc: compute lb"<<std::endl;
        double global_lb_at_y = compute_lb(csp_lower_loup_boxes,local_solutions,x_cell,loup);
        std::cout<<"      LghtLoc: lower bound at y_sol after global optim: "<<global_lb_at_y<<std::endl;
        if (global_lb_at_y>loup) // E y such as f([x],y) > loup => prune [x]
            return false;
        else {
            data_x->fmax &= Interval(global_lb_at_y,POS_INFINITY);
        }
    }


    return true;

}

std::vector<Vector> LightLocalSolver::supy_local_sols(const Vector& global_xy_sol,Cell* x_cell){
//    std::cout<<"      LghtLoc: filling supy slos from global sol "<<global_xy_sol<<std::endl;
    std::vector<Vector> local_sols;
    for(int i=0;i<nb_sols;i++) {
        Vector xy_start_point = xy_stpt(global_xy_sol,x_cell);
//        std::cout<<"      LghtLoc: start point: "<<xy_start_point<<std::endl;
        IntervalVector search_box = search_box_over_y(xy_start_point);
//        std::cout<<"      LghtLoc: search box: "<<search_box<<std::endl;
        local_solver_over_y->set_box(search_box);
        Vector final_point(xy_start_point);
        local_solver_over_y->minimize(xy_start_point,final_point,1e-3); // maximizes
//        std::cout<<"      LghtLoc: final point: "<<final_point<<std::endl;
        local_sols.push_back(final_point);
    }
    return local_sols;

}

IntervalVector LightLocalSolver::search_box_over_y(const Vector& xy_start_point) {
    IntervalVector search_box(xy_start_point);
    for(int i = x_dim;i<xy_start_point.size();i++) { // x is point, y free in y_box
        search_box[i] = xy_sys.box[i];
    }
    return search_box;
}

IntervalVector LightLocalSolver::search_box_over_xy(Cell* x_cell) {
    IntervalVector search_box(x_dim+y_dim);
    for(int i = 0;i<x_dim;i++) { // x free in x_box, y free in y_box
        search_box[i] = x_cell->box[i];
    }
    for(int i=x_dim;i<x_dim+y_dim;i++) {
        search_box[i] = xy_sys.box[i];
    }
    return search_box;
}

IntervalVector LightLocalSolver::search_box_over_x(const Vector& xy_start_point,Cell* x_cell) {
    IntervalVector search_box(xy_start_point);
    for(int i = 0;i<x_dim;i++) { // x is point, y free in y_box
        search_box[i] = x_cell->box[i];
    }
    return search_box;
}

Vector LightLocalSolver::xy_stpt(const Vector& global_xy_sol,Cell* x_cell) { // set x as random in [x], y does not change from global_xy_sol
    Vector x_start_point = x_cell->box.random();
    Vector xy_stpt(global_xy_sol);
    for(int i=0;i<x_start_point.size();i++) {
        xy_stpt[i] = x_start_point[i];
    }
    return xy_stpt;
}


IntervalVector LightLocalSolver::hull_ysol_box(const Vector& global_xy_sol, std::vector<Vector> local_sols){
    IntervalVector hull(y_dim,Interval::ALL_REALS);
    if(local_sols.size()==0)
        return IntervalVector(y_dim,Interval::EMPTY_SET);
    for(int i=0;i<y_dim;i++) {
//        std::cout<<"          lightloc: local sol in list: "<<local_sols.at(0)<<std::endl;
//        std::cout<<"          lightloc: local sol in list at x_dim+i"<<x_dim+i<<"  : "<<local_sols.at(0)[x_dim+i]<<std::endl;
//        double min = global_xy_sol[x_dim+i];
//        double max = global_xy_sol[x_dim+i];
        double min = local_sols.at(0)[x_dim+i];
        double max = local_sols.at(0)[x_dim+i];
//        std::cout<<"          lightloc: min: "<<min<<"   max: "<<max<<std::endl;
        for(int j=1;j<local_sols.size();j++) {
//            std::cout<<"          lightloc: local sol in list: "<<local_sols.at(j)<<std::endl;
            min = min<local_sols.at(j)[x_dim+i]?min:local_sols.at(j)[x_dim+i];
            max = max>local_sols.at(j)[x_dim+i]?max:local_sols.at(j)[x_dim+i];
//            std::cout<<"          lightloc: min: "<<min<<"   max: "<<max<<std::endl;
        }
        hull[i] = Interval(min,max);
    }
    return hull;
}

Vector LightLocalSolver::sols_std_dev(const std::vector<Vector>& local_sols){
    Vector std(y_dim);
    Vector mean = sols_mean(local_sols);
    for(int i=0;i<y_dim;i++) {
        for(int j=1;j<local_sols.size();j++) {
            std[i] += (local_sols.at(j)[i+x_dim]-mean[i])*(local_sols.at(j)[i+x_dim]-mean[i]);
        }
//        std[i] = sqrt(std[i]/local_sols.size());
    }
    return std;
}

Vector LightLocalSolver::sols_mean(const std::vector<Vector>& local_sols) {
    Vector mean(y_dim);
    for(int j=0;j<y_dim;j++) {
        for(int i=0;i<local_sols.size();i++){
            mean[j] += local_sols.at(i)[j+x_dim];
        }
        mean[j] /= local_sols.size();
    }
    return mean;
}

std::vector<Vector> LightLocalSolver::reject_far_y_sol(const Vector& global_sol,std::vector<Vector> local_sols) {
    std::vector<Vector> keep_sol;
//    Vector mean = sols_mean(local_sols);
    IntervalVector admissible_box(y_dim);
    Vector rad = xy_sys.box.subvector(x_dim,x_dim+y_dim-1).rad();
    for(int i=0;i<y_dim;i++) {
        admissible_box[i] = Interval(global_sol[x_dim+i]-rad[i]*y_sol_radius,global_sol[x_dim+i]+rad[i]*y_sol_radius);
//        admissible_box[i] = Interval(mean[i]-rad[i]*y_sol_radius,mean[i]+rad[i]*y_sol_radius);
    }
//    std::cout<<"    admissible box: "<<admissible_box<<std::endl;
    while(!local_sols.empty()) {
        Vector sol = local_sols.back();
        local_sols.pop_back();
        if(admissible_box.contains(sol.subvector(x_dim,x_dim+y_dim-1))) {
//            std::cout<<"   keep sol : "<<sol.subvector(x_dim,x_dim+y_dim-1)<<std::endl;
            keep_sol.push_back(sol);
        }
    }

    return keep_sol;
}


std::queue<std::pair<IntervalVector,Interval> > LightLocalSolver::sivia_x_bsct(Cell* x_cell,const std::pair<std::vector<Matrix>,std::vector<Vector> >& local_solutions,double loup) {
    std::queue<std::pair<IntervalVector,Interval> > x_queue;
    std::queue<std::pair<IntervalVector,Interval> > csp_satisfy_list;
    DataMinMax *data_x;
    if (csp_actif )
        data_x = &(x_cell->get<DataMinMaxCsp>());
    else
        data_x = &(x_cell->get<DataMinMaxOpti>());
    x_queue.push(std::pair<IntervalVector,Interval>(x_cell->box,data_x->fmax));
    Bsc* bsc = new LargestFirst();
    int iter = 0;
    while (iter<nb_sivia_iter && !x_queue.empty()) {
        iter++;
//        std::cout<<"        LghtLoc:  SIVIA: iter nb "<<iter<<" sivia iter: "<<nb_sivia_iter<<std::endl;
//        std::cout<<"        LghtLoc:  SIVIA: loup: "<<loup<<std::endl;
         IntervalVector current_x_box = x_queue.front().first;
//        std::cout<<"        LghtLoc:  SIVIA: x_box:  "<<current_x_box<<std::endl;
        x_queue.pop();
        try
        {
//            std::cout<<"        LghtLoc:  SIVIA: try bisection "<<std::endl;
//            std::pair<IntervalVector,IntervalVector> cur_boxes = current_x_box.bisect(current_x_box.extr_diam_index(false),0.45);
            std::pair<IntervalVector,IntervalVector> cur_boxes = bsc->bisect(current_x_box);
//                    std::cout<<"        LghtLoc:  SIVIA: bisection done "<<std::endl;

            // handle first cell
            x_ctc.contract(cur_boxes.first);
            if(!cur_boxes.first.is_empty())
            {
//                std::cout<<"   sivia eval... "<<std::endl;
                Interval eval = eval_backward_max_solutions(local_solutions,cur_boxes.first, loup); // eval and contract box wrt < loup
//                std::cout<<"    eval: "<<eval<<std::endl;
                if (!eval.is_empty()){
//                    std::cout<<"   boxe after eval: "<<cur_boxes.first<<std::endl;
                    x_queue.push(std::pair<IntervalVector,Interval>(cur_boxes.first,eval));
//                IntervalVector y_box = y_param.submatrix(0,y_dim-1,0,x_dim-1)*cur_boxes.first+y_param.col(x_dim); // y = alpha*x + beta
//                IntervalVector current_xy_box = set_x_box_y_box(cur_boxes.first,y_box);
//                //        std::cout<<"        LghtLoc:  SIVIA: first box to eval: "<<current_xy_box<<std::endl;
//                Interval eval = xy_sys.goal->eval(current_xy_box);
////                std::cout<<"        LghtLoc:  SIVIA: eval box"<<current_xy_box<<"res: "<<eval<<std::endl;
//                if(eval.ub()<=loup) // is lower than best current solution, nothing to do anymore with this box
//                    csp_satisfy_list.push(std::pair<IntervalVector,Interval>(cur_boxes.first,eval));
//                else if(eval.lb()<loup) { // loup in [eval], put in list to bisect again
//                    xy_sys.goal->backward(Interval(NEG_INFINITY,loup),current_xy_box);
//                    if(!current_xy_box.is_empty()) {
//                        cur_boxes.first = current_xy_box.subvector(0,x_dim-1);
//                        x_queue.push(std::pair<IntervalVector,Interval>(cur_boxes.first,eval));
//                    }
                }
            }



            x_ctc.contract(cur_boxes.second);
            if(!cur_boxes.second.is_empty()){
                // handle second cell
//                std::cout<<"   sivia eval... "<<std::endl;
                Interval eval = eval_backward_max_solutions(local_solutions,cur_boxes.second, loup); // eval and contract box wrt < loup
//                std::cout<<"    eval: "<<eval<<std::endl;
                if (!eval.is_empty()){
//                    std::cout<<"   boxe after eval: "<<cur_boxes.first<<std::endl;
                    x_queue.push(std::pair<IntervalVector,Interval>(cur_boxes.second,eval));
//                IntervalVector y_box = y_param.submatrix(0,y_dim-1,0,x_dim-1)*cur_boxes.second+y_param.col(x_dim); // y = alpha*x + beta
//                IntervalVector current_xy_box = set_x_box_y_box(cur_boxes.second,y_box);
//                Interval eval = xy_sys.goal->eval(current_xy_box);
////                std::cout<<"        LghtLoc:  SIVIA: eval box"<<current_xy_box<<"res: "<<eval<<std::endl;
//                if(eval.ub()<=loup)
//                    csp_satisfy_list.push(std::pair<IntervalVector,Interval>(cur_boxes.second,eval));
//                else if(eval.lb()<loup) { // loup in [eval], put in list to bisect again
//                    xy_sys.goal->backward(Interval(NEG_INFINITY,loup),current_xy_box);
//                    if(!current_xy_box.is_empty()) {
//                        cur_boxes.second = current_xy_box.subvector(0,x_dim-1);
//                        x_queue.push(std::pair<IntervalVector,Interval>(cur_boxes.second,eval));
//                    }
                }
            }
        }
        catch (NoBisectableVariableException& ) {
//            std::cout<<"        LghtLoc:  SIVIA: not bisectable caught "<<std::endl;
            csp_satisfy_list.push(std::pair<IntervalVector,Interval>(current_x_box,xy_sys.goal->eval(current_x_box)));
        }

//        std::cout<<"        LghtLoc:  SIVIA: second box eval res: "<<eval<<std::endl;
    }
//    std::cout<<"        LghtLoc:  SIVIA: fill csp list "<<std::endl;
    while(!x_queue.empty()) {
        csp_satisfy_list.push(x_queue.front());
        x_queue.pop();
    }
    delete bsc;
//    std::cout<<"        LghtLoc:  SIVIA: done "<<std::endl;
    return csp_satisfy_list;

}

IntervalVector LightLocalSolver::list_hull_box(std::queue<std::pair<IntervalVector,Interval> > list) {
    if(list.empty()) {
        return IntervalVector(x_dim);
    }
    IntervalVector hull_box = list.front().first;
    list.pop();
    while(!list.empty()) {
        hull_box |= list.front().first;
        list.pop();
    }
    return hull_box;
}

double LightLocalSolver::list_volume(std::queue<std::pair<IntervalVector,Interval> > list) {
    double volume = 0;
    for(int i=0;i<list.size();i++) {
        volume += list.front().first.volume();
        list.push(list.front());
        list.pop();
    }
    return volume;
}

double LightLocalSolver::compute_lb(std::queue<std::pair<IntervalVector,Interval> > x_list,const std::pair<std::vector<Matrix>,std::vector<Vector> >& local_solutions,Cell* x_cell,double loup) {
    if(x_list.empty())
        return NEG_INFINITY;
    Bsc* bsc(new LargestFirst());
    Cell * subx_cell;
    for(int i=0;i<x_list.size();i++) {
        subx_cell = new Cell(x_list.front().first);
        subx_cell->add<OptimData>();
        OptimData  *data = &(subx_cell->get<OptimData>());
        data->pf = x_list.front().second;
        x_heap->push(subx_cell);
        x_list.pop();
    }
    bsc->add_backtrackable(*subx_cell);
    int iter = 0;
//    double min_prec = x_cell->box.max_diam()/2;
//    std::cout<<"min prec: "<<min_prec<<std::endl;
    double x_loup = loup;
    std::vector<Cell*> memory;
    OptimData  *data = &(x_heap->top()->get<OptimData>());
    double diam = x_loup-data->pf.lb();
    std::queue<double> uplo_mem;
    double deriv = POS_INFINITY;
//    while (iter<nb_optim_iter && !x_heap->empty()) {
//    while (diam>1 && !x_heap->empty()) {
    while (deriv > 1e-3 && !x_heap->empty()) {
        OptimData  *data = &(x_heap->top()->get<OptimData>());
        diam = x_loup-data->pf.lb();
        uplo_mem.push(data->pf.lb());

        if(uplo_mem.size()>5) {
            uplo_mem.pop();
        }
        if(iter>7) {
            std::pair<Vector,double> coefs = fit_line(uplo_mem);
            deriv = coefs.first[0]/data->pf.lb();
        }

        std::cout<<"uplo : "<<data->pf.lb()<<" loup: "<<x_loup<<" diam : "<<diam<<"     deriv: "<<deriv<<std::endl;
//        std::cout<<"    nb iter: "<<iter<<"  size of x_heap: "<<x_heap->size()<<std::endl;
        iter++;
        subx_cell = x_heap->pop();
        try
        {
//            std::cout<<"x_loup: "<<x_loup<<std::endl;
            std::pair<Cell*,Cell*> subcells_pair=bsc->bisect_cell(*subx_cell);
            delete subx_cell;


            subx_cell = subcells_pair.first;
            IntervalVector current_subx_box = subx_cell->box;
            x_ctc.contract(current_subx_box);
//            std::cout<<"     current box contracted "<<std::endl;
            if(current_subx_box.is_empty())
                delete subx_cell;
            else
            {
                Interval eval = eval_backward_max_solutions(local_solutions,current_subx_box, loup); // eval and contract box wrt < loup
//                IntervalVector y_box = y_param.submatrix(0,y_dim-1,0,x_dim-1)*current_subx_box+y_param.col(x_dim); // alpha*x + beta to get y box from x box
//                IntervalVector xy_box = set_x_box_y_box(current_subx_box,y_box);
//                xy_sys.goal->backward(Interval(NEG_INFINITY,x_loup),xy_box);
//                if(xy_box.is_empty()){
                if(eval.is_empty()) {
                    delete subx_cell;
//                    std::cout<<"      sub cell deleted"<<std::endl;
                }
                else
                {
                    OptimData  *data_subx = &(subx_cell->get<OptimData>());
//                    data_subx->pf = xy_sys.goal->eval(xy_box);
                    data_subx->pf = eval;
//                                std::cout<<" xy_box.mid : "<<xy_box.mid()<<std::endl;
                    IntervalVector mid_x(current_subx_box.mid());
                    x_ctc.contract(mid_x);
                    if(!mid_x.is_empty())
                    {
//                        Interval mid_eval = xy_sys.goal->eval(xy_box.mid());
                        Interval mid_eval = eval_backward_max_solutions(local_solutions,mid_x, loup);
                        if(!mid_eval.is_empty())
                            x_loup = mid_eval.ub()<x_loup?mid_eval.ub():x_loup;
                    }
//                                std::cout<<"      eval of box "<<xy_box<<" : "<<data_subx->pf<<std::endl;
//                    if(data_subx->pf.lb()>x_loup)
//                        delete subx_cell;
//                    //        else if (subx_cell->box.max_diam()<min_prec)
//                    //            memory.push_back(subx_cell);
//                    else
                         x_heap->push(subx_cell);
                }
            }

            subx_cell = subcells_pair.second;
            current_subx_box = subx_cell->box;
            x_ctc.contract(current_subx_box);
//            std::cout<<"     current 2nd  box contracted "<<std::endl;
            if(current_subx_box.is_empty()) {
//                std::cout<<"      sub cell deleted"<<std::endl;
                delete subx_cell;
            }
            else
            {
                Interval eval = eval_backward_max_solutions(local_solutions,current_subx_box, loup); // eval and contract box wrt < loup
//                IntervalVector y_box = y_param.submatrix(0,y_dim-1,0,x_dim-1)*current_subx_box+y_param.col(x_dim); // alpha*x + beta to get y box from x box
//                IntervalVector xy_box = set_x_box_y_box(current_subx_box,y_box);
//                xy_sys.goal->backward(Interval(NEG_INFINITY,x_loup),xy_box);
//                if(xy_box.is_empty())
                if(eval.is_empty())
                    delete subx_cell;
                else
                {
                    OptimData  *data_subx = &(subx_cell->get<OptimData>());
//                    data_subx->pf = xy_sys.goal->eval(xy_box);
                    data_subx->pf = eval;
                    //            std::cout<<" xy_box.mid : "<<xy_box.mid()<<std::endl;
                    IntervalVector mid_x(current_subx_box.mid());
                    x_ctc.contract(mid_x);
                    if(!mid_x.is_empty())
                    {
//                        Interval mid_eval = xy_sys.goal->eval(xy_box.mid());
                        Interval mid_eval = eval_backward_max_solutions(local_solutions,mid_x, loup);
                        if(!mid_eval.is_empty())
                            x_loup = mid_eval.ub()<x_loup?mid_eval.ub():x_loup;
                    }
//                                std::cout<<"      eval of box "<<xy_box<<" : "<<data_subx->pf<<std::endl;
                    if(data_subx->pf.lb()>x_loup)
                        delete subx_cell;
                    //        else if (subx_cell->box.max_diam()<min_prec)
                    //            memory.push_back(subx_cell);
                    else
                        x_heap->push(subx_cell);
                }
            }
        }
        catch (NoBisectableVariableException& ) {
            std::cout<<"    no bisect exept caught "<<std::endl;
            memory.push_back(subx_cell);
//            delete subx_cell;
        }

    }
//    std::cout<<"            LghtLox: fill x_heap"<<std::endl;
    while(!memory.empty()) {
        x_heap->push(memory.back());
        memory.pop_back();
    }
    double lb;
    if(x_heap->empty()) {  // no x in [x] is lower than the loup at y_sol, return something greater than the loup to discard x
        lb = loup+1;
    }
    else{
        lb = x_heap->top()->get<OptimData>().pf.lb();
        x_heap->flush();
    }
    std::cout<<"            LghtLox: optim process finished"<<std::endl;
    return lb;
}

IntervalVector LightLocalSolver::set_x_box_y_vect(const IntervalVector& x_box, const Vector& y_vect) {
    IntervalVector xy_box(x_dim+y_dim);
    for(int i=0;i<x_dim;i++)
        xy_box[i] = x_box[i];
    for(int i=0;i<y_dim;i++)
        xy_box[x_dim+i] = Interval(y_vect[i]);
    return xy_box;
}

IntervalVector LightLocalSolver::set_x_box_y_box(const IntervalVector& x_box, const IntervalVector& y_vect) {
    IntervalVector xy_box(x_dim+y_dim);
    for(int i=0;i<x_dim;i++)
        xy_box[i] = x_box[i];
    for(int i=0;i<y_dim;i++)
        xy_box[x_dim+i] = y_vect[i];
    return xy_box;
}

std::pair<Matrix,Vector> LightLocalSolver::affine_regression(const std::vector<Vector>& local_sols) {
    Matrix M(local_sols.size(),x_dim+1);
    Matrix y_param(y_dim,x_dim+1);
    Matrix y_sol(y_dim,local_sols.size());
    Vector error(y_dim);
    for(int i=0;i<local_sols.size();i++) {
        M.set_row(i,local_sols.at(i).subvector(0,x_dim));
        y_sol.set_col(i,local_sols.at(i).subvector(x_dim,x_dim+y_dim-1));
    }
//    std::cout<<"   matrix init ok "<<std::endl;std::cout<<"      LghtLoc: regression "<<std::endl;
    M.set_col(x_dim,Vector(local_sols.size(),1)); // constant coef
    Matrix invMtM(x_dim+1,x_dim+1);
    try {
        real_inverse(M.transpose()*M, invMtM);
    }
    catch (SingularMatrixException&) {
        return std::pair<Matrix,Vector>(Matrix(0,0),Vector(0));
    }
    Matrix S = invMtM*M.transpose();
    for(int i=0;i<y_dim;i++) {
        y_param.set_row(i,S*y_sol.row(i));
        Vector er =  y_sol.row(i)-M*y_param.row(i);
        error[i] =sqrt(er*er).ub();
    }
    return std::pair<Matrix,Vector>(y_param,error);

}



std::vector<Vector> LightLocalSolver::find_max_candidates(double uplo,Cell* x_cell) {
//    std::cout<<"find candidates"<<std::endl;

    DataMinMax *data_x;
    if (csp_actif )
        data_x = &(x_cell->get<DataMinMaxCsp>());
    else
        data_x = &(x_cell->get<DataMinMaxOpti>());

    std::vector<Vector> candidates;
    Vector global_xy_sol(x_dim+y_dim);
    int nbs = data_x->fsbl_pt_list.size();
    for(int i = 0;i<nbs;i++) {
        candidates.push_back(data_x->fsbl_pt_list.at(i).point);
    }
//    std::cout<<"    fsbl_pt_list candidates ok"<<std::endl;
    // local solver candidates
    Vector strt_pt = xy_sys.box.random();

    // search from top1
    Vector y_sol = data_x->y_heap->top1()->box.mid();
    for(int i=0;i<y_dim;i++)
        strt_pt[x_dim+i] = y_sol[i];
    IntervalVector search_box = search_box_over_y(strt_pt);
    local_solver_over_y->set_box(search_box);
//    strt_pt = xy_stpt(strt_pt,x_cell);
    local_solver_over_y->minimize(strt_pt,global_xy_sol,1e-3);
    if(xy_sys.goal->eval(global_xy_sol).ub()>uplo && check_twins(global_xy_sol,candidates)) // check if something to do a this max and if not already in candidates
        candidates.push_back(global_xy_sol);

//    std::cout<<" top1 candidate: "<<global_xy_sol<<std::endl;

    // search from top2
    y_sol = data_x->y_heap->top2()->box.mid();
//    strt_pt = xy_sys.box.random();
    for(int i=0;i<y_dim;i++)
        strt_pt[x_dim+i] = y_sol[i];
//    std::cout<<"  strt pt top 2 init"<<std::endl;
//    strt_pt = xy_stpt(strt_pt,x_cell);
//    search_box = search_box_over_y(strt_pt);
    local_solver_over_y->set_box(search_box);
    local_solver_over_y->minimize(strt_pt,global_xy_sol,1e-3);
//    std::cout<<"    minimization done "<<std::endl;
    if(xy_sys.goal->eval(global_xy_sol).ub()>uplo && check_twins(global_xy_sol,candidates)) // check if something to do a this max and if not already in candidates
        candidates.push_back(global_xy_sol);

//    std::cout<<" top2 candidate: "<<global_xy_sol<<std::endl;
    // search from random points
//    std::cout<<"search random candidates "<<std::endl;
    for(int i = 0;i<nb_rdm_candidates;i++)
    {
        y_sol = xy_sys.box.subvector(x_dim,x_dim+y_dim-1).random();
        for(int i=0;i<y_dim;i++)
            strt_pt[x_dim+i] = y_sol[i];
//        strt_pt = xy_sys.box.random();
//        search_box = search_box_over_y(strt_pt);
        local_solver_over_y->set_box(search_box);
        local_solver_over_y->minimize(strt_pt,global_xy_sol,1e-3);
        if(xy_sys.goal->eval(global_xy_sol).ub()>uplo && check_twins(global_xy_sol,candidates)) // check if something to do a this max and if not already in candidates
            candidates.push_back(global_xy_sol);
//        std::cout<<" random candidate: "<<global_xy_sol<<std::endl;
    }
    std::cout<<" candidates are: "<<std::endl;
    for(int i = 0;i<candidates.size();i++) {
        std::cout<<"   "<<candidates.at(i)<<std::endl;
    }
    return candidates;
}

bool LightLocalSolver::check_twins(const Vector& v,const std::vector<Vector>& list) {
    double eps = 1e-4;
    IntervalVector tol(y_dim,Interval(-eps,eps));
    for(int i =0;i<list.size();i++) {
        Vector diff((v-list.at(i)).subvector(x_dim,y_dim+x_dim-1));
        if(tol.contains(diff))  {
//            std::cout<< " diff: "<<diff<<" tol interval: "<<tol<<std::endl;
            return false;
        }
    }
    return true;
}

std::pair<std::vector<Matrix>,std::vector<Vector> > LightLocalSolver::get_local_solutions(std::vector<Vector> candidates,double loup,Cell* x_cell) {
    std::vector<Matrix> regression_parameters;
    std::vector<Vector> point_solutions;
    DataMinMax *data_x;
    if (csp_actif )
        data_x = &(x_cell->get<DataMinMaxCsp>());
    else
        data_x = &(x_cell->get<DataMinMaxOpti>());
//    point_solutions.push_back(data_x->y_heap->top1()->box.mid());
//    point_solutions.push_back(Vector(1,3));
    while(regression_parameters.size() + point_solutions.size()<15 && !candidates.empty())
    {
        //        std::cout<<" candidate: "<<candidate<<std::endl;

        Vector candidate = candidates.back();
        candidates.pop_back();
        std::vector<Vector> local_sols = supy_local_sols(candidate,x_cell);
        std::vector<Vector> kept_sols = reject_far_y_sol(candidate,local_sols);
        if(local_sols.empty())
            continue;

        //        for(int i=0;i<kept_sols.size();i++) {
        //            std::cout<<" kept sol: "<<kept_sols.at(i)<<std::endl;
        //        }
        IntervalVector hullb = hull_ysol_box(candidate,kept_sols);
        std::cout<<"      LghtLoc: hullbox of y sols "<<hullb<<std::endl;
        if(hullb.is_empty())
            continue;

        // evaluation at a given y, generally useless since done by mid_y eval in lightoptimminmax, yet usefull if lightoptimminmax not used

        if(hullb.max_diam()<min_acpt_diam) {// y of global sol seems to be solution at several x in [x] of sup fx(y)

            //        IntervalVector x_box_y_sol(search_box_over_x(global_xy_sol,x_cell));
            IntervalVector x_box_y_sol(set_x_box_y_vect(x_cell->box,hullb.mid()));
            std::cout<<"      LghtLoc: eval funtion at "<<x_box_y_sol<<std::endl;
            Interval ev = xy_sys.goal->eval(x_box_y_sol);
            std::cout<<"      LghtLoc: value "<<ev<<" loup: "<<loup<<std::endl;

            if (ev.lb()>loup) { // E y such as f([x],y) > loup => prune [x], return 0 size matrix to inform infeasible box
                regression_parameters.clear();
                regression_parameters.push_back(Matrix(0,0));
                return std::pair<std::vector<Matrix>,std::vector<Vector> >(regression_parameters,point_solutions);
            }
            //            else if(ev.ub()<uplo) // no chance to improve
            //                return true;
            //            else {

            //                data_x->fmax &= Interval(ev.lb(),POS_INFINITY);
            //                xy_sys.goal->backward(Interval(NEG_INFINITY,loup),x_box_y_sol);
            //                std::cout<<"      LghtLoc: contracted box wrt <loup: "<<x_box_y_sol<<std::endl;
            //                if(x_box_y_sol.is_empty())
            //                    return false;
            //                x_cell->box  = x_box_y_sol.subvector(0,x_dim-1);
            //            }

            point_solutions.push_back(hullb.mid()); // set y_sol in constant coeff of y_param
//            feasible_point* pt = new feasible_point(x_box_y_sol.mid(),ev);
//           feasible_point pt(x_box_y_sol.mid(),ev);
//            data_x->fsbl_pt_list.push_back(pt);
        }
        else {
            Matrix y_param(y_dim,x_dim+1);
            //        for(int i = 0;i<local_sols.size();i++) {
            //            std::cout<<"local sol after rejection: "<<local_sols.at(i)<<std::endl;
            //        }

            std::pair<Matrix,Vector> reg = affine_regression(kept_sols);

            if(reg.second.size()==0) {
                std::cout<<"      LghtLoc: regression impossible (non inversible matrix)"<<std::endl;
                continue;
            }

            y_param = reg.first;  // cancel regression over i-th dimension of y where regression was not needed :diam(y_hull(i))<min_acpt_diam
            for(int i = 0;i<y_dim;i++) {
                if(hullb[i].diam()<min_acpt_diam){
                    y_param.set_row(i,Vector(x_dim+1));
                    y_param[i][x_dim] = hullb[i].mid();
                    reg.second[i] = 0;

                }
            }
            std::cout<<"      LghtLoc:  regression error: "<<reg.second.max()<<std::endl;
            //        std::cout<<"      LghtLoc: alpha matrix: "<<y_param<<std::endl;
            //        std::cout<<"      LghtLoc: y sol estim: "<<std::endl;
            //        for(int j =0;j<local_sols.size();j++) {
            //            std::cout<<y_param.submatrix(0,y_dim-1,0,x_dim-1)*local_sols.at(j).subvector(0,x_dim-1)+y_param.col(x_dim)<<",";
            //        }
            IntervalVector reg_y_box = y_param.submatrix(0,y_dim-1,0,x_dim-1)*x_cell->box+y_param.col(x_dim);
            //        std::cout<<"reg y box: "<<reg_y_box<<std::endl<<" subset of "<<xy_sys.box.subvector(x_dim,x_dim+y_dim-1)<<" ? "<<reg_y_box.is_subset(xy_sys.box.subvector(x_dim,x_dim+y_dim-1))<<std::endl;
            //        std::cout<<"max of error: "<<reg.second.max()<<" acpt reg error: "<<reg_acpt_error<<std::endl;
            if(reg.second.max()<reg_acpt_error & reg_y_box.is_subset(xy_sys.box.subvector(x_dim,x_dim+y_dim-1))) // regression error acceptable and alpha*[x] + beta contained in [y].
            {
                regression_parameters.push_back(y_param);
            }

        }
    }
    return std::pair<std::vector<Matrix>,std::vector<Vector> >(regression_parameters,point_solutions);
}

Interval LightLocalSolver::eval_backward_max_solutions(const std::pair<std::vector<Matrix>,std::vector<Vector> >& local_solutions,IntervalVector x_box,double loup) {
    Interval eval(0);
//    std::cout<<" eval init at: "<<eval<<std::endl;
    for(int i = 0;i<local_solutions.first.size();i++) {
        Matrix y_param = local_solutions.first.at(i);
//        std::cout<<"    reg matrix: "<<y_param<<std::endl;
        IntervalVector y_box = y_param.submatrix(0,y_dim-1,0,x_dim-1)*x_box+y_param.col(x_dim);
//        std::cout<<"    y_box "<<y_box<<std::endl;
        IntervalVector current_xy_box = set_x_box_y_box(x_box,y_box);
        Interval ev = xy_sys.goal->eval(current_xy_box);
//        std::cout<<" eval at "<<current_xy_box<<"  : "<<ev<<std::endl;
        if(ev.lb()>loup) {
//            std::cout<<"    greater than loup : "<<loup<<std::endl;
            return Interval::EMPTY_SET;
        }
        else if(ev.ub()<loup) {
//            std::cout<<"     backward"<<std::endl;
            xy_sys.goal->backward(Interval(NEG_INFINITY,loup),current_xy_box);
//            std::cout<<"    ctc box: "<<current_xy_box<<std::endl;
            x_box &= current_xy_box.subvector(0,x_dim-1);
//            std::cout<<"    new x_box: "<<x_box<<std::endl;
            if(x_box.is_empty())
                return Interval::EMPTY_SET;
        }
//        std::cout<<"   eval: "<<eval<<"   current eval: "<<ev<<std::endl;
        eval = max(eval,ev);


    }
    for(int i = 0; i<local_solutions.second.size();i++) {
        Vector y_box = local_solutions.second.at(i);
//        std::cout<<"   ysolution: "<<y_box<<std::endl;
        IntervalVector current_xy_box = set_x_box_y_vect(x_box,y_box);
        Interval ev = xy_sys.goal->eval(current_xy_box);
//        std::cout<<" eval at "<<current_xy_box<<"  : "<<ev<<std::endl;
        if(ev.lb()>loup) {
//            std::cout<<"    greater than loup : "<<loup<<std::endl;
            return Interval::EMPTY_SET;
        }
        else {
//            std::cout<<"     backward"<<std::endl;
            xy_sys.goal->backward(Interval(NEG_INFINITY,loup),current_xy_box);
//            std::cout<<"    ctc box: "<<current_xy_box<<std::endl;
            x_box &= current_xy_box.subvector(0,x_dim-1);
//            std::cout<<"    new x_box: "<<x_box<<std::endl;
            if(x_box.is_empty())
                return Interval::EMPTY_SET;
        }
//        std::cout<<"   eval: "<<eval<<"   current eval: "<<ev<<std::endl;
        eval = max(eval,ev);
//        std::cout<<"    new eval: "<<eval<<std::endl;
    }
    return eval;
}


std::pair<Vector,double> fit_line(std::queue<double>& observ) {
    Matrix M(observ.size(),2);
    Vector obs(observ.size());
    for(int i = 0;i<M.nb_rows();i++) {
        double r[2] = {i,1};
        Vector row(2,r);
        M.set_row(i,row);
        obs[i] = observ.front();
        observ.push(observ.front());
        observ.pop();
    }
    Matrix invMtM(2,2);
    try {
        real_inverse(M.transpose()*M, invMtM);
    }
    catch (SingularMatrixException&) {
        return std::pair<Vector,double>(Vector(0),double(0));
    }
    Matrix S = invMtM*M.transpose();
    Vector y_param = S*obs;
    Vector er =  obs-M*y_param;
    double error =sqrt(er*er).ub();
    return std::pair<Vector,double>(y_param,error);

}




//========================================================
// *************** matrix inversion **********************
//========================================================

//Matrix pivotize(const Matrix& M) {
//    Matrix P(Matrix::eye(M.nb_rows()));
//    for(int j = 0;j<M.nb_rows()-1;j++){
//        int id = j;
//        double max = M[j][j];
//        for(int i = j+1;i<M.nb_rows();i++) {
//            double val =M[i][j]<0?-M[i][j]:M[i][j];
//            if(max<val) {
//                id = i;
//            }
//        if(j != id) {
//            Vector row = P.row(i);
//            P.set_row(i,P.row(id));
//            P.set_row(id,row);
//        }
//        }
//    }
//    return P;
//}





//void PLU_decompose(const Matrix& A,Matrix& P,Matrix& L,Matrix& U) {
//    P = Matrix(pivotize(A));
//    Matrix A2 = P*A;
//    for(int j = 0;j<A.nb_cols();j++) {
//        (L)[j][j] = 1;
//        for(int i=0;i<j+1;i++) {
//            double s1 = 0;
//            for(int k=0;k<i;k++) {
//                s1 += (U)[k][j]*(L)[i][k];
//            }
//            (U)[i][j] = A2[i][j]-s1;
//        }
//        for(int i=j;i<A.nb_rows();i++) {
//            double s2 = 0;
//            for(int k=0;k<j;k++) {
//                s2 += (U)[k][j]*(L)[i][k];
//            }
//            if((U)[j][j]==0) { // plu decomposition impossible
//                (P)[0][0] = -1;
//                return;
//            }
//            (L)[i][j] = (A2[i][j]-s2)/(U)[j][j];
//        }
//    }
//}

//Matrix inverse(const Matrix& A) {
//    Matrix inv_PA(A.nb_cols(),A.nb_cols());
//    Matrix P(A.nb_rows(),A.nb_rows());
//    Matrix L(A.nb_rows(),A.nb_rows());
//    Matrix U(A.nb_rows(),A.nb_rows());
//    PLU_decompose(A,P,L,U);
//    if(P[0][0]==-1) {
//        return Matrix(0,0);
//    }
//    for(int i=0;i<A.nb_cols();i++) {
//        Vector b(A.nb_cols());
//        b[i] = 1;
//        Vector x = luEvaluate(L,U,b);
//        inv_PA.set_col(i,x);
//    }
//    Matrix inv_A = inv_PA*(P);
//    return inv_A;
//}

//Vector luEvaluate(const Matrix& L, const Matrix& U, const Vector& b)
//{
//    // Ax = b -> LUx = b. Then y is defined to be Ux

//    Vector x(b.size());
//    Vector y(b.size());

//    // Forward solve Ly = b
//    for (int i = 0; i < b.size(); i++)
//    {
//        y[i] = b[i];
//        for (int j = 0; j < i; j++)
//        {
//            y[i] -= L[i][j] * y[j];
//        }
//        y[i] /= L[i][i];
//    }

//    // Backward solve Ux = y
//    for (int i = b.size() - 1; i >= 0; i--)
//    {
//        x[i] = y[i];
//        for (int j = i + 1; j < b.size(); j++)
//        {
//            x[i] -= U[i][j] * x[j];
//        }
//        x[i] /= U[i][i];
//    }

//    return x;
//}


}




