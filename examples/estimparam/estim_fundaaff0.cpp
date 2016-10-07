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
	double time0= atof(argv[6]);
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
        int n=8;
        Variable v(8);
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
	double *** linfuna;
	linfuna = new double**[p];
	for (int i=0; i<p; i++)
	  {  linfuna[i] = new double*[n+1];
	    for (int j=0; j<n+1; j++)
	      linfuna[i][j]= new double[K];
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
	m_det = new Function (v, v[0]*(v[4]-v[7]*v[5]) - v[3]*(v[1] - v[2]*v[7] ) + v[6]*(v[1]*v[5]-v[2]*v[4]));

	cdet = new CtcFwdBwd(*m_det);
	
	for (int i=0; i<p; i++) {
	  m_func[i]= new Function (v, (v[0]* (x1->at(i) * x2->at(i)) +
				   v[1]* (x1->at(i) * y2->at(i)) +
				   v[2] * x1->at(i) +
				   v[3] *  x2->at(i) * y1->at(i) +
				   v[4] * y1->at(i) * y2->at(i) +
				   v[5] * y1->at(i) +
				   v[6]* x2->at(i) + 
                                   v[7]* y2->at(i) + 
                                   Interval(1,1) + Interval(-epseq,epseq)));
          linfun[i][0][0]= -1/y2->at(i);
          linfun[i][1][0]=- (x1->at(i) * x2->at(i) / y2->at(i));
          linfun[i][2][0]= - x1->at(i) ;
          linfun[i][3][0]= -(x1->at(i)/ y2-> at(i)) ;
          linfun[i][4][0]= -(x2->at(i) * y1->at(i)/ y2->at(i));
          linfun[i][5][0]=- y1->at(i) ;
          linfun[i][6][0]=- (y1->at(i) / y2-> at(i));
          linfun[i][7][0]= -(x2->at(i) / y2->at(i)) ;

	  linfuna[i][8][0]=1;
          linfuna[i][0][0]=x1->at(i) * x2->at(i);
          linfuna[i][1][0]=x1->at(i) * y2->at(i);
          linfuna[i][2][0]=x1->at(i) ;
          linfuna[i][3][0]=x2->at(i) * y1->at(i);
          linfuna[i][4][0]=y1->at(i) * y2->at(i);
          linfuna[i][5][0]=y1->at(i);
          linfuna[i][6][0]=x2->at(i) ;
          linfuna[i][7][0]=y2->at(i) ;



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
	/*
	_box[0][0]= 0;
	_box[0][1]= 0.1;

	_box[1][0]=-0.1;
	_box[1][1]=0;
	_box[2][0]=0.3;
	_box[2][1]=0.4;
	_box[3][0]=0;
	_box[3][1]=0.1;
        _box[4][0]= 0;
        _box[4][1]= 0.1;
	_box[5][0]=-0.3;
	_box[5][1]=-0.2;

	_box[6][0]=-0.4;
	_box[6][1]=-0.3;

	_box[7][0]= 0.2;
	_box[7][1]= 0.3;
	*/

	/* problem USACtest5 
	_box[0][0]= -1.e-4;
	_box[0][1]= 0.;

	_box[1][0]=-1.e-4;
	_box[1][1]=0;
	_box[2][0]=1.e-3;
	_box[2][1]=2.e-3;
	_box[3][0]=0;
	_box[3][1]=1.e-4;
        _box[4][0]= -3.e-4;
        _box[4][1]= 0;
	_box[5][0]=-3.e-4;
	_box[5][1]=-2.e-4;
	_box[6][0]=-3.e-3;
	_box[6][1]=-2.e-3;

	_box[7][0]= 2.e-4;
	_box[7][1]= 3.e-4;
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
	*//*
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
	fundmat[3]=-0.325999;
        fundmat[6]= 1.91435;
        fundmat[1]=0.025682;
        fundmat[4]=-0.536157;
        fundmat[7]=  3.12895;
        fundmat[2]= -0.210514;
        fundmat[5]= 3.93254;
	fundmat[8]= -22.9974;
	*/

	/*  
	// tolerance 0.001
	fundmat[0]=-0.00174786;
	fundmat[3]=-0.652105;
        fundmat[6]= 3.12043;
        fundmat[1]=0.84431;
        fundmat[4]=-0.0153238;
        fundmat[7]=  -4.40509;
        fundmat[2]= -6.00301;
        fundmat[5]= 0.585943;
	fundmat[8]= 29.0443;
	*/
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
	 
	//meilleure solution à 169  tolerance 0.002
	/*
	fundmat[0]=-0.0004189112345737468;
	fundmat[3]=0.02703988017829571;
        fundmat[6]= -0.2426299406900574;
        fundmat[1]=-0.02723329876815711;
        fundmat[4]=-0.001256242961387644;
        fundmat[7]=0.08182133148953307;
        fundmat[2]=0.1537286266715119;
        fundmat[5]=-0.1550964303556178 ;
	fundmat[8]=1;
	*/
	
	//meilleure solution à 146  tolerance 0.001
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
	  {_box[i][0]= fundmat[i]/fundmat[8]-eps1;
	    _box[i][1]= fundmat[i]/fundmat[8]+eps1;
	  }

	IntervalVector box(8,_box);

	Vector prec(8);
        for (int j=0; j<8; j++)
	  prec[j]=prec0;

	CellStack buff;
	//	RoundRobinQInter bs (prec,0.5);
	//	RoundRobin bs (prec,0.5);
	//        LargestFirst bs(prec,0.5);
	RoundRobinNvar bs (7,prec,0.5);
	cout << " Q " << Q << endl;
	//CtcQInter ctcq(8,m_ctc,Q);
	//CtcQInterLinear ctcq(8,m_ctc,linfun,epseq,Q);
	CtcQInterAffLinear ctcq(8,m_ctc,linfun,epseq,Q);
	CtcCompo ctcqf0(*cdet,ctcq);
	cout << " avant solver " << endl;
	//	SolverQInter s(ctcqf0,bs,buff,ctcq);
	SolverQInter s(ctcqf0,bs,buff,ctcq,1);
	cout << " apres solver " << endl;
	s.time_limit = time0;
	s.trace=1;

	vector<IntervalVector> res=s.solve(box);
	
	cout << "Number of branches : " << s.nb_cells << endl;
	cout << "time used : " << s.time << endl;

	s.report_maximal_solutions(res);
	
	vector<int> inliers;
	
	s.keep_one_solution_pergroup(res,inliers);

        if (res.size() > 0) {
	cout << "best sol " << res[res.size()-1] << endl;
	
	cout << "nbvar " <<  ctcq.nb_var << endl;
	cout << " best sol point " ;
	for (int i=0; i< ctcq.nb_var; i++)
	  cout << s.bestsolpoint[i] << " ";
	cout << endl;

        int nbinliers=0;
	int sampsoninliers=0;
	double sampsontolerance= 0.015 * 0.015;
	cout << " inliers number " << endl;
        for (int i=0;i<p ; i++)
	  {double err = 1;
	    for (int j=0; j<8; j++)
	      err+=linfuna[i][j][0] * s.bestsolpoint[j];
	    /*
	    double errxc = s.bestsolpoint[0] * linfuna[i][6][0] + s.bestsolpoint[3] * linfuna[i][7][0] + s.bestsolpoint[6];
	    double erryc = s.bestsolpoint[1] * linfuna[i][6][0] + s.bestsolpoint[4] * linfuna[i][7][0] + s.bestsolpoint[7];
	    double errwc = s.bestsolpoint[2] * linfuna[i][6][0] + s.bestsolpoint[5] * linfuna[i][7][0] + 1;
	    */
	    double errxc = s.bestsolpoint[0] * linfuna[i][6][0] + s.bestsolpoint[1] * linfuna[i][7][0] + s.bestsolpoint[2];
	    double erryc = s.bestsolpoint[3] * linfuna[i][6][0] + s.bestsolpoint[4] * linfuna[i][7][0] + s.bestsolpoint[5];
	    double errwc = s.bestsolpoint[6] * linfuna[i][6][0] + s.bestsolpoint[7] * linfuna[i][7][0] + 1;
	    //	    cout << "errxc " << errxc << " erryc " << erryc << " errwc " << errwc << endl;
            double err1 = linfuna[i][2][0] * errxc + linfuna[i][5][0] * erryc + errwc;
	    double rx= s.bestsolpoint[0]* linfuna[i][2][0] + s.bestsolpoint[3] *  linfuna[i][5][0] + s.bestsolpoint[6];
            double ry=  s.bestsolpoint[1]* linfuna[i][2][0] + s.bestsolpoint[4] *  linfuna[i][5][0] + s.bestsolpoint[7];
            double err_sampson= err1*err1 / (errxc*errxc + erryc*erryc + rx * rx + ry* ry);
	    //	    cout << " err " << err << " err_sampson " << err_sampson << endl;
	    if (fabs(err1) < epseq)
	      nbinliers++;
	    if (err_sampson < sampsontolerance)
	      sampsoninliers++;
	  }

	cout << " inliers points :  " << nbinliers << " sampson inliers " << sampsoninliers <<  endl;
	}
	for (int i=0; i<p; i++)
	  {delete m_func[i];
	    delete &m_ctc[i];
	  }
	Timer::stop();
	cout <<  " time used " << s.time << "  "<< endl ;
	cout <<" total branch number " << s.nb_cells << endl;

 }
