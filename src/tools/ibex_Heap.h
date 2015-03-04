//============================================================================
//                                  I B E X                                   
// File        : ibex_Heap.h
// Author      : Gilles Chabert, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 23, 2014
//============================================================================

#ifndef __IBEX_HEAP_H__
#define __IBEX_HEAP_H__

#include <iostream>
#include <cassert>

namespace ibex {

/** \ingroup strategy
 *
 * \brief Heap
 *
 * The heap is built so that:
 *  <ul>
 *  <li> #pop() returns in logarithmic time
 *       the element with the minimal "cost" (criterion).</li>
 *  <li> #push() is also in logarithmic time.</li>
 *  </ul>
 *
 */
template<class T> class HeapNode;
template<class T> class HeapElt;
template<class T> class DoubleHeap;

template<class T>
class CostFunc {
public:
	virtual ~CostFunc() { }
	virtual double cost(const T&) const=0;
};

template<class T>
class SharedHeap  {

public:

	/** create this with a specific identifier, and if you need to update the cost a each contract of sort. */
	SharedHeap(CostFunc<T>& cost, int id=0, bool updateCost=false);

	/** Delete this. */
	virtual  ~SharedHeap();

	/**
	 * \brief Flush the buffer.
	 *
	 * All the remaining elements will be *deleted*
	 * (with a call to the destructor of class T).
	 */
	void flush();

	/** Return the size of the buffer. */
	unsigned int size() const;

	/** Return true if the buffer is empty. */
	bool empty() const;

	/** push a new element on the stack.
	 * Complexity: o(log(nb_cells))
	 */
	void push(T* el);

	/** Pop a element from the stack and return it.
	 * Complexity: o(log(nb_cells))
	 */
	T* pop();

	/** Return the next box (but does not pop it).
	 * Complexity: o(1)
	 */
	T* top() const;

//	/**
//	 * \brief Contracts the heap.
//	 *
//	 * Removes (and deletes) from the heap all the elements
//	 * with a cost greater than \a lb.
//	 */
//	void contract(double lb);

	/** Return the minimum (the criterion for
	 * the first element) */
	inline double minimum() const{	return root->elt->crit[heap_id];	}

//	/**
//	 * \brief Access to the ith Cell rank by largest-first order
//	 * Complexity: o(log(nb_cells))
//	 */
//	T* get(unsigned int i) const;

	/**
	 * \brief update the cost and sort all the heap
	 * complexity: o(nb_cells*log(nb_cells))
	 */
	void sort();

	/** Count the number of cells pushed since
	 * the object is created. */
	unsigned int nb_cells;

	//	inline bool needUpdate() const {return updateCost;}

	inline int get_id() const {return heap_id;}

	CostFunc<T>& costf;

	/** Identifier of this heap */
	const int heap_id;

	/**
	 * A boolean which indicate if all the cost must be update at each contract and sort
	 */
	const bool updateCost;

//protected:

	friend class DoubleHeap<T>;

	/** The "cost" of an element. */
	double cost(const T& data) const { return costf.cost(data); }


	/** The root of the heap */
	HeapNode<T>* root;

	/**
	 * Pop an element and return it.
	 *
	 * Complexity: O(log(nb_cells))
	 */
	HeapElt<T>* pop_elt();

	/**
	 * Push an element
	 *
	 * Complexity: O(log(nb_cells))
	 */
	void push_elt(HeapElt<T>* elt);

	/**
	 * Percolate (or "heapify") from the node \var node downto the bottom.
	 */
	void percolate(HeapNode<T>* node);

	/**
	 * \brief Remove the ith element and update the heap in consequence.
	 *
	 * (call percolate).
	 */
	void erase_node(HeapNode<T>* node);

	/**
	 * \brief Remove the ith element.
	 *
	 * The last node is actually removed and its element is put in place
	 * of the ith position element.
	 *
	 * The heap is not updated after (the new ith node is not at its right place
	 * anymore and the heap is in undefined state).
	 *
	 * Complexity: O(1)
	 */
	HeapNode<T>* erase_node_no_percolate(HeapNode<T>* node);

	/**
	 * \brief Access to the ith node, rank by largest-first order.
	 *
	 * \param i - the node number
	 */
	HeapNode<T>* get_node(unsigned int i) const;

	/**
	 * \brief Streams out the heap
	 */
	std::ostream& print(std::ostream& os) const;

private:
	/** Used in the contract function (proceed by recursivity) */
	void contract_rec(double new_loup, HeapNode<T>* node, SharedHeap<T>& heap);

	/** Used in the sort function (proceed by recursivity) */
	void sort_rec(HeapNode<T>* node, SharedHeap<T>& heap);

};



/**
 * \ingroup strategy
 *
 * \brief Heap node
 *
 */
template<class T>
class HeapNode {

//private:
	friend class SharedHeap<T>;
	friend class DoubleHeap<T>;

	/** Create a node from an element and the father node. */
	explicit HeapNode(HeapElt<T>* elt, HeapNode<T>* father=NULL);

	/** Delete the node and all its sons */
	~HeapNode() ;

	/** the stored element. */
	HeapElt<T>* elt;

	/** Right sub-node */
	HeapNode<T>* right;

	/** Left sub-node */
	HeapNode<T>* left;

	/** Father node. */
	HeapNode<T>* father;

	/** The way to compare two pairs (cells,crit). */
	bool isSup(HeapNode<T>* node, int heap_id) const;
	bool isSup(double d, int heap_id) const ;

	/** Switch the CellHeapElt between *this and node */
	void switchElt(HeapNode<T>* node, int heap_id);

	template<class U>
	friend std::ostream& operator<<(std::ostream& os, const HeapNode<U>& node) ;

};





template<class T>
class HeapElt {

//private:
	friend class HeapNode<T>;
	friend class SharedHeap<T>;
	friend class DoubleHeap<T>;

	/** Create an CellHeapElt with a cell and its criteria */
	//CellHeapElt(int nb_crit, T* elt, double *crit);

	/** Create an HeapElt with a cell and one criterion */
	explicit HeapElt(T* cell, double crit_1);

	/** Create an HeapElt with a cell and two criteria */
	explicit HeapElt(T* cell, double crit_1, double crit_2);

	/** Delete the element */
	~HeapElt() ;

	/**
	 * Compare the criterion of a given heap with the value d.
	 * Return true if the criterion is greater.
	 */
	bool isSup(double d, int heap_id) const ;

	/** the stored Cell */
	T* cell;

	/** The number of different heaps this element belongs to */
	int nb_heaps;

	/** the criteria of the stored cell (one for each heap it
	 * belongs to). */
	double *crit;

	/** The index of this element in each heap it belongs to. */
	//unsigned int *indice;
	HeapNode<T>** holder;

	template<class U>
	friend std::ostream& operator<<(std::ostream& os, const HeapElt<U>& node) ;
};





/*================================== inline implementations ========================================*/




template<class T>
SharedHeap<T>::SharedHeap(CostFunc<T>& cost, int id,bool updateCost) :
nb_cells(0), costf(cost), heap_id(id), updateCost(updateCost),  root(NULL) {

}
template<class T>
SharedHeap<T>::~SharedHeap() {
	if (root) delete root; 	// warning: delete all sub-nodes
	root = NULL;
}

template<class T>
void SharedHeap<T>::flush() {
	if (root) delete root; 	// warning: delete all sub-nodes
	nb_cells =0;
	root = NULL;
}

template<class T>
unsigned int SharedHeap<T>::size() const {
	return nb_cells;
}

template<class T>
bool SharedHeap<T>::empty() const {
	return (nb_cells==0);
}

template<class T>
T* SharedHeap<T>::top() const {
	return root->elt->cell;
}



//// E.g.: called by manage_buffer in Optimizer in case of a new upper bound
//// on the objective ("loup"). This function then removes (and deletes) from
//// the heap all the cells with a cost greater than loup.
//template<class T>
//void SharedHeap<T>::contract(double new_loup) {
//	if (nb_cells==0) return;
//
//	SharedHeap<T> * heap_tmp = new SharedHeap<T>(costf, heap_id, updateCost);
//
//	contract_rec(new_loup, root, *heap_tmp);
//
//	root = heap_tmp->root;
//	nb_cells = heap_tmp->size();
//	heap_tmp->root = NULL;
//	delete heap_tmp;
//
//}


template<class T>
void SharedHeap<T>::contract_rec(double new_loup, HeapNode<T>* node, SharedHeap<T>& heap) {

	if(updateCost)
		node->elt->crit[heap_id] = cost(*(node->elt->cell));

	if (!(node->isSup(new_loup, heap_id))) {
		heap.push_elt(node->elt);
		if (node->left)	 contract_rec(new_loup, node->left, heap);
		if (node->right) contract_rec(new_loup, node->right, heap);
		node->left=NULL;
		node->right=NULL;
		node->elt=NULL;
	}
	delete node;
	node= NULL;
}

template<class T>
void SharedHeap<T>::sort() {
	if (nb_cells==0) return;

	SharedHeap<T>* heap_tmp = new SharedHeap<T>(costf, heap_id, updateCost);

	// recursive sort : o(n*log(n))
	sort_rec(root, *heap_tmp);

	root = heap_tmp->root;
	nb_cells = heap_tmp->size();

	heap_tmp->root = NULL;
	delete heap_tmp; 	// warning: delete all sub-nodes

}

template<class T>
void SharedHeap<T>::sort_rec(HeapNode<T> * node, SharedHeap<T> & heap) {

	if (updateCost)
		node->elt->crit[heap_id] = cost(*(node->elt->cell));

	heap.push_elt(node->elt);
	if (node->left)	 sort_rec(node->left, heap);
	if (node->right) sort_rec(node->right, heap);

	node->left=NULL;
	node->right=NULL;
	node->elt = NULL;
	delete node;	// warning: delete all sub-nodes
	node = NULL;
}


template<class T>
void SharedHeap<T>::push(T* cell) {
	push_elt(new HeapElt<T>(cell,cost(*cell)));
}



template<class T>
void SharedHeap<T>::push_elt(HeapElt<T>* elt) {

	if (nb_cells==0) {
		root = new HeapNode<T>(elt,NULL);
		root->elt->holder[heap_id] = root;
		nb_cells++;
	} else {
		nb_cells++;
		// calculate height
		int height = 0;
		int aux = nb_cells;
		while(aux>1) { aux /= 2; height++;	}

		// pt = pointer on the current element
		HeapNode<T> * pt = root;
		for (int pos=height-1; pos>0; pos--) {
			if ( nb_cells & (1 << pos)) {  // test the "pos"th bit of the integer nb_cells
				pt = pt->right;
			} else {
				pt = pt->left;
			}
		}
		HeapNode<T>* tmp= new HeapNode<T>(elt,pt);
		tmp->elt->holder[heap_id] = tmp;
		if (nb_cells%2==0)	{ pt->left =tmp; }
		else 				{ pt->right=tmp; }
		pt = tmp;

		// PT indicates the added element
		// we have now to perform an update upto the root
		bool b = true;
		while (b&&(pt->father)) {
			if (pt->father->isSup(pt,heap_id)) {
				pt->switchElt(pt->father,heap_id);
				pt = pt->father;
			} else {
				b= false;
			}
		}

	}
}

template<class T>
HeapNode<T> * SharedHeap<T>::get_node(unsigned int i) const {
	assert(i<nb_cells);
	assert(nb_cells>0);

	// GET THE LAST ELEMENT and delete the associated node
	// calculate height
	int height = 0;
	unsigned int aux = i+1;
	while(aux>1) { aux /= 2; height++; }
	//std::cout << "height of "<<i<<"  = " << height <<std::endl;
	// pt = pointer on the current element
	HeapNode<T>* pt = root;
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

//template<class T>
//T* SharedHeap<T>::get(unsigned int i) const {
//	assert(i<nb_cells);
//	assert(nb_cells>0);
//	return get_node(i)->elt->cell;
//}

template<class T>
T* SharedHeap<T>::pop() {
	assert(nb_cells>0);
	HeapElt<T>* tmp =pop_elt();
	T * c = tmp->cell;
	tmp->cell = NULL;
	delete tmp;
	tmp=NULL;
	return c;
}

template<class T>
HeapElt<T>* SharedHeap<T>::pop_elt() {
	assert(nb_cells>0);
	HeapElt<T>* c_return = root->elt;
	erase_node(root);
	return c_return;
}

// erase a node and update the order
template<class T>
void SharedHeap<T>::erase_node(HeapNode<T>* node) {
	assert(node);
	assert(nb_cells>0);

	if (erase_node_no_percolate(node)) percolate(node);
}

// erase a node and update the order
template<class T>
HeapNode<T>* SharedHeap<T>::erase_node_no_percolate(HeapNode<T>* pt_root) {
	assert(nb_cells>0);

	if (nb_cells==1) {
		assert(pt_root==root);
		root->elt=NULL;
		delete root;
		root = NULL;
		pt_root = NULL; // return NULL
	} else {
		// GET THE LAST ELEMENT and delete the associated node without destroying the HeapElt
		HeapNode<T>* last = get_node(nb_cells-1);

		// cell points on the last node data, that we will put in place of the deleted node
		HeapElt<T>* cell = last->elt;

		if (nb_cells%2==0)	{ last->father->left =NULL; }
		else 				{ last->father->right=NULL; }

		// to avoid the element to be deleted with the node pt
		last->elt = NULL;

		if (pt_root!=last) { // if the node to be deleted is not the last
			pt_root->elt = cell;
			pt_root->elt->holder[heap_id] = pt_root;
		} else {
			pt_root = NULL; // return NULL
		}
		delete last;
	}
	nb_cells--;
	return pt_root;
}



template<class T>
void SharedHeap<T>::percolate(HeapNode<T> *node) {
	assert(node);

	// PERMUTATION to maintain the order in the heap when going down
	if (node) {
		bool b=true;
		while (b && (node->left)) {
			if (node->right) {
				if (node->isSup(node->left,heap_id)) {
					if (node->right->isSup(node->left,heap_id)) {
						// left is the smallest
						node->switchElt(node->left,heap_id);
						node = node->left;  // next
					} else {
						// right is the smallest
						node->switchElt(node->right,heap_id);
						node = node->right;  // next
					}
				} else {
					if (node->isSup(node->right,heap_id)) {
						// right is the smallest
						node->switchElt(node->right,heap_id);
						node = node->right;  // next
					} else { // current node is the smallest among the 3: stop
						b=false;
					}
				}
			} else { // no more right child but there is a left child
				if (node->isSup(node->left,heap_id)) {
					// left is the smallest
					node->switchElt(node->left,heap_id);
					node = node->left;  // next
				} else { // current node is the smallest among the 3: stop (and we must have reached the bottom of the heap)
					b=false;
				}
			}
		}
	}
}



//////////////////////////////////////////////////////////////////////////////////////
//HeapNode::HeapNode(): elt(NULL), right(NULL), left(NULL), father(NULL) { }

template<class T>
HeapNode<T>::HeapNode(HeapElt<T>* elt, HeapNode<T> * father): elt(elt), right(NULL), left(NULL), father(father) { }

template<class T>
HeapNode<T>::~HeapNode() {
//	// warning: delete all sub-nodes
//
//	if (elt) {
//		// Next loop will allow us to call the destructor
//		// on other holders of the same element "elt"
//		// They will not try to delete twice "elt".
//		for (int i=0; i<elt->nb_heaps; i++) {
//			if (elt->holder[i]!=this) // skip myself
//				elt->holder[i]->elt=NULL;
//		}
//		delete elt;
//	}
//
//	if (right) 	delete right;
//	if (left)  	delete left;
}

template<class T>
bool HeapNode<T>::isSup(HeapNode<T>* node, int heap_id) const {
	return elt->isSup(node->elt->crit[heap_id],heap_id);
}

template<class T>
bool HeapNode<T>::isSup(double d, int heap_id) const {
	return elt->isSup(d,heap_id);
}

template<class T>
void HeapNode<T>::switchElt(HeapNode<T>* node, int heap_id) {

	elt->holder[heap_id] = node;
	node->elt->holder[heap_id] = this;

	HeapElt<T> * elt_tmp = elt;
	elt = node->elt;
	node->elt = elt_tmp;

}

//////////////////////////////////////////////////////////////////////////////////////
/** create an node with a cell and its criterion */
//CellHeapElt::CellHeapElt(int nb_crit,T* cell, double *crit_in) : cell(cell), crit(new double[nb_crit]), indice(new unsigned int[nb_crit]){
//	for (int i=0;i<nb_crit;i++) {
//		crit[i] = crit_in[i];
//		indice[i] = 0;
//	}
//}

template<class T>
HeapElt<T>::HeapElt(T* cell, double crit_1) : cell(cell), nb_heaps(1), crit(new double[1]), holder(new HeapNode<T>*[1]){
	crit[0] = crit_1;
	holder[0] = NULL;
}
template<class T>
HeapElt<T>::HeapElt(T* cell, double crit_1, double crit_2) : cell(cell), nb_heaps(2), crit(new double[2]), holder(new HeapNode<T>*[2]){
	crit[0] = crit_1;
	crit[1] = crit_2;
	holder[0] = NULL;
	holder[1] = NULL;
}


/** Delete the node and all its sons */
template<class T>
HeapElt<T>::~HeapElt() {
	if (cell) 	delete cell;
	delete [] crit;
	delete [] holder;
}

template<class T>
bool HeapElt<T>::isSup(double d, int ind_crit) const {
	return (crit[ind_crit] >= d);
}



//////////////////////////////////////////////////////////////////////////////////////
template<class T>
std::ostream& operator<<(std::ostream& os, const HeapElt<T>& node) {
	os << "n°"<<node.indice[0]<< *(node.cell);
	return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const HeapNode<T>& node) {
	os  <<*(node.elt) << " ";
	if (node.left)  os <<  *(node.left);
	if (node.right) os <<  *(node.right);
	return os;
}

template<class T>
std::ostream& SharedHeap<T>::print(std::ostream& os) const{
	os << "[ ";
	if (root) 	os << *(root) << " ";
	return os << "]";
}



} // end namespace ibex
#endif // __IBEX_HEAP_H__
