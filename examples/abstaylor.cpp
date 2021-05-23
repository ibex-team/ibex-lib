/**
 *
 * This main shows how to use the upperbounding methods: AbsTaylor and Iterative.
 *
 * As output you can expect the feasible point and the cost.
 */
#include "ibex.h"

using namespace std;
using namespace ibex;


int main(int argc, char** argv) {

	if (argc < 4){
		cout << "Use the format: ./abstaylor FILE [abst|iterative] [mid|rnd]" << endl;
		return 0;
	}
	System *sys;
	pair<IntervalVector,double> p;
	sys = new System(argv[1]);
	string loup_finder = argv[2];
	string exp_point= argv[3];
	LinearizerXTaylor* lr = new LinearizerXTaylor(*sys,LinearizerXTaylor::RESTRICT,LinearizerXTaylor::RANDOM);


	//Point definition, either mid or random
	LinearizerAbsTaylor* lr2;

	if (exp_point == "mid")
		lr2 = new LinearizerAbsTaylor(*sys,LinearizerAbsTaylor::MID);
	else if (exp_point == "rnd"){
		lr2 = new LinearizerAbsTaylor(*sys,LinearizerAbsTaylor::RANDOM);
		}
	else{
		cout << "Use either mid for the midpoint or rnd for a random point inside the box" << endl;
		exit(1);
	}

	cout << endl<<"The search of upperbounds will be performed in the box : "<< sys->box << endl<<endl;
	cout << "Press a key to continue"<< endl;
	string aux; cin >> aux;
//
	//abstaylor section
	LoupFinderIP* finder_ip_abst = new LoupFinderIP(*sys,lr2);

	if (loup_finder == "abst"){
		try {
			p = finder_ip_abst->find(sys->box,sys->box.mid(),POS_INFINITY);
			cout << "The point :    ";
			cout << p.first.ub() << endl;
			cout << "corresponds to an upperbound of the problem with a cost of ";
			cout << p.second << endl << endl;

		} catch(LoupFinder::NotFound&) {cout << "Upperbound not found"<<endl; }
	}
//
	//iterative section
	else if (loup_finder == "iterative"){
		LoupFinderIP* finder_ip_xt = new LoupFinderIP(*sys,lr);
		vector<LoupFinderIP*> finders;
		finders.push_back(finder_ip_abst); finders.push_back(finder_ip_xt);
		LoupFinderIterative* finder_trust = new LoupFinderIterative(*sys,sys->box,finders);
		try {
			finder_trust->set_trace(true);
			finder_trust->find(sys->box,sys->box.mid(),POS_INFINITY);
		} catch(LoupFinder::NotFound&) {cout << "Upperbound not found"<<endl; }
	}
	else{
		cout << "Method not found!" << endl;
		exit(1);
	}


}
