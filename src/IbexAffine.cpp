// #include <vector>
#include "Functions.h"
#include "IbexAffine.h"

namespace ibex {

   AF2_expr::AF2_expr(INTERVAL a, int i, int n) : n(n), type(OK){
     x.resize(n+2,INTERVAL(0.0));
     if(a.unbounded()) 
       type=AF2_expr::UNBOUNDED;
     else{
       x[0]=Mid(a);
       if(Diam(a)==0)
         x[i+1]=0;
       else{
	 //if(x[0]==0 ) x[i+1]=Sup(a); else 
	 x[i+1]=max( Sup(INTERVAL(Sup(a))-x[0]), Sup(x[0]-INTERVAL(Inf(a))));
       }
       type=AF2_expr::OK;
     }
  }
  
  INTERVAL AF2_expr::toI() const{
    if(type==AF2_expr::UNBOUNDED){ return INTERVAL(BiasNegInf,BiasPosInf); }
    INTERVAL s=0;
     for(int i=1; i<=n+1; i++)
       s+=Abs(x[i]);
       
     s=INTERVAL(Inf(-s),Sup(s));
     return (s+x[0]);
  }

 INTERVAL AF2_expr::ev_linear() const{
  if(type==AF2_expr::UNBOUNDED){ return INTERVAL(BiasNegInf,BiasPosInf); }
  INTERVAL s=0;
   for(int i=1; i<=n; i++)
     s+=Abs(x[i]);
   
   s=INTERVAL(Inf(-s),Sup(s));
   return (s+x[0]);
 }

INTERVAL AF2_expr::ev(INTERVAL_VECTOR& a){
  if(type==AF2_expr::UNBOUNDED){ return INTERVAL(BiasNegInf,BiasPosInf); }
  INTERVAL s=0;
   for(int i=1; i<=n; i++)
     s+=x[i]*a(i);
   
   s+=x[0]+x[n+1]*INTERVAL(-1,1);
   return s;
}


void minus(const AF2_expr& x, AF2_expr& z){
   if(z.type==AF2_expr::EMPTY) z=AF2_expr(x.n);
   if(x.type==AF2_expr::UNBOUNDED){ z.type=AF2_expr::UNBOUNDED; return; }
   
   int n=x.n;

   for(int i=0; i<=x.n; i++)
     z[i]=-x[i];
     
   z[n+1]=x[n+1];
   z.type=AF2_expr::OK;  
}

void add(const AF2_expr& x, const AF2_expr& y, AF2_expr& z){
   if(z.type==AF2_expr::EMPTY) z=AF2_expr(x.n);
   if(x.type==AF2_expr::UNBOUNDED || y.type==AF2_expr::UNBOUNDED){ z.type=AF2_expr::UNBOUNDED; return; }

   for(int i=0; i<x.size(); i++)
     z[i]=x[i]+y[i];
  z.type=AF2_expr::OK;
}

void sub(const AF2_expr& x, const AF2_expr& y, AF2_expr& z){
   if(z.type==AF2_expr::EMPTY) z=AF2_expr(x.n);
   if(x.type==AF2_expr::UNBOUNDED || y.type==AF2_expr::UNBOUNDED){ z.type=AF2_expr::UNBOUNDED; return; }

   int n=x.n;
   for(int i=0; i<=n; i++)
     z[i]=x[i]-y[i];

   
   z[n+1]=x[n+1]+y[n+1];
   z.type=AF2_expr::OK;
}


void mult(const AF2_expr& x, const AF2_expr& y, AF2_expr& z){
   if(z.type==AF2_expr::EMPTY) z=AF2_expr(x.n);
   if(x.type==AF2_expr::UNBOUNDED || y.type==AF2_expr::UNBOUNDED){ z.type=AF2_expr::UNBOUNDED; return; }
   int n=x.n;


   INTERVAL zero=INTERVAL(0,0);
   if(!(x[0]==zero)){
     for(int i=1;i<=n;i++)
       if(!(y[i]==zero)) 
	   z[i]=x[0]*y[i];
       else z[i]=0;
       
    
   }else{
     for(int i=1;i<=n+1;i++)
       z[i]=0;
   }
      
   if(!(y[0]==zero)){
    for(int i=1;i<=n;i++)
      if(x[i]!=zero){
	   z[i]+=y[0]*x[i];
      }
      
   }


   INTERVAL z_error=(x[0]+x[n+1]*INTERVAL(-1,1))*(y[0]+y[n+1]*INTERVAL(-1,1));
   
   for(int i=1;i<x.size();i++){
     if(x[i]!=zero)
       for(int j=1;j<x.size();j++){
	 if(y[j]!=zero){
   	   INTERVAL xy= x[i]*y[j];
	   if(i==j && i<=x.n){ // the same variable
	   
              if(Sup(xy)>0) z_error+=INTERVAL(0,Sup(xy));
	      if(Inf(xy)<0) z_error+=INTERVAL(Inf(xy),0);
	      
	   }else if((i==n+1 && j==n+1)) continue;
           else z_error+=INTERVAL(Inf(-INTERVAL(Abs(xy))), Abs(xy));
	 }
       }
   }
   
   z[0]=Mid(z_error);
   z[n+1]=max( Sup(INTERVAL(Sup(z_error))-z[0]), Sup(z[0]-INTERVAL(Inf(z_error))));   

   

   
  z.type=AF2_expr::OK;
}

void div(const AF2_expr& x, const AF2_expr& y,  AF2_expr& z, AF2_expr::linear_mode lmode){
  if(x.type==AF2_expr::UNBOUNDED || y.type==AF2_expr::UNBOUNDED){ z.type=AF2_expr::UNBOUNDED; return; }
  AF2_expr inv_y;
  
  inv(y, inv_y, lmode);
  mult(x, inv_y, z);
}



void sqrt(const AF2_expr& x, AF2_expr& z, AF2_expr::linear_mode mode){
  if(z.type==AF2_expr::EMPTY) z=AF2_expr(x.n);
  if(x.type==AF2_expr::UNBOUNDED){ z.type=AF2_expr::UNBOUNDED; return; }
  
  INTERVAL Ix=x.toI();
  
  if( !(Ix &= INTERVAL(0,BiasPosInf)) ) {z.type=AF2_expr::EMPTY; return;}
  INTERVAL sqrt_x=Sqrt(Ix);
  
  INTERVAL a=Inf(Ix);
  INTERVAL b=Sup(Ix);
  INTERVAL sqrt_a=Inf(Sqrt(a));
  INTERVAL sqrt_b=Sup(sqrt_x);

  INTERVAL alpha, u, f_u, r_u;
  if(mode==AF2_expr::CHEB){
    alpha = INTERVAL(1) / (sqrt_a+ sqrt_b);
    u=INTERVAL(0.25) / Sqr(alpha);
    f_u=Sqrt(u);
    r_u=sqrt_a+alpha*(u-a);
  }else if(mode==AF2_expr::MINR){
    alpha = INTERVAL(1/2) / sqrt_b;
    u=a; //the point that maximize f(x)-r(x) = diameter of error
    f_u=sqrt_a;
    r_u=sqrt_b-alpha*(b-u);
  }
    cheb(x, z, f_u, r_u, alpha, u);
    
}


INTERVAL Root2(INTERVAL a, int r){
  if(r>=2) 
    return Root(a,r);
  else if(r==1)
    return a;
  else if(r==0)
    return 1;
  else if(r<0){
    return 1/Root(a,-r);
  }
}

void pow(const AF2_expr& x, int p, AF2_expr& z, AF2_expr::linear_mode mode){
  if(z.type==AF2_expr::EMPTY) z=AF2_expr(x.n);
  if(x.type==AF2_expr::UNBOUNDED){ z.type=AF2_expr::UNBOUNDED; return; }

  INTERVAL Ix=x.toI();



  if(/*Diam(Ix)>=1e-10 && */((p%2==0 && p >= 2 && mode!=AF2_expr::MINR) || (Inf(Ix)>=1e-10 || Sup(Ix)<=-1e-10)) && p!=0 && p!=1){ //convex
    INTERVAL pow_x=Power(Ix,p);
    INTERVAL a=Inf(Ix);  INTERVAL b=Sup(Ix);
    INTERVAL pow_a=Power(a,p);
    INTERVAL pow_b=Power(b,p);    
    
    INTERVAL alpha, u, f_u, r_u;
    
    if(mode==AF2_expr::CHEB){
      alpha = (pow_b - pow_a) / (b-a);
      u = Root2((alpha/REAL(p)), p-1);
      f_u=Power(u,p);
      r_u=pow_a+alpha*(u-a);

    }else if(mode==AF2_expr::MINR){
      if(abs(p)%2!=0){ //it depends on the sign of p
          alpha = INTERVAL(p)*Power( (p>0)? a:b, p-1);
	  u= (p>0)? b:a;
	  f_u= (p>0)? pow_b:pow_a;
	  r_u= (p>0)? pow_a+alpha*(u-a) : pow_b-alpha*(b-u);
      }else{ //it depends on the sign of x and p
          alpha = INTERVAL(p)*Power( ((Inf(Ix)>=0 && p>0) || (Sup(Ix)<=0 && p<0))? a:b, p-1);
	  u= ((Inf(Ix)>=0 && p>0) || (Sup(Ix)<=0 && p<0))? b:a;
	  f_u= ((Inf(Ix)>=0 && p>0) || (Sup(Ix)<=0 && p<0))? pow_b:pow_a;
	  r_u= ((Inf(Ix)>=0 && p>0) || (Sup(Ix)<=0 && p<0))? pow_a+alpha*(u-a) : pow_b-alpha*(b-u);
      }
    }
  
    cheb(x, z, f_u, r_u, alpha, u);  
    

  
  }else if(p==0){
    z=AF2_expr(1.0,x.n,x.n);
  }else if(p==1){
    z=x;
  }else{
//     cout << "here?" << endl;
    if(p<0) z=AF2_expr(1/Power(Ix,-p),x.n,x.n);
    else z=AF2_expr(Power(Ix,p),x.n,x.n);

  }


}

void inv(const AF2_expr& x, AF2_expr& z,  AF2_expr::linear_mode mode){ /* 1/x */
  if(x.type==AF2_expr::UNBOUNDED){ z.type=AF2_expr::UNBOUNDED; return; }
//   z=AF2_expr(1/x.toI(),x.n,x.n);
  pow(x, -1, z, mode);
}

void sqr(const AF2_expr& x, AF2_expr& z, AF2_expr::linear_mode mode){
  if(z.type==AF2_expr::EMPTY) z=AF2_expr(x.n);
  if(x.type==AF2_expr::UNBOUNDED){ z.type=AF2_expr::UNBOUNDED; return; }

  INTERVAL Ix=x.toI();

//  cout << (mode==AF2_expr::MINR) << endl;
  if(/*Diam(Ix)<1e-10 ||*/ (mode==AF2_expr::MINR && Ix.contains(0))){
    z=AF2_expr(Sqr(Ix),x.n,x.n);
    return;
  }

  INTERVAL a=Inf(Ix);
  INTERVAL b=Sup(Ix);
  INTERVAL sqr_a=Sqr( a );
  INTERVAL sqr_b=Sqr( b );

  INTERVAL alpha, u, f_u,r_u;
  if(mode==AF2_expr::CHEB){
    alpha = alpha = (sqr_b - sqr_a) / (b-a);
    u= alpha/INTERVAL(2);
    f_u=Sqr(u);
    r_u=sqr_a+alpha*(u-a);
  }else if(mode==AF2_expr::MINR){
      if(Inf(Ix)>=0){
        alpha = 2*a;
        u=b;
        f_u=sqr_b;
	r_u=sqr_a+alpha*(u-a);
      }else if(Sup(Ix)<=0){
        alpha = 2*b;
        u=a;
        f_u=sqr_a;
	r_u=sqr_b-alpha*(b-u);
      }
  }


  cheb(x, z, f_u, r_u, alpha, u); 
}

void log(const AF2_expr& x, AF2_expr& z, AF2_expr::linear_mode mode){
  if(z.type==AF2_expr::EMPTY) z=AF2_expr(x.n);
  if(x.type==AF2_expr::UNBOUNDED){ z.type=AF2_expr::UNBOUNDED; return; }


  INTERVAL Ix=x.toI();

  if( !(Ix &= INTERVAL(0,BiasPosInf)) ) {z.type=AF2_expr::EMPTY; return;}
  

  INTERVAL a=Inf(Ix);
  INTERVAL b=Sup(Ix);
  INTERVAL log_a=Log( a );
  INTERVAL log_b=Log( b );
  if(log_a.unbounded() || log_a==BiasNegInf){
      z=AF2_expr(Log(Ix),x.n,x.n);
      return;       
  }
  
  INTERVAL alpha, u, f_u, r_u;  
  if(mode==AF2_expr::CHEB){
    alpha = (log_b - log_a) / (b-a);
    if(alpha.unbounded()){
      z=AF2_expr(Log(Ix),x.n,x.n);
      return;    
    }   
    u=1/alpha;
    f_u=Log(u); 
    r_u=log_a+alpha*(u-a);
  }else if(mode==AF2_expr::MINR){
    alpha = INTERVAL(1.0) / b;
    if(alpha.unbounded()){
      z=AF2_expr(Log(Ix),x.n,x.n);
      return;    
    }
    u=a;
    f_u=log_a;  
    r_u=log_b-alpha*(b-u);
  }
  
  cheb(x, z, f_u, r_u, alpha, u); 

}


void exp(const AF2_expr& x, AF2_expr& z, AF2_expr::linear_mode mode){
  if(z.type==AF2_expr::EMPTY) z=AF2_expr(x.n);
  if(x.type==AF2_expr::UNBOUNDED){ z.type=AF2_expr::UNBOUNDED; return; }

  INTERVAL Ix=x.toI();
  
  INTERVAL a=Inf(Ix);
  INTERVAL b=Sup(Ix);
  INTERVAL exp_a=Exp( a );
  INTERVAL exp_b=Exp( b );
  
  
  INTERVAL alpha, u, f_u;
  if(mode==AF2_expr::CHEB){
    alpha = (exp_b - exp_a) / (b-a);
    if(alpha.unbounded()){
       z=AF2_expr(Exp(Ix),x.n,x.n);
       return;    
    }
    u=Log(alpha);
    f_u=Exp(u);
  }else if(mode==AF2_expr::MINR){
    alpha = exp_a;
    if(alpha.unbounded()){
      z=AF2_expr(Exp(Ix),x.n,x.n);
      return;    
    }
    u=b;
    f_u=exp_b;
  }
  
  INTERVAL r_u=exp_a+alpha*(u-a);
  
  cheb(x, z, f_u, r_u, alpha, u); 
}

void cheb(const AF2_expr& x, AF2_expr& z, INTERVAL &f_u, INTERVAL& r_u, INTERVAL& alpha, INTERVAL& u){
  INTERVAL delta=Abs((f_u-r_u)/INTERVAL(2));
  INTERVAL dseta =(f_u+r_u)/INTERVAL(2)-alpha*u;
 
  z[0]=alpha*x[0] + dseta;
  
  for(int i=1;i<=x.n;i++){
    if(x[i]!=INTERVAL(0,0)) z[i]=alpha*x[i];
    else z[i]=0;
  }
  
  z[x.n+1]=Abs(alpha*x[x.n+1]) + delta; //z[x.n+2]=0; z[x.n+3]=0;
  /*if(Sup(alpha)>0){
    z[x.n+2]=Sup(alpha)*x[x.n+2]; 
    z[x.n+3]=Sup(alpha)*x[x.n+3]; 
  }
  if(Inf(alpha)<0){
    z[x.n+2]+= Sup(-alpha)*x[x.n+3]; 
    z[x.n+3]+= Sup(-alpha)*x[x.n+2]; 
  } */ 

  z.type=AF2_expr::OK;
}


std::ostream& operator<<(std::ostream& os, const AF2_expr& x) {
  os << Mid(x[0]);
  for(int i=1;i<=x.n;i++)
    if(x[i] != INTERVAL(0,0)) os<< "+" << Mid(x[i]) << "*eps" << i;
  int n=x.n;
  
  if(x[n+1] != INTERVAL(0,0)) os << " + " << x[n+1] << "eps+-";
  return os;
}

/* -------------------------------------------------------------------------------------------*/

} // end namespace ibex
