#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <ctime>

#ifndef _IBEX_WITH_PARAM_ESTIM_
#error "You need the plugin Param Estim to run this example."
#endif

using namespace std;
using namespace ibex;

/*
 * 
 * Detects circles in noisy pictures using q-intersection. For more details, see L. Jaulin and S. Bazeille, 
 * "Image Shape Extraction using Interval Methods", Sysid 2009
 * 
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
const double POS_ERROR = 0.4;						//Uncertainty of the pixels positions
const double MIN = 0;								//Minimum value for x,y,r
const double MAX = 256;								//Maximum value for x,y,r
const double eps = 2;								//Maximum diameter of the outputted boxes
const double Qprop = 0.20;							//Minimum ratio of consistent measurements
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
	for (unsigned int i=0; i<x->size(); i++) {
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

int main() {
	
	vector<int> *x = readFile(input_x_filename);
	vector<int> *y = readFile(input_y_filename);
	
	assert(x->size() == y->size());
	
	Variable u(3);
	Variable w(2);
	Function onCircle(u,w,sqr(u[0]-w[0])+sqr(u[1]-w[1])-sqr(u[2]));
	
	int p = x->size();
	int Q = p*Qprop;
	
	Function *m_func[p]; 
	Array<Ctc> m_ctc(p);
	for (int i=0; i<p; i++) {
		
		Variable v(3);
		
		/* Our measured point : we add some uncertainty */
		IntervalVector a(2);
		a[0] = (double)x->at(i) + POS_ERROR*Interval(-1,1);
		a[1] = (double)y->at(i) + POS_ERROR*Interval(-1,1);
		
		/* We must be on the circle defined by v */
		m_func[i] = new Function(v,onCircle(v,a));
		m_ctc.set_ref(i,*new CtcFwdBwd(*m_func[i]));
		
	}
		
	double _box[3][2];
	for (int j=0; j<3; j++) {
		_box[j][0] = MIN;
		_box[j][1] = MAX;
	}
		
	IntervalVector box(3,_box);

	IntervalVector b(3);
	
	list<IntervalVector> pendingList;
	list<IntervalVector> res;
	
	pendingList.push_back(box);
	
	CtcQInterProjF ctcq(m_ctc,Q);
	CtcFixPoint ctcf(ctcq);

	int counter = 0;
	
	/* Main optimization loop */
	clock_t start, end;
	start = clock();
	while (!pendingList.empty()) {
		counter++;
		b = pendingList.front();
		pendingList.pop_front();


		ctcf.contract(b);
		if (b.is_empty()) continue;
		
		if (b.max_diam() > eps) {
			assert(b.is_bisectable());
			pair<IntervalVector, IntervalVector> pr = b.bisect(b.extr_diam_index(false),0.5);
			pendingList.push_front(pr.first);
			pendingList.push_front(pr.second);
		} else {
			cout << "circle found : " << b.mid() << endl;
			res.push_back(b);
		}
	}
	end = clock();
	
	cout << "Shape extraction : OK. Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	cout << "Number of branches : " << counter << endl;
	/* output the box */
	printOutput(x, y, res);
};






