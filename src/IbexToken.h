/*---------------------------------------------------------------------------------
 * 
 * Declaration of the token
 * ------------------------
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

#ifndef _IBEX_TOKEN_H
#define _IBEX_TOKEN_H

#include <iostream>

namespace ibex {

typedef enum { 
  LT      = 0,  
  LEQ     , 
  EQU     , 
  GEQ     , 
  GT      ,
  M_EQU   ,

  ___TOK__N1___ 

} CmpOpType;

typedef enum { 
  ADD     = ___TOK__N1___, 
  SUB     ,
  MUL     ,
  DIV     ,
  MIN     ,
  MAX     ,
  ARCTAN2 ,
  M_ADD   ,
  M_SUB   ,
  M_MUL   ,
  M_SCAL  , // scalar-matrix  multiplication
  M_VEC   , // matrix-vector  multiplication
  V_DOT   , // dot product
    
  ___TOK__N2___ 

} BinOpType;

typedef enum { 
  SIGN    = ___TOK__N2___,
  MINUS   , 
  SQR     , 
  SQRT    , 
  LOG     , 
  EXP     , 
  COS     , 
  SIN     , 
  TAN     , 
  ARCCOS  , 
  ARCSIN  , 
  ARCTAN  , 
  COSH    , 
  SINH    , 
  TANH    , 
  ARCCOSH , 
  ARCSINH , 
  ARCTANH , 
  ABS     , 
  M_MINUS , 
  M_TRANS , // transposition
  
  ___TOK__N3___

} UnaOpType;

typedef enum {
  INF     = ___TOK__N3___,
  MID     , 
  SUP     , 
  M_INF   , 
  M_MID   , 
  M_SUP   ,

  ___TOK__N4___

} DomainOpType;



/* extra tokens */


typedef enum {
  CST     = ___TOK__N4___,
  SYMBOL  ,
  POW     , 
  APPLY   ,
  /*--------------*/
  M_CST   ,
  M_SYMBOL,
  M_APPLY ,

  ___TOK__N5___

} SpecialToken;

std::ostream& operator<<(std::ostream& os, const CmpOpType&);

std::ostream& operator<<(std::ostream& os, const BinOpType&);

std::ostream& operator<<(std::ostream& os, const UnaOpType&);

std::ostream& operator<<(std::ostream& os, const DomainOpType&);

} // end namespace

#endif
