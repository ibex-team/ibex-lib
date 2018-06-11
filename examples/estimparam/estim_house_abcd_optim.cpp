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
 * with internal constraint  a^2+b^2+c^2=1
 * 
 * 
 * 
 */




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
	string input_file_name=argv[1];
        double epseq=atof(argv[2]);
	int Q= atoi(argv[3]);
	double precbc= atof(argv[4]);
	double precd=atof(argv[5]);
	int flist = atoi(argv[6]);
	int gaplimit= atoi(argv[7]);
	int nbrand= atoi(argv[8]);
	string bisect = argv[9];
        srand (atoi (argv[10]));
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
	srand(1);

	cout << " nb points " << x->size() << endl;
	//	for (int i=0; i< x->size(); i++) cout << (*x)[i] <<  " " << (*y)[i] << " " << (*z)[i] <<endl;
	
	

	int n=4;
	
	//Variable u(3);
	//	Variable w(3);
	
	int p = x->size();
	int nz=0;
	for (int i=0;i<p;i++) {if ((*z)[i] >=2.79 && (*z)[i] <3) nz++;      }
	cout << "nz " << nz << endl;
	  


	int K=1;
	int np;

	if (K==1)np=p;


	double ** linfun;
       

	linfun = new double*[p];
	for (int i=0; i<p; i++)
	  {  linfun[i] = new double[n];
	  }
       	   



	Array<Ctc> m_ctc(p);
	Array<Ctc> m_ctc1(np);

	Function* m_func[p] ;	
        Function* m_f0;

	Variable v(4);
	double R[p];
	double alpha[p];
	clock_t start, start0, end;
	int nb_cells=0;
	start = clock();
	start0=clock();
	int Q0=Q;

	int Qvalid=Q;
	cout << np << "  " << Q0 << " " << Qvalid << " " << epseq << endl;
	Function* fnorm = new Function(v,sqr(v[0])+sqr(v[1])+sqr(v[2]) -1);
	CtcFwdBwd* ctcnorm = new CtcFwdBwd(*fnorm);
	SystemFactory fac;
	fac.add_var(v);
	fac.add_ctr(*fnorm);
	System sys(fac);
	for (int i=0; i<p; i++) {
	       m_func[i] = new Function(v,(v[0]*x->at(i)
				      +v[1]*y->at(i)+v[2]*z->at(i)-v[3]-Interval(-epseq,epseq)));

	       m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));

	   
	      for (int i=0; i<p; i++)  {
	    /* We must be on the plane defined by v */
		m_ctc1.set_ref(i,m_ctc[i]);
		
	      

	      linfun[i][0]=0;
		linfun[i][1]=x->at(i);
		linfun[i][2]=y->at(i);
		linfun[i][3]=z->at(i);
	      }
	}
	   
	 

	    double _box[4][2];
	    for (int j=0; j<3; j++) {
		_box[j][0] = -1;
		_box[j][1] = 1;
	    }

	    

	    _box[0][0]=0;
	    _box[3][0]=-100;
	    _box[3][1]=100;
	    
	    IntervalVector box(4,_box);

	    vector<IntervalVector> resgroup;


	    Vector prec(4);
	    /*
	    prec[0]=0.0005;
	    prec[1]=0.0005;
	    prec[2]=0.0005;
	    prec[3]=0.005;
	    */
	    
	    prec[0]=precbc;
	    prec[1]=precbc;
	    prec[2]=precbc;
	    prec[3]=precd;
	    


	    cout << "precision bissection " <<  prec[0] << " " << prec[1] << " " << prec[2] << " " << prec[3] <<  endl;
	    
	    CellHeapQInter buff;
	    BeamSearch str(buff);
	    //CellStack buff;
	    Bsc * bs;
	    if (bisect=="rr")
	      bs = new RoundRobin(prec, 0.5);
	    else if (bisect =="rr2")
	      bs= new RoundRobinNvar(3,prec,0.5);


	    //	ProbaBisect bs (prec, proba, 0.45);
	    //LargestFirst bs (prec,0.45);
	    /* Main optimization loop */

	    //CtcQInterProjF ctcq(3,m_ctc1,Q);
	    //	CtcQInterProjF ctcq(3,m_ctc1,K,m_fun,Q);
	    

	    CtcQInterAffPlane ctcq(n,p,m_ctc1,linfun,epseq,Q0,QINTERPROJ);

	    	    

	    //	    Ctc3BCid cid(n,ctcq,5,1,1);
	    //	    CtcCompo ctcid(ctcq ,cid);
            CtcCompo ctcqf0(*ctcnorm,ctcq);

	    CtcFixPoint ctcf(ctcqf0, 1);
	    //CtcFixPoint ctcf(ctcqf0, 0.1);
	    //CtcFixPoint ctcf(ctcid,0.1);

	    double epscont =1.e-4;
	    //	    SolverOptConstrainedQInter s(sys,ctcf,*bs,str,ctcq,epscont);
	    SolverOptQInter s(ctcf,*bs,str,ctcq);

	    s.str.with_oracle=false;
	    s.str.with_storage=true;
	    s.timeout = 3600;
	    s.trace=1;
	    s.feasible_tries=nbrand;
	    s.gaplimit=gaplimit;
	    //	    s.tolerance_constraints_number=10000;  // no second call for feasible point  for SolverOptConstrainedQInter
	    IntervalVector res=s.solve(box);

	    cout << "Number of branches : " << s.nb_cells << endl;
	    nb_cells +=s.nb_cells;
	    cputime += s.time;
	    cout << " nb sols " << res.size() << endl;   

  


	    s.report_possible_inliers();
	    s.report_solution();

	
	

	end = clock();
	totaltime += ((double)(end)-(double)(start))/CLOCKS_PER_SEC;
	start= clock();



	
	for (int i=0; i<p; i++)
	  {delete m_func[i];
	    delete &m_ctc[i];
	  }

	  
	end=clock();
	cout << "Shape extraction : OK. Time : " << ((double)(end)-(double)(start0))/CLOCKS_PER_SEC << " seconds" << endl;
	cout << " total time " << totaltime << endl;
	cout << " cpu time " << cputime << endl;
	cout <<" total branch number " << nb_cells << endl;

	delete bs;
	for (int i=0; i<p; i++)
	      delete [] linfun[i];
	delete [] linfun;
		

       
	
}






