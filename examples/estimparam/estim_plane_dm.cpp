#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
using namespace ibex;

 
/*
 * 
 * Detects plan in noisy pictures using q-intersection. 
 *
 * 
 * 
 */



/*===============parameters==================*/



const double epseq=0.001;                                        // epsilon on plane constraint

/*===========================================*/



bool  max_dist (IntervalVector& v1, IntervalVector& v2, Vector& prec)
{double d;
  for (int i=0; i< v1.size(); i++)
    { if (v1[i].lb() > v2[i].ub()) 
	d= v1[i].lb() - v2[i].ub();
      else
	if (v2[i].lb() > v1[i].ub())
	  d= v2[i].lb() - v1[i].ub();
      if (d > 10* prec[i]) return false;
    }
  
  return true;
}
	 
int main(int argc, char** argv) {

	vector<double> *x = new vector<double>;
	vector<double> *y = new vector<double>;
	vector<double> *z = new vector<double>;

	double cputime =0;
	double totaltime=0;
        int pb= atoi(argv[1]);
	int NP = atoi(argv[2]);
	int flist=atoi(argv[3]);
	int fixpoint = atoi(argv[4]);
	int Q=atoi(argv[5]);
	int NT3=atoi(argv[6]);
	int Qi;
	switch(pb)
	  {case 1 : {Qi=0.1001*NP;break;}
	  //	  {case 1 : {Qi=0.099*NP;break;}
	  case 2 : {Qi=0.0501*NP;break;}
	  case 3 : {Qi=0.0401*NP;break;}
	  case 4 : {Qi=0.0201*NP;break;}
	  case 5 : {Qi=0.0151*NP;break;}
	  case 6 : {Qi=0.0101*NP;break;}
	  }

	cout << " pb " << pb << " nb points " << NP << " Q " << Q << endl;
	for (int i=0;i<NP;i++)
	  z->push_back(rand()%256);
        if (pb <=3)
	  { int kk;
	    if (pb==1) kk=150;
	    else if (pb==2) kk=50;
	    else if (pb==3) kk=40;
	    for (int i=0;i<kk;i++)
	      {	
		y->push_back(rand()%256);
		y->push_back(rand()%256);
		y->push_back(rand()%256);
		y->push_back(22 + (((double)rand()/(double)RAND_MAX)*0.001));
		y->push_back(12 + (((double)rand()/(double)RAND_MAX)*0.001));
	      }
	    if (pb==1) kk=250;
	    else if (pb==2) kk=750;
	    else if (pb==3) kk=800;
	    for (int i=0;i<kk;i++)
	      y->push_back(rand()%256);
	
	    int kk1 = NP/10;
	    for (int i=0;i<kk1;i++)
	      {x->push_back(10+ (((double)rand()/(double)RAND_MAX)*0.001));
		if (pb==1)
		  x->push_back(10 + (((double)rand()/(double)RAND_MAX)*0.001));
		else
		  x->push_back(rand()%256);
		x->push_back(100 + (((double)rand()/(double)RAND_MAX)*0.001));
		x->push_back(rand()%256);
		x->push_back(rand()%256);
		x->push_back(rand()%256);
		x->push_back(rand()%256);
		x->push_back(rand()%256);
		x->push_back(rand()%256);
		x->push_back(rand()%256);
	      }
	  }
	
	 
	else{

	  for (int i=0;i<NP;i++)
	    y->push_back(rand()%256);
          if (pb==5){

	    for (int i=0;i<Qi;i++)  {

	    x->push_back(300- y->at(65*i) - z-> at(65*i)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(- y->at(65*i+1)  + z-> at(65*i+1)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-200+ y->at(65*i+2)  + z-> at(65*i+2)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back( y->at(65*i+3)  - z-> at(65*i+3)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(320- y->at(65*i+4) - z-> at(65*i+4)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(10 - y->at(65*i+5)  + z-> at(65*i+5)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(5- y->at(65*i+6)  + z-> at(65*i+6)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(15- y->at(65*i+7)  + z-> at(65*i+7)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(330- y->at(65*i+8) - z-> at(65*i+8)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(20- y->at(65*i+9)  + z-> at(65*i+9)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-210+ y->at(65*i+10)  + z-> at(65*i+10)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(25 -y->at(65*i+11)  - z-> at(65*i+11)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(340- y->at(65*i+12) - z-> at(65*i+12)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(30 - y->at(65*i+13)  + z-> at(65*i+13)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(35- y->at(65*i+14)  + z-> at(65*i+14)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(40- y->at(65*i+15)  + z-> at(65*i+15)+ (((double)rand()/(double)RAND_MAX)*0.001));

	     x->push_back(350- y->at(65*i+16) - z-> at(65*i+16)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(3- y->at(65*i+17)  + z-> at(65*i+17)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-220+ y->at(65*i+18)  + z-> at(65*i+18)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back( 12 + y->at(65*i+19)  - z-> at(65*i+19)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(360- y->at(65*i+20) - z-> at(65*i+20)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(12 - y->at(65*i+21)  + z-> at(65*i+21)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(7- y->at(65*i+22)  + z-> at(65*i+22)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(17- y->at(65*i+23)  + z-> at(65*i+23)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(370- y->at(65*i+24) - z-> at(65*i+24)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    for (int j=0; j<40; j++)  x->push_back(rand()%256);
	    }
	    int K0=0.0251*NP; 
	    for (int i=0; i< K0;i++) x->push_back(rand()%256);
	  }
	  else

	for (int i=0;i< Qi;i++)
	  {

	    x->push_back(300- y->at(50*i) - z-> at(50*i)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(- y->at(50*i+1)  + z-> at(50*i+1)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-200+ y->at(50*i+2)  + z-> at(50*i+2)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back( y->at(50*i+3)  - z-> at(50*i+3)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(320- y->at(50*i+4) - z-> at(50*i+4)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(10 - y->at(50*i+5)  + z-> at(50*i+5)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(5- y->at(50*i+6)  + z-> at(50*i+6)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(15- y->at(50*i+7)  + z-> at(50*i+7)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(330- y->at(50*i+8) - z-> at(50*i+8)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(20- y->at(50*i+9)  + z-> at(50*i+9)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-210+ y->at(50*i+10)  + z-> at(50*i+10)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(25 -y->at(50*i+11)  - z-> at(50*i+11)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(340- y->at(50*i+12) - z-> at(50*i+12)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(30 - y->at(50*i+13)  + z-> at(50*i+13)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(35- y->at(50*i+14)  + z-> at(50*i+14)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(40- y->at(50*i+15)  + z-> at(50*i+15)+ (((double)rand()/(double)RAND_MAX)*0.001));

	     x->push_back(350- y->at(50*i+16) - z-> at(50*i+16)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(3- y->at(50*i+17)  + z-> at(50*i+17)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-220+ y->at(50*i+18)  + z-> at(50*i+18)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back( 12 + y->at(50*i+19)  - z-> at(50*i+19)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(360- y->at(50*i+20) - z-> at(50*i+20)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(12 - y->at(50*i+21)  + z-> at(50*i+21)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(7- y->at(50*i+22)  + z-> at(50*i+22)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(17- y->at(50*i+23)  + z-> at(50*i+23)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(370- y->at(50*i+24) - z-> at(50*i+24)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    int K1=25;
	    for (int j=0; j< K1;j++) x->push_back(rand()%256);
	  }
	
	  if (pb==6)	for (int j=0; j< NP/2;j++) x->push_back(rand()%256);
	}
	srand(2);

	cout << " nb points " << x->size() << endl;
	//	for (int i=0; i< x->size(); i++) cout << (*x)[i] <<  " " << (*y)[i] << " " << (*z)[i] <<endl;
	
	int n=3;
	
	Variable u(3);
	Variable w(3);
	
	int p = NP;

	int K=3;
	int np;


	np=NT3*p;
	cout << np << "  " << Q << endl;

	
	Function ***m_fun;
        m_fun=new Function **[K];
	for (int i=0; i<K; i++)
	  m_fun[i]=new Function*[np];
	

	double *** linfun;
        linfun = new double**[np];
	for (int i=0; i<np; i++)
	  {  linfun[i] = new double*[n];
	    for (int j=0; j<n; j++)
	      linfun[i][j]= new double[K];
	  }
	
	double *** linfun1;

	linfun1 = new double**[p];
	    for (int i=0; i<p; i++)
	      {  linfun1[i] = new double*[n];
		for (int j=0; j<n; j++)
		  linfun1[i][j]= new double[1];
	      }	   
	
	   

	Function* m_f0;
	Ctc* ctc0;
	Function**  m_func= new Function* [p] ;	

	Variable v(3);

	clock_t start, start0, end;
	int nb_cells=0;
	start = clock();
	start0= clock();
	int Q0=Q;
	
	int* index2 = new int [np];
	int* index3= new int [np];
	
	
	if (K==3)
	  for (int nt=0; nt<NT3;nt++)
	    for (int i=0; i<p; i++) {
	      int j = rand() % (p-1);
	      int k = rand() % (p-2);
	      if (j>=i) j=j+1;
	      if (k >=j && k >= i) k=k+2;
	      else
		{if (k >=j || k>=i ) k=k+1;
		  if (k==i || k==j) k=k+1;
		}
	      index2[i+p*nt]=j;
	      index3[i+p*nt]=k;
	    }
	




	for (int oct=0; oct <4; oct++)

	  { 
	    int pmax=0;
	    Array<Ctc> m_ctc(p);
	    Array<Ctc> m_ctc1(np);

	    IntervalMatrix* boxes = new IntervalMatrix (np,n);	 
	    
	    int diry= pow(-1,oct%2);
	    int dirz= pow(-1,oct/2);
	    
	    double _box[3][2];

	    if (diry==1) {_box[0][0] = 0;
	      _box[0][1] = 1;}
	    else {_box[0][0] = -1;_box[0][1] = 0;}
	    if (dirz==1)
	      {	    _box[1][0] = 0;	    _box[1][1] = 1;}
	    else
	      {	    _box[1][0] = -1;	    _box[1][1] = 0;}
	    _box[2][0]=-500;
	    _box[2][1]=500;
	    IntervalVector box(3,_box);
	    
	    m_f0= new Function(v, (diry*v[0]+dirz*v[1]-1));

            ctc0=new CtcFwdBwd(*m_f0,LEQ);
	    for (int i=0; i<p; i++) {
	 
	  //	  Function fi (v,(x->at(i) +diry*v[0]*(y->at(i)-x->at(i))+dirz*v[1]*(z->at(i)-x->at(i))-v[2]-Interval(-epseq,epseq)));
	    
	      m_func[i] = new Function(v,(x->at(i) +v[0]*(y->at(i)- diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-v[2]-Interval(-epseq,epseq)));
	      m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));

	      //	      m_ctc.set_ref(i,*new Ctc3BCid(3,* new  CtcFwdBwd(*m_func[i]),5,1,3));  	  

	  //	  m_ctc.set_ref(i,*new CtcCompo (*new CtcFwdBwd(*m_func[i]), * new CtcFwdBwd(*m_func[i], EQ, AFFINE2_MODE)));
	  //	  m_ctc.set_ref(i, (* new CtcFixPoint (* new CtcFwdBwd(*m_func[i]),0.2)));

	      //	      m_ctc.set_ref(i,*new CtcCompo (*new CtcFixPoint (* new CtcFwdBwd(*m_func[i]),0.2),  // cid seul ne marche pas
		//			     *new Ctc3BCid(3,* new CtcFixPoint (* new CtcFwdBwd(*m_func[i]),0.2),5,1,3)));

	      //	      m_ctc.set_ref(i,*new CtcCompo (*new CtcFixPoint (* new CtcFwdBwd(*m_func[i]),0.2),  // cid seul ne marche pas
                //                             *new Ctc3BCid(3,* new CtcFixPoint (* new CtcFwdBwd(*m_func[i]),0.2))));



	  //	  m_ctc.set_ref(i,*new CtcCompo (* new CtcFwdBwd(*m_func[i]),  // cid seul ne marche pas
	  //					 *new Ctc3BCid(3, * new CtcFwdBwd(*m_func[i]),5,1,3)));
	    }



	    if (K==1) {
	      for (int i=0; i<p; i++)  {
	    /* We must be on the plane defined by v */
		m_ctc1.set_ref(i,m_ctc[i]);
		m_fun[0][i]= new Function(v,(x->at(i) +v[0]*(y->at(i)-diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-Interval(-epseq,epseq)));
		linfun[i][0][0]=x->at(i);
		linfun[i][1][0]=y->at(i)-diry*x->at(i);
		linfun[i][2][0]=z->at(i)-dirz*x->at(i);
	      }
	    }
	    /*
	    else if(K==2)
	      {
		for (int nt=0; nt<NT1;nt++)
		  for (int i=0; i<p; i++) {
		    int j = rand() % (p-1);
		    if (j>=i) j=j+1;

		    vector<LinearRelaxXTaylor::corner_point> cpoints;
		    cpoints.push_back(LinearRelaxXTaylor::RANDOM);
		    SystemFactory fac;
		    fac.add_var(v);
		    fac.add_ctr(*m_func[i]);
		    fac.add_ctr(*m_func[j]);
                    System* sys = new System(fac);
		    LinearRelaxXTaylor* lrt= new LinearRelaxXTaylor(*sys,cpoints);
		    //LinearRelaxCombo* lrt= new LinearRelaxCombo(*sys,LinearRelaxCombo::ART);
		    CtcPolytopeHull* poly=new CtcPolytopeHull( lrt);
		    systemlr[p*nt+i]=sys;
		    lrtaylor[p*nt+i]=lrt;
		    polytopeh[p*nt+i]= poly;
		    //    m_ctc1.set_ref(p*nt+i,*new CtcFixPoint (* new CtcCompo (m_ctc[i],m_ctc[j]),0.1));

		    //		    m_ctc1.set_ref(p*nt+i,* poly);
		    //m_ctc1.set_ref(p*nt+i,* new CtcCompo (m_ctc[i],m_ctc[j]));
		    //m_fun[0][p*nt+i]=new Function(v,(x->at(i)  +v[0]*(y->at(i)-diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-Interval(-epseq,epseq)));
		    
		    linfun[p*nt+i][0][0]=x->at(i);
		    linfun[p*nt+i][1][0]=y->at(i)-diry*x->at(i);
		    linfun[p*nt+i][2][0]=z->at(i)-dirz*x->at(i);


		    // m_fun[1][p*nt+i]=new Function(v,(x->at(j) +v[0]*(y->at(j)-diry*x->at(j))+v[1]*(z->at(j)-dirz*x->at(j))-Interval(-epseq,epseq)));
		    linfun[p*nt+i][0][1]=x->at(j);
		    linfun[p*nt+i][1][1]=y->at(j)-diry*x->at(j);
		    linfun[p*nt+i][2][1]=z->at(j)-dirz*x->at(j);

		  }
	      }
	    */

	    else if(K==3)
	      {
		for (int nt=0; nt<NT3;nt++)
		  for (int i=0; i<p; i++) {
		    int j = index2[nt*p+i];
                    int k=  index3[nt*p+i];
		  

                    Interval a1,a2,a3,b1,b2,b3,d1,d2,d3;
                    a1=y->at(i)-diry*x->at(i);
                    a2=y->at(j)-diry*x->at(j);
                    a3=y->at(k)-diry*x->at(k);
                    b1=z->at(i)-dirz*x->at(i);
                    b2=z->at(j)-dirz*x->at(j);
                    b3=z->at(k)-dirz*x->at(k);
                    d1=Interval(-epseq,epseq)+x->at(i);
                    d2=Interval(-epseq,epseq)+x->at(j);
                    d3=Interval(-epseq,epseq)+x->at(k);


                    Interval x1,y1,z1;
                    x1= ((d1-d2)*(b1-b3) - (d1-d3)*(b1-b2) )/ ((a1-a2) *(b1-b3) - (a1-a3)* (b1-b2));
                    y1= ( (d1-d3)*(a1-a2) -(d1-d2)*(a1-a3)  )/ ((a1-a2) *(b1-b3) - (a1-a3)* (b1-b2));
		    //                    x1&=((d1-d3)*(b1-b2) - (d1-d2)*(b1-b3) )/ ((a1-a3) *(b1-b2) - (a1-a2)* (b1-b3));
		    //                    y1&=( (d1-d2)*(a1-a3) -(d1-d3)*(a1-a2)  )/ ((a1-a3)*(b1-b2) - (a1-a2)* (b1-b3));
		    x1&= ((d2-d1)*(b2-b3) - (d2-d3)*(b2-b1) )/ ((a2-a1) *(b2-b3) - (a2-a3)* (b2-b1));
		    y1&= ( (d2-d3)*(a2-a1) -(d2-d1)*(a2-a3)  )/ ((a2-a1) *(b2-b3) - (a2-a3)* (b2-b1));
		    z1=d1 - a1*x1 - b1*y1;
		    //		    z1&=d2 - a2*x1 - b2*y1;
		    //		    z1&=d3 - a3*x1 - b3*y1;


		    //		    cout << x1 << " " << y1 << " " << "" <<  z1 << endl;

                    
		    IntervalVector* box1= new IntervalVector(box);
		    //		    cout << "box avant inter " << x1 << " " << y1 << " " << -z1 << endl;
		    
                    (*box1)[0]&=-x1;
                    (*box1)[1]&=-y1;
		    (*box1)[2]&=z1;
		    
		    if ((*box1)[0].is_empty()) box1->set_empty();
		    else if ((*box1)[1].is_empty()) box1->set_empty();
		    else if ((*box1)[2].is_empty()) box1->set_empty();
		    else if (((*box1)[0]*diry+(*box1)[1]*dirz).lb()>1) box1->set_empty();   // ?? pourquoi cette condition
		    (*boxes)[p*nt+i]= *box1;
		    if (!(box1->is_empty())) pmax++;
		

		    //		    cout << "box apres inter " << *box1 << endl;
		    //		    delete sys;
		    //		    delete lrt;
		    //		    delete poly; 

		    //		    m_ctc1.set_ref(p*nt+i,* poly);
		    m_ctc1.set_ref(p*nt+i,*new CtcCompo (m_ctc[i],m_ctc[j],m_ctc[k]));


 
		    //		    m_fun[0][p*nt+i]= new Function(v,(x->at(i) +v[0]*(y->at(i)-diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-Interval(-epseq,epseq)));
		    //		    m_fun[1][p*nt+i]= new Function(v,(x->at(j) +v[0]*(y->at(j)-diry*x->at(j))+v[1]*(z->at(j)-dirz*x->at(j))-Interval(-epseq,epseq)));
		    //		    m_fun[2][p*nt+i]= new Function(v,(x->at(k) +v[0]*(y->at(k)-diry*x->at(k))+v[1]*(z->at(k)-dirz*x->at(k))-Interval(-epseq,epseq)));
	
		    linfun[p*nt+i][0][0]=x->at(i);
		    linfun[p*nt+i][1][0]=y->at(i)-diry*x->at(i);
		    linfun[p*nt+i][2][0]=z->at(i)-dirz*x->at(i);

		    linfun[p*nt+i][0][1]=x->at(j);
		    linfun[p*nt+i][1][1]=y->at(j)-diry*x->at(j);
		    linfun[p*nt+i][2][1]=z->at(j)-dirz*x->at(j);

		    linfun[p*nt+i][0][2]=x->at(k);
		    linfun[p*nt+i][1][2]=y->at(k)-diry*x->at(k);
		    linfun[p*nt+i][2][2]=z->at(k)-dirz*x->at(k);
	
		  }

	      }


	    vector<IntervalVector> resgroup;


	    Vector prec(3);

	    /*
	    prec[0]=0.001;
	    prec[1]=0.001;
	    prec[2]=0.005;
	    */
	    

	    // les precisions dans l'article soumis



	    prec[0]=0.000007;
	    prec[1]=0.000007;
	    prec[2]=0.0007;


	    Vector proba(3);
	    proba[0]=0.33;
	    proba[1]=0.33;
	    proba[2]=0.34;
	    CellStack buff;
	    //	    RoundRobin bs (prec,0.5);
	    RoundRobinNvar bs (2,prec,0.5);
	    // RoundRobinQInter bs (2,prec,0.5);
	    //	ProbaBisect bs (prec, proba, 0.45);
	    // LargestFirst bs (prec,0.5);


            CtcQInter* ctcq;	    
	    //	    CtcQInter ctcq(3,m_ctc1,Q);
	    //	    CtcQInterAff ctcq(3,m_ctc1,Q,m_fun,QINTERPROJ,K);
	    if (flist==1)
	      ctcq = new CtcQInterAffPlane (n,m_ctc1,linfun,boxes,epseq,Q,QINTERPROJ,K);
	    else
	      ctcq= new  CtcQInterPlane (n,m_ctc1,linfun,boxes,epseq,Q,QINTERPROJ,K);
	    //	    CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,QINTERPROJ,K);
	    //	    CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,QINTERCORE,K);
	    //	    CtcQInterPlane  ctcq(n,m_ctc1,linfun,epseq,Q, QINTERCORE,K );
	    //	    CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,QINTERFULL, K);

	    CtcCompo ctcqf0 (*ctc0,*ctcq);
	    Ctc3BCid cid(*ctcq,10,1,3);
	    CtcCompo ctcid(*ctcq ,cid);

	    //	    CtcCompo ctcqf0(*ctc0,ctcid);


	    //	    CtcFixPoint ctcf(ctcqf0,0.1);
	    CtcFixPoint ctcf(ctcid,0.1);

	    // Ctc3BCid cid(ctcqf0,5,1,3); 	    
	    /*
	   K=1;
	    
	    for (int i=0; i<p; i++)  {

	      //		m_ctc1.set_ref(i,m_ctc[i]);

	//		m_fun[0][i]= new Function(v,(x->at(i) +v[0]*(y->at(i)-diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-Interval(-epseq,epseq)));
		linfun1[i][0][0]=x->at(i);
		linfun1[i][1][0]=y->at(i)-diry*x->at(i);
		linfun1[i][2][0]=z->at(i)-dirz*x->at(i);
	      }
	    Q=40;

	    CtcQInterPlane ctcq(n,m_ctc,linfun1,boxes,epseq,Q,flist,QINTERPROJ,K);
	    cout << " apres contracteur " << endl;
	    for (int i=0 ; i<np; i++)
	      {int count =0;
		if (!(boxes[i]->is_empty()))
		  {//cout << i << " " << *(boxes[i])  << endl;
		    countc=ctcq.points_count(*(boxes[i]));
		    if (count >= Q) 
		      cout << i << " " << *(boxes[i])  << " " << count << endl;
		  }
	      }
	    cout << " apres comptage " << endl;
	    K=3;
	    */
	    Ctc* ctcs;
	    if (fixpoint==0) ctcs=&ctcqf0;
	    else ctcs=&ctcf;
	    		       
	    
	    SolverQInter s(*ctcs,bs,buff,*ctcq);

	    //Solver s(ctcqf0,bs,buff);
	    s.time_limit = 1000;
	    s.trace=0;
	    cout << " avant resolution " << endl;

	    vector<IntervalVector> res=s.solve(box);


	    cout << "Number of branches : " << s.nb_cells << endl;
	    cout << "Number of solutions : " << res.size() << endl;
	    nb_cells +=s.nb_cells;
	    cputime += s.time;


	// cout << " nb sols " << res.size() << endl;   
	//	  cout << " nb active sols " << s.maximal_sol.size() << endl;   
	    s.report_maximal_solutions(res);
	    vector <int> maxsolinliers;
	    s.keep_one_solution_pergroup(res, maxsolinliers);


	    set<int> trueinliers;
	    for (int i=0; i< maxsolinliers.size(); i++)
	      {  trueinliers.insert(i%p);
		trueinliers.insert(index2[i]);
		trueinliers.insert(index3[i]);
	      }

	    set<int>::iterator iter = trueinliers.begin();
	    cout << " nb inliers " << trueinliers.size() << endl;
	    
	    while( iter !=trueinliers.end()){
	      cout << *iter << " " ; 
	      iter++;
	    }
	    
	    cout << endl;



	  /*
	if (res.size() >1)
	  {	resgroup.push_back(res[0]);

	for (int i=0; i<res.size(); i++)
	  {if (max_dist (res[i],resgroup.back(),  prec))
	      resgroup[resgroup.size()-1]= res[i]|resgroup.back();
	    else
	      resgroup.push_back(res[i]);
	      }
	cout << "regroupement  solutions " << endl;
	for (int i=0; i<resgroup.size(); i++)
	  {	  cout << resgroup[i] << " "  << endl;
	    int k=0;
	    for (int j=0; j<p; j++)
	      { Interval evalctc= m_func[j]->eval(resgroup[i]);
		if (evalctc.contains(0)) {k++; 
		  //		  cout << j << (*x)[j] << " " << (*y)[j] <<  " " << (*z)[j] <<  " " <<
		  //		    m_func[j]->eval(resgroup[i]) << endl;
		}
	      }

	    cout << "nb points " << k <<endl;
	  }


	vector<IntervalVector> resgroup2;

	if (resgroup.size() >1)
	  {	cout << "regroupement  solutions 2" << endl;
	    resgroup2.push_back(resgroup[0]);
	    for (int i=0; i<resgroup.size(); i++)
	  {if (max_dist (resgroup[i],resgroup2.back(), prec))
	      resgroup2[resgroup2.size()-1]= resgroup[i]|resgroup2.back();
	    else
	      resgroup2.push_back(resgroup[i]);
	  }


	for (int i=0; i<resgroup2.size(); i++)
	  {	  cout << resgroup2[i] << " "  << endl;
	    int k=0;
	    for (int j=0; j<p; j++)
	      { Interval evalctc= m_func[j]->eval(resgroup2[i]);
		if (evalctc.contains(0)) {k++; 
		  //		  cout << j << (*x)[j] << " " << (*y)[j] <<  " " << (*z)[j] <<  " " <<
		  //		    m_func[j]->eval(resgroup[i]) << endl;
		}
	      }

	    cout << "nb points " << k <<endl;
	    //  Q0=k+1;
	  }
	  
	  }
	  }
	  */
	  

		
	
	
	for (int i=0; i<p; i++)
	  { delete m_func[i];
	       delete &m_ctc[i];
	  }

	m_ctc.clear();
	m_ctc1.clear();
	cout << " fin quadrant " << endl;
	end = clock();
	totaltime += ((double)(end)-(double)(start))/CLOCKS_PER_SEC;
	start= clock();
	  }
	  				       
	end=clock();
	cout << "Shape extraction : OK. Time : " << ((double)(end)-(double)(start0))/CLOCKS_PER_SEC << " seconds" << endl;
	cout << " total time " << totaltime << endl;
	cout << " cpu time " << cputime << endl;
	cout <<" total branch number " << nb_cells << endl;

	for (int i=0; i<np; i++)
	  {for (int j=0; j<n; j++)
	      delete [] linfun[i][j];
	    
	    delete [] linfun[i];
	  }
	delete [] linfun;
	
	for (int i=0; i<p; i++)
	  {for (int j=0; j<n; j++)
	      delete [] linfun1[i][j];
	    
	    delete [] linfun1[i];
	  }

	delete [] linfun1;

	delete [] m_func;
	
	delete[] index2;
	delete[] index3;
	
};






