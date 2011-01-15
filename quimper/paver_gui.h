/*---------------------------------------------------------------------------------
 * 
 *  QUIMPER graphical user interface (in experimental GTK)
 * -------------------------------------------------------
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

#ifndef __PAVER_GUI__H
#define __PAVER_GUI__H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char* name;
  double bg_red, bg_green, bg_blue;  
  double fg_red, fg_green, fg_blue;
} contractor;

/* gets the list of the selected 
 * contractors (names + colors).
 * The number of contractors is stored in the argument. */
contractor** get_ctc_list(int *number);

/* refresh (expose) the OpenGL window */
void gl_refresh();

/* check for paver interruption (the button has been pressed) */
int check_interrupt();

extern int step_mode;      // true if step-by-step mode is activated, false otherwise.

#ifdef __cplusplus
}
#endif

#endif
