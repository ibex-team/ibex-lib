//============================================================================
//                                  I B E X                                   
// File        : ibex_DoubleHeap.h
// Author      : Gilles Chabert, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 12, 2014
//============================================================================

#ifndef __IBEX_DOUBLE_HEAP_H__
#define __IBEX_DOUBLE_HEAP_H__

#include "ibex_Heap.h"
#include <stdlib.h> // for rand()

namespace ibex {

/**
 * \brief Double-heap
 */
template<class T>
class DoubleHeap {
public:

	DoubleHeap(CostFunc<T>& cost1, CostFunc<T>& cost2, int critpr);

	/** Flush the buffer.
	 * All the remaining cells will be *deleted* */
	void flush();

	/** Return the size of the buffer. */
	unsigned int size() const;

	/** Return true if the buffer is empty. */
	bool empty() const;

	/** push a new cell on the stack. */
	void push(T* cell);

	/** Pop a cell from the stack and return it.*/
	T* pop();

	/** Pop a cell from the first heap and return it.*/
	T* pop1();

	/** Pop a cell from the second heap and return it.*/
	T* pop2();

	/** Return the next box (but does not pop it).*/
	T* top() const;

	/** Return the next box of the first heap (but does not pop it).*/
	T* top1() const;

	/** Return the next box of the second heap  (but does not pop it).*/
	T* top2() const;

	/**
	 * Return the minimum (the criterion for the first heap)
	 *
	 * Complexity: o(1)
	 */
	inline double minimum() const {	return heap1->minimum(); }

	/**
	 * Return the first minimum (the criterion for the first heap)
	 *
	 * Complexity: o(1)
	 */
	inline double minimum1() const {	return heap1->minimum(); }

	/**
	 * Return the second minimum (the criterion for the second heap)
	 *
	 * Complexity: o(1)
	 */
	inline double minimum2() const {	return heap2->minimum(); }

	/**
	 * Removes (and deletes) from the heap all the cells
	 * with a cost greater than \a loup.
	 */
	void contract(double loup);

	/**
	 * Delete this
	 */
	virtual ~DoubleHeap();

	template<class U>
	friend std::ostream& operator<<(std::ostream& os, const DoubleHeap<U>& heap);

//protected:
	/** Count the number of cells pushed since
	 * the object is created. */
	unsigned int nb_cells;

	/** the first heap */
	SharedHeap<T> *heap1;

	/** the second heap */
	SharedHeap<T> *heap2;

	/** Probability to choose the second
	 * (see details in the constructor) */
	const int critpr;

	/** Current selected buffer. */
	mutable int indbuf;

	/** use in the contractHeap function by recursivity */
	void contract_rec(double new_loup, HeapNode<T>* node, SharedHeap<T>& heap);

	/** erase a node in the second heap */
	void erase_subnodes(HeapNode<T>* node, bool percolate);

	std::ostream& print(std::ostream& os) const;
};


/*================================== inline implementations ========================================*/

template<class T>
DoubleHeap<T>::DoubleHeap(CostFunc<T>& cost1, CostFunc<T>& cost2, int critpr) :
		 nb_cells(0), heap1(new SharedHeap<T>(cost1,0,false)),
		heap2(new SharedHeap<T>(cost2,1)), critpr(critpr), indbuf(0) {

}

template<class T>
DoubleHeap<T>::~DoubleHeap() {
	if (heap1) delete heap1;
	if (heap2) delete heap2;
}

template<class T>
void DoubleHeap<T>::flush() {
	heap1->flush();
	heap2->flush();
//	if (nb_cells>0) {
//		erase_subnodes(heap1->root,false);
//		heap1->nb_cells=0;
//		nb_cells=0;
//	}
}

template<class T>
unsigned int DoubleHeap<T>::size() const {
	assert(heap1->size()==heap2->size());
	return nb_cells;
}

template<class T>
void DoubleHeap<T>::contract(double new_loup) {

	if (nb_cells==0) return;

	SharedHeap<T>* copy1 = new SharedHeap<T>(heap1->costf, 0, heap1->updateCost);

	contract_rec(new_loup, heap1->root, *copy1);

	heap1->root = copy1->root;
	heap1->nb_cells = copy1->size();
	nb_cells = copy1->size();
	copy1->root = NULL; // avoid to delete heap1 with copy1
	delete copy1;

	assert(nb_cells==heap2->size());
	assert(nb_cells==heap1->size());

}


template<class T>
void DoubleHeap<T>::contract_rec(double new_loup, HeapNode<T>* node, SharedHeap<T>& heap) {

	if (node->isSup(new_loup, 0)) {
		// we must remove from the other heap all the sub-nodes
		if (heap2) erase_subnodes(node, heap2->updateCost);
	} else {
		heap.push_elt(node->elt);
		if (node->left)	 contract_rec(new_loup, node->left, heap);
		if (node->right) contract_rec(new_loup, node->right, heap);

		delete node;
	}
}
template<class T>
void DoubleHeap<T>::erase_subnodes(HeapNode<T>* node, bool percolate) {
	if (node->left)	erase_subnodes(node->left, percolate);
	if (node->right) erase_subnodes(node->right, percolate);

	if (!percolate)
		// there is no need to update the order now in the second heap
		// since all costs will have to be recalculated.
		// The heap2 will be sorted at the end (see contract)
		heap2->erase_node_no_percolate(node->elt->holder[1]);
	else
		heap2->erase_node(node->elt->holder[1]);

	delete node->elt;
	delete node;
}
template<class T>
bool DoubleHeap<T>::empty() const {
	// if one buffer is empty, the other is also empty
	return (nb_cells==0);
}
template<class T>
void DoubleHeap<T>::push(T* cell) {
	HeapElt<T>* elt;
	if (heap2) {
		elt = new HeapElt<T>(cell, heap1->cost(*cell), heap2->cost(*cell));
	} else {
		elt = new HeapElt<T>(cell, heap1->cost(*cell));
	}

	// the cell is put into the first heap
	heap1->push_elt(elt);
	if (heap2) heap2->push_elt(elt);

	nb_cells++;
}
template<class T>
T* DoubleHeap<T>::pop() {
	// Select the heap
	HeapElt<T>* elt;
	if (indbuf==0) {
		elt = heap1->pop_elt();
		if (heap2) heap2->erase_node(elt->holder[1]);
	} else {
		elt = heap2->pop_elt();
		heap1->erase_node(elt->holder[0]);
	}
	T* cell = elt->cell;
	elt->cell=NULL; // avoid the cell to be deleted with the element
	delete elt;

	nb_cells--;

	return cell;
}

// Invariant: the top of each heaps is an alive cell
template<class T>
T* DoubleHeap<T>::top() const {

	// Select the heap
	if (rand() % 100 >=critpr) {
		// the first heap is used
		indbuf=0;
		return heap1->top();
	}
	else {
		// the second heap is used
		indbuf=1;
		return heap2->top();
	}
}

// Invariant: the top of each heaps is an alive cell
template<class T>
T* DoubleHeap<T>::top1() const {
	// the first heap is used
	indbuf=0;
	return heap1->top();
}

// Invariant: the top of each heaps is an alive cell
template<class T>
T* DoubleHeap<T>::top2() const {
	// the second heap is used
	indbuf=1;
	return heap2->top();

}

template<class T>
std::ostream& DoubleHeap<T>::print(std::ostream& os) const{
	os << "First Heap:  "<< heap1 <<std::endl;
	os << "Second Heap: "<< heap2 <<std::endl;
	return os;

}

template<class T>
std::ostream& operator<<(std::ostream& os, const DoubleHeap<T>& heap) {
	return heap.print(os);
}


} // namespace ibex

#endif // __IBEX_DOUBLE_HEAP_H__
