//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor specialized for essential matrix  modelization with Sampson criterion
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 2, 2015
// Last Update : May 31, 2015
//============================================================================

using namespace std;
#include "ibex_CtcQInterSampson.h"
#include "ibex_Affine2.h"
#include "ibex_AffineEval.h"

namespace ibex {

  
  double CtcQInterSampson::compute_err_iter(IntervalVector & box, int iter){return 0;} 

  
  CtcQInterSampson::CtcQInterSampson(int n, const Array<Ctc>& ctc_list, double*** linfun,double** mfun1,
				   double epseq, int q, qintermethod meth) : 
    CtcQInter(n,ctc_list,q,meth),linfun(linfun),mfun1(mfun1),
    epseq(epseq) {eps= Interval(-epseq,epseq); eps0=Interval(0,epseq);
    init();
  }
  
 
  

 
  int CtcQInterSampson::effective_size(const IntervalVector & box) {return box.size();}

  Interval CtcQInterSampson::eval_dist(const IntervalVector & box, int iter, int k)
  {
    int nbvar=effective_size(box);
    Interval evald= linfun[iter][0][k] * box[nbvar-1];

    for (int i=0; i< nbvar-1; i++)
      {//cout << i << " : " << linfun[iter][i+1][k] << "  " << box[i] << "   " << linfun[iter][i+1][k]*box[i] << " evald " << evald <<endl;
      evald+=linfun[iter][i+1][k]*box[i];
      }
    //    cout << "iter " << iter << " " << evald  <<  " " << evald * evald << endl; 

    return evald;
  }
  
  double CtcQInterSampson::mideval_dist(const Vector & box, int iter, int k)
  {
    int nbvar=9;
    double evald= linfun[iter][0][k] * box[nbvar-1];

    for (int i=0; i< nbvar-1; i++)
      {//cout << i << " : " << linfun[iter][i+1][k] << "  " << box[i] << "   " << linfun[iter][i+1][k]*box[i] << " evald " << evald <<endl;
      evald+=linfun[iter][i+1][k]*box[i];
      }
    //    cout << "iter " << iter << " " << evald  <<  " " << evald * evald << endl; 

    return evald;
  }
  
  double    CtcQInterSampson::max_diam_threshold(const IntervalVector& box) {return box.max_diam();}

void CtcQInterSampson::point_contract_exact(IntervalVector & box, int iter)
 {
   //   cout << "iter " << iter << " box  " << box << endl;
   point_contract(box,iter);
  }
  
  // returns the number of active measurements at parameter point vec
int CtcQInterSampson::midactivepoints_count(const Vector& vec){
  int p=0;
    //    cout << " points size active count " << points->size() << " box " << box <<endl;
  list<int>::iterator iter = points->begin() ;
  
  while (iter != points->end()){
     
    if (midpoint_eval(vec,*iter)) p++;
    
    iter++;
  }


    return p;
 }

  /*
void CtcQInterSampson::point_contract_exact(IntervalVector & box, int iter)
 {
   //   cout << "iter " << iter << " box  " << box << endl;
   Vector point(9);
   for (int i; i<9; i++)
     point[i]=box[i].mid();
   if (! midpoint_contract(point,iter)) box.set_empty();
  }
  */



  // fwd constraint check only  if box_maxdiam > 1,  else fwdbwd with generic ibex code, using functions.
  
void CtcQInterSampson::point_contract(IntervalVector & box, int iter)

{ //if (box_maxdiam > 0.1)
    //cout << "iter " << iter << " box  " << box << endl;
  if (box_maxdiam > 0.01)
  {
  if  (eval_ctc(box, iter, 0).is_empty())
    {box.set_empty();return;}
  }
  else
    CtcQInter::point_contract(box,  iter);    
}
  
  //  fwdbwd avec fonctions ibex 
  /*
void  CtcQInterSampson::point_contract(IntervalVector & box, int iter)
{ CtcQInter::point_contract(box,  iter);}
  */

bool CtcQInterSampson::midpoint_eval(const Vector & vec, int iter)
  	{ 

	  //	  cout << "iter " << iter << " box  " << box << endl;
	  return (mideval_ctc(vec, iter, 0));
	}

bool CtcQInterSampson::mideval_ctc(const Vector & vec, int iter, int k){
    return (mideval_sampson(vec ,iter,k) < epseq);  
  }

Interval CtcQInterSampson::eval_ctc(const IntervalVector & box, int iter, int k){
    return (eval_sampson(box,iter,k) & eps0);  
  }


 Interval CtcQInterSampson::eval_sampson(const IntervalVector & box, int i, int k){
   Interval errdist = sqr(eval_dist(box, i, 0));
   Interval errxc = box[0] * mfun1[i][0] + box[1] * mfun1[i][2] + box[2]*mfun1[i][8];
   Interval erryc = box[3] * mfun1[i][4] + box[4] * mfun1[i][6] + box[5]*mfun1[i][9];
   Interval rx= box[0]* mfun1[i][1] + box[3] *  mfun1[i][3] + box[6]*mfun1[i][8];
   Interval ry= box[1]* mfun1[i][5] + box[4] *  mfun1[i][7] + box[7]*mfun1[i][9];
   Interval err_sampson= errdist / (sqr(errxc) + sqr(erryc) + sqr(rx)+ sqr(ry));
   //    cout << box << " " << i << " errdist " << errdist << " err_sampson " << err_sampson << endl;
   return err_sampson;
 }


  /*  punctual evaluation : for valid points */
double CtcQInterSampson::mideval_sampson(const Vector & box, int i, int k){
   double errdist = (mideval_dist(box, i, 0));
   double errxc = box[0] * mfun1[i][0] + box[1] * mfun1[i][2] + box[2]*mfun1[i][8];
   double erryc = box[3] * mfun1[i][4] + box[4] * mfun1[i][6] + box[5]*mfun1[i][9];
   double rx= box[0]* mfun1[i][1] + box[3] *  mfun1[i][3] + box[6]*mfun1[i][8];
   double ry=  box[1]* mfun1[i][5] + box[4] *  mfun1[i][7] + box[7]*mfun1[i][9];
   double err_sampson= errdist*errdist / (errxc*errxc + erryc*erryc + rx*rx + ry*ry);
    //    cout << box << " " << i << " errdist " << errdist << " err_sampson " << err_sampson << endl;
   return err_sampson;
 }


  // fwdbwd not implemented : call to the generic one
  void CtcQInterSampson::point_fwdbwd(IntervalVector & box, int iter){
    CtcQInter::point_fwdbwd(box,iter);
  }

  // call to generic qinter measure contraction (  generic ibex contraction ; fwdbwd)
  /*  
  void CtcQInterSampson::point_contract(IntervalVector & box, int iter){
    CtcQInter::point_contract(box,iter);
  }
  */

  // call to fwd contraction defined in  CtcQInterLinear
  /*
 void CtcQInterSampson::point_contract(IntervalVector & box, int iter){
   CtcQInterLinear::point_contract(box,iter);
 }
  */

  CtcQInterAffSampson::CtcQInterAffSampson(int n, const Array<Ctc>& ctc_list, double*** linfun, double** linfun1, Function *** mfun, double epseq, int q, qintermethod meth ) :
    CtcQInter(n,ctc_list,q,meth),
    CtcQInterSampson (n,ctc_list,linfun,linfun1,epseq,q,meth),
    CtcQInterAff(n,ctc_list,q,mfun,meth)
    {}

  int CtcQInterAffSampson::effective_size(const IntervalVector & box)
  {return 9;}
  int CtcQInterAffSampson::affine_threshold(){return 10;}
  //  int CtcQInterAffSampson::affine_threshold(){return INT_MAX;}
  /*
void CtcQInterAffSampson::compute_affine_evaluation( int k, int iter,  Affine2& af, Interval& af2) {
  
  Variable v(10);
  Function f1 (v,1/(sqr(v[1])+sqr(v[2])));
  Variable w(9);
  Function f2 (w, sqr(linfun[iter][0][0] * w[8]+ linfun[iter][1][0] * w[0] + 
					  linfun[iter][2][0] * w[1]+	linfun[iter][3][0] * w[2]  +
					  linfun[iter][4][0] * w[3]+	linfun[iter][5][0] * w[4]  +
					  linfun[iter][6][0] * w[5]+	linfun[iter][7][0] * w[6]  +
					  linfun[iter][8][0] * w[7])
				   /
				   (sqr (w[0]* mfun1[iter][0] +w[1] * mfun1[iter][2] + w[2]*mfun1[iter][8])
				    +sqr(w[3] * mfun1[iter][4] + w[4] * mfun1[iter][6] + w[5]*mfun1[iter][9])
				    + sqr(w[0]* mfun1[iter][1] + w[3] *  mfun1[iter][3] + w[6]*mfun1[iter][8])
				    +sqr (w[1]* mfun1[iter][5] + w[4] *  mfun1[iter][7] + w[7]*mfun1[iter][9])
				    ));

  //  cout <<  " compute_affine_evaluation " << endl;
  //  Affine2Eval affine_eval (*(function_list[0][iter]));
  cout << "boite " << (*boxes)[iter] << endl;
  Affine2Eval affine_eval (f2);
  Affine2Eval affine_eval1 (f1);
  af2=affine_eval.eval((*boxes)[iter]).i();
  cout << " eval affine af2 " << af2 << endl;
  af2=affine_eval1.eval((*boxes)[iter]).i();
  //Interval  af3=affine_eval.eval(vec).i();
  cout << " eval affine af1 " << af2 << endl;
  Interval ev1= f2.eval((*boxes)[iter]);
  cout << " inter eval " << ev1 << endl;

  af2=  eval_af[iter]->eval((*boxes)[iter]).i();
  cout << " eval affine af2 " << af2 << endl;
  //Interval ev1= function_list[0][iter]->eval((*boxes)[iter]);
  //  Interval ev1= function_list[0][iter]->eval(vec);

  //  af=eval_af[iter]->af2.top->i();
  af=affine_eval.af2.top->i();
    
  }
  */
}
