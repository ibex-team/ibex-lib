/*---------------------------------------------------------------------------------
 * 
 *  OpenGL window for the QUIMPER GUI
 * ----------------------------------
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

#include <GL/glut.h>
#include "viewer.h"
#include <vector>
/*#include <string.h>
#include <stdio.h>
#include <stdlib.h>*/
#include <iostream>
//#include <vector>
//#include <plib/pw.h>
//#include <plib/fnt.h>

#include "viewer_gui.h"

/* ---- FRAME -----*/
static double Xmin,Xmax,Ymin,Ymax;  // enclosing box of all points to be plot (in the real frame)

static double xmin,xmax,ymin,ymax;  // the same enclosing box in the OpenGL viewport [-1,1]x[-1,1]

static double xmin_f,xmax_f, ymin_f,ymax_f; // coordinates of the horizontal & vertical axis (in the OpenGL viewport)

static double midx, midy, hx, hy;

static double xticks;               // position of x-ticks (OpenGL viewport)
static double yticks;               // position of y-ticks (OpenGL viewport)

/* --- Affichage texte ---- */
//static fntRenderer *text ;
//static fntTexFont *font ;
bool frame=false;
double shiftx = 0;           // horiz shift between the origin of the window & of the viewport (in pixels)
double shifty = 0;           // vert shift between the origin of the window & of the viewport (in pixels)
char xlabel[10] = "x[1]";
char ylabel[10] = "x[2]";
/*--------------------------*/

static std::vector<box2D> data;

extern "C" void glScale(double _xmin, double _xmax, double _ymin, double _ymax) {

  data.clear();

  /*--------- INIT --------------*/
  /* slight enlargement (10%) for displaying the axis */
  xmin   = Xmin = _xmin;
  xmax   = Xmax = _xmax;
  ymin   = Ymin = _ymin;
  ymax   = Ymax = _ymax;
  xmin_f = xmin - 0.05*(Xmax-Xmin);
  xmax_f = xmax + 0.05*(Xmax-Xmin);
  ymin_f = ymin - 0.05*(Ymax-Ymin);
  ymax_f = ymax + 0.05*(Ymax-Ymin);
  xticks = xmin_f - 0.04*(Xmax-Xmin);
  yticks = ymin_f - 0.04*(Ymax-Ymin);

  midx = (xmax_f+xmin_f)/2.0;
  midy = (ymax_f+ymin_f)/2.0;
  hx = 2.0/(1.1*(xmax_f-xmin_f)); // augmented by 10% to take axis+sticks into account
  hy = 2.0/(1.1*(ymax_f-ymin_f));

  xmin   = (xmin-midx)*hx;
  xmax   = (xmax-midx)*hx;
  ymin   = (ymin-midy)*hy;
  ymax   = (ymax-midy)*hy;
  xmin_f = (xmin_f-midx)*hx;
  xmax_f = (xmax_f-midx)*hx;
  ymin_f = (ymin_f-midy)*hy;
  ymax_f = (ymax_f-midy)*hy;
  xticks = (xticks-midx)*hx;
  yticks = (yticks-midy)*hy;

}

extern "C" void glInit() {
  glClearColor(0.0,0.0,0.0,0.0);
//  glClearColor(1.0,1.0,1.0,1.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(2.0);
}

extern "C" void glAddBox(struct box2D box) {
 /*--------- RESCALE ------------*/
 box.x1 -= midx;
 box.x2 -= midx;
 box.y1 -= midy;
 box.y2 -= midy;

 box.x1 *= hx;
 box.x2 *= hx;
 box.y1 *= hy;
 box.y2 *= hy;
 /*------------------------------*/

 data.push_back(box);
}

static void glDrawBox(const struct box2D& box) {
  glLoadIdentity();
 //--- without using "rescale": ----
 //   glScalef(hx,hy,1);
 //   glTranslatef(-midx,-midy,0.0);
 // -------------------------------

  glBegin(GL_POLYGON);
  glColor3f(box.cl.red1,box.cl.green1,box.cl.blue1);
  glVertex2f(box.x1,box.y1);
  glVertex2f(box.x1,box.y2);
  glVertex2f(box.x2,box.y2);
  glVertex2f(box.x2,box.y1);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glColor3f(box.cl.red2,box.cl.green2,box.cl.blue2);
  glVertex2f(box.x1,box.y1);
  glVertex2f(box.x1,box.y2);
  glVertex2f(box.x2,box.y2);
  glVertex2f(box.x2,box.y1);
  glEnd();

}

extern "C" void glClearScreen() {
  data.clear();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

extern "C" void glDraw() {
  /* draw in here */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  glPushMatrix();
//  glShadeModel(GL_FLAT);

  for (std::vector<struct box2D>::iterator it=data.begin(); it!=data.end(); it++) {
    //if (current_ctc==0 || it->ctc==current_ctc-1)
      glDrawBox(*it);
  }
    		
//  glPopMatrix ();
//  glFlush();
//  glutPostRedisplay();
//  glutSwapBuffers(); 
}

extern "C" void glReshape(int x, int y) {
// glLoadIdentity();
 if (x<y) {
    glViewport(0,(y-x)/2,x,x);
    shiftx=0;
    shifty=(y-x)/2;
  }
  else {
    glViewport((x-y)/2,0,y,y);
    shiftx=(x-y)/2;
    shifty=0;
  }

 gl_refresh();

//  glEnable (GL_BLEND); 
//  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}	



