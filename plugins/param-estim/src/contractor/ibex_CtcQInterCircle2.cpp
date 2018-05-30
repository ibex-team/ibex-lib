//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor specialized for circle detection (uncertainty on the equation)
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 6, 2015
// Last Update : Feb 6, 2015
//============================================================================

using namespace std;
#include "ibex_CtcQInterCircle2.h"
#include "ibex_Affine2.h"



namespace ibex {

  /* fwdbwd for the circle constraint  box[n]= (linfun[iter][0][k]- box[0])^2  + (linfun[iter][0][k]- box[1]) ^2
for n=2,3,4 */

Interval CtcQInterCircle2::eval_dist(IntervalVector & box, int iter)
{Interval evald =sqr (measure[iter][0][0] - box[0]) + sqr (measure[iter][1][0] - box[1]);
  return evald;
}

  Interval CtcQInterCircle2::eval_ctc(IntervalVector & box, int iter, int k) {
Interval eps (-epseq,epseq);
 Interval evald= eps + sqr (measure[iter][0][k] - box[0]) + sqr (measure[iter][1][k] - box[1]);
 return evald;
  }


int CtcQInterCircle2::midactivepoints_count(const Vector& vec){
 int p=0;
    list<int>::iterator iter = points->begin() ;
    while (iter != points->end()){
      IntervalVector box1(vec);
      for (int k=0; k<kfun; k++)
	box1[nb_var-1]&= eval_ctc (box1,*iter,k);
      if (!box1[nb_var-1].is_empty()) p++;
      iter++;
    }
    return p;
 }
  
  int CtcQInterCircle2::activepoints_count(IntervalVector& box){
    int p=0;
    list<int>::iterator iter = points->begin() ;
    while (iter != points->end()){
      IntervalVector box1=box;
      for (int k=0; k<kfun; k++)
	box1[nb_var-1]&= eval_ctc (box1,*iter,k);
      if (!box1[nb_var-1].is_empty()) p++;
      iter++;
    }
    return p;
 }


int CtcQInterCircle2::activepoints_contract_count(IntervalVector& box){
    int p=0;
    list<int>::iterator iter = points->begin() ;
    while (iter != points->end()){
      IntervalVector box1=box;
      for (int k=0; k<kfun; k++)
	box1[nb_var-1]&= eval_ctc (box1,*iter,k);
      if (!box1[nb_var-1].is_empty()) {p++; iter++;}
      else 
	iter=points->erase(iter);
    }
    return p;
}

  

double CtcQInterCircle2::compute_err_iter(Vector & vmid, int iter){
    Interval eps (-epseq,epseq);
    IntervalVector mid (vmid);
    //    Interval res=  eval_dist(mid,iter) - box[nb_var-1].mid();
    Interval res=  eval_dist(mid,iter) - vmid[nb_var-1];
    //    cout << " " << iter << " " << eval_dist(mid,iter) << " " <<box[nb_var-1].mid() << " res " <<   res <<  " " << res.mag() << endl;
    return res.mag();
  }

  void CtcQInterCircle2::fwd(IntervalVector & box, int iter)
  { 
    for (int k=0; k<kfun; k++)
      {  Interval eps (-epseq,epseq);
	Interval evald= eps+sqr (measure[iter][0][k] - box[0]) + sqr (measure[iter][1][k] - box[1]);

	box[nb_var-1]&=evald;
	if  (box[nb_var-1].is_empty())
	  {box.set_empty(); return;}
      }
  }


  void CtcQInterCircle2::fwdbwd(IntervalVector & box, int iter)
  { 
    for (int k=0; k<kfun; k++)
      {  Interval eps (-epseq,epseq);
	Interval evald= eps+sqr (measure[iter][0][k] - box[0]) + sqr (measure[iter][1][k] - box[1]);

	box[nb_var-1]&=evald;
	if  (box[nb_var-1].is_empty())
	  {box.set_empty();return;}

       	Interval yb= eps +box[nb_var-1] - sqr (measure[iter][1][k] - box[1]);
	Interval yb1 = sqrt(yb);
	Interval yb2 = (box[0] & (measure[iter][0][k] + yb1)) |( box[0] & (measure[iter][0][k] - yb1));
	box[0]=yb2;

	if  (box[0].is_empty())
	  {box.set_empty();return;}
	Interval xa= eps + box[nb_var-1] - sqr (measure[iter][0][k] - box[0]);
	Interval xa1 = sqrt(xa);
        Interval xa2 = (box[1] & (measure[iter][1][k] + xa1)) | ( box[1] & (measure[iter][1][k] - xa1));
	box[1]=xa2;

	if  (box[1].is_empty())
	  {box.set_empty();return;}
	  
      }
  }


  IntervalVector CtcQInterCircle2::pointintersection(int* iter){return pointintersection(iter[0], iter[1], iter[2]);}
  /*
  IntervalVector CtcQInterCircle2::pointintersection( int iter1, int iter2, int iter3){
    Vector  mid1 (3);
    double x1= measure[iter1][0][0];
    double y1= measure[iter1][1][0];
    double x2= measure[iter2][0][0];
    double y2= measure[iter2][1][0];
    double x3= measure[iter3][0][0];
    double y3= measure[iter3][1][0];
    mid1[0]=- (( std::pow( x3,2) - std::pow( x2,2) +  std::pow( y3,2) -std::pow( y2,2)) / (2 * ( y3- y2))- 
	     ( std::pow( x2,2) - std::pow( x1,2) +  std::pow( y2,2) -std::pow( y1,2)) / (2 * ( y2- y1))
	     )
      /
      ( 
	 (x2- x1)/ (y2-y1) 
	 -
	 (x3- x2)/ (y3-y2) 
	)
      ;
    mid1[1]= -(x2- x1)/ (y2 - y1) * mid1[0]
      +  (std::pow( x2,2) - std::pow(x1,2) +  std::pow( y2,2) -std::pow( y1,2))
    / (2 * ( y2-  y1));

    mid1[2]= std::pow(x1- mid1[0],2) + std::pow(y1 - mid1[1],2) ;
    IntervalVector res (3);
    for (int i=0; i<3; i++)
      res[i]= Interval(-epseq+mid1[i],epseq+mid1[i]);
    
    return res;
  }
  */
 IntervalVector CtcQInterCircle2::pointintersection( int iter1, int iter2, int iter3){
    IntervalVector  mid1 (3);
    Interval eps (-epseq,epseq);
    double x1= measure[iter1][0][0];
    double y1= measure[iter1][1][0];
    double x2= measure[iter2][0][0];
    double y2= measure[iter2][1][0];
    double x3= measure[iter3][0][0];
    double y3= measure[iter3][1][0];

    mid1[0]= (( std::pow( x3,2) - std::pow( x2,2) )/ (y3-y2) - (std::pow( x1,2) - std::pow( x2,2)) / (y1-y2) +y3 - y1  + 2 * eps / (y1-y2) + 2 * eps /(y3 - y2)) / (2 * (x3-x2) / (y3-y2)  - 2 * (x1-x2) / (y1 -y2)) ;

    mid1[1]= -(x2- x1)/ (y2 - y1) * mid1[0]
      +  (std::pow( x2,2) - std::pow(x1,2) +  std::pow( y2,2) -std::pow( y1,2))
      / (2 * ( y2-  y1)) + eps /(y1-y2);

    mid1[2]= sqr (x1- mid1[0]) + sqr (y1 - mid1[1]) + eps;

    return mid1;
 }

IntervalVector CtcQInterCircle2::boxintersection( IntervalVector& box, int iter1, int iter2, int iter3){
  IntervalVector box1 = pointintersection(iter1,iter2,iter3);
  box1&= box;
  return box1;
}

 IntervalVector CtcQInterCircle2::boxintersection( IntervalVector& box, int* iter){
   return (boxintersection( box, iter[0], iter[1], iter[2]));
 }

  CtcQInterCircle2::CtcQInterCircle2(int n, const Array<Ctc>& ctc_list, double*** measure, double epseq, int q, qintermethod meth) : CtcQInter(n,ctc_list,q,meth),measure(measure),epseq(epseq) {CtcQInter::init();}


 bool  CtcQInterCircle2::contract_with_outliers (IntervalVector& box, int gap)  {
   return  CtcQInter::contract_with_outliers(box,gap);}

  /*

 bool  CtcQInterCircle2::contract_with_outliers (IntervalVector& box, int gap)  {
    int outliers=0;
    list<int>::iterator iter = points->begin();
    IntervalVector box1=box;
    int index[3];
    while (iter != points->end()){

      int k=0;
      while (k<3 && iter!= points->end())
	{index[k]=*iter; k++; iter++;}
      //      cout << " cwo : box avant " << box <<  " " << index[0] << " " <<index[1] << " " <<  index[2] << endl;
      if (iter == points->end()) break;
      //    cout << " box avant " << box << endl;
      box1= boxintersection(box1,index[0],index[1],index[2]);

    
      if (box1.is_empty()) {
	outliers++; 
	if (outliers== gap+1) 
	  return true;
	else {box1=box; continue;}
      }

      else{
	list<int>::iterator iter1 = iter;
	while (iter1 != points->end()){
	//	CtcQInter::point_contract(box1,*iter);   // pour faire un vrai fwdbwd  (fwd seul ne sert à rien)
	  point_fwdbwd(box1,*iter);   // pour faire un vrai fwdbwd  (fwd seul ne sert à rien)
      //      cout << " box " << box << endl;

	  iter++; iter1++;
	  if (box1.is_empty()) {
	    outliers++; 
	    if (outliers== gap+1) 
	      return true;
	    else  {box1=box; break;}
	  }
	}

	}

    }
 
    return false;
 }
  */


void CtcQInterCircle2::ctc_contract_all(IntervalVector& box){
    
      int nbp = points->size();

      list<int>::iterator iter = points->begin() ;
      int index[3];
      
      int nbk = nbp/3;
      //int nbk = 1;
      for (int j=0; j< nbk; j++){
	int k=0;      
	while (k<3)
	  {index[k]=*iter; k++; iter++;}
	//	cout << " box avant " << box <<  " " << index[0] << " " <<index[1] << " " <<  index[2] << endl;
	box= boxintersection(box,index[0],index[1],index[2]);
      }
      //      cout << " box apres " << box << endl;
      
      while (iter != points->end()){
	CtcQInter::point_contract(box,*iter);
	iter++;
      }
      //      cout << " box apres ctc " << box << endl;
    }

/* for circle constraint, CtcFwdBWd made by hand */
 void  CtcQInterCircle2::point_contract(IntervalVector& box, int iter) 
 { fwdbwd(box,iter);
   //   fwd(box,iter);
}

  double CtcQInterAffCircle2::err_compute(int iter, int k, const IntervalVector& box, Affine2& af)
  {

    double err1=0;
    for (int j=0; j<nb_var-1; j++){
      err1 += ((std::pow(box[j].ub(),2) + std::pow(box[j].lb(),2))/2 - std::pow(box[j].mid(),2))/2;
    }
    return epseq + err1 ;
  }

  double CtcQInterAffCircle2::valmean_compute(int iter, int k, const IntervalVector& box, Affine2& af)
  { double valmean = 0;
    for (int j =0; j< nb_var-1; j++)
      {
	double err1 = ((std::pow(box[j].ub(),2) + std::pow(box[j].lb(),2))/2 - std::pow(box[j].mid(),2))/2;
	valmean+=std::pow(box[j].mid(),2) + std::pow(measure[iter][j][k],2)+ err1 -2 *measure[iter][j][k]* box[j].mid() ;

      }
    return valmean;
  }
 
  double CtcQInterAffCircle2::slope_compute(int iter, int j, int k, const IntervalVector& box, Affine2& af)
  {  return (
	     (std::pow(box[j].ub()-measure[iter][j][k],2) - std::pow(box[j].lb()-measure[iter][j][k],2)) / box[j].diam() 
	     );
}

  void CtcQInterAffCircle2::compute_affine_evaluation (int i , int iter, Affine2& af, Interval & af2)
  {;}


 CtcQInterAffCircle2::CtcQInterAffCircle2(int n, const Array<Ctc>& ctc_list, double*** measure, double epseq, int q,  qintermethod meth) :
   CtcQInter(n,ctc_list,q,meth),
   CtcQInterCircle2(n,ctc_list,measure,epseq, q,meth),
   CtcQInterAff(n,ctc_list,q,meth) {;}

  //  int  CtcQInterAffCircle2::affine_threshold ()  {return 10;}
  int  CtcQInterAffCircle2::affine_threshold ()  {return INT_MAX;}

  //  double  CtcQInterAffCircle2::max_diam_threshold (const IntervalVector& box)  {return 1.0;}
  double  CtcQInterAffCircle2::max_diam_threshold (const IntervalVector& box)  {return 0.0;}
  //  int  CtcQInterAffCircle2::max_diam_threshold ()  {return 0.0;}


  /*
  void CtcQInterAffCircle2::bwd1(IntervalVector & box, int iter) 
  { int k=0;
    Interval eps (-epseq,epseq);
    Interval ev=eps + box[nb_var-1];
    AffineLin af;
    box[0]&=(ev - (slope_compute (iter,1,k,box,af)-aff_dir[1])*box[1])/(slope_compute (iter,0,k,box,af)-aff_dir[0]);
	    if  (box[0].is_empty())
	  {box.set_empty();return;}


    box[1]&=(ev - (slope_compute (iter,0,k,box,af)-aff_dir[0])*box[0])/(slope_compute (iter,1,k,box,af)-aff_dir[1]);
	    if  (box[1].is_empty())
	      {box.set_empty();return;}
  }
  */

}
