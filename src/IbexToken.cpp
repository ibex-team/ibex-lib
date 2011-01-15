/*---------------------------------------------------------------------------------
 * 
 * A few functions for printing tokens
 * -----------------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#include "IbexToken.h"

namespace ibex {

  std::ostream& operator<<(std::ostream& os, const CmpOpType& op) {
    switch (op) {
    case LT      : return os << "<";         break; 
    case LEQ     : return os << "<=";        break; 
    case EQU     : return os << "=";         break; 
    case M_EQU   : return os << "=";         break; 
    case GEQ     : return os << ">=";        break; 
    case GT      : return os << ">";         break; 
    default      : return os << "? (code=" << ((int) op) << ")";
    }    
  }

  std::ostream& operator<<(std::ostream& os, const BinOpType& op) {
    switch (op) {
    case SUB     : 
    case M_SUB   : return os << "-";         break; 
    case ADD     : 
    case M_ADD   : return os << "+";         break; 
    case DIV     : return os << "/";         break; 
    case MAX     : return os << "max";       break; 
    case MIN     : return os << "min";       break; 
    case ARCTAN2 : return os << "arctan2";   break;
    case MUL     : 
    case M_MUL   : 
    case M_VEC   :
    case M_SCAL  : return os << "*";         break; 
    case V_DOT   : return os << ".";         break;
    default      : return os << "? (code=" << ((int) op) << ")";
    }

  }

  std::ostream& operator<<(std::ostream& os, const UnaOpType& op) {
    switch (op) {
    case MINUS   : return os << "-";         break; 
    case SIGN    : return os << "sign";      break; 
    case SQR     : return os << "sqr";       break; 
    case SQRT    : return os << "sqrt";      break;                  
    case LOG     : return os << "ln";        break;                  
    case EXP     : return os << "exp";       break; 
    case COS     : return os << "cos";       break; 
    case SIN     : return os << "sin";       break; 
    case TAN     : return os << "tan";       break; 
    case ARCCOS  : return os << "arccos";    break; 
    case ARCSIN  : return os << "arcsin";    break; 
    case ARCTAN  : return os << "arctan";    break; 
    case COSH    : return os << "cosh";      break; 
    case SINH    : return os << "sinh";      break; 
    case TANH    : return os << "tanh";      break; 
    case ARCCOSH : return os << "arccosh";   break; 
    case ARCSINH : return os << "arcsinh";   break; 
    case ARCTANH : return os << "arctanh";   break; 
    case ABS     : return os << "abs";       break;
    case M_MINUS : return os << "-";       break; 
    case M_TRANS : return os << "transp";  break; 
    default      : return os << "? (code=" << ((int) op) << ")";
    }

  }

  std::ostream& operator<<(std::ostream& os, const DomainOpType& op) {
    switch (op) {
    case INF   : return os << "inf";      break; 
    case MID   : return os << "mid";      break; 
    case SUP   : return os << "sup";      break; 
    default    : return os << "? (code=" << ((int) op) << ")";
    }

  }

  // std::ostream& operator<<(std::ostream& os, const Token& tok) {
  //  switch (tok) {
  //   case IDENT   : return os << "id";        break;
  //   case LT      : return os << "<";         break;
  //   case LEQ     : return os << "<=";        break;
  //   case EQU     : return os << "=";         break;
  //   case GEQ     : return os << ">=";        break;
  //   case GT      : return os << ">";         break;
  //   case CST     : return os << "const";     break;
  //   case SUB     : return os << "-";         break;
  //   case ADD     : return os << "+";         break;
  //   case DIV     : return os << "/";         break;
  //   case MUL     : return os << "*";         break;
  //   case MINUS   : return os << "-";         break;
  //   case POW     : return os << "^";         break;
  //   case SQR     : return os << "sqr";       break;
  //   case SQRT    : return os << "sqrt";      break;
  //   case LOG     : return os << "ln";        break;
  //   case EXP     : return os << "exp";       break;
  //   case COS     : return os << "cos";       break;
  //   case SIN     : return os << "sin";       break;
  //   case TAN     : return os << "tan";       break;
  //   case ARCCOS  : return os << "arccos";    break;
  //   case ARCSIN  : return os << "arcsin";    break;
  //   case ARCTAN  : return os << "arctan";    break;
  //   case ARCTAN2 : return os << "arctan2";    break;
  //   case COSH    : return os << "cosh";      break;
  //   case SINH    : return os << "sinh";      break;
  //   case TANH    : return os << "tanh";      break;
  //   case ARCCOSH : return os << "arccosh";   break;
  //   case ARCSINH : return os << "arcsinh";   break;
  //   case ARCTANH : return os << "arctanh";   break;

  //   }
  // }

} // end namespace
