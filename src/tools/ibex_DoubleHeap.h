//============================================================================
//                                  I B E X                                   
// File        : ibex_DoubleHeap.h
// Author      : Gilles Chabert, Jordan Ninin
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 12, 2014
// Last Update : Dec 25, 2017
//============================================================================

#ifndef __IBEX_DOUBLE_HEAP_H__
#define __IBEX_DOUBLE_HEAP_H__

#include "ibex_SharedHeap.h"
#include "ibex_Random.h"

namespace ibex {

/**
 * \brief Double-heap
 */
template<class T>
class DoubleHeap {
public:
	/**
	 * \brief Create a double heap
	 *
	 * \param cost1                     - cost function for the first heap
	 * \param update_cost1_when_sorting - whether this cost is recalculated when sorting
	 * \param cost2                     - cost function for the second heap
	 * \param update_cost2_when_sorting - whether this cost is recalculated when sorting
	 * \param critpr                    - probability to chose the second heap as an integer in [0,100] (default value 50).
	 *                                    Value 0 or correspond to use a single criterion for node selection
	 *                                    > 0 = only the first heap
	 *                                    > 100 = only the second heap.
	 */
	DoubleHeap(CostFunc<T>& cost1, bool update_cost1_when_sorting, CostFunc<T>& cost2, bool update_cost2_when_sorting, int critpr=50);

	/**
 		 * \brief Copy constructor.
	 */
	explicit DoubleHeap(const DoubleHeap& dhcp, bool deep_copy=false);

	/**
	 * \brief Flush the heap.
	 *
	 * All the remaining data will be *deleted*.
	 */
	void flush();

	/**
	 * \brief Clear the heap.
	 *
	 * All the remaining data will be *removed* without being *deleted*.
	 */
	void clear();

	/** \brief Return the size of the buffer. */
	unsigned int size() const;

	/** \brief Return true if the buffer is empty. */
	bool empty() const;

	/** \brief Push new data on the heap. */
	void push(T* data);

	/** \brief Pop data from the stack and return it.*/
	T* pop();

	/** \brief Pop data from the first heap and return it.*/
	T* pop1();

	/** \brief Pop data from the second heap and return it.*/
	T* pop2();

	/** \brief Return next data (but does not pop it).*/
	T* top() const;

	/** \brief Return next data of the first heap (but does not pop it).*/
	T* top1() const;

	/** \brief Return next data of the second heap  (but does not pop it).*/
	T* top2() const;

	/**
	 * \brief Return the minimum (the criterion for the first heap)
	 *
	 * Complexity: o(1)
	 */
	double minimum() const;

	/**
	 * \brief Return the first minimum (the criterion for the first heap)
	 *
	 * Complexity: o(1)
	 */
	double minimum1() const;

	/**
	 * \brief Return the second minimum (the criterion for the second heap)
	 *
	 * Complexity: o(1)
	 */
	double minimum2() const;

	/**
	 * \brief Contract the heap
	 *
	 * Removes (and deletes) from the two heaps all the data
	 * with a cost (according to the cost function of the first heap)
	 * that is greater than \a loup1.
	 *
	 * The costs of the first heap are assumed to be up-to-date.
	 *
	 * TODO: in principle we should implement the symmetric
	 * case where the contraction is performed with respect
	 * to the cost of the second heap.
	 */
	void contract(double loup1);

	/**
	 * \brief Delete this
	 */
	virtual ~DoubleHeap();

	template<class U>
	friend std::ostream& operator<<(std::ostream& os, const DoubleHeap<U>& heap);

protected:
	/** Count the number of nodes pushed since
	 * the object is created. */
	unsigned int nb_nodes;

	/** the first heap */
	SharedHeap<T> *heap1;

	/** the second heap */
	SharedHeap<T> *heap2;

	/** Probability to choose the second
	 * (see details in the constructor) */
	const int critpr;

	/** Current selected heap. */
	mutable int current_heap_id;

	/**
	 * Used in the contract function by recursivity
	 *
	 * \param heap: the new heap1 under construction (will
	 *        eventually replace the current heap1).
	 */
	void contract_rec(double new_loup, HeapNode<T>* node, SharedHeap<T>& heap, bool percolate);

private:
	/**
	 * Erase all the subnodes of node (including itself) in the first heap
	 * and manage the impact on the second heap.
	 * If "percolate" is true, the second heap is left in a correct state.
	 * Otherwise, the second heap has a binary tree structure but not sorted
	 * anymore. Therefore, "sort" should be called on the second heap.
	 *
	 * So the heap structure is maintained for the second heap
	 * but not the first one. The reason is that this function is called
	 * either by "contract" or "flush". "contract" will build a new heap from scratch.
	 */
	void erase_subnodes(HeapNode<T>* node, bool percolate);

	std::ostream& print(std::ostream& os) const;
};


/*================================== inline implementations ========================================*/

template<class T>
DoubleHeap<T>::DoubleHeap(CostFunc<T>& cost1, bool update_cost1_when_sorting, CostFunc<T>& cost2, bool update_cost2_when_sorting, int critpr) :
		 nb_nodes(0), heap1(new SharedHeap<T>(cost1,update_cost1_when_sorting,0)),
		              heap2(new SharedHeap<T>(cost2,update_cost2_when_sorting,1)),
		              critpr(critpr), current_heap_id(0) {

}

template<class T>
DoubleHeap<T>::DoubleHeap(const DoubleHeap &dhcp, bool deep_copy) :
nb_nodes(dhcp.nb_nodes), heap1(NULL), heap2(NULL), critpr(dhcp.critpr), current_heap_id(dhcp.current_heap_id) {
	heap1 = new SharedHeap<T>(*dhcp.heap1, 2, deep_copy);
	std::vector<HeapElt<T>*> p = heap1->elt();
	heap2 = new SharedHeap<T>(dhcp.heap2->costf, dhcp.heap2->update_cost_when_sorting, dhcp.heap2->heap_id);

	while(!p.empty()) {
		heap2->push_elt(p.back());
		p.pop_back();
	}
}

template<class T>
DoubleHeap<T>::~DoubleHeap() {
	clear(); // what for?
	if (heap1) delete heap1;
	if (heap2) delete heap2;

}

template<class T>
void DoubleHeap<T>::flush() {
	if (nb_nodes>0) {
		heap1->clear(SharedHeap<T>::NODE);
		heap2->clear(SharedHeap<T>::NODE_ELT_DATA);
		nb_nodes=0;
	}
}

template<class T>
void DoubleHeap<T>::clear() {
	if (nb_nodes>0) {
		heap1->clear(SharedHeap<T>::NODE);
		heap2->clear(SharedHeap<T>::NODE_ELT);
		nb_nodes=0;
	}
}

template<class T>
unsigned int DoubleHeap<T>::size() const {
	assert(heap1->size()==heap2->size());
	return nb_nodes;
}

template<class T>
void DoubleHeap<T>::contract(double new_loup1) {

	if (nb_nodes==0) return;

	SharedHeap<T>* copy1 = new SharedHeap<T>(heap1->costf, heap1->update_cost_when_sorting, 0);

	contract_rec(new_loup1, heap1->root, *copy1, !heap2->update_cost_when_sorting);

	heap1->root = copy1->root;
	heap1->nb_nodes = copy1->size();
	nb_nodes = copy1->size();
	copy1->root = NULL;
	copy1->nb_nodes=0;// avoid to delete heap1 with copy1
	delete copy1;

	if (heap2->update_cost_when_sorting) heap2->sort();

	assert(nb_nodes==heap2->size());
	assert(nb_nodes==heap1->size());
	assert(heap1->heap_state());
	assert(!heap2 || heap2->heap_state());
}


template<class T>
void DoubleHeap<T>::contract_rec(double new_loup1, HeapNode<T>* node, SharedHeap<T>& heap, bool percolate) {

	// the cost are assumed to be up-to-date for the 1st heap
	if (node->is_sup(new_loup1, 0)) {
		// we must remove from the other heap all the sub-nodes
		if (heap2) erase_subnodes(node, percolate);
	} else {
		heap.push_elt(node->elt);
		if (node->left)	 contract_rec(new_loup1, node->left, heap, percolate);
		if (node->right) contract_rec(new_loup1, node->right, heap, percolate);

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

	if (node->elt->data) delete node->elt->data;
	delete node->elt;
	delete node;
}

template<class T>
bool DoubleHeap<T>::empty() const {
	// if one buffer is empty, the other is also empty
	return (nb_nodes==0);
}

template<class T>
void DoubleHeap<T>::push(T* data) {
	HeapElt<T>* elt;
	if (heap2) {
		elt = new HeapElt<T>(data, heap1->cost(*data), heap2->cost(*data));
	} else {
		elt = new HeapElt<T>(data, heap1->cost(*data));
	}

	// the data is put into the first heap
	heap1->push_elt(elt);
	if (heap2) heap2->push_elt(elt);

	nb_nodes++;
}

template<class T>
T* DoubleHeap<T>::pop() {
	assert(size()>0);

	//std::cout << " \n\n Heap1=" << (*heap1);

	// Select the heap
	HeapElt<T>* elt;
	if (current_heap_id==0) {
		elt = heap1->pop_elt();
		if (heap2) heap2->erase_node(elt->holder[1]);
	} else {
		elt = heap2->pop_elt();
		heap1->erase_node(elt->holder[0]);
	}
	T* data = elt->data;
	elt->data=NULL; // avoid the data to be deleted with the element
	delete elt;

	nb_nodes--;

	assert(heap1->heap_state());
	assert(!heap2 || heap2->heap_state());

	// select the heap
	if (RNG::rand() % 100 >= static_cast<unsigned>(critpr)) {
		current_heap_id=0;
	}
	else {
		current_heap_id=1;
	}

	return data;
}

template<class T>
T* DoubleHeap<T>::pop1()  {
	// the first heap is used
	current_heap_id=0;
	return pop();
}

template<class T>
T* DoubleHeap<T>::pop2()  {
	// the second heap is used
	current_heap_id=1;
	return pop();

}

template<class T>
T* DoubleHeap<T>::top() const {
	assert(size()>0);

	if (current_heap_id==0) {
		return heap1->top();
	}
	else {
		// the second heap is used
		return heap2->top();
	}
}

template<class T>
T* DoubleHeap<T>::top1() const {
	// the first heap is used
	current_heap_id=0;
	return heap1->top();
}

template<class T>
T* DoubleHeap<T>::top2() const {
	// the second heap is used
	current_heap_id=1;
	return heap2->top();

}

template<class T>
inline double DoubleHeap<T>::minimum() const {	return heap1->minimum(); }

template<class T>
inline double DoubleHeap<T>::minimum1() const { return heap1->minimum(); }

template<class T>
inline double DoubleHeap<T>::minimum2() const { return heap2->minimum(); }

template<class T>
std::ostream& DoubleHeap<T>::print(std::ostream& os) const{
	if (this->empty())  {
		os << " EMPTY ";
		os<<std::endl;
	} else {
		os << "First Heap:  "<<std::endl;
		heap1->print(os);
		os<<std::endl;
		os << "Second Heap: "<<std::endl;
		heap2->print(os);
		os<<std::endl;
	}
	return os;


}

template<class T>
std::ostream& operator<<(std::ostream& os, const DoubleHeap<T>& heap) {
	return heap.print(os);
}


} // namespace ibex

#endif // __IBEX_DOUBLE_HEAP_H__
