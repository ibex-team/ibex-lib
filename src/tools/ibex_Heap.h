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

template<class T>
class Heap  {

public:

	/** create this with a specific indice, and if you need to update the cost a each contract of sort. */
	Heap(int ind_crit= 0, bool updateCost=false);

	/** Delete this. */
	virtual  ~Heap();

	/**
	 * \brief Flush the buffer.
	 *
	 * All the remaining elements will be *deleted*
	 * (with a call to the destructor of class T).
	 */
	virtual void flush();

	/** Return the size of the buffer. */
	virtual unsigned int size() const;

	/** Return true if the buffer is empty. */
	virtual bool empty() const;

	/** push a new element on the stack.
	 * Complexity: o(log(nb_cells))
	 */
	virtual void push(T* el);

	/** Pop a element from the stack and return it.
	 * Complexity: o(log(nb_cells))
	 */
	virtual T* pop();

	/** Return the next box (but does not pop it).
	 * Complexity: o(1)
	 */
	virtual T* top() const;

	/**
	 * \brief Contracts the heap.
	 *
	 * Removes (and deletes) from the heap all the elements
	 * with a cost greater than \a lb.
	 */
	virtual void contractHeap(double lb);

	/** Return the minimum (the criterion for
	 * the first element) */
	inline double minimum() const{	return root->elt->crit[heap_id];	}

	/**
	 * \brief Access to the ith Cell rank by largest-first order
	 * Complexity: o(log(nb_cells))
	 */
	T* get(unsigned int i) const;

	/**
	 * \brief update the cost and sort all the heap
	 * complexity: o(nb_cells*log(nb_cells))
	 */
	void sort();

	/** Count the number of cells pushed since
	 * the object is created. */
	unsigned int nb_cells;

//	inline bool needUpdate() const {return updateCost;}

	inline int getId() const {return heap_id;}


	virtual std::ostream& print(std::ostream& os) const;

protected:
	friend class CellDoubleHeap;

	/** The "cost" of a element. */
	virtual double cost(const T&) const=0;


	virtual Heap<T>* init_copy() const =0;

	/** A boolean which indicate if all the cost must be update at each contract and sort
	 *
	 */
	const bool updateCost;


	/** Index of the criterion selected for this heap */
	const int heap_id;

	/** The root of the heap */
	HeapNode<T>* root;

	/**
	 * Pop a CellHeapElt from the stack and return it.
	 * Complexity: o(log(nb_cells))
	 */
	virtual HeapElt<T>* popElt();

	/**
	 * Useful only for CellDoubleHeap
	 * Complexity: o(log(nb_cells))
	 */
	virtual void push(HeapElt<T>* elt);

	/** Update the heap to reorder the elements from the node \var node to the down */
	virtual void updateOrder(HeapNode<T>* node);

	/** Erase only this HeapNope without touch the element */
	virtual void eraseNode(unsigned int i);

	/** Remove the last node and put its element at the ith position */
	virtual HeapNode<T>* eraseNode_noUpdate(unsigned int i);

	/**
	 * Access to the ith node rank by largest-first order
	 *
	 * \param i - the cell number
	 */
	HeapNode<T>* getNode(unsigned int i) const;

private:
	/** Used in the contractHeap function (proceed by recursivity) */
	void contractRec(double new_loup, HeapNode<T>* node, Heap<T>& heap);

	/** Used in the sort function (proceed by recursivity) */
	void sortRec(HeapNode<T>* node, Heap<T>& heap);

};



/**
 * \ingroup strategy
 *
 * \brief Heap node
 *
 */
template<class T>
class HeapNode {

private:
	friend class Heap<T>;
	friend class CellDoubleHeap;

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
	bool isSup(HeapNode<T>* node, int ind_crit) const;
	bool isSup(double d, int ind_crit) const ;

	/** Switch the CellHeapElt between *this and node */
	void switchElt(HeapNode<T>* node, int ind_crit);

	template<class U>
	friend std::ostream& operator<<(std::ostream& os, const HeapNode<U>& node) ;

};





template<class T>
class HeapElt {

private:
	friend class HeapNode<T>;
	friend class Heap<T>;
	friend class CellDoubleHeap;

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

	/** the criteria of the stored cell (one for each heap it
	 * belongs to). */
	double *crit;

	/** The index of this element in each heap it belongs to. */
	unsigned int *indice;

	template<class U>
	friend std::ostream& operator<<(std::ostream& os, const HeapElt<U>& node) ;
};





/*================================== inline implementations ========================================*/




template<class T>
Heap<T>::Heap(int ind_crit,bool updateCost) : heap_id(ind_crit), updateCost(updateCost), nb_cells(0), root(NULL) {

}
template<class T>
Heap<T>::~Heap() {
	if (root) delete root; 	// warning: delete all sub-nodes
	root = NULL;
}

template<class T>
void Heap<T>::flush() {
	if (root) delete root; 	// warning: delete all sub-nodes
	nb_cells =0;
	root = NULL;
}

template<class T>
unsigned int Heap<T>::size() const {
	return nb_cells;
}

template<class T>
bool Heap<T>::empty() const {
	return (nb_cells==0);
}

template<class T>
T* Heap<T>::top() const {
	return root->elt->cell;
}



// E.g.: called by manage_buffer in Optimizer in case of a new upper bound
// on the objective ("loup"). This function then removes (and deletes) from
// the heap all the cells with a cost greater than loup.
template<class T>
void Heap<T>::contractHeap(double new_loup) {
	if (nb_cells==0) return;

	Heap<T> * heap_tmp = init_copy();

	contractRec(new_loup, root, *heap_tmp);

	root = heap_tmp->root;
	nb_cells = heap_tmp->size();
	heap_tmp->root = NULL;
	delete heap_tmp;

}


template<class T>
void Heap<T>::contractRec(double new_loup, HeapNode<T>* node, Heap<T>& heap) {

	if(updateCost)
		node->elt->crit[heap_id] = cost(*(node->elt->cell));

	if (!(node->isSup(new_loup, heap_id))) {
		heap.push(node->elt);
		if (node->left)	 contractRec(new_loup, node->left, heap);
		if (node->right) contractRec(new_loup, node->right, heap);
		node->left=NULL;
		node->right=NULL;
		node->elt=NULL;
	}
	delete node;
	node= NULL;
}

template<class T>
void Heap<T>::sort() {
	if (nb_cells==0) return;

	Heap<T>* heap_tmp = init_copy();

	// recursive sort : o(n*log(n))
	sortRec(root, *heap_tmp);

	root = heap_tmp->root;
	nb_cells = heap_tmp->size();

	heap_tmp->root = NULL;
	delete heap_tmp; 	// warning: delete all sub-nodes

}

template<class T>
void Heap<T>::sortRec(HeapNode<T> * node, Heap<T> & heap) {

	if (updateCost)
		node->elt->crit[heap_id] = cost(*(node->elt->cell));

	heap.push(node->elt);
	if (node->left)	 sortRec(node->left, heap);
	if (node->right) sortRec(node->right, heap);

	node->left=NULL;
	node->right=NULL;
	node->elt = NULL;
	delete node;	// warning: delete all sub-nodes
	node = NULL;
}


template<class T>
void Heap<T>::push(T* cell) {
	push(new HeapElt<T>(cell,cost(*cell)));
}



template<class T>
void Heap<T>::push(HeapElt<T>* cell) {

	if (nb_cells==0) {
		root = new HeapNode<T>(cell,NULL);
		root->elt->indice[heap_id] = 0;
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
		HeapNode<T>* tmp= new HeapNode<T>(cell,pt);
		tmp->elt->indice[heap_id] = nb_cells-1;
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
HeapNode<T> * Heap<T>::getNode(unsigned int i) const {
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

template<class T>
T* Heap<T>::get(unsigned int i ) const {
	assert(i<nb_cells);
	assert(nb_cells>0);
	return getNode(i)->elt->cell;
}

template<class T>
T* Heap<T>::pop() {
	assert(nb_cells>0);
	HeapElt<T>* tmp =popElt();
	T * c = tmp->cell;
	tmp->cell = NULL;
	delete tmp;
	tmp=NULL;
	return c;
}

template<class T>
HeapElt<T>* Heap<T>::popElt() {
	assert(nb_cells>0);
	HeapElt<T>* c_return = root->elt;
	eraseNode(0);
	return c_return;
}

// erase a node and update the order
template<class T>
void Heap<T>::eraseNode(unsigned int i) {
	assert(i<nb_cells);
	assert(nb_cells>0);

	updateOrder(eraseNode_noUpdate(i));
}

// erase a node and update the order
template<class T>
HeapNode<T> * Heap<T>::eraseNode_noUpdate(unsigned int i) {
	assert(i<nb_cells);
	assert(nb_cells>0);

	HeapNode<T>* pt_root=NULL;

	if (nb_cells==1) {
		root->elt=NULL;
		delete root;
		root = NULL;
	} else if (i==(nb_cells-1)) {
		// GET THE LAST ELEMENT and delete the associated node without destroying the HeapElt
		HeapNode<T>* pt= getNode(nb_cells-1);
		if (nb_cells%2==0)	{ pt->father->left =NULL; }
		else 				{ pt->father->right=NULL; }
		pt->elt = NULL;
		delete pt;
		pt = NULL;
	} else {
		// GET THE LAST ELEMENT and delete the associated node without destroying the HeapElt
		HeapNode<T> * pt= getNode(nb_cells-1);
		pt_root= getNode(i);
		HeapElt<T>* cell = pt->elt;
//std::cout << cell->indice[ind_crit] << std::endl;
		if (nb_cells%2==0)	{ pt->father->left =NULL; }
		else 				{ pt->father->right=NULL; }
		pt->elt = NULL;
		delete pt;
		pt = NULL;
		// cell points on the element we put in place of the deleted node
		pt_root= getNode(i);
		pt_root->elt = cell;
		pt_root->elt->indice[heap_id] = i;
	}
	nb_cells--;
	return pt_root;
}



template<class T>
void Heap<T>::updateOrder(HeapNode<T> *pt) {
	// PERMUTATION to maintain the order in the heap when going down
	if (pt) {
		bool b=true;
		while (b && (pt->left)) {
			if (pt->right) {
				if (pt->isSup(pt->left,heap_id)) {
					if (pt->right->isSup(pt->left,heap_id)) {
						// left is the smallest
						pt->switchElt(pt->left,heap_id);
						pt = pt->left;  // next
					} else {
						// right is the smallest
						pt->switchElt(pt->right,heap_id);
						pt = pt->right;  // next
					}
				} else {
					if (pt->isSup(pt->right,heap_id)) {
						// right is the smallest
						pt->switchElt(pt->right,heap_id);
						pt = pt->right;  // next
					} else { // current node is the smallest among the 3: stop
						b=false;
					}
				}
			} else { // no more right child but there is a left child
				if (pt->isSup(pt->left,heap_id)) {
					// left is the smallest
					pt->switchElt(pt->left,heap_id);
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

template<class T>
HeapNode<T>::HeapNode(HeapElt<T>* elt, HeapNode<T> * father): elt(elt), right(NULL), left(NULL), father(father) { }

template<class T>
HeapNode<T>::~HeapNode() {
	// warning: delete all sub-nodes
	if (elt) 	delete elt;
	if (right) 	delete right;
	if (left)  	delete left;
}

template<class T>
bool HeapNode<T>::isSup(HeapNode<T> *n, int ind_crit) const {
	return elt->isSup(n->elt->crit[ind_crit],ind_crit);
}

template<class T>
bool HeapNode<T>::isSup(double d, int ind_crit) const {
	return elt->isSup(d,ind_crit);
}

template<class T>
void HeapNode<T>::switchElt(HeapNode<T> *pt, int ind_crit) {
	// not forget indices permutation
	unsigned int ind_tmp = elt->indice[ind_crit];
	elt->indice[ind_crit] = pt->elt->indice[ind_crit];
	pt->elt->indice[ind_crit] = ind_tmp;

	HeapElt<T> * elt_tmp = elt;
	elt = pt->elt;
	pt->elt = elt_tmp;

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
HeapElt<T>::HeapElt(T* cell, double crit_1) : cell(cell), crit(new double[1]), indice(new unsigned int[1]){
		crit[0] = crit_1;
		indice[0] = 0;
}
template<class T>
HeapElt<T>::HeapElt(T* cell, double crit_1, double crit_2) : cell(cell), crit(new double[2]), indice(new unsigned int[2]){
		crit[0] = crit_1;
		crit[1] = crit_2;
		indice[0] = 0;
		indice[1] = 0;
}


/** Delete the node and all its sons */
template<class T>
HeapElt<T>::~HeapElt() {
	if (cell) 	delete cell;
	delete [] crit;
	delete [] indice;
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
std::ostream& Heap<T>::print(std::ostream& os) const{
	os << "[ ";
	if (root) 	os << *(root) << " ";
	return os << "]";
}



} // end namespace ibex
#endif // __IBEX_HEAP_H__
