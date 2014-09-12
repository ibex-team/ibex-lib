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

CellHeap_2::CellHeap_2() : root(NULL) { }

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
	return root->elt;
}


// E.g.: called by manage_buffer in Optimizer in case of a new upper bound
// on the objective ("loup"). This function then removes (and deletes) from
// the heap all the cells with a cost greater than loup.
void CellHeap_2::contract_heap(double new_loup) {
	if (nb_cells==0) return;

	CellHeap_2 heap_tmp;
	contract_tmp(new_loup, root, heap_tmp);

	if (root) delete root;
	root = heap_tmp.root;
	nb_cells = heap_tmp.size();
	heap_tmp.root = NULL;

}


void CellHeap_2::contract_tmp(double new_loup, HeapNode * node, CellHeap_2 & heap) {
	if (node->elt) {
		if (node->isSup(new_loup)) {
			heap.push(node->elt,node->crit);
			if (node->left)	 contract_tmp(new_loup, node->left, heap);
			if (node->right) contract_tmp(new_loup, node->right, heap);
		} else {
			delete node;
		}
	} else {
		if (node->left)	 contract_tmp(new_loup, node->left, heap);
		if (node->right) contract_tmp(new_loup, node->right, heap);
	}
}


void CellHeap_2::push(Cell* cell) {
	push(cell,cost(*cell));
}


void CellHeap_2::push(Cell* cell, double crit) {
	if (capacity>0 && size()==capacity) throw CellBufferOverflow();

	if (nb_cells==0) {
		root = new HeapNode(cell,crit);
		nb_cells++;
	} else {
		nb_cells++;
		// calcul de la hauteur
		int hauteur = 0;
		int aux = nb_cells;
		while(aux>1) { aux /= 2; hauteur++;	}

		// pt = pointeur sur l element courant
		HeapNode * pt = *root;
		for (int pos=hauteur-1; (pos=0); pos--) {
			if ( nb_cells & (1 << pos)) {  // test the "pos"th bit of the integer nb_cells
				//	   print *,  "DROITE"
				if (pos!=0) {
					pt = pt->right;
				} else {
					HeapNode * tmp= new HeapNode(cell,crit);
					tmp->father = pt;
					pt->right = tmp;
					pt = tmp;
				}
			} else { //  print *, "GAUCHE"
				if (pos!=0) {
					pt = pt->left;
				} else {
					HeapNode * tmp= new HeapNode(cell,crit);
					tmp->father = pt;
					pt->left = tmp;
					pt = tmp;
				}
			}
		}

		// PT indique l element ajoute
		bool b = true;
		while (b&&(pt->father)) {
			if (pt->father->isSup(pt)) {
				pt->switchElt(pt->father);
				pt = pt->father;
			} else {
				b= false;
			}
		}

	}
}

HeapNode * CellHeap_2::selectNode(int i) const {
	assert((i>-1)&&(i<nb_cells));
	// RECUPERATION DU DERNIER ELEMENT et suppression du noeud associe
	// calcul de la hauteur
	int hauteur = 0;
	int aux = i+1;
	while(aux>1) { aux /= 2; hauteur++; }

	// pt = pointeur sur l element courant
	HeapNode * pt = *root;
	for (int pos=hauteur-1; (pos=0); pos--) {
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

Cell* CellHeap_2::pop() {
	assert(nb_cells>0);

	Cell* c_return = root->elt;

	if (nb_cells==1) {
		root->elt=NULL;
		delete root;
	} else {
		// RECUPERATION DU DERNIER ELEMENT et suppression du noeud associe
		HeapNode * pt= selectNode(nb_cells-1);
		Cell* cell = pt->elt;
		double crit_cell = pt->crit;
		if (nb_cells%2==0)	{ pt->father->left =NULL; }
		else 				{ pt->father->right=NULL; }

		pt->elt = NULL;
		delete pt;
		// CEL pointe vers l element qu on remet en tete
		root->elt = cell;
		root->crit = crit_cell;

		// PERMUTATION
		bool b=true;
		pt = root;
		while (b&&(pt->left)) {
			if (pt->right) {
				if (pt->isSup(pt->left)) {
					if (pt->right->isSup(pt->left)) {
						// le gauche est le plus petit
						pt->switchElt(pt->left);
						pt = pt->left;  // au suivant
					} else {
						// le droit est le plus petit
						pt->switchElt(pt->right);
						pt = pt->right;  // au suivant
					}
				} else {
					if (pt->isSup(pt->right)) {
						// le droit est le plus petit
						pt->switchElt(pt->right);
						pt = pt->right;  // au suivant
					} else {// le noeud courant est le plus petit des 3 donc on arrete
						b=false;
					}
				}
			} else { // il n'y a plus de fils droit mais il y a un fils gauche
				if (pt->isSup(pt->left)) {
					// le gauche est le plus petit
					pt->switchElt(pt->left);
					pt = pt->left;  // au suivant
				} else { // le noeud courant est le plus petit des 3 donc on arrete , on doit meme avoir touche le fond ;-)
					b=false;
				}
			}
		}
	}
	nb_cells--;
	return c_return;
}


std::ostream& operator<<(std::ostream& os, const CellHeap_2& heap) {
	os << "[ ";
	os << heap.root << " ";
	return os << "]";
}

HeapNode::HeapNode(): elt(NULL), crit(NEG_INFINITY), right(NULL), left(NULL), father(NULL) { }

HeapNode::HeapNode(Cell* elt, double crit): elt(elt), crit(crit), father(NULL), right(NULL), left(NULL) { }

HeapNode::~HeapNode() {
	if (elt) 	delete elt;
	if (right) 	delete right;
	if (left)  	delete left;
}

bool HeapNode::isSup(HeapNode *n) const {
	return crit >= n->crit;
}

bool HeapNode::isSup(double d) const {
	return crit >= d;
}

void HeapNode::switchElt(HeapNode *pt) {
	Cell * cell = elt;
	double crit_cell = crit;

	elt = pt->elt;
	crit= pt->crit;

	pt->elt = cell;
	pt->crit = crit_cell;
}

std::ostream& operator<<(std::ostream& os, const HeapNode& node) {
	os << *(node.elt) << " ";
	os <<  *(node.right) << *(node.left);
	return os;
}


}  // end namespace ibex
