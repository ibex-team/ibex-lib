//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor  with projection in the average gradient direction of the measurement equation
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 30, 2012
// Last Update : Feb 4, 2015
//============================================================================


using namespace std;
#include "ibex_CtcQInterAff.h"
#include "ibex_QInter.h"
#include "ibex_Affine2.h"
#include "ibex_AffineEval.h"

#include "ibex_AffineProjectionException.h"
#include<vector>
#include<list>
#include<algorithm>

namespace ibex {
  CtcQInterAff::CtcQInterAff(int n, const Array<Ctc>& ctc_list, int q,  qintermethod meth) : CtcQInter(n,ctc_list,q,meth) {
      for(int i=0;i<nb_var-1;i++) aff_dir.push_back(0);
      pmax=ctc_list.size();

      init_interaff(false);

  }


  CtcQInterAff::CtcQInterAff(int n, int nb_obs1, const Array<Ctc>& ctc_list, int q,  qintermethod meth) : CtcQInter(n,nb_obs1,ctc_list,q,meth) {
      for(int i=0;i<nb_var-1;i++) aff_dir.push_back(0);
      pmax=nb_obs1;
      init_interaff(false);
    
  }

 

  CtcQInterAff::CtcQInterAff(int n, const Array<Ctc>& ctc_list, int q, Function*** funlist, qintermethod meth ) :  CtcQInter(n,ctc_list,q,meth),  function_list(funlist) {	for(int i=0;i<nb_var-1;i++) aff_dir.push_back(0);
    pmax=ctc_list.size(); init_interaff(true); }
  
  void CtcQInterAff::init_interaff(bool fun){
    funl=fun;
    interaf=new Interval[pmax];

    //    cout << " funl " << funl << endl;

    if (funl) {
      eval_af= new Affine2Eval*[pmax];
      for (int i=0; i<pmax; i++)
	eval_af[i]= new Affine2Eval (*(function_list[0][i]));



      alpha= new double**[pmax];
      for (int i=0; i<pmax; i++)
	{alpha[i] = new double*[nb_var];
	  for (int j=0; j<nb_var; j++)
	    alpha[i][j]= new double[kfun];
      }
      err= new double*[pmax];
    for (int i=0; i<pmax; i++)
      err[i]=new double[kfun];

    
    valmean= new double*[pmax];
    for (int i=0; i<pmax; i++)
      valmean[i]=new double[kfun];
    }
  }
  CtcQInterAff::~CtcQInterAff(){

    delete [] interaf;
    if (funl){

      for (int i=0; i<pmax; i++)
	{delete eval_af[i];}
      delete [] eval_af;
      for (int i=0; i<pmax; i++)
	{for (int j=0; j<nb_var; j++)
	    delete [] alpha[i][j];
	  delete [] alpha[i];
	}
    delete [] alpha;
    for (int i=0; i<pmax; i++)
      delete [] err[i];
    delete [] err;


    
    for (int i=0; i<pmax; i++)
      delete [] valmean[i];
    delete [] valmean;
    }

      
  }

  
  double CtcQInterAff::slope_compute(int iter, int j , int k, const IntervalVector& box,  Affine2& af)
  { return 2*af.val(j+1)/box[j].diam();}
  double CtcQInterAff::valmean_compute(int iter, int k, const IntervalVector& box,  Affine2& af)
  {return af.mid();}
  double CtcQInterAff::err_compute( int iter, int k, const IntervalVector& box, Affine2& af)
  {return af.err();}

 // compute the boxes corresponding to a projection in a direction computed as the average (or median)  of the coefficients of the affine 
  // formula  giving the last variable of box in function of the  others
  // This formula is computed by an affine evaluation .
  // This is also useful by adding a nth+1 variable in case of an implicit constraint f(x1,...,xn) =0
  // the last variable of each measurement box (*boxes)[iter][nb_var-1] is replaced by this projection


  void  CtcQInterAff::affine_projection(int& pp)  {
    for (int j=0; j<nb_var-1; j++) aff_dir[j]=0;
    
    int k=0;
    Affine2 af;
    //    vector<double> affineslope[n-1];   // variante médiane
    list<int>::iterator iter = points->begin() ; 
    int p1=0;

    Interval af2 (0.0,1.0);
    while (iter != points->end()){
      //      (*boxes)[*iter] &=box;  // ne sert à rien.
      if ((*boxes)[*iter].is_empty() ) {iter++;p1++;continue;}
      int sortie_ok=1;
      for (int i=0; i<kfun;i++){
	//	cout  << *iter << " box_iter " << (*boxes)[*iter] << endl;
	compute_affine_evaluation( i, *iter, af, af2);
	//	cout << " af is actif " << af.is_actif() << endl;
	//	cout << " af2 " << af2 << endl;
	
	//	cout << " afval " << af.val(0) << " " << af.val(1) << " " << af.val(2) << endl;
	// boxes[*iter][n-1]&=af2;  //inutile 
	//	cout << "apres compute_affine_evaluation " << (*boxes)[*iter][0] << endl;
       

	if (

	    af2.is_empty()
	    //	      boxes[*iter][n-1].is_empty()//inutile 
	    )
	  {//cout << " af2 empty " << endl;
	    //iter=points->erase(iter);
	    if (!((*boxes)[*iter].is_empty()))
	      {(*boxes)[*iter].set_empty();pp--;}
	    iter++;sortie_ok=0;p1++;
	    break;}  // evaluation affine vide exemple sqrt nb négatif
      
	for (int j =0; j< nb_var-1; j++) {
	  if ((*boxes)[*iter][j].diam() < 1.e-20 || (funl && !(af.is_actif()))) throw AffineProjectionException();
          double slope;
	  //	  cout << "funl " << funl << endl;
	  if (funl) {alpha[p1][j][i]= slope_compute(*iter,j,i, (*boxes)[*iter], af);
	    slope=alpha[p1][j][i];
	    //	    cout << p1 << " " << j  << " " << i << " slope " << slope << endl;
	  }
	  else slope = slope_compute(*iter,j,i, (*boxes)[*iter], af);
	  //	  cout << " slope "  << *iter << " " << j << "  " << slope << "  box " << (*boxes)[*iter] << endl;
	  //	    affineslope[j].push_back(alpha[*iter][j][i]);    // variante médiane
	  aff_dir[j] = (aff_dir[j]*k + slope) /(k+1);  // calcul incrémental de la pente moyenne
	}
	if (funl) {
	  valmean[p1][i]=valmean_compute(*iter,i, (*boxes)[*iter], af);
	  err[p1][i]=err_compute(*iter,i, (*boxes)[*iter], af);
	}
	k++;
      }
      if (sortie_ok) {iter++;p1++;}
    }
    
    // essai pente random : moins bon
    /*
    int pr =  rand() % p1; 
    for (int j =0; j< nb_var-1; j++) 
      aff_dir[j]= alpha[pr][j][0];
    */

    // essai : utiliser pente médiane au lieu de pente moyenne : trop cher à cause du sort
    /*
    for (int j =0; j< n-1; j++)
      {sort(affineslope[j].begin(),affineslope[j].end());
	aff_dir[j]= affineslope[j][affineslope[j].size()/2];
      }
    */

    // calcul de l'intervalle dans la direction à projeter  aff_dir
    
    /*
    for (int j=0; j<nb_var-1 ; j++)
      cout << j << " affdir  " << aff_dir[j] << endl;
    cout << "p " << pp << endl;
    */
    iter = points->begin() ; p1=0;
    while (iter != points->end()){
      if ((*boxes)[*iter].is_empty() ) {iter++;p1++;continue;}
      for (int i=0; i<kfun;i++){
	double errj=0;double valmeanj=0;
	  for (int j =0; j< nb_var-1; j++){
	    double alpha1;
	    if (funl) alpha1= alpha[p1][j][i]-aff_dir[j];
	    else alpha1= slope_compute(*iter,j,i, (*boxes)[*iter], af) - aff_dir[j];

	    //	    cout << *iter << " alpha1 " << "  " << alpha1 << endl;
	    errj +=fabs(alpha1)*(*boxes)[*iter][j].diam();
	    
	    valmeanj +=aff_dir[j] *  (*boxes)[*iter][j].mid();
	    //	    cout << j << " " << "affdir " <<aff_dir[j] << " " << " midbox " << (*boxes)[*iter][j].mid() << endl;
	  }
	  //	  cout <<  " valmeanj " << valmeanj << endl;
	  double valmean1;
	  if (funl) valmean1  = valmean[p1][i] - valmeanj;
	  else valmean1= valmean_compute(*iter,i, (*boxes)[*iter], af) - valmeanj;

	  double errfun;
	  if (funl) errfun= err[p1][i];
	  else
	    errfun= err_compute(*iter,i, (*boxes)[*iter],af);
	  double err1=2*errfun+errj;
	  //	  cout << " valmean1 " << valmean1 << " err1 " << err1 << endl;
	  if (i==0)
	    interaf[p1]= Interval(valmean1-(err1/2), valmean1+(err1/2));
	  else
	    interaf[p1]&= Interval(valmean1-(err1/2), valmean1+(err1/2));
	  //	  cout << *iter << "  " << interaf[p1] << endl;
	  //	  cout << (*boxes)[*iter][0].lb() << " " << (*boxes)[*iter][0].ub() <<  " " << valmean1 << " " <<   "  "  << err1 << endl;

      }
      if (interaf[p1].is_empty()){ // cas se produisant uniquement en cas de mesure multiple (kfun >1)
	//iter=points->erase(iter);
	if (!( (*boxes)[*iter].is_empty())) {(*boxes)[*iter].set_empty();pp--;}
	iter++;p1++;}
      else
	{(*boxes)[*iter][nb_var-1]=interaf[p1]; 
	  //	  cout << *iter << (*boxes)[*iter][nb_var-1]  << endl;
	  iter++;p1++;
	}
    }


  }

  int CtcQInterAff::affine_threshold(){
        return INT_MAX;
    //    return 10;
  }


void CtcQInterAff::contract(IntervalVector& box) {
  //cout << " avant ctc_contract  "<< box << endl;
  int p=ctc_contract(box);  // contraction of all the measurements in current box (depending on the pb : fwd or fwdbwd)
  //cout << " p " << p << endl;
  int gap=affine_threshold();

  if (box.is_empty()) return;
  if (p<q) {box.set_empty();return;}

  // qinter on the directions of the original box only if p-q < gap
  //    cout << " avant direct proj " << box << endl;
  if (p-q < gap || !(side_effects())) { direct_qintercontract(p, box);} 

  //    cout << " apres direct proj " << box << endl;
  if (box.is_empty()) return;

  int  p0=p;

  // la projection affine n'est appelée que par la contraction directe (pas dans 3bcid)
  
  if (
      side_effects()  
      )
     {
     
    try {

      affine_projection (p);


    //    p0=p;
    //    int res2=1;


    // while(res2){
    // res2=0;
    // p updated by previous call to qinter_projf
    //    cout << " iter proj affine " << p << endl;
      int n0=nb_var-1;
      int n1=nb_var;
      IntervalVector box1 = qinter_contract(p,n0,n1);
    //    cout << " apres 2me proj" << " p " << p << " " << box << "   " << box1 << endl;
      if (box1.is_empty()) {box.set_empty();return;}
      for (int i=n0; i<n1;i++) {if (box1[i].is_empty()) {box.set_empty(); return;}}
      if (p<q) {box.set_empty();return;}
    //    cout << box1[0].lb() << " " << box1[0].ub() << "  " << box1[1].mid() <<  " "<<  0 << " " << box1[1].diam() << endl;
      Interval res1;
      Interval resi(0,0);
      // the n-1 first directions
      for (int i=n0; i<nb_var-1;i++)
	box[i]&=box1[i];
      // projecting the result of the new nth direction (the mean gradient) on the old nth direction 
      for (int i=0;i<nb_var-1;i++)
	resi+=aff_dir[i]*box[i];
      res1=box1[nb_var-1] + resi;
      //      IntervalVector initbox(n);
      //      initbox=box;
      //    cout << box[0].lb() << " " << box[0].ub() << "  " << res1.mid() <<  " "<<  0 << " " << res1.diam() << endl;
      box[nb_var-1]&=res1;
      for (int i=n0; i<nb_var;i++) {
	if (box[i].is_empty())  { box.set_empty(); return;}
      }
      
    //      if (initbox.rel_distance(box) > 0.1) res2=1;
      
    // updating points only in case of direct contraction call (not in  cid) using  side_effects
      

      if ( p<p0 &&   side_effects ()) 
	//	updateinterpoints(box1);  // inutile ?
	updatepoints();

    //    cout << " fin contract proj affine " <<  box << endl;
    }
    catch (AffineProjectionException err) {
      //cout << " erreur proj affine " << endl;
    }
      //      if (points->size()<q) {box.set_empty();}
     }
      
      
  

}


void CtcQInterAff::compute_affine_evaluation( int k, int iter,  Affine2& af, Interval& af2) {
 af2=  eval_af[iter]->eval((*boxes)[iter]).i();
 af=eval_af[iter]->af2.top->i();
 if (!(af.is_actif())) throw AffineProjectionException();
}

}
