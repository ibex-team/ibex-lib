/*---------------------------------------------------------------------------------
 * 
 *  IBEX "C" interface for QUIMPER
 * --------------------------------
 *
 * Copyright (C) 2008 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef __SOLVER_IBEX__H
#define __SOLVER_IBEX__H

#ifdef __cplusplus
extern "C" {
#endif

double get_prec_epsilon();

struct selected_ctc {
  char* name;
  int selected; // 0=not selected, 1=selected
};


/* starts paving.
 * - ctc is the array of selected contractors. 
 * - size is the number of selected contractors. 
 * - rr_prec is the precision of the round-robin bisection.
 * - filename is the name of the file to export data (all the boxes
 *   of selected contractors). If NULL, data is not exported.
 * returns 0 in case of success, -1 if filename could not be created.
 *
*/
int run_solver(struct selected_ctc *ctc, int size, double rr_prec, char* filename);

#ifdef __cplusplus
}
#endif

#endif
