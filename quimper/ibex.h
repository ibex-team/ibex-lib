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

/* parses an input file in the QuIMPEr syntax. 
 *
 * All parameters except filename are "output" parameters.
 *
 * In case of success:
 *   Returns 1. *ctc_names is a dynamically allocated array of ctc_size contractor names.
 *              both ctc_names[i] and ctc_names[] have to be disallocated.
 *              *error_message and *error_token are set to NULL, *error_line to -1.
 * In case of failure:
 *   Returns 0.
 *   *ctc_names is set to NULL, *ctc_size to -1.
 *   *error_message points to the description of the error
 *   (and *error_message has to be desallocated).
 *   *error_token points to a copy of the token (and has to be desallocated as well) if the error is 
 *   a syntax error, NULL otherwise.
 *   *error_line is the line number if the error is a syntax error, -1 otherwise.
 */
int parse(const char* filename, char*** ctc_names, int* ctc_size, 
                                char** error_message, char** error_token, int* error_line);


/*
 * Returns:
 *   0 - success
 *  -1 - invalid file name
 *  -2 - invalid variable name or bad file format */
int import_data(const char* filename, const char* varname);

#ifdef __cplusplus
}
#endif

