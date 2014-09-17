//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeap_2.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Sept 11, 2014
// Last Update : Sept 11, 2014
//============================================================================

#include "ibex_CellHeap_2.h"


namespace ibex {


CellHeapVarLB::CellHeapVarLB(int ind_var, int ind_crit) : CellHeap_2(LB, ind_var, ind_crit) { };

CellHeapVarUB::CellHeapVarUB(int ind_var, int ind_crit) : CellHeap_2(UB, ind_var, ind_crit) { };


CellHeapCost::CellHeapCost(criterion crit, int ind_crit) : CellHeap_2(crit, -1, ind_crit) {
	if (crit==LB) {
		ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF");
	} else if (crit==UB) {
		ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF");
	}

};


double CellHeapCost::cost(const Cell& c) const {
	const OptimCell * t = dynamic_cast<const OptimCell *>(&c);
	if (t) return cost(*t);
	else {
		ibex_error("CellHeapCost::cost: that must be a OptimCell and not just a Cell");
		return POS_INFINITY;
	}
}

// TODO heu.. là il faudrait avoir le type de Cell
double CellHeapCost::cost(const OptimCell& c) const {

		switch (crit)	{
		case LB : ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF"); return POS_INFINITY;
		case UB : ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF"); return POS_INFINITY;
		case C3 : return -((loup - c.pf.lb()) / c.pf.diam() );
		case C5 : return -(c.pu * (loup - c.pf.lb()) / c.pf.diam());
		case C7 : return c.box[ind_var].lb()/(c.pu*(c.loup-c.pf.lb())/c.pf.diam());
		case PU : return -c.pu	;
		case PF_LB : return c.pf.lb();
		case PF_UB : return c.pf.ub();
		default : ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF"); return POS_INFINITY;
		}
}

//////////////////////////////////////////////////////////////////////////////////////

CellHeap_2::CellHeap_2(criterion crit, int ind_var, int ind_crit) :
		ind_crit(ind_crit), crit(crit), ind_var(ind_var), loup(POS_INFINITY), root(NULL) {
	if (((crit==LB)||(crit==UB))&&(ind_var<0)) {
		ibex_error("CellHeap_2: you need to indicate the indice of the variable which strae the objective function");
	}
}

CellHeap_2::~CellHeap_2() {
	// attention le delete supprime tous les noeuds d'en dessous
	if (root) delete root;
	root = NULL;
}

void CellHeap_2::flush() {
	// attention le delete supprime tous les noeuds d'en dessous
	if (root) delete root;
	nb_cells =0;
	root = NULL;
}

int CellHeap_2::size() const {
	return nb_cells;
}

bool CellHeap_2::empty() const {
	return (nb_cells==0);
}

Cell* CellHeap_2::top() const {
	return root->elt->cell;
}

void CellHeap_2::setLoup( double new_loup) {
	loup = new_loup;
}

// E.g.: called by manage_buffer in Optimizer in case of a new upper bound
// on the objective ("loup"). This function then removes (and deletes) from
// the heap all the cells with a cost greater than loup.
void CellHeap_2::contract_heap(double new_loup) {
	loup = new_loup;
	if (nb_cells==0) return;

	CellHeap_2 * heap_tmp = init_copy();
	heap_tmp->setLoup(loup);

	contract_tmp(new_loup, root, *heap_tmp);

	if (root) delete root;
	root = heap_tmp->root;
	nb_cells = heap_tmp->size();
	heap_tmp->root = NULL;
	delete heap_tmp;

}


void CellHeap_2::contract_tmp(double new_loup, HeapNode * node, CellHeap_2 & heap) {
	double new_cost;
	switch (crit)	{
	case C3 :  case C5 :  case C7 :
		node->elt->crit[ind_crit] = cost(*(node->elt->cell)); break;
	default: break;
	}
	if (node->isSup(new_loup, ind_crit)) {
		delete node;
	} else {
		heap.push(node->elt);
		if (node->left)	 contract_tmp(new_loup, node->left, heap);
		if (node->right) contract_tmp(new_loup, node->right, heap);
	}
}



void CellHeap_2::sort() {
	if (nb_cells==0) return;

	CellHeap_2 * heap_tmp = init_copy();
	heap_tmp->setLoup(loup);

	// recursive sort : o(n*log(n))
	sort_tmp(root, *heap_tmp);

	// attention le delete supprime tous les noeuds d'en dessous
	if (root) delete root;
	root = heap_tmp->root;
	nb_cells = heap_tmp->size();

	// attention le delete supprime tous les noeuds d'en dessous
	heap_tmp->root = NULL;
	delete heap_tmp;

}


void CellHeap_2::sort_tmp(HeapNode * node, CellHeap_2 & heap) {

	if (node->left)	 sort_tmp(node->left, heap);
	if (node->right) sort_tmp(node->right, heap);

	switch (crit)	{
	case C3 :  case C5 :  case C7 :
		node->elt->crit[ind_crit] = cost(*node->elt->cell); break;
	default: break;
	}
	heap.push(node->elt);
	node->elt = NULL;

	// attention le delete supprime tous les noeuds d'en dessous
	delete node;
}



void CellHeap_2::push(Cell* cell) {
	push(new HeapElt(cell,cost(*cell)));
}


void CellHeap_2::push(HeapElt* cell) {

	if (nb_cells==0) {
		root = new HeapNode(cell);
		root->elt->indice[ind_crit] = 0;
		nb_cells++;
	} else {
		nb_cells++;
		// calcul de la hauteur
		int hauteur = 0;
		int aux = nb_cells;
		while(aux>1) { aux /= 2; hauteur++;	}

		// pt = pointeur sur l element courant
		HeapNode * pt = root;
		for (int pos=hauteur-1; pos>0; pos--) {
			if ( nb_cells & (1 << pos)) {  // test the "pos"th bit of the integer nb_cells
				//	   print *,  "DROITE"
				pt = pt->right;
			} else { //  print *, "GAUCHE"
				pt = pt->left;
			}
		}
		HeapNode * tmp= new HeapNode(cell,pt);
		tmp->elt->indice[ind_crit] = nb_cells-1;
		if (nb_cells%2==0)	{ pt->left =tmp; }
		else 				{ pt->right=tmp; }
		pt = tmp;

		// PT indique l element ajoute
		// on doit maintenant faire un update en remontant vers la racine
		bool b = true;
		while (b&&(pt->father)) {
			if (pt->father->isSup(pt,ind_crit)) {
				pt->switchElt(pt->father,ind_crit);
				pt = pt->father;
			} else {
				b= false;
			}
		}

	}
}

HeapNode * CellHeap_2::getNode(unsigned long i) const {
	assert(i<nb_cells);
	assert(nb_cells>0);
	// RECUPERATION DU DERNIER ELEMENT et suppression du noeud associe
	// calcul de la hauteur
	int hauteur = 0;
	unsigned long aux = i+1;
	while(aux>1) { aux /= 2; hauteur++; }
	//std::cout << "hauteur de "<<i<<"  = " << hauteur<<std::endl;
	// pt = pointeur sur l element courant
	HeapNode * pt = root;
	for (int pos=hauteur-1; pos>=0; pos--) {
		if ( (i+1) & (1 << pos)) {  // test the "pos"th bit of the integer nb_cells
			//std::cout << "Droit ";
			pt = pt->right;
		} else {
			//std::cout << "gauche ";
			pt = pt->left;
		}
	}
	return pt;
}

Cell * CellHeap_2::getCell(unsigned long i ) const {
	assert(i<nb_cells);
	assert(nb_cells>0);
	return getNode(i)->elt->cell;
}

Cell* CellHeap_2::pop() {
	assert(nb_cells>0);
	HeapElt* tmp =pop_elt();
	Cell * c = tmp->cell;
	tmp->cell = NULL;
	delete tmp;
	return c;
}

HeapElt* CellHeap_2::pop_elt() {
	assert(nb_cells>0);
	HeapElt* c_return = root->elt;
	eraseNode(0);
	return c_return;
}

// erase a node and update the order
void CellHeap_2::eraseNode(unsigned long i) {
	assert(i<nb_cells);
	assert(nb_cells>0);

	updateOrder(eraseNode_noUpdate(i));
}

// erase a node and update the order
HeapNode * CellHeap_2::eraseNode_noUpdate(unsigned long i) {
	assert(i<nb_cells);
	assert(nb_cells>0);
	HeapNode * pt_root;
	if (nb_cells==1) {
		root->elt=NULL;
		delete root;
		root = NULL;
	} else if (i==(nb_cells-1)) {
		// RECUPERATION DU DERNIER ELEMENT et suppression du noeud associe sans detruire le HeapElt
		HeapNode * pt= getNode(nb_cells-1);
		if (nb_cells%2==0)	{ pt->father->left =NULL; }
		else 				{ pt->father->right=NULL; }
		pt->elt = NULL;
		delete pt;
	} else {
		// RECUPERATION DU DERNIER ELEMENT et suppression du noeud associe sans detruire le HeapElt
		HeapNode * pt= getNode(nb_cells-1);
		HeapElt* cell = pt->elt;
//std::cout << cell->indice[ind_crit] << std::endl;
		if (nb_cells%2==0)	{ pt->father->left =NULL; }
		else 				{ pt->father->right=NULL; }
		pt->elt = NULL;
		delete pt;
		// CEL pointe vers l element que l on remet à la place du noeud supprime
		pt_root= getNode(i);
		pt_root->elt = cell;
		pt_root->elt->indice[ind_crit] = i;
	}
	nb_cells--;
	return pt_root;
}



void CellHeap_2::updateOrder(HeapNode *pt) {
	// PERMUTATION pour maintenir l'ordre dans le tas en descendant
	bool b=true;
	while (b && (pt->left)) {
		if (pt->right) {
			if (pt->isSup(pt->left,ind_crit)) {
				if (pt->right->isSup(pt->left,ind_crit)) {
					// le gauche est le plus petit
					pt->switchElt(pt->left,ind_crit);
					pt = pt->left;  // au suivant
				} else {
					// le droit est le plus petit
					pt->switchElt(pt->right,ind_crit);
					pt = pt->right;  // au suivant
				}
			} else {
				if (pt->isSup(pt->right,ind_crit)) {
					// le droit est le plus petit
					pt->switchElt(pt->right,ind_crit);
					pt = pt->right;  // au suivant
				} else {// le noeud courant est le plus petit des 3 donc on arrete
					b=false;
				}
			}
		} else { // il n'y a plus de fils droit mais il y a un fils gauche
			if (pt->isSup(pt->left,ind_crit)) {
				// le gauche est le plus petit
				pt->switchElt(pt->left,ind_crit);
				pt = pt->left;  // au suivant
			} else { // le noeud courant est le plus petit des 3 donc on arrete , on doit meme avoir touche le fond ;-)
				b=false;
			}
		}
	}
}



//////////////////////////////////////////////////////////////////////////////////////
//HeapNode::HeapNode(): elt(NULL), right(NULL), left(NULL), father(NULL) { }

HeapNode::HeapNode(HeapElt* elt, HeapNode * father): elt(elt), right(NULL), left(NULL), father(father) { }

HeapNode::~HeapNode() {
	// attention le delete supprime tous les noeuds d'en dessous
	if (elt) 	delete elt;
	if (right) 	delete right;
	if (left)  	delete left;
}

bool HeapNode::isSup(HeapNode *n, int ind_crit) const {
	return elt->isSup(n->elt->crit[ind_crit],ind_crit);
}

bool HeapNode::isSup(double d, int ind_crit) const {
	return elt->isSup(d,ind_crit);
}

void HeapNode::switchElt(HeapNode *pt, int ind_crit) {
	//ne pas oublier la permutation des indices
	HeapElt * elt_tmp = elt;

	pt->elt->indice[ind_crit] = elt->indice[ind_crit];
	elt = pt->elt;

	elt_tmp->indice[ind_crit] = pt->elt->indice[ind_crit];
	pt->elt = elt_tmp;

}

//////////////////////////////////////////////////////////////////////////////////////
/** create an node with a cell and its criterion */
HeapElt::HeapElt(int nb_crit,Cell* cell, double *crit_in) : cell(cell), crit(new double[nb_crit]), indice(new unsigned long[nb_crit]){
	for (int i=0;i<nb_crit;i++) {
		crit[i] = crit_in[i];
		indice[i] = 0;
	}
}

HeapElt::HeapElt(Cell* cell, double crit_1) : cell(cell), crit(new double[1]), indice(new unsigned long[1]){
		crit[0] = crit_1;
		indice[0] = 0;
}
HeapElt::HeapElt(Cell* cell, double crit_1, double crit_2) : cell(cell), crit(new double[2]), indice(new unsigned long[2]){
		crit[0] = crit_1;
		crit[1] = crit_2;
		indice[0] = 0;
		indice[1] = 0;
}


/** Delete the node and all its sons */
HeapElt::~HeapElt() {
	delete crit;
	delete indice;
	if (cell) 	delete cell;
}

bool HeapElt::isSup(double d, int ind_crit) const {
	return (crit[ind_crit] > d);
}



//////////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const HeapElt& node) {
	os << *(node.cell);
	return os;
}

std::ostream& operator<<(std::ostream& os, const HeapNode& node) {
	os << *(node.elt) << " ";
	os <<  *(node.right) << *(node.left);
	return os;
}


std::ostream& operator<<(std::ostream& os, const CellHeap_2& heap) {
	os << "[ ";
	os << *(heap.root) << " ";
	return os << "]";
}


}  // end namespace ibex
