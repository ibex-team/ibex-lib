#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <set>
using namespace std;
using namespace ibex;

Interval matrixtrace (IntervalMatrix& M){
  Interval inter(0,0);
  if (M.nb_rows()==M.nb_cols())
    for (int k =0; k< M.nb_rows(); k++)
      inter+=M[k][k];
  return inter;
}


 int main(int argc, char** argv) {


   vector<double> x1;
   vector<double> y1;
   vector<double> x2;
	vector<double> y2;
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
	    x1.push_back(in);
	    //	    cout << in << " " ;
	    input >> in;
	    y1.push_back(in);
	    //	    cout << in << " " ;
	    input >> in;
            x2.push_back(in);
	    //	    cout << in << " " ;

            input >> in;
            y2.push_back(in);
	    //	    cout << in << " " << endl;
	  }
	x1.pop_back();y1.pop_back();x2.pop_back();y2.pop_back();
	
	cout << " fin lecture " << endl;
	int n=9;
        Variable v(3,3);
        
	int p = x1.size();
	cout << " p " << p << endl;
	
	/*
	for (int i=0; i<p ; i++){
	  x1[i]=x1[i]/1000;
	  x2[i]=x2[i]/1000;
	  y1[i]=y1[i]/1000;
	  y2[i]=y2[i]/1000;
	}
	*/

	int K=1;
    	double *** linfun;
	linfun = new double**[p];
	for (int i=0; i<p; i++)
	  {  linfun[i] = new double*[n+1];
	    for (int j=0; j<n+1; j++)
	      linfun[i][j]= new double[K];
	  }


	Array<Ctc> m_ctc(p);

	Function* m_func[p] ;	
	Function *** m_fun;
	m_fun=new Function **[K];
	for (int i=0; i<K; i++)
	  m_fun[i]=new Function*[p];

	Ctc* ctcnorm;

	Function* m_det;

	Ctc* cdet;


	
        Function* m_norm = new Function (v, sqr(v[0][0])+sqr(v[0][1])+sqr(v[0][2])+ sqr(v[1][0]) + sqr(v[1][1]) + sqr(v[1][2]) + sqr(v[2][0]) + sqr(v[2][1])+sqr(v[2][2]) -1 + Interval(-1.e-8,1.e-8) );
	ctcnorm = new CtcFwdBwd(*m_norm);
	
	//	m_det = new Function (v, v[0]*(v[4]-v[7]*v[5]) - v[3]*(v[1] - v[2]*v[7] ) + v[6]*(v[1]*v[5]-v[2]*v[4]));
	m_det = new Function (v, v[0][0]*(v[1][1]-v[2][1]*v[1][2]) - v[1][0]*(v[0][1] - v[0][2]*v[2][1] ) + v[2][0]*(v[0][1]*v[1][2]-v[0][2]*v[1][1]) + Interval(-1.e-2,1.e-2));
	cdet = new CtcFwdBwd(*m_det);
	//	m_essential = new Function(v, 2*v*transpose(v)*v - matrixtrace(v*transpose(v))*v);
	//	const ExprNode* e= v.transpose(v);
	//	const ExprNode* tr= e[0]+e[1]+e[2];
	cout << " avant lecture " << endl;
	try {
       	Function m_essential("essentialmatrix.txt");
	IntervalMatrix eps (3, 3, Interval(-1.e-6,1.e-6));
	Function m_essential1(v, m_essential(v) + eps);
	//	Function m_id(v, v + eps);
	Function m_id(v, v );
	cout << " apres lecture " << endl;

	double b1=939.5973154362416;
	double b3=600.0;
	double b5=b1;
	double b6=399;
	double a1=1/b1;
        double a3=1 - b3/b1;
	double a5=1/b5;
	double a6=-b6/b5;

	Matrix k1 (3,3);
	k1[0][0]=b1;
	k1[0][1]=0;
	k1[0][2]=b3;
	k1[1][0]=0;
	k1[1][1]=b5;
	k1[1][2]=b6;
	k1[2][0]=0;
	k1[2][1]=0;
	k1[2][2]=1;


        Matrix F (3,3);
	
	/*
	F[0][0]=-3.31924e-07 ;
	F[0][1]=-2.99341e-05;
	F[0][2]=0.0117221 ;
	F[1][0]=3.03704e-05;
	F[1][1]=2.37055e-07;
	F[1][2]=-0.0275756;
	F[2][0]=-0.0113157;
	F[2][1]=-0.0275756;
	F[2][2]=6.45807;
	*/

	
	/*
	F[2][2]=8.01696 ;
	F[0][0]=2.17745e-06 / F[2][2] ;
	F[0][1]=-2.19382e-05/ F[2][2];
	F[0][2]=0.0115024/ F[2][2] ;
	F[1][0]=2.12724e-05/ F[2][2];
	F[1][1]=-1.99877e-07 / F[2][2];
	F[1][2]=-0.020719/ F[2][2] ;
	F[2][0]=-0.0138558/ F[2][2];
	F[2][1]=0.00894409/ F[2][2];
	F[2][2]=1;
	*/

	F[0][0]=2.17745e-06 ;
	F[0][1]=-2.19382e-05;
	F[0][2]=0.0115024 ;
	F[1][0]=2.12724e-05;
	F[1][1]=-1.99877e-07 ;
	F[1][2]=-0.020719 ;
	F[2][0]=-0.0138558;
	F[2][1]=0.00894409;
	F[2][2]=8.01696 ;





        
	Matrix G (3,3);
        G=F;

	//	G= k1*F*k1.transpose();

	IntervalVector vec(9);
	vec[0]=Interval(G[0][0]);
	vec[1]=Interval(G[0][1]);
	vec[2]=Interval(G[0][2]);
	vec[3]=Interval(G[1][0]);
	vec[4]=Interval(G[1][1]);
	vec[5]=Interval(G[1][2]);
	vec[6]=Interval(G[2][0]);
	vec[7]=Interval(G[2][1]);
	vec[8]=Interval(G[2][2]);

	cout << "id G " << m_id.eval_matrix(vec) << endl;
	G= k1.transpose()*F*k1;

       	cout << "essential sol ransac " << m_essential1.eval_matrix(vec) << endl;
	cout << G[0][0] << "  " << G[0][1] <<  "  " << G[0][2] << endl;
	cout << G[1][0] << "  " << G[1][1] <<  "  " << G[1][2] << endl;
	cout << G[2][0] << "  " << G[2][1] <<  "  " << G[2][2] << endl;



	//Function m_essential(v, 2*e*v - tr*v);
	CtcFwdBwd c_essential (m_essential1);
	for (int i=0; i<p; i++) {
	  /*
	  m_func[i]= new Function (v, (v[0][0]* (x1.at(i) * x2.at(i)) +
				       v[0][1]* (x1.at(i) * y2.at(i)) +
				       v[0][2] * x1.at(i) +
				       v[1][0] *  x2.at(i) * y1.at(i) +
				       v[1][1] * y1.at(i) * y2.at(i) +
				       v[1][2] * y1.at(i) +
				       v[2][0]* x2.at(i) + 
				       v[2][1]* y2.at(i) + 
				       //                                   Interval(1,1) + Interval(-epseq,epseq)));
				       v[2][2]+Interval(-epseq,epseq)));
	  */

	  m_func[i]= new Function (v,(v[0][0]* (x1.at(i) * x2.at(i) * a1*a1  + x1.at(i) * a1 * a3 + x2.at(i) *a1* a3 + a3*a3 ) 
				      +  v[0][1] * (x1.at(i)*y2.at(i)*a1*a5 + x1.at(i)* a1* a6 + y2.at(i)*a3*a5 + a3*a6)
				      + v[0][2]*(x1.at(i) * a1 + a3) 
				      +  v[1][0]*(x2.at(i) *  y1.at(i) * a1*a5 + x2.at(i)* a1*a6 + y1.at(i) * a3*a5 + a6*a3)
				      +  v[1][1] * ( y1.at(i) * y2.at(i) * a5*a5 +  y1.at(i) * a5 * a6 + y2.at(i)*a5 * a6 + a6*a6)
				      +  v[1][2] * ( y1.at(i)* a5 + a6) 
				      +  v[2][0]* (x2.at(i) *a1+ a3) 
				      +  v[2][1] * ( y2.at(i)* a5 + a6) 
				      +  v[2][2]+Interval(-epseq,epseq)));

	  m_fun[0][i]=new Function (v,(-(v[0][0]* (x1.at(i) * x2.at(i) * a1*a1  + x1.at(i) * a1 * a3 + x2.at(i) *a1* a3 + a3*a3 ) 
				      +  v[0][1] * (x1.at(i)*y2.at(i)*a1*a5 + x1.at(i)* a1* a6 + y2.at(i)*a3*a5 + a3*a6)
				      + v[0][2]*(x1.at(i) * a1 + a3) 
				      +  v[1][0]*(x2.at(i) *  y1.at(i) * a1*a5 + x2.at(i)* a1*a6 + y1.at(i) * a3*a5 + a6*a3)
				      +  v[1][1] * ( y1.at(i) * y2.at(i) * a5*a5 +  y1.at(i) * a5 * a6 + y2.at(i)*a5 * a6 + a6*a6)
				      +  v[1][2] * ( y1.at(i)* a5 + a6) 
				      +  v[2][0]* (x2.at(i) *a1+ a3) 
					 +  v[2][1] * ( y2.at(i)* a5 + a6)) +Interval(-epseq,epseq)));
/*
	  linfun[i][0][0]=1;
          linfun[i][1][0]=x1.at(i) * x2.at(i);
          linfun[i][2][0]=x1.at(i) * y2.at(i);
          linfun[i][3][0]=x1.at(i) ;
          linfun[i][4][0]=x2.at(i) * y1.at(i);
          linfun[i][5][0]=y1.at(i) * y2.at(i);
          linfun[i][6][0]=y1.at(i);
          linfun[i][7][0]=x2.at(i) ;
          linfun[i][8][0]=y2.at(i) ;
*/
// 
	linfun[i][0][0]=0;
	linfun[i][1][0]= -(x1.at(i) * x2.at(i) * a1*a1 + x1.at(i) * a1 * a3 + x2.at(i) * a1* a3 + a3*a3);
	linfun[i][2][0]=-(x1.at(i)*y2.at(i)*a1*a5 + x1.at(i)* a1* a6 + y2.at(i)*a3*a5 + a3*a6);
	linfun[i][3][0]=-(x1.at(i) * a1 + a3);
	linfun[i][4][0]=-(x2.at(i) *  y1.at(i) * a1*a5 + x2.at(i)* a1*a6 + y1.at(i) * a3*a5 + a6*a3);
	linfun[i][5][0]=-( y1.at(i) * y2.at(i) * a5*a5 +  y1.at(i) * a5 * a6 + y2.at(i)* a5 * a6 + a6*a6);
	linfun[i][6][0]=-(y1.at(i)* a5 + a6);
	linfun[i][7][0]=-(x2.at(i)*a1 +a3);
	linfun[i][8][0]=-(y2.at(i)* a5 + a6) ;
	linfun[i][9][0]=-1;
	
	  m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));
	}

	cout << " apres contraintes " << endl;
	double _box[9][2];
	
	for (int j=0; j<9; j++){
	  _box[j][0] = -1;
	  _box[j][1] = 1;
	}
	

	



	Vector fundmat(9);

	fundmat[0]=0.0;
	fundmat[1]=0.0;
        fundmat[2]= 0.0;
        fundmat[3]=0.0;
        fundmat[4]=0.0;
        fundmat[5]= 0.0;
        fundmat[6]= 0.0;
        fundmat[7]= 0.0;
	fundmat[8]= 0.0 ;
	
	


	fundmat[0]=-3.31924e-07;
	fundmat[1]= -2.99341e-05;
	fundmat[2]= 0.0117221;
	fundmat[3]=3.03704e-05;
	fundmat[4]=2.37055e-07;
	fundmat[5]= -0.0275756 ;
	fundmat[6]= -0.0113157;
	fundmat[7]=0.00991355;
	fundmat[8]= 6.45807 ;

	/*
	fundmat[0]=0.25;
	fundmat[1]=0;
        fundmat[2]= 0.5;
        fundmat[3]=0.5;
        fundmat[4]=0.5;
        fundmat[5]= -0.625;
        fundmat[6]= -1.125;
        fundmat[7]= -0.5;
	fundmat[8]= 1.0 ;

	fundmat[0]=0.375;
	fundmat[1]= -0.625;
        fundmat[2]= 0.625;
        fundmat[3]= 1.125;
        fundmat[4]=0.25;
        fundmat[5]= -1.33984375125;
        fundmat[6]= -1.375;
        fundmat[7]= 0.25;
	fundmat[8]= 1.0 ;

	fundmat[0]=0.625;
	fundmat[1]= -1.875;
        fundmat[2]= 0.875;
        fundmat[3]= 1.875;
        fundmat[4]=0.;
        fundmat[5]= -1.08984375125;
        fundmat[6]= -1.625;
        fundmat[7]= 0.0;
	fundmat[8]= 1.0 ;
	*/

	/*
	fundmat[0]=G[0][0];
	fundmat[1]= G[0][1];
        fundmat[2]= G[0][2];
        fundmat[3]= G[1][0];
        fundmat[4]= G[1][1];
        fundmat[5]= G[1][2];
        fundmat[6]= G[2][0];
        fundmat[7]= G[2][1];
	fundmat[8]= G[2][2];
	*/
	/*
	fundmat[0]=G[0][0]/G[2][2];
	fundmat[1]= G[0][1]/G[2][2];
        fundmat[2]= G[0][2]/G[2][2];
        fundmat[3]= G[1][0]/G[2][2];
        fundmat[4]= G[1][1]/G[2][2];
        fundmat[5]= G[1][2]/G[2][2];
        fundmat[6]= G[2][0]/G[2][2];
        fundmat[7]= G[2][1]/G[2][2];
	fundmat[8]= 1;
	*/
	/*
	for (int i=0; i< 8; i++)
	  {_box[i][0]= fundmat[i]/fundmat[8] - eps1;
	    _box[i][1]= fundmat[i]/fundmat[8] + eps1;
	  }
	*/
	for (int i=0; i< 8; i++)
	  {_box[i][0]= fundmat[i] - eps1;
	    _box[i][1]= fundmat[i]+ eps1;
	  }
	_box[8][0]=1-1.e-8;
	_box[8][1]=1+1.e-8;
	/*	
	for (int i=0; i< 9; i++)
	  {_box[i][0]= - eps1;
	    _box[i][1]= eps1;
	  }
	*/
	IntervalVector box(9,_box);
	//	box[8]=Interval(1,1);
	cout << " box " << box << endl;
	Vector prec(9);
        for (int j=0; j<9; j++)
	  prec[j]=prec0;

	//CellHeapQInter buff;
	CellStack buff;
	//	RoundRobin bs (prec,0.5);
	//	RoundRobinQInter bs (prec,0.5);
	//        LargestFirst bs(prec,0.5);
	RoundRobinNvar bs (8,prec,0.5);
	cout << " Q " << Q << endl;
	//	CtcQInter ctcq(9,m_ctc,Q);
	//	CtcQInterAff ctcq(9,m_ctc,Q,m_fun);

	CtcQInterAffLinear ctcq(9,m_ctc,linfun,epseq,Q);
	//	CtcCompo ctcqf0(*cdet,c_essential, ctcq);
	CtcCompo ctcqf0(*ctcnorm,*cdet,c_essential, ctcq);
	//CtcCompo ctcqf0(*cdet, c_essential,  ctcq);

	//CtcCompo ctcqf0(*cdet,  ctcq);
	CtcFixPoint ctcf(ctcqf0,0.1);

	cout << " solution ransac " <<  ctcq.activepoints_count(vec) << endl;

	
	cout << " avant solver " << endl;



        int m = ctcq.activepoints_contract_count(box);
	cout << " nb inliers " << m << endl;


	//	SolverQInter s(ctcqf0,bs,buff,ctcq,1);
	//	SolverOptQInter s(ctcqf0,bs,buff,ctcq,1);
        SolverOptQInter s(ctcf,bs,buff,ctcq,1);
	cout << " apres solver " << endl;
	s.time_limit = time0;
	s.trace=1;
	s.nbr=nbr;
	s.func=& m_essential1;
	s.gaplimit=gaplimit;
	cout << "box init " << box << endl;

	cout << "essential " << m_essential1.eval_matrix(box) << endl;
	c_essential.contract(box);
	cout << " box apres essential  " << box << endl;
	cout << "essential apres contract " << m_essential1.eval_matrix(box) << endl;

	cout << "determinant " << m_det->eval(box);
	vector<IntervalVector> res=s.solve(box);

	cout << "Number of branches : " << s.nb_cells << endl;
	cout << "time used : " << s.time << endl;

	s.report_maximal_solutions(res);



	if (res.size() > 0) {
	  cout << "best sol " << res[res.size()-1] << endl;
	}

	cout << " best sol point " ;
	for (int i=0; i< ctcq.nb_var; i++)
	  cout << s.bestsolpoint[i] << " ";
	cout << endl;
	IntervalVector bestsolbox(s.bestsolpoint);
	cout << "essential " << m_essential1.eval_matrix(bestsolbox);
	cout << "determinant " << m_det->eval(bestsolbox);

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
	catch (SyntaxError e)
	  {cout <<e << endl;}
 }
