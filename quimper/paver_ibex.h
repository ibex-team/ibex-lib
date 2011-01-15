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

#ifndef __PAVER_IBEX__H
#define __PAVER_IBEX__H

#ifdef __cplusplus
extern "C" {
#endif

#include "viewer.h"

char* get_first_var_name();

char* get_second_var_name();

double get_prec_epsilon();

struct selected_ctc {
  char* name;
  struct color cl;
};

/* starts paving.
 * - selected_ctc is the array of selected contractors. 
 * - size is the number of selected contractors. 
 * - x and y are the full names of the variables (i.e., the projected components)
 * - rr_prec is the precision of the round-robin bisection.
 *
 * Returns: 0 in case of success,
 *          -1 in case of a domain with infinite bound
 *          1 (or 2) if the name of the first (or second)
 *          variable is invalid. */
int run_paver(struct selected_ctc *ctc, int size, char* x, char* y, double rr_prec);

#ifdef __cplusplus
}
#endif

#endif
