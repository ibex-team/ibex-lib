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

/****
Divide de box in four monotone boxes (if [x] and [y] contains 0). For each subbox an innerbox is found.
Then, randomly one of them is returned.
*****/
bool inner_projection_nonmonotone(INTERVAL& x, INTERVAL& y, INTERVAL z, int op){
        int nb_boxes=0;
        INTERVAL initx=x,inity=y;
        INTERVAL_VECTOR** boxes=new INTERVAL_VECTOR*[4];
        for(int i=0; i<4; i++)
           boxes[i]=NULL;

	if(Inf(x)<0){
	  if(Inf(y)<0 && Sup(z)>=0){
	    x=INTERVAL(Inf(x),(Sup(x)<0)? Sup(x):0);
	    y=INTERVAL(Inf(y),(Sup(y)<0)? Sup(y):0);
            if(inner_projection(x,y,z, op)){
              boxes[0]=new INTERVAL_VECTOR(2);
              (*boxes[0])(1)=x;
              (*boxes[0])(2)=y;
              nb_boxes++;
              x=initx;
              x=inity;
            }
	  }
	  if(Sup(y)>0 && Inf(z)<=0){
	    x=INTERVAL(Inf(x),(Sup(x)<0)? Sup(x):0);
	    y=INTERVAL((Inf(y)>0)? Inf(y):0,Sup(y));
            if(inner_projection(x,y,z, op)){
              boxes[1]=new INTERVAL_VECTOR(2);
              (*boxes[1])(1)=x;
              (*boxes[1])(2)=y;
              nb_boxes++;
              x=initx;
              x=inity;
            }
	  }
	}
        if(Sup(x)>0){
	  if(Inf(y)<0 && Inf(z)<=0){
	    x=INTERVAL((Inf(x)>0)? Inf(x):0,Sup(x));
	    y=INTERVAL(Inf(y),(Sup(y)<0)? Sup(y):0);
            if(inner_projection(x,y,z, op)){
              boxes[2]=new INTERVAL_VECTOR(2);
              (*boxes[2])(1)=x;
              (*boxes[2])(2)=y;
              nb_boxes++;
              x=initx;
              x=inity;
            }
	  }
          if(Sup(y)>0 && Sup(z)>=0){
	    x=INTERVAL((Inf(x)>0)? Inf(x):0,Sup(x));
	    y=INTERVAL((Inf(y)>0)? Inf(y):0,Sup(y));
            if(inner_projection(x,y,z, op)){
              boxes[3]=new INTERVAL_VECTOR(2);
              (*boxes[3])(1)=x;
              (*boxes[3])(2)=y;
              nb_boxes++;
            }
	  }
	}
        if(nb_boxes==0) return false;
// 	cout << nb_boxes << endl;
        int k=rand()%nb_boxes;
        int i=0, j=0;
        while(i<=k){
           if(boxes[j]){
             if(i==k){ 
                x=(*boxes[j])(1);
                y=(*boxes[j])(2);
                break; 
             }
             i++;
           }
           j++;
        }

        for(int i=0; i<4; i++){
           if(boxes[i])
             delete boxes[i];
        }
        delete[] boxes;
        return true;	  
}



bool inner_projection(INTERVAL& x, INTERVAL& y, INTERVAL z, int op){
        INTERVAL evout;
	INTERVAL t=z;
	switch (op) { 
	  case ADD :
	       evout = x+y;
	      break;
	  case SUB :
	      evout = x-y;
	      break;
	  case MUL : 
	       evout = x*y;
	      break;
	  case DIV :
	       evout=x/y;
	      break;
	}  
        t&=evout;

        if(t.empty()) return false;
	
	//we find a point (x0,y0) such that x+y<=sup(z)
	REAL x0_inf, x0_sup;
	REAL x0,y0;
	INTERVAL xx=x;
	if (Sup(z)<Sup(evout)) //otherwise all the box [x]x[y] satisfies x+y<=sup(z)
	switch (op) {
	  case ADD : 
		     BiasRoundDown(); x0_sup = Sup(z)-Inf(y); 
                      x0_inf = Sup(z)-Sup(y); //x0_inf + Sup(y) <= Sup(z)
		     BiasRoundNear();

                     if(x0_sup < Inf(x)) {
                       return false;
		     }else if(x0_inf > Sup(x)){
                       x0=Sup(x);
                     }else{
                          if(x0_inf>x0_sup) x0_inf=x0_sup;
			  xx&=INTERVAL(x0_inf,x0_sup);
		          x0= Inf(xx) + (REAL)rand()/(REAL)RAND_MAX*(Diam(xx));
	                  if(!xx.contains(x0)) x0=Mid(xx);
                     }
		     BiasRoundDown(); y0= Sup(z)-x0; // x0+y0 <= Sup(z)
		     BiasRoundNear();
		     
		     if(y0>Sup(y)) y0=Sup(y);
		     else if(y0<Inf(y)) y0=Inf(y);
                     

		     x = INTERVAL(Inf(x),x0);
		     y = INTERVAL(Inf(y),y0);
	             break;
		     
	  case SUB :  
		     BiasRoundDown(); x0_sup = Sup(z)+Sup(y); 
                                      x0_inf = Sup(z)+Inf(y);  
		     BiasRoundNear();
                     
                     if(x0_sup < Inf(x)){
                       return false;
		     }else if(x0_inf > Sup(x)){
                       x0=Sup(x);
                     }else{
                          if(x0_inf>x0_sup) x0_inf=x0_sup;  //this should never happen, but happens!
			  xx&=INTERVAL(x0_inf,x0_sup);
		          x0= Inf(xx) + (REAL)rand()/(REAL)RAND_MAX*(Diam(xx));
	                  if(!xx.contains(x0)) x0=Mid(xx);
                     }
		     BiasRoundUp(); y0 = x0-Sup(z); // x0-y0 <= Sup(z)
		     BiasRoundNear();


		     if(y0<Inf(y)) y0=Inf(y);
		     else if(y0>Sup(y)) y0=Sup(y);
                     

		     x = INTERVAL(Inf(x),x0);
		     y = INTERVAL(y0,Sup(y));
	             break;
	  case MUL :
                     (Inf(y)>=0)? BiasRoundDown():BiasRoundUp();
	    	     if((Inf(x)>=0  && Inf(y)>=0) || (Sup(x)<=0  && Sup(y)<=0)){ //y decroissante
                       x0_inf = (Sup(y)==0)? Inf(x):Sup(z)/Sup(y);
		       x0_sup = (Inf(y)==0)? Sup(x):Sup(z)/Inf(y);
		     }else{// if(Sup(z)<=0){
		       x0_inf =  (Inf(y)==0)? Inf(x):Sup(z)/Inf(y);
		       x0_sup = (Sup(y)==0)? Sup(x):Sup(z)/Sup(y);	       
		     }
		     BiasRoundNear();
                    
                     if((Inf(y)>=0 && x0_sup < Inf(x)) || (Sup(y)<=0 && x0_inf > Sup(x))) {
                       return false;
		     }else if(Inf(y)>=0 && x0_inf > Sup(x)){
                       x0=Sup(x);
                     }else if(Sup(y)<=0 && x0_sup < Inf(x)){
                       x0=Inf(x);
                     }else{
		        if(!(xx&=INTERVAL(x0_inf,x0_sup))) return false;
		        x0= Inf(xx) + (REAL)rand()/(REAL)RAND_MAX*(Diam(xx));
	                if(!xx.contains(x0)) x0=Mid(xx);
                     }

		    if(x0!=0){
		     if(Inf(x)>=0){ //y croissante
		       BiasRoundDown(); y0= Sup(z)/x0; // x0*y0 <= Sup(z)
		       if(y0<Inf(y)) y0=Inf(y);
		       if(y0>Sup(y)) y0=Sup(y);
		       y = INTERVAL(Inf(y),y0); 
		     }else{ // if(Sup(x)<=0){
		       BiasRoundUp(); y0= Sup(z)/x0; // x0*y0 <= Sup(z)	       
		       if(y0>Sup(y)) y0=Sup(y);
		       if(y0<Inf(y)) y0=Inf(y);
		       y = INTERVAL(y0, Sup(y)); 
		     }
                     BiasRoundNear();	
		    }

		     x = (Inf(y)>=0)? INTERVAL(Inf(x),x0):INTERVAL(x0,Sup(x));
		     break;
	  case DIV:  (Inf(y)>=0)? BiasRoundDown():BiasRoundUp();
	    	     if(Inf(z)>=0){ //y croissante
		       x0_inf = Sup(z)*Inf(y);
		       x0_sup = Sup(z)*Sup(y);
		     }else{// if(Sup(z)<=0){
		       x0_inf = Sup(z)*Sup(y);
		       x0_sup = Sup(z)*Inf(y);	       
		     }	    
		     BiasRoundNear();

                     if((Inf(y)>=0 && x0_sup < Inf(x)) || (Sup(y)<=0 && x0_inf > Sup(x))) {
                       return false;
		     }else if(Inf(y)>=0 && x0_inf > Sup(x)){
                       x0=Sup(x);
                     }else if(Sup(y)<=0 && x0_sup < Inf(x)){
                       x0=Inf(x);
                     }else{
		        if(!(xx&=INTERVAL(x0_inf,x0_sup))) return false;
		        x0= Inf(xx) + (REAL)rand()/(REAL)RAND_MAX*(Diam(xx));
	                if(!xx.contains(x0)) x0=Mid(xx);
                     }
		    
		     if(Inf(x)>=0){ //y decroissante
		       BiasRoundUp(); y0= x0/Sup(z); // x0/y0 <= Sup(z)
		       if(y0>Sup(y)) y0=Sup(y);
		       if(y0<Inf(y)) y0=Inf(y);
		       y = INTERVAL(y0,Sup(y)); 
		     }else{ // if(Sup(x)<=0){
		       BiasRoundDown(); y0= x0/Sup(z); // x0/y0 <= Sup(z)
		       if(y0<Inf(y)) y0=Inf(y);
		       if(y0>Sup(y)) y0=Sup(y);
		       y = INTERVAL(Inf(y), y0); 
		     }
		     BiasRoundNear();

		     x = (Inf(y)>=0)? INTERVAL(Inf(x),x0):INTERVAL(x0,Sup(x));
		     break;
	}

	xx=x;
	if (Inf(z)>Inf(evout)) //otherwise all the box [x]x[y] satisfies x+y>=inf(z)
	switch (op) {
	  case ADD : 
		     BiasRoundUp(); x0_sup = Inf(z)-Inf(y); 
                                    x0_inf = Inf(z)-Sup(y); //x0_inf + Sup(y) >= Inf(z)
		     BiasRoundNear();

                     if(x0_inf > Sup(x)) 
                       return false;
		     else if(x0_sup < Inf(x))
                       x0=Inf(x);
                     else{
			  xx&=INTERVAL(x0_inf,x0_sup);

		          x0= Inf(xx) + (REAL)rand()/(REAL)RAND_MAX*(Diam(xx));
	                  if(!xx.contains(x0)) x0=Mid(xx);
                     }
		     BiasRoundUp(); y0= Inf(z)-x0; // x0+y0 >= Inf(z)
		     BiasRoundNear();
		     
		     if(y0<Inf(y)) y0=Inf(y);
		     else if(y0>Sup(y)) y0=Sup(y);

		     x = INTERVAL(x0,Sup(x));
		     y = INTERVAL(y0,Sup(y));
	             break;
		     
	  case SUB : 
		     BiasRoundUp();   x0_sup = Inf(z)+Sup(y); 
                                      x0_inf = Inf(z)+Inf(y);  
		     BiasRoundNear();
                     if(x0_inf > Sup(x)){
                       return false;
		     }else if(x0_sup < Inf(x)){
                       x0=Inf(x);
                     }else{
//                           if(x0_inf>x0_sup) x0_inf=x0_sup;  //this should never happen, but happens!
			  xx&=INTERVAL(x0_inf,x0_sup);
                          
		          x0= Inf(xx) + (REAL)rand()/(REAL)RAND_MAX*(Diam(xx));
	                  if(!xx.contains(x0)) x0=Mid(xx);
                     }
		     BiasRoundDown(); y0 = x0-Inf(z); // x0-y0 <= Sup(z)
		     BiasRoundNear();

		     if(y0>Sup(y)) y0=Sup(y);
		     else if(y0<Inf(y)){
//                        x0=Sup(xx); //it is necessary?
                       y0=Inf(y);
                     }

		     x = INTERVAL(x0,Sup(x));
		     y = INTERVAL(Inf(y),y0);
// 		     if(Sup(SubBounds(x0,y0)) > Sup(z)) cout << "error Sup-" << endl;
	             break;
	  case MUL : 
		     (Inf(y)>=0)? BiasRoundUp():BiasRoundDown();
	    	     if((Inf(x)>=0  && Inf(y)>=0) || (Sup(x)<=0  && Sup(y)<=0)){ //y decroissante
                       x0_inf = (Sup(y)==0)? BiasNegInf:Inf(z)/Sup(y);
		       x0_sup = (Inf(y)==0)? BiasPosInf:Inf(z)/Inf(y);
		     }else{// if(Sup(z)<=0){
		       x0_inf =  (Inf(y)==0)? BiasPosInf:Inf(z)/Inf(y);
		       x0_sup = (Sup(y)==0)? BiasNegInf:Inf(z)/Sup(y);	       
		     }
		     BiasRoundNear();

                     if((Inf(y)>=0 && x0_inf > Sup(x)) || (Sup(y)<=0 && x0_sup < Inf(x))) {
                       return false;
		     }else if(Inf(y)>=0 && x0_sup < Inf(x)){
                       x0=Inf(x);
                     }else if(Sup(y)<=0 && x0_inf > Sup(x)){
                       x0=Sup(x);
                     }else{
// 		        if(x0_sup<x0_inf) return false;
		        if(!(xx&=INTERVAL(x0_inf,x0_sup))) return false;
		        x0= Inf(xx) + (REAL)rand()/(REAL)RAND_MAX*(Diam(xx));
	                if(!xx.contains(x0)) x0=Mid(xx);
                     }
		     if(x0!=0){
		       if(Inf(x)>=0){ //y croissante
		         BiasRoundUp(); y0= Inf(z)/x0; // x0*y0 <= Sup(z)
		       }else{ // if(Sup(x)<=0){
		         BiasRoundDown(); y0= Inf(z)/x0; // x0*y0 <= Sup(z)	       
		       }
                       BiasRoundNear();
		       if(y0>Sup(y)) y0=Sup(y);
		       if(y0<Inf(y)) y0=Inf(y);	
                     }
		     

		     x = (Inf(y)>=0)? INTERVAL(x0,Sup(x)):INTERVAL(Inf(x),x0);
                     if(x0!=0) y = (Inf(x)>=0)? INTERVAL(y0,Sup(y)):INTERVAL(Inf(y),y0); 
		     break;
	  case DIV:  (Inf(y)>=0)? BiasRoundUp():BiasRoundDown();
	    	     if(Sup(z)>=0){ //y croissante
		       x0_inf = Inf(z)*Inf(y);
		       x0_sup = Inf(z)*Sup(y);
		     }else{// if(Sup(z)<=0){
		       x0_inf = Inf(z)*Sup(y);
		       x0_sup = Inf(z)*Inf(y);	       
		     }	    
		     BiasRoundNear();

                     if((Inf(y)>=0 && x0_inf > Sup(x)) || (Sup(y)<=0 && x0_sup < Inf(x))) {
                       return false;
		     }else if(Inf(y)>=0 && x0_sup < Inf(x)){
                       x0=Inf(x);
                     }else if(Sup(y)<=0 && x0_inf > Sup(x)){
                       x0=Sup(x);
                     }else{
// 		        if(x0_sup<x0_inf) return false;
		        if(!(xx&=INTERVAL(x0_inf,x0_sup))) return false;
		        x0= Inf(xx) + (REAL)rand()/(REAL)RAND_MAX*(Diam(xx));
	                if(!xx.contains(x0)) x0=Mid(xx);
                     }
		    
		     if(Inf(x)>=0){ //y decroissante
		       BiasRoundDown(); y0= x0/Inf(z); // x0/y0 <= Sup(z)
		     }else{ // if(Sup(x)<=0){
		       BiasRoundUp(); y0= x0/Inf(z); // x0/y0 <= Sup(z)
		     }
		     BiasRoundNear();

		     if(y0>Sup(y)) y0=Sup(y);
		     if(y0<Inf(y)) y0=Inf(y);
		     x = (Inf(y)>=0)? INTERVAL(x0,Sup(x)):INTERVAL(Inf(x),x0);
                     y = (Inf(x)>=0)? INTERVAL(Inf(y),y0):INTERVAL(y0,Sup(y));
		     break;
	}

// 	switch (op) {
// 	  case ADD :
// 	    if(!((x+y) <= t))  cout << "floating error!"  << x << "+" << y << "=" << (x+y) << " contained in? " << t << endl;
// 	    if(Diam(x+y)> 1e-7 && Diam(x+y) < 0.9*Diam(t)) cout << "diameter error!"  << x << "+" << y << "=" << (x+y) << " contained in? " << t << endl;
// 	    break;
// 	  case SUB :
// 	    if(! ((x-y) <= t))  cout << "floating error!"  << x << "-" << y << "=" << (x-y) << "contained in? " << t << endl;
//     	    if(Diam(x-y)> 1e-7 && Diam(x-y) < 0.9*Diam(t)) cout << "diameter error!"  << x << "-" << y << "=" << (x-y) << " contained in? " << t << endl;
// 
// 	    break;
// 	  case MUL:
// //             cout << "res:" <<x << "*" << y << " in " << t <<endl;
// 	    if(! ((x*y) <= t))  cout << "floating error!"  << x << "*" << y << "=" << (x*y) << "contained in? "<< t << endl;
//     	    if(Diam(x*y)> 1e-7 && Diam(x*y) < 0.9*Diam(t)) cout << "diameter error!"  << x << "*" << y << "=" << (x*y) << " contained in? " << t << endl;
// 	    break;
// 	  case DIV:
// 	  if(!((x/y) <= t))  cout << "floating error!: " << x << "/" << y << "=" << (x/y) << "contained in? "<< t << endl;
//           if(Diam(x/y)> 1e-7 && Diam(x/y) < 0.9*Diam(t)) cout << "diameter error!"  << x << "/" << y << "=" << (x/y) << " contained in? " << t << endl;
// 	    
// 	  break;
// 	}
	return true;
}
}
