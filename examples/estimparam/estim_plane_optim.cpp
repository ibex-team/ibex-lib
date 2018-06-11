#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
using namespace ibex;

 
/*
 * 
 * Detects plan with maximal number of inliers in noisy pictures using q-intersection. 
 *
 * 
 * 
 */



/*===============parameters==================*/



const double epseq=0.001;             // epsilon on plane constraint

/*===========================================*/


	 
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

	cout << " pb " << pb << " nb points " << NP << " Q " << Qi << endl;
	for (int i=0;i<NP;i++)
	  z->push_back(rand()%256);
        if (pb <=3)
	  { int kk;
	    if (pb==1) kk=150 * NP/1000;
	    else if (pb==2) kk=50 * NP /1000;
	    else if (pb==3) kk=40 * NP/ 1000;
	    for (int i=0;i<kk;i++)
	      {	
		y->push_back(rand()%256);
		y->push_back(rand()%256);
		y->push_back(rand()%256);
		y->push_back(22 + (((double)rand()/(double)RAND_MAX)*0.001));
		y->push_back(12 + (((double)rand()/(double)RAND_MAX)*0.001));
	      }
	    int kk2 = NP - 5*kk;
	    for (int i=0;i<kk2;i++)
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


	//	cout << " nb points " << x->size() <<  " " << y->size() << " " << z->size() << endl;
	//	for (int i=0; i< x->size(); i++) cout << (*x)[i] <<  " " << (*y)[i] << " " << (*z)[i] <<endl;
	srand(atoi(argv[10])); // graine ici pour que la variation de la graine ne joue que sur l'execution.	
	int n=3;
	
	Variable u(3);
	Variable w(3);
	
	int p = NP;
	int Q = Qi;

	int Q1=17;

	int Q2= 4;

	int NT1=3;
	int NT2=6;

	int K=1;
	int np;

	if (K==1)np=p;
	else if (K==2) {np=NT1*p;Q=Q1;}
	else {np=NT2*p;Q=Q2;}
	cout << np << "  " << Q << endl;

	/*
	Function ***m_fun;
        m_fun=new Function **[K];
	for (int i=0; i<K; i++)
	  m_fun[i]=new Function*[np];
	*/
	double ** linfun;
        linfun = new double*[p];
	for (int i=0; i<p; i++)
	  {  linfun[i] = new double[n];
	  }
	
	


	Function* m_f0;
	Ctc* ctc0;
	Function**  m_func= new Function* [p] ;	

	Variable v(3);

	clock_t start, start0, end;
	int nb_cells=0;
	start = clock();
	start0= clock();
	

	int Qoct=Q;
	int bestsolpointnumber=0;
	Vector bestsol (3);
	for (int oct=0; oct <4; oct++)

	  { 


	    Array<Ctc> m_ctc(p);
	    
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
		m_ctc.set_ref(i,m_ctc[i]);
		//		m_fun[0][i]= new Function(v,(x->at(i) +v[0]*(y->at(i)-diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-Interval(-epseq,epseq)));
		linfun[i][0]=x->at(i);
		linfun[i][1]=y->at(i)-diry*x->at(i);
		linfun[i][2]=z->at(i)-dirz*x->at(i);
	      }
	    }
	  

	    Vector prec(3);

	    // les precisions dans l'article   ICTAI


	    /*
	    prec[0]=0.000007;
	    prec[1]=0.000007;
	    prec[2]=0.0007;
	    */

	  
	    prec[0]=atof(argv[8]);
	    prec[1]=atof(argv[8]);
	    prec[2]=atof(argv[9]);

	    Vector proba(3);
	    proba[0]=0.33;
	    proba[1]=0.33;
	    proba[2]=0.34;
	    //CellStack buff;
	    CellHeapQInter buff;
	    BeamSearch str(buff);
	     //	    BestFirstSearch str (buff);
	    //	    DepthFirstSearch str (buff);
	    Bsc * bs;
	    if (bisect=="rr")
	      bs = new RoundRobin(prec, 0.5);
	    else if (bisect =="rr2")
	      bs= new RoundRobinNvar(2,prec,0.5);
	    else if (bisect=="lf")
	      bs = new LargestFirst(prec,0.5);
	    

            CtcQInter* ctcq;	    
	    //	    CtcQInter ctcq(3,m_ctc,Q);
	    //	    ctcq = new CtcQInterAff (3,m_ctc,Q,m_fun,QINTERPROJ,K);
	    
	    if (flist==1)
	      ctcq = new CtcQInterAffPlane (n,p,m_ctc,linfun,epseq,Qoct,QINTERPROJ);
	    else
	      //  ctcq= new  CtcQInterPlane (n,p,m_ctc,linfun,epseq,Qoct,QINTERPROJ,K);
	      ctcq= new  CtcQInter (n,m_ctc,Qoct,QINTERPROJ);
	    //	    CtcQInterPlane ctcq(n,m_ctc,linfun,epseq,Q,QINTERPROJ,K);
	    //	    CtcQInterPlane ctcq(n,m_ctc,linfun,epseq,Q,QINTERCORE,K);
	    //	    CtcQInterPlane  ctcq(n,m_ctc,linfun,epseq,Q, QINTERCORE,K );
	    //	    CtcQInterPlane ctcq(n,m_ctc,linfun,epseq,Q,QINTERFULL, K);

	    CtcCompo ctcqf0 (*ctc0,*ctcq);
	    Ctc3BCid cid(*ctcq,10,1,3);
	    CtcCompo ctcid(*ctcq ,cid);

	    //	    CtcCompo ctcqf0(*ctc0,ctcid);


	    CtcFixPoint ctcf(ctcqf0,0.1);
	    //CtcFixPoint ctcf(ctcid,0.1);

	  
	    Ctc* ctcs;
	    if (fixpoint==0) ctcs=&ctcqf0;
	    else ctcs=&ctcf;

	    SolverOptQInter s(*ctcs,*bs,str,*ctcq);
	    s.str.with_oracle=0;
	    //
	    
	    if (flist==1)
	      s.str.with_storage=true;
	    else
	      s.str.with_storage=false;
	    

	    s.str.with_storage=false;
	    s.timeout = 3600;
	    s.epsobj=1;
	    s.trace=1;
	    s.feasible_tries=nbrand;
	    s.gaplimit=gaplimit;
	    s.bestsolpointnumber=bestsolpointnumber;
	    s.bestsolpoint=bestsol;

	    IntervalVector res=s.solve(box);


	    cout << "Number of branches : " << s.nb_cells << endl;
	    cout << " bestsol " << s.bestsolpoint << endl;
	    nb_cells +=s.nb_cells;
	    cputime += s.time;
	    if (s.bestsolpointnumber){
            Qoct=s.bestsolpointnumber;
	    bestsol=s.bestsolpoint;
            bestsolpointnumber=s.bestsolpointnumber;
	    }


	    s.report_possible_inliers();
	    s.report_solution();	    

	    
	for (int i=0; i<p; i++)
	  { delete m_func[i];
	       delete &m_ctc[i];
	  }

	m_ctc.clear();

	cout << " end of octant " << endl;
	end = clock();
	totaltime += ((double)(end)-(double)(start))/CLOCKS_PER_SEC;
	start= clock();
	delete m_f0;
	delete ctc0;
	delete ctcq;
	delete bs;
	  }
	  				       
	end=clock();
	cout << "Shape extraction : OK. Time : " << ((double)(end)-(double)(start0))/CLOCKS_PER_SEC << " seconds" << endl;
	cout << " total time " << totaltime << endl;
	cout << " cpu time " << cputime << endl;
	cout <<" total branch number " << nb_cells << endl;

	for (int i=0; i<p; i++)
	      delete [] linfun[i];
	delete [] linfun;
	
	delete [] m_func;
	

};






