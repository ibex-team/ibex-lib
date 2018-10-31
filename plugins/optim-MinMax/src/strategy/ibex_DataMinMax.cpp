//============================================================================
//                                  I B E X                                   
// File        : ibex_DataMinMax.cpp
// Author      : Dominique Monnet, Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 1, 2016
//============================================================================

#include "ibex_DataMinMax.h"

namespace ibex {


feasible_point::feasible_point(Vector box,Interval eval) : point(box), eval(eval) {
}

feasible_point::feasible_point(const feasible_point& pt) : point(pt.point), eval(pt.eval) {
}

feasible_point::~feasible_point() {
}

CellCostMaxPFub DataMinMax::y_heap_costf1;
CellCostPFlb DataMinMax::y_heap_costf2;

DataMinMax::DataMinMax() : fmax(), pu(0), y_heap(new DoubleHeap<Cell>(y_heap_costf1,false,y_heap_costf2,false)),nb_bisect(0) {

}

DataMinMax::DataMinMax(const DataMinMax& e) : fmax(e.fmax), pu(e.pu),y_heap(e.y_heap->deepcopy()),fsbl_pt_list(e.fsbl_pt_list),best_sol(NULL),nb_bisect(e.nb_bisect) {
}

std::pair<Backtrackable*,Backtrackable*> DataMinMax::down() {
	return std::pair<Backtrackable*,Backtrackable*>(new DataMinMax(*this),new DataMinMax(*this));
}

DataMinMax::~DataMinMax() {
//    std::cout<<"dataminmax destructor"<<std::endl;
    if (y_heap!=NULL) {
//        std::cout<<" flushing y_heap, which contains"<<y_heap->size()<< "elements"<<std::endl;
        y_heap->flush();
//        std::cout<<"y_heap flushed"<<std::endl;
        delete y_heap;
    }
//    std::cout<<"yheap flushed, delete best sol..."<<std::endl;
        if(best_sol != NULL)
            delete best_sol;
}

void DataMinMax::clear_fsbl_list() {
//    std::cout<<"delete fsbl list of size"<<fsbl_pt_list.size()<<std::endl;
//    feasible_point fp;
//    while(!fsbl_pt_list.empty()) {
//        fp = fsbl_pt_list.back();
////        std::cout<<"fp point: "<<fp->point<<std::endl;
//        delete fp;
//        fsbl_pt_list.pop_back();
//    }
    fsbl_pt_list.clear();
//    std::cout<<"clear done"<<std::endl;
}

void DataMinMax::clear_notin_point(const IntervalVector& x_box,bool strong_del) {
    int size = fsbl_pt_list.size();
    std::vector<feasible_point> save_vect;
//    std::cout<<"======================= "<<std::endl;
//    std::cout<<" check in box: "<<x_box<<std::endl;
    for(int i=0;i<size;i++) {
        feasible_point pt = fsbl_pt_list.back();
        fsbl_pt_list.pop_back();
//        std::cout<<" feas pt "<<pt->point<<std::endl;
        if((x_box.contains(pt.point.subvector(0,x_box.size()-1)))) {
//            if(strong_del)
//                delete pt;
            save_vect.push_back(pt);
//            std::cout<<"      is deleted"<<std::endl;
        }
//        else {
//            std::cout<<"      is NOT deleted"<<std::endl;

//        }
    }
    fsbl_pt_list = save_vect;
//    std::cout<<"***********************"<<std::endl;
}

/* Cost functions for DataMinMax class */
CellCostFmaxlb::CellCostFmaxlb(): CellCostFunc(false) { }

double CellCostFmaxlb::cost(const Cell& elem) const {
    return elem.get<DataMinMax>().fmax.lb();
}


void CellCostFmaxlb::add_backtrackable(Cell& root) {
        root.add<DataMinMax>();
}

CellCostmaxFmaxub::CellCostmaxFmaxub(): CellCostFunc(false) { }

double CellCostmaxFmaxub::cost(const Cell& elem) const {
    return -(elem.get<DataMinMax>().fmax.ub());
}


void CellCostmaxFmaxub::add_backtrackable(Cell& root) {
        root.add<DataMinMax>();
}

CellCostFmaxub::CellCostFmaxub(): CellCostFunc(false) { }

double CellCostFmaxub::cost(const Cell& elem) const {
    return (elem.get<DataMinMax>().fmax.ub());
}


void CellCostFmaxub::add_backtrackable(Cell& root) {
        root.add<DataMinMax>();
}

/* Cost functions for DataMinMaxOpti class */
CellCostFmaxlb_opt::CellCostFmaxlb_opt(): CellCostFunc(false) { }

double CellCostFmaxlb_opt::cost(const Cell& elem) const {
    return elem.get<DataMinMaxOpti>().fmax.lb();
}


void CellCostFmaxlb_opt::add_backtrackable(Cell& root) {
        root.add<DataMinMaxOpti>();
}

CellCostmaxFmaxub_opt::CellCostmaxFmaxub_opt(): CellCostFunc(false) { }

double CellCostmaxFmaxub_opt::cost(const Cell& elem) const {
    return -(elem.get<DataMinMaxOpti>().fmax.ub());
}


void CellCostmaxFmaxub_opt::add_backtrackable(Cell& root) {
        root.add<DataMinMaxOpti>();
}

CellCostFmaxub_opt::CellCostFmaxub_opt(): CellCostFunc(false) { }

double CellCostFmaxub_opt::cost(const Cell& elem) const {
    return (elem.get<DataMinMaxOpti>().fmax.ub());
}


void CellCostFmaxub_opt::add_backtrackable(Cell& root) {
        root.add<DataMinMaxOpti>();
}


/* Cost functions for DataMinMaxCsp class */
CellCostFmaxlb_csp::CellCostFmaxlb_csp(): CellCostFunc(false) { }

double CellCostFmaxlb_csp::cost(const Cell& elem) const {
    return elem.get<DataMinMaxCsp>().fmax.lb();
}


void CellCostFmaxlb_csp::add_backtrackable(Cell& root) {
        root.add<DataMinMaxCsp>();
}

CellCostmaxFmaxub_csp::CellCostmaxFmaxub_csp(): CellCostFunc(false) { }

double CellCostmaxFmaxub_csp::cost(const Cell& elem) const {
    return -(elem.get<DataMinMaxCsp>().fmax.ub());
}


void CellCostmaxFmaxub_csp::add_backtrackable(Cell& root) {
        root.add<DataMinMaxCsp>();
}

CellCostFmaxub_csp::CellCostFmaxub_csp(): CellCostFunc(false) { }

double CellCostFmaxub_csp::cost(const Cell& elem) const {
    return (elem.get<DataMinMaxCsp>().fmax.ub());
}


void CellCostFmaxub_csp::add_backtrackable(Cell& root) {
        root.add<DataMinMaxCsp>();
}

} // end namespace ibex
