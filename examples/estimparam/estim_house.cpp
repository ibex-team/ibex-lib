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
	srand(2);

	cout << " nb points " << x->size() << endl;
	//	for (int i=0; i< x->size(); i++) cout << (*x)[i] <<  " " << (*y)[i] << " " << (*z)[i] <<endl;
	
	

	int n=3;
	
	Variable u(3);
	Variable w(3);
	
	int p = x->size();
	int nz=0;
	for (int i=0;i<p;i++) {if ((*z)[i] >=2.79 && (*z)[i] <3) nz++;      }
	cout << "nz " << nz << endl;
	  
	


	//	int Q2= Q*Qprop;
	int Q1=27;
	//	int Q1=9;
	//	int Q2= 4;
	int Q2= 3;
	//	int NT2= 5.01 / (p * (pow (Qprop,3)));
	//	int NT2= 4.5 / (p * (pow (Qprop,3))) + 1;
	int NT1=3;
	int NT2=5;
	//	int NT1= 12.01 / (p * (pow (Qprop,2)));
	//int NT1= 9.01 / (p * (pow (Qprop,2)));
	int K=1;
	int np;

	if (K==1)np=p;
	else if (K==2) {np=NT1*p;Q=Q1;}
	else {np=NT2*p;Q=Q2;}



	Function ***m_fun;
	double ** linfun;

	linfun = new double*[p];
	for (int i=0; i<p; i++)
	  {  linfun[i] = new double[n];
	  }
	
        m_fun=new Function **[K];
	for (int i=0; i<K; i++)
	  m_fun[i]=new Function*[np];


      
		   



	Array<Ctc> m_ctc(p);
	Array<Ctc> m_ctc1(np);

	Function* m_func[p] ;	
        Function* m_f0;
	Ctc* ctc0;
	Variable v(3);
	//	double R[p];
	//	double alpha[p];
	clock_t start, start0, end;
	int nb_cells=0;
	start = clock();
	start0=clock();

	int Qvalid=Q;
	cout << np << "  " << Q << " " << epseq << endl;

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


	    if (K==1) {
	      for (int i=0; i<p; i++)  {
	    /* We must be on the plane defined by v */
		m_ctc1.set_ref(i,m_ctc[i]);
		//		m_fun[0][i]= new Function(v,(x->at(i) +v[0]*(y->at(i)- diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-Interval(-epseq,epseq)));
		linfun[i][0]=x->at(i);
		linfun[i][1]=y->at(i)-diry*x->at(i);
		linfun[i][2]=z->at(i)-dirz*x->at(i);
	      }
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

	    vector<IntervalVector> resgroup;


	    Vector prec(3);
	    /*
	    prec[0]=0.0005;
	    prec[1]=0.0005;
	    prec[2]=0.005;
	    */
	    
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
	    CellStack buff;
	    //CellHeapQInter buff;
	    Bsc * bs;
	    if (bisect=="rr")
	      bs = new RoundRobin(prec, 0.5);
	    else if (bisect =="rr2")
	      bs= new RoundRobinNvar(2,prec,0.5);

	    CtcQInter* ctcq;
	    //	ProbaBisect bs (prec, proba, 0.45);
	    //LargestFirst bs (prec,0.45);
	    /* Main optimization loop */

	    //CtcQInterProjF ctcq(3,m_ctc1,Q);
	    //	CtcQInterProjF ctcq(3,m_ctc1,K,m_fun,Q);
	    
	    //	    CtcQInterAff ctcq(3,m_ctc1,Q,m_fun,QINTERPROJ,);
	    //CtcQInterAffPlane ctcq(n,p,m_ctc1,linfun,epseq,Q,QINTERPROJ);
	    //CtcQInterPlane ctcq(n,p,m_ctc1,linfun,epseq,Q,QINTERPROJ);

	    cout << " flist " << flist << endl;
	    if (flist==1)
	      ctcq = new CtcQInterAffPlane (n,p,m_ctc1,linfun,epseq,Q,QINTERPROJ);
	    else
	      ctcq= new  CtcQInterPlane (n,p,m_ctc1,linfun,epseq,Q,QINTERPROJ);

	    Ctc3BCid cid(*ctcq,5,1,1);
	    CtcCompo ctcid(*ctcq ,cid);
            CtcCompo ctcqf0(*ctc0,*ctcq);

	    CtcFixPoint ctcf(ctcqf0, 1);
	    //CtcFixPoint ctcf(ctcqf0, 0.1);
	    //CtcFixPoint ctcf(ctcid,0.1);


	    SolverQInter s(ctcf,*bs,buff,*ctcq);
	    

	    //	    Solver s(ctcf,*bs,buff);
	    //	    s.onesol_permaximalset=true;
	    //	    s.qvalid=Qvalid;
	    s.timeout = 10000;
            s.trace=0;
	    s.feasible_tries=nbrand;
	    s.gaplimit=gaplimit;

	    cout << "feasible_tries" <<  nbrand << " gaplimit " << gaplimit << endl;

	    vector<IntervalVector> res=s.solve(box);
	    
	    cout << "Number of branches : " << s.nb_cells << endl;
	    nb_cells +=s.nb_cells;
	    cputime += s.time;
	    cout << " nb sols " << res.size() << endl;   

	    s.report_possible_inliers();
	    vector <int> maxsolinliers;
	    s.keep_one_solution_pergroup(res,maxsolinliers );
	    
	    end = clock();
	    totaltime += ((double)(end)-(double)(start))/CLOCKS_PER_SEC;
	    start= clock();
	    
	  }

	
	for (int i=0; i<p; i++)
	  {delete m_func[i];
	    delete &m_ctc[i];
	  }

	  
	end=clock();
	cout << "Shape extraction : OK. Time : " << ((double)(end)-(double)(start0))/CLOCKS_PER_SEC << " seconds" << endl;
	cout << " total time " << totaltime << endl;
	cout << " cpu time " << cputime << endl;
	cout <<" total branch mumber " << nb_cells << endl;

	  				       


	
	for (int i=0; i<p; i++)
	      delete [] linfun[i];
	delete [] linfun;
		
}






