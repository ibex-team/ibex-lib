//============================================================================
//                                  I B E X                                   
// File        : ibex_SharedHeap.h
// Author      : Gilles Chabert, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 23, 2014
//============================================================================

#ifndef __IBEX_SHARED_HEAP_H__
#define __IBEX_SHARED_HEAP_H__

#include <iostream>
#include <cassert>
#include "ibex_Heap.h" // just for the declaration of CostFunc<T>

namespace ibex {

template<class T> class HeapNode;
template<class T> class HeapElt;
template<class T> class DoubleHeap;


/**
 * \brief Shared heap (internal)
 *
 * Class to be used by DoubleHeap only.
 *
 * Important remark: no function of this class creates or
 * destroy an element (HeapElt). Therefore, there is no
 * impact on the other shared heaps.
 *
 * It is the role of DoubleHeap to manage shared heap
 * synchronization.
 *
 *
 * The heap is built so that:
 *  <ul>
 *  <li> #pop() returns in logarithmic time
 *       the element with the minimal "cost" (criterion).</li>
 *  <li> #push() is also in logarithmic time.</li>
 *  </ul>
 */
template<class T>
class SharedHeap  {

public:

	/** create this with a specific identifier, and if you need to update the cost a each contract of sort. */
	SharedHeap(CostFunc<T>& cost, int id=0);

	/** Delete this. */
	virtual  ~SharedHeap();

	/** Return the size of the buffer. */
	unsigned int size() const;

	/** Return true if the buffer is empty. */
	bool empty() const;

	/** Return the next box (but does not pop it).
	 * Complexity: o(1)
	 */
	T* top() const;

	/** Return the minimum (the criterion for
	 * the first element) */
	double minimum() const;

	/**
	 * \brief update the cost and sort all the heap
	 * complexity: o(nb_nodes*log(nb_nodes))
	 *
	 * \param update_cost: if true, costs are all recalculated
	 */
	void sort(bool update_cost);

	/** Count the number of nodes pushed since
	 * the object is created. */
	unsigned int nb_nodes;

	/**
	 * Cost function associated to this heap
	 */
	CostFunc<T>& costf;

	/** Identifier of this heap */
	const int heap_id;

protected:

	friend class DoubleHeap<T>;

	/** The "cost" of an element. */
	double cost(const T& data) const { return costf.cost(data); }

	/** The root of the heap */
	HeapNode<T>* root;

	/**
	 * Pop an element and return it.
	 *
	 * Complexity: O(log(nb_nodes))
	 */
	HeapElt<T>* pop_elt();

	/**
	 * Push an element
	 *
	 * Complexity: O(log(nb_nodes))
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

	/** Used in the sort function (proceed by recursivity) */
	void sort_rec(HeapNode<T>* node, SharedHeap<T>& heap, bool update_cost);

};



/**
 * \ingroup strategy
 *
 * \brief Shared heap node (internal)
 *
 * Class to be used by DoubleHeap only.
 */
template<class T>
class HeapNode {

private:
	friend class SharedHeap<T>;
	friend class DoubleHeap<T>;

	/** Create a node from an element and the father node. */
	explicit HeapNode(HeapElt<T>* elt, HeapNode<T>* father=NULL);

	/** Delete the node and all its sons */
	//~HeapNode() ;

	/** the stored element. */
	HeapElt<T>* elt;

	/** Right sub-node */
	HeapNode<T>* right;

	/** Left sub-node */
	HeapNode<T>* left;

	/** Father node. */
	HeapNode<T>* father;

	/** Compare the heap_id-cost of this node to "node". */
	bool is_sup(HeapNode<T>* node, int heap_id) const;

	/** Compare the heap_id-cost of this node to the value "d". */
	bool is_sup(double d, int heap_id) const;

	/** Switch the elements between *this and node */
	void switch_elt(HeapNode<T>* node, int heap_id);

	template<class U>
	friend std::ostream& operator<<(std::ostream& os, const HeapNode<U>& node) ;

};

/**
 * \ingroup strategy
 *
 * \brief Shared heap element (internal)
 *
 * Class to be used by DoubleHeap only.
 */
template<class T>
class HeapElt {

private:
	friend class HeapNode<T>;
	friend class SharedHeap<T>;
	friend class DoubleHeap<T>;

	/** Create an CellHeapElt with a data and its criteria */
	//CellHeapElt(int nb_crit, T* elt, double *crit);

	/** Create an HeapElt with a data and one criterion */
	explicit HeapElt(T* data, double crit_1);

	/** Create an HeapElt with a data and two criteria */
	explicit HeapElt(T* data, double crit_1, double crit_2);

	/** Delete the element */
	~HeapElt() ;

	/**
	 * Compare the criterion of a given heap with the value d.
	 * Return true if the criterion is greater.
	 */
	bool is_sup(double d, int heap_id) const;

	/** the stored data. */
	T* data;

//	/** The number of different heaps this element belongs to */
//	int nb_heaps;

	/** the criteria of the stored data (one for each heap this
	 * element belongs to). */
	double *crit;

	/** The node that holds this element, for each heap. */
	HeapNode<T>** holder;

	template<class U>
	friend std::ostream& operator<<(std::ostream& os, const HeapElt<U>& node) ;
};



/*================================== inline implementations ========================================*/


template<class T>
SharedHeap<T>::SharedHeap(CostFunc<T>& cost, int id) : nb_nodes(0), costf(cost), heap_id(id), root(NULL) {

}

template<class T>
SharedHeap<T>::~SharedHeap() {
	if (root) delete root; 	// warning: delete all sub-nodes
	root = NULL;
}

template<class T>
inline double SharedHeap<T>::minimum() const {
	return root->elt->crit[heap_id];
}

template<class T>
unsigned int SharedHeap<T>::size() const {
	return nb_nodes;
}

template<class T>
bool SharedHeap<T>::empty() const {
	return (nb_nodes==0);
}

template<class T>
T* SharedHeap<T>::top() const {
	return root->elt->data;
}


template<class T>
void SharedHeap<T>::sort(bool update_cost) {
	if (nb_nodes==0) return;

	SharedHeap<T>* heap_tmp = new SharedHeap<T>(costf, heap_id);

	// recursive sort : o(n*log(n))
	sort_rec(root, *heap_tmp, update_cost);

	root = heap_tmp->root;
	nb_nodes = heap_tmp->size();

	heap_tmp->root = NULL;
	delete heap_tmp; 	// warning: delete all sub-nodes

}

template<class T>
void SharedHeap<T>::sort_rec(HeapNode<T> * node, SharedHeap<T> & heap, bool update_cost) {

	if (update_cost)
		node->elt->crit[heap_id] = cost(*(node->elt->data));

	heap.push_elt(node->elt);
	if (node->left)	 sort_rec(node->left, heap, update_cost);
	if (node->right) sort_rec(node->right, heap, update_cost);

	node->left=NULL;
	node->right=NULL;
	node->elt = NULL;
	delete node;
	node = NULL;
}

template<class T>
void SharedHeap<T>::push_elt(HeapElt<T>* elt) {

	if (nb_nodes==0) {
		root = new HeapNode<T>(elt,NULL);
		root->elt->holder[heap_id] = root;
		nb_nodes++;
	} else {
		nb_nodes++;
		// calculate height
		int height = 0;
		int aux = nb_nodes;
		while(aux>1) { aux /= 2; height++;	}

		// pt = pointer on the current element
		HeapNode<T> * pt = root;
		for (int pos=height-1; pos>0; pos--) {
			if ( nb_nodes & (1 << pos)) {  // test the "pos"th bit of the integer nb_nodes
				pt = pt->right;
			} else {
				pt = pt->left;
			}
		}
		HeapNode<T>* tmp= new HeapNode<T>(elt,pt);
		tmp->elt->holder[heap_id] = tmp;
		if (nb_nodes%2==0)	{ pt->left =tmp; }
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

template<class T>
HeapNode<T> * SharedHeap<T>::get_node(unsigned int i) const {
	assert(i<nb_nodes);
	assert(nb_nodes>0);

	// GET THE LAST ELEMENT and delete the associated node
	// calculate height
	int height = 0;
	unsigned int aux = i+1;
	while(aux>1) { aux /= 2; height++; }
	//std::cout << "height of "<<i<<"  = " << height <<std::endl;
	// pt = pointer on the current element
	HeapNode<T>* pt = root;
	for (int pos=height-1; pos>=0; pos--) {
		if ( (i+1) & (1 << pos)) {  // test the "pos"th bit of the integer nb_nodes
			//std::cout << "Right ";
			pt = pt->right;
		} else {
			//std::cout << "Left ";
			pt = pt->left;
		}
	}
	return pt;
}

template<class T>
HeapElt<T>* SharedHeap<T>::pop_elt() {
	assert(nb_nodes>0);
	HeapElt<T>* c_return = root->elt;
	erase_node(root);
	return c_return;
}

// erase a node and update the order
template<class T>
void SharedHeap<T>::erase_node(HeapNode<T>* node) {
	assert(node);
	assert(nb_nodes>0);

	if (erase_node_no_percolate(node)) percolate(node);
}

// erase a node and update the order
template<class T>
HeapNode<T>* SharedHeap<T>::erase_node_no_percolate(HeapNode<T>* pt_root) {
	assert(nb_nodes>0);

	if (nb_nodes==1) {
		assert(pt_root==root);
		root->elt=NULL;
		delete root;
		root = NULL;
		pt_root = NULL; // return NULL
	} else {
		// GET THE LAST ELEMENT and delete the associated node without destroying the HeapElt
		HeapNode<T>* last = get_node(nb_nodes-1);

		// elt points on the last node data, that we will put in place of the deleted node
		HeapElt<T>* elt = last->elt;

		if (nb_nodes%2==0)	{ last->father->left =NULL; }
		else 				{ last->father->right=NULL; }

		// to avoid the element to be deleted with the node pt
		last->elt = NULL;

		if (pt_root!=last) { // if the node to be deleted is not the last
			pt_root->elt = elt;
			pt_root->elt->holder[heap_id] = pt_root;
		} else {
			pt_root = NULL; // return NULL
		}
		delete last;
	}
	nb_nodes--;
	return pt_root;
}

template<class T>
void SharedHeap<T>::percolate(HeapNode<T>* node) {
	assert(node);

	// PERMUTATION to maintain the order in the heap when going down
	if (node) {
		bool b=true;
		while (b && (node->left)) {
			if (node->right) {
				if (node->is_sup(node->left,heap_id)) {
					if (node->right->is_sup(node->left,heap_id)) {
						// left is the smallest
						node->switch_elt(node->left,heap_id);
						node = node->left;  // next
					} else {
						// right is the smallest
						node->switch_elt(node->right,heap_id);
						node = node->right;  // next
					}
				} else {
					if (node->is_sup(node->right,heap_id)) {
						// right is the smallest
						node->switch_elt(node->right,heap_id);
						node = node->right;  // next
					} else { // current node is the smallest among the 3: stop
						b=false;
					}
				}
			} else { // no more right child but there is a left child
				if (node->is_sup(node->left,heap_id)) {
					// left is the smallest
					node->switch_elt(node->left,heap_id);
					node = node->left;  // next
				} else { // current node is the smallest among the 3: stop (and we must have reached the bottom of the heap)
					b=false;
				}
			}
		}
	}
}

template<class T>
HeapNode<T>::HeapNode(HeapElt<T>* elt, HeapNode<T>* father): elt(elt), right(NULL), left(NULL), father(father) { }

//template<class T>
//HeapNode<T>::~HeapNode() {
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
//}

template<class T>
bool HeapNode<T>::is_sup(HeapNode<T>* node, int heap_id) const {
	return elt->is_sup(node->elt->crit[heap_id],heap_id);
}

template<class T>
bool HeapNode<T>::is_sup(double d, int heap_id) const {
	return elt->is_sup(d,heap_id);
}

template<class T>
void HeapNode<T>::switch_elt(HeapNode<T>* node, int heap_id) {

	elt->holder[heap_id] = node;
	node->elt->holder[heap_id] = this;

	HeapElt<T> * elt_tmp = elt;
	elt = node->elt;
	node->elt = elt_tmp;

}

//////////////////////////////////////////////////////////////////////////////////////
/** create an node with a data and its criterion */
//CellHeapElt::CellHeapElt(int nb_crit,T* data, double *crit_in) : data(data), crit(new double[nb_crit]), indice(new unsigned int[nb_crit]){
//	for (int i=0;i<nb_crit;i++) {
//		crit[i] = crit_in[i];
//		indice[i] = 0;
//	}
//}

template<class T>
HeapElt<T>::HeapElt(T* data, double crit_1) : data(data), /*nb_heaps(1),*/ crit(new double[1]), holder(new HeapNode<T>*[1]){
	crit[0] = crit_1;
	holder[0] = NULL;
}

template<class T>
HeapElt<T>::HeapElt(T* data, double crit_1, double crit_2) : data(data), /*nb_heaps(2),*/ crit(new double[2]), holder(new HeapNode<T>*[2]){
	crit[0] = crit_1;
	crit[1] = crit_2;
	holder[0] = NULL;
	holder[1] = NULL;
}

template<class T>
HeapElt<T>::~HeapElt() {
	if (data) 	delete data;
	delete [] crit;
	delete [] holder;
}

template<class T>
bool HeapElt<T>::is_sup(double d, int ind_crit) const {
	return (crit[ind_crit] >= d);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const HeapElt<T>& elt) {
	os << "n°" << elt.indice[0]<< *(elt.data);
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

#endif // __IBEX_SHARED_HEAP_H__
