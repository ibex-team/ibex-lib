#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <set>
using namespace std;
using namespace ibex;

 

/*
 * 
 * Detects plan in real 3d point clouds using q-intersection. 
 * 
 * 
 * 
 */


	 
int main(int argc, char** argv) {
	
	vector<double> *x = new vector<double>;
	vector<double> *y = new vector<double>;
	vector<double> *z = new vector<double>;


	double cputime =0;
	double totaltime=0;
	string input_file_name=argv[1];
        double epseq=atof(argv[2]);
	int Q= atoi(argv[3]);
	double precbc= atof(argv[4]);
	double precd=atof(argv[5]);
	int flist = atoi(argv[6]);
	int gaplimit= atoi(argv[7]);
	int nbrand= atoi(argv[8]);
	string bisect = argv[9];

	cout << input_file_name << endl;
	ifstream input(input_file_name.c_str());
	while (!input.eof())
	  {double in;
	    input >> in;
	    x->push_back(in);
	    //	    cout << in << " " ;
	    input >> in;
	    y->push_back(in);
	    //	    cout << in << " " ;
	    input >> in;
            z->push_back(in);
	    //	    cout << in << " " << endl;
            
	  }
	x->pop_back();y->pop_back();z->pop_back();
	srand(atoi(argv[10]));

	cout << " nb points " << x->size() << endl;
	//	for (int i=0; i< x->size(); i++) cout << (*x)[i] <<  " " << (*y)[i] << " " << (*z)[i] <<endl;
	
	

	int n=3;
	
	Variable u(3);
	Variable w(3);
	
	int p = x->size();
	int nz=0;
	for (int i=0;i<p;i++) {if ((*z)[i] >=2.79 && (*z)[i] <3) nz++;      }
	cout << "nz " << nz << endl;
	  


	int K=1;
	int np=p;


	double ** linfun;

	linfun = new double*[p];
	for (int i=0; i<p; i++)
	  {  linfun[i] = new double[n];
	  }

	

          



	Array<Ctc> m_ctc(p);
	Array<Ctc> m_ctc1(np);
	Function* m_func[p] ;	
        Function* m_f0;
	Ctc* ctc0;
	Variable v(3);
	/*
	double R[p];
	double alpha[p];
	*/
	clock_t start, start0, end;
	int nb_cells=0;
	start = clock();
	start0=clock();
	int Q0=Q;

	int Qvalid=Q;
	cout << np << "  " << Q0 << " " << Qvalid << " " << epseq << endl;

	int bestsolpointnumber=0;
	int Qoct=Q;
	Vector bestsol (3);
	for (int oct=0; oct <4; oct++)
	  { int diry= pow(-1,oct%2);
	    int dirz= pow(-1,oct/2);
	    m_f0= new Function(v, (diry*v[0]+dirz*v[1]-1));
            ctc0=new CtcFwdBwd(*m_f0,LEQ);
	    for (int i=0; i<p; i++) {
	  /*		
	  R[i] = sqrt( pow(x->at(i),2)+pow(y->at(i),2));
	  alpha[i] = atan(y->at(i)/x->at(i));
	  m_func[i] = new Function(v,sqrt(1-sqr(v[0]))*R[i]*cos(v[1]-alpha[i])+v[0]*z->at(i)-v[2]+Interval(-epseq,epseq));
	  */
	      
	      m_func[i] = new Function(v,(x->at(i) +v[0]*(y->at(i)- diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-v[2]-Interval(-epseq,epseq)));
	      m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));
	  
	  //	  m_ctc.set_ref(i,*new CtcCompo (*new CtcFwdBwd(*m_func[i]), * new CtcFwdBwd(*m_func[i], EQ, AFFINE2_MODE)));
	  //	  m_ctc.set_ref(i, (* new CtcFixPoint (* new CtcFwdBwd(*m_func[i]),0.2)));
	  //	  m_ctc.set_ref(i,*new Ctc3BCid(3,* new  CtcFwdBwd(*m_func[i]),5,1,3));  
	  //m_ctc.set_ref(i,*new CtcCompo (*new CtcFixPoint (* new CtcFwdBwd(*m_func[i]),0.2),  // cid seul ne marche pas
	  //					 *new Ctc3BCid(3,* new CtcFixPoint (* new CtcFwdBwd(*m_func[i]),0.2),5,1,3)));
	  //	  m_ctc.set_ref(i,*new CtcCompo (* new CtcFwdBwd(*m_func[i]),  // cid seul ne marche pas
	  //					 *new Ctc3BCid(3, * new CtcFwdBwd(*m_func[i]),5,1,3)));
	    }


	    for (int i=0; i<p; i++)  {
	    /* We must be on the plane defined by v */
		m_ctc1.set_ref(i,m_ctc[i]);
	
		linfun[i][0]=x->at(i);
		linfun[i][1]=y->at(i)-diry*x->at(i);
		linfun[i][2]=z->at(i)-dirz*x->at(i);
	      }

	    
	    double _box[3][2];

	    if (diry==1) {_box[0][0] = 0;
	      _box[0][1] = 1;}
	    else {_box[0][0] = -1;_box[0][1] = 0;}
	    if (dirz==1)
	      {_box[1][0] = 0; _box[1][1] = 1;}
	    else
	      {_box[1][0] = -1; _box[1][1] = 0;}
	    _box[2][0]=-100;
	    _box[2][1]=100;
	
	    IntervalVector box(3,_box);


	    Vector prec(3);
	   
	    prec[0]=precbc;
	    prec[1]=precbc;
	    prec[2]=precd;
	    


	    cout << "precision bissection " <<  prec[0] << " " << prec[1] << " " << prec[2] << endl;
	    /*
	    prec[0]=0.001;
	    prec[1]=0.001;
	    prec[2]=0.001;
	    */
	    Vector proba(3);
	    proba[0]=0.33;
	    proba[1]=0.33;
	    proba[2]=0.34;
	    //	    CellStack buff;
	    // DepthFirstSearch str (buff);
	    CellHeapQInter buff;
	    BeamSearch str(buff);
	    //	    BestFirstSearch str(buff);

	    Bsc * bs;
	    if (bisect=="rr")
	      bs = new RoundRobin(prec, 0.5);
	    else if (bisect=="rr2")
	      bs= new RoundRobinNvar(2,prec,0.5);


	    //bs = new LargestFirst (prec,0.5);
	    /* Main optimization loop */

	    CtcQInterAffPlane ctcq(n,p,m_ctc1,linfun,epseq,Qoct,QINTERPROJ);
	    	    
	    //	    Ctc3BCid cid(ctcq,5,1,1);
	    //	    CtcCompo ctcid(ctcq ,cid);
            CtcCompo ctcqf0(*ctc0,ctcq);

	    CtcFixPoint ctcf(ctcqf0, 1);
	    //CtcFixPoint ctcf(ctcqf0, 0.1);

	    SolverOptQInter s(ctcf,*bs,str,ctcq);

	    s.str.with_oracle=false;
	    s.str.with_storage=true;
	    s.timeout = 3600;
	    s.trace=1;
	    s.gaplimit=gaplimit;
	    s.feasible_tries=nbrand;

	    s.bestsolpointnumber=bestsolpointnumber;
	    s.bestsolpoint=bestsol;
	    cout << " avant resolution " << endl;
	    cout << " q " << ctcq.q << endl;
	   IntervalVector res=s.solve(box);

	cout << "Number of branches : " << s.nb_cells << endl;
	nb_cells +=s.nb_cells;
	cputime += s.time;
	if (s.bestsolpointnumber) {
	  Qoct=s.bestsolpointnumber+s.epsobj;
	  bestsol=s.bestsolpoint;
	  bestsolpointnumber=s.bestsolpointnumber;
	}
	s.report_possible_inliers();
	s.report_solution();


  
	end = clock();
	totaltime += ((double)(end)-(double)(start))/CLOCKS_PER_SEC;
	start= clock();
	delete bs;
	  }

	
	for (int i=0; i<p; i++)
	  {delete m_func[i];
	    delete &m_ctc[i];
	  }

	  
	end=clock();
	cout << "Shape extraction : OK. Time : " << ((double)(end)-(double)(start0))/CLOCKS_PER_SEC << " seconds" << endl;
	cout << " total time " << totaltime << endl;
	cout << " cpu time " << cputime << endl;
	cout <<" total branch number " << nb_cells << endl;

	  				       
	for (int i=0; i<p; i++)
	      delete [] linfun[i];
	delete [] linfun;


	
}






