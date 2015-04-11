//============================================================================
//                                  I B E X
// File        : ibex_NodeType.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : 18 Aug 2014
//============================================================================

#include "ibex_NodeType.h"

// =========== shortcuts ==================
#define IN         __IBEX_IN__
#define OUT        __IBEX_OUT__
#define UNK        __IBEX_UNK__
#define UNK_IN     __IBEX_UNK_IN__
#define UNK_OUT    __IBEX_UNK_OUT__
#define UNK_IN_OUT __IBEX_UNK_IN_OUT__
#define IN_TMP     __IBEX_IN_TMP__
// ========================================

namespace ibex {
// relic from irregular set, need for SetBisect constructor
NodeType operator|(NodeType x, NodeType y) {
	switch (x) {
	case IN : {
		switch(y) {
		case IN :        return IN;
		case OUT :
		case UNK_OUT:
		case UNK_IN_OUT: return UNK_IN_OUT;
		default :        return UNK_IN;
		}
	}
	break;
	case OUT : {
		switch(y) {
		case OUT :       return OUT;
		case IN :
		case UNK_IN:
		case UNK_IN_OUT: return UNK_IN_OUT;
		default :        return UNK_OUT;
		}
	}
	break;
	case UNK : {
		switch(y) {
		case IN :
		case UNK_IN:  return UNK_IN;
		case OUT :
		case UNK_OUT: return UNK_OUT;
		case UNK:     return UNK;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	case UNK_IN : {
		switch(y) {
		case IN :
		case UNK_IN:
		case UNK :    return UNK_IN;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	case UNK_OUT : {
		switch(y) {
		case OUT :
		case UNK_OUT:
		case UNK :    return UNK_OUT;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	default :
		//Warning: IN_TMP not considered here.
		return UNK_IN_OUT;
	}
}



// define operator or between two nodetype, assuming the two boxes associated are equal
NodeType uni(NodeType x,NodeType y) {
	switch (x) {
	case IN : {
		return IN;
		
		}
	break;
	case OUT : {
		return y;
		}
	break;
	case UNK : {
		switch(y) {
		case IN :     return IN;
		case UNK_IN:  return UNK_IN;
		case OUT :    return UNK;
		case UNK :	  return UNK;
		case UNK_OUT: return UNK_OUT;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	case UNK_IN : {
		switch(y) {
		case IN :
		case UNK_IN:
		case UNK :    return UNK_IN;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	case UNK_OUT : {
		switch(y) {
		case OUT :
		case UNK_OUT:
		case UNK :    return UNK_OUT;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	default :
		//Warning: IN_TMP not considered here.
		return UNK_IN_OUT;
	}

}

// define operator or between two nodetype, assuming box of nodetype y is subset of box of nodetype x;
NodeType uni_in(NodeType x, NodeType y) {
	switch (x) {
	case IN : {
		switch(y) {
		case IN :        return IN;
		case OUT :		 return IN;
		case UNK :		 return IN;
		case UNK_OUT:
		case UNK_IN_OUT: return UNK_IN_OUT;
		default :        return UNK_IN;
		}
	}
	break;
	case OUT : {
		switch(y) {
		case OUT :       return OUT;
		case IN :		 return UNK;
		case UNK :		 return UNK;
		case UNK_IN:
		case UNK_IN_OUT: return UNK_IN_OUT;
		default :        return UNK_OUT;
		}
	}
	break;
	case UNK : {
		switch(y) {
		return UNK;
		}
	}
	break;
	/*case UNK_IN : {
		switch(y) {
		case IN :
		case UNK_IN:
		case UNK :    return UNK_IN;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	case UNK_OUT : {
		switch(y) {
		case OUT :
		case UNK_OUT:
		case UNK :    return UNK_OUT;
		default:      return UNK_IN_OUT;
		}
	}
	break;*/
	default :
		//Warning: IN_TMP not considered here.
		return UNK_IN_OUT;
		
	}
}

// define operator and between two nodetype, assuming the two boxes associated are equal
NodeType inte(NodeType x, NodeType y) {
	switch (x) {
	case IN : {
		return y;
		}
	break;
	case OUT : {
		return OUT;
		}
	break;
	case UNK : {
		switch(y) {
		case IN :     return UNK;
		case UNK_IN:  return UNK_IN;
		case OUT :    return OUT;
		case UNK :	  return UNK;
		case UNK_OUT: return UNK_OUT;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	case UNK_IN : {
		switch(y) {
		case IN :
		case UNK_IN:
		case UNK :    return UNK_IN;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	case UNK_OUT : {
		switch(y) {
		case OUT :
		case UNK_OUT:
		case UNK :    return UNK_OUT;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	default :
		//Warning: IN_TMP not considered here.
		return UNK_IN_OUT;
	}
}


// define operator and between two nodetype, assuming box of nodetype y is subset of box of nodetype x;
NodeType inte_in(NodeType x, NodeType y) {
	switch (x) {
	case IN : {
		switch(y) {
		case IN :        return IN;
		case OUT :		 return UNK;
		case UNK :		 return UNK;
		case UNK_OUT:
		case UNK_IN_OUT: return UNK_IN_OUT;
		default :        return UNK_IN;
		}
	}
	break;
	case OUT : {
		switch(y) {
		case OUT :       return OUT;
		case IN :		 return OUT;
		case UNK :		 return OUT;
		case UNK_IN:
		case UNK_IN_OUT: return UNK_IN_OUT;
		default :        return UNK_OUT;
		}
	}
	break;
	case UNK : {
		switch(y) {
		case IN :	  return UNK;
		case UNK_IN:  return UNK_IN;
		case OUT :	  return UNK;
		case UNK_OUT: return UNK_OUT;
		case UNK:     return UNK;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	case UNK_IN : {
		switch(y) {
		case IN :
		case UNK_IN:
		case UNK :    return UNK_IN;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	case UNK_OUT : {
		switch(y) {
		case OUT :
		case UNK_OUT:
		case UNK :    return UNK_OUT;
		default:      return UNK_IN_OUT;
		}
	}
	break;
	default :
		//Warning: IN_TMP not considered here.
		return UNK_IN_OUT;
	}
}

bool certainly_contains_in(NodeType x) {
	return x==IN || x==UNK_IN || x==UNK_IN_OUT;
}

bool certainly_contains_out(NodeType x) {
	return x==OUT || x==UNK_OUT || x==UNK_IN_OUT;
}

bool possibly_contains_in(NodeType x) {
	return x!=OUT;
}

bool possibly_contains_out(NodeType x) {
	return x!=IN;
}

} // namespace ibex
