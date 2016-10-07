#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <set>
using namespace std;
using namespace ibex;

 int main(int argc, char** argv) {
	
	vector<double> *x1 = new vector<double>;
	vector<double> *y1 = new vector<double>;
	vector<double> *x2= new vector<double>;	
	vector<double> *y2= new vector<double>;
	string input_file_name=argv[1];
	int Q = atoi(argv[2]);
	double epseq = atof(argv[3]);
        double eps1 = atof(argv[4]);
	double prec0= atof(argv[5]);
	int gaplimit = atoi (argv[6]);
	int nbr = atoi (argv[7]);
	double time0= atof(argv[8]);
	cout << input_file_name << endl;
	ifstream input(input_file_name.c_str());
	while (!input.eof())
	  {double in;
	    input >> in;
	    x1->push_back(in);
	    //	    cout << in << " " ;
	    input >> in;
	    y1->push_back(in);
	    //	    cout << in << " " ;
	    input >> in;
            x2->push_back(in);
	    //	    cout << in << " " ;

            input >> in;
            y2->push_back(in);
	    //	    cout << in << " " << endl;
	  }
	x1->pop_back();y1->pop_back();x2->pop_back();y2->pop_back();
	cout << " fin lecture " << endl;
	int n=9;
        Variable v(3,3);
        
	int p = x1->size();
	cout << " p " << p << endl;
	int K=1;
    	double *** linfun;
	linfun = new double**[p];
	for (int i=0; i<p; i++)
	  {  linfun[i] = new double*[n];
	    for (int j=0; j<n; j++)
	      linfun[i][j]= new double[K];
	  }


	Array<Ctc> m_ctc(p);

	Function* m_func[p] ;	

	Function* m_norm;
	Ctc* ctcnorm;

	Function* m_det;
	Ctc* cdet;
	/*
        m_norm = new Function (v, sqr(v[0])+sqr(v[1])+sqr(v[2])+ sqr(v[3]) + sqr(v[4]) + sqr(v[5]) + sqr(v[6]) + sqr(v[7])
			       + sqr(v[8]) -1);
	ctcnorm = new CtcFwdBwd(*mnorm);
	*/
	//	m_det = new Function (v, v[0]*(v[4]-v[7]*v[5]) - v[3]*(v[1] - v[2]*v[7] ) + v[6]*(v[1]*v[5]-v[2]*v[4]));
	m_det = new Function (v, v[0][0]*(v[1][1]-v[2][1]*v[1][2]) - v[1][0]*(v[0][1] - v[0][2]*v[2][1] ) + v[2][0]*(v[0][1]*v[1][2]-v[0][2]*v[1][1]));
	cdet = new CtcFwdBwd(*m_det);
	
	for (int i=0; i<p; i++) {
	  m_func[i]= new Function (v, (v[0][0]* (x1->at(i) * x2->at(i)) +
				   v[0][1]* (x1->at(i) * y2->at(i)) +
				   v[0][2] * x1->at(i) +
				   v[1][0] *  x2->at(i) * y1->at(i) +
				   v[1][1] * y1->at(i) * y2->at(i) +
				   v[1][2] * y1->at(i) +
				   v[2][0]* x2->at(i) + 
                                   v[2][1]* y2->at(i) + 
                                   Interval(1,1) + Interval(-epseq,epseq)));
	  //  linfun[i][0][0]=13.7066 ;
	  //	  linfun[i][0][0]=17.9195;
	  linfun[i][0][0]=1;
          linfun[i][1][0]=x1->at(i) * x2->at(i);
          linfun[i][2][0]=x1->at(i) * y2->at(i);
          linfun[i][3][0]=x1->at(i) ;
          linfun[i][4][0]=x2->at(i) * y1->at(i);
          linfun[i][5][0]=y1->at(i) * y2->at(i);
          linfun[i][6][0]=y1->at(i);
          linfun[i][7][0]=x2->at(i) ;
          linfun[i][8][0]=y2->at(i) ;

	
	  m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));
	}

	cout << " apres contraintes " << endl;
	double _box[8][2];
	
	for (int j=0; j<8; j++){
	  _box[j][0] = -1;
	  _box[j][1] = 1;
	}
	

	/*
	_box[0][0]= 2.095476614233516998e-06;
	_box[0][1]= 2.096476614233516998e-06;

	_box[1][0]=-1.315413782616479304e-03 ;
	_box[1][1]=-1.31413782616479304e-03; 
	_box[2][0]=3.430335393555071222e-01;
	_box[2][1]=3.4310335393555071222e-01;
	_box[3][0]=1.318180671071483064e-03;
	_box[3][1]=1.319180671071483064e-03;
        _box[4][0]= 7.804479136707811334e-06;
        _box[4][1]= 7.814479136707811334e-06;
	_box[5][0]=-2.181323455965984981e-01;
	_box[5][1]=-2.171323455965984981e-01;

	_box[6][0]=-3.455621949424880768e-01;
	_box[6][1]=-3.4535621949424880768e-01;

	_box[7][0]= 2.131829225063142097e-01;
	_box[7][1]= 2.1321829225063142097e-01;
	/*
	
	_box[0][0]= 0;
	_box[0][1]= 2.096476614233516998e-02;

	_box[1][0]=-1.315413782616479304e-03 ;
	_box[1][1]=-1.31413782616479304e-04; 
	_box[2][0]=3.430335393555071222e-01;
	_box[2][1]=3.510335393555071222e-01;
	_box[3][0]=1.318180671071483064e-03;
	_box[3][1]=1.419180671071483064e-03;
        _box[4][0]= 7.804479136707811334e-06;
        _box[4][1]= 7.914479136707811334e-06;
	_box[5][0]=-2.181323455965984981e-01;
	_box[5][1]=-2.171323455965984981e-01;

	_box[6][0]=-3.455621949424880768e-01;
	_box[6][1]=-3.35621949424880768e-01;

	_box[7][0]= 2.131829225063142097e-01;
	_box[7][1]= 2.2321829225063142097e-01;
	*/



	// problem USAC_test_rescaled
	/*
	_box[0][0]= -0.03;
	_box[0][1]= -0.02;

	_box[1][0]=0.08;
	_box[1][1]=0.09;
	_box[2][0]=-0.21;
	_box[2][1]=-0.2;
	_box[3][0]=-0.015;
	_box[3][1]=-0.005;
        _box[4][0]= 0.02;
        _box[4][1]= 0.03;
	_box[5][0]=-0.04;
	_box[5][1]=-0.03;
	_box[6][0]=0.16;
	_box[6][1]=0.17;

	_box[7][0]=-0.48;
	_box[7][1]=-0.47;
	*/

	/*
	_box[0][0]=  -2.822661878117506754e-02-eps1;
	_box[0][1]= -2.822661878117506754e-02 + eps1;

	_box[1][0]= 8.668698362007185021e-02- eps1;
	_box[1][1]=8.668698362007185021e-02 + eps1;
	_box[2][0]=-2.005177270305623649e-01 - eps1;
	_box[2][1]=-2.005177270305623649e-01 + eps1;
	_box[3][0]= -1.002747740554128797e-02- eps1;
	_box[3][1]=-1.002747740554128797e-02 + eps1;
        _box[4][0]=2.232763912850601468e-02 - eps1;
	_box[4][1]=2.232763912850601468e-02 + eps1;
	_box[5][0]= -3.243381596418165591e-02- eps1;
	_box[5][1]= -3.243381596418165591e-02 + eps1;
	_box[6][0]= 1.675846041234200123e-01- eps1;
	_box[6][1]=1.675846041234200123e-01 + eps1;

	_box[7][0]=-4.730805917038985453e-01 - eps1;
	_box[7][1]=-4.730805917038985453e-01 + eps1;
	*/

	Vector fundmat(9);
	/*
	fundmat[0]=0.220969;
	fundmat[1]=-0.0943866;
        fundmat[2]=-1.06502;
        fundmat[3]=0.364431;
        fundmat[4]=-0.010772;
        fundmat[5]=-2.31193;
        fundmat[6]=-2.94925;
        fundmat[7]=0.29334;
	fundmat[8]=17.9195;
	*/
	/*	
	fundmat[0]=0.0775658;
	fundmat[1]=-0.232285;
        fundmat[2]= 0.617284;
        fundmat[3]=0.364431;
        fundmat[4]=-0.00260262;
        fundmat[5]= -2.06668;
        fundmat[6]=-2.686;
        fundmat[7]=0.353597;
	fundmat[8]=13.7066 ;


	*/
	/*
	fundmat[0]=0.0211328;
	fundmat[1]=-0.325999;
        fundmat[2]= 1.91435;
        fundmat[3]=0.025682;
        fundmat[4]=-0.536157;
        fundmat[5]=  3.12895;
        fundmat[6]= -0.210514;
        fundmat[7]= 3.93254;
	fundmat[8]= -22.9974 ;
	*/
	//        -0.0056749 -0.369115 2.0815 0.364431 -0.0163823 -2.09462 -3.27829 1.10643 13.5226 
	/*
	  fundmat[0]=-0.0056749;
	fundmat[3]=-0.369115;
        fundmat[6]= 2.0815;
        fundmat[1]= 0.364431;
        fundmat[4]=-0.0163823;
        fundmat[7]= -2.09462;
        fundmat[2]= -3.27829 ;
        fundmat[5]=  1.10643;
	fundmat[8]=  13.5226 ;
	*/

	fundmat[0]=0.00457429 ;
	fundmat[3]=0.287084;
        fundmat[6]=-1.59225;
        fundmat[1]= -0.28708;
        fundmat[4]=0.0154291;
        fundmat[7]= 1.6015;
        fundmat[2]= 2.50874;
        fundmat[5]= -0.794495;
	fundmat[8]= -10.3983 ;
	


	for (int i=0; i< 8; i++)
	  {_box[i][0]= fundmat[i]/fundmat[8] - eps1;
	    _box[i][1]= fundmat[i]/fundmat[8] + eps1;
	  }
	

	IntervalVector box(9,_box);

	Vector prec(9);
        for (int j=0; j<9; j++)
	  prec[j]=prec0;

	CellStack buff;
	//	RoundRobin bs (prec,0.5);
	//	RoundRobinQInter bs (prec,0.5);
	//        LargestFirst bs(prec,0.5);
	RoundRobinNvar bs (8,prec,0.5);
	cout << " Q " << Q << endl;
	//	CtcQInter ctcq(9,m_ctc,Q);

	CtcQInterLinear ctcq(9,m_ctc,linfun,epseq,Q);
	CtcCompo ctcqf0(*cdet,ctcq);
	cout << " avant solver " << endl;
        int m = ctcq.activepoints_contract_count(box);
	cout << " nb inliers " << m << endl;



	SolverQInter s(ctcqf0,bs,buff,ctcq,1);
	cout << " apres solver " << endl;
	s.time_limit = time0;
	s.trace=1;
	s.nbr=nbr;
	s.gaplimit=gaplimit;
	vector<IntervalVector> res=s.solve(box);

	cout << "Number of branches : " << s.nb_cells << endl;
	cout << "time used : " << s.time << endl;

	s.report_maximal_solutions(res);

	vector<int> inliers;
	s.keep_one_solution_pergroup(res, inliers);
	for (int i=0; i<p; i++)
	  {delete m_func[i];
	    delete &m_ctc[i];
	  }
	Timer::stop();
	cout <<  " time used " << s.time << "  "<< endl ;
	cout <<" total branch number " << s.nb_cells << endl;

 }
