//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor specialized for linear modelization
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 6, 2015
// Last Update : Feb 6, 2015
//============================================================================

using namespace std;
#include "ibex_CtcQInterLinearv8.h"
#include "ibex_Affine2.h"

// Special for estimating the fundamental matrix with the algebraic criterion 
// with a model with 8 variables  (the 9th is set to 1).
// 
// 

namespace ibex {

 
  /*
Interval CtcQInterLinearv8::fwd(IntervalVector & box, int iter)
  {
    Interval eps (-epseq,epseq);
    Interval evald= eps+linfun[iter][0][0];
    
    for (int j=0;j<nb_var-1;j++)
      evald+=linfun[iter][j+1][0]*box[j];
    return evald;
  }
  
  */
 // the evaluation of the iterth equation with uncertainties
  // to check if the iterth measurement is valid (0 must be in the returned interval)
  // algebraic evaluation (call eval_dist)  or Sampson evaluation  (call eval_sampson)
  
  Interval CtcQInterLinearv8::eval_ctc(IntervalVector & box, int iter, int k){
    return (eval_dist(box,iter,k) & eps);
  }

  
  // 
  Interval CtcQInterLinearv8::eval_dist(IntervalVector & box, int iter, int k)
  {
    int nbvar=nb_var;
    Interval evald= linfun[iter][0][k];

    for (int i=0; i< nbvar; i++)
      {//cout << j << " : " << linfun[iter][j+1][k] << "  " << box[j] << "   " << linfun[iter][j+1][k]*box[j] << endl;
      evald+=linfun[iter][i+1][k]*box[i];
      }
    //    cout << "iter " << iter << " " << evald  <<  " " << evald * evald << endl; 

    return evald;
  }
  

 
  // the individual error at the middle of box for the membership equation of the iterth point to be on the
  // plane 
  // property : it is less than epseq  :  only implemented for kfun=1 

  /*
  double CtcQInterLinearv8::compute_err_iter(IntervalVector & box, int iter){
    IntervalVector mid (box.mid());
    Interval res=  eval_dist(mid,iter,0) - box[nb_var-1].mid();
    //    cout << " " << iter << " " << res << endl;
    return res.mag();
  }
  */

  double CtcQInterLinearv8::compute_err_iter(IntervalVector & box, int iter){return 0;}

/* fwd  evaluation of the linear constraint  box[n]= linfun[iter][1][k]* box[0] + linfun[iter][2][k]* box[1]  + ...  
for n=2,3,4 */
  //  void CtcQInterLinearv8::fwd(IntervalVector & box, int iter)

  // for Linear constraint, simple forward evaluation of the constraint (fwdbwd too expensive)  
 
  
  void CtcQInterLinearv8::point_contract(IntervalVector & box, int iter)
  {
    //    cout << "iter " << iter << " box  " << box << endl;
      for (int k=0; k<kfun; k++)
	{ 
	  Interval eval=eval_ctc(box, iter, k);
	  
	  if  (eval.is_empty())
	    {box.set_empty();return;}
	}
  }
  
  /*
   
  void CtcQInterLinearv8::point_contract(IntervalVector & box, int iter){
    point_fwdbwd (box,iter);
  }
  
  */
  
void CtcQInterLinearv8::point_fwdbwd(IntervalVector & box, int iter)
  {
    //    cout << "iter " << iter << " box  " << box << endl;
      for (int k=0; k<kfun; k++)
	{ 
	  fwdbwd(box, iter, k);

	  if  (box.is_empty()) return;
	}
  }
  

  /*
void CtcQInterLinearv8::point_fwdbwd(IntervalVector & box, int iter)
{CtcQInter::point_contract(box,iter);
}
  */

  Interval CtcQInterLinearv8::sumlin(IntervalVector & box, int iter, int j, int k){
    Interval sumlin (linfun[iter][0][k],linfun[iter][0][k]) ;
    int nbvar=nb_var;

    for (int i=0; i< nbvar; i++)
      {if (i==j) continue;
	sumlin+= box[i] * linfun[iter][i+1][k];
      }
    return sumlin;
  }

	  
    
  /*
  void CtcQInterLinearv8::fwdbwd(IntervalVector & box, int iter, int k){
    Interval eval=eval_ctc(box, iter, k);
  for (int j=0; j< nb_var; j++)
    {box[j]&= (  eval  - sumlin(box,iter,j,k) )   /linfun[iter][j+1][k];
      if  (box[j].is_empty())
	{box.set_empty();return;}
    }
  }
  */

  /*
  void CtcQInterLinearv8::fwdbwd(IntervalVector & box, int iter, int k){
    Interval eval=eval_dist(box, iter, k);
    Interval eval1= eval & eps;
    if (eval1.is_empty()) {box.set_empty(); return;}
    for (int j=0; j< nb_var; j++)
      {box[j]&= (  eval1  - eval + linfun[iter][j+1][k]*box[j] )   /linfun[iter][j+1][k];
	if  (box[j].is_empty())
	  {box.set_empty();return;}
      }
  }
  */
  
 

void CtcQInterLinearv8::fwdbwd(IntervalVector & box, int iter, int k){
  int nbvar=nb_var;
    Interval eval[nbvar+1];
    eval[0]= linfun[iter][0][k];
    for (int i=1; i< nbvar+1; i++) {
      eval[i]= eval[i-1]+ box[i-1]* linfun[iter][i][k];
    }
    eval[nbvar]&=eps;
    for (int i=nbvar-1; i>= 0; i--) {
      box[i] &= (eval[i+1] - eval[i])/ linfun[iter][i+1][k];
      if (box[i].is_empty()) {box.set_empty();return;}
      if(i>0) eval[i] &= eval[i+1]-box[i]* linfun[iter][i+1][k];
    }


  }





  CtcQInterLinearv8::CtcQInterLinearv8(int n, const Array<Ctc>& ctc_list, double*** linfun, 
				 double epseq, int q, qintermethod meth) : 
    CtcQInter(n,ctc_list,q,meth),linfun(linfun),
    epseq(epseq) {eps= Interval(-epseq,epseq); init();
  }
  /*
  int CtcQInterLinearv8::points_count(IntervalVector& box){
    int p=0;
    list<int>::iterator iter = points->begin() ;
    int ll= points->size();
    while (iter != points->end()){
      if (!(fwd (box,*iter)&box[nb_var-1]).is_empty()) p++;
      if (p + ll-*iter < q) break; // counting p only if it can be greater than q
      iter++;

    }
    
    return p;
 }
  */


  /*

// no need to redefine activepoints_count and activepoints_contract_count
// 


int CtcQInterLinearv8::activepoints_count(IntervalVector& box){
    int p=0;
    list<int>::iterator iter = points->begin() ;
    //    int ll= points->size();
    while (iter != points->end()){
      if (!(eval_ctc (box,*iter)&box[nb_var-1]).is_empty()) p++;
      //      if (p + ll-*iter < q) break; // counting p only if it can be greater than q
      iter++;

    }
    
    return p;
 }
 

int CtcQInterLinearv8::activepoints_contract_count(IntervalVector& box){
    int p=0;
    list<int>::iterator iter = points->begin() ;
    while (iter != points->end()){
      if (!(eval_ctc (box,*iter)&box[nb_var-1]).is_empty()){
	p++;
	iter++;}
      else
	iter=points->erase(iter);
    }
    
    return p;
 }
  */
 

/* for linear constraint, CtcFwdBWd made by hand */

/*
 void  CtcQInterLinearv8::point_contract(IntervalVector& box, int iter) 
 {
       fwdbwd(box,iter);
       //  fwd(box,iter);   // more efficient ->  fix point useless on CtcQInter constraint
 }
*/

  /* computations for the affine projection : see  CtcQInterAff.cpp */


CtcQInterAffLinearv8::CtcQInterAffLinearv8(int n, const Array<Ctc>& ctc_list, double*** linfun, 
				 double epseq, int q,  qintermethod meth ) : 
  CtcQInter(n,ctc_list,q,meth),
  CtcQInterLinearv8 (n,ctc_list,linfun,epseq,q,meth),
  CtcQInterAff(n,ctc_list,q,meth)
        {
	}


 double CtcQInterAffLinearv8::err_compute( int iter, int k, const IntervalVector& box,Affine2& af)
 {return (fabs( epseq * linfun[iter][0][k]));}

  
double CtcQInterAffLinearv8::valmean_compute(int iter, int k, const IntervalVector& box, Affine2& af)
{ int nbvar=nb_var;
     double valmean = linfun[iter][0][k];
    for (int j =0; j< nbvar-1; j++)
      valmean+=linfun[iter][j+1][k]*box[j].mid();
    return valmean;}



  double  CtcQInterAffLinearv8::slope_compute(int iter, int j , int k , const IntervalVector& box,Affine2& af2)
  {    return linfun[iter][j+1][k];
  }

  
  // no need to affine arithmetics : functions err_compute, valmean_compute, and slope_compute use directly linfun
  void CtcQInterAffLinearv8::compute_affine_evaluation( int i, int iter,  Affine2& af, Interval& af2) {
   ; }
  

  
  Interval CtcQInterAffLinearv8::eval_dist(IntervalVector & box, int iter, int k)
  {
    int nbvar=nb_var;
    Interval evald= linfun[iter][0][k];
    for (int j=0;j<nb_var-1;j++)
      evald+=linfun[iter][j+1][k]*box[j];
    return evald;
  }

  // contraction with fwd evaluation only
  
 
void CtcQInterAffLinearv8::point_contract(IntervalVector & box, int iter)
  {
    int nbvar=nb_var;
      for (int k=0; k<kfun; k++)
	{ 
	  box[nb_var-1]&=eval_ctc(box, iter, k);
	  if  (box[nb_var-1].is_empty())
	    {box.set_empty();return;}
	}
  }




  // contraction with fwd bwd
  
  /*
 void CtcQInterAffLinearv8::point_contract(IntervalVector & box, int iter)
 {point_fwdbwd(box,iter);
 }
  */


  /*
  void CtcQInterAffLinearv8::fwdbwd(IntervalVector & box, int iter, int k){
    box[nb_var-1]&=eval_ctc(box, iter, k);
    if  (box[nb_var-1].is_empty())
      {box.set_empty();return;}
       
    for (int j=0; j< nb_var-1; j++)
      { //box[j]&= (  box[nb_var-1]  - sumlin0(box,iter,j,k) )   /linfun[iter][j+1][k];
	box[j]&= -  sumlin(box,iter,j,k) / linfun[iter][j+1][k] * linfun[iter][0][k];
	//	cout << " j " << box[j] << endl;
	if  (box[j].is_empty())
	  {box.set_empty();return;}
      }
    
 }
  */
  
void CtcQInterAffLinearv8::fwdbwd(IntervalVector & box, int iter, int k){
  int nbvar=nb_var;
    Interval eval[nbvar];

    // fwd
    eval[0]= Interval(1,1);
    for (int i=1; i< nbvar; i++) {
      eval[i]= eval[i-1]+ box[i-1]* linfun[iter][i][k] / linfun[iter][0][k];
    }

    box[nbvar-1]&= (eval[nbvar-1]+eps) * linfun[iter][0][k];
    if (box[nbvar-1].is_empty()) {box.set_empty();return;}
    // bwd
    eval[nbvar-1]&= box[nbvar-1] / linfun[iter][0][k]-eps;

    for (int i=nbvar-2; i>= 0; i--) {
      box[i] &= (eval[i+1] - eval[i])/ linfun[iter][i+1][k] * linfun[iter][0][k];
      if (box[i].is_empty()) {box.set_empty();return;}
      if(i>0) eval[i] &= eval[i+1]-box[i]* linfun[iter][i+1][k] / linfun[iter][0][k];
    }
}

void CtcQInterAffLinearv8::bwd1(IntervalVector & box, int iter){
  int nbvar=nb_var;
    Interval eval[nbvar];
    int k=0;
    // fwd
    eval[0]= Interval(1,1);
    for (int i=1; i< nbvar; i++) {
      eval[i]= eval[i-1]+ box[i-1]* (linfun[iter][i][k]-aff_dir[i-1]) / linfun[iter][0][k];
    }
    // bwd
    eval[nbvar-1]&= box[nbvar-1] / linfun[iter][0][k]-eps;

    for (int i=nbvar-2; i>= 0; i--) {
      box[i] &= (eval[i+1] - eval[i])/ (linfun[iter][i+1][k]-aff_dir[i]) * linfun[iter][0][k];
      if (box[i].is_empty()) {box.set_empty();return;}
      if(i>0) eval[i] &= eval[i+1]-box[i]* (linfun[iter][i+1][k]-aff_dir[i]) / linfun[iter][0][k];
    }
}

  


    /*
void CtcQInterLinearv8::fwdbwd(IntervalVector & box, int iter, int k){
    Interval eval[nb_var+1];
    eval[0]= linfun[iter][0][k];
    for (int i=1; i< nb_var+1; i++) {
      eval[i]= eval[i-1]+ box[i-1]* linfun[iter][i][k];
    }
    eval[nb_var]&=eps;
    for (int i=nb_var-1; i>= 0; i--) {
      box[i] &= (eval[i+1] - eval[i])/ linfun[iter][i+1][k];
      if (box[i].is_empty()) {box.set_empty();return;}
      if(i>0) eval[i] &= eval[i+1]-box[i]* linfun[iter][i+1][k];
    }

  }
    */  
  






  Interval CtcQInterAffLinearv8::sumlin(IntervalVector & box, int iter, int j, int k){
    int nbvar=nb_var;
    Interval sumlin (-epseq,epseq);

    sumlin+=1;
    for (int i=0; i< nbvar-1; i++)
      {	if (i==j) continue;
	sumlin+= box[i] * linfun[iter][i+1][k] / linfun[iter][0][k];}
    sumlin+= box[nbvar-1] / - (linfun[iter][0][k]);
    return sumlin;
  }


  Interval CtcQInterAffLinearv8::sumlin0(IntervalVector & box, int iter, int j, int k){
    int nbvar=nb_var;
    double flin0 = fabs (linfun[iter][0][k]);
    Interval sumlin (-epseq * flin0,epseq *flin0);


    for (int i=0; i< nbvar-1; i++)
      {if (i==j) continue;
	sumlin+= box[i] * linfun[iter][i+1][k];
      }
    return sumlin;
  }


  int  CtcQInterAffLinearv8::affine_threshold () {return 10;}


 Interval CtcQInterAffLinearv8::eval_ctc(IntervalVector & box, int iter, int k){
   double flin0 = fabs (linfun[iter][0][k]);
   Interval eps1 (-epseq * flin0,epseq *flin0);
    //    cout << iter << " eps "  << eps << " " << " dist " << eval_dist(box,iter,k) << endl;
   return (eval_dist(box,iter,k) +   eps1 );
  }


  /*
  CtcQInterLinearSampson::CtcQInterLinearSampson(int n, const Array<Ctc>& ctc_list, double*** linfun, 
				 double epseq, int q, qintermethod meth ) : 
    CtcQInter(n,ctc_list,q,meth),
    CtcQInterLinear (n,ctc_list,linfun,epseq,q,meth) {
  }

Interval CtcQInterLinearSampson::eval_ctc(IntervalVector & box, int iter, int k){
    Interval eps0 (0,epseq);  // for  Sampson  epseq  (avec carré)
    return (eval_sampson(box,iter,k) & eps0);  
  }

 Interval CtcQInterLinearSampson::eval_sampson(IntervalVector & box, int i, int k){
    Interval errxc = box[0] * linfun[i][7][k] + box[1] * linfun[i][8][k] + box[2];
    Interval erryc = box[3] * linfun[i][7][k] + box[4] * linfun[i][8][k] + box[5];
    Interval errwc = box[6] * linfun[i][7][k] + box[7] * linfun[i][8][k] + Interval(1,1);
    //    cout << "errxc " << errxc << " erryc " << erryc << " errwc " << errwc << endl;
    Interval err1 = linfun[i][3][k] * errxc + linfun[i][6][k] * erryc + errwc;
    Interval rx= box[0]* linfun[i][3][k] + box[3] *  linfun[i][6][k] + box[6];
    Interval ry=  box[1]* linfun[i][3][k] + box[4] *  linfun[i][6][k] + box[7];
    Interval err_sampson= sqr(err1) / (sqr(errxc) + sqr(erryc) + sqr(rx)+ sqr(ry));
    //   cout << " i " << i << "err " << err1 << " sampson " << err_sampson << endl;
    return err_sampson;
  }

  // fwdbwd not implemented : call to the generic one
  void CtcQInterLinearSampson::point_fwdbwd(IntervalVector & box, int iter){
    CtcQInter::point_fwdbwd(box,iter);
  }

  // call to generic qinter measure contraction (  generic ibex contraction ; fwdbwd)

  void CtcQInterLinearSampson::point_contract(IntervalVector & box, int iter){
    CtcQInter::point_contract(box,iter);
  }
  

  // call to fwd contraction defined in  CtcQInterLinear
 void CtcQInterLinearSampson::point_contract(IntervalVector & box, int iter){
   CtcQInterLinearv8::point_contract(box,iter);
 }
  */
}
