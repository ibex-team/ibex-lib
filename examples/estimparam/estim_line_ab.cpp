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

const double Qprop = 0.1;							//Minimum ratio of consistent measurements
//const double Qprop = 0.015;							//Minimum ratio of consistent measurements
double epseq=0.001;                                                       // epsilon on plane constraint

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
	int Qi=0.1*NP;
	cout << " NP " << NP << " Q " << Qi << endl;
	srand(2);
	
	for (int i=0;i<NP;i++)
	  y->push_back(rand()%256);
       
	cout << y-> size() << endl;

	for (int i=0;i<Qi;i++)  {

	  //	  x->push_back(300- y->at(10*i) + (((double)rand()/(double)RAND_MAX)*epseq));
	    x->push_back(rand()%256);
	    x->push_back(- y->at(10*i+1)  + (((double)rand()/(double)RAND_MAX)*epseq));	
	    x->push_back(-200+ y->at(10*i+2) + (((double)rand()/(double)RAND_MAX)*epseq));	

	    //	    x->push_back( y->at(10*i+3)  + (((double)rand()/(double)RAND_MAX)*epseq));	
		    //	  	  	    x->push_back(320- y->at(10*i+4) + (((double)rand()/(double)RAND_MAX)*epseq));
	    /*

	    x->push_back(10 - y->at(65*i+5) + (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(5- y->at(65*i+6)  +  (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(15- y->at(65*i+7)  +  (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(330- y->at(65*i+8) + (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(20- y->at(65*i+9)  +  (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-210+ y->at(65*i+10)  +  (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(25 -y->at(65*i+11)  + (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(340- y->at(65*i+12) + (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(30 - y->at(65*i+13) + (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(35- y->at(65*i+14)  + (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(40- y->at(65*i+15)  + (((double)rand()/(double)RAND_MAX)*0.001));

	    x->push_back(350- y->at(65*i+16) + (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(3- y->at(65*i+17)  +  (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-220+ y->at(65*i+18)  + (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back( 12 + y->at(65*i+19)  + (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(360- y->at(65*i+20)  + (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(12 - y->at(65*i+21)  + (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(7- y->at(65*i+22)  + (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(17- y->at(65*i+23) + (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(370- y->at(65*i+24) + (((double)rand()/(double)RAND_MAX)*0.001));
	    for (int j=0; j<40; j++)  x->push_back(rand()%256);
	    */
	    for (int j=0; j<7 ; j++)  x->push_back(rand()%256);
            cout << x->size() <<  endl;
	}

	int K0=x->size();


	for (int i=K0; i< NP;i++) x->push_back(rand()%256);

	


	cout << " nb points " << x->size() << endl;
	for (int i=0; i< x->size(); i++) 
	  cout << (*x)[i] <<  " " << (*y)[i] << " "  <<endl;

	double time=0;

	int n=3;
	

	
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
	


	double *** linfun;
	
        m_fun=new Function **[K];
	for (int i=0; i<K; i++)
	  m_fun[i]=new Function*[np];
	
	linfun = new double**[np];
	for (int i=0; i<np; i++)
	  {  linfun[i] = new double*[n];
	    for (int j=0; j<n; j++)
	      linfun[i][j]= new double[K];
	  }
	

       

	Array<Ctc> m_ctc(p);
	Array<Ctc> m_ctc1(np);
	vector<bool> active;
	for (int i=0;i<np;i++)
	  active.push_back(true);

	Function* m_f0;
	Ctc* ctc0;
	Function* m_func[p] ;	
	Variable v(3);
	clock_t start, end;
	int nb_cells=0;
	start = clock();
	int Q0=Q;

        cout << " avant contraintes " << endl;
	for (int i=0; i<p; i++) {
	  //	  cout << " i " << i << endl;
	  m_func[i] = new Function(v,(v[0]*x->at(i)
				      +v[1]*y->at(i)-v[2]-Interval(-epseq,epseq)));
	  //	  cout << " apres m_func " << i << endl;
	  m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));
	  //	  cout << " apres m_ctc " << i << endl;
	}

	for (int i=0; i<p; i++)  {
	    /* We must be on the line defined by v */
	    m_ctc1.set_ref(i,m_ctc[i]);

		m_fun[0][i]= new Function(v,(v[0]*x->at(i) +v[1]*y->at(i)-Interval(-epseq,epseq)));

		linfun[i][0][0]=0;
		linfun[i][1][0]=x->at(i);
		linfun[i][2][0]=y->at(i);


	}
	
	    
	cout << " avant box " << endl;

	double _box[3][2];

	for (int j=0; j<2; j++) {
	  _box[j][0] = MIN1;
	  _box[j][1] = MAX1;
	}

	   
	_box[0][0]=-100;
	_box[0][1]=100;
	_box[1][0]=1;
	_box[1][1]=1.00000001;
	_box[2][0]=-500;
	_box[2][1]=500;
	
	IntervalVector box(3,_box);

	    vector<IntervalVector> resgroup;


	    Vector prec(3);
	    /*
	    prec[0]=0.001;
	    prec[1]=0.001;
	    prec[2]=0.005;

	    */
	    
	    prec[0]=0.00001;
	    prec[1]=1;
	    prec[2]=0.001;
	    
	    CellStack buff;
	    RoundRobin bs (prec,0.5);


	    //	    RoundRobinNvar bs (3,prec,0.5);
	    // RoundRobinQInter bs (2,prec,0.5);
	    //	ProbaBisect bs (prec, proba, 0.45);
	    // LargestFirst bs (prec,0.5);
	    /* Main optimization loop */

	    //	    CtcQInter ctcq(3,m_ctc1,Q);
	    //	    CtcQInterAff ctcq(3,m_ctc1,Q,m_fun,QINTERPROJ,K);
	    CtcQInter* ctcq;
	    if (flist==1)
	      ctcq = new CtcQInterAffPlane (n,m_ctc1,linfun,epseq,Q,QINTERPROJ,K);
	    else
	      ctcq= new  CtcQInterPlane (n,m_ctc1,linfun,epseq,Q,QINTERPROJ,K);

	    //	    CtcQInterPlane ctcq(3,m_ctc1,linfun,epseq,Q,flist,QINTERPROJ,K);
	    //	    CtcQInterPlane  ctcq(n,m_ctc1,linfun,epseq,Q,flist, QINTERCORE,K );
	    //CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,flist, CtcQInterGen::FULL, K);
	    //	    Ctc3BCid cid(3,ctcq,5,1,1);
	    //	    CtcCompo ctcid(ctcq ,cid);


	    CtcFixPoint ctcf(*ctcq,0.1);


	    Ctc* ctcs;
	    if (fixpoint==0) ctcs=ctcq;
	    else ctcs=&ctcf;
	    
	    //	    SolverQInter s(*ctcs,bs,buff,ctcq);
	    //	    SolverQInter s(ctcf,bs,buff,ctcq);
	    SolverQInter s(*ctcs,bs,buff,*ctcq);
	    //Solver s (*ctcs,bs,buff);
	    s.time_limit = 10000;
	    s.trace=0;
	    cout << " avant resolution " << endl;

	    vector<IntervalVector> res=s.solve(box);

	    cout << "Number of branches : " << s.nb_cells << endl;
	    cout << "time used : " << s.time << endl;
	    nb_cells +=s.nb_cells;
	
	    s.report_maximal_solutions(res);

	     //	     s.keep_one_solution_pergroup(res);
	
	     vector <int> maxsolinliers;
	     s.keep_one_solution_pergroup(res,maxsolinliers );
	  

	
	     for (int i=0; i<p; i++)
	  {delete m_func[i];
	    delete &m_ctc[i];
	  }
	m_ctc.clear();
	m_ctc1.clear();

	  
	  				       
	end = clock();
	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();
	cout <<  " time used " << s.time << "  "<< endl ;
	cout << "Shape extraction : OK. Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	cout <<" total branch number " << nb_cells << endl;
	for (int i=0; i<np; i++)
	  {for (int j=0; j<n; j++)
	      delete [] linfun[i][j];
	    
	    delete [] linfun[i];
	  }
	delete [] linfun;

	

}







