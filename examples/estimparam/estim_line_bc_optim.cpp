#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
using namespace ibex;

 
/*
 * 
 * Finds the optimal  line (maximizing the number of inliers)
in noisy pictures using q-intersection. 
 *
 * 
 * 
 */






/*===========================================*/

const double MIN1 = -1+0.0005;								//Minimum value for x,y,r
const double MAX1 = 1;	


	 
int main(int argc, char** argv) {
	
	vector<double> *x = new vector<double>;
	vector<double> *y = new vector<double>;

	int NP = atoi(argv[1]);
	int Q = atoi(argv[2]);
        double epseq=atof(argv[3]);
	int flist = atoi(argv[4]);
	int fixpoint = atoi(argv[5]);
	int gaplimit= atoi(argv[6]);
	int nbrand= atoi(argv[7]);
	int sr= atoi(argv[8]);
	cout << " sr " << sr << endl;
	srand(sr);
	if (sr==0)
	  { string filename = argv[9];	
	    ifstream input(filename.c_str());
	    double in;
	    for (int i=0;i<NP;i++) {
	      input >> in;
	      x->push_back(in);
	      input >> in;
	      y->push_back(in);
	    }

	  }
	else

	  for (int i=0;i<NP;i++)
	    {
	      x->push_back(((double)rand()/(double)RAND_MAX)*100);
	      y->push_back(((double)rand()/(double)RAND_MAX)*100);
	    }

        
	

	cout << " nb points " << x->size() << endl;
	// for (int i=0; i< x->size(); i++) cout << (*x)[i] <<  " " << (*y)[i] << endl;

	double cputime=0;
	double totaltime=0;
	int n=2;
	
	int p = NP;


	int K=1;
	int np=p;

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

	
	Function* m_func[p] ;	
	Variable v(2);
	clock_t start, start0, end;
	int nb_cells=0;

	start = clock();

	start0= clock();
	
      
	int Qoct=Q;
	Vector bestsol (2);
	int bestsolval=0;

	for (int qua=0; qua <2; qua++){
	  int dir= pow(-1,qua%2);

	    Array<Ctc> m_ctc(p);
	    Array<Ctc> m_ctc1(np);

	    cout << " avant contraintes " << endl;
	    for (int i=0; i<p; i++) {
	      
	      	      m_func[i] = new Function(v,(dir*x->at(i) + v[0]*(y->at(i)- dir*x->at(i))
	      		      -v[1]-Interval(-epseq,epseq)));

	      m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));
	  

	
	    }

	for (int i=0; i<p; i++)  {
	    /* We must be on the line  defined by v */
		m_ctc1.set_ref(i,m_ctc[i]);

		//		m_fun[0][i]= new Function(v,(v[0]*x->at(i) +v[1]*y->at(i)-Interval(-epseq,epseq)));

		linfun[i][0][0]=dir*x->at(i);
		linfun[i][1][0]=y->at(i)- dir*x->at(i);

	}

	   

	double _box[2][2];

	
	   
	_box[0][0]=0;
	_box[0][1]=1;
	/*
	_box[1][0]=-100*sqrt(2);
	_box[1][1]=100*sqrt(2);
	*/
	_box[1][0]=-sqrt(2);
	_box[1][1]=sqrt(2);
	IntervalVector box(2,_box);

	vector<IntervalVector> resgroup;


	Vector prec(2);
	    /*
	    prec[0]=0.001;
	    prec[1]=0.001;
	    prec[2]=0.005;
	    */
	
	
	prec[0]=0.000007;
	prec[1]=0.0007;
	




	
	
	prec[0]=1.e-8;
	prec[1]=1.e-6;
	

	CellHeapQInter buff;
        BeamSearch str(buff);
	//	RoundRobin bs (prec,0.5);
		    
	RoundRobinNvar bs (1,prec,0.5);

	CtcQInter* ctcq;	 
	if (flist==1)
	  ctcq = new CtcQInterAffLine (n,m_ctc1,linfun,epseq,Qoct,QINTERPROJ);
	else
	  ctcq= new  CtcQInterLine (n,m_ctc1,linfun,epseq,Qoct,QINTERPROJ);	    
	    /* Main optimization loop */




	    CtcFixPoint ctcf(*ctcq,0.1);


	    Ctc* ctcs;
	    if (fixpoint==0) ctcs=ctcq;
	    else ctcs=&ctcf;
	    

	    SolverOptQInter s(*ctcs,bs,str,*ctcq);
	    //Solver s (*ctcs,bs,buff);
	    s.timeout = 1000;
	    s.str.with_oracle=0;
	    if (flist==1)
	      s.str.with_storage=true;
	    else
	      s.str.with_storage=false;
	    s.trace=1;
	    s.epsobj=1;
	    s.nbr=nbrand;
	    s.gaplimit=gaplimit;
	    s.bestsolpointnumber=bestsolval;
	    s.bestsolpoint=bestsol;

	    cout << " avant resolution " <<  box << endl;

	    IntervalVector res=s.solve(box);

	    cout << "Number of branches : " << s.nb_cells << endl;
	    cout << "time used : " << s.time << endl;
	    nb_cells +=s.nb_cells;
	    cputime += s.time;
	    if (s.bestsolpointnumber > Qoct) Qoct=s.bestsolpointnumber;
	    bestsolval=s.bestsolpointnumber;
	    bestsol=s.bestsolpoint;
	    cout << " bestsol " << bestsol << endl;
	    //	    cout << " remaining small boxes  " << s.possiblesols;
	    //	    if (s.possiblesols)    cout << " max possible inliers in small boxes " << s.qmax_epsboxes;
	    cout << endl;
	    
	    s.report_possible_inliers();
	    s.report_solution();
	  

	

	
	for (int i=0; i<p; i++)
	  {delete m_func[i];
	    delete &m_ctc[i];
	  }
	m_ctc.clear();
	m_ctc1.clear();

	}
	  				       
	end = clock();

	start= clock();

	cout << "Shape extraction : OK. Time : " << ((double)(end)-(double)(start0))/CLOCKS_PER_SEC << " seconds" << endl;
	cout <<" total branch number " << nb_cells << endl;
	for (int i=0; i<np; i++)
	  {for (int j=0; j<n; j++)
	      delete [] linfun[i][j];
	    
	    delete [] linfun[i];
	  }
	delete [] linfun;

	


};






