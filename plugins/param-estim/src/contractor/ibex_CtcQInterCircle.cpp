//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor specialized for circle detection 
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 6, 2015
// Last Update : Feb 6, 2015
//============================================================================

using namespace std;
#include "ibex_CtcQInterCircle.h"
#include "ibex_Affine2.h"



namespace ibex {

  /* fwdbwd for the circle constraint  box[n]= (linfun[iter][0][k]- box[0])^2  + (linfun[iter][0][k]- box[1]) ^2
for n=2,3,4 */

Interval CtcQInterCircle::eval_dist(IntervalVector & box, int iter)
{Interval evald =sqr (measure[iter][0][0] - box[0]) + sqr (measure[iter][1][0] - box[1]);
  return evald;
}

  Interval CtcQInterCircle::eval_ctc(IntervalVector & box, int iter, int k) {
Interval eps (-epseq,epseq);
 Interval evald= eps + sqr (measure[iter][0][k] - box[0]) + sqr (measure[iter][1][k] - box[1]);
 return evald;
  }
  
  // redefinition of activepoints_count ; a simple forward evaluation is sufficient.

 int CtcQInterCircle::activepoints_count(IntervalVector& box){
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


int CtcQInterCircle::activepoints_contract_count(IntervalVector& box){
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


double CtcQInterCircle::compute_err_iter(Vector & vmid, int iter){
    Interval eps (-epseq,epseq);
    IntervalVector mid (vmid);
    //    Interval res=  eval_dist(mid,iter) - box[nb_var-1].mid();
    Interval res=  eval_dist(mid,iter) - vmid[nb_var-1];
    //    cout << " " << iter << " " << eval_dist(mid,iter) << " " <<box[nb_var-1].mid() << " res " <<   res <<  " " << res.mag() << endl;
    return res.mag();
  }

  void CtcQInterCircle::fwdbwd(IntervalVector & box, int iter)
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

 CtcQInterCircle::CtcQInterCircle(int n, const Array<Ctc>& ctc_list, Interval*** measure, double epseq, int q, qintermethod meth) : CtcQInter(n,ctc_list,q,meth),measure(measure),epseq(epseq) {;}


/* for circle constraint, CtcFwdBWd made by hand */
 void  CtcQInterCircle::point_contract(IntervalVector& box, int iter) 
 { fwdbwd(box,iter);}

  double CtcQInterAffCircle::err_compute(int iter, int k, const  IntervalVector& box, Affine2& af)
  {

    double err1=0;
    double err2=0;
    double err3=0;
    for (int j=0; j<nb_var-1; j++){
      err1 += ((std::pow(box[j].ub(),2) + std::pow(box[j].lb(),2))/2 - std::pow(box[j].mid(),2))/2;
      err2 += (sqr(measure[iter][j][k]).diam())/2;
      err3 += measure[iter][j][k].diam() * box[j].diam() /2;
      
    }
    return epseq + err1 +err2+err3;
  }

  double CtcQInterAffCircle::valmean_compute(int iter, int k, const IntervalVector& box, Affine2& af)
  { double valmean = 0;
    for (int j =0; j< nb_var-1; j++)
      {
	double err1 = ((std::pow(box[j].ub(),2) + std::pow(box[j].lb(),2))/2 - std::pow(box[j].mid(),2))/2;
	//	valmean+=std::pow(box[j].mid(),2) + std::pow(measure[iter][j][k].mid(),2)+ err1 -2 *measure[iter][j][k].mid() * box[j].mid() ;
	valmean+=std::pow(box[j].mid(),2) + sqr(measure[iter][j][k]).mid()+ err1 -2 *measure[iter][j][k].mid() * box[j].mid() ;
      }
    return valmean;
  }
 
  double CtcQInterAffCircle::slope_compute(int iter, int j, int k, const IntervalVector& box, Affine2& af)
  {  return (
	     (std::pow(box[j].ub(),2) - std::pow(box[j].lb(),2)) / box[j].diam() - 2*(measure[iter][j][k].mid())
	     );
}
 void CtcQInterAffCircle::compute_affine_evaluation (int i , int iter, Affine2& af, Interval & af2)
  {;}

 
 CtcQInterAffCircle::CtcQInterAffCircle(int n, const Array<Ctc>& ctc_list, Interval*** measure, double epseq, int q,  qintermethod meth) :
   CtcQInter(n,ctc_list,q,meth),
   CtcQInterCircle(n,ctc_list,measure,epseq, q,meth),
   CtcQInterAff(n,ctc_list,q,meth) {;}

  int  CtcQInterAffCircle::affine_threshold ()  {return 10;}
  //int  CtcQInterAffCircle::affine_threshold ()  { cout << "thr" << endl;return RAND_MAX;}


}

