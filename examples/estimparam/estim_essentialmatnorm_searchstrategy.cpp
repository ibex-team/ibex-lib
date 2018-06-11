#include "ibex.h"

#include <iostream>
#include <stdlib.h>
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

void correspondences_read(ifstream & input, vector<double>& x11,  vector<double>& y11, vector<double> & x22, vector<double> & y22){
  double in;
  input >> in;
  x11.push_back(in);
  //  	    cout << in << " " ;
  input >> in;
  y11.push_back(in);
  //	    cout << in << " " ;
  input >> in;
  x22.push_back(in);
  // 	    cout << in << " " ;
  input >> in;
  y22.push_back(in);
  //	    cout << in << " " << endl;
}

// reading correspondences data for Oxford corridor and Valbonne church
void correspondences_read_corridor(ifstream& input, vector<double>& x11,  vector<double>& y11, vector<double> & x22, vector<double> & y22)
 {double in;
	    input >> in;
	    y11.push_back(in);
	    //	    cout << in << " " ;
	    input >> in;
	    x11.push_back(in);
	    //	    cout << in << " " ;
	    input >> in;
	    input >> in;
	    input >> in;
            y22.push_back(in);
	    //	    cout << in << " " ;
            input >> in;
            x22.push_back(in);
	    input >> in;
	    input >> in;
	    //	    cout << in << " " << endl;
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
   bool corridor= atoi(argv[2]);  // 0 for USAC , 1 for Valbonne Church and Oxford Corridor
   
   int nbp = atoi(argv[3]);
   cout << " corridor " << corridor << endl; 
   cout << " nbp " << nbp << endl;
   string calib_file_name=argv[4];
   //	string oracle_file_name=argv[4];  // not used in DFS
	
	
   int Q = atoi(argv[5]);        // minimum Q value for a solution (initial value for Q-inter algorithm)
   double epseq = atof(argv[6]); // tolerance
   double eps1 = atof(argv[7]);  // width of initial box (2 for a complete domain : will be restricted to -1/sqrt(2), 1/sqrt(2)) 
   double prec0= atof(argv[8]);  // epsbox 
   double epscont= atof(argv[9]); // tolerance for the internal constraints
   int gaplimit = atoi (argv[10]); // for checking set of ouliers when qmax - qmin <= gap
   int nbr = atoi (argv[11]);     // number of tries for searching a feasible point


   int dmax= atoi (argv[12]);  // used for Depth First Search only : only one branch hen depth > dmax
   int eobj= atoi (argv[13]);  // precision on the objective : 1 for the maximum precision (one integer), k for coarser precision.

   int searchstrategy = atoi(argv[14]); // 0 DFS  1 BeamSearch 2 BFS
   double time0= atof(argv[15]);     // timeout

   cout << " Q " << Q << endl;
   cout << " tolerance " << epseq << endl;
   cout << " initboxsize " << eps1 << endl;
   cout << " epsbox" << prec0 << endl;
   cout << " epscont " << epscont << endl;


   cout << " dmax " << dmax << endl;
   cout << " deltaobj " << eobj << endl;
   cout << " timeout " << time0 << endl;
   srand (atoi(argv[16]));

   cout << input_file_name << endl;
   ifstream input(input_file_name.c_str());
   int nb_pairs;
   if (corridor==0){   // correspondences read : file format depending on corridor parameter
     input >> nb_pairs;
     cout << "nb_pairs" << nb_pairs << endl;
   }
   cout << " corridor " << corridor << endl;
   while (!input.eof()){
     if (corridor)
       correspondences_read_corridor(input, x11,y11,x22,y22);
     else
       correspondences_read(input, x11,y11,x22,y22);
   }
   x11.pop_back();y11.pop_back();x22.pop_back();y22.pop_back();

   // the correspondences with multiple instances are reduced to one instance
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
   
   cout << "nb of single correspondences " << x1.size() << endl;


   cout << " end of correspondences reading " << endl;
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

	Function EtE (v, transpose(v)*v);
	Function fess(v, sqr((EtE(v)[0][0] +EtE(v)[1][1]+EtE(v)[2][2])/2) -

		      (EtE(v)[0][0]*EtE(v)[1][1]-EtE(v)[0][1]*EtE(v)[1][0] +EtE(v)[0][0]*EtE(v)[2][2]-EtE(v)[0][2]*EtE(v)[2][0] +
		       EtE(v)[1][1]*EtE(v)[2][2]-EtE(v)[2][1]*EtE(v)[1][2]));

	Function fess1(v, sqr((EtE(v)[0][0] +EtE(v)[1][1]+EtE(v)[2][2])/2) -

		      (EtE(v)[0][0]*EtE(v)[1][1]-EtE(v)[0][1]*EtE(v)[1][0] +EtE(v)[0][0]*EtE(v)[2][2]-EtE(v)[0][2]*EtE(v)[2][0] +
		       EtE(v)[1][1]*EtE(v)[2][2]-EtE(v)[2][1]*EtE(v)[1][2]) + eps2);


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

    	double ** linfun;
	linfun = new double*[p];
	for (int i=0; i<p; i++)
	  {  linfun[i] = new double[n+1];
	   
	  }


	Array<Ctc> m_ctc(p);

	Function* m_func[p] ;	
	Function ** m_fun;
	m_fun=new Function *[p];

	Ctc* ctcnorm;

	Function* m_det;
	Function* m_det1;

	Ctc* cdet;

        Function* m_norm = new Function (v, sqr(v[0][0])+sqr(v[0][1])+sqr(v[0][2])+ sqr(v[1][0]) + sqr(v[1][1]) + sqr(v[1][2]) + sqr(v[2][0]) + sqr(v[2][1])+sqr(v[2][2]) -1 + eps2);
	Function* m_norm1 = new Function (v, sqr(v[0][0])+sqr(v[0][1])+sqr(v[0][2])+ sqr(v[1][0]) + sqr(v[1][1]) + sqr(v[1][2]) + sqr(v[2][0]) + sqr(v[2][1])+sqr(v[2][2]) -1 );
	
	NumConstraint* c_norm = new NumConstraint(*m_norm, GEQ);
	NumConstraint* c_norm1 = new NumConstraint(*m_norm1, GEQ);
	ctcnorm = new CtcFwdBwd(*m_norm);
	

	m_det = new Function (v, v[0][0]*(v[1][1]*v[2][2]-v[2][1]*v[1][2]) - v[1][0]*(v[0][1]*v[2][2] - v[0][2]*v[2][1] ) + v[2][0]*(v[0][1]*v[1][2]-v[0][2]*v[1][1]) + eps2 );
	m_det1 = new Function (v, v[0][0]*(v[1][1]*v[2][2]-v[2][1]*v[1][2]) - v[1][0]*(v[0][1]*v[2][2] - v[0][2]*v[2][1] ) + v[2][0]*(v[0][1]*v[1][2]-v[0][2]*v[1][1]) );
	cdet = new CtcFwdBwd(*m_det);
	//	m_essential = new Function(v, 2*v*transpose(v)*v - matrixtrace(v*transpose(v))*v);
	//	const ExprNode* e= v.transpose(v);
	//	const ExprNode* tr= e[0]+e[1]+e[2];

	
	fac.add_ctr(fessential1);
        fac.add_ctr(fessential2);
	fac.add_ctr(fessential3);
	
	//	fac.add_ctr(fess);
	//	fac.add_ctr(fessential4);
	//        fac.add_ctr(fessential5);
	//        fac.add_ctr(fessential6);
	//        fac.add_ctr(fessential7);
	//        fac.add_ctr(fessential8);
	//        fac.add_ctr(fessential9);
	fac.add_ctr(*m_norm);	
	//fac.add_ctr(*c_norm);
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
	//fac0.add_ctr(*c_norm);
	fac0.add_ctr(*m_det);
	//	fac0.add_ctr(fess);
	fac1.add_ctr(fessential11);
	fac1.add_ctr(fessential12);
	fac1.add_ctr(fessential13);
	fac1.add_ctr(fessential14);
	fac1.add_ctr(fessential15);
	fac1.add_ctr(fessential16);
	fac1.add_ctr(fessential17);
	fac1.add_ctr(fessential18);
	fac1.add_ctr(fessential19);
	
	//	fac1.add_ctr(fess1);
	fac1.add_ctr(*m_det1);
	fac1.add_ctr(*m_norm1);

	//fac1.add_ctr(*c_norm1);

	System sys (fac);
	System sys0 (fac0);
	System sys1 (fac1);

	
	
	/*
	vector<LinearRelaxXTaylor::corner_point> cpoints;
	cpoints.push_back(LinearRelaxXTaylor::RANDOM);
	cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);
	LinearRelaxXTaylor lr (sys,cpoints);
	*/
	//	LinearRelaxCombo lr (sys,LinearRelaxCombo::COMPO);
	LinearizerCombo lr (sys,LinearizerCombo::ART);
	
	CtcPolytopeHull lphull (lr);


	cout << " avant lecture " << endl;
	try {
       	Function m_essential("essentialmatrix.txt");
	IntervalMatrix eps (3, 3, eps2);
	Function m_essential1(v, m_essential(v) + eps);

	Function m_id(v, v );
	//	cout << " apres lecture " << endl;

	
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


	  m_fun[i]=new Function (v,(-(v[0][0]* (x1.at(i) * x2.at(i) * a1*a11  + x1.at(i) * a1 * a13 + x2.at(i) *a11* a3 + a3*a13 ) 
					 +  v[0][1] * (x1.at(i)*y2.at(i)*a1*a15 + x1.at(i)* a1* a16 + y2.at(i)*a3*a15 + a3*a16)
					 +  v[0][2]*(x1.at(i) * a1 + a3) 
					 +  v[1][0]*(x2.at(i) *  y1.at(i) * a11*a5 + x2.at(i)* a11*a6 + y1.at(i) * a13*a5 + a6*a13)
					 +  v[1][1] * ( y1.at(i) * y2.at(i) * a5*a15 +  y1.at(i) * a5 * a16 + y2.at(i)*a15 * a6 + a6*a16)
					 +  v[1][2] * ( y1.at(i)* a5 + a6) 
					 +  v[2][0]* (x2.at(i) *a11+ a13) 

					 +  v[2][1] * ( y2.at(i)* a15 + a16)) +Interval(-epseq,epseq)));

	  /*
	  linfun[i][0]=1;
          linfun[i][1]=x1.at(i) * x2.at(i);
          linfun[i][2]=x1.at(i) * y2.at(i);
          linfun[i][3]=x1.at(i) ;
          linfun[i][4]=x2.at(i) * y1.at(i);
          linfun[i][5]=y1.at(i) * y2.at(i);
          linfun[i][6]=y1.at(i);
          linfun[i][7]=x2.at(i) ;
          linfun[i][8]=y2.at(i) ;
	  */


/* for AffLinear, explicit function v[2][2] of the other variables */


	linfun[i][0]=0;
	linfun[i][1]= -(x1.at(i) * x2.at(i) * a1*a11 + x1.at(i) * a1 * a13 + x2.at(i) * a11* a3 + a3*a13);
	linfun[i][2]=-(x1.at(i)*y2.at(i)*a1*a15 + x1.at(i)* a1* a16 + y2.at(i)*a3*a15 + a3*a16);
	linfun[i][3]=-(x1.at(i) * a1 + a3);
	linfun[i][4]=-(x2.at(i) *  y1.at(i) * a11*a5 + x2.at(i)* a11*a6 + y1.at(i) * a13*a5 + a6*a13);
	linfun[i][5]=-( y1.at(i) * y2.at(i) * a5*a15 +  y1.at(i) * a5 * a16 + y2.at(i)* a15 * a6 + a6*a16);
	linfun[i][6]=-(y1.at(i)* a5 + a6);
	linfun[i][7]=-(x2.at(i)*a11 +a13);
	linfun[i][8]=-(y2.at(i)* a15 + a16) ;
	linfun[i][9]=-1;
	
	m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));
	}

	//	cout << " apres contraintes " << endl;
	double _box[9][2];
	
	for (int j=0; j<9; j++){
	  _box[j][0] = -1/sqrt(2);
	  _box[j][1] = 1/sqrt(2);
	}
	

	
	//  possibilité d'ajouter un oracle :  inutile en BeamSearch	
	
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
	if (_box[1][0] <0 &&_box[1][1] >0) {_box[1][1]=0;}                  //symmetry 



	IntervalVector box(9,_box);

	cout << " box " << box << endl;

	Vector prec(9);
        for (int j=0; j<9; j++)
	  prec[j]=prec0;

	CellBuffer * buff;
	SearchStrategy* str;
	cout << " searchstrategy " << searchstrategy << endl;

	if (searchstrategy==0)
	  {
	    buff = new CellStack();
	    str = new DepthFirstSearch(*buff);
	  }
	else if (searchstrategy==1){
	  buff = new CellHeapQInter();
	  str = new BeamSearch(*buff);
	}
	else{
	  buff = new CellHeapQInter();
	  str= new BestFirstSearch(*buff);
	}


	//SmearSumRelative bs(sys,prec,0.5);
	//SmearMaxRelative  bs(sys,prec,0.5);
       	//RoundRobin bs (prec,0.5);
	LargestFirst bs(prec,0.5);
        //  LSmear bs(prec,0.5);
	// RoundRobinNvar bs (8,prec,0.5);
	cout << " Q " << Q << endl;
	//	CtcQInterAff ctcq(9,m_ctc,Q,m_fun);
	//	CtcQInter ctcq(9,m_ctc,Q);
	CtcQInterAffLinear ctcq(9,m_ctc,linfun,epseq,Q);
	CtcFixPoint ctcqfp(ctcq,0.2);
	//	CtcQInterAffLinear ctcq(9,m_ctc,linfun,epseq,Q,QINTERCORE);
	//	CtcCompo ctcqf0(*cdet,c_essential, ctcq);
	//	CtcCompo ctcqf1(*ctcnorm,*cdet,c_essential);
	//	CtcCompo ctcqf0(ctcqf1,lphull);
	//	CtcCompo ctcqf0(*ctcnorm,*cdet,c_essential, lphull, ctcq);
        //	CtcCompo ctcqf0(*ctcnorm,*cdet,c_essential,  ctcq);


	CtcHC4 hc44cid(sys0,0.005,true);
	//	CtcHC4 hc44cid(sys0,10,false);
	//	CtcHC4 hc44cid(sys0,0.001,true);

	CtcAcid acidhc4(sys0,hc44cid,true);
	//	CtcCompo ctcqf0(lphull,  ctcq);
	//	CtcCompo ctcqf0(hc44cid,  acidhc4,lphull,ctcq);

	CtcCompo ctcqf0(hc44cid, lphull,  ctcq);
	//	CtcCompo ctcqf0(lphull,  ctcq);
	//	CtcCompo ctcqf0(hc44cid, lphull,  ctcq, hc44cid, lphull);
	//	CtcCompo ctcqf0(acidhc4, lphull,  ctcq);
	//	CtcCompo ctcqf0(hc44cid,  ctcq);
	//	CtcCompo ctcqf1(*ctcnorm,*cdet,c_essential, lphull);
	//	CtcFixPoint ctcf0(ctcqf1, 0.1);
	//	CtcCompo ctcqf0(ctcf0, ctcq);
	//	CtcCompo ctcqf0(*ctcnorm,*cdet,c_essential, ctcq);
	//      CtcCompo ctcqf0(*cdet, c_essential,  ctcq);

	//       CtcCompo ctcqf0(*cdet,  ctcq);
	CtcFixPoint ctcf(ctcqf0,0.2);



	
	cout << " avant solver " << endl;


	/*
        int m = ctcq.activepoints_contract_count(box);
	cout << " nb inliers " << m << endl;
	*/

	SolverOptConstrainedQInter* s;

	s= new SolverOptConstrainedQInter (sys1,ctcqf0,bs,*str,ctcq,epscont);


	//	SolverOptQInter s(ctcf,bs,buff,ctcq,1);
	//	cout << " apres solver " << endl;
	s->timeout = time0;
	s->trace=1;
	s->feasible_tries=nbr;  
	s->epsobj=eobj;
	s->str.depthmax=dmax;
	s->gaplimit=gaplimit;
	//s->tolerance_constraints_number=5;
	s->tolerance_constraints_number=10000;  // no second call for feasible point 
	//	s->oracle=oraclemat;
	//	cout << " oracle " << oraclemat << endl;
	s->str.with_oracle=false;
	s->str.with_storage=false;

	cout << "box init " << box << endl;

	cout << "essential " << m_essential1.eval_matrix(box) << endl;
	c_essential.contract(box);
	cout << " box after essential contract " << box << endl;
	ctcnorm->contract(box);
	cout << " box after ctcnorm  contract " << box << endl;
	cout << "essential after contract " << m_essential1.eval_matrix(box) << endl;


	cout << "determinant " << m_det->eval(box);
	IntervalVector res=s->solve(box);

	cout << "Number of branches : " << s->nb_cells << endl;
	cout << "time used : " << s->time << endl;


	s->report_possible_inliers();
       	s->report_solution();

	IntervalVector bestsolbox(s->bestsolpoint);
	
	  

	CtcQInterAffLinear ctcq2(9,m_ctc,linfun,epseq,Q);
	int nb= ctcq2.activepoints_contract_count(bestsolbox);

	//int nb=ctcq2.midactivepoints_count(s->bestsolpoint);
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
	cout << "essential11" << fessential11.eval_matrix(bestsolbox) << endl;
	cout << "essential12" << fessential12.eval_matrix(bestsolbox) << endl;
	cout << "essential13" << fessential13.eval_matrix(bestsolbox) << endl;
	cout << "essential14" << fessential11.eval_matrix(bestsolbox) << endl;
	cout << "essential15" << fessential12.eval_matrix(bestsolbox) << endl;
	cout << "essential16" << fessential13.eval_matrix(bestsolbox) << endl;
	cout << "essential17" << fessential11.eval_matrix(bestsolbox) << endl;
	cout << "essential18" << fessential12.eval_matrix(bestsolbox) << endl;
	cout << "essential19" << fessential13.eval_matrix(bestsolbox) << endl;
	cout << "fess" << fess.eval_matrix(bestsolbox) << endl;
	cout << "determinant " << m_det->eval(bestsolbox);
       
	for (int i=0; i<p; i++)
	  {delete m_func[i];
	    delete &m_ctc[i];
	  }

	cout <<  " time used " << s->time << "  "<< endl ;
	cout <<" total branch number " << s->nb_cells << endl;
	delete buff;
	delete str;
	delete s;
	}
	catch (SyntaxError e)
	  {cout <<e << endl;}
 }
