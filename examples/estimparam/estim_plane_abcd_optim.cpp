#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
using namespace ibex;

 
/*
 * 
 * Detects plans in noisy pictures using q-intersection. 
 * model with 4 variables  a,b,c,d  ax+by+cz+d = 0,  with constraint a^2+b^2+c^2=1
 * 
 * 
 */



/*===============parameters==================*/

const double Qprop = 0.1;							//Minimum ratio of consistent measurements
//const double Qprop = 0.015;							//Minimum ratio of consistent measurements
const double epseq=0.001;                                                       // epsilon on plane constraint

/*===========================================*/

const double MIN1 = -1+0.0005;								//Minimum value for x,y,r
const double MAX1 = 1;	

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
	int pb= atoi(argv[1]);
	int NP = atoi(argv[2]);
	int flist=atoi(argv[3]);
	int fixpoint = atoi(argv[4]);
	int gaplimit= atoi(argv[5]);
	int nbrand= atoi(argv[6]);
	string bisect = argv[7];
	int Qi;

	switch(pb)
	  {case 1 : {Qi=0.1001*NP;break;}
	  case 2 : {Qi=0.0501*NP;break;}
	  case 3 : {Qi=0.0401*NP;break;}
	  case 4 : {Qi=0.0201*NP;break;}
	  case 5 : {Qi=0.0151*NP;break;}
	  case 6 : {Qi=0.0101*NP;break;}
	  }
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
	

	    for (int i=0;i<NP/10;i++)
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
	srand(atoi(argv[10]));

	cout << " nb points " << x->size() << endl;
	//	for (int i=0; i< x->size(); i++) cout << (*x)[i] <<  " " << (*y)[i] << " " << (*z)[i] <<endl;

	double time=0;

	int n=4;
	

	
	int p = NP;
	int Q = Qi;

	int K=1;
	int NT1=3;
	int NT2=6;
	int Q1=17;
	int Q2=4;
	int np;

	if (K==1)np=p;
	else if (K==2) {np=NT1*p;Q=Q1;}
	else {np=NT2*p;Q=Q2;}
	cout << np << "  " << Q << endl;

	
	Function ***m_fun;
	m_fun=new Function **[K];
	for (int i=0; i<K; i++)
	  m_fun[i]=new Function*[np];
	

	
	double ** linfun;
        linfun = new double*[p];
	for (int i=0; i<p; i++)
	  {  linfun[i] = new double[n];
	  }
	
       

	Array<Ctc> m_ctc(p);
	Array<Ctc> m_ctc1(np);
	vector<bool> active;
	for (int i=0;i<np;i++)
	  active.push_back(true);

	Function* m_f0;
	Ctc* ctc0;
	Function* m_func[p] ;	
	Variable v(4);
	clock_t start, end;
	int nb_cells=0;
	start = clock();
	int Q0=Q;
	SystemFactory fac;
	fac.add_var(v);


	Function* fnorm = new Function(v,sqr(v[0])+sqr(v[1])+sqr(v[2]) -1);
	CtcFwdBwd* ctcnorm = new CtcFwdBwd(*fnorm);
	fac.add_ctr(*fnorm);
	System sys(fac);
	//	CtcFwdBwd* ctcnorm = new CtcFwdBwd(*new Function(v,sqr(v[0])+sqr(v[1])+sqr(v[2]) -1));
        cout << " avant contraintes " << endl;
	for (int i=0; i<p; i++) {
	  /*		
	  R[i] = sqrt( pow(x->at(i),2)+pow(y->at(i),2));
	  alpha[i] = atan(y->at(i)/x->at(i));
	  m_func[i] = new Function(v,sqrt(1-sqr(v[0]))*R[i]*cos(v[1]-alpha[i])+v[0]*z->at(i)-v[2]+Interval(-epseq,epseq));
	  */
	  //	  Function fi (v,(x->at(i) +diry*v[0]*(y->at(i)-x->at(i))+dirz*v[1]*(z->at(i)-x->at(i))-v[2]-Interval(-epseq,epseq))); 
	      m_func[i] = new Function(v,(v[0]*x->at(i)
				      +v[1]*y->at(i)+v[2]*z->at(i)-v[3]-Interval(-epseq,epseq)));

	      //            m_ctc.set_ref(i,*new CtcFixPoint (* new CtcCompo (* new CtcFwdBwd(*m_func[i]), *ctcnorm), 0.2));
	      m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));
	  

	  //	  m_ctc.set_ref(i,*new CtcCompo (*new CtcFwdBwd(*m_func[i]), * new CtcFwdBwd(*m_func[i], EQ, AFFINE2_MODE)));
	  //	  m_ctc.set_ref(i, (* new CtcFixPoint (* new CtcFwdBwd(*m_func[i]),0.2)));
	  //	  m_ctc.set_ref(i,*new Ctc3BCid(3,* new  CtcFwdBwd(*m_func[i]),5,1,3));  
	  //m_ctc.set_ref(i,*new CtcCompo (*new CtcFixPoint (* new CtcFwdBwd(*m_func[i]),0.2),  // cid seul ne marche pas
	  //					 *new Ctc3BCid(3,* new CtcFixPoint (* new CtcFwdBwd(*m_func[i]),0.2),5,1,3)));
	  //	  m_ctc.set_ref(i,*new CtcCompo (* new CtcFwdBwd(*m_func[i]),  // cid seul ne marche pas
	  //					 *new Ctc3BCid(3, * new CtcFwdBwd(*m_func[i]),5,1,3)));
	    }

	if (K==1) {
	      for (int i=0; i<p; i++)  {
	    /* We must be on the plane defined by v */
		m_ctc1.set_ref(i,m_ctc[i]);

		m_fun[0][i]= new Function(v,(v[0]*x->at(i) +v[1]*y->at(i)+v[2]*z->at(i)-Interval(-epseq,epseq)));

		linfun[i][0]=0;
		linfun[i][1]=x->at(i);
		linfun[i][2]=y->at(i);
		linfun[i][3]=z->at(i);

	      }
	    }
	/*
	    else if(K==2)
	      {
		for (int nt=0; nt<NT1;nt++)
		  for (int i=0; i<p; i++) {
		    int j = rand() % (p-1);
		    if (j>=i) j=j+1;
		    //		    m_ctc1.set_ref(p*nt+i,*new CtcFixPoint (* new CtcCompo (m_ctc[i],m_ctc[j]),0.1));
		    m_ctc1.set_ref(p*nt+i,* new CtcCompo (m_ctc[i],m_ctc[j]));
		    //  m_fun[0][p*nt+i]=new Function(v,(x->at(i)  +v[0]*(y->at(i)-diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-Interval(-epseq,epseq)));



		    // m_fun[1][p*nt+i]=new Function(v,(x->at(j) +v[0]*(y->at(j)-diry*x->at(j))+v[1]*(z->at(j)-dirz*x->at(j))-Interval(-epseq,epseq)));


		  }
	      }
	    else if(K==3)
	      {
		for (int nt=0; nt<NT2;nt++)
		  for (int i=0; i<p; i++) {
		    int j = rand() % (p-1);
		    int k = rand() % (p-2);
		    if (j>=i) j=j+1;
		    if (k >=j && k >= i) k=k+2;
		    else
		      {if (k >=j || k>=i ) k=k+1;
			if (k==i || k==j) k=k+1;
		      }
		    m_ctc1.set_ref(p*nt+i,*new CtcFixPoint (* new CtcCompo (m_ctc[i],m_ctc[j],m_ctc[k]),0.1));


		    //		    m_fun[0][p*nt+i]= new Function(v,(x->at(i) +v[0]*(y->at(i)-diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-Interval(-epseq,epseq)));
		    //		    m_fun[1][p*nt+i]= new Function(v,(x->at(j) +v[0]*(y->at(j)-diry*x->at(j))+v[1]*(z->at(j)-dirz*x->at(j))-Interval(-epseq,epseq)));
		    //		    m_fun[2][p*nt+i]= new Function(v,(x->at(k) +v[0]*(y->at(k)-diry*x->at(k))+v[1]*(z->at(k)-dirz*x->at(k))-Interval(-epseq,epseq)));


		  }

	      }
	*/
	cout << " avant box " << endl;

	    double _box[4][2];

	    for (int j=0; j<3; j++) {
		_box[j][0] = MIN1;
		_box[j][1] = MAX1;
	    }

	   
	    _box[0][0]=0;
	    _box[3][0]=-500;
	    _box[3][1]=500;
	
	    IntervalVector box(4,_box);

	    vector<IntervalVector> resgroup;


	    Vector prec(4);
	    /*
	    prec[0]=0.001;
	    prec[1]=0.001;
	    prec[2]=0.001;
	    prec[3]=0.005;
	    
	    
	    prec[0]=0.000007;
	    prec[1]=0.000007;
	    prec[2]=0.000007;
	    prec[3]=0.0007;
	    */

	      prec[0]=atof(argv[8]);
	    prec[1]=atof(argv[8]);
	    prec[2]=atof(argv[8]);
	    prec[3]=atof(argv[9]);


	    CellHeapQInter buff;
	    BeamSearch str(buff);
	    //   CellStack buff;
	    Bsc * bs;

	    if (bisect=="rr")
	      bs = new RoundRobin(prec, 0.5);
	    else if (bisect =="rr2")
	      bs= new RoundRobinNvar(3,prec,0.5);
	    //	    RoundRobin bs (prec,0.5);
	    //	    RoundRobinNvar bs (3,prec,0.5);
	    // RoundRobinQInter bs (2,prec,0.5);
	    //	ProbaBisect bs (prec, proba, 0.45);
	    // LargestFirst bs (prec,0.5);
	    /* Main optimization loop */

	    //   CtcQInter ctcq(4,m_ctc1,Q);
	    //CtcQInterAff ctcq(4,m_ctc1,Q,m_fun,QINTERPROJ,K);
	    //	    CtcQInterAff ctcq(4,m_ctc1,Q,flist,K,QINTERPROJ);
	    CtcQInter* ctcq;	 
	    if (flist==1)
	      ctcq = new CtcQInterAffPlane (n,p,m_ctc1,linfun,epseq,Q,QINTERPROJ);
	    else
	      ctcq= new  CtcQInterPlane (n,p,m_ctc1,linfun,epseq,Q,QINTERPROJ);

	    //	    CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,flist,QINTERPROJ);
	    //	    CtcQInterPlane  ctcq(n,m_ctc1,linfun,epseq,Q,flist, QINTERCORE);
	    //CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,flist, CtcQInterGen::FULL);
	    Ctc3BCid cid(*ctcq,5,1,1);
	    CtcCompo ctcid(*ctcq ,cid);
	    //CtcCompo ctcqf0(*ctcnorm,ctcq,*ctcnorm);
	    CtcCompo ctcqf0(*ctcnorm,*ctcq);
	    CtcFixPoint ctcf(ctcqf0,0.1);
	    //CtcFixPoint ctcf(ctcid,0.1);

	    Ctc* ctcs;
	    if (fixpoint==0) ctcs=&ctcqf0;
	    else ctcs=&ctcf;
	    double epscont =1.e-4;
	    //	    SolverOptConstrainedQInter s(sys,ctcqf0,*bs,str,*ctcq,epscont)
	    SolverOptQInter s(ctcqf0,*bs,str,*ctcq);
	    
	    if (flist==1)
	      s.str.with_storage=true;

	    s.timeout = 10000;
	    s.trace=1;
	    s.feasible_tries=nbrand;
	    s.gaplimit=gaplimit;
	    //	    s.tolerance_constraints_number=10000;  // no second call for feasible point 
	    cout << " avant resolution " << endl;

	    IntervalVector res=s.solve(box);

	    cout << "Number of branches : " << s.nb_cells << endl;
	    cout << "time used : " << s.time << endl;
	    nb_cells +=s.nb_cells;
	    s.report_possible_inliers();
  
	    s.report_solution();
	
	for (int i=0; i<p; i++)
	  {delete m_func[i];
	    delete &m_ctc[i];
	  }
	m_ctc.clear();
	m_ctc1.clear();

	  
	  				       
	end = clock();
	cout <<  " time used " << s.time << "  "<< endl ;
	cout << "Shape extraction : OK. Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	cout <<" total branch number " << nb_cells << endl;
	

	for (int i=0; i<p; i++)
	  delete [] linfun[i];
	delete [] linfun;


};






