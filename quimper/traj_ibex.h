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

#ifdef __cplusplus
extern "C" {
#endif


/* Applies a contractor */
void contract(const char* ctc_name);

/* Display the result 
 * x is the name of the trajectory variable (array of 2D/3D vectors)
 * Returns: 0 in case of success,
 *          -1 in case of a domain with infinite bound 
 *          -2 in case of invalid variable name 
 *          -3 in case of invalid dimensions
 *          [Warning only] -4 in case of 3D vector (not supported yet) 
 *          -5 nothing to plot (no file loaded) */
int plot(const char* x);

#ifdef __cplusplus
}
#endif

