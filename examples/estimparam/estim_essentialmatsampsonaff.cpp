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
	string oracle_file_name=argv[4];
	int Q = atoi(argv[5]);
	double epseq = atof(argv[6]);
        double eps1 = atof(argv[7]);
	double prec0= atof(argv[8]);
	double epscont= atof(argv[9]);
	int gaplimit = atoi (argv[10]);
	int nbr = atoi (argv[11]);
	int dmax= atoi (argv[12]);
	int eobj= atoi (argv[13]);

	int optim = atoi(argv[14]);
	double time0= atof(argv[15]);

	srand (atoi(argv[16]));
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
	ifstream oraclefile(oracle_file_name.c_str());
	Vector  oraclemat(9);
	for (int i=0; i<9; i++)
	  oraclefile >> oraclemat[i];
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

	cout << " fin lecture " << endl;
	int n=10;
        Variable v(3,3);
	Variable r;
        SystemFactory fac;
        SystemFactory fac1;
	fac.add_var(v);
	fac.add_var(r);
	fac1.add_var(v);
	fac1.add_var(r);
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
	else p = x1.size();

	cout << " p " << p << endl;

	int K=1;
    	double *** linfun;
	linfun = new double**[p];
	for (int i=0; i<p; i++)
	  {  linfun[i] = new double*[n+1];
	    for (int j=0; j<n+1; j++)
	      linfun[i][j]= new double[K];
	  }
	double ** linfun1;
	linfun1 = new double*[p];
	for (int i=0; i<p; i++)
	  {  linfun1[i] = new double[10];
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

	System sys1 (fac1);

	
	
	
	vector<LinearRelaxXTaylor::corner_point> cpoints;
	cpoints.push_back(LinearRelaxXTaylor::RANDOM);
	cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);
	
	//LinearRelaxCombo lr (sys,LinearRelaxCombo::COMPO);
	LinearRelaxCombo lr (sys,LinearRelaxCombo::ART);
	//	LinearRelaxXTaylor lr (sys2,cpoints);
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
	
	
	F[0][0]=-2.20246e-06 ;
	F[0][1]=7.06071e-05 ;
	F[0][2]=-0.0315415;
	F[1][0]=-2.3367e-05;
	F[1][1]=-8.85794e-07;
	F[1][2]=0.12567;
	F[2][0]=0.0149924;
	F[2][1]=-0.127773;
	F[2][2]=-1.86073;


	
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


	F[0][0]=2.17745e-06 ;
	F[0][1]=-2.19382e-05;
	F[0][2]=0.0115024 ;
	F[1][0]=2.12724e-05;
	F[1][1]=-1.99877e-07 ;
	F[1][2]=-0.020719 ;
	F[2][0]=-0.0138558;
	F[2][1]=0.00894409;
	F[2][2]=8.01696 ;

	*/



        
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
	  linfun[i][0][0]=1;
	  linfun[i][1][0]= (x1.at(i) * x2.at(i) * a1*a11 + x1.at(i) * a1 * a13 + x2.at(i) * a11* a3 + a3*a13);
	  linfun[i][2][0]=(x1.at(i)*y2.at(i)*a1*a15 + x1.at(i)* a1* a16 + y2.at(i)*a3*a15 + a3*a16);
	  linfun[i][3][0]=(x1.at(i) * a1 + a3);
	  linfun[i][4][0]=(x2.at(i) *  y1.at(i) * a11*a5 + x2.at(i)* a11*a6 + y1.at(i) * a13*a5 + a6*a13);
	  linfun[i][5][0]=( y1.at(i) * y2.at(i) * a5*a15 +  y1.at(i) * a5 * a16 + y2.at(i)* a15 * a6 + a6*a16);
	  linfun[i][6][0]=(y1.at(i)* a5 + a6);
	  linfun[i][7][0]=(x2.at(i)*a11 +a13);
	  linfun[i][8][0]=(y2.at(i)* a15 + a16) ;

	  // a changer pour calib non symétriques

          linfun1[i][0]= x2.at(i)* a1*a1+ a1* a3;          
	  linfun1[i][1]= x1.at(i)* a1*a1+ a1* a3;
          linfun1[i][2]=  y2.at(i)*a1*a5+a1*a6;
          linfun1[i][3]=  y1.at(i)*a1*a5+a1*a6;
	  linfun1[i][4]= x2.at(i)*a1*a5+a3*a5;
	  linfun1[i][5]= x1.at(i)*a1*a5+a3*a5;	  
          linfun1[i][6]= y2.at(i)*a5*a5+a5*a6;
          linfun1[i][7]= y1.at(i)*a5*a5+a5*a6;
          linfun1[i][8]= a1;
          linfun1[i][9]= a5;

	  

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

	  m_func[i]= new Function (v, r, sqr(linfun[i][0][0] * v[2][2]+ linfun[i][1][0] * v[0][0] + 
					  linfun[i][2][0] * v[0][1]+	linfun[i][3][0] * v[0][2]  +
					  linfun[i][4][0] * v[1][0]+	linfun[i][5][0] * v[1][1]  +
					  linfun[i][6][0] * v[1][2]+	linfun[i][7][0] * v[2][0]  +
					  linfun[i][8][0] * v[2][1])
				   /
				   (sqr (v[0][0] * linfun1[i][0] + v[0][1] * linfun1[i][2] + v[0][2]*linfun1[i][8])
				    +sqr(v[1][0] * linfun1[i][4] + v[1][1] * linfun1[i][6] + v[1][2]*linfun1[i][9])
				    + sqr(v[0][0]* linfun1[i][1] + v[1][0] *  linfun1[i][3] + v[2][0]*linfun1[i][8])
				    +sqr (v[0][1]* linfun1[i][5] + v[1][1] *  linfun1[i][7] + v[2][1]*linfun1[i][9])
				    )
				   -r
				   -Interval(0,epseq));

	  m_fun[0][i]= new Function (v, sqr(linfun[i][0][0] * v[2][2]+ linfun[i][1][0] * v[0][0] + 
					  linfun[i][2][0] * v[0][1]+	linfun[i][3][0] * v[0][2]  +
					  linfun[i][4][0] * v[1][0]+	linfun[i][5][0] * v[1][1]  +
					  linfun[i][6][0] * v[1][2]+	linfun[i][7][0] * v[2][0]  +
					  linfun[i][8][0] * v[2][1])
				   /
				   (sqr (v[0][0] * linfun1[i][0] + v[0][1] * linfun1[i][2] + v[0][2]*linfun1[i][8])
				    +sqr(v[1][0] * linfun1[i][4] + v[1][1] * linfun1[i][6] + v[1][2]*linfun1[i][9])
				    + sqr(v[0][0]* linfun1[i][1] + v[1][0] *  linfun1[i][3] + v[2][0]*linfun1[i][8])
				    +sqr (v[0][1]* linfun1[i][5] + v[1][1] *  linfun1[i][7] + v[2][1]*linfun1[i][9])
				    )
				   -Interval(0,epseq));
       
	  /*


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
	  */
	  /*
	  linfun[i][0][0]=1;
          linfun[i][1][0]=x1.at(i) * x2.at(i);
          linfun[i][2][0]=x2.at(i) * y1.at(i);
          linfun[i][3][0]=x2.at(i) ;
          linfun[i][4][0]=x1.at(i) * y2.at(i);
          linfun[i][5][0]=y1.at(i) * y2.at(i);
          linfun[i][6][0]=y2.at(i);
          linfun[i][7][0]=x1.at(i) ;
          linfun[i][8][0]=y1.at(i) ;
	  */
	  
// 
/*
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
*/	
	  m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));
	}

	cout << " apres contraintes " << endl;
	double _box[10][2];
	
	for (int j=0; j<9; j++){
	  _box[j][0] = -1/sqrt(2);
	  _box[j][1] = 1/sqrt(2);
	}
	
	_box[9][0]=0;
	_box[9][1]=1.e-20;
	



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
	/*
	fundmat[0]=2.1327e-02;
	fundmat[1]=-3.7447e-01;
	fundmat[2]=0.0420022;
	fundmat[3]=5.1689e-02 ;
	fundmat[4]=4.5638e-04;
	fundmat[5]=-7.0392e-01;
	fundmat[6]=-3.9969e-02 ;
        fundmat[7]=5.9810e-01;
	fundmat[8]=2.0207e-02;
	*/
	/*
	fundmat[0]=2.0467e-02;
	fundmat[1]=-3.6940e-01;
	fundmat[2]=3.7268e-02;
	fundmat[3]=4.5726e-02;
	fundmat[4]=8.4992e-05;
	fundmat[5]=-7.0437e-01;
	fundmat[6]= -3.8536e-02;
        fundmat[7]=6.0137e-01;
	fundmat[8]= 1.9674e-02;

	*/

	//solution 421
	/*	double tab[]=
	  {




	    -0.0121301,
	    0.0061545,
	    0.0233053,
	    0.3186520,
	    0.0073285,
	    -0.6246010,
	    -0.0205754,
	    0.7120312,
	    -0.0118669};


 

	double tab[]={
	     0.0245523,
	     0.1518636,
	     -0.0470842,
	     -0.4453679,
	     -0.0054417,
	     0.5285097,
	     0.0463824,
	     -0.7024712,
	     0.0282114};
	*/
	// avec nouvelle version USAC  279
	
	double tab[]={
	  0.0121462,
	  -0.1388590,
	  -0.0281015,
	  -0.1495173,
	  0.0014760,
	  0.6904252,
	  0.0272993,
	  -0.6926802,
	  0.0136108};
	
	// avec nouvelle version USAC  284
	/*
	double tab[]={
	  0.0116704,
	  -0.6977590,
	  -0.0223571,
	  0.6424804,
	  -0.0020735,
	  0.2945656,
	  0.0175622,
	  0.1117035,
	  0.0124898,};
	*/

	/*
0.0230321,
	   0.0929656,
	   -0.0434799,
	   -0.4020631,
	   -0.0040107,
	   0.5668785,
	   0.0417051,
	   -0.7095960,
	   0.0254688};
	*/



	for (int i=0; i< 9; i++)
	  {

	    fundmat[i]=tab[i];
	    if (fundmat[i] < - 1/sqrt(2) ) fundmat[i]=-1/sqrt(2);
	    if (fundmat[i] > 1/sqrt(2) ) fundmat[i]=1/sqrt(2);
	  }
	
	

//{1.41078e-05, 3.03704e-05, -0.0470103 ,-0.000295304, -2.03532e-06, 0.672895, 0.133288, -0.599274,-9.03183};
	
	swap(fundmat[1],fundmat[3]);
	swap(fundmat[2],fundmat[6]);
	swap(fundmat[5],fundmat[7]);

	
	/*  solution critere algébrique : mauvais oracle pour critère Sampson
	fundmat[0]=0.051878505;
	fundmat[1]=-0.67384236;
	fundmat[2]=0.17677669;
	fundmat[3]=0.61617291;
	fundmat[4]=-0.060021;
	fundmat[5]=-0.29566712;
	fundmat[6]=-0.1575514;
        fundmat[7]=-0.0931053;
	fundmat[8]=0.108733;
	*/
	fundmat[0]=0.0123088842481506 ;
	fundmat[1]=-0.1695172999999999 ; 
	fundmat[2]=0.0222993 ; 
	fundmat[3]=-0.1426569709445837 ; 
	fundmat[4]=0.001476000000000002 ; 
	fundmat[5]=-0.6920955389229777 ; 
	fundmat[6]=-0.0281015 ; 
	fundmat[7]=0.6857227394114003 ; 
	fundmat[8]=0.01354322141801832 ;


 

fundmat[0]=0.01264265638778085 ;fundmat[1]= -0.1895172999999998 ;fundmat[2]= 0.02229930000000001 ; fundmat[3]=-0.1264550697036973 ; fundmat[4]=0.001476000000000006 ; fundmat[5]=-0.6952441461702683 ; fundmat[6]=-0.0281015 ; fundmat[7]=0.6804680057817976 ; fundmat[8]=0.01359002895389031 ;
fundmat[0]=0.004973299637651936 ; fundmat[1]=-0.2187495222726825 ; fundmat[2]=0.007997978287603392 ; fundmat[3]=-0.1171908213207728 ; fundmat[4]=-0.001453396513006809 ; fundmat[5]=-0.6972671885067933 ; fundmat[6]=-0.01057296010348493 ; fundmat[7]=0.6723133098774586 ; fundmat[8]=0.003132584139120488 ;

 fundmat[0]=0.01064079179477381 ; fundmat[1]=-0.2644238712876459 ; fundmat[2]=0.01799797828760341 ; fundmat[3]=-0.08483390380414423 ; fundmat[4]=-0.0004412470495029621 ; fundmat[5]=-0.7017328918319272 ; fundmat[6]=-0.0198493281129698 ; fundmat[7]=0.6554107177707676 ; fundmat[8]=0.009691811312245058 ;



	for (int i=0; i< 9; i++)
	  cout << fundmat[i] << "  ";
	cout << endl;
	
	for (int i=0; i< 9; i++)
	  {_box[i][0]= fundmat[i] - eps1;
	    _box[i][1]= fundmat[i]+ eps1;
	  }
	
	IntervalVector box1(9,_box);

	for (int i=0; i< 9; i++){
	  if (_box[i][0] < - 1/sqrt(2) ) _box[i][0]=-1/sqrt(2);
	  if (_box[i][1] > 1/sqrt(2) ) _box[i][1]=1/sqrt(2);
	}

	if (_box[0][0] <0 &&_box[0][1] >0) {_box[0][0]=0;}
	for (int i=0; i< 9; i++){
	  cout << _box[i][0] << " " <<  _box[i][1] << endl;
	}
	/*
	_v[2][2][0]=1-1.e-8;
	_v[2][2][1]=1+1.e-8;
	*/
	/*	
	for (int i=0; i< 9; i++)
	  {_box[i][0]= - eps1;
	    _box[i][1]= eps1;
	  }
	*/
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

	IntervalVector box(10,_box);

	cout << " box " << box << endl;
	Vector prec(10);
        for (int j=0; j<9; j++)
	  prec[j]=prec0;
	prec[9]=10;


	CellBuffer * buff;
	if (optim==0)
	  buff = new CellStack();
	else
	  buff = new CellHeapQInter();

	
	RoundRobinNvar bs (9,prec,0.5);
	//	RoundRobinQInter bs (prec,0.5);
	//LargestFirst bs(prec,0.5);
	//RoundRobinNvar bs (8,prec,0.5);
	cout << " Q " << Q << endl;
	//	CtcQInter ctcq(9,m_ctc,Q);
	//	CtcQInterAff ctcq(9,m_ctc,Q,m_fun);

	CtcQInterAffSampson ctcq(10,m_ctc,linfun,linfun1,m_fun,epseq,Q);
	//	CtcCompo ctcqf0(*cdet,c_essential, ctcq);
	//	CtcCompo ctcqf1(*ctcnorm,*cdet,c_essential);
	//	CtcCompo ctcqf0(ctcqf1,lphull);
	//	CtcCompo ctcqf0(*ctcnorm,*cdet,c_essential, lphull, ctcq);
	//	CtcCompo ctcqf0(*ctcnorm,*cdet,c_essential,  ctcq);

	CtcCompo ctcqf0(lphull,  ctcq);

	//	CtcCompo ctcqf1(*ctcnorm,*cdet,c_essential, lphull);
	//	CtcFixPoint ctcf0(ctcqf1, 0.1);
	//	CtcCompo ctcqf0(ctcf0, ctcq);
	//	CtcCompo ctcqf0(*ctcnorm,*cdet,c_essential, ctcq);
	//CtcCompo ctcqf0(*cdet, c_essential,  ctcq);

	//CtcCompo ctcqf0(*cdet,  ctcq);
	CtcFixPoint ctcf(ctcqf0,0.1);

	//	cout << " solution ransac " <<  ctcq.activepoints_count(vec) << endl;

	
	cout << " avant solver " << box << endl;



	//        int m = ctcq.activepoints_contract_count(box);
        int m = ctcq.activepoints_count(box1);
	cout << " nb inliers " << m << endl;

	SolverOptQInter* s;

	//SolverOptConstrainedQInter s(sys1,ctcqf0,bs,buff,ctcq,epscont,1);
	//	SolverOptConstrainedQInter s(sys1,ctcqf0,bs,buff,ctcq,epscont,2);
	if (optim ==0) // DFS
	  s= new SolverOptConstrainedQInter (sys1,ctcqf0,bs,*buff,ctcq,epscont,1);
	else if (optim ==1)  // FD
	  s= new SolverOptBSConstrainedQInter (sys1,ctcqf0,bs,*buff,ctcq,epscont);
	else // BFS
	  s= new SolverOptConstrainedQInter (sys1,ctcqf0,bs,*buff,ctcq,epscont,2);



	cout << " apres solver " << endl;
	s->time_limit = time0;
	s->trace=1;
	s->nbr=nbr;
	s->epsobj=eobj;
	s->depthmax=dmax;
	//	s->func=& m_essential1;
	s->gaplimit=gaplimit;
	s->oracle=oraclemat;
	s->with_oracle=1;
	cout << "box init " << box << endl;

	cout << "essential " << m_essential1.eval_matrix(box) << endl;
	c_essential.contract(box);
	cout << " box apres essential  " << box << endl;
	ctcnorm->contract(box);
	cout << " box apres ctcnorm  " << box << endl;
	cout << "essential apres contract " << m_essential1.eval_matrix(box) << endl;

	cout << "determinant " << m_det->eval(box);
	vector<IntervalVector> res=s->solve(box);

	cout << "Number of branches : " << s->nb_cells << endl;
	cout << "time used : " << s->time << endl;


	s->report_possible_inliers();
	s->report_solution();

	
	IntervalVector bestsolbox(s->bestsolpoint);
	//	CtcQInterAffLinear ctcq2(9,m_ctc,linfun,epseq,Q);
	CtcQInterSampson ctcq2(9,m_ctc,linfun,linfun1,epseq,Q);


	cout << "essential " << m_essential1.eval_matrix(bestsolbox);
	cout << "determinant " << m_det->eval(bestsolbox);
       


	for (int i=0; i<p; i++)
	  {delete m_func[i];
	    delete &m_ctc[i];
	  }
	Timer::stop();
	cout <<  " time used " << s->time << "  "<< endl ;
	cout <<" total branch number " << s->nb_cells << endl;
	}
	catch (SyntaxError e)
	  {cout <<e << endl;}
 }
