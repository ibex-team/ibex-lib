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
	 vector<double> x11;
   vector<double> y11;
   vector<double> x22;
	vector<double> y22;
	string input_file_name=argv[1];
	int nbp = atoi(argv[2]);
	string calib_file_name=argv[3];
	//	string oracle_file_name=argv[4];
	
	
	int Q = atoi(argv[4]);
	double epseq = atof(argv[5]);
        double eps1 = atof(argv[6]);
	double prec0= atof(argv[7]);
	double epscont= atof(argv[8]);
	int gaplimit = atoi (argv[9]);
	int nbr = atoi (argv[10]);


	int dmax= atoi (argv[11]);
	int eobj= atoi (argv[12]);

	int optim = atoi(argv[13]);
	double time0= atof(argv[14]);
	srand (atoi(argv[15]));

	cout << input_file_name << endl;
	ifstream input(input_file_name.c_str());
	int nb_pairs;
	input >> nb_pairs;
	cout << "nb_pairs" << nb_pairs << endl;
	while (!input.eof())
	  {double in;
	    input >> in;
	    x11.push_back(in);
	    //	    cout << in << " " ;
	    input >> in;
	    y11.push_back(in);
	    //	    cout << in << " " ;
	    input >> in;
            x22.push_back(in);
	    //	    cout << in << " " ;
            input >> in;
            y22.push_back(in);
	    //	    cout << in << " " << endl;
	  }
	x11.pop_back();y11.pop_back();x22.pop_back();y22.pop_back();
	for (int i=0; i< x11.size() ; i++){
	  int present=0;
	  for (int j=0; j<i; j++)
	    if (x11[i]==x11[j] && y11[i]==y11[j]&& x22[i]==x22[j] &&  y22[i]==y22[j]){
	      present=1; break;}
	  if (present==0){
	    x1.push_back(x11[i]); 
	    y1.push_back(y11[i]); 
            x2.push_back(x22[i]); 
	    y2.push_back(y22[i]);
	  }
	}
	
	cout << "nb points sans doublons" << x1.size() << endl;


	cout << " fin lecture points " << endl;
	cout << calib_file_name << endl;
	ifstream calib(calib_file_name.c_str());
	double b1,b2,b3,b5,b6;
	double b11,b12,b13,b15,b16;
	while (!calib.eof())
	  {calib >> b1;
	    calib >> b2;
            calib >> b3;
	    calib >> b5;
            calib >> b6;
	    calib >> b11;
	    calib >> b12;
            calib >> b13;
	    calib >> b15;
            calib >> b16;
	  }
	
	/*
	ifstream oraclefile(oracle_file_name.c_str());
	Vector  oraclemat(9);
	for (int i=0; i<9; i++)
	  oraclefile >> oraclemat[i];
	*/
	cout << "b1 " << b1;
	cout << " b2 " << b2;
	cout << " b3 " << b3;
	cout << " b5 " << b5;
	cout << " b6 " << b6 << endl;
	cout << "b11 " << b11;
	cout << " b12 " << b12;
	cout << " b13 " << b13;
	cout << " b15 " << b15;
	cout << " b16 " << b16 << endl;


	int n=9;
        Variable v(3,3);
        SystemFactory fac;
	SystemFactory fac0;
        SystemFactory fac1;
	fac.add_var(v);
	fac0.add_var(v);
	fac1.add_var(v);
	Interval eps2 (-epscont,epscont);

	Function fessential1 (v, v[0][0]*(sqr(v[0][0])+sqr(v[0][1])+sqr(v[0][2]) + sqr(v[1][0])  + sqr(v[2][0]) -( sqr(v[1][1])+sqr(v[1][2])+ sqr(v[2][1])+sqr(v[2][2]))) + 2* (v[1][0]*v[0][1]*v[1][1] + v[1][0]*v[0][2]*v[1][2]  + v[0][1]*v[2][0]*v[2][1] + v[0][2]*v[2][0]*v[2][2]) + eps2);
	Function fessential2(v, v[0][1]*(sqr(v[0][0])+sqr(v[0][1])+sqr(v[0][2]) + sqr(v[1][1])+sqr(v[2][1]) - (sqr(v[1][0]) + sqr(v[1][2])+ sqr(v[2][0])+ sqr(v[2][2]))) + 2*(v[0][0]*v[1][0]*v[1][1] + v[1][1]*v[0][2]*v[1][2] + v[0][0]*v[2][0]*v[2][1] + v[0][2]*v[2][1]*v[2][2])+eps2);
	Function fessential3(v, v[0][2]*(sqr(v[0][0])+sqr(v[0][1])+sqr(v[0][2]) + sqr(v[1][2])+sqr(v[2][2]) -( sqr(v[1][0]) + sqr(v[1][1]) + sqr(v[2][0])+ sqr(v[2][1]))) + 2*(v[0][0]*v[1][0]*v[1][2] + v[0][1]*v[1][1]*v[1][2]+ v[0][0]*v[2][0]*v[2][2] + v[0][1]*v[2][1]*v[2][2])+eps2);
	Function fessential4(v, v[1][0]*(sqr(v[0][0])+sqr(v[1][0])+sqr(v[1][1])+sqr(v[1][2]) + sqr(v[2][0]) - (sqr(v[0][1])+sqr(v[0][2]) + sqr(v[2][1])+sqr(v[2][2]))) + 2*(v[0][0]*v[0][1]*v[1][1] + v[0][0]*v[0][2]*v[1][2] + v[2][0]*v[1][1]*v[2][1] + v[1][2]*v[2][0]*v[2][2])+ eps2)
;
	Function fessential5(v, v[1][1]*(sqr(v[0][1])+sqr(v[1][0])+sqr(v[1][1])+sqr(v[1][2]) + sqr(v[2][1]) - (sqr(v[0][0])+ sqr(v[0][2]) + sqr(v[2][0])+sqr(v[2][2]))) + 2*( v[0][0]*v[0][1]*v[1][0] + v[0][1]*v[0][2]*v[1][2]+ v[1][0]*v[2][0]*v[2][1] + v[1][2]*v[2][1]*v[2][2])+ eps2);
	Function fessential6(v, v[1][2]*(sqr(v[0][2])+sqr(v[1][0])+sqr(v[1][1])+sqr(v[1][2]) + sqr(v[2][2])- (sqr(v[0][0])+ sqr(v[0][1])+  sqr(v[2][0])+sqr(v[2][1]))) + 2*(v[0][0]*v[0][2]*v[1][0] + v[0][1]*v[0][2]*v[1][1] + v[1][0]*v[2][0]*v[2][2]+ v[1][1]*v[2][1]*v[2][2])+eps2);
	Function fessential7(v, v[2][0]*( sqr(v[0][0])+sqr(v[1][0])+ sqr(v[2][0])+sqr(v[2][1])+sqr(v[2][2])- (sqr(v[0][1])+sqr(v[0][2])+  sqr(v[1][1])+sqr(v[1][2]))) + 2*(v[0][0]*v[0][1]*v[2][1] + v[0][0]*v[0][2]*v[2][2] + v[1][0]*v[1][1]*v[2][1] + v[1][0]*v[1][2]*v[2][2])+eps2);
	Function fessential8(v, v[2][1]*(sqr(v[0][1])+sqr(v[1][1])+ sqr(v[2][0])+sqr(v[2][1])+sqr(v[2][2])- (sqr(v[0][0])+ sqr(v[0][2]) +sqr(v[1][0])+sqr(v[1][2])) ) + 2*(v[0][0]*v[0][1]*v[2][0] + v[0][1]*v[0][2]*v[2][2] + v[1][0]*v[1][1]*v[2][0] + v[1][1]*v[1][2]*v[2][2])+ eps2);
	Function fessential9(v, v[2][2]*(sqr(v[0][2])+sqr(v[1][2]) + sqr(v[2][0])+sqr(v[2][1])+sqr(v[2][2])-(sqr(v[0][0])+sqr(v[0][1])+sqr(v[1][0])+sqr(v[1][1]))) + 2 * (v[0][0]*v[0][2]*v[2][0]+ v[0][1]*v[0][2]*v[2][1] + v[1][0]*v[1][2]*v[2][0] + v[1][1]*v[1][2]*v[2][1])+eps2);

	Function fessential11 (v, v[0][0]*(sqr(v[0][0])+sqr(v[0][1])+sqr(v[0][2]) + sqr(v[1][0])  + sqr(v[2][0]) -( sqr(v[1][1])+sqr(v[1][2])+ sqr(v[2][1])+sqr(v[2][2]))) + 2* (v[1][0]*v[0][1]*v[1][1] + v[1][0]*v[0][2]*v[1][2]  + v[0][1]*v[2][0]*v[2][1] + v[0][2]*v[2][0]*v[2][2]));
	Function fessential12(v, v[0][1]*(sqr(v[0][0])+sqr(v[0][1])+sqr(v[0][2]) + sqr(v[1][1])+sqr(v[2][1]) - (sqr(v[1][0]) + sqr(v[1][2])+ sqr(v[2][0])+ sqr(v[2][2]))) + 2*(v[0][0]*v[1][0]*v[1][1] + v[1][1]*v[0][2]*v[1][2] + v[0][0]*v[2][0]*v[2][1] + v[0][2]*v[2][1]*v[2][2]));
	Function fessential13(v, v[0][2]*(sqr(v[0][0])+sqr(v[0][1])+sqr(v[0][2]) + sqr(v[1][2])+sqr(v[2][2]) -( sqr(v[1][0]) + sqr(v[1][1]) + sqr(v[2][0])+ sqr(v[2][1]))) + 2*(v[0][0]*v[1][0]*v[1][2] + v[0][1]*v[1][1]*v[1][2]+ v[0][0]*v[2][0]*v[2][2] + v[0][1]*v[2][1]*v[2][2]));
	Function fessential14(v, v[1][0]*(sqr(v[0][0])+sqr(v[1][0])+sqr(v[1][1])+sqr(v[1][2]) + sqr(v[2][0]) - (sqr(v[0][1])+sqr(v[0][2]) + sqr(v[2][1])+sqr(v[2][2]))) + 2*(v[0][0]*v[0][1]*v[1][1] + v[0][0]*v[0][2]*v[1][2] + v[2][0]*v[1][1]*v[2][1] + v[1][2]*v[2][0]*v[2][2]));
	Function fessential15(v, v[1][1]*(sqr(v[0][1])+sqr(v[1][0])+sqr(v[1][1])+sqr(v[1][2]) + sqr(v[2][1]) - (sqr(v[0][0])+ sqr(v[0][2]) + sqr(v[2][0])+sqr(v[2][2]))) + 2*( v[0][0]*v[0][1]*v[1][0] + v[0][1]*v[0][2]*v[1][2]+ v[1][0]*v[2][0]*v[2][1] + v[1][2]*v[2][1]*v[2][2]));
	Function fessential16(v, v[1][2]*(sqr(v[0][2])+sqr(v[1][0])+sqr(v[1][1])+sqr(v[1][2]) + sqr(v[2][2])- (sqr(v[0][0])+ sqr(v[0][1])+  sqr(v[2][0])+sqr(v[2][1]))) + 2*(v[0][0]*v[0][2]*v[1][0] + v[0][1]*v[0][2]*v[1][1] + v[1][0]*v[2][0]*v[2][2]+ v[1][1]*v[2][1]*v[2][2]));
	Function fessential17(v, v[2][0]*( sqr(v[0][0])+sqr(v[1][0])+ sqr(v[2][0])+sqr(v[2][1])+sqr(v[2][2])- (sqr(v[0][1])+sqr(v[0][2])+  sqr(v[1][1])+sqr(v[1][2]))) + 2*(v[0][0]*v[0][1]*v[2][1] + v[0][0]*v[0][2]*v[2][2] + v[1][0]*v[1][1]*v[2][1] + v[1][0]*v[1][2]*v[2][2]));
	Function fessential18(v, v[2][1]*(sqr(v[0][1])+sqr(v[1][1])+ sqr(v[2][0])+sqr(v[2][1])+sqr(v[2][2])- (sqr(v[0][0])+ sqr(v[0][2]) +sqr(v[1][0])+sqr(v[1][2])) ) + 2*(v[0][0]*v[0][1]*v[2][0] + v[0][1]*v[0][2]*v[2][2] + v[1][0]*v[1][1]*v[2][0] + v[1][1]*v[1][2]*v[2][2]));
	Function fessential19(v, v[2][2]*(sqr(v[0][2])+sqr(v[1][2]) + sqr(v[2][0])+sqr(v[2][1])+sqr(v[2][2])-(sqr(v[0][0])+sqr(v[0][1])+sqr(v[1][0])+sqr(v[1][1]))) + 2 * (v[0][0]*v[0][2]*v[2][0]+ v[0][1]*v[0][2]*v[2][1] + v[1][0]*v[1][2]*v[2][0] + v[1][1]*v[1][2]*v[2][1]));



        
	/*
	for (int i=0; i<p ; i++){
	  x1[i]=x1[i]/1000;
	  x2[i]=x2[i]/1000;
	  y1[i]=y1[i]/1000;
	  y2[i]=y2[i]/1000;
	}
	*/


	int p;
	if (nbp>0) p=nbp;
	else
	  p=x1.size();
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
	Function* m_det1;

	Ctc* cdet;

        Function* m_norm = new Function (v, sqr(v[0][0])+sqr(v[0][1])+sqr(v[0][2])+ sqr(v[1][0]) + sqr(v[1][1]) + sqr(v[1][2]) + sqr(v[2][0]) + sqr(v[2][1])+sqr(v[2][2]) -1 + eps2);
	Function* m_norm1 = new Function (v, sqr(v[0][0])+sqr(v[0][1])+sqr(v[0][2])+ sqr(v[1][0]) + sqr(v[1][1]) + sqr(v[1][2]) + sqr(v[2][0]) + sqr(v[2][1])+sqr(v[2][2]) -1 );

	ctcnorm = new CtcFwdBwd(*m_norm);
	
	//	m_det = new Function (v, v[0]*(v[4]-v[7]*v[5]) - v[3]*(v[1] - v[2]*v[7] ) + v[6]*(v[1]*v[5]-v[2]*v[4]));
	m_det = new Function (v, v[0][0]*(v[1][1]*v[2][2]-v[2][1]*v[1][2]) - v[1][0]*(v[0][1]*v[2][2] - v[0][2]*v[2][1] ) + v[2][0]*(v[0][1]*v[1][2]-v[0][2]*v[1][1]) + eps2 );
	m_det1 = new Function (v, v[0][0]*(v[1][1]*v[2][2]-v[2][1]*v[1][2]) - v[1][0]*(v[0][1]*v[2][2] - v[0][2]*v[2][1] ) + v[2][0]*(v[0][1]*v[1][2]-v[0][2]*v[1][1]) );
	cdet = new CtcFwdBwd(*m_det);
	//	m_essential = new Function(v, 2*v*transpose(v)*v - matrixtrace(v*transpose(v))*v);
	//	const ExprNode* e= v.transpose(v);
	//	const ExprNode* tr= e[0]+e[1]+e[2];

	
	fac.add_ctr(fessential1);
        fac.add_ctr(fessential2);
	fac.add_ctr(fessential3);
	//        fac.add_ctr(fessential4);
	//        fac.add_ctr(fessential5);
	//        fac.add_ctr(fessential6);
	//        fac.add_ctr(fessential7);
	//        fac.add_ctr(fessential8);
	//        fac.add_ctr(fessential9);
	fac.add_ctr(*m_norm);
	fac.add_ctr(*m_det);
	
	fac0.add_ctr(fessential1);
        fac0.add_ctr(fessential2);
	fac0.add_ctr(fessential3);
	fac0.add_ctr(fessential4);
	fac0.add_ctr(fessential5);
	fac0.add_ctr(fessential6);
	fac0.add_ctr(fessential7);
	fac0.add_ctr(fessential8);
	fac0.add_ctr(fessential9);
	fac0.add_ctr(*m_norm);
	fac0.add_ctr(*m_det);
	

	fac1.add_ctr(fessential11);
        fac1.add_ctr(fessential12);
        fac1.add_ctr(fessential13);
	fac1.add_ctr(fessential14);
	fac1.add_ctr(fessential15);
	fac1.add_ctr(fessential16);
	fac1.add_ctr(fessential17);
	fac1.add_ctr(fessential18);
	fac1.add_ctr(fessential19);
	fac1.add_ctr(*m_det1);
	fac1.add_ctr(*m_norm1);

	System sys (fac);
	System sys0 (fac0);
	System sys1 (fac1);

	
	
	
	vector<LinearRelaxXTaylor::corner_point> cpoints;
	cpoints.push_back(LinearRelaxXTaylor::RANDOM);
	cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);
	
	//	LinearRelaxCombo lr (sys,LinearRelaxCombo::COMPO);
	LinearRelaxCombo lr (sys,LinearRelaxCombo::ART);
	//   LinearRelaxXTaylor lr (sys,cpoints);
	CtcPolytopeHull lphull (lr, CtcPolytopeHull::ALL_BOX);


	cout << " avant lecture " << endl;
	try {
       	Function m_essential("essentialmatrix.txt");
	IntervalMatrix eps (3, 3, eps2);
	//	IntervalMatrix eps (3, 3, Interval(-1.e-8,1.e-8));
	//	IntervalMatrix eps (3, 3, Interval(0,0));
	Function m_essential1(v, m_essential(v) + eps);

	Function m_id(v, v );
	cout << " apres lecture " << endl;

	
	double a1=1/b1;
        double a3=- b3/b1;
	double a5=1/b5;
	double a6=-b6/b5;
	double a11=1/b11;
        double a13=- b13/b11;
	double a15=1/b15;
	double a16=-b16/b15;
        cout << a1 << endl;
        cout << a3 << endl;
        cout << a5 << endl;
        cout << a6 << endl;

	/*
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


	Matrix G (3,3);
	G=F;



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
	*/
	//	cout << "id G " << m_id.eval_matrix(vec) << endl;
	
	//	G= k1.transpose()*F*k1;

	/*	
       	cout << "essential sol ransac " << m_essential1.eval_matrix(vec) << endl;
	cout << G[0][0] << "  " << G[0][1] <<  "  " << G[0][2] << endl;
	cout << G[1][0] << "  " << G[1][1] <<  "  " << G[1][2] << endl;
	cout << G[2][0] << "  " << G[2][1] <<  "  " << G[2][2] << endl;
	*/


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

	  m_func[i]= new Function (v,(v[0][0]* (x1.at(i) * x2.at(i) * a1*a11  + x1.at(i) * a1 * a13 + x2.at(i) *a11* a3 + a3*a13 ) 
				      +  v[0][1] * (x1.at(i)*y2.at(i)*a1*a15 + x1.at(i)* a1* a16 + y2.at(i)*a3*a15 + a3*a16)
				      +  v[0][2]*(x1.at(i) * a1 + a3) 
				      +  v[1][0]*(x2.at(i) *  y1.at(i) * a11*a5 + x2.at(i)* a11*a6 + y1.at(i) * a13*a5 + a6*a13)
				      +  v[1][1] * ( y1.at(i) * y2.at(i) * a5*a15 +  y1.at(i) * a5 * a16 + y2.at(i)*a15 * a6 + a6*a16)
				      +  v[1][2] * ( y1.at(i)* a5 + a6) 
				      +  v[2][0]* (x2.at(i) *a11+ a13) 
				      +  v[2][1] * ( y2.at(i)* a15 + a16) 
				      +  v[2][2]
				      +Interval(-epseq,epseq)
				      ));

	  m_fun[0][i]=new Function (v,(-(v[0][0]* (x1.at(i) * x2.at(i) * a1*a11  + x1.at(i) * a1 * a13 + x2.at(i) *a11* a3 + a3*a13 ) 
					 +  v[0][1] * (x1.at(i)*y2.at(i)*a1*a15 + x1.at(i)* a1* a16 + y2.at(i)*a3*a15 + a3*a16)
					 +  v[0][2]*(x1.at(i) * a1 + a3) 
					 +  v[1][0]*(x2.at(i) *  y1.at(i) * a11*a5 + x2.at(i)* a11*a6 + y1.at(i) * a13*a5 + a6*a13)
					 +  v[1][1] * ( y1.at(i) * y2.at(i) * a5*a15 +  y1.at(i) * a5 * a16 + y2.at(i)*a15 * a6 + a6*a16)
					 +  v[1][2] * ( y1.at(i)* a5 + a6) 
					 +  v[2][0]* (x2.at(i) *a11+ a13) 

					 +  v[2][1] * ( y2.at(i)* a15 + a16)) +Interval(-epseq,epseq)));

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
	linfun[i][1][0]= -(x1.at(i) * x2.at(i) * a1*a11 + x1.at(i) * a1 * a13 + x2.at(i) * a11* a3 + a3*a13);
	linfun[i][2][0]=-(x1.at(i)*y2.at(i)*a1*a15 + x1.at(i)* a1* a16 + y2.at(i)*a3*a15 + a3*a16);
	linfun[i][3][0]=-(x1.at(i) * a1 + a3);
	linfun[i][4][0]=-(x2.at(i) *  y1.at(i) * a11*a5 + x2.at(i)* a11*a6 + y1.at(i) * a13*a5 + a6*a13);
	linfun[i][5][0]=-( y1.at(i) * y2.at(i) * a5*a15 +  y1.at(i) * a5 * a16 + y2.at(i)* a15 * a6 + a6*a16);
	linfun[i][6][0]=-(y1.at(i)* a5 + a6);
	linfun[i][7][0]=-(x2.at(i)*a11 +a13);
	linfun[i][8][0]=-(y2.at(i)* a15 + a16) ;
	linfun[i][9][0]=-1;
	
	m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));
	}

	cout << " apres contraintes " << endl;
	double _box[9][2];
	
	for (int j=0; j<9; j++){
	  _box[j][0] = -1/sqrt(2);
	  _box[j][1] = 1/sqrt(2);
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
	
	
	
	
	/*	
	
	fundmat[0]=0.0;
	fundmat[1]= -0.5;
        fundmat[2]= 0.0;
        fundmat[3]= -0.5;
        fundmat[4]=0.0;
        fundmat[5]=-0.5;;
        fundmat[6]= 0.;
        fundmat[7]= 0.5;
	fundmat[8]= 0.0 ;
	
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
	/*
	for (int i=0; i< 9; i++)
	  {_box[i][0]= fundmat[i] - eps1;
	    _box[i][1]= fundmat[i]+ eps1;
	  }

	*/	
	Vector  oraclemat(9);
	for (int i=0; i<9; i++) oraclemat[i]=0;

	for (int i=0; i< 9; i++)
	  {_box[i][0]=  oraclemat[i]- eps1;
	    _box[i][1]= oraclemat[i]+ eps1;
	  }

	for (int i=0; i< 9; i++){
	  if (_box[i][0] < 1/sqrt(2) ) _box[i][0]=-1/sqrt(2);
	  if (_box[i][1] > 1/sqrt(2) ) _box[i][1]=1/sqrt(2);
	}

	//	if (_box[0][0] <0 &&_box[0][1] >0) {_box[0][0]=0;}
	if (_box[1][0] <0 &&_box[1][1] >0) {_box[1][1]=0;}
	/*
	_box[8][0]=1-1.e-8;
	_box[8][1]=1+1.e-8;
	*/
	/*	
	for (int i=0; i< 9; i++)
	  {_box[i][0]= - eps1;
	    _box[i][1]= eps1;
	  }
	*/
	// solution ransac
	/*
	fundmat[0]=0.0212236;
	fundmat[1]=-0.4939788;
	fundmat[2]=0.0420022;
	fundmat[3]=0.1820916 ;
	fundmat[4]=-0.0056804;
	fundmat[5]=-0.6818421;
	fundmat[6]=-0.0376943;
        fundmat[7]=0.5039347 ;
	fundmat[8]=0.0173491;
	*/
	

	IntervalVector box(9,_box);

	cout << " box " << box << endl;
	Vector prec(9);
        for (int j=0; j<9; j++)
	  prec[j]=prec0;

	CellBuffer * buff;
	if (optim==0)
	  buff = new CellStack();
	else
	  buff = new CellHeapQInter();
	
	//	CellHeapQInter buff;
	//	CellStack buff;
	SmearSumRelative bs(sys,prec,0.5);
	//	SmearMaxRelative  bs(sys,prec,0.5);
       	//RoundRobin bs (prec,0.5);
	//	RoundRobinQInter bs (prec,0.5);
	//LargestFirst bs(prec,0.5);
	// RoundRobinNvar bs (8,prec,0.5);
	cout << " Q " << Q << endl;
	//	CtcQInterAff ctcq(9,m_ctc,Q,m_fun);
	//	CtcQInter ctcq(9,m_ctc,Q);
	CtcQInterAffLinear ctcq(9,m_ctc,linfun,epseq,Q);
	//	CtcQInterAffLinear ctcq(9,m_ctc,linfun,epseq,Q,QINTERCORE);
	//	CtcCompo ctcqf0(*cdet,c_essential, ctcq);
	//	CtcCompo ctcqf1(*ctcnorm,*cdet,c_essential);
		//		CtcCompo ctcqf0(ctcqf1,lphull);
	//	CtcCompo ctcqf0(*ctcnorm,*cdet,c_essential, lphull, ctcq);
        //	CtcCompo ctcqf0(*ctcnorm,*cdet,c_essential,  ctcq);

	//	CtcHC4 hc44cid(sys0,0.005,true);
	CtcHC4 hc44cid(sys0,0.005,true);
	//	CtcHC4 hc44cid(sys0,10,false);
	

	CtcAcid acidhc4(sys0,hc44cid,true);
	//	CtcCompo ctcqf0(lphull,  ctcq);
	//	CtcCompo ctcqf0(hc44cid,  acidhc4,lphull,ctcq);

	//	CtcCompo ctcqf0(hc44cid, acidhc4,lphull,ctcq);
	CtcCompo ctcqf0(hc44cid, lphull,  ctcq);
	//CtcCompo ctcqf0(acidhc4, lphull,  ctcq);
	//	CtcCompo ctcqf0(hc44cid,  ctcq);
	//	CtcCompo ctcqf1(*ctcnorm,*cdet,c_essential, lphull);
	//	CtcFixPoint ctcf0(ctcqf1, 0.1);
	//	CtcCompo ctcqf0(ctcf0, ctcq);
	//	CtcCompo ctcqf0(*ctcnorm,*cdet,c_essential, ctcq);
	//CtcCompo ctcqf0(*cdet, c_essential,  ctcq);

	//CtcCompo ctcqf0(*cdet,  ctcq);
	CtcFixPoint ctcf(ctcqf0,0.2);



	
	cout << " avant solver " << endl;



        int m = ctcq.activepoints_contract_count(box);
	cout << " nb inliers " << m << endl;

	SolverOptQInter* s;

	//SolverOptConstrainedQInter s(sys1,ctcqf0,bs,buff,ctcq,epscont,1);
	//	SolverOptConstrainedQInter s(sys1,ctcqf0,bs,buff,ctcq,epscont,2);
	if (optim ==0)
	  s= new SolverOptConstrainedQInter (sys1,ctcqf0,bs,*buff,ctcq,epscont,1);
	else if (optim==1)
	  s= new SolverOptBSConstrainedQInter (sys1,ctcqf0,bs,*buff,ctcq,epscont);
	else 
	  s= new SolverOptConstrainedQInter (sys1,ctcqf0,bs,*buff,ctcq,epscont,2);  //BFS

	//	SolverOptQInter s(ctcf,bs,buff,ctcq,1);
	cout << " apres solver " << endl;
	s->time_limit = time0;
	s->trace=1;
	s->nbr=nbr;
	s->epsobj=eobj;
	s->depthmax=dmax;
	s->gaplimit=gaplimit;
	//	s->oracle=fundmat;
	//	s->oracle=oraclemat;
	s->with_oracle=0;
	//s->with_oracle=1;
	cout << " oracle " << oraclemat << endl;
	cout << "box init " << box << endl;

	cout << "essential " << m_essential1.eval_matrix(box) << endl;
	c_essential.contract(box);
	cout << " box apres essential  " << box << endl;
	ctcnorm->contract(box);
	cout << " box apres ctcnorm  " << box << endl;
	cout << "essential apres contract " << m_essential1.eval_matrix(box) << endl;


	cout << "determinant " << m_det->eval(box);
	IntervalVector res=s->solve(box);

	cout << "Number of branches : " << s->nb_cells << endl;
	cout << "time used : " << s->time << endl;


	s->report_possible_inliers();
       	s->report_solution();

	IntervalVector bestsolbox(s->bestsolpoint);
	
	  

	CtcQInterAffLinear ctcq2(9,m_ctc,linfun,epseq,Q);
	int nb= ctcq2.activepoints_contract_count(bestsolbox);
	cout << " nb inliers " << nb << " " << ctcq2.points->size() << endl;
	
	Matrix msol (3,3);
	Matrix msolt (3,3);
	Vector pt1 (3);
	Vector pt2 (3);
	Vector res1(3);
	Vector res2(3);

	for (int i=0;i<3;i++)
	  for (int j=0;j<3;j++)
	    { msol[i][j]=(s->bestsolpoint)[3*i+j];
	      msolt[j][i]=(s->bestsolpoint)[3*i+j];
	    }
	
	list<int>::iterator iter = ctcq2.points->begin() ;
	while (iter != ctcq2.points->end()){
	  
	  /*
	  pt1[0]= x1.at(*iter) * a1 + a3;
	  pt1[1]=y1.at(*iter) * a5 + a6;
	  pt1[2]=1;

	  res1= msolt * pt1;


	  pt2[0]= x2.at(*iter) * a11 + a13;
	  pt2[1]=y2.at(*iter) * a15 + a16;
	  pt2[2]=1;
	
	  res2 = msol * pt2;
	  */
	  cout << *iter << "  ";
	    //<< endl;
	    
	  
	  //	  cout << " res1 " << res1 << endl;
	  //	  cout << " res2 " << res2 << endl;
	  

	  //	  cout << m_func[*iter]->eval(bestsolbox);
	  iter++;}
	
	cout << endl;
	cout << "essential " << m_essential1.eval_matrix(bestsolbox) << endl;
	cout << "determinant " << m_det->eval(bestsolbox);
       
	for (int i=0; i<p; i++)
	  {delete m_func[i];
	    delete &m_ctc[i];
	  }
	Timer::stop();
	cout <<  " time used " << s->time << "  "<< endl ;
	cout <<" total branch number " << s->nb_cells << endl;
	delete buff;
	delete s;
	}
	catch (SyntaxError e)
	  {cout <<e << endl;}
 }
