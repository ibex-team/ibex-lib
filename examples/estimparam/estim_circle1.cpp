#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <ctime>

using namespace std;
using namespace ibex;

/*
 * 
 * Detects circles in noisy pictures using q-intersection. For more details, see L. Jaulin and S. Bazeille, 
 * "Image Shape Extraction using Interval Methods", Sysid 2009
 * 
 * Variant with punctual measutements.
 * To see the results, use the command "gnuplot -persist data/out.plt". 
 * 
 * If the blue circles don't show up, use the "replot" button on the top left corner of the gnuplot window.
 * 
 */

/*==============input options================*/
const int max_line_length = 1024;					//Number of characters we read in each line
const string input_x_filename = "data/x.txt";		//File containing the "x" coordinates
const string input_y_filename = "data/y.txt";		//File containing the "y" coordinates
const string output_datafile = "data/xy.dat";		//points (x,y) formatted for GNUPLOT
const string output_scriptfile = "data/out.plt";	//GNUPLOT script to see the results
/*===========================================*/

/*===============parameters==================*/
//const double POS_ERROR = 0.4;						//Uncertainty of the pixels positions
//const double POS_ERROR = 0.001;						//Uncertainty of the pixels positions
const double POS_ERROR = 0.0;						//Uncertainty of the pixels positions
const double MIN = 0;								//Minimum value for x,y,r
const double MAX = 256;								//Maximum value for x,y,r
const double eps= 0.2;					//Maximum diameter of the outputted boxes (for QInterEstim)
//const double eps= 2.0;				//Maximum diameter of the outputted boxes  (for Jaulin)

/*===========================================*/

vector<int> *readFile(string filename) {
	
	vector<int> *c = new vector<int>();
	
	ifstream input(filename.c_str());

	assert(input != NULL);
	
	char line[max_line_length];
	
	input.getline(line, max_line_length);
	assert(line[0] == '{');
	
	while (line[0] != '}') {
		input.getline(line, max_line_length);
		c->push_back(atof(line));
	}
		
	input.close();
	
	return c;
};

void printOutput(vector<int> *x, vector<int> *y, list<IntervalVector>& res) {
	
	ofstream xy(output_datafile.c_str());
	for (int i=0; i<x->size(); i++) {
		xy << x->at(i) << " " << y->at(i) << endl;
	}
	
	xy.close();
	
	/* Create the gnuplot script file */
	ofstream o(output_scriptfile.c_str());
	
	o << "set size ratio 1" << endl;
	o << "set xrange [0:256]" << endl;
	o << "set yrange [0:256]" << endl;
	
	o << "plot \"" << output_datafile << "\" ps 0.1" << endl;
	
	Vector estimate(3);
	/* Draw the circles */
	while (!res.empty()) {
		estimate = res.front().mid();
		res.pop_front();
		o << "set object circle at " << estimate[0] << "," << estimate[1] << " size first " << estimate[2] << " fc rgb \"navy\"" << endl;
	}
	
	o.close();
};

int main(int argc, char** argv) {
	
	vector<int> *x = readFile(input_x_filename);
	vector<int> *y = readFile(input_y_filename);
	
	int nsup= atoi(argv[1])*x->size();
	cout << " nsup " << nsup << endl;
	double epseq= atof(argv[2]);
        int Q=atoi(argv[3]);
	int flist=atoi(argv[4]);
        int fixpoint=atoi(argv[5]);
		//int nsup=0;

	for (int i = 0; i< nsup; i++)
	  {	 
	    x->push_back((double)rand()/(double)RAND_MAX*MAX);
	    y->push_back((double)rand()/(double)RAND_MAX*MAX);
	  }

	assert(x->size() == y->size());

	

	Variable u(3);
	Variable w(2);


	
	int p = x->size();
	double time=0;
	
	cout << "nb mesures " << p << endl;
	cout << " Q " << Q << endl;
	int K=1;
	int n=3;
	int np=p;
	Function *m_func[p]; 
	Function *** m_fun;
	m_fun=new Function **[K];
	for (int i=0; i<K; i++)
	  m_fun[i]=new Function*[np];

        double *** measure;
	measure = new double**[np];
	for (int i=0; i<np; i++)
	  {  measure[i] = new double*[n-1];
	    for (int j=0; j<n; j++)
	      measure[i][j]= new double[K];
	  }
	Array<Ctc> m_ctc(p);
	for (int i=0; i<p; i++) {
		
		Variable v(3);

		
		/* Our measured point : we add some uncertainty */
		double  a[2];
		a[0] = (double)x->at(i) + (double) rand()/ (double) RAND_MAX * POS_ERROR;
		a[1] = (double)y->at(i) + (double) rand()/ (double) RAND_MAX * POS_ERROR;
		
		/* We must be on the circle defined by v */

		m_func[i]=new Function(v,sqr(v[0]-a[0])+sqr(v[1]-a[1])-v[2] + Interval(-epseq,epseq));
		//		m_func[i]=new Function(v,sqr(v[0]-a[0])+sqr(v[1]-a[1])-sqr(v[2]));
		m_ctc.set_ref(i,*new CtcFwdBwd(*m_func[i]));
		m_fun[0][i]= new Function(v,(sqr(v[0]-a[0])+sqr(v[1]-a[1])+ Interval(-epseq,epseq)));
		measure[i][0][0] = a[0];
		measure[i][1][0] = a[1];


	}
		
	double _box[3][2];
	for (int j=0; j<3; j++) {
		_box[j][0] = MIN;
		_box[j][1] = MAX;
	}
        _box[2][0]=1100;
	_box[2][1]= MAX*MAX;  // for v[2]=d^2
	IntervalVector box(3,_box);

	IntervalVector b(3);
	Vector prec(3);
	prec[0]=eps/10;
	prec[1]=eps/10;
	//	prec[2]=eps;
	prec[2]=(eps/10)*(eps/10);
	//	prec[2]=eps/10;
        cout << "epseq " << epseq;
        cout << " precision bissection " << prec[0] << "  " << prec[1] << " " << prec[2] << endl;
	

	//	CtcQInter ctcq(3,m_ctc,Q);
	//	CtcQInter ctcq(3,m_ctc,Q,QINTERPROJ);
	//	CtcQInterAff ctcq(3,m_ctc,Q,m_fun,QINTERPROJ,K);
	 CtcQInterAffCircle2 ctcq(3,m_ctc,measure,epseq,Q,QINTERPROJ,K);
	 //	 CtcQInterCircle2 ctcq(3,m_ctc,measure,epseq,Q,flist,QINTERPROJ,K);
	//	CtcQInterCircle2 ctcq(3,m_ctc,measure,epseq,Q,flist,QINTERCORE,K);
	//	CtcQInterCircle2 ctcq(3,m_ctc,measure,epseq,Q,m_fun,QINTERPROJ,K);
	//		CtcQInterAff ctcq(3,m_ctc,Q,m_fun,QINTERCORE,K);
	//	CtcQInterAff ctcq(3,m_ctc,Q,m_fun,QINTERFULL,K);
	//	CtcQInterAff ctcq(3,m_ctc,Q,m_fun,CtcQInter::GRID,K);
	CtcFixPoint ctcf(ctcq,0.1);




	CellStack buff;
	//RoundRobin bs (prec,0.5);
	RoundRobinNvar  bs (2,prec,0.5);

	//LargestFirst bs (prec,0.5);
	/* Main optimization loop */
	clock_t start, end;
	start = clock();


	Ctc* ctcs;
	if (fixpoint)
	  ctcs=&ctcf;
	else
	  ctcs=&ctcq;
	SolverQInter s(*ctcs,bs,buff,ctcq);
	//	Solver s(*ctcs,bs,buff);
	s.time_limit = 3600;
	s.trace=1;
	    
	vector<IntervalVector> res=s.solve(box);

	/* output the box */
	//	printOutput(x, y, res);

	s.report_maximal_solutions(res);
	//	s.keep_one_solution_pergroup(res);

	end = clock();

	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();
	cout <<  " time used " << s.time << "  "<< endl ;
	cout << "Shape extraction : OK. Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	cout << "Number of branches : " << s.nb_cells << endl;
};








