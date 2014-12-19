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


// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapVarLB::CellHeapVarLB(int ind_var, int ind_crit) : CellHeap_2(LB, ind_var, ind_crit) { };

double CellHeapVarLB::cost(const Cell& c) const { return c.box[goal_var].lb(); }

CellHeapVarLB* CellHeapVarLB::init_copy() { return new CellHeapVarLB(goal_var,heap_id); }

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapVarUB::CellHeapVarUB(int ind_var, int ind_crit) : CellHeap_2(UB, ind_var, ind_crit) { };

double CellHeapVarUB::cost(const Cell& c) const { return c.box[goal_var].ub(); }

CellHeapVarUB* CellHeapVarUB::init_copy() { return new CellHeapVarUB(goal_var,heap_id); }

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeapCost::CellHeapCost(criterion crit, int ind_var, int ind_crit) : CellHeap_2(crit, ind_var, ind_crit) {
	if (crit==LB) {
		ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF");
	} else if (crit==UB) {
		ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF");
	}

};

// TODO heu.. là il faudrait avoir le type de Cell
double CellHeapCost::cost(const Cell& c) const {

		const OptimData *data = &(c.get<OptimData>());
		switch (crit)	{
		case LB : ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF"); return POS_INFINITY;
		case UB : ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF"); return POS_INFINITY;
		case C3 : return -((loup - data->pf.lb()) / data->pf.diam() );
		case C5 : return -(data->pu * (loup - data->pf.lb()) / data->pf.diam());
		case C7 : return c.box[goal_var].lb()/(data->pu*(loup-data->pf.lb())/data->pf.diam());
		case PU : return -data->pu	;
		case PF_LB : return data->pf.lb();
		case PF_UB : return data->pf.ub();
		default : ibex_error("CellHeapCost::CellHeapCost : invalid flag, that must be C3,C5,C7,PU or PF"); return POS_INFINITY;
		}
}

CellHeapCost* CellHeapCost::init_copy() { return new CellHeapCost(crit,goal_var,heap_id); }

// -----------------------------------------------------------------------------------------------------------------------------------

CellHeap_2::CellHeap_2(criterion crit, int ind_var, int ind_crit) :
		crit(crit), heap_id(ind_crit),  goal_var(ind_var), loup(POS_INFINITY), root(NULL) {
	if (((crit==LB)||(crit==UB))&&(ind_var<0)) {
		ibex_error("CellHeap_2: you need to indicate the index of the variable which contains the objective function");
	}
}

CellHeap_2::~CellHeap_2() {
	if (root) delete root; 	// warning: delete all sub-nodes
	root = NULL;
}

void CellHeap_2::flush() {
	if (root) delete root; 	// warning: delete all sub-nodes
	nb_cells =0;
	root = NULL;
}

unsigned int CellHeap_2::size() const {
	return nb_cells;
}

bool CellHeap_2::empty() const {
	return (nb_cells==0);
}

Cell* CellHeap_2::top() const {
	return root->elt->cell;
}

void CellHeap_2::set_loup( double new_loup) {
	loup = new_loup;
}

// E.g.: called by manage_buffer in Optimizer in case of a new upper bound
// on the objective ("loup"). This function then removes (and deletes) from
// the heap all the cells with a cost greater than loup.
void CellHeap_2::contract_heap(double new_loup) {
	loup = new_loup;
	if (nb_cells==0) return;

	CellHeap_2 * heap_tmp = init_copy();
	heap_tmp->set_loup(loup);

	contract_rec(new_loup, root, *heap_tmp);

	root = heap_tmp->root;
	nb_cells = heap_tmp->size();
	heap_tmp->root = NULL;
	delete heap_tmp;

}


void CellHeap_2::contract_rec(double new_loup, CellHeapNode* node, CellHeap_2& heap) {

	switch (crit)	{
	case C3 :  case C5 :  case C7 : {
		node->elt->crit[heap_id] = cost(*(node->elt->cell)); break;
	}
	default: break;
	}
	if (!(node->isSup(new_loup, heap_id))) {
		heap.push(node->elt);
		if (node->left)	 contract_rec(new_loup, node->left, heap);
		if (node->right) contract_rec(new_loup, node->right, heap);
		node->left=NULL;
		node->right=NULL;
		node->elt=NULL;
	}
	delete node;
	node= NULL;
}

void CellHeap_2::sort() {
	if (nb_cells==0) return;

	CellHeap_2* heap_tmp = init_copy();
	heap_tmp->set_loup(loup);

	// recursive sort : o(n*log(n))
	sort_rec(root, *heap_tmp);

	root = heap_tmp->root;
	nb_cells = heap_tmp->size();

	heap_tmp->root = NULL;
	delete heap_tmp; 	// warning: delete all sub-nodes

}

void CellHeap_2::sort_rec(CellHeapNode * node, CellHeap_2 & heap) {

	switch (crit)	{
	case C3 :  case C5 :  case C7 :
		node->elt->crit[heap_id] = cost(*(node->elt->cell)); break;
	default: break;
	}
	heap.push(node->elt);
	if (node->left)	 sort_rec(node->left, heap);
	if (node->right) sort_rec(node->right, heap);

	node->left=NULL;
	node->right=NULL;
	node->elt = NULL;
	delete node;	// warning: delete all sub-nodes
	node = NULL;
}


void CellHeap_2::push(Cell* cell) {
	push(new CellHeapElt(cell,cost(*cell)));
}


void CellHeap_2::push(CellHeapElt* cell) {

	if (nb_cells==0) {
		root = new CellHeapNode(cell,NULL);
		root->elt->indice[heap_id] = 0;
		nb_cells++;
	} else {
		nb_cells++;
		// calculate height
		int height = 0;
		int aux = nb_cells;
		while(aux>1) { aux /= 2; height++;	}

		// pt = pointer on the current element
		CellHeapNode * pt = root;
		for (int pos=height-1; pos>0; pos--) {
			if ( nb_cells & (1 << pos)) {  // test the "pos"th bit of the integer nb_cells
				pt = pt->right;
			} else {
				pt = pt->left;
			}
		}
		CellHeapNode* tmp= new CellHeapNode(cell,pt);
		tmp->elt->indice[heap_id] = nb_cells-1;
		if (nb_cells%2==0)	{ pt->left =tmp; }
		else 				{ pt->right=tmp; }
		pt = tmp;

		// PT indicates the added element
		// we have now to perform an update upto the root
		bool b = true;
		while (b&&(pt->father)) {
			if (pt->father->is_sup(pt,heap_id)) {
				pt->switch_elt(pt->father,heap_id);
				pt = pt->father;
			} else {
				b= false;
			}
		}

	}
}

CellHeapNode * CellHeap_2::get_node(unsigned int i) const {
	assert(i<nb_cells);
	assert(nb_cells>0);

	// GET THE LAST ELEMENT and delete the associated node
	// calculate height
	int height = 0;
	unsigned int aux = i+1;
	while(aux>1) { aux /= 2; height++; }
	//std::cout << "height of "<<i<<"  = " << height <<std::endl;
	// pt = pointer on the current element
	CellHeapNode* pt = root;
	for (int pos=height-1; pos>=0; pos--) {
		if ( (i+1) & (1 << pos)) {  // test the "pos"th bit of the integer nb_cells
			//std::cout << "Right ";
			pt = pt->right;
		} else {
			//std::cout << "Left ";
			pt = pt->left;
		}
	}
	return pt;
}

Cell* CellHeap_2::get_cell(unsigned int i ) const {
	assert(i<nb_cells);
	assert(nb_cells>0);
	return get_node(i)->elt->cell;
}

Cell* CellHeap_2::pop() {
	assert(nb_cells>0);
	CellHeapElt* tmp =pop_elt();
	Cell * c = tmp->cell;
	tmp->cell = NULL;
	delete tmp;
	tmp=NULL;
	return c;
}

CellHeapElt* CellHeap_2::pop_elt() {
	assert(nb_cells>0);
	CellHeapElt* c_return = root->elt;
	erase_node(0);
	return c_return;
}

// erase a node and update the order
void CellHeap_2::erase_node(unsigned int i) {
	assert(i<nb_cells);
	assert(nb_cells>0);

	update_order(erase_node_no_update(i));
}

// erase a node and update the order
CellHeapNode * CellHeap_2::erase_node_no_update(unsigned int i) {
	assert(i<nb_cells);
	assert(nb_cells>0);

	CellHeapNode* pt_root=NULL;

	if (nb_cells==1) {
		root->elt=NULL;
		delete root;
		root = NULL;
	} else if (i==(nb_cells-1)) {
		// GET THE LAST ELEMENT and delete the associated node without destroying the HeapElt
		CellHeapNode* pt= get_node(nb_cells-1);
		if (nb_cells%2==0)	{ pt->father->left =NULL; }
		else 				{ pt->father->right=NULL; }
		pt->elt = NULL;
		delete pt;
		pt = NULL;
	} else {
		// GET THE LAST ELEMENT and delete the associated node without destroying the HeapElt
		CellHeapNode * pt= get_node(nb_cells-1);
		pt_root= get_node(i);
		CellHeapElt* cell = pt->elt;
//std::cout << cell->indice[ind_crit] << std::endl;
		if (nb_cells%2==0)	{ pt->father->left =NULL; }
		else 				{ pt->father->right=NULL; }
		pt->elt = NULL;
		delete pt;
		pt = NULL;
		// cell points on the element we put in place of the deleted node
		pt_root= get_node(i);
		pt_root->elt = cell;
		pt_root->elt->indice[heap_id] = i;
	}
	nb_cells--;
	return pt_root;
}



void CellHeap_2::update_order(CellHeapNode *pt) {
	// PERMUTATION to maintain the order in the heap when going down
	if (pt) {
		bool b=true;
		while (b && (pt->left)) {
			if (pt->right) {
				if (pt->is_sup(pt->left,heap_id)) {
					if (pt->right->is_sup(pt->left,heap_id)) {
						// left is the smallest
						pt->switch_elt(pt->left,heap_id);
						pt = pt->left;  // next
					} else {
						// right is the smallest
						pt->switch_elt(pt->right,heap_id);
						pt = pt->right;  // next
					}
				} else {
					if (pt->is_sup(pt->right,heap_id)) {
						// right is the smallest
						pt->switch_elt(pt->right,heap_id);
						pt = pt->right;  // next
					} else { // current node is the smallest among the 3: stop
						b=false;
					}
				}
			} else { // no more right child but there is a left child
				if (pt->is_sup(pt->left,heap_id)) {
					// left is the smallest
					pt->switch_elt(pt->left,heap_id);
					pt = pt->left;  // next
				} else { // current node is the smallest among the 3: stop (and we must have reached the bottom of the heap)
					b=false;
				}
			}
		}
	}
}



//////////////////////////////////////////////////////////////////////////////////////
//HeapNode::HeapNode(): elt(NULL), right(NULL), left(NULL), father(NULL) { }

CellHeapNode::CellHeapNode(CellHeapElt* elt, CellHeapNode * father): elt(elt), right(NULL), left(NULL), father(father) { }

CellHeapNode::~CellHeapNode() {
	// warning: delete all sub-nodes
	if (elt) 	delete elt;
	if (right) 	delete right;
	if (left)  	delete left;
}

bool CellHeapNode::is_sup(CellHeapNode *n, int ind_crit) const {
	return elt->is_sup(n->elt->crit[ind_crit],ind_crit);
}

bool CellHeapNode::isSup(double d, int ind_crit) const {
	return elt->is_sup(d,ind_crit);
}

void CellHeapNode::switch_elt(CellHeapNode *pt, int ind_crit) {
	// not forget indices permutation
	unsigned int ind_tmp = elt->indice[ind_crit];
	elt->indice[ind_crit] = pt->elt->indice[ind_crit];
	pt->elt->indice[ind_crit] = ind_tmp;

	CellHeapElt * elt_tmp = elt;
	elt = pt->elt;
	pt->elt = elt_tmp;

}

//////////////////////////////////////////////////////////////////////////////////////
/** create an node with a cell and its criterion */
//CellHeapElt::CellHeapElt(int nb_crit,Cell* cell, double *crit_in) : cell(cell), crit(new double[nb_crit]), indice(new unsigned int[nb_crit]){
//	for (int i=0;i<nb_crit;i++) {
//		crit[i] = crit_in[i];
//		indice[i] = 0;
//	}
//}

CellHeapElt::CellHeapElt(Cell* cell, double crit_1) : cell(cell), crit(new double[1]), indice(new unsigned int[1]){
		crit[0] = crit_1;
		indice[0] = 0;
}
CellHeapElt::CellHeapElt(Cell* cell, double crit_1, double crit_2) : cell(cell), crit(new double[2]), indice(new unsigned int[2]){
		crit[0] = crit_1;
		crit[1] = crit_2;
		indice[0] = 0;
		indice[1] = 0;
}


/** Delete the node and all its sons */
CellHeapElt::~CellHeapElt() {
	if (cell) 	delete cell;
	delete [] crit;
	delete [] indice;
}

bool CellHeapElt::is_sup(double d, int ind_crit) const {
	return (crit[ind_crit] >= d);
}



//////////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const CellHeapElt& node) {
	os << "n°"<<node.indice[0]<< *(node.cell);
	return os;
}

std::ostream& operator<<(std::ostream& os, const CellHeapNode& node) {
	os  <<*(node.elt) << " ";
	if (node.left)  os <<  *(node.left);
	if (node.right) os <<  *(node.right);
	return os;
}


std::ostream& operator<<(std::ostream& os, const CellHeap_2& heap) {
	os << "[ ";
	if (heap.root) 	os << *(heap.root) << " ";
	return os << "]";
}


}  // end namespace ibex
