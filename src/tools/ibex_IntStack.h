#ifndef __IBEX_INTSTACK__
#define __IBEX_INTSTACK__

/* IntStack : Sparse Set of integers.
 * 
 * This code is extracted from Mistral (Emmanuel Hebrard). */

#include <cstring>

namespace ibex {

class IntStack {
public:

	int *list_;
	unsigned int capacity;
	unsigned int size;
	unsigned int *index_;
	unsigned int *start_;

	IntStack(const int lb, const int ub, bool full=true) {
		initialise(lb, ub, full);
	}
	
	IntStack(IntStack *cpy) {
		size = cpy->size;
		capacity = cpy->capacity;
		int lb = (int)(cpy->start_-cpy->index_);
		list_ = new int[capacity];
		start_ = new unsigned int[capacity];
		index_ = start_ - lb;
	
		memcpy(list_, cpy->list_, capacity*sizeof(int));
		memcpy(start_, cpy->start_, capacity*sizeof(unsigned int));
	}
	
	virtual ~IntStack()
    {
      delete [] list_;
      delete [] start_;
    }
    
	virtual void initialise(const int lb, const int ub, const bool full=true) {
		capacity = ub-lb+1;
		list_ = new int[capacity];
		start_ = new unsigned int[capacity];
		index_ = start_ - lb;      

		for(int i=lb; i<=ub; ++i) {
			index_[i] = i-lb;
			list_[i-lb] = i;
		}
      
		size = (full ? capacity : 0);
	}
    
	void extend(const int new_elt) {
		int lb = (int)(start_-index_), new_lb = lb;
		int ub = capacity+lb-1, new_ub = ub;
		if(new_elt < lb) {
			new_lb = new_elt;
		} else if (new_elt > ub) {
			new_ub = new_elt;
		} else {
			return;
		}
      
		unsigned int new_capacity = new_ub-new_lb+1;
		if(new_capacity < capacity*2) new_capacity = capacity*2;
		if(new_lb < lb) {
			new_lb = ub-new_capacity+1;
		} else {
			new_ub = lb+new_capacity-1;
		}

		int *aux_list = list_;
		list_ = new int[new_capacity];
		memcpy(list_, aux_list, capacity*sizeof(int));
		delete [] aux_list;

		unsigned int *aux_start = start_;
		start_ = new unsigned int[new_capacity];
		memcpy(start_+(lb-new_lb), aux_start, capacity*sizeof(unsigned int));
		delete [] aux_start;

		index_ = start_ - new_lb;
		int k = 0;
		for(int i=new_lb; i<lb; ++i) {
			index_[i] = size+k;
			list_[capacity+k++] = i;
		}
		for(int i=ub+1; i<=new_ub; ++i) {
			index_[i] = size+k;
			list_[capacity+k++] = i;
		}

		capacity = new_capacity;
	}
    
	inline bool contain(const int elt) const {
		return index_[elt]<size;
	} 
    
	inline bool empty()const {
		return !size;
	} 
    
	inline int next(const int elt) const {
		unsigned int idx = index_[elt]+1;
		return (idx < size ? list_[idx] : elt);
	}
    
	inline void clear() {
		size = 0;
	}
    
	inline void remove(const int elt) {
		--size;
		index_[list_[size]] = index_[elt];
		list_[index_[elt]] = list_[size];
		list_[size] = elt;
		index_[elt] = size;
	}
    
	inline int head() {
		return *list_;
	}
    
	inline void add(const int elt) {
		index_[list_[size]] = index_[elt];
		list_[index_[elt]] = list_[size];
		list_[size] = elt;
		index_[elt] = size;
		++size;
	}
    
	inline void revert_to(const int level) {
		size = level;
    }
    
};
    
};

#endif