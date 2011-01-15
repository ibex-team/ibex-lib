/************************************************************************
 *
 * Implementation of standard functions (REAL and INTERVAL)
 * --------------------------------------------------------
 *
 * Copyright (C) 1993, 1997 Olaf Knueppel
 *               2005 Christian Keil
 *
 * This file is part of PROFIL/BIAS.
 *
 * PROFIL/BIAS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 * $Id: Functions.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static char rcs_id[] = "$Id: Functions.C 478 2006-08-09 13:13:30Z keil $";

#include <Functions.h>
#include <BIAS/BiasF.h>

REAL Constant::Pi         = 0.0;
REAL Constant::TwoPi      = 0.0;
REAL Constant::PiHalf     = 0.0;
REAL Constant::PiQuarter  = 0.0;
REAL Constant::e          = 0.0;
REAL Constant::Sqrt2      = 0.0;
REAL Constant::InvSqrt2   = 0.0;
REAL Constant::Ln10       = 0.0;

Constant UsefulConstants; // constructor is called automatically

Constant::Constant ()
{
  BiasFuncInit ();  

  Constant::Pi         = BiasPi;
  Constant::TwoPi      = BiasTwoPi;
  Constant::PiHalf     = BiasPiHalf;
  Constant::PiQuarter  = BiasPiQuarter;
  Constant::e          = BiasE;
  Constant::Sqrt2      = BiasSqrt2;
  Constant::InvSqrt2   = BiasInvSqrt2;
  Constant::Ln10       = BiasLn10;
}

REAL ArSinh (REAL x)
{
  return log (x + sqrt (x * x + 1.0));
}

REAL ArCosh (REAL x)
{
  return log (x + sqrt (x * x - 1.0));
}

REAL ArTanh (REAL x)
{
  return 0.5 * log ((1.0 + x) / (1.0 - x));
}

REAL ArCoth (REAL x)
{
  return 0.5 * log ((x + 1.0) / (x - 1.0));
}

REAL Power (REAL x, INT n)
// Calculates x^n, all cases are considered
{
  INT i, absn;
  REAL y = 1.0;

  absn = (n < 0) ? (-n) : n;
  for (i = 1; i <= absn; i++) y *= x;
  if (n < 0) return 1.0 / y;
  else return y;
}

INTERVAL Sin (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasSin (Bias(r), Bias(x));
  return r;
}

INTERVAL Cos (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasCos (Bias(r), Bias(x));
  return r;
}

INTERVAL Tan (CONST INTERVAL & x)
{
  INTERVAL r;
  
  //<chabert>
  INTERVAL nb_period = (x+INTERVAL((Inf(x)<0?-1:1)*Inf(INTERVAL::Pi)/2.0,(Sup(x)<0?-1:1)*Sup(INTERVAL::Pi)/2.0))/INTERVAL::Pi;
  if ((int) Inf(nb_period)< ((int) Sup(nb_period))) return INTERVAL(BiasNegInf, BiasPosInf);

  BiasTan (Bias(r), Bias(x));
  return r;
  //</chabert>
}

INTERVAL Cot (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasCot (Bias(r), Bias(x));
  return r;
}

INTERVAL ArcSin (CONST INTERVAL & x)
{
  INTERVAL r;

  //<chabert>
  if (Sup(x)<-1.0 || Inf(x)>1.0) 
    return INTERVAL::EMPTY;
  else {
    bool minus1, plus1;
    BiasArcSin(Bias(r), Bias(INTERVAL((minus1=Inf(x)<-1)?-1:Inf(x),
				      (plus1=Sup(x)>1)?1:Sup(x))));
    return INTERVAL(minus1?Inf(-INTERVAL::Pi/2.0):Inf(r), plus1?Sup(INTERVAL::Pi/2.0):Sup(r));
  }
  //</chabert>
}

INTERVAL ArcCos (CONST INTERVAL & x)
{
  INTERVAL r;

  //<chabert>
  if (Sup(x)<-1.0 || Inf(x)>1.0) 
    return INTERVAL::EMPTY;
  else {
    bool minus1, plus1;
    BiasArcCos(Bias(r), Bias(INTERVAL((minus1=Inf(x)<=-1)?-1:Inf(x),
    				      (plus1=Sup(x)>=1)?1:Sup(x))));    
    return INTERVAL(plus1?0.0:Inf(r), minus1?Sup(INTERVAL::Pi):Sup(r));  
  }
  //</chabert>
}

INTERVAL ArcTan (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasArcTan (Bias(r), Bias(x));
  return r;
}


// ArcTan2 by Firas Khemane (feb 2010)
INTERVAL ArcTan2(const INTERVAL& a, const INTERVAL& b) {

  if (( Inf(a) > 0 && Inf(b) > 0) || (  Inf(a) > 0 &&   Sup(b) < 0 )  || ( Inf(b) < 0 && Sup(b)> 0  && Inf(a) > 0 ) )

    return ArcTan(b/a);

  else if  ( Inf(a)<=0  &&  Sup(a)>0 )
			
    return 2*ArcTan(b/a);

  else if  (Sup(a)< 0 && Inf(b)> 0)

    return ArcTan(b/a)+INTERVAL::Pi;

  else if (Sup(a) < 0 && Sup(b)< 0)

    return ArcTan(b/a)-INTERVAL::Pi;

  else if (Inf(b) < 0 && Sup(b) > 0 && Sup(a)< 0 ) {

    if (  (Inf(ArcTan(b/a)+INTERVAL::Pi)) <= (Sup(ArcTan(b/a)-INTERVAL::Pi)) )

      return 2*INTERVAL(Inf(ArcTan(b/a)+INTERVAL::Pi), Sup(ArcTan(b/a)-INTERVAL::Pi));

    else
      
      return 2*INTERVAL(Sup(ArcTan(b/a)-INTERVAL::Pi) , Inf(ArcTan(b/a)+INTERVAL::Pi) );           
  }
}

INTERVAL ArcCot (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasArcCot (Bias(r), Bias(x));
  return r;
}

INTERVAL Sinh (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasSinh (Bias(r), Bias(x));
  return r;
}

INTERVAL Cosh (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasCosh (Bias(r), Bias(x));
  return r;
}

INTERVAL Tanh (CONST INTERVAL & x)
{
  INTERVAL r;

  //<chabert>
  if (Inf(x)==BiasNegInf) 
    if (Sup(x)==BiasPosInf) return INTERVAL(-1, 1);
    else {
      BiasTanh (Bias(r), Bias(INTERVAL(Sup(x))));
      return INTERVAL(-1,Sup(r));
    }
  else
    if (Sup(x)==BiasPosInf) {
      BiasTanh (Bias(r), Bias(INTERVAL(Inf(x))));
      return INTERVAL(Inf(r),1);
    } 
    else {
      BiasTanh (Bias(r), Bias(x));
      return r;
    }
  //</chabert>
}

INTERVAL Coth (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasCoth (Bias(r), Bias(x));
  return r;
}

INTERVAL ArSinh (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasArSinh (Bias(r), Bias(x));
  return r;
}

INTERVAL ArCosh (CONST INTERVAL & x)
{
  // <chabert>
  if (Sup(x)<1.0) return INTERVAL::EMPTY;

  INTERVAL r;

  BiasArCosh (Bias(r), Bias(INTERVAL(Inf(x)<1?1:Inf(x),Sup(x))));
  return r;
  // </chabert>
}

INTERVAL ArTanh (CONST INTERVAL & x)
{  
  INTERVAL r;

  // <chabert>
  if (Sup(x)<-1.0 || Inf(x)>1.0) 
    return INTERVAL::EMPTY;
  else 
    if (Inf(x)<=-1) 
      if (Sup(x)>=1) 
	return INTERVAL(BiasNegInf, BiasPosInf);
      else {
	BiasArTanh(Bias(r), Bias(INTERVAL(Sup(x))));
	return INTERVAL(BiasNegInf,Sup(r)); 
      }
    else 
      if (Sup(x)>=1) {
	BiasArTanh(Bias(r), Bias(INTERVAL(Inf(x))));
	return INTERVAL(Inf(r), BiasPosInf);
      }
  // </chabert>


  BiasArTanh (Bias(r), Bias(x));
  return r;
}

INTERVAL ArCoth (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasArCoth (Bias(r), Bias(x));
  return r;
}

INTERVAL Exp (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasExp (Bias(r), Bias(x));
  return r;
}

INTERVAL Log (CONST INTERVAL & x)
{
  INTERVAL r;

  // <chabert>
  if (Sup(x)<=0)
    return INTERVAL::EMPTY; // REM: maybe should compute [-inf, -inf] with [_,0]
  else
    if (Inf(x)<=0) {
      BiasLog (Bias(r), Bias(INTERVAL(Sup(x))));
      return INTERVAL(BiasNegInf, Sup(r));
    }
    else {
      BiasLog (Bias(r), Bias(x));
      return r;
    }
  // </chabert>
}

INTERVAL Log10 (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasLog10 (Bias(r), Bias(x));
  return r;
}

INTERVAL IAbs (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasIAbs (Bias(r), Bias(x));
  return r;
}

INTERVAL Sqr (CONST INTERVAL & x)
{
  INTERVAL r;

  BiasSqr (Bias(r), Bias(x));
  return r;
}

INTERVAL Sqrt (CONST INTERVAL & x)
{
  INTERVAL r;

  // <chabert>
  if (Sup(x)<0) 
    return INTERVAL::EMPTY;
  else 
    if (Inf(x)<0) {
      BiasSqrt (Bias(r), Bias(INTERVAL(0.0, Sup(x))));
      return r;
    } 
    else {
      BiasSqrt (Bias(r), Bias(x));
      return r;
    }
  // </chabert>
}

INTERVAL Root (CONST INTERVAL & x, INT n)
{
  INTERVAL r;

  // <chabert>
  if (Inf(x)==0 && Sup(x)==0) return INTERVAL(0.0,0.0);

  // odd root
  if (n%2==0) {
    if (Sup(x)<0) 
      return INTERVAL::EMPTY;
    else 
      if (Inf(x)<0) {
	BiasRoot(Bias(r), Bias(INTERVAL(0.0, Sup(x))), n);
	return r;
      } else {
	BiasRoot(Bias(r), Bias(x), n);
	return r;
      }
  } else {
    // even root
    if (Inf(x)>=0) {
      BiasRoot(Bias(r), Bias(x), n);
      return r;
    }
    if (Sup(x)<=0) {
      BiasRoot(Bias(r), Bias(-x), n);
      return -r;

    }
    INTERVAL r2;
    BiasRoot(Bias(r), Bias(INTERVAL(0,-Inf(x))), n);
    BiasRoot(Bias(r2), Bias(INTERVAL(0,+Sup(x))), n);
	     
    return INTERVAL(-Sup(r), Sup(r2));
  }
  // </chabert>
}

INTERVAL Power (CONST INTERVAL & x, INT n)
{
  INTERVAL r;

  BiasPowerN (Bias(r), Bias(x), n);
  return r;
}

INTERVAL Power (CONST INTERVAL & x, CONST INTERVAL & y)
{
  INTERVAL r;

  BiasPowerI (Bias(r), Bias(x), Bias(y));
  return r;
}
