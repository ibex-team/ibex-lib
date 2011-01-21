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


string op_str(int i){
  switch(i){
    case ADD: return " + ";
    case SUB: return " - ";
    case MUL: return " * ";
    case DIV: return " / ";
  }
}



  REAL max(const REAL& x, const REAL& y) {
  return x>y? x : y;
 }

  REAL min(const REAL& x, const REAL& y) {
  return x<y? x : y;
 }

  REAL UP(INTERVAL (*F)(const INTERVAL&), const REAL& _real_) {
  return Sup(F(INTERVAL(_real_,_real_)));
 }

  REAL LO(INTERVAL (*F)(const INTERVAL&), const REAL& _real_) {
  return Inf(F(INTERVAL(_real_,_real_)));
 }

  REAL UP_root(const REAL& _real_, int expon) {
  return Sup(Root(INTERVAL(_real_,_real_), expon));
 }

  REAL LO_root(const REAL& _real_, int expon) {
  return Inf(Root(INTERVAL(_real_,_real_), expon));
 }

INTERVAL eval(const INTERVAL& x, const INTERVAL& y, int op){
  switch(op){
    case ADD: return x+y; 
    case SUB: return x-y;
    case MUL: return x*y;
    case DIV: return x/y;
  }
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

void expand2_mult(const INTERVAL &xin, const INTERVAL& yin, INTERVAL& x, INTERVAL& y, INTERVAL z){
  INTERVAL xin1(INTERVAL::EMPTY);
  INTERVAL xin2(INTERVAL::EMPTY);
  INTERVAL yin1(INTERVAL::EMPTY);
  INTERVAL yin2(INTERVAL::EMPTY);
  INTERVAL z1(INTERVAL::EMPTY);
  INTERVAL z2(INTERVAL::EMPTY);
//   INTERVAL xini(x);
//   INTERVAL yini(y);
//   assert(xin.included(x));
//   assert(yin.included(y));
//   assert(eval(xin,yin,MUL).included(z));
//   cout << xin << " * " << yin << "=" << z<< endl;

      if(Sup(xin)>=0)
          Intersection(xin1,xin,INTERVAL(0,BiasPosInf));
      else
          Intersection(xin2,xin,INTERVAL(BiasNegInf,0));

      if(Sup(yin)>=0)
          Intersection(yin1,yin,INTERVAL(0,BiasPosInf));
      else
          Intersection(yin2,yin,INTERVAL(BiasNegInf,0));

      if(Sup(z)>0)
          Intersection(z1,z,INTERVAL(0,BiasPosInf));
      else if (Inf(z)<0)
          Intersection(z2,z,INTERVAL(BiasNegInf,0));

// cout << z1 << "," << z2 << endl;

      if(!xin1.empty() && !yin1.empty() && !z1.empty()){ //first quadrant
        INTERVAL xt; Intersection(xt,x,INTERVAL(0,BiasPosInf));
        INTERVAL yt; Intersection(yt,y,INTERVAL(0,BiasPosInf));
        expand2(xin1, yin1, xt, yt, z1, MUL);
        x=INTERVAL((Inf(x)<0 && !z2.empty())? Inf(x):Inf(xt),Sup(xt));
        y=INTERVAL((Inf(y)<0 && !z2.empty())? Inf(y):Inf(yt),Sup(yt));
      }

      if(!xin1.empty() && !yin2.empty() && !z2.empty()){ //fourth quadrant
        INTERVAL xt; Intersection(xt,x,INTERVAL(0,BiasPosInf));
        INTERVAL yt; Intersection(yt,y,INTERVAL(BiasNegInf,0));
        expand2(xin1, yin2, xt, yt, z2, MUL);
        x=INTERVAL((Inf(x)<0 && !z1.empty())? Inf(x):Inf(xt),Sup(xt));
        y=INTERVAL(Inf(yt),(Sup(y)>0 && !z1.empty())? Sup(y):Sup(yt));
      }

      if(!xin2.empty() && !yin1.empty() && !z2.empty()){ //second quadrant
        INTERVAL xt; Intersection(xt,x,INTERVAL(BiasNegInf,0));
        INTERVAL yt; Intersection(yt,y,INTERVAL(0,BiasPosInf));
        expand2(xin2, yin1, xt, yt, z2, MUL);
        x=INTERVAL(Inf(xt),(Sup(x)>0 && !z1.empty())? Sup(x):Sup(xt));
        y=INTERVAL((Inf(y)<0 && !z1.empty())? Inf(y):Inf(yt),Sup(yt));
      }

      if(!xin2.empty() && !yin2.empty() && !z1.empty()){ //third quadrant
        INTERVAL xt; Intersection(xt,x,INTERVAL(BiasNegInf,0));
        INTERVAL yt; Intersection(yt,y,INTERVAL(BiasNegInf,0));
        expand2(xin2, yin2, xt, yt, z1, MUL);
        x=INTERVAL(Inf(xt),(Sup(x)>0 && !z2.empty())? Sup(x):Sup(xt));
        y=INTERVAL(Inf(yt),(Sup(y)>0 && !z2.empty())? Sup(y):Sup(yt));
      }

  int op=MUL;
   x|=xin;
   y|=yin;
//   cout << x << op_str(op) << y << "=" << eval(x,y,op) << " in? " << z << endl;
//   assert(x.included(xini));
//   assert(y.included(yini));
//   assert(eval(x,y,op).included(z));
//   assert(xin.included(x));
//   assert(yin.included(y));

}

//Requeriments:
//the function op(x,y) is monotonic wrt x and y in [x] x [y]
//[xin]x[yin] is contained in [x]x[y]
//and [op]([xin],[yin]) is contained in [z]
void expand2(const INTERVAL &xin, const INTERVAL& yin, INTERVAL& x, INTERVAL& y, INTERVAL z, int op){
  bool inc_var1, inc_var2;
//   INTERVAL xini(x);
//   INTERVAL yini(y);
//     cout << xin << op_str(op) << yin << " = " << eval(xin,yin,op) << " in? " << z << endl;
//   assert(xin.included(x));
//   assert(yin.included(y));
//   assert(eval(xin,yin,op)<=z);

  switch(op){
    case ADD: inc_var1=true, inc_var2=true; break;
    case SUB: inc_var1=true, inc_var2=false; break;
    case MUL: inc_var1=(Inf(yin)>=0), inc_var2=(Inf(xin)>=0); break;
    case DIV: inc_var1=(Inf(yin)>=0), inc_var2=(Sup(xin)<=0); break;
  }  

 
  INTERVAL x1=(inc_var1)? INTERVAL(Sup(xin),Sup(x)): INTERVAL(Inf(x),Inf(xin));
  INTERVAL y1=(inc_var2)? INTERVAL(Sup(yin),Sup(y)): INTERVAL(Inf(y),Inf(yin));
  if(!leq_inner_projection(x1, y1, Sup(z), op, inc_var1, inc_var2)){
    x1=(inc_var1)? Sup(xin):Inf(xin); //generalized interval
    y1=(inc_var2)? Sup(yin):Inf(yin); //generalized interval
//   cout <<  x1 << op_str(op) << y1 << "=" << eval(x1,y1,op) << " < " << Sup(z) << endl;
  }

  INTERVAL x2=(inc_var1)? INTERVAL(Inf(x),Inf(xin)): INTERVAL(Sup(xin),Sup(x));
  INTERVAL y2=(inc_var2)? INTERVAL(Inf(y),Inf(yin)): INTERVAL(Sup(yin),Sup(y));

  if(!geq_inner_projection(x2, y2, Inf(z), op, inc_var1, inc_var2)){
    x2=(inc_var1)? Inf(xin):Sup(xin); //generalized interval
    y2=(inc_var2)? Inf(yin):Sup(yin); //generalized interval
//   cout <<  x2 << op_str(op) << y2 << "=" << eval(x2,y2,op) << " > " << Inf(z) << endl;
  }
  x=Hull(x1,x2);
  y=Hull(y1,y2);

   x|=xin;
   y|=yin;
//   cout << x << op_str(op) << y << "=" << eval(x,y,op) << " in? " << z << endl;
//   assert(x.included(xini));
//   assert(y.included(yini));
//   assert(eval(x,y,op).included(z));
//   assert(xin.included(x));
//   assert(yin.included(y));

}




bool innerproj_exp(const INTERVAL& evl, INTERVAL& exp_evl) {
    REAL inf=UP(Log,Inf(evl)); 
    REAL sup=LO(Log,Sup(evl));
    if(inf>sup) return false;

    INTERVAL result = INTERVAL(inf,sup);
    return exp_evl &= result;
}

bool innerproj_log(const INTERVAL& evl, INTERVAL& exp_evl) {
   return (exp_evl &= INTERVAL(UP(Exp,Inf(evl)),LO(Exp,Sup(evl)))); 
}

void expand_power(const INTERVAL &xin, const INTERVAL& p_evl, INTERVAL& p_exp_evl, int expon){
//    INTERVAL xini(p_exp_evl);
//    assert(xin.included(p_exp_evl));
//    assert(Power(xin,expon).included(p_evl));
   if(xin.contains(0))
        innerproj_power(p_evl, p_exp_evl, expon);
   else if(Inf(xin)>0){
       p_exp_evl &= INTERVAL(0,BiasPosInf); //should be true
       if(!innerproj_power(p_evl, p_exp_evl, expon))
         p_exp_evl=xin;
   }else{
       p_exp_evl &= INTERVAL(BiasNegInf,0); //should be true
       if(!innerproj_power(p_evl, p_exp_evl, expon))
         p_exp_evl=xin;
   }
//    cout << p_exp_evl << "^" << expon << "=" << Power(p_exp_evl,expon) << "=" << p_evl << endl;
   p_exp_evl|=xin;
//    assert(Power(p_exp_evl,expon).included(p_evl));
//    assert(p_exp_evl.included(xini));
}


void expand_sqr(const INTERVAL& xin, const INTERVAL& Y, INTERVAL& X) {
//   cout << "sqr x=" << xin << " X=" << X << " Y=" << Y << endl;
// INTERVAL xini(X);  
//   assert(xin.included(X));
//   assert(Sqr(xin).included(Y));

  REAL up=LO(Sqrt,Sup(Y));
  if (up<0) up=0; // may happen because of rounding

  if (Inf(Y)>0) {
    REAL lo=UP(Sqrt,Inf(Y));
    if (lo<up) { // may not hold because of rounding      
      if (Inf(xin)>0) {
	X &= INTERVAL(lo,up);
      } else { // we cannot have 0 in x since sqr(x) must be included in Y
	//if (Sup(x)>0) throw NonRecoverableException("Bug expand_sqr");
	X &= INTERVAL(-up,-lo);
      }
      // it may happen at this point that x is not anymore included inside X
      // (but still overlapping) because Sqrt can be more pessimistic than Sqr
      X |= xin; 
    }
    else
      X = xin;
  } else {    
    X &= INTERVAL(-up,up);
    X |= xin; // see comment above
  }

  // note that Sqr(X) might not be included anymore in Y
  // if Sqr was more pessimistic than Sqrt 
//   assert(Sqr(X).included(Y));
//  assert(X<=xini);
//  assert(xin <=X);
  //cout << "new X=" << X << endl;
}





bool innerproj_power(const INTERVAL& p_evl, INTERVAL& p_exp_evl, int expon) {
  
    REAL inf=UP_root(Inf(p_evl), expon); 
    REAL sup=LO_root(Sup(p_evl), expon);
    if(inf>sup) return false;
    INTERVAL proj=INTERVAL(inf,sup);

  if (expon % 2 ==0) {

    INTERVAL tmp1, tmp2;      
    bool pos_proj=Intersection(tmp1, proj, p_exp_evl);
    bool neg_proj=Intersection(tmp2, -proj, p_exp_evl);
    
    if (pos_proj && neg_proj) {
//       monotonicity = false;
//       if (Inf(p_evl)>0) continuity = false;
      p_exp_evl = (random()%2)? tmp1:tmp2;
    } 
    else if (pos_proj) p_exp_evl = tmp1;
    else if (neg_proj) p_exp_evl = tmp2;
    
    return (pos_proj | neg_proj);
  
  } else {

    return p_exp_evl &= proj;
    
  }
}


bool innerproj_sqr(const INTERVAL& evl, INTERVAL& exp_evl) { //, bool& continuity, bool& monotonicity) {
  INTERVAL projpos=INTERVAL(UP(Sqrt,Inf(evl)),LO(Sqrt,Sup(evl)));
  INTERVAL projneg=INTERVAL(-LO(Sqrt,Inf(evl)),-UP(Sqrt,Sup(evl)));

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






bool innerproj_sqrt(const INTERVAL& evl, INTERVAL& exp_evl) {
  INTERVAL proj=(Inf(evl)>0? UP(Sqr,Inf(evl)) : 0, LO(Sqr,Sup(evl)));
  exp_evl &= proj;
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

}
