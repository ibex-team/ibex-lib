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


CellHeapVarLB::CellHeapVarLB(int ind_var, int ind_crit) : CellHeap_2(CellHeap_2::LB, ind_var, ind_crit) { };
CellHeapVarUB::CellHeapVarUB(int ind_var, int ind_crit) : CellHeap_2(CellHeap_2::UB, ind_var, ind_crit) { };
CellHeapCost::CellHeapCost(criterion & crit, int ind_crit) : CellHeap_2(crit, -1, ind_crit) {
	if (crit==LB) {
		ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF");
	} else if (crit==UB) {
		ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF");
	}

};


// TODO heu.. là il faudrait avoir le type de Cell
double CellHeapCost::cost(const OptimCell& c) const {

		switch (crit)	{
		case C3 : return -((loup - c.pf.lb()) / c.pf.diam() );
		case C5 : return -(c.pu * (loup - c.pf.lb()) / c.pf.diam());
		case C7 : return c.box[ind_var].lb()/(c.pu*(c.loup-c.pf.lb())/c.pf.diam());
		case PU : return -c.pu	;
		case PF : return c.pf.lb();
		}

}

//////////////////////////////////////////////////////////////////////////////////////

CellHeap_2::CellHeap_2(criterion crit, int ind_var, int ind_crit) :
		nb_cells(0), root(NULL), ind_crit(ind_crit), crit(crit), ind_var(ind_var), loup(POS_INFINITY) {
	if (((crit==LB)||(crit==UB))&&(ind_var<0)) {
		ibex_error("CellHeap_2: you need to indicate the indice of the variable which strae the objective function");
	}
}

CellHeap_2::~CellHeap_2() {
	if (root) delete root;
}

void CellHeap_2::flush() {
	if (root) delete root;
	nb_cells =0;
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

// TODO heu.. là il faudrait avoir le type de Cell
double CellHeap_2::cost(const Cell& c) const {

	const OptimCell* t=dynamic_cast<const OptimCell*>(&c);
	if (t) {
		switch (crit)	{
		case LB : return c.box[ind_var].lb();
		case UB : return c.box[ind_var].ub();
		case C3 : return -((loup - t->pf.lb()) / t->pf.diam() );
		case C5 : return -(t->pu * (loup - t->pf.lb()) / t->pf.diam());
		case C7 : return t->box[ind_var].lb()/(t->pu*(t->loup-t->pf.lb())/t->pf.diam());
		case PU : return -t->pu	;
		case PF : return t->pf.lb();
		}
	} else {
		switch (crit)	{
		case LB : return c.box[ind_var].lb();
		case UB : return c.box[ind_var].ub();
		default : ibex_error("CellHeap_2::cost : you need a OptimCell and not just a Cell"); return POS_INFINITY;
		}

	}
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

	if (root) delete root;
	root = heap_tmp->root;
	nb_cells = heap_tmp->size();
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

	delete node;
}



void CellHeap_2::push(Cell* cell) {
	double * crit =new double[1];
	crit[0] = cost(*cell);
	push(new HeapElt(1,cell,crit));
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
		for (int pos=hauteur-1; pos<0; pos--) {
			if ( nb_cells & (1 << pos)) {  // test the "pos"th bit of the integer nb_cells
				//	   print *,  "DROITE"
				pt = pt->right;
			} else { //  print *, "GAUCHE"
				pt = pt->left;
			}
		}
		HeapNode * tmp= new HeapNode(cell);
		tmp->elt->indice[ind_crit] = nb_cells-1;
		tmp->father = pt;
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

HeapNode * CellHeap_2::getNode(int i) const {
	assert((i>-1)&&(i<nb_cells));
	assert(nb_cells>0);
	// RECUPERATION DU DERNIER ELEMENT et suppression du noeud associe
	// calcul de la hauteur
	int hauteur = 0;
	int aux = i+1;
	while(aux>1) { aux /= 2; hauteur++; }

	// pt = pointeur sur l element courant
	HeapNode * pt = root;
	for (int pos=hauteur-1; pos==0; pos--) {
		if ( nb_cells & (1 << pos)) {  // test the "pos"th bit of the integer nb_cells
			//	   print *,  "DROITE"
			pt = pt->right;
		} else {
			//  print *, "GAUCHE"
			pt = pt->left;
		}
	}
	return pt;
}

Cell * CellHeap_2::getCell(int i) const {
	assert((i>-1)&&(i<nb_cells));
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

Cell* CellHeap_2::pop() {
	assert(nb_cells>0);
	HeapElt* tmp =pop_elt();
	Cell * c = tmp->cell;
	tmp->cell = NULL;
	delete tmp;
	return c;
}

// erase a node and update the order
void CellHeap_2::eraseNode(int i) {
	assert((i>-1)&&(i<nb_cells));
	assert(nb_cells>0);

	updateOrder(eraseNode_noUpdate(i));
}

// erase a node and update the order
HeapNode * CellHeap_2::eraseNode_noUpdate(int i) {
	HeapNode * pt_root;
	if (nb_cells==1) {
		root->elt=NULL;
		delete root;
	} else if (i==nb_cells-1) {
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

HeapNode::HeapNode(HeapElt* elt): elt(elt), father(NULL), right(NULL), left(NULL) { }

HeapNode::~HeapNode() {
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
HeapElt::HeapElt(int nb_crit,Cell* cell, double *crit) : cell(cell), crit(crit), indice(new unsigned long[nb_crit]){
	for (int i=0;i<nb_crit;i++) indice[i] = 0;
}

/** Delete the node and all its sons */
HeapElt::~HeapElt() {
	if (crit) delete crit;
	if (indice) delete indice;
	if (cell) delete cell;
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
	os << heap.root << " ";
	return os << "]";
}


}  // end namespace ibex
