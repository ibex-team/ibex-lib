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
#include <stack>
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

	/**
	 * \brief Create a shared heap.
	 *
	 * \param cost                     - the cost function for each element
	 * \param update_cost_when_sorting - whether the cost is recalculated or not, when the heap is sorted
	 * \param id                       - the identifier of this heap.
	 */
	SharedHeap(CostFunc<T>& cost, bool update_cost_when_sorting, int id);

	/** \brief Delete this. */
	virtual  ~SharedHeap();

	/** \brief Return the size of the buffer. */
	unsigned int size() const;

	/** \brief Return true if the buffer is empty. */
	bool empty() const;

	/**
	 * \brief Return the next box (but does not pop it).
	 *
	 * Complexity: o(1)
	 */
	T* top() const;

	/**
	 * \brief Return the minimum (the criterion for
	 *        the first element)
	 */
	double minimum() const;

	/**
	 * \brief update the cost and sort all the heap
	 * complexity: o(nb_nodes*log(nb_nodes))
	 *
	 * \param update_cost: if true, costs are all recalculated
	 */
	void sort();

	/**
	 * \brief Count the number of nodes pushed since
	 *         the object is created. */
	unsigned int nb_nodes;

	/**
	 * \brief Cost function associated to this heap
	 */
	CostFunc<T>& costf;

	/**  \brief Identifier of this heap */
	const int heap_id;

protected:

	friend class DoubleHeap<T>;

	/** The "cost" of an element. */
	double cost(const T& data) const;

	/** The root of the heap */
	HeapNode<T>* root;

	/** Whether the cost function is called again inside sort. */
	bool update_cost_when_sorting;

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
	void percolate_down(HeapNode<T>* node);

	/**
	 * Percolate (or "heapify") from the node \var node upto the root.
	 */
	void percolate_up(HeapNode<T>* node);

	/**
	 * \brief Remove a node and update the heap in consequence.
	 *
	 * (call percolate_down and percolate_up).
	 */
	void erase_node(HeapNode<T>* node);

	/**
	 * \brief Remove a node.
	 *
	 * The last node is actually removed and its element is put in place
	 * of the node element, except if the node is the last.
	 *
	 * The heap is not updated after (the new node is not at its right place
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
	 *
	 * Shows the tree-structure with only the cost of each node (not all data).
	 */
	template<class U>
	friend std::ostream& operator<<(std::ostream& os, const SharedHeap<U>& heap);

	/**
	 * \brief Check if the heap is well-formed
	 */
	bool heap_state();

private:

	/** Used in the sort function (proceed by recursivity) */
	void sort_rec(HeapNode<T>* node, SharedHeap<T>& heap);
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
	friend std::ostream& operator<<(std::ostream& os, const HeapNode<U>& node);

	template<class U>
	friend std::ostream& operator<<(std::ostream& os, const SharedHeap<U>& heap);

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

	template<class U>
	friend std::ostream& operator<<(std::ostream& os, const SharedHeap<U>& heap);
};



/*================================== inline implementations ========================================*/


template<class T>
SharedHeap<T>::SharedHeap(CostFunc<T>& cost, bool update_cost, int id) : nb_nodes(0), costf(cost), heap_id(id), root(NULL), update_cost_when_sorting(update_cost) {

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
void SharedHeap<T>::sort() {
	if (nb_nodes==0) return;

	SharedHeap<T>* heap_tmp = new SharedHeap<T>(costf, update_cost_when_sorting, heap_id);

	// recursive sort : o(n*log(n))
	sort_rec(root, *heap_tmp);

	root = heap_tmp->root;
	nb_nodes = heap_tmp->size();

	heap_tmp->root = NULL;
	delete heap_tmp; 	// warning: delete all sub-nodes

}

template<class T>
inline double SharedHeap<T>::cost(const T& data) const {
	return costf.cost(data);
}

template<class T>
void SharedHeap<T>::sort_rec(HeapNode<T>* node, SharedHeap<T>& heap) {

	if (update_cost_when_sorting)
		node->elt->crit[heap_id] = cost(*(node->elt->data));

	heap.push_elt(node->elt);
	if (node->left)	 sort_rec(node->left, heap);
	if (node->right) sort_rec(node->right, heap);

	delete node;
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

		percolate_up(tmp);
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

	if (erase_node_no_percolate(node)) {
		percolate_down(node);
		percolate_up(node);
	}
}

// erase a node and update the order
template<class T>
HeapNode<T>* SharedHeap<T>::erase_node_no_percolate(HeapNode<T>* node) {
	assert(nb_nodes>0);

	if (nb_nodes==1) {
		assert(node==root);
		root->elt=NULL;
		delete root;
		root = NULL;
		node = NULL; // return NULL
	} else {
		// Get the last element and delete the associated node without destroying the HeapElt
		HeapNode<T>* last = get_node(nb_nodes-1);

		// elt points on the last node data, that we will put in place of the deleted node
		HeapElt<T>* elt = last->elt;

		if (nb_nodes%2==0)	{ last->father->left =NULL; }
		else 				{ last->father->right=NULL; }

		// to avoid the element to be deleted with the node pt
		last->elt = NULL;

		if (node!=last) { // if the node to be deleted is not the last
			node->elt = elt;
			node->elt->holder[heap_id] = node;
		} else {
			node = NULL; // return NULL
		}
		delete last;
	}
	nb_nodes--;
	return node;
}

template<class T>
void SharedHeap<T>::percolate_up(HeapNode<T>* node) {
	assert(node);

	while (node->father && node->father->is_sup(node,heap_id)) {
		node->switch_elt(node->father,heap_id);
		node = node->father;
	}
}

template<class T>
void SharedHeap<T>::percolate_down(HeapNode<T>* node) {
	assert(node);

	// PERMUTATION to maintain the order in the heap when going down
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

template<class T>
bool SharedHeap<T>::heap_state() {

	if (empty()) return true;

	std::stack<HeapNode<T>* > s;
	s.push(root);
	while (!s.empty()) {
		HeapNode<T>* node=s.top();
		s.pop();
		if (node->right && (!node->left)) {
			//std::cerr << "node with only right child" << std::endl;
			return false;
		}
		if (node->left) {
			if (node->is_sup(node->left,heap_id)) {
				//std::cerr << "node elt:" << node->elt->crit[heap_id] << " node left:" <<  node->left->elt->crit[heap_id] << std::endl;
				return false;
			}
			else s.push(node->left);
		}
		if (node->right) {
			if (node->is_sup(node->right,heap_id)) {
				//std::cerr << "node elt:" << node->elt->crit[heap_id] << " node left:" <<  node->right->elt->crit[heap_id] << std::endl;
				return false;
			}
			else s.push(node->right);
		}
	}
	return true;
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
	return (crit[ind_crit] > d);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const HeapElt<T>& elt) {
	os << "{ crit[0]= " << (elt.crit[0]) << ": "<< *(elt.data)<<" } ";
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
std::ostream& operator<<(std::ostream& os, const SharedHeap<T>& heap) {
	if (!heap.root) return os << "(empty heap)";
	os << std::endl;
	std::stack<std::pair<HeapNode<T>*,int> > s;
	s.push(std::pair<HeapNode<T>*,int>(heap.root,0));
	while (!s.empty()) {
		std::pair<HeapNode<T>*,int> p=s.top();
		s.pop();
		for (int i=0; i<p.second; i++) os << "   ";
		os  << (p.first->elt->crit[heap.heap_id]) << std::endl;
		if (p.first->right) s.push(std::pair<HeapNode<T>*,int>(p.first->right,p.second+1));
		if (p.first->left) s.push(std::pair<HeapNode<T>*,int>(p.first->left,p.second+1));
	}
	return os;
}

} // end namespace ibex

#endif // __IBEX_SHARED_HEAP_H__
