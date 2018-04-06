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
#include "ibex_CtcQInterLinear.h"
#include "ibex_Affine2.h"
#include<math.h>


// can be used for parameter estimation with a linear criterion   abs (sum_i=1..n ( ai *xi) ) < epseq

namespace ibex {

Interval norm(const IntervalVector & box){
    Interval  norm(0,0);
    for (int i=0; i< box.size(); i++)
      //   norm+= box[i]*box[i];
      norm+= sqr(box[i]);
    return norm;
  } 

  double vnormf(const Vector & vec){
    double vnorm =0;
    for (int i=0;i<9;i++)
      vnorm+= vec[i]*vec[i];
    return (std::sqrt(vnorm));
  }

  /*
Interval CtcQInterLinear::fwd(IntervalVector & box, int iter)
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
  
  double CtcQInterLinear::get_epseq() {return epseq;}

  Interval CtcQInterLinear::eval_ctc(IntervalVector & box, int iter){
    return (eval_dist(box,iter) & eps);
  }

  
  int CtcQInterLinear::effective_size(const IntervalVector & box) {return box.size()-1;}

  

  
  Interval CtcQInterLinear::eval_dist(IntervalVector & box, int iter)
  {
    int nbvar=effective_size(box);
    Interval evald= linfun[iter][0];

    for (int i=0; i< nbvar; i++)
      {//cout << j << " : " << linfun[iter][j+1][k] << "  " << box[j] << "   " << linfun[iter][j+1][k]*box[j] << endl;
      evald+=linfun[iter][i+1]*box[i];
      }
    //    cout << "iter " << iter << " " << evald  <<  " " << evald * evald << endl; 

    return evald;
  }
  

 
  // the individual error at the middle of box for the membership equation of the iterth point to be on the
  // plane 
  // property : it is less than epseq  :  only implemented for kfun=1 

  /*
  double CtcQInterLinear::compute_err_iter(IntervalVector & box, int iter){
    IntervalVector mid (box.mid());
    Interval res=  eval_dist(mid,iter,0) - box[nb_var-]1.mid();
    //    cout << " " << iter << " " << res << endl;
    return res.mag();
  }
  */

  double CtcQInterLinear::compute_err_iter(IntervalVector & box, int iter){return 0;}

/* fwd  evaluation of the linear constraint  box[n]= linfun[iter][1][k]* box[0] + linfun[iter][2][k]* box[1]  + ...  
for n=2,3,4 */
  //  void CtcQInterLinear::fwd(IntervalVector & box, int iter)

  // for Linear constraint, simple forward evaluation of the constraint (fwdbwd too expensive)  
 
  /* version avec kfun  */
  /*
  void CtcQInterLinear::point_contract(IntervalVector & box, int iter)
  {
    //    cout << "iter " << iter << " box  " << box << endl;
      for (int k=0; k<kfun; k++)
	{ 
	  Interval eval=eval_ctc(box, iter, k);
	  
	  if  (eval.is_empty())
	    {box.set_empty();return;}
	}
  }
  */

  // version simplifiee
  /*
 void CtcQInterLinear::point_contract(IntervalVector & box, int iter)
  
    //    cout << "iter " << iter << " box  " << box << endl;
	{ 
	  if  (eval_ctc(box, iter, 0).is_empty())
	    {box.set_empty();return;}
	}
  */
  

 

  
  void CtcQInterLinear::point_contract(IntervalVector & box, int iter){
       point_fwdbwd (box,iter);
  }
  
  //double    CtcQInterLinear::max_diam_threshold(const IntervalVector& box) {return box.max_diam();}
  double    CtcQInterLinear::max_diam_threshold(const IntervalVector& box) {return 0;}
  
void CtcQInterLinear::point_fwdbwd(IntervalVector & box, int iter)
  {
    //    cout << "iter " << iter << " box  " << box << endl;
    //	  if  (box_maxdiam > 0.1)
    //		  fwd(box, iter);
    //	  else
	  	  fwdbwd(box, iter);
  }
  
  
void CtcQInterLinear::point_contract_exact(IntervalVector & box, int iter)
 {
	  fwdbwd(box,iter);
 }
  

  /*
void CtcQInterLinear::point_fwdbwd(IntervalVector & box, int iter)
{CtcQInter::point_contract(box,iter);
}
  */

  Interval CtcQInterLinear::sumlin(IntervalVector & box, int iter, int j){
    Interval sumlin (linfun[iter][0],linfun[iter][0]) ;
    int nbvar=nb_var;

    for (int i=0; i< nbvar; i++)
      {if (i==j) continue;
	sumlin+= box[i] * linfun[iter][i+1];
      }
    return sumlin;
  }

	  
    
  /*
  void CtcQInterLinear::fwdbwd(IntervalVector & box, int iter, int k){
    Interval eval=eval_ctc(box, iter, k);
  for (int j=0; j< nb_var; j++)
    {box[j]&= (  eval  - sumlin(box,iter,j,k) )   /linfun[iter][j+1][k];
      if  (box[j].is_empty())
	{box.set_empty();return;}
    }
  }
  */

  /*
  void CtcQInterLinear::fwdbwd(IntervalVector & box, int iter, int k){
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
  
 
  /*
void CtcQInterLinear::fwdbwd(IntervalVector & box, int iter, int k){
  //  int nbvar=nb_var;
  int nbvar=effective_size(box);
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

  */




void CtcQInterLinear::fwdbwd(IntervalVector & box, int iter){
  //  int nbvar=nb_var;
  int nbvar=effective_size(box);
    Interval eval[nbvar+1];
    eval[0]= linfun[iter][0];
    for (int i=1; i< nbvar+1; i++) {
      eval[i]= eval[i-1] + box[i-1]* linfun[iter][i];
    }
    //    cout << "eval0 " << eval[nbvar] << endl;
    if (eval[nbvar].is_subset(eps)) cout << " measure validation " << " iter " << iter << endl;
    eval[nbvar]&=eps;
    cout << "eval " << eval[nbvar] << endl;
    for (int i=nbvar-1; i>= 0; i--) {
      box[i] &= (eval[i+1] - eval[i])/ linfun[iter][i+1];
      cout << i << "  " << box[i] << endl;
      if (box[i].is_empty()) {box.set_empty();return;}
      if(i>0) eval[i] &= eval[i+1]-box[i]* linfun[iter][i+1];
    }


  }

void CtcQInterLinear::fwd(IntervalVector & box, int iter){
  //  int nbvar=nb_var;
  int nbvar=effective_size(box);
    Interval eval[nbvar+1];
    eval[0]= linfun[iter][0];
    for (int i=1; i< nbvar+1; i++) {
      eval[i]= eval[i-1] + box[i-1]* linfun[iter][i];
    }
    //    cout << "eval0 " << eval[nbvar] << endl;

    if (eval[nbvar].is_subset(eps)) cout << " measure validation " << " iter " << iter << endl;
    eval[nbvar]&=eps;


}



  CtcQInterLinear::CtcQInterLinear(int n, const Array<Ctc>& ctc_list, double** linfun, 
				 double epseq, int q, qintermethod meth ) : 
    CtcQInter(n,ctc_list,q,meth),linfun(linfun),
    epseq(epseq) 
  {eps= Interval(-epseq,epseq); 
    init();
  }
  /*
  int CtcQInterLinear::points_count(IntervalVector& box){
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


int CtcQInterLinear::activepoints_count(IntervalVector& box){
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
 

int CtcQInterLinear::activepoints_contract_count(IntervalVector& box){
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
 void  CtcQInterLinear::point_contract(IntervalVector& box, int iter) 
 {
       fwdbwd(box,iter);
       //  fwd(box,iter);   // more efficient ->  fix point useless on CtcQInter constraint
 }
*/

  /* computations for the affine projection : see  CtcQInterAff.cpp */


CtcQInterAffLinear::CtcQInterAffLinear(int n, const Array<Ctc>& ctc_list, double** linfun, 
				 double epseq, int q,  qintermethod meth ) : 
  CtcQInter(n,ctc_list,q,meth),
  CtcQInterLinear (n,ctc_list,linfun,epseq,q,meth),
  CtcQInterAff(n,ctc_list,q,meth)
{ epipole_epsilon=0.05;
	}

  int CtcQInterAffLinear::effective_size(const IntervalVector & box) {return box.size();}

 double CtcQInterAffLinear::err_compute( int iter, int k, const IntervalVector& box,Affine2& af)
 // {return (fabs( epseq * linfun[iter][0][k]));}
 {return  epseq ;}
  
   
double CtcQInterAffLinear::valmean_compute(int iter, int k, const IntervalVector& box, Affine2& af)
{ int nbvar=nb_var;
     double valmean = linfun[iter][0];
    for (int j =0; j< nbvar-1; j++)
      valmean+=linfun[iter][j+1]*box[j].mid();
    return valmean;}

 double  CtcQInterAffLinear::slope_compute(int iter, int j , int k , const IntervalVector& box,Affine2& af2)
  {    return linfun[iter][j+1];
  }

void CtcQInterAffLinear::fwdbwd(IntervalVector & box, int iter){
  //  int nbvar=nb_var;
  //  int nbvar=effective_size(box);
  int nbvar=9;
  
    Interval eval[nbvar+1];
    eval[0]= linfun[iter][0];
    for (int i=1; i< nbvar+1; i++) {
      eval[i]= eval[i-1] - box[i-1]* linfun[iter][i];
    }
    //    cout << "eval0 " << eval[nbvar] << endl;
    //    if (eval[nbvar].is_subset(eps)) cout << " measure validation " << " iter " << iter << endl;
    eval[nbvar]&=eps;
    if (eval[nbvar].is_empty()) {box.set_empty();return;}  // ajout 10 juin
    //    cout << "eval " << eval[nbvar] << endl;
    for (int i=nbvar-1; i>= 0; i--) {
      box[i] &= (eval[i+1] - eval[i])/ - (linfun[iter][i+1]);
      //      cout << i << "  " << box[i] << endl;
      if (box[i].is_empty()) {box.set_empty();return;}
      if(i>0) eval[i] &= eval[i+1] + box[i]* linfun[iter][i+1];
    }


  }

void CtcQInterAffLinear::fwd(IntervalVector & box, int iter){
  //  int nbvar=effective_size(box);
  int nbvar=9;
  Interval eval[nbvar+1];
  eval[0]= linfun[iter][0];
    for (int i=1; i< nbvar+1; i++) {
      eval[i]= eval[i-1] - box[i-1]* linfun[iter][i];
    }
    //      cout << "eval0 " << eval[nbvar] << endl;
    //    if (eval[nbvar].is_subset(eps)) cout << " measure validation " << " iter " << iter << endl;
    eval[nbvar]&=eps;
    if (eval[nbvar].is_empty())
      box.set_empty(); 
}
  

  double CtcQInterAffLinear::lincoeff(int iter, int i){
    int nbvar=9;
    if (i<nbvar)
      return linfun[iter][i+1];
    if (i==nbvar)
      return linfun[iter][0];
  }


 

  
  // no need to affine arithmetics : functions err_compute, valmean_compute, and slope_compute use directly linfun
  void CtcQInterAffLinear::compute_affine_evaluation( int i, int iter,  Affine2& af, Interval& af2) {
   ; }
  

  
  Interval CtcQInterAffLinear::eval_dist(IntervalVector & box, int iter)
  {
    //    int nbvar=nb_var;
    int nbvar=effective_size(box);
    Interval evald= linfun[iter][0];
    for (int j=0;j<nb_var-1;j++)
      evald+=linfun[iter][j+1]*box[j];
    return evald;
  }
  



  // contraction with fwd bwd 
  
  
  void CtcQInterAffLinear::point_contract (IntervalVector & box, IntervalMatrix& msol, IntervalMatrix & msolt, int iter){
    point_epipole_check (box,msol, msolt, iter);    //avec retrait des mesures telles que Ep2 < epsilon ou p1Et < epsilon 
    if (!(box.is_empty()))
     //     point_fwdbwd(box,iter);
      fwdbwd(box,iter);
     //  fwd(box,iter);
  }
  
  
  int CtcQInterAffLinear::midactivepoints_count(const Vector& vec){
    return   feasible_points(vec).size();
  }


  vector<int> CtcQInterAffLinear::feasible_points(const Vector& vec) {

    vector<int> feasiblepoints;

    list<int>::iterator iter = points->begin() ;
    
    IntervalMatrix msol (3,3);
    IntervalMatrix msolt (3,3);
    IntervalVector box(vec);
   
   


    for (int i=0;i<3;i++)
      for (int j=0;j<3;j++)
	{ msol[i][j]=box[3*i+j];
	  msolt[j][i]=box[3*i+j];
	}

    norm_epipole_epsilon= epipole_epsilon/vnormf(vec);

    /*
    Matrix EtE (3,3);
    EtE=msolt*msol;
    double B= EtE[0][0]+EtE[1][1]+EtE[2][2];
    B= (B/2);
    B=B*B;
    double A= EtE[0][0]*EtE[1][1]-EtE[0][1]*EtE[1][0] +EtE[0][0]*EtE[2][2]-EtE[0][2]*EtE[2][0] + 
      EtE[1][1]*EtE[2][2]-EtE[2][1]*EtE[1][2];
    cout << " A " << A <<  " B " << B << endl;
    */
    
    while (iter != points->end()){
    
      /*    
      pt1[0]= - linfun[*iter][3];
      pt1[1]=- linfun[*iter][6];
      pt1[2]=1;

      res1= msolt * pt1;


      pt2[0]= - linfun[*iter][7];
      pt2[1]= - linfun[*iter][8];
      pt2[2]=1;
	
      res2 = msol * pt2;


      if ((fabs(res1[0]) < epsilon && fabs(res1[1]) < epsilon && fabs(res1[2]) < epsilon)
	  ||
	  (fabs(res2[0]) < epsilon && fabs(res2[1]) < epsilon && fabs(res2[2]) < epsilon))
	{
	  iter++; continue;}
      */
      IntervalVector box1 = box;
      point_contract(box1,msol, msolt, *iter);
      if (!(box1.is_empty())) {

	feasiblepoints.push_back(*iter);
      }
      iter++;
    }
    return feasiblepoints;
  }
  
  // 
  void CtcQInterAffLinear::point_epipole_check (IntervalVector & box, const IntervalMatrix& msol,  const IntervalMatrix& msolt,  int iter) {

   IntervalVector pt1 (3);
   IntervalVector res1(3);


  
   pt1[0]= - linfun[iter][3];
   pt1[1]= - linfun[iter][6];
   pt1[2]=1;

   res1= msolt * pt1;
   if ((res1.mag()).max()< norm_epipole_epsilon){
     box.set_empty(); return;}
   pt1[0]= - linfun[iter][7];
   pt1[1]= - linfun[iter][8];
   pt1[2]=1;
   
   res1 = msol * pt1;
      //      cout << iter << "  " << res1.mag() << "  " << res2.mag() << endl;
   if ((res1.mag()).max() < norm_epipole_epsilon){
     //cout << iter << " res1 " << res1 << " res2 " << res2 << endl; 
     box.set_empty(); return;}
 }

  /*
  void CtcQInterAffLinear::fwdbwd(IntervalVector & box, int iter, int k){
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
  //  void CtcQInterAffLinear::fwdbwd(IntervalVector & box, int iter, int k){ CtcQInterLinear::fwdbwd(box,iter,k);}
  /*
void CtcQInterAffLinear::fwdbwd(IntervalVector & box, int iter, int k){
  int nbvar=effective_size(box);
  //int nbvar=nb_var;
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
  */



void CtcQInterAffLinear::bwd1(IntervalVector & box, int iter){
 int nbvar=effective_size(box);
  // int nbvar=nb_var;
    Interval eval[nbvar];
 
    // fwd
    eval[0]= Interval(1,1);
    for (int i=1; i< nbvar; i++) {
      eval[i]= eval[i-1]+ box[i-1]* (linfun[iter][i]-aff_dir[i-1]) / linfun[iter][0];
    }
    // bwd
    eval[nbvar-1]&= box[nbvar-1] / linfun[iter][0]-eps;

    for (int i=nbvar-2; i>= 0; i--) {
      box[i] &= (eval[i+1] - eval[i])/ (linfun[iter][i+1]-aff_dir[i]) * linfun[iter][0];
      if (box[i].is_empty()) {box.set_empty();return;}
      if(i>0) eval[i] &= eval[i+1]-box[i]* (linfun[iter][i+1]-aff_dir[i]) / linfun[iter][0];
    }
}

  


    /*
void CtcQInterLinear::fwdbwd(IntervalVector & box, int iter, int k){
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
  






  Interval CtcQInterAffLinear::sumlin(IntervalVector & box, int iter, int j){
 int nbvar=effective_size(box);
 //    int nbvar=nb_var;
    Interval sumlin (-epseq,epseq);

    sumlin+=1;
    for (int i=0; i< nbvar-1; i++)
      {	if (i==j) continue;
	sumlin+= box[i] * linfun[iter][i+1] / linfun[iter][0];}
    sumlin+= box[nbvar-1] / - (linfun[iter][0]);
    return sumlin;
  }


  Interval CtcQInterAffLinear::sumlin0(IntervalVector & box, int iter, int j){
 int nbvar=effective_size(box);
 //    int nbvar=nb_var;
    double flin0 = fabs (linfun[iter][0]);
    Interval sumlin (-epseq * flin0,epseq *flin0);


    for (int i=0; i< nbvar-1; i++)
      {if (i==j) continue;
	sumlin+= box[i] * linfun[iter][i+1];
      }
    return sumlin;
  }

  int CtcQInterAffLinear::affine_threshold(){ 
    //return 10;
    return INT_MAX;
  }

  bool CtcQInterLinear::constraint_check(IntervalVector & newvalidpoint , double epscont)
  {return true;}
  
 Interval CtcQInterAffLinear::eval_ctc(IntervalVector & box, int iter){
   double flin0 = fabs (linfun[iter][0]);
   Interval eps1 (-epseq * flin0,epseq *flin0);
   cout << iter << " eps "  << eps << " " << " dist " << eval_dist(box,iter) << endl;
   return (eval_dist(box,iter) +   eps1 );
  }


  int CtcQInterAffLinear::activepoints_contract_count(IntervalVector& box){
    int p=0;
    list<int>::iterator iter = points->begin() ;
    //    cout << " point size " << points->size() << endl;
    //    cout << " box  contract count " << box << endl;

    IntervalMatrix msol(3,3);
    IntervalMatrix msolt(3,3);
    for (int i=0;i<3;i++)
      for (int j=0;j<3;j++)
	{ msol[i][j]=box[3*i+j];
	  msolt[j][i]=box[3*i+j];
       }
    while (iter != points->end()){
      IntervalVector box1=box;
      
      point_epipole_check (box1,msol, msolt, *iter);    //avec retrait des mesures telles que Ep2 < epsilon ou p1Et < epsilon 
      if (!(box1.is_empty()))
     //     point_fwdbwd(box,iter);
	fwdbwd(box1,*iter);
      if (box1.is_empty())
	iter=points->erase(iter);
      else {
	p++; iter++;
      }
    }
    //    cout << " point size after " << points->size() << endl;
    return p;
 }

  int  CtcQInterAffLinear::ctc_contract (IntervalVector & box){
    int p=0;
    box_maxdiam=max_diam_threshold(box);
    norm_epipole_epsilon= epipole_epsilon/vnormf(box.mid());
   
    IntervalMatrix msol (3,3);
    IntervalMatrix msolt (3,3);
    for (int i=0;i<3;i++)
     for (int j=0;j<3;j++)
       { msol[i][j]=box[3*i+j];
	 msolt[j][i]=box[3*i+j];
       }
    list<int>::iterator iter = points->begin() ;
    //    cout << " nb points  " << points->size() <<  " box max diam " << box_maxdiam << endl;
    while (iter != points->end()) { 

      (*boxes)[*iter]=box;
      //      cout << *iter <<  " box " << box << " p " << p << endl;;
      point_contract((*boxes)[*iter],msol, msolt, *iter);  // contraction of the measurement *iter
      //      cout << *iter <<  " box " << (*boxes)[*iter] << " p " << p << endl;;
      //	cout << 	(*boxes)[*iter] << endl;
      if (! (*boxes)[*iter].is_empty()) {
	p++;
	iter++;
      }
      else
	{
	  
	  if (side_effects())    // use of side_effects for not updating points in case of 3bcid
	    iter=points->erase(iter);
	  else 
	    iter++;
	}
    }
    return p;
  }



void CtcQInterAffLinear::ctc_contract_all(IntervalVector& box)
{
 
    norm_epipole_epsilon=epipole_epsilon/vnormf (box.mid());
    IntervalMatrix msol (3,3);
    IntervalMatrix msolt (3,3);
    for (int i=0;i<3;i++)
     for (int j=0;j<3;j++)
       { msol[i][j]=box[3*i+j];
	 msolt[j][i]=box[3*i+j];
       }
  //  cout << " appel contract_all" << endl;
  list<int>::iterator iter = points->begin() ;

  while (iter != points->end())
    {

      point_contract(box,msol, msolt, *iter);  // contraction of the measurement *iter

      if (box.is_empty()) return;
      iter++;



    }
}
  /*
Interval CtcQInterAffLinear::eval_ctc(IntervalVector & box, int iter, int k){
  Interval eps1 (-epseq ,epseq );
   return (eval_dist(box,iter,k) &   eps1 );
  */
 
 
}
