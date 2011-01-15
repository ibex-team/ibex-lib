/*---------------------------------------------------------------------------------
 * 
 *  OpenGL interface for QUIMPER
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

#ifndef __VIEWER__H
#define __VIEWER__H

#ifdef __WIN32__
#define GLUT_DISABLE_ATEXIT_HACK
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct color {
  double red1, green1, blue1; // box interior
  double red2, green2, blue2; // box border
};

/*-------------------- BOXES ---------------------*/
struct box2D {
  double x1, x2, y1, y2;
  int ctc;           // subpaving (contractor) number
  struct color cl;
};


/* OpenGL initialization. This function must be 
 * called once, when the GUI loads. */
void glInit();

/* Scales the OpenGl viewport with extremal coordinates and
 * clears the screen (removes all the boxes currently displayed).
 * This function must be called before any box is displayed,
 * with the endpoints of the biggest (enclosing) box. */
void glScale(double _xmin, double _xmax, double _ymin, double _ymax);

/* Adds a new box in the OpenGL area. */
 void glAddBox(struct box2D box);

void glReshape(int width, int height);

//void glDrawBox(struct box2D box);

void glClearScreen();

void glDraw();

  //void glFlush ();

#ifdef __cplusplus
}
#endif

#endif
