/* ============================================================================
 * I B E X - ibex_SIPManifold.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_SIPMANIFOLD_H__
#define __SIP_IBEX_SIPMANIFOLD_H__

#include "ibex_SIPSolver.h"
#include "ibex_SIPSolverOutputBox.h"

#include <fstream>
#include <vector>

namespace ibex {
/**
 * \ingroup strategy
 *
 * \brief Representation of a manifold with boxes.
 *
 * Contrary to a paving, boxes may overlap in a "manifold"
 * (more than just on their boundaries).
 */
class SIPManifold {
public:

	SIPManifold(int n, int m, int nb_ineq);

	/**
	 * Delete this.
	 */
	virtual ~SIPManifold();

	/**
	 * \brief Load a manifold from a file.
	 */
	void load(const char* filename);

	/**
	 * \brief Write the manifold into a file.
	 */
	void write(const char* filename) const;

	/**
	 * \brief Write the manifold into a file (in text format)
	 *
	 * See #format().
	 */
	void write_txt(const char* filename) const;

	/**
	 * \brief Clear all.
	 */
	void clear();

	/**
	 * \brief Total number of boxes
	 *
	 * size = #inner + #boundary + #unknown + #pending
	 */
	int size() const;

	/**
	 * \brief Return the format of the report.
	 */
	static std::string format();

	//Status status() const;

	/**
	 * \brief Number of variables
	 */
	unsigned int n;

	/**
	 * \brief Number of equalities.
	 */
	unsigned int m;

	/**
	 * \brief Number of inequalities.
	 */
	unsigned int nb_ineq;

	/*
	 * \brief Return status of the last solving.
	 */
	SIPSolver::Status status;

	/*
	 * \brief Inner boxes
	 */
	std::vector<SIPSolverOutputBox> inner;

	/*
	 * \brief Boundary boxes
	 */
	std::vector<SIPSolverOutputBox> boundary;

	/*
	 * \brief Boundary boxes
	 */
	std::vector<SIPSolverOutputBox> unknown;

	/*
	 * \brief Pending boxes
	 */
	std::vector<SIPSolverOutputBox> pending;

	/*
	 * \brief CPU running time used to obtain this manifold.
	 */
	double time;

	/**
	 * \brief Number of cells used to obtain this manifold.
	 */
	unsigned int nb_cells;

	/**
	 * \brief Manifold file format version.
	 */
	static const int FORMAT_VERSION;

protected:

	static const int  SIGNATURE_LENGTH;
	static const char* SIGNATURE;

	unsigned int read_int(std::ifstream& f);
	double read_double(std::ifstream& f);
	void read_signature(std::ifstream& f);
	SIPSolverOutputBox read_output_box(std::ifstream& f);

	void write_int(std::ofstream& f, uint32_t x) const;
	void write_double(std::ofstream& f, double x) const;
	void write_signature(std::ofstream& f) const;
	void write_output_box(std::ofstream& f, const SIPSolverOutputBox& sol) const;
	void write_output_box_txt(std::ofstream& file, const SIPSolverOutputBox& sol) const;
};

std::ostream& operator<<(std::ostream& os, const SIPManifold& manif);

/*============================================ inline implementation ============================================ */

inline const SIPManifold& SIPSolver::get_manifold() const { return *manif; }

inline double SIPSolver::get_time() const { return time; }

inline double SIPSolver::get_nb_cells() const { return nb_cells; }

inline int SIPManifold::size() const {
	return inner.size() + boundary.size() + unknown.size() + pending.size();
}

} // end namespace ibex

#endif // __SIP_IBEX_SIPMANIFOLD_H__
