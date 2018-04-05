//============================================================================
//                                  I B E X                                   
// File        : ibex_Heap.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 23, 2014
// Last Update : Dec 25, 2017
//============================================================================

#ifndef __IBEX_HEAP_H__
#define __IBEX_HEAP_H__

#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>

namespace ibex {

/** \ingroup strategy
 *
 * \brief Cost function (for Heap/DoubleHeap)
 *
 * Allows to give a cost to data of a given type T.
 *
 */
template<class T>
class CostFunc {
public:

	virtual ~CostFunc() { }

	/**
	 * \brief Return the cost associated to "data"
	 */
	virtual double cost(const T& data) const=0;
};


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
template<class T>
class Heap  {
public:

	/** Create an empty heap. */
	Heap(CostFunc<T>& costf);

	/**
	 * Deep copy of another heap.
	 *
	 * Precondition: class T has a copy constructor.
	 */
	Heap(const Heap& heap);

	/** Delete the heap. */
	~Heap() { flush(); }

	/**
	 * \brief Flush the heap.
	 *
	 * All the remaining elements will be *deleted*
	 * (with a call to the destructor of class T).
	 */
	void flush();

	/** Return the size of the heap. */
	int size() const;

	/** Return true if the heap is empty. */
	bool empty() const;

	/** push a new element on the stack. */
	void push(T* el);

	/** Pop a element from the stack and return it.*/
	T* pop();

	/** Return the next box (but does not pop it).*/
	T* top() const;

	/**
	 * \brief Contracts the heap.
	 *
	 * Removes (and deletes) from the heap all the elements
	 * with a cost greater than \a lb.
	 */
	void contract(double lb);

	/** Return the minimum (the criterion for
	 * the first element) */
	double minimum() const;

	/** The cost function */
	CostFunc<T>& costf;

protected:
	/** The "cost" of a element. */
	double cost(const T& data) const;

	/** The way to compare two pairs (element,crit). */
	bool operator()(const std::pair<T*,double>& c1, const std::pair<T*,double>& c2) const;

	// elements and associated "costs"
	std::vector<std::pair<T*,double> > l;

	template<class U>
	friend std::ostream& operator<<(std::ostream&, const Heap<U>&);
};

/** Display the heap */
template<class T>
std::ostream& operator<<(std::ostream&, const Heap<T>&);


/*================================== inline implementations ========================================*/


/* we need this dummy class just because
 * the xxx_heap functions take the comparator
 * argument by copy (hence, we cannot give
 * "*this" since the class is abstract) */
template<class T>
struct HeapComparator {
	bool operator()(const std::pair<T*,double>& c1, const std::pair<T*,double>& c2) {
		return c1.second >= c2.second;
	}
};

template<class T>
Heap<T>::Heap(CostFunc<T>& costf) : costf(costf) {

}

template<class T>
Heap<T>::Heap(const Heap& heap) : costf(heap.costf) {
	for(typename std::vector<std::pair<T*,double> >::const_iterator it=heap.l.begin(); it!=heap.l.end(); it++) {
		l.push_back(make_pair(new T(*(it->first)), it->second));
	}
}

template<class T>
bool Heap<T>::operator()(const std::pair<T*,double>& c1, const std::pair<T*,double>& c2) const {
	return c1.second >= c2.second;
}

template<class T>
void Heap<T>::flush() {
	for (typename std::vector<std::pair<T*,double> >::iterator it=l.begin(); it!=l.end(); it++)
		delete it->first;

	l.clear();
}

template<class T>
int Heap<T>::size() const {
	return l.size();
}

// E.g.: called by manage_buffer in Optimizer in case of a new upper bound
// on the objective ("loup"). This function then removes (and deletes) from
// the heap all the elements with a cost greater than loup.
template<class T>
void Heap<T>::contract(double loup) {
	//cout << " before contract heap  " << l.size() << endl;

	sort_heap(l.begin(),l.end(),HeapComparator<T>());
	typename std::vector<std::pair<T*,double> >::iterator it0=l.begin();

	int k=0;
	while (it0!=l.end() && it0->second > loup) { it0++; k++; }

	for (int i=0;i<k;i++) {
		delete l[i].first;
	}

	if (k>0) l.erase(l.begin(),it0);

	make_heap(l.begin(), l.end() ,HeapComparator<T>());

	//cout << " after contract heap " << l.size() << endl;

}

template<class T>
bool Heap<T>::empty() const {
	return l.empty();
}

template<class T>
void Heap<T>::push(T* el) {
	l.push_back(std::pair<T*,double>(el,cost(*el)));
	push_heap(l.begin(), l.end(), HeapComparator<T>());
}

template<class T>
T* Heap<T>::pop() {
	T* c = l.front().first;
	pop_heap(l.begin(),l.end(), HeapComparator<T>()); // put the "best" at the end
	l.pop_back(); // removes the "best"
	return c;     // and return it
}

template<class T>
T* Heap<T>::top() const {
	return l.front().first;
}

template<class T>
double Heap<T>::minimum() const {
	return l.begin()->second;
}

template<class T>
double Heap<T>::cost(const T& data) const {
	return costf.cost(data);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Heap<T>& heap) {
	os << "[ ";
	for (typename std::vector<std::pair<T*,double> >::const_iterator it=heap.l.begin(); it!=heap.l.end(); it++)
		os << (*it->first) << " ";
	return os << "]";
}

} // end namespace ibex
#endif // __IBEX_HEAP_H__
