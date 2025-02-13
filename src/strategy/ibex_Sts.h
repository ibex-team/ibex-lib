//============================================================================
//                                  I B E X
// File        : ibex_Sts.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Feb, 13, 2025
// Last Update : Feb, 13, 2025
//============================================================================

#ifndef __IBEX_STS_H__
#define __IBEX_STS_H__

#include <string>

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Abstract class for the statistics of a given operator.
 *
 * This class allows to store statistics of any operator (contractor, 
 * bisector, etc.).
 */
class Sts {
public:

	/**
	 * \brief Build the statistics.
	 * 
	 * \param op_name Name of the operator
	 */
	Sts(const std::string& op_name);
	
	/** 
	 * \brief Destructor.
	 */
	virtual ~Sts();
	
	/**
	 * \brief Add a new call to this operator in the stats.
	 */
	void add_call();
	
	/**
	 * \brief Report statistics.
	 */
	virtual std::string report() const = 0;	
	
protected:
	/* Name of the operator */
	std::string op_name;
	
	/* Number of times it has been called. */
	long int nb_calls;
};

inline Sts::~Sts() { }

inline Sts::Sts(const std::string& op_name) : op_name(op_name), nb_calls(0) { }

inline void Sts::add_call() { nb_calls++; }

} /* namespace ibex */

#endif /* __IBEX_STS_H__ */
