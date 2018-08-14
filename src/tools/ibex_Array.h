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
	 * \brief Concatenate with another array (increments the size)
	 */
	void add(const Array<T>& a);

	/**
	 * \brief Concatenate with another array (increments the size)
	 */
	void add(const std::vector<T*>& vec);

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
	 * \brief Create an array with 7 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7);

	/**
	 * \brief Create an array with 8 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8);

	/**
	 * \brief Create an array with 9 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9);

	/**
	 * \brief Create an array with 10 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10);

	/**
	 * \brief Create an array with 11 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11);

	/**
	 * \brief Create an array with 12 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12);

	/**
	 * \brief Create an array with 13 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13);

	/**
	 * \brief Create an array with 14 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14);

	/**
	 * \brief Create an array with 15 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15);

	/**
	 * \brief Create an array with 16 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15, T& x16);

	/**
	 * \brief Create an array with 17 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15, T& x16, T& x17);

	/**
	 * \brief Create an array with 18 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15, T& x16, T& x17, T& x18);

	/**
	 * \brief Create an array with 19 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15, T& x16, T& x17, T& x18, T& x19);

	/**
	 * \brief Create an array with 20 references.
	 */
	Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15, T& x16, T& x17, T& x18, T& x19, T& x20);

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
void Array<T>::add(const Array<T>& a) {
	int n=size();
	resize(n+a.size());

	for (int i=0; i<a.size(); i++)
		set_ref(n+i,a[i]);
}

template<class T>
void Array<T>::add(const std::vector<T*>& vec) {
	int n=size();
	resize(n+vec.size());

	int i=0;
	for (typename std::vector<T*>::const_iterator it=vec.begin(); it!=vec.end(); it++) {
		set_ref(n+i, **it);
		i++;
	}
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
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7) : _nb(7), array(new T*[7]) {
	array[0] = &x1;
	array[1] = &x2;
	array[2] = &x3;
	array[3] = &x4;
	array[4] = &x5;
	array[5] = &x6;
	array[6] = &x7;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8) : _nb(8), array(new T*[8]) {
	array[0] = &x1;
	array[1] = &x2;
	array[2] = &x3;
	array[3] = &x4;
	array[4] = &x5;
	array[5] = &x6;
	array[6] = &x7;
	array[7] = &x8;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9) : _nb(9), array(new T*[9]) {
	array[0] = &x1;
	array[1] = &x2;
	array[2] = &x3;
	array[3] = &x4;
	array[4] = &x5;
	array[5] = &x6;
	array[6] = &x7;
	array[7] = &x8;
	array[8] = &x9;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10) : _nb(10), array(new T*[10]) {
	array[0]  = &x1;
	array[1]  = &x2;
	array[2]  = &x3;
	array[3]  = &x4;
	array[4]  = &x5;
	array[5]  = &x6;
	array[6]  = &x7;
	array[7]  = &x8;
	array[8]  = &x9;
	array[9]  = &x10;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11) : _nb(11), array(new T*[11]) {
	array[0]  = &x1;
	array[1]  = &x2;
	array[2]  = &x3;
	array[3]  = &x4;
	array[4]  = &x5;
	array[5]  = &x6;
	array[6]  = &x7;
	array[7]  = &x8;
	array[8]  = &x9;
	array[9]  = &x10;
	array[10] = &x11;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12) : _nb(12), array(new T*[12]) {
	array[0]  = &x1;
	array[1]  = &x2;
	array[2]  = &x3;
	array[3]  = &x4;
	array[4]  = &x5;
	array[5]  = &x6;
	array[6]  = &x7;
	array[7]  = &x8;
	array[8]  = &x9;
	array[9]  = &x10;
	array[10] = &x11;
	array[11] = &x12;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13) : _nb(13), array(new T*[13]) {
	array[0]  = &x1;
	array[1]  = &x2;
	array[2]  = &x3;
	array[3]  = &x4;
	array[4]  = &x5;
	array[5]  = &x6;
	array[6]  = &x7;
	array[7]  = &x8;
	array[8]  = &x9;
	array[9]  = &x10;
	array[10] = &x11;
	array[11] = &x12;
	array[12] = &x13;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14) : _nb(14), array(new T*[14]) {
	array[0]  = &x1;
	array[1]  = &x2;
	array[2]  = &x3;
	array[3]  = &x4;
	array[4]  = &x5;
	array[5]  = &x6;
	array[6]  = &x7;
	array[7]  = &x8;
	array[8]  = &x9;
	array[9]  = &x10;
	array[10] = &x11;
	array[11] = &x12;
	array[12] = &x13;
	array[13] = &x14;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15) : _nb(15), array(new T*[15]) {
	array[0]  = &x1;
	array[1]  = &x2;
	array[2]  = &x3;
	array[3]  = &x4;
	array[4]  = &x5;
	array[5]  = &x6;
	array[6]  = &x7;
	array[7]  = &x8;
	array[8]  = &x9;
	array[9]  = &x10;
	array[10] = &x11;
	array[11] = &x12;
	array[12] = &x13;
	array[13] = &x14;
	array[14] = &x15;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15, T& x16) : _nb(16), array(new T*[16]) {
	array[0]  = &x1;
	array[1]  = &x2;
	array[2]  = &x3;
	array[3]  = &x4;
	array[4]  = &x5;
	array[5]  = &x6;
	array[6]  = &x7;
	array[7]  = &x8;
	array[8]  = &x9;
	array[9]  = &x10;
	array[10] = &x11;
	array[11] = &x12;
	array[12] = &x13;
	array[13] = &x14;
	array[14] = &x15;
	array[15] = &x16;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15, T& x16, T& x17) : _nb(17), array(new T*[17]) {
	array[0]  = &x1;
	array[1]  = &x2;
	array[2]  = &x3;
	array[3]  = &x4;
	array[4]  = &x5;
	array[5]  = &x6;
	array[6]  = &x7;
	array[7]  = &x8;
	array[8]  = &x9;
	array[9]  = &x10;
	array[10] = &x11;
	array[11] = &x12;
	array[12] = &x13;
	array[13] = &x14;
	array[14] = &x15;
	array[15] = &x16;
	array[16] = &x17;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15, T& x16, T& x17, T& x18) : _nb(18), array(new T*[18]) {
	array[0]  = &x1;
	array[1]  = &x2;
	array[2]  = &x3;
	array[3]  = &x4;
	array[4]  = &x5;
	array[5]  = &x6;
	array[6]  = &x7;
	array[7]  = &x8;
	array[8]  = &x9;
	array[9]  = &x10;
	array[10] = &x11;
	array[11] = &x12;
	array[12] = &x13;
	array[13] = &x14;
	array[14] = &x15;
	array[15] = &x16;
	array[16] = &x17;
	array[17] = &x18;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15, T& x16, T& x17, T& x18, T& x19) : _nb(19), array(new T*[19]) {
	array[0]  = &x1;
	array[1]  = &x2;
	array[2]  = &x3;
	array[3]  = &x4;
	array[4]  = &x5;
	array[5]  = &x6;
	array[6]  = &x7;
	array[7]  = &x8;
	array[8]  = &x9;
	array[9]  = &x10;
	array[10] = &x11;
	array[11] = &x12;
	array[12] = &x13;
	array[13] = &x14;
	array[14] = &x15;
	array[15] = &x16;
	array[16] = &x17;
	array[17] = &x18;
	array[18] = &x19;
}

template<class T>
Array<T>::Array(T& x1, T& x2, T& x3, T& x4, T& x5, T& x6, T& x7, T& x8, T& x9, T& x10, T& x11, T& x12, T& x13, T& x14, T& x15, T& x16, T& x17, T& x18, T& x19, T& x20) : _nb(20), array(new T*[20]) {
	array[0]  = &x1;
	array[1]  = &x2;
	array[2]  = &x3;
	array[3]  = &x4;
	array[4]  = &x5;
	array[5]  = &x6;
	array[6]  = &x7;
	array[7]  = &x8;
	array[8]  = &x9;
	array[9]  = &x10;
	array[10] = &x11;
	array[11] = &x12;
	array[12] = &x13;
	array[13] = &x14;
	array[14] = &x15;
	array[15] = &x16;
	array[16] = &x17;
	array[17] = &x18;
	array[18] = &x19;
	array[19] = &x20;
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
