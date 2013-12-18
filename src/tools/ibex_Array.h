/* ============================================================================
 * I B E X - Array of references
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 22, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_ARRAY_H__
#define __IBEX_ARRAY_H__

#include <vector>
#include <cassert>
#include <stdlib.h>

namespace ibex {

/**
 * \defgroup tools Tools
 */

/**
 * \ingroup tools
 * \brief Array of references.
 *
 */
template<class T>
class Array {
public:
	/**
	 * \brief Create an initialized array
	 *
	 * \warning You must use #resize() and then initialize the references with set().
	 */
	Array();

	/**
	 * \brief Create an array of uninitialized references.
	 *
	 * \warning You must initialize the references with set().
	 */
	Array(int n);

	/**
	 * \brief Resize the array.
	 */
	void resize(int n);

	/**
	 * \brief Increments the size of the array by 1 and add the new element.
	 */
	void add(T& obj);

	/**
	 * \brief Set the ith reference to the object \a obj.
	 */
	void set_ref(int i, T& obj);

	/**
	 * \brief Create an array of references from an array of pointers.
	 */
	Array(T** array, int n);

	/**
	 * \brief Create an array of references from a vector of pointers.
	 *
	 * \pre The vector must be nonempty.
	 */
	Array(const std::vector<T*>&);

	/**
	 * \brief Create an array with a single reference.
	 */
	Array(T& x);

	/**
	 * \brief Create an array with 2 references.
	 */
	Array(T& x1, T& x2);

	/**
	 * \brief Create an array with 3 references.
	 */
	Array(T& x1, T& x2, T& x3);


	/**
	 * \brief Create an array with 4 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4);


	/**
	 * \brief Create an array with 5 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5);


	/**
	 * \brief Create an array with 6 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6);

	/**
	 * \brief Duplicate this instance.
	 *
	 */
	Array(const Array&);

	/**
	 * \brief Delete this instance.
	 */
	~Array();

	/**
	 * \brief Return true if empty.
	 */
	bool is_empty() const;

	/**
	 * \brief Return the number of objects.
	 */
	int size() const;

	/**
	 * \brief Return the ith reference (as const).
	 */
	T& operator[](int i);


	/**
	 * \brief Return the ith reference (as const).
	 */
	T& operator[](int i) const;

	/**
	 * \brief Remove all the references (the size
	 * is unchanged).
	 */
	void clear();

protected:
	int _nb;

	/** Array of sub-Ts */
	T** array;
private:
	T& operator=(const Array<T>&); //forbidden
};


/*================================== inline implementations ========================================*/

template<class T>
Array<T>::Array() : _nb(0), array(NULL) {

}

template<class T>
Array<T>::Array(int n) : _nb(n), array(new T*[n]) {
	assert(n>=0);
	for (int i=0; i<_nb; i++) {
		array[i] = NULL;
	}
}

template<class T>
void Array<T>::resize(int n) {
	assert(n>=0);
	T** new_array=new T*[n];
	int i=0;
	for (; i<_nb; i++) {
		if (i<n) new_array[i] = array[i];
		else if (array[i]) delete array[i];
	}
	for (; i<n; i++) {
		new_array[i]=NULL;
	}
	if (array) delete[] array;
	array=new_array;
	_nb=n;
}

template<class T>
void Array<T>::set_ref(int i, T& obj) {
	assert(i>=0 && i<_nb);
	assert(!array[i]);

	array[i]=&obj;
}

template<class T>
void Array<T>::add(T& obj) {
	resize(size()+1);
	set_ref(size()-1,obj);
}

template<class T>
Array<T>::Array(T** a, int n) : _nb(n), array(new T*[n]) {
	assert(n>=0);
	for (int i=0; i<_nb; i++) {
		array[i] = a[i];
	}
}

template<class T>
Array<T>::Array(const std::vector<T*>& vec) : _nb(vec.size()), array(new T*[vec.size()]) {

	assert(vec.size()>0);
	int i=0;
	for (typename std::vector<T*>::const_iterator it=vec.begin(); it!=vec.end(); it++) {
		array[i++] = *it;
	}
}

template<class T>
Array<T>::Array(T& x) : _nb(1), array(new T*[1]) {
	array[0] = &x;
}

template<class T>
Array<T>::Array(T& x1, T& x2) : _nb(2), array(new T*[2]) {
	array[0] = &x1;
	array[1] = &x2;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3) : _nb(3), array(new T*[3]) {
	array[0] = &x1;
	array[1] = &x2;
	array[2] = &x3;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4) : _nb(4), array(new T*[4]) {
	array[0] = &x1;
	array[1] = &x2;
	array[2] = &x3;
	array[3] = &x4;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5) : _nb(5), array(new T*[5]) {
	array[0] = &x1;
	array[1] = &x2;
	array[2] = &x3;
	array[3] = &x4;
	array[4] = &x5;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6) : _nb(6), array(new T*[6]) {
	array[0] = &x1;
	array[1] = &x2;
	array[2] = &x3;
	array[3] = &x4;
	array[4] = &x5;
	array[5] = &x6;
}

template<class T>
Array<T>::Array(const Array<T>& a) : _nb(a.size()), array(new T*[a.size()]) {
	for (int i=0; i<_nb; i++) {
		array[i] = &a[i];
	}
}

template<class T>
Array<T>::~Array() {
	delete[] array;
}

template<class T>
bool Array<T>::is_empty() const {
	return _nb==0;
}

template<class T>
int Array<T>::size() const {
	return _nb;
}

template<class T>
T& Array<T>::operator[](int i) {
	assert(i>=0 && i<_nb);
	assert(array[i]);

	return *array[i];
}

template<class T>
T& Array<T>::operator[](int i) const {
	assert(i>=0 && i<_nb);
	assert(array[i]);

	return *array[i];
}

template<class T>
void Array<T>::clear() {
	for (int i=0; i<_nb; i++) {
		array[i] = NULL;
	}
}
} // namespace ibex
#endif // __IBEX_ARRAY_H__
