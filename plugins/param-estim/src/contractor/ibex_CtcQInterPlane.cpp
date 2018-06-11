//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor specialized for plane detection 
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 6, 2015
// Last Update : Feb 6, 2015
//============================================================================

using namespace std;
#include "ibex_CtcQInterPlane.h"
#include "ibex_Affine2.h"

// can be used with 2 modelizations that have a linear constraint : 
// the classic one (ax+by+cz+d=eps) with a^2+b^2+c^2=0   nb_var=4
// and with the modelization   (1-b-c)x + by + cz + d  explained in ictai paper     nb_var=3

namespace ibex {


  int CtcQInterPlane::original_obs (int q1) {
    if (kfun==1) return q1;
    if (q1 <= 4) return q1;
    else if (q1 < 10) return 4;
    else if (q1 < 20) return 5;
    else if (q1 < 35) return 6;
    else if (q1 < 56) return 7;
    else if (q1 < 84) return 8;
    else if (q1 < 120) return 9;
    else if (q1 <  165) return 10;
    else if (q1 < 220) return 11;
    else if (q1 < 286) return 12;
    else if (q1 < 455) return 14;

    else return 1 + (int) std::pow(6*q1, 0.33333);
    
  }
 




  /*
Interval CtcQInterPlane::fwd(IntervalVector & box, int iter)
  {
    Interval eps (-epseq,epseq);1
    Interval evald= eps+linfun[iter][0][0];
    
    for (int j=0;j<nb_var-1;j++)
      evald+=linfun[iter][j+1][0]*box[j];
    return evald;
  }
  
  */
 // the evaluation of the iterth equation with uncertainties
  // to check if the iterth measurement is valid (0 must be in the returned interval)

  
  Interval CtcQInterPlane::eval_ctc(IntervalVector & box, int iter, int k){
    return (eval_dist(box,iter,k) + eps);
  }
    
  
  /*
  Interval CtcQInterPlane::eval_dist(IntervalVector & box, int iter, int k)
  {
    if (kfun ==1) {
    Interval evald (linfun[iter][0][k]);
    for (int j=0;j<nb_var-1;j++)
      evald+=linfun[iter][j+1][k]*box[j];
    return evald; 
    }
    else{
      Interval evald (linfun[index[iter][k]][0][0]);
      for (int j=0;j<nb_var-1;j++)
	evald+=linfun[index[iter][k]][j+1][0]*box[j];
      return evald;
    }
}
  */
Interval CtcQInterPlane::eval_dist(IntervalVector & box, int iter, int k)
  {
    if (kfun ==1) {
    Interval evald (linfun[iter][0]);
    for (int j=0;j<nb_var-1;j++)
      evald+=linfun[iter][j+1]*box[j];
    return evald; 
    }
    else{
      Interval evald (linfun[index[iter][k]][0]);
      for (int j=0;j<nb_var-1;j++)
	evald+=linfun[index[iter][k]][j+1]*box[j];
      return evald;
    }


  }


double CtcQInterPlane::eval_dist_mid(Vector & vec, int iter, int k)
  {
    double evald= linfun[iter][0];
    for (int j=0;j<nb_var-1;j++)
      evald+=linfun[iter][j+1]*vec[j];
    return evald; 
  }




  // the individual error at the middle of box for the membership equation of the iterth point to be on the
  // plane 
  // property : it is less than epseq  :  only implemented for kfun=1 
  

 double CtcQInterPlane::compute_err_iter(Vector & mid, int iter){
 if (kfun==1){
    double res=  eval_dist_mid(mid,iter,0) - mid[nb_var-1];
    //    cout << " " << iter << " " << res << endl;
    return fabs(res);}
 else return 0;
  }


  // for Plane constraint, simple forward evaluation of the constraint (fwdbwd too expensive)  
  void CtcQInterPlane::point_contract(IntervalVector & box, int iter)
  {
    //    cout << " iter " << iter ;
    //    cout << " box " << box << " boxiter " << (*boxesini)[iter] << endl;
    if (kfun > 1) {box &= (*boxesini)[iter];return;}
    else
      /*      for (int k=0; k<kfun; k++)
	{ 
	  box[nb_var-1]&=eval_ctc(box, iter, k);
	  if  (box[nb_var-1].is_empty())
	    {box.set_empty();return;}
	}

      */
      
      { 
	//	  box[nb_var-1]&=eval_ctc(box, iter, 0);
	box[nb_var-1]&=eval_dist(box, iter, 0) + eps;
	  if  (box[nb_var-1].is_empty())
	    {box.set_empty();return;}

      }
      
      //      fwdbwd (box, iter);
  }
  
  // for computing the validated measurements  box is a small box around a validating point
  // in case of d-measures, we check that 
void CtcQInterPlane::point_contract_exact(IntervalVector & box, int iter)
 {
    if (kfun > 1) box &= (*boxesini)[iter];
    if (box.is_empty()) return;
    for (int k=0; k<kfun; k++)
	{ 
	  box[nb_var-1]&=eval_ctc(box, iter, k);
	  if  (box[nb_var-1].is_empty())
	    {box.set_empty();return;}
	}
  }



/* fwdbwd for the linear constraint  box[n]= linfun[iter][1][k]* box[0] + linfun[iter][2][k]* box[1]  + ...  
for n=2,3,4 */
/*
  void CtcQInterAffPlane::bwd1(IntervalVector & box, int iter){
    int k=0;
    if (nb_var==2)
      {box[0]&=(eps +box[nb_var-1]- linfun[iter][0][k])/ (linfun[iter][1][k]-aff_dir[0]);
	    if  (box[0].is_empty())
	      {box.set_empty();return;}
      }
    else if (nb_var==3)
      { Interval ev=eps + box[nb_var-1]-  linfun[iter][0][k] ;

	    box[0]&=(ev - (linfun[iter][2][k]-aff_dir[1])*box[1])/(linfun[iter][1][k]-aff_dir[0]);
	    if  (box[0].is_empty())
	  {box.set_empty();return;}

	    box[1]&=(ev- (linfun[iter][1][k] - aff_dir[0])*box[0])/(linfun[iter][2][k]-aff_dir[1]);
	    if  (box[1].is_empty())
	      {box.set_empty();return;}
	  }
    else if (nb_var==4)
	  {box[0]&=(eps +box[nb_var-1] -linfun[iter][0][k]- (linfun[iter][2][k]-aff_dir[1])*box[1] -(linfun[iter][3][k]-aff_dir[2])*box[2] )/(linfun[iter][1][k]-aff_dir[0]);
	    if  (box[0].is_empty())
	      {box.set_empty();return;}

	    box[1]&=(eps+ box[nb_var-1] -linfun[iter][0][k]- (linfun[iter][1][k] - aff_dir[0])*box[0] - (linfun[iter][3][k]-aff_dir[2])*box[2])/(linfun[iter][2][k]-aff_dir[1]);
	    if  (box[1].is_empty())
	      {box.set_empty();return;}
	  
	    box[2]&=(eps +box[nb_var-1] -linfun[iter][0][k]- (linfun[iter][1][k]-aff_dir[0])*box[0] - (linfun[iter][2][k]-aff_dir[1])*box[1])/(linfun[iter][3][k]-aff_dir[2]);
		     if  (box[2].is_empty())
		       {box.set_empty();return;}
	  }
  }
*/


 void CtcQInterPlane::fwdbwd(IntervalVector & box, int iter)
  { //cout << " contract planeprojf " << endl;
    int indexa=1;
    //    IntervalVector old_box(n);
    //    Interval eps (-epseq,epseq);
    while (indexa) {
      //old_box=box;

      for (int k=0; k<kfun; k++)
	{  


	  //	Interval evald= eps+linfun[iter][0][k];

	  //	for (int j=0;j<nb_var-1;j++)
	  //	  evald+=linfun[iter][j+1][k]*box[j];
	  box[nb_var-1]&=eval_ctc(box,iter,k);
	if  (box[nb_var-1].is_empty())
	  {box.set_empty();return;}

        if (nb_var==2)
	  {box[0]&=(eps +box[nb_var-1]- linfun[iter][0])/linfun[iter][1];
	    if  (box[0].is_empty())
	      {box.set_empty();return;}
	  }
	else if (nb_var==3)
	  { 
	    box[0]&=(eps + box[nb_var-1]-  linfun[iter][0] - linfun[iter][2]*box[1])/linfun[iter][1];
	    if  (box[0].is_empty())
	  {box.set_empty();return;}

	    box[1]&=(eps + box[nb_var-1]-  linfun[iter][0] - linfun[iter][1]*box[0])/linfun[iter][2];
	    if  (box[1].is_empty())
	      {box.set_empty();return;}
	  }
	else if (nb_var==4)
	  {box[0]&=(eps +box[nb_var-1] -linfun[iter][0]- linfun[iter][2]*box[1] -linfun[iter][3]*box[2] )/linfun[iter][1];
	    if  (box[0].is_empty())
	      {box.set_empty();return;}

	    box[1]&=(eps+ box[nb_var-1] -linfun[iter][0]- linfun[iter][1]*box[0] - linfun[iter][3]*box[2])/linfun[iter][2];
	    if  (box[1].is_empty())
	      {box.set_empty();return;}
	  
	    box[2]&=(eps +box[nb_var-1] -linfun[iter][0]- linfun[iter][1]*box[0] - linfun[iter][2]*box[1])/linfun[iter][3];
	    if  (box[2].is_empty())
	      {box.set_empty();return;}
	  }
	}
      indexa=0;
      /*
      if (kfun==1) 
	index=0;
      else if (old_box.rel_distance(box) < 0.1)
	index=0;
      */
      //      cout << " box " << box << endl;
    }
  }
 


  /*
void CtcQInterPlane::init ()
  {
    points= new list<int> (); 
    boxes= new IntervalMatrix(nb_obs,nb_var);
    for(int i=0;i<nb_obs;i++) {points->push_back(i);}
    _side_effects=false;
    points_to_delete=true;
    qmax=nb_obs;
    dim=nb_var;
  }
  */


  CtcQInterPlane::CtcQInterPlane(int n,int nb_obs, const Array<Ctc>& ctc_list, double** linfun, 
				 double epseq, int q, qintermethod meth) : 
    CtcQInter(n,nb_obs,ctc_list,q,meth),linfun(linfun),
    epseq(epseq) {eps= Interval(-epseq,epseq);
    // init();
  }

  CtcQInterPlane::CtcQInterPlane(int n, int nb_obs, const Array<Ctc>& ctc_list, double** linfun, int** index, IntervalMatrix * boxesini,
				 double epseq, int q, qintermethod meth) : 
    CtcQInter(n,nb_obs,ctc_list,q,meth),linfun(linfun),
    epseq(epseq), boxesini(boxesini), index(index) {eps= Interval(-epseq,epseq);
    // init();
  }


 bool CtcQInterPlane::stopping_condition (int q1, int q2){
   if (kfun==1) 
     return CtcQInter::stopping_condition(q1,q2);
   else{
     //     if (q2 >= q && original_obs(q1- q2)  < original_obs(q))
     //         cout << " q1 " << q1 << " q2 " << q2 <<   " ori2 " <<  original_obs(q1-q2)  << " oriq " <<  original_obs(q) << endl;
     
     return (q2 >= q && original_obs(q1 - q2) < original_obs(q));
   }
 }

 

  IntervalVector CtcQInterPlane::randomvalidpoint( IntervalVector & box){
    if (kfun==1) return CtcQInter::randomvalidpoint(box);
    else
      {int p = points->size();
	int r= rand()%p;
	 int index1[dim];
      list<int>::iterator iter=points->begin();
      int k=0; int kk=0;
      while (iter != points->end()){
	if (r==k) {kk=*iter; break;}
	iter++;k++;
      }
      index1[0]=index[kk][0];
      index1[1]= index[kk][1];   
      index1[2]= index[kk][2];   
      


      return  pointintersection(box,index1);
      }      
  }

  // solving the intersection of 3 thick linear constraints by an interval Gaussian elimination 
  
  /*
 IntervalVector CtcQInterPlane::randomvalidpoint( IntervalVector & box)
 {if (nb_var==3)
  {int i,j,k;
    int p=points->size();
     i = rand() % p;
     j = rand() % (p-1);
     k = rand() % (p-2);
     if (j>=i) j=j+1;
     if (k >=j && k >= i) k=k+2;
     else
       {if (k >=j || k>=i ) k=k+1;
	 if (k==i || k==j) k=k+1;
       }
     int iter1,iter2,iter3;
     list<int>::iterator iter=points->begin();
     int nbfound=0; int kk=0;
     while (iter != points->end()){
       //cout << kk << " " << *iter << endl;
       if (kk==i) {iter1=*iter; nbfound++;}
       else if (kk==j) {iter2=*iter;nbfound++;}
       else if (kk==k) {iter3=*iter;nbfound++;}
       if (nbfound==3) break;
       iter++; kk++;
     }
     //     cout << " iter 1 2 3 " << iter1 << " " << iter2 << " " << iter3 << endl;
     return  box=boxintersection(box,iter1,iter2,iter3);
  }
   
   else CtcQInter::randomvalidpoint(box);
  
//   else {
//     IntervalVector box1(nb_var);
//     box1.set_empty();
//     return box1;
//   }
  
 }
  */

 bool  CtcQInterPlane::contract_with_outliers (IntervalVector& box, int gap)  {
 if (nb_var==3) {

    int outliers=0;
    list<int>::iterator iter = points->begin();
    /*
    int depart = rand()%3;
    for (int i=0; i< depart; i++) iter++;
    */
    IntervalVector box1=box;
    int indexa[3];
    while (iter != points->end()){

      int k=0;
      while (k<3 && iter!= points->end())
	{indexa[k]=*iter; k++; iter++;}
      //      cout << " cwo : box avant " << box <<  " " << index[0] << " " <<index[1] << " " <<  index[2] << endl;
      if (iter == points->end()) break;
      //    cout << " box avant " << box << endl;
      //      box1= 
      boxintersection(box1,indexa[0],indexa[1],indexa[2]);

    
      if (box1.is_empty() ) {
	outliers++; 
	if (outliers== gap+1) 
	  return true;
	else {box1=box; continue;}
      }
      /*
      else{
	list<int>::iterator iter1 = iter;
	//	while (iter1 != points->end()){
	if (iter1 != points->end()){
	//	CtcQInter::point_contract(box1,*iter);   // pour faire un vrai fwdbwd  (fwd seul ne sert à rien)
	  point_fwdbwd(box1,*iter);   // pour faire un vrai fwdbwd  (fwd seul ne sert à rien)
      //      cout << " box " << box << endl;

	  iter++; iter1++;
	  if (box1.is_empty()) {
	    outliers++; 
	    if (outliers== gap+1) 
	      return true;
	    //  else  {box1=box; break;}
	    else  {box1=box;}
	  }
	}

      }
      */
    }
 
    return false;
 }
 else 
  return   CtcQInter::contract_with_outliers(box, gap);
 }

  void CtcQInterPlane::ctc_contract_all(IntervalVector& box){
    
    if (nb_var==3) {
      //      cout << " contract_all " << endl;
      int nbp = points->size();

      list<int>::iterator iter = points->begin() ;
      int indexa[3];
      
      int nbk = nbp/3;
      //int nbk = 1;
      for (int j=0; j< nbk; j++){
	int k=0;      
	while (k<3)
	  {indexa[k]=*iter; k++; iter++;}
	//	cout << " box avant " << box <<  " " << index[0] << " " <<index[1] << " " <<  index[2] << endl;
	//	box= boxintersection(box,indexa[0],indexa[1],indexa[2]);
	boxintersection(box,indexa[0],indexa[1],indexa[2]);
      }
      //    cout << " box apres " << box << endl;
      
      while (iter != points->end()){
	CtcQInter::point_contract(box,*iter);
	iter++;
      }
      //        cout << " box apres ctc " << box << endl;
    }
    else 
    
      CtcQInter::ctc_contract_all(box);
  }
	  
  /*
IntervalVector CtcQInterPlane::boxintersection(int* iter){
    if (nb_var==3)
      return boxintersection(iter[0], iter[1], iter[2]);
    else
      return CtcQInter::boxintersection( iter);
  }
  */

  IntervalVector CtcQInterPlane::pointintersection(IntervalVector & box, int* iter){
    if (nb_var==3)
      return pointintersection(iter[0], iter[1], iter[2]);
    else
      return pointintersection_abcd( box, iter);
  }


  // TO DO : solving the equation system 4x4 with 3 measurements + norm constraint
  IntervalVector CtcQInterPlane::pointintersection_abcd(IntervalVector & box, int* iter){
    //    cout << " box " << box <<endl;
    return CtcQInter::boxintersection(box, iter);}   

  /*
  Vector CtcQInterPlane::pointintersection(int* iter){
    if (nb_var==3)
      return pointintersection(iter[0], iter[1], iter[2]);
    else
      return CtcQInter::pointintersection( iter);
  }
  */
  
  IntervalVector CtcQInterPlane::boxintersection( IntervalVector& box, int* iter){
    if (nb_var==3){
      //      return boxintersection( box, iter[0], iter[1], iter[2]);
       boxintersection( box, iter[0], iter[1], iter[2]);
       IntervalVector box1(box);
       return box1;
    }
    else
      
      return CtcQInter::boxintersection(box, iter);
  }
  

  
  IntervalVector CtcQInterPlane::validpoint( IntervalVector& box) {
    //  cout << "valid point " << endl;
    if (nb_var==3)
      return CtcQInter::validpoint(box);
    else   // model  a b c d   with a^2 + b^2 + c^2 = 1
{
      Vector mid1 (box.mid());
      mid1[0] = std::sqrt (1 -  std::pow(mid1[1],2) -  std::pow(mid1[2],2));
      IntervalVector mid (mid1);
      if (!(box.contains(mid1)))

 
	mid.set_empty();
      //      cout << " mid " << mid << endl;
      return mid;
    }
  }
  
      

  /*
IntervalVector CtcQInterPlane::boxintersection( IntervalVector box, int* iter){
  return CtcQInter::boxintersection(box, iter);}
  */			      


  // used by randomvalidpoints : the intersection with the current box is not done
  /*
  IntervalVector CtcQInterPlane::boxintersection( int iter1, int iter2, int iter3){
   
    Interval a1,a2,a3,b1,b2,b3,d1,d2,d3; //,denom;
  a1=linfun[iter1][1][0];
  a2=linfun[iter2][1][0];
  a3=linfun[iter3][1][0];
  b1=linfun[iter1][2][0];
  b2=linfun[iter2][2][0];
  b3=linfun[iter3][2][0];
  d1=Interval(-epseq,epseq)+linfun[iter1][0][0];
  d2=Interval(-epseq,epseq)+linfun[iter2][0][0];
  d3=Interval(-epseq,epseq)+linfun[iter3][0][0];
  //  denom= ((a1-a2) *(b1-b3) - (a1-a3)* (b1-b2));
  Interval x1,y1,z1;
    x1= ((d1-d2)*(b1-b3) - (d1-d3)*(b1-b2) )/ ((a1-a2) *(b1-b3) - (a1-a3)* (b1-b2));
    y1= ( (d1-d3)*(a1-a2) -(d1-d2)*(a1-a3)  )/ ((a1-a2) *(b1-b3) - (a1-a3)* (b1-b2));
  //  x1= ((d1-d2)*(b1-b3) - (d1-d3)*(b1-b2) )/ denom;
  //  y1= ( (d1-d3)*(a1-a2) -(d1-d2)*(a1-a3)  )/ denom;
  z1=d1 - a1*x1 - b1*y1;
  
  
  IntervalVector box1 (3);
  box1[0] =-x1;
  box1[1] =-y1;
  box1[2] = z1;
  
  //  cout <<  " box1 " << box1 << endl;
  if (box1[0].is_empty()) box1.set_empty();
  else if (box1[1].is_empty()) box1.set_empty();
  else if (box1[2].is_empty()) box1.set_empty();
  return box1;
  }

  */


 IntervalVector CtcQInterPlane::pointintersection( int iter1, int iter2, int iter3){
   
    Interval a1,a2,a3,b1,b2,b3,d1,d2,d3; //,denom;
  a1=linfun[iter1][1];
  a2=linfun[iter2][1];
  a3=linfun[iter3][1];
  b1=linfun[iter1][2];
  b2=linfun[iter2][2];
  b3=linfun[iter3][2];
  d1=Interval(linfun[iter1][0]-epseq,linfun[iter1][0]+epseq);
  d2=Interval(linfun[iter2][0]-epseq,linfun[iter2][0]+epseq);
  d3=Interval(linfun[iter3][0],linfun[iter3][0]);
  //  denom= ((a1-a2) *(b1-b3) - (a1-a3)* (b1-b2));
  Interval x1,y1,z1;
    x1= ((d1-d2)*(b1-b3) - (d1-d3)*(b1-b2) )/ ((a1-a2) *(b1-b3) - (a1-a3)* (b1-b2));
    y1= ( (d1-d3)*(a1-a2) -(d1-d2)*(a1-a3)  )/ ((a1-a2) *(b1-b3) - (a1-a3)* (b1-b2));
  //  x1= ((d1-d2)*(b1-b3) - (d1-d3)*(b1-b2) )/ denom;
  //  y1= ( (d1-d3)*(a1-a2) -(d1-d2)*(a1-a3)  )/ denom;
  z1=d1 - a1*x1 - b1*y1;
  
  
  IntervalVector box1 (3);
  box1[0] =-x1;
  box1[1] =-y1;
  box1[2] = z1;
  
  //  cout <<  " box1 " << box1 << endl;
  if (box1[0].is_empty()) box1.set_empty();
  else if (box1[1].is_empty()) box1.set_empty();
  else if (box1[2].is_empty()) box1.set_empty();
  return box1;
  }
  
  /*
  Vector CtcQInterPlane::pointintersection( int iter1, int iter2, int iter3){
   
  double a1,a2,a3,b1,b2,b3,d1,d2,d3; //,denom;
  a1=linfun[iter1][1][0];
  a2=linfun[iter2][1][0];
  a3=linfun[iter3][1][0];
  b1=linfun[iter1][2][0];
  b2=linfun[iter2][2][0];
  b3=linfun[iter3][2][0];
  d1=linfun[iter1][0][0];
  d2=linfun[iter2][0][0];
  d3=linfun[iter3][0][0];
  //  denom= ((a1-a2) *(b1-b3) - (a1-a3)* (b1-b2));
  double x1,y1,z1;
    x1= ((d1-d2)*(b1-b3) - (d1-d3)*(b1-b2) )/ ((a1-a2) *(b1-b3) - (a1-a3)* (b1-b2));
    y1= ( (d1-d3)*(a1-a2) -(d1-d2)*(a1-a3)  )/ ((a1-a2) *(b1-b3) - (a1-a3)* (b1-b2));
  //  x1= ((d1-d2)*(b1-b3) - (d1-d3)*(b1-b2) )/ denom;
  //  y1= ( (d1-d3)*(a1-a2) -(d1-d2)*(a1-a3)  )/ denom;
  z1=d1 - a1*x1 - b1*y1;
  
  Vector  mid1 (3);
  mid1[0]=-x1;
  mid1[1]=-y1;
  mid1[2]=z1;
  return mid1;
  }

  */

// used by contract_all and contract_with_outliers : the intersection with the current box is  done
void CtcQInterPlane::boxintersection( IntervalVector& box, int iter1, int iter2, int iter3){
//IntervalVector CtcQInterPlane::boxintersection( IntervalVector& box, int iter1, int iter2, int iter3){
   
  double a1,a2,a3,b1,b2,b3;
  Interval d1,d2,d3;
  a1=linfun[iter1][1];
  a2=linfun[iter2][1];
  a3=linfun[iter3][1];
  b1=linfun[iter1][2];
  b2=linfun[iter2][2];
  b3=linfun[iter3][2];
  /*
  d1=Interval(-epseq,epseq)+linfun[iter1][0][0];
  d2=Interval(-epseq,epseq)+linfun[iter2][0][0];
  d3=Interval(-epseq,epseq)+linfun[iter3][0][0];
  */
  d1=eps+linfun[iter1][0];
  d2=eps+linfun[iter2][0];
  d3=eps+linfun[iter3][0];

  double c1= (a1-a2) *(b1-b3) - (a1-a3)* (b1-b2);

 
  
  Interval x1,y1,z1;
  x1= ((d1-d2)*(b1-b3) - (d1-d3)*(b1-b2) )/c1;
  box[0]&=-x1;
  if (box[0].is_empty()) {box.set_empty(); return;}

  y1= ( (d1-d3)*(a1-a2) -(d1-d2)*(a1-a3)  )/ c1;
  box[1]&=-y1;
  if  (box[1].is_empty()) {box.set_empty();return;}

 /*
  c1 = (a2-a1) *(b2-b3) - (a2-a3)* (b2-b1);
	      
  x1&= ((d2-d1)*(b2-b3) - (d2-d3)*(b2-b1) )/c1;
  y1&= ( (d2-d3)*(a2-a1) -(d2-d1)*(a2-a3)  )/ c1;  
  */


  z1=d1 - a1*x1 - b1*y1;  


  box[2]&=z1;


  if (box[2].is_empty()) box.set_empty();

  /*
  IntervalVector box1 (box);
  box1[0]&=-x1;
  box1[1]&=-y1;
  box1[2]&=z1;
    
  //  cout << " box " << box << " box1 " << box1 << endl;
  if (box1[0].is_empty()) box1.set_empty();
  else if (box1[1].is_empty()) box1.set_empty();
  else if (box1[2].is_empty()) box1.set_empty();
  return box1;
  */
  }





  /*
  int CtcQInterPlane::points_count(IntervalVector& box){
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


int CtcQInterPlane::activepoints_count(IntervalVector& box){
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
 

int CtcQInterPlane::activepoints_contract_count(IntervalVector& box){
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
 void  CtcQInterPlane::point_contract(IntervalVector& box, int iter) 
 {
       fwdbwd(box,iter);
       //  fwd(box,iter);   // more efficient ->  fix point useless on CtcQInter constraint
 }
*/

  /* computations for the affine projection : see  CtcQInterAff.cpp */


  CtcQInterAffPlane::CtcQInterAffPlane(int n, int nb_obs, const Array<Ctc>& ctc_list, double** linfun, 
				 double epseq, int q,  qintermethod meth) : 
  CtcQInter(n,ctc_list,q,meth),
  CtcQInterPlane (n,nb_obs,ctc_list,linfun,epseq,q,meth),
  CtcQInterAff(n,ctc_list,q,meth)
        {
	}


  CtcQInterAffPlane::CtcQInterAffPlane(int n, int nb_obs,const Array<Ctc>& ctc_list, double** linfun, int** index, IntervalMatrix* boxes, double epseq, int q,  qintermethod meth) : 
  CtcQInter(n,ctc_list,q,meth),
    CtcQInterPlane (n,nb_obs,ctc_list,linfun,index,boxes,epseq,q,meth),
    CtcQInterAff(n,nb_obs,ctc_list,q,meth)
        {
	}

  double CtcQInterAffPlane::err_compute( int iter, int k, const IntervalVector& box,Affine2& af)
  {return epseq;}

 double CtcQInterAffPlane::valmean_compute(int iter, int k, const IntervalVector& box, Affine2& af){
   if (kfun==3){
     double valmean = linfun[index[iter][k]][0];
     for (int j =0; j< nb_var-1; j++)
       valmean+=linfun[index[iter][k]][j+1]*box[j].mid();
     return valmean;
   }
   else{
     double valmean = linfun[iter][0];
     for (int j =0; j< nb_var-1; j++)
       valmean+=linfun[iter][j+1]*box[j].mid();
     return valmean;
   }
 }
 



  double  CtcQInterAffPlane::slope_compute(int iter, int j , int k , const IntervalVector & box, Affine2& af2)
  {  
      return linfun[iter][j+1];
  }

 vector<int> CtcQInterPlane::initial_observations (int i){
   if (kfun==1)
     return CtcQInter::initial_observations ( i);
   else{
     
    vector<int> vec;
    vec.push_back(index[i][0]);
    vec.push_back(index[i][1]);
    vec.push_back(index[i][2]);
    return vec;}
 }
  
  // no need to affine arithmetics : functions err_compute, valmean_compute, and slope_compute use directly linfun
  void CtcQInterAffPlane::compute_affine_evaluation( int i, int iter,  Affine2& af, Interval& af2) {
   ; }
  
  // the qinter in initial directions is not called  if the gap between the number of possible and Q is greater than the affine threshold 
  //int CtcQInterAffPlane::affine_threshold(){return 10;}
    int CtcQInterAffPlane::affine_threshold(){return INT_MAX;}


  // the qinter in initial directions is not called  if the threshold is > 0.1
  double  CtcQInterAffPlane::max_diam_threshold(const IntervalVector& box) {
    return 1.0;}
    //return 0.0;}
}
