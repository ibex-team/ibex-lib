//============================================================================
//                                  I B E X                                   
// File        : ibex_DataMinMax.h
// Author      : Dominique Monnet, Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 1, 2016
//============================================================================

#ifndef __IBEX_DATA_MINMAX__
#define __IBEX_DATA_MINMAX__

#include "ibex_Backtrackable.h"
#include "ibex_Interval.h"
#include "ibex_DoubleHeap.h"
#include "ibex_CellCostFunc.h"
#include <list>
#include <stdio.h>

namespace ibex {




class feasible_point {
public:
    feasible_point(Vector point,Interval eval);
    feasible_point(const feasible_point& pt);
    ~feasible_point();


    Vector point;
    Interval eval;
};


/**
 * \ingroup strategy
 *
 * \brief Data required for the Optimizer
 */
class DataMinMax : public Backtrackable {

public:
	/**
	 * \brief Constructor for the root node (followed by a call to init_root).
	 */
        DataMinMax();


	/**
	 * \brief Delete *this.
	 */
	~DataMinMax();

        /**
     * @brief clear list of feasible by deleting contained pointer on feasible_point object, call only when x_cell discard because not solution. /!\ not when x_cell bissected
     */
    void clear_fsbl_list();

    /*
     * remove pointers from fsbl_point_list that point is not contained in x_box, object pointed at is also deleted if strong_del is true
     * */
    void clear_notin_point(const IntervalVector& x_box,bool strong_del);

	/**
	 * \brief Duplicate the structure into the left/right nodes
	 */
	std::pair<Backtrackable*,Backtrackable*> down();


    /**
     * Enclosure of maximum of the objective function
     */
    Interval fmax;

	/**
	 * \brief Casado criterion
	 *
	 * Constraint factor of the current box : between 0 infeasible and 1 for all constraints satisfied.
	 */
	double pu;

	/**
	 * y_heap inherited from father of box
	 */
    DoubleHeap<Cell>* y_heap;

    std::vector<feasible_point> fsbl_pt_list;


	/**
	 * Cost function of the heap to store the element of the light solver
	 */
	static CellCostMaxPFub y_heap_costf1;
	static CellCostPFlb y_heap_costf2;

protected:


	/**
	 * \brief Constructor by copy.
	 */
	explicit DataMinMax(const DataMinMax& e);

	/**
	 * \brief Create a copy
	 */
	Backtrackable* copy() const { return new DataMinMax(*this);};

};

/* Inherited classes from DataMinMax */

class DataMinMaxOpti : public DataMinMax {

protected:
    Backtrackable* copy() const { return new DataMinMaxOpti(*this);};

};


class DataMinMaxCsp : public DataMinMax {

protected:
    Backtrackable* copy() const { return new DataMinMaxCsp(*this);};


};


/* Cost function definition for DataMinMaxOpti class */


class CellCostFmaxlb : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

        CellCostFmaxlb();

        /**
         * \brief Return the cost associated to "data"
         */
        virtual double cost(const Cell& elem) const;

        /**
         * \brief Add backtrackable data required by this cost function
         *
         * This function is called for the root cell (before a
         * strategy is executed).
         *
         * Does nothing by default.
         */
     virtual void add_backtrackable(Cell& root);

};

class CellCostmaxFmaxub : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

        CellCostmaxFmaxub();
        /**
         * \brief Return the cost associated to "data"
         */
        virtual double cost(const Cell& elem) const;

        /**
         * \brief Add backtrackable data required by this cost function
         *
         * This function is called for the root cell (before a
         * strategy is executed).
         *
         * Does nothing by default.
         */
     virtual void add_backtrackable(Cell& root);

};

class CellCostFmaxub : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

        CellCostFmaxub();
        /**
         * \brief Return the cost associated to "data"
         */
        virtual double cost(const Cell& elem) const;

        /**
         * \brief Add backtrackable data required by this cost function
         *
         * This function is called for the root cell (before a
         * strategy is executed).
         *
         * Does nothing by default.
         */
     virtual void add_backtrackable(Cell& root);

};


/* Cost function definition for DataMinMaxOpti class */


class CellCostFmaxlb_opt : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

        CellCostFmaxlb_opt();

        /**
         * \brief Return the cost associated to "data"
         */
        virtual double cost(const Cell& elem) const;

        /**
         * \brief Add backtrackable data required by this cost function
         *
         * This function is called for the root cell (before a
         * strategy is executed).
         *
         * Does nothing by default.
         */
     virtual void add_backtrackable(Cell& root);

};

class CellCostmaxFmaxub_opt : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

        CellCostmaxFmaxub_opt();
        /**
         * \brief Return the cost associated to "data"
         */
        virtual double cost(const Cell& elem) const;

        /**
         * \brief Add backtrackable data required by this cost function
         *
         * This function is called for the root cell (before a
         * strategy is executed).
         *
         * Does nothing by default.
         */
     virtual void add_backtrackable(Cell& root);

};

class CellCostFmaxub_opt : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

        CellCostFmaxub_opt();
        /**
         * \brief Return the cost associated to "data"
         */
        virtual double cost(const Cell& elem) const;

        /**
         * \brief Add backtrackable data required by this cost function
         *
         * This function is called for the root cell (before a
         * strategy is executed).
         *
         * Does nothing by default.
         */
     virtual void add_backtrackable(Cell& root);

};

/* Cost function definition for DataMinMaxCsp class */


class CellCostFmaxlb_csp : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

        CellCostFmaxlb_csp();

        /**
         * \brief Return the cost associated to "data"
         */
        virtual double cost(const Cell& elem) const;

        /**
         * \brief Add backtrackable data required by this cost function
         *
         * This function is called for the root cell (before a
         * strategy is executed).
         *
         * Does nothing by default.
         */
     virtual void add_backtrackable(Cell& root);

};

class CellCostmaxFmaxub_csp : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

        CellCostmaxFmaxub_csp();
        /**
         * \brief Return the cost associated to "data"
         */
        virtual double cost(const Cell& elem) const;

        /**
         * \brief Add backtrackable data required by this cost function
         *
         * This function is called for the root cell (before a
         * strategy is executed).
         *
         * Does nothing by default.
         */
     virtual void add_backtrackable(Cell& root);

};

class CellCostFmaxub_csp : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

        CellCostFmaxub_csp();
        /**
         * \brief Return the cost associated to "data"
         */
        virtual double cost(const Cell& elem) const;

        /**
         * \brief Add backtrackable data required by this cost function
         *
         * This function is called for the root cell (before a
         * strategy is executed).
         *
         * Does nothing by default.
         */
     virtual void add_backtrackable(Cell& root);

};


} // end namespace ibex
#endif // __IBEX_DATA_MINMAX_H__
