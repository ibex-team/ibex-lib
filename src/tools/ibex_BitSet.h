#ifndef __IBEX_BITSET_H__
#define __IBEX_BITSET_H__

/* BitSet : Bitset representation of integer sets.
 * 
 * This code is extracted from Mistral (Emmanuel Hebrard). */

#include <limits.h>
#include <algorithm>

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
#define INFTY NOVAL/2

namespace ibex {

template< class WORD_TYPE, class FLOAT_TYPE >
class Bitset { 

  public:
    
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

    /// index of the first word used to represent the set
    int pos_words;
    /// 1 + index of the last word used to represent the set
    int neg_words;
    /// A vector of bits 
    WORD_TYPE* table;

    Bitset()
    {
      initialise();
    }

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
      int lb =  NOVAL;
      int ub = -NOVAL;
      for(int i=0; i<sz; ++i) {
	if(elt[i] > ub) ub = elt[i];
	if(elt[i] < lb) lb = elt[i];
      }

      initialise(lb,ub,empt);

      for(int i=0; i<sz; ++i) 
	add( elt[i] );
    }

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

    void initialise(Bitset<WORD_TYPE,FLOAT_TYPE>& s) 
    {
      pos_words = s.pos_words;
      neg_words = s.neg_words;
  
      table = new WORD_TYPE[pos_words-neg_words];
      table -= neg_words;
      for(unsigned int i=neg_words; i<pos_words; ++i) 
	table[i].initialise(s.table+i, s.size(i));
    }

    inline void declare(const int elt)
    {
      int i = (elt >> EXP);
      if( (i < neg_words) ||
	  (i >=  pos_words) ) 
	{
	  extend(elt);
	}
      fast_add(elt);
    }

    void extend(const int elt) 
    {
      int nval = (elt >> EXP);
      if( (nval < neg_words) ||
	  (nval >=  pos_words) ) 
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

    inline void  remove(const int elt) 
    {
      int i = (elt >> EXP);
      if( (i >= neg_words) && 
	  (i <  pos_words) )
	table[i] &= (full ^ ((WORD_TYPE)1 << (elt & CACHE)));
    }

    inline void  fast_remove(const int elt) 
    {
      table[(elt >> EXP)] &= (full ^ ((WORD_TYPE)1 << (elt & CACHE)));
    }

    inline void  word_remove(const int elt) 
    {
      table[neg_words] &= (full ^ ((WORD_TYPE)1 << (elt & CACHE)));
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

    inline bool includes(const Bitset<WORD_TYPE,FLOAT_TYPE>* s) const 
    {
      return includes( *s );
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
    inline  bool contain(const int elt)const 
    {
      int i = (elt >> EXP);
      return ( (i >= neg_words) && 
	       (i <  pos_words) && 
	       (table[i] & ((WORD_TYPE)1 << (elt & CACHE))) );
    }

    inline  bool fast_contain(const int elt)const 
    {
      return ( (table[(elt >> EXP)] & ((WORD_TYPE)1 << (elt & CACHE))) );
    }

    inline  bool word_contain(const int elt)const 
    {
      return ( (table[neg_words] & ((WORD_TYPE)1 << (elt & CACHE))) );
    }
    /*!
      Add element elt into the set [O(1)]
    */

    inline  void add(const int elt)
    {
      int i = (elt >> EXP);
      if( (i >= neg_words) && 
	  (i <  pos_words) ) 
	table[i] |= ((WORD_TYPE)1 << (elt & CACHE));
    }

    inline  void fast_add(const int elt)
    {
      table[(elt >> EXP)] |= ((WORD_TYPE)1 << (elt & CACHE));
    }

    inline   void word_add(const int elt)
    {
      table[neg_words] |= ((WORD_TYPE)1 << (elt & CACHE));
    }

    /*!
      Return true iff the set is empty [O(N/32)]
    */
    inline  bool empty()const
    { 
      int i = pos_words;
      while( i-- > neg_words ) 
	if(table[i]) return false;
      return true;  
	}

    /*!
      Remove all elements [O(N/32)]
    */
    inline  void clear()
    {
      int i = pos_words;
      while( i > neg_words )
	table[--i] = empt;
    }

    inline bool operator[](const int i)
    {
      return fast_contain(i);
    }

  };
  
typedef Bitset< unsigned long long int, double > Bitset64;
typedef Bitset< unsigned int, float > Bitset32;

#ifdef _BIT64
  
  typedef Bitset64 BitSet;

#else

  typedef Bitset32 BitSet;

#endif
  
}; // end namespace ibex
  
#endif