/*---------------------------------------------------------------------------------
 * 
 * Implementation of projections
 * -----------------------------
 *
 * Copyright (C) 2007 Gilles Chabert
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

#include <stdlib.h>
#include "IbexInnerProjections.h"
#include "Functions.h"
#include "IbexToken.h"
#include "Functions.h"
#include "Utilities.h"
#include "IbexEvaluator.h"
#include <BIAS/BiasF.h>

namespace ibex {

bool innerproj_sqr(const INTERVAL& evl, INTERVAL& exp_evl) { //, bool& continuity, bool& monotonicity) {
  INTERVAL projpos=INTERVAL(Sup(Sqrt(INTERVAL(Inf(evl)))),Inf(Sqrt(INTERVAL(Sup(evl)))));
  INTERVAL projneg=INTERVAL(-Inf(Sqrt(INTERVAL(Sup(evl)))),-Sup(Sqrt(INTERVAL(Inf(evl)))));

  if (projpos.empty()) return false;
  
  INTERVAL tmp1, tmp2;      
  bool pos_proj=Intersection(tmp1, projpos, exp_evl);
  bool neg_proj=Intersection(tmp2, projneg, exp_evl);
    
  if (pos_proj && neg_proj)
    exp_evl = (rand()%2)? tmp1:tmp2;
  else if (pos_proj)  exp_evl = tmp1;
  else if (neg_proj) exp_evl = tmp2;   

  return (pos_proj | neg_proj);   
}


REAL projx(REAL z, REAL y, int op, bool round_up){
  (round_up)? BiasRoundUp():BiasRoundDown();
  switch(op){
    case ADD: return z-y; 
    case SUB: return z+y;
    case MUL: return (y==0)? BiasPosInf:z/y;
    case DIV: return z*y;
  }
  BiasRoundNear();
}

REAL projy(REAL z, REAL x, int op, bool round_up){
  (round_up)? BiasRoundUp():BiasRoundDown();
  switch(op){
    case ADD: return z-x; 
    case SUB: return x-z;
    case MUL: return (x==0)? BiasPosInf:z/x;
    case DIV: return (z==0)? BiasPosInf:x/z;
  }
  BiasRoundNear();
}

INTERVAL eval(INTERVAL x, INTERVAL y, int op){
  switch(op){
    case ADD: return x+y; 
    case SUB: return x-y;
    case MUL: return x*y;
    case DIV: return x/y;
  }
}




bool leq_inner_projection(INTERVAL& x, INTERVAL& y, REAL z_sup, int op, bool inc_var1, bool inc_var2){
   REAL xmin, xmax, x0,y0;

   INTERVAL xx=x;  

      xmax=projx(z_sup,(inc_var1 == inc_var2)? Inf(y):Sup(y),op,(inc_var1)?false:true);
      xmin=projx(z_sup,(inc_var1 == inc_var2)? Sup(y):Inf(y),op,(inc_var1)?false:true);


   if(xmax==BiasPosInf) xmax=Sup(x);
   if(xmin==BiasPosInf) xmin=Inf(x);

   if((inc_var1 && xmax < Inf(x)) || (!inc_var1 && xmin > Sup(x))) {
      return false;
   }else if((inc_var1 && xmin > Sup(x)) || (!inc_var1 && xmax < Inf(x))){
      x0=(inc_var1)? Sup(x):Inf(x);
   }else{
//           if(xmin>xmax) xmin=xmax;
      xx&=INTERVAL(xmin,xmax);
      x0= Inf(xx) + (REAL)rand()/(REAL)RAND_MAX*(Diam(xx));
      if(!xx.contains(x0)) x0= (x0 < Inf(xx))? Inf(xx):Sup(xx);
   }
   y0=projy(z_sup,x0,op,(inc_var2)?false:true);

   if(y0!=BiasPosInf){
      if(y0>Sup(y)) y0=Sup(y);
      else if(y0<Inf(y)) y0=Inf(y);
      y = (inc_var2)? INTERVAL(Inf(y),y0): INTERVAL(y0,Sup(y));
   }

   x = (inc_var1)? INTERVAL(Inf(x),x0): INTERVAL(x0,Sup(x));
   return true;
}


bool geq_inner_projection(INTERVAL& x, INTERVAL& y, REAL z_inf, int op, bool inc_var1, bool inc_var2){
   REAL xmin, xmax, x0,y0;
   INTERVAL xx=x;  

      xmax=projx(z_inf,(inc_var1 == inc_var2)? Inf(y):Sup(y),op,(inc_var1)?true:false); //true->ROUND_UP, false->ROUND_DOWN
      xmin=projx(z_inf,(inc_var1 == inc_var2)? Sup(y):Inf(y),op,(inc_var1)?true:false);


   if(xmax==BiasPosInf) xmax=Sup(x);
   if(xmin==BiasPosInf) xmin=Inf(x);

//    cout << xmin << "," << xmax << endl;
   if((inc_var1 && xmin > Sup(x)) || (!inc_var1 && xmax < Inf(x))) {
      return false;
   }else if((inc_var1 && xmax < Inf(x)) || (!inc_var1 && xmin > Sup(x))){
      x0=(inc_var1)? Inf(x):Sup(x);
   }else{
//           if(xmin>xmax) xmin=xmax;
      xx&=INTERVAL(xmin,xmax);
      x0= Inf(xx) + (REAL)rand()/(REAL)RAND_MAX*(Diam(xx));
      if(!xx.contains(x0)) x0= (x0 < Inf(xx))? Inf(xx):Sup(xx);
   }
   y0=projy(z_inf,x0,op,(inc_var2)?true:false);


   if(y0!=BiasPosInf){
      if(y0>Sup(y)) y0=Sup(y);
      else if(y0<Inf(y)) y0=Inf(y);

      y = (inc_var2)? INTERVAL(y0,Sup(y)): INTERVAL(Inf(y),y0);
   }

   x = (inc_var1)? INTERVAL(x0,Sup(x)):INTERVAL(Inf(x),x0);

   return true;
}



//Requeriments:
//the function op(x,y) is monotonic wrt x and y in [x] x [y]
//[xin]x[yin] is contained in [x]x[y]
//and [op]([xin],[yin]) is contained in [z]
void inflation(INTERVAL& x, INTERVAL& y, INTERVAL z, const INTERVAL &xin, const INTERVAL& yin, int op){
  bool inc_var1, inc_var2;
  switch(op){
    case ADD: inc_var1=true, inc_var2=true; break;
    case SUB: inc_var1=true, inc_var2=false; break;
    case MUL: inc_var1=(Inf(y)>=0), inc_var2=(Inf(x)>=0); break;
    case DIV: inc_var1=(Inf(y)>=0), inc_var2=(Sup(x)<=0); break;
  }  
  INTERVAL x1=(inc_var1)? INTERVAL(Sup(xin),Sup(x)): INTERVAL(Inf(x),Inf(xin));
  INTERVAL y1=(inc_var2)? INTERVAL(Sup(yin),Sup(y)): INTERVAL(Inf(y),Inf(yin));
  
  if(!leq_inner_projection(x1, y1, Sup(z), op, inc_var1, inc_var2)){
    x1=(inc_var1)? Sup(xin):Inf(xin); //generalized interval
    y1=(inc_var2)? Sup(yin):Inf(yin); //generalized interval
  }

  INTERVAL x2=(inc_var1)? INTERVAL(Inf(x),Inf(xin)): INTERVAL(Sup(xin),Sup(x));
  INTERVAL y2=(inc_var2)? INTERVAL(Inf(y),Inf(yin)): INTERVAL(Sup(yin),Sup(y));

  if(!geq_inner_projection(x2, y2, Sup(z), op, inc_var1, inc_var2)){
    x2=(inc_var1)? Inf(xin):Sup(xin); //generalized interval
    y2=(inc_var2)? Inf(yin):Sup(yin); //generalized interval
  }
  x=Hull(x1,x2);
  y=Hull(y1,y2);
}


bool inner_projection(INTERVAL& x, INTERVAL& y, INTERVAL z, int op){
  INTERVAL ev=eval(x,y,op);
  if(!(z&=ev)) return false;
  bool inc_var1,inc_var2;
  if(op==MUL || op==DIV){ //One box of the four possible boxes is chosen randomly
  if(Inf(z)>=0 || Sup(z)<=0){
    if(Inf(y)>=0)
      x=(Inf(z)>=0)? INTERVAL(max(0.0,Inf(x)),Sup(x)):INTERVAL(Inf(x),min(0.0,Sup(x))); //the only choice
    else if(Sup(y)<=0)
      x=(Sup(z)<=0)? INTERVAL(max(0.0,Inf(x)),Sup(x)):INTERVAL(Inf(x),min(0.0,Sup(x))); //the only choice
    else{
       if(Inf(x)<0 && Sup(x)>0) x=(rand()%2)? INTERVAL(0,Sup(x)):INTERVAL(Inf(x),0);
       if(Inf(x)>=0)
         y=(Inf(z)>=0)? INTERVAL(max(0.0,Inf(y)),Sup(y)):INTERVAL(Inf(y),min(0.0,Sup(y))); //the only choice
       else if(Sup(x)<=0)
         y=(Sup(z)<=0)? INTERVAL(max(0.0,Inf(y)),Sup(y)):INTERVAL(Inf(y),min(0.0,Sup(y))); //the only choice
    }
  }else if(Inf(z)<0 && Sup(z)>0){
    if(Sup(x)>0 && Inf(x)<0)
      x=(rand()%2)? INTERVAL(0,Sup(x)):INTERVAL(Inf(x),0) ;
    if(Sup(y)>0 && Inf(y)<0)      
      y=(rand()%2)? INTERVAL(0,Sup(y)):INTERVAL(Inf(y),0) ;
      z=((Sup(x)<=0 && Sup(y)<=0) || (Inf(x)>=0 && Inf(y)>=0))? INTERVAL(max(0.0,Inf(z)),Sup(z)):INTERVAL(Inf(z),min(0.0,Sup(z))); //the only choice
  }
}

  switch(op){
    case ADD: inc_var1=true, inc_var2=true; break;
    case SUB: inc_var1=true, inc_var2=false; break;
    case MUL: inc_var1=(Inf(y)>=0), inc_var2=(Inf(x)>=0); break;
    case DIV: inc_var1=(Inf(y)>=0), inc_var2=(Sup(x)<=0); break;
  } 

  if(Sup(z)<Sup(ev) && !leq_inner_projection(x,y,Sup(z),op,inc_var1,inc_var2))
     return false;
  
  if((Inf(z)>Inf(ev) && !geq_inner_projection(x,y,Inf(z),op,inc_var1,inc_var2)))
     return false;

//    if(!(eval(x,y,op) <= z)) {
//       cout << x << " op " << y << "=" << z << endl;
//       cout << "error!"  << endl;
//    }

  return true;
}

  


}

