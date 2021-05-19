/* ============================================================================
 * I B E X - Iterative loup finder
 * ============================================================================
 *
 * Author(s)   : Ignacio Araya, Victor Reyes
 * Created     : May 2021
 * Updated     : May 2021
 * ---------------------------------------------------------------------------- */

#include "ibex_LoupFinderIterative.h"

using namespace std;

namespace ibex {


LoupFinderIterative::LoupFinderIterative(const System& sys,const IntervalVector& initial_box,std::vector<LoupFinderIP*> loup_finders,double alpha,int max_iter,double prec) :
	initial_box(initial_box),alpha(alpha),sys(sys),max_iter(max_iter),prec(prec),trace(false),loup_finders(loup_finders) {

	if (loup_finders.size() == 0) get_default_finders(LoupFinderIterative::loup_finders);
}

void LoupFinderIterative::get_default_finders(std::vector<LoupFinderIP*> & loup_finders){
	LinearizerXTaylor* lr = new LinearizerXTaylor(sys,LinearizerXTaylor::RESTRICT,LinearizerXTaylor::RANDOM);
	LinearizerAbsTaylor* lr2 = new LinearizerAbsTaylor(sys);
	LoupFinderIP* finder_ip_abst = new LoupFinderIP(sys,lr2); LoupFinderIP* finder_ip_xt = new LoupFinderIP(sys,lr);
	loup_finders.push_back(finder_ip_abst); loup_finders.push_back(finder_ip_xt);
}


void LoupFinderIterative::reduce_and_move(IntervalVector& box_aux, Vector old_exp){
	for (int i = 0 ; i < box_aux.size() ; i++){
		if ((old_exp[i]-(alpha)*box_aux[i].diam()/2>=initial_box[i].lb()) && (old_exp[i]+(alpha)*box_aux[i].diam()/2<=initial_box[i].ub()))
			box_aux[i] = Interval(old_exp[i]-(alpha)*box_aux[i].diam()/2,old_exp[i]+(alpha)*box_aux[i].diam()/2);
		else if ((old_exp[i]-(alpha)*box_aux[i].diam()/2>=initial_box[i].lb()) && (old_exp[i]+(alpha)*box_aux[i].diam()/2>initial_box[i].ub()))
			box_aux[i] = Interval(old_exp[i]-(alpha)*box_aux[i].diam()/2,initial_box[i].ub());
		else if ((old_exp[i]-(alpha)*box_aux[i].diam()/2<initial_box[i].lb()) && (old_exp[i]+(alpha)*box_aux[i].diam()/2<=initial_box[i].ub()))
			box_aux[i] = Interval(initial_box[i].lb(),old_exp[i]+(alpha)*box_aux[i].diam()/2);
		else if ((old_exp[i]-alpha*box_aux[i].diam()/2<initial_box[i].lb()) && (old_exp[i]+alpha*box_aux[i].diam()/2>initial_box[i].ub()))
			box_aux[i] = Interval(initial_box[i].lb(),initial_box[i].ub());
	}
}

void LoupFinderIterative::print_ub(std::pair<IntervalVector,double> p){
	cout << "The point :    ";
	cout << p.first.ub() << endl;
	cout << "corresponds to an upperbound of the problem with a cost of ";
	cout << p.second << endl << endl;
}

std::pair<IntervalVector, double> LoupFinderIterative::find(const IntervalVector& box, const IntervalVector& old_point, double old_loup) {

	pair<IntervalVector,double> p=make_pair(old_point, old_loup);

	bool found=false;
	pair<IntervalVector,double> new_ub = p;
	pair<IntervalVector,double> old_ub = p;
 	bool flag = true;

 	for (int i = 0 ; i < loup_finders.size() ; i++){
		try{
			pair<IntervalVector,double> new_ub=loup_finders[i]->find(box,old_point,p.second);
			if(new_ub.second < p.second){
				found = true;
				p = new_ub;
				if (trace) print_ub(p);
			}
			else throw NotFound();
		} catch(NotFound&) { }
 	}

	if (!found){
		throw NotFound();
	}
	IntervalVector box_aux(box.size());
	box_aux = box;
	int nb_iter = 0;
	while ((old_ub.second-p.second > prec) || (flag)){
		if (old_ub.second-p.second < prec) flag = false;
		else flag = true;

		Vector old_exp = p.first.mid();
		reduce_and_move(box_aux,old_exp);
		old_ub = p;

		try {
			new_ub=loup_finders[0]->find(box_aux,p.first,p.second);
			if(new_ub.second < p.second){
				p = new_ub;
				if (trace) print_ub(p);
			}
		} catch(NotFound&) {}

		nb_iter++;
		if (nb_iter >= max_iter)
			break;
	}
	if (found){
		return p;
	}
	else
		throw NotFound();
}


LoupFinderIterative::~LoupFinderIterative() {

}

} /* namespace ibex */
