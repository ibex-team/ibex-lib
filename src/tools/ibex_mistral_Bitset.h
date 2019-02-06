/**********************************************
 * Bitset (from MISTRAL)
 *
 * Version extracted from Mistral on Aug 19 2014
 *
 * https://github.com/ehebrard/Mistral-2.0
 **********************************************/

#ifndef __MISTRAL_BITSET_H__
#define __MISTRAL_BITSET_H__

#include <iostream>
#include <limits.h>
#include <algorithm>
#include <string.h>


#if defined(_MSC_VER)
#include <intrin.h>

static unsigned int __inline __builtin_ctz(unsigned int x){
  unsigned long r = 0;
  _BitScanForward(&r, x);
  return r;
}

static unsigned int __inline __builtin_clz(unsigned int x){
  unsigned long r = 0;
  _BitScanReverse(&r, x);
  return (32 - r);
}
#  define __builtin_popcount __popcnt
#endif


namespace Mistral {

template <class WORD_TYPE>
void showUint(WORD_TYPE n, std::ostream& os) {
WORD_TYPE mask=1;
while(mask){
if(mask & n) os<<1;
else os<<0;
mask = mask << 1;
}
}

const int getlast[256] = {-1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};

const int NOVAL = (int)((~(unsigned int)0)/2);

/*! \class Bitset
\brief A representation of sets using a vector of bits.
The sets have a static capacity.
Template are used so that ReversibleWords can used instead of unsigned int
 */
template< class WORD_TYPE, class FLOAT_TYPE >
class Bitset
{
public:
	/*!@name Class attributes*/
	//@{
	static const WORD_TYPE empt = 0;
	static const WORD_TYPE full = ~0;
	static const unsigned int EXP = (sizeof(empt) == 4 ? 5 /*32 bits*/ : 6 /*64 bits*/);
	static const unsigned int size_word_bit = (1 << EXP);
	static const unsigned int size_word_byte = (size_word_bit >> 3);
	static const unsigned int CACHE = (size_word_bit - 1);
	static const unsigned int LASTCHAR = (size_word_bit - 8);
	static const unsigned int mantissa = (sizeof(empt) == 4 ? 23 /*32 bits*/ : 52 /*64 bits*/);
	static const unsigned int float_offset = (sizeof(empt) == 4 ? (0x7f) /*32 bits*/ : (0x3ff) /*64 bits*/);
	static const WORD_TYPE mask_first_char = 0xff;
	static const WORD_TYPE mask_last_char = (mask_first_char << ((size_word_bit) - 8));
	//@}
	/*!@name Parameters*/
	//@{
	/// index of the first word used to represent the set
	int pos_words;
	/// 1 + index of the last word used to represent the set
	int neg_words;
	/// A vector of bits
	WORD_TYPE* table;
	//@}

	Bitset()
	{
		initialise();
	}
	// gch: Note: don't use this constructor. Use empty(int) or all(int) instead.
	Bitset(int sz)
	{
		if(sz>0) {
			initialise(sz,0);
		} else {
			initialise();
		}
	}
	void initialise()
	{
		pos_words = 0;
		neg_words = 0;
		table = NULL;
	}
	Bitset(const int sz, const int* elt)
	{
		int lb = NOVAL;
		int ub = -NOVAL;
		for(int i=0; i<sz; ++i) {
			if(elt[i] > ub) ub = elt[i];
			if(elt[i] < lb) lb = elt[i];
		}
		initialise(lb,ub,empt);
		for(int i=0; i<sz; ++i)
			add( elt[i] );
	}
//	void initialise(const Vector< int >& elt)
//	{
//		int min = elt.front();
//		// int max = elt.front();
//		// for(unsigned int i=1; i<elt.size; ++i) {
//		// if(elt[i] < min) min = elt[i];
//		// if(elt[i] > max) max = elt[i];
//		// }
//		int max = elt.back();
//		initialise(min,max,empt);
//		for(unsigned int i=0; i<elt.size; ++i)
//			add( elt[i] );
//	}
//	void initialise(const int lb, const int ub, const Vector< int >& elt)
//	{
//		// std::cout << "initialise bitset with " << lb << ".." << ub << ": " ;
//		// elt.display(std::cout);
//		// std::cout << std::endl;
//		initialise(lb,ub,empt);
//		// display(std::cout);
//		// std::cout << std::endl;
//		for(unsigned int i=0; i<elt.size; ++i) {
//			add( elt[i] );
//			// display(std::cout);
//			// std::cout << std::endl;
//		}
//	}
	Bitset(const int lb, const int ub, const WORD_TYPE p)
	{
		initialise(lb,ub,p,NULL);
	}
	inline int word_index(const int elt) const
	{
		return (elt >> EXP);
	}
	bool operator==(const Bitset<WORD_TYPE,FLOAT_TYPE>& s) {
		return equal(s);
	}
	bool operator!=(const Bitset<WORD_TYPE,FLOAT_TYPE>& s) {
		return !equal(s);
	}
	Bitset<WORD_TYPE,FLOAT_TYPE>& operator=(const Bitset<WORD_TYPE,FLOAT_TYPE>& q)
	{
		if(!table)
			clone(q);
		else
			copy(q);
		return *this;
	}
	void reinitialise(const int lb, const int ub, const WORD_TYPE p)
	{
		table += neg_words;
		delete [] table;
		initialise(lb, ub, p, NULL);
	}
	void initialise(const int sz, const WORD_TYPE p)
	{
		pos_words = sz;
		neg_words = 0;
		if( sz>=0 ) {
			table = new WORD_TYPE[pos_words];
			for(int i=0; i<pos_words; ++i)
				table[i]=p;
		} else table = NULL;
	}
	void initialise(const int lb, const int ub, const WORD_TYPE p, WORD_TYPE *pool=NULL)
	{
		neg_words = (lb >> EXP);
		pos_words = (ub >> EXP)+1;
		if(pool==NULL) table = new WORD_TYPE[pos_words-neg_words];
		else table = pool;
		for(int i=0; i<pos_words-neg_words; ++i)
			table[i]=p;
		table[pos_words-neg_words-1] &=
				(p >> (size_word_bit-1-(ub & CACHE)));
		table[0] &= (p << (lb & CACHE));
		table -= neg_words;
	}
	void initialise(const Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		pos_words = s.pos_words;
		neg_words = s.neg_words;
		table = new WORD_TYPE[pos_words-neg_words];
		table -= neg_words;
		for(int i=neg_words; i<pos_words; ++i)
			//table[i].initialise(s.table+i, s.size(i));
			table[i] = s.table[i];
	}
	inline void declare(const int elt)
	{
		int i = (elt >> EXP);
		if( (i < neg_words) ||
				(i >= pos_words) )
		{
			extend(elt);
		}
		fast_add(elt);
	}
	void extend(const int elt)
	{
		int nval = (elt >> EXP);
		if( (nval < neg_words) ||
				(nval >= pos_words) )
		{
			int new_neg_words = neg_words;
			//nval;
			int new_pos_words = pos_words;
			//nval+1;
			bool need_to_extend = false;
			if(nval < new_neg_words) {
				new_neg_words = nval;
				need_to_extend = true;
			}
			if(nval >= new_pos_words) {
				new_pos_words = nval+1;
				need_to_extend = true;
			}
			if(need_to_extend) {
				WORD_TYPE *aux = table;
				table = new WORD_TYPE[new_pos_words-new_neg_words];
				table -= new_neg_words;
				memcpy(table+neg_words, aux+neg_words,
						(pos_words-neg_words)*sizeof(WORD_TYPE));
				if(new_neg_words < neg_words)
					std::fill(table+new_neg_words, table+neg_words, 0);
				if(new_pos_words > pos_words)
					std::fill(table+pos_words, table+new_pos_words, 0);
				aux += neg_words;
				delete [] aux;
				pos_words = new_pos_words;
				neg_words = new_neg_words;
			}
		}
	}
	Bitset(const Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		initialise();
		clone( s );
	}
	void clone(const Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		if(table) {
			table += neg_words;
			delete [] table;
		}
		neg_words = s.neg_words;
		pos_words = s.pos_words;
		table = new WORD_TYPE[pos_words-neg_words];
		memcpy(table, s.table+neg_words,
				size_word_byte*(pos_words-neg_words));
		table -= neg_words;
	}
	void point_to(Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		neg_words = s.neg_words;
		pos_words = s.pos_words;
		table = s.table;
	}
	void point_to(WORD_TYPE *t)
	{
		neg_words = 0;
		pos_words = 1;
		table = t;
	}
	void copy(const Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		int i = (pos_words > s.pos_words ? s.pos_words : pos_words);
		int k, j = (neg_words < s.neg_words ? s.neg_words : neg_words);
		for( k=neg_words; k<j; ++k)
			table[k] = empt;
		for( k=i; k<pos_words; ++k)
			table[k] = empt;
		if( i>j )
			memcpy(table+j,s.table+j,size_word_byte*(i-j));
	}
	virtual ~Bitset()
	{
		table += neg_words;
		delete [] table;
	}
	void destroy()
	{
		table += neg_words;
		neg_words = 0;
		delete [] table;
		table = NULL;
	}
	bool is_built()
	{
		return (table != NULL);
	}
	inline void swap(Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		WORD_TYPE *aux = s.table;
		s.table = table;
		table = aux;
	}
	void iterate_into_b(const int size, int *buffer) {
		int elt;
		int idx;
		int nval;
		union {FLOAT_TYPE f; WORD_TYPE i; } t;
		WORD_TYPE b;
		WORD_TYPE v;
		nval = 1;
		elt = buffer[0];
		idx = ((elt+1) >> EXP);
		v = (table[idx] & (full << ((elt+1) & CACHE)));
		while(nval < size) {
			// find the next word that is not null
			while(!v) v = table[++idx];
			// find the first element in the set:
			// remove all other element
			b = v & -v;
			// cast into float, which will be coded as 1*2^exp, and 'exp' is precisely the index of the first element
			t.f = (FLOAT_TYPE)b; // cast the least significant bit in v to a float
			// keep only the exponant part
			elt = t.i >> mantissa;
			elt += idx * size_word_bit - float_offset;
			do {
				// put the element in the buffer
				buffer[nval] = elt;
				++nval;
				// remove it from v
				v ^= b;
				do {
					// try the next element
					b <<= 1;
					++elt;
				} while( b && !(v & b) );
			} while(v);
		}
	}
	void iterate_into(const int size, int *buffer) {
		int elt;
		int idx;
		int nval;
		union {FLOAT_TYPE f; WORD_TYPE i; } t;
		WORD_TYPE b;
		WORD_TYPE v;
		nval = 1;
		elt = buffer[0];
		idx = ((elt+1) >> EXP);
		v = (table[idx] & (full << ((elt+1) & CACHE)));
		elt = (idx * size_word_bit - float_offset);
		while(nval < size) {
			// find the next word that is not null
			while(!v) {
				v = table[++idx];
				elt += size_word_bit;
			}
			// find the first element in the set:
			do {
				// remove all other element
				b = v & -v;
				// cast into float, which will be coded as 1*2^exp, and 'exp' is precisely the index of the first element
				t.f = (FLOAT_TYPE)b; // cast the least significant bit in v to a float
				// put the element in the buffer
				buffer[nval++] = (t.i >> mantissa)+elt;
				// remove it from v
				v ^= b;
			} while(v);
		}
	}
	inline int lsb_mantissa(const WORD_TYPE v) const {
		union {FLOAT_TYPE f; WORD_TYPE i; } t;
		WORD_TYPE b = v & -v;
		t.f = (FLOAT_TYPE)b; // cast the least significant bit in v to a float
		b = t.i >> mantissa;
		return b - float_offset;
	}
	inline int lsb_gcc(const WORD_TYPE v) const {
		return __builtin_ctz(v);
	}
	inline int msb_gcc(const WORD_TYPE v) const {
		return __builtin_clz(v);
	}
	inline int minimum_element(int idx, WORD_TYPE v, const int def=NOVAL) const
	{
		while(v == 0) {
			if( ++idx >= pos_words )
				return def;
			v = table[idx];
		}
		/*
union {FLOAT_TYPE f; WORD_TYPE i; } t;
WORD_TYPE b = v & -v;
t.f = (FLOAT_TYPE)b; // cast the least significant bit in v to a float
b = t.i >> mantissa;
		 */
		//return b + idx * size_word_bit - float_offset;
		return __builtin_ctz(v) //__builtin_ffs(v) - 1
				+ (idx * size_word_bit);
	}
	/*!
Minimum element in the set [O(N/32)].
	 */
	inline int min() const
	{
		int idx = neg_words;
		WORD_TYPE v = table[idx];
		return minimum_element(idx,v);
	}
	/*!
Maximum element in the set [O(N/8)].
	 */
	inline int max() const
	{
		WORD_TYPE tab;
		int i=pos_words, j, k;
		while( i-- > neg_words )
			if( (tab = table[i]) ) {
				j = size_word_byte;
				while( j-- ) {
					if( (k = getlast[(tab & mask_last_char) >> LASTCHAR]) >= 0 )
						return ( (i<<EXP)+(j<<3)+k );
					tab = (tab << 8);
				}
			}
		return NOVAL;
	}
	inline void remove(const int elt)
	{
		int i = (elt >> EXP);
		if( (i >= neg_words) &&
				(i < pos_words) )
			table[i] &= (full ^ ((WORD_TYPE)1 << (elt & CACHE)));
	}
	inline void fast_remove(const int elt)
	{
		table[(elt >> EXP)] &= (full ^ ((WORD_TYPE)1 << (elt & CACHE)));
	}
	inline void word_remove(const int elt)
	{
		table[neg_words] &= (full ^ ((WORD_TYPE)1 << (elt & CACHE)));
	}
	inline int next(const int elt) const {
		int idx = ((elt+1) >> EXP);
		if(idx >= pos_words) return elt;
		WORD_TYPE v = (table[idx] & (full << ((elt+1) & CACHE)));
		while(v == 0) {
			if(++idx >= pos_words) return elt;
			v = table[idx];
		}
		return lsb_gcc(v) + (idx * size_word_bit);
	}
	/*
inline int next(const int elt) const {
int idx = ((elt+1) >> EXP);
if(idx >= pos_words) return elt;
WORD_TYPE v = (table[idx] & (full << ((elt+1) & CACHE)));
return minimum_element(idx,v,elt);
}
	 */
	inline int prev(const int elt) const {
		WORD_TYPE tab;
		int i = ((elt-1) >> EXP);
		int SHFT = size_word_byte;
		if( i >= neg_words ) {
			int e = ((elt-1) & CACHE), k;
			int j = 1+(e >> 3);
			if( (tab = ((table[i] & (full >> (CACHE - e))) << ((SHFT-j) << 3))) )
				while( j-- ) {
					if( (k = getlast[(tab & mask_last_char) >> LASTCHAR]) >= 0 )
						return ( (i<<EXP)+(j<<3)+k );
					tab = (tab << 8);
				}
			while( i-- > neg_words )
				if( (tab = table[i]) ) {
					j = size_word_byte;
					while( j-- ) {
						if( (k = getlast[(tab & mask_last_char) >> LASTCHAR]) >= 0 )
							return ( (i<<EXP)+(j<<3)+k );
						tab = (tab << 8);
					}
				}
		}
		return elt;
	}
	inline void xor_to(const Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		int i = (pos_words > s.pos_words ? s.pos_words : pos_words);
		int j = (neg_words < s.neg_words ? s.neg_words : neg_words);
		while( i-- > j )
			s.table[i] ^= table[i];
	}
	inline void fast_xor_to(const Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		int i = pos_words;
		while( i-- > neg_words )
			s.table[i] ^= table[i];
	}
	inline void xor_with(const Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		int i = (pos_words > s.pos_words ? s.pos_words : pos_words);
		int j = (neg_words < s.neg_words ? s.neg_words : neg_words);
		while( i-- > j )
			table[i] ^= s.table[i];
	}
	inline void fast_xor_with(const Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		int i = pos_words;
		while( i-- > neg_words )
			table[i] ^= s.table[i];
	}
	inline void union_with(const Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		int i = (pos_words > s.pos_words ? s.pos_words : pos_words);
		int j = (neg_words < s.neg_words ? s.neg_words : neg_words);
		while( i-- > j )
			table[i] |= s.table[i];
	}
	inline void union_with(const int s)
	{
		if(pos_words>0 && neg_words<=0) table[0] |= s;
	}
	inline void union_to(Bitset<WORD_TYPE,FLOAT_TYPE>& s) const
	{
		s.union_with( *this );
	}
	inline void intersect_with(const Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		int i = (pos_words > s.pos_words ? s.pos_words : pos_words);
		int j = (neg_words < s.neg_words ? s.neg_words : neg_words);
		int k = pos_words;
		while( k > i ) {
			--k;
			table[k] = empt;
		}
		while( k > j ) {
			--k;
			table[k] &= s.table[k];
		}
		while( k-- > neg_words )
			table[k] = empt;
	}
	inline void intersect_with(const int s)
	{
		int i=pos_words;
		while(i-- > 1) table[i] = empt;
		i = 0;
		while(i-- > neg_words) table[i] = empt;
		if(pos_words>0 && neg_words<=0) table[0] &= s;
	}
	inline void intersect_to(Bitset<WORD_TYPE,FLOAT_TYPE>& s) const
	{
		s.intersect_with( *this );
	}
	inline void setminus_with (const Bitset<WORD_TYPE,FLOAT_TYPE>& s)
	{
		int i = (pos_words > s.pos_words ? s.pos_words : pos_words);
		int j = (neg_words < s.neg_words ? s.neg_words : neg_words);
		while( i-- > j )
			table[i] &= (~(s.table[i]));
	}
	inline void setminus_to (Bitset<WORD_TYPE,FLOAT_TYPE>& s) const
	{
		s.setminus_with( *this );
	}
	inline void xor_with(const Bitset<WORD_TYPE,FLOAT_TYPE>* s)
	{
		xor_with(*s);
	}
	inline void union_with (const Bitset<WORD_TYPE,FLOAT_TYPE>* s)
	{
		union_with(*s);
	}
	inline void intersect_with(const Bitset<WORD_TYPE,FLOAT_TYPE>* s)
	{
		intersect_with(*s);
	}
	inline void setminus_with (const Bitset<WORD_TYPE,FLOAT_TYPE>* s)
	{
		setminus_with(*s);
	}
	inline void union_to (Bitset<WORD_TYPE,FLOAT_TYPE>* s) const
	{
		s->union_with(*this);
	}
	inline void intersect_to(Bitset<WORD_TYPE,FLOAT_TYPE>* s) const
	{
		s->intersect_with(*this);
	}
	inline void setminus_to (Bitset<WORD_TYPE,FLOAT_TYPE>* s) const
	{
		s->setminus_with(*this);
	}
	inline bool equal(const Bitset<WORD_TYPE,FLOAT_TYPE>& s) const
	{
		int i=pos_words;
		int j=s.pos_words;
		int k;
		while( j > i )
			if( s.table[--j] ) return false;
		while( i > j )
			if( table[--i] ) return false;
		j=neg_words;
		k=s.neg_words;
		while( j > k )
			if( s.table[k++] ) return false;
		while( k > j )
			if( table[j++] ) return false;
		while( i-- > j )
			if( table[i] != s.table[i] ) return false;
		return true;
	}
	inline bool includes(const WORD_TYPE s) const
	{
		return( pos_words && neg_words<1 && (table[0] & s) == s );
	}
	inline bool included(const WORD_TYPE s) const
	{
		bool inc = true;
		int k = pos_words;
		if(neg_words>0 || pos_words<1) {
			while(k>neg_words && inc) inc = !(table[--k]);
		} else {
			while(k>1 && inc) inc = !(table[--k]);
			inc = ((table[--k] & s) == table[0]);
			while(k>neg_words && inc) inc = !(table[--k]);
		}
		return inc;
	}
	inline bool included(const Bitset<WORD_TYPE,FLOAT_TYPE>& s) const
	{
		int i = (pos_words > s.pos_words ? s.pos_words : pos_words);
		int j = (neg_words < s.neg_words ? s.neg_words : neg_words);
		int k = pos_words;
		while( k > i ) {
			--k;
			if( table[k] ) return false;
		}
		while( k > j ) {
			--k;
			if( table[k] != (table[k] & s.table[k]) ) return false;
		}
		while( k-- > neg_words )
			if( table[k] ) return false;
		return true;
	}
	inline bool includes(const Bitset<WORD_TYPE,FLOAT_TYPE>& s) const
	{
		int i = (pos_words > s.pos_words ? s.pos_words : pos_words);
		int j = (neg_words < s.neg_words ? s.neg_words : neg_words);
		int k = s.pos_words;
		while( k > i ) {
			--k;
			if( s.table[k] ) return false;
		}
		while( k > j ) {
			--k;
			if( s.table[k] != (table[k] & s.table[k]) ) {
				return false;
			}
		}
		while( k-- > s.neg_words ) {
			if( s.table[k] ) return false;
		}
		return true;
	}
	inline bool included(const Bitset<WORD_TYPE,FLOAT_TYPE>* s) const
	{
		return included( *s );
	}
	inline bool includes(const Bitset<WORD_TYPE,FLOAT_TYPE>* s) const
	{
		return includes( *s );
	}
	inline bool intersect(const Bitset<WORD_TYPE,FLOAT_TYPE>* s) const
	{
		return intersect( *s );
	}
	inline bool included(const int lb, const int ub) const
	{
		int neg_int = lb >> EXP;
		int pos_int = ub >> EXP;
		int k = pos_words;
		while( k > pos_int )
			if( table[--k] ) return false;
		k = neg_words;
		while( k < neg_int )
			if( table[k++] ) return false;
		if(neg_int == pos_int) {
			k = ((full << (lb & CACHE)) & (full >> (CACHE - (ub & CACHE))));
			return (k & table[neg_int]) == table[neg_int];
		} else {
			return ((((full << (lb & CACHE)) & table[neg_int]) == table[neg_int]) &&
					(((full >> (CACHE - (ub & CACHE))) & table[pos_int]) == table[pos_int]));
		}
	}
	inline bool includes(const int lb, const int ub) const
	{
		int neg_int = lb >> EXP;
		int pos_int = ub >> EXP;
		int k = pos_int-1;
		unsigned int u, l;
		while( k > neg_int ) {
			if( table[k] != full ) return false;
			--k;
		}
		if(neg_int == pos_int) {
			u = ((full << (lb & CACHE)) & (full >> (CACHE - (ub & CACHE))));
			return (u & table[neg_int]) == u;
		} else {
			u = (full >> (CACHE - (ub & CACHE)));
			l = (full << (lb & CACHE));
			return (((l & table[neg_int]) == l) &&
					((u & table[pos_int]) == u));
		}
	}
	/*!
	 * Returns the number of bits set in v.
	 * For a derivation of this algorithm, see
	 * "Algorithms and data structures with applications to
	 * graphics and geometry", by Jurg Nievergelt and Klaus Hinrichs,
	 * Prentice Hall, 1993.
	 */
	inline unsigned int word_size(WORD_TYPE v) const
	{
		return __builtin_popcount(v);
		/*
v = v - ((v >> 1) & (WORD_TYPE)~(WORD_TYPE)0/3); // temp
v = (v & (WORD_TYPE)~(WORD_TYPE)0/15*3) + ((v >> 2) & (WORD_TYPE)~(WORD_TYPE)0/15*3); // temp
v = (v + (v >> 4)) & (WORD_TYPE)~(WORD_TYPE)0/255*15; // temp
return (WORD_TYPE)(v * ((WORD_TYPE)~(WORD_TYPE)0/255)) >> (sizeof(v) - 1) * CHAR_BIT; // count
		 */
	}
	inline unsigned int size() const
	{
		int i=pos_words;
		unsigned int c=0;
		WORD_TYPE v;
		while( i-- > neg_words )
			if( (v = table[i]) )
				c += word_size(v);
		return c;
	}
	inline unsigned int word_size() const
	{
		unsigned int v, c=0;
		if( (v = table[neg_words]) )
			c = word_size(v);
		return c;
	}
	inline unsigned int size( const int i ) const
	{
		WORD_TYPE v;
		unsigned int c=0;
		if( (v = table[i]) )
			c = word_size(v);
		return c;
	}
	/*!
Check if element elt belong to the set [O(1)]
	 */
	inline bool contain(const int elt)const
	{
		int i = (elt >> EXP);
		return ( (i >= neg_words) &&
				(i < pos_words) &&
				(table[i] & ((WORD_TYPE)1 << (elt & CACHE))) );
	}
	inline bool fast_contain(const int elt)const
	{
		return ( (table[(elt >> EXP)] & ((WORD_TYPE)1 << (elt & CACHE))) );
	}
	inline bool word_contain(const int elt)const
	{
		return ( (table[neg_words] & ((WORD_TYPE)1 << (elt & CACHE))) );
	}
	/*!
Add element elt into the set [O(1)]
	 */
	inline void add(const int elt)
	{
		int i = (elt >> EXP);
		if( (i >= neg_words) &&
				(i < pos_words) )
			table[i] |= ((WORD_TYPE)1 << (elt & CACHE));
	}
	inline void fast_add(const int elt)
	{
		table[(elt >> EXP)] |= ((WORD_TYPE)1 << (elt & CACHE));
	}
	inline void word_add(const int elt)
	{
		table[neg_words] |= ((WORD_TYPE)1 << (elt & CACHE));
	}
	/*!
Add element elt into the set or remove it if it is already contain [O(1)]
	 */
	inline void invert(const int elt)
	{
		int i = (elt >> EXP);
		if( (i >= neg_words) &&
				(i < pos_words) )
			table[i] ^= ((WORD_TYPE)1 << (elt & CACHE));
	}
	inline void fast_invert(const int elt)
	{
		table[(elt >> EXP)] ^= ((WORD_TYPE)1 << (elt & CACHE));
	}
	inline void word_invert(const int elt)
	{
		table[neg_words] ^= ((WORD_TYPE)1 << (elt & CACHE));
	}
	/*!
Return true iff the set is empty [O(N/32)]
	 */
	inline bool empty()const
	{
		int i = pos_words;
		while( i-- > neg_words )
			if(table[i]) return false;
		return true;
	}
	/*!
Return true iff the calling object intersect s [O(N/32)]
	 */
	inline bool intersect(const Bitset<WORD_TYPE,FLOAT_TYPE>& s)const
	{
		int i = (pos_words > s.pos_words ? s.pos_words : pos_words);
		int j = (neg_words < s.neg_words ? s.neg_words : neg_words);
		while( i-- > j )
			if(table[i] & s.table[i]) return true;
		return false;
	}
	/*!
Return true iff the calling object intersect s (s is assumed to be a bitset in {0,..,31}) [O(N/32)]
	 */
	inline bool intersect(const int s) const
	{
		return(pos_words && neg_words<1 && (table[0]&s));
	}
	inline bool word_intersect(const Bitset<WORD_TYPE,FLOAT_TYPE>& s) const
	{
		return ( table[neg_words] & s.table[neg_words] ) ;
	}
	inline bool fast_intersect(const Bitset<WORD_TYPE,FLOAT_TYPE>& s, int& idx)const
	{
		if( table[idx] & s.table[idx] ) return true;
		if( pos_words > neg_words ) {
			idx = pos_words;
			while( idx > neg_words ) {
				--idx;
				if(table[idx] & s.table[idx]) return true;
			}
		}
		return false;
	}
	/*!
Return true iff the calling object intersect [lo..up] [O(N/32)]
	 */
	inline bool intersect(const int lb, const int ub)const
	{
		int i = (ub >> EXP);
		int j = (lb >> EXP);
		if( i < neg_words || j >= pos_words )
			return false;
		WORD_TYPE masked_lb = (full << (lb & CACHE));
		WORD_TYPE masked_ub = (full >> (CACHE - (ub & CACHE)));
		if( i == j ) {
			if( table[i] & (masked_lb & masked_ub) ) return true;
			else return false;
		}
		if( i >= pos_words )
			i = pos_words-1;
		else if( table[i--] & masked_ub ) return true;
		if( j < neg_words )
			j = neg_words;
		else if( table[j++] & masked_lb ) return true;
		while( i >= j )
			if(table[i--]) return true;
		return false;
	}
	/*!
Increment by x all elements in the set.
Any element greater or equal than the capacity
of the set minus x is removed [O(N/32)]
	 */
	inline void increment(const int v)
	{
		int step = (v >> EXP);
		int i = pos_words;
		int e = (v & CACHE);
		int f = size_word_bit-e;
		int j = neg_words+step;
		WORD_TYPE mask = ((WORD_TYPE)~0 << f);
		while( --i > j )
			table[i] = ((table[i-step] << e) | ((table[i-step-1] & mask) >> f));
		if( i >= neg_words+step ) table[i] = (table[i-step] << e);
		while( i > neg_words ) table[--i] = 0;
	}
	/*!
Decrement by x all elements in the set.
Any element lower than x is removed [O(N/32)]
	 */
	inline void decrement(const int v)
	{
		int step = (v >> EXP);
		int i = neg_words-1;
		int e = (v & CACHE);
		int f = size_word_bit-e;
		int j = pos_words-step-1;
		WORD_TYPE mask = ((WORD_TYPE)~0 >> e);
		while( ++i < j )
			table[i] = ((table[i+step] >> e) | ((table[i+step+1] & mask) << f));
		if( i < pos_words-step ) table[i] = (table[i+step] >> e);
		while( ++i < pos_words ) table[i] = 0;
	}
	/*!
Changes every value to its arythmetic negation
	 */
	inline void negate( Bitset<WORD_TYPE,FLOAT_TYPE>& s ) const
	{
		int i = (pos_words > -s.neg_words ? -s.neg_words : pos_words);
		int j = (neg_words < -s.pos_words ? -s.pos_words : neg_words);
		unsigned int a;
		WORD_TYPE mask, v, aux, rest = ( i < pos_words && (table[i] & 1) );
		while( i-- > j ) {
			aux = (table[i] & 1);
			v = (table[i] >> 1);
			mask = ~0;
			a = sizeof(v) * CHAR_BIT; // bit size; must be power of 2
			while ((a >>= 1) > 0)
			{
				mask ^= (mask << a);
				v = ((v >> a) & mask) | ((v << a) & ~mask);
			}
			s.table[-i-1] = (v | rest);
			rest = aux;
		}
		if(rest)
			s.table[i+1] |= rest;
	}
	/*!
Add all elements between 0 to capacity [O(N/32)]
	 */
	inline void fill()
	{
		int i = pos_words;
		while( i > neg_words )
			table[--i] = full;
	}
	inline void fill(const int lb, const int ub)
	{
		int i = (ub >> EXP);
		int j = (lb >> EXP);
		if( i >= neg_words || j < pos_words ) {
			WORD_TYPE masked_lb = (full << (lb & CACHE));
			WORD_TYPE masked_ub = (full >> (CACHE - (ub & CACHE)));
			if( i == j ) {
				table[i] |= (masked_lb & masked_ub);
			} else {
				if( i >= pos_words ) {
					i = pos_words-1;
				} else {
					table[i--] |= masked_ub;
				}
				if( j < neg_words ) {
					j = neg_words;
				} else {
					table[j++] |= masked_lb;
				}
				while( i >= j )
					table[i--] |= full;
			}
		}
	}
	/*!
Remove all elements [O(N/32)]
	 */
	inline void clear()
	{
		int i = pos_words;
		while( i > neg_words )
			table[--i] = empt;
	}
	/*!
Remove all elements but v [O(N/32)]
	 */
	inline void set_to( const int v )
	{
		int i, j = (v >> EXP);
		for(i=neg_words; i<j; ++i)
			table[i] = empt;
		table[j] = ((WORD_TYPE)1 << v);
		for(i=j+1; i<pos_words; ++i)
			table[i] = empt;
	}
	/*!
flip all elements [O(N/32)]
	 */
	inline void flip()
	{
		int i = pos_words;
		while( i > neg_words )
			table[--i] ^= full;
	}
	/*!
Remove all elements strictly lower than l [O(N/32)]
	 */
	inline void set_min(const int bound)
	{
		int ith_word=(bound >> EXP);
		if( ith_word >= neg_words ) {
			if( ith_word < pos_words ) {
				int i=ith_word;
				while( i-- > neg_words ) table[i]=0;
				table[ith_word] &= (full << (bound & CACHE));
			} else clear();
		}
	}
	/*!
Remove all elements strictly greater than u [O(N/32)]
	 */
	inline void set_max(const int bound)
	{
		int ith_word=(bound >> EXP);
		if( ith_word < pos_words ) {
			if( ith_word >= neg_words ) {
				int i=pos_words;
				while( --i > ith_word ) table[i]=0;
				table[ith_word] &= (full >> (CACHE - (bound & CACHE)));
			} else clear();
		}
	}
	/*!
Remove all elements in the interval [l..u] [O(N/32)]
	 */
	inline void remove_interval(const int lb, const int ub)
	{
		if( lb <= ub ) {
			int lb_word = lb >> EXP;
			int ub_word = ub >> EXP;
			lb_word = ( lb_word < neg_words ? neg_words : lb_word );
			ub_word = ( ub_word >= pos_words ? pos_words-1 : ub_word );
			WORD_TYPE masked_lb = 0;
			WORD_TYPE masked_ub = 0;
			if( lb_word >= neg_words )
				// add a '0' on the 32nd bit, because >> 32 does nothing
				masked_lb = ((full/2) >> (CACHE - (lb & CACHE)));
			if( ub_word < pos_words )
				masked_ub = ((full-1) << (ub & CACHE));
			if( lb_word == ub_word ) {
				table[lb_word] &= (masked_lb | masked_ub);
			} else {
				table[lb_word] &= masked_lb;
				table[ub_word] &= masked_ub;
				while( --ub_word > lb_word )
					table[ub_word] = 0;
			}
		}
	}
	/*!
Add all elements in the interval [l..u] [O(N/32)]
	 */
	inline void add_interval(int lb, int ub)
	{
		if( lb <= ub ) {
			int lb_word = lb >> EXP;
			int ub_word = ub >> EXP;
			lb_word = ( lb_word < neg_words ? neg_words : lb_word );
			ub_word = ( ub_word >= pos_words ? pos_words-1 : ub_word );
			WORD_TYPE masked_lb = full;
			WORD_TYPE masked_ub = full;
			if( lb_word >= neg_words )
				//masked_lb ^= (full >> (CACHE - (lb & CACHE) + 1));
				masked_lb ^= ((full/2) >> (CACHE - (lb & CACHE)));
			if( ub_word < pos_words )
				//masked_ub ^= ((full-1) << (ub & CACHE) );
				masked_ub ^= ((full-1) << (ub & CACHE));
			if( lb_word == ub_word )
				table[lb_word] |= (masked_lb & masked_ub);
			else {
				table[lb_word] |= masked_lb;
				table[ub_word] |= masked_ub;
				while( --ub_word > lb_word )
					table[ub_word] = full;
			}
		}
	}
	inline bool operator[](const int i) const // "const" added by gch
	{
		return fast_contain(i);
	}
	std::ostream& display(std::ostream& os) const {
		os << "{";
		if( !empty() ) {
			int last = NOVAL, cur=min(), aft;
			bool flag=false;
			do{
				aft = next(cur);
				if(aft != cur+1 || cur != last+1) {
					if( flag )
						os << ",";
					os << (int)cur;
					flag = true;
				} else if(flag) {
					os << "..";
					flag = false;
				}
				last = cur;
				cur = aft;
			} while( cur != NOVAL && cur != last );
		}
		os << "}";
		return os;
	}
	void print_bits(std::ostream & os) const
	{
		os << "[";
		for(int i=neg_words; i<pos_words; ++i)
			showUint( table[i], os );
		os << "]";
	}
};

typedef Bitset< unsigned long long int, double > Bitset64;
typedef Bitset< unsigned int, float > Bitset32;

#ifdef _BIT64

typedef Bitset64 BitSet;

#else

typedef Bitset32 BitSet;

#endif

} // end namespace Mistral

#endif
