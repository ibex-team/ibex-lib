/************************************************************************
*  $Id:	LrCvt0.c		Langzahlarithmetik, Konvertierungen, Level 0
*
*  Copyright (C) 1989, 1990, 1992 Dirk Husung
*
*  This file is part of PROFIL/BIAS.
* 
*  PROFIL/BIAS is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
* 
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
* 
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
*  USA.
* 
*  Version 1.0	10.10.89	Dirk Husung
*  Version 1.1	16.05.90
*    neu	: C-Versionen von dtolr(), lrtod()
*  Version 1.2	03.10.92
*    neu	: C-Version von ddtolr()
*  Version 1.3	14.10.92
*    korrigiert : ddtolr()
*  Letzte Aenderung:		Wed 14-Oct-1992 10:38:33
************************************************************************/

#include "Portab.h"

#if defined (__ANSI__)
#include <string.h>
#else
#include <memory.h>
#include <malloc.h>
#endif

#include "Lr.h"

/************************************************************************
*  Konstanten- und Variablen-Definitionen
************************************************************************/

static LDIGIT NegSmall [] = {
  LR_NEGATIVE |	0x0001,	      1, 0x0000
};
static LDIGIT PosSmall [] = {
		0x0001,	      1, 0x0000
};
static LDIGIT NegBig []	= {
  LR_NEGATIVE |	0x0002,	      2, 0x0000, 0x0000
};
static LDIGIT PosBig []	= {
		0x0002,	      2, 0x0000, 0x0000
};

/************************************************************************
*  Implementierung globaler Funktionen
************************************************************************/

INT FAR	CALL ustolr (LREAL res,	USHORT us)
 /***********************************************************************
  *  Funktion :	Konvertierung: USHORT -> LREAL
  *  Speicher :	Bedarf fuer "res" in Bytes:
  *		  sizeof (LREALHDR) + 2	* sizeof (LDIGIT)
  *  Ergebnis :	LR_ZERO, LR_NORMAL
  *  Info     :	Es wird	die kuerzeste LREAL-Zahl geliefert, die	"us" exakt
  *		darstellt.
  */
{
  if (us) {
    memcpy ((PCHAR) res, (PCHAR) PosSmall, sizeof (PosSmall));
    LrPMSD (res) [0] = us;
    return (LR_NORMAL);
  }
  LrSetPosZero (res);
  return (LR_ZERO);
} /* ustolr */

INT FAR	CALL stolr (LREAL res, SHORT s)
 /***********************************************************************
  *  Funktion :	Konvertierung: SHORT ->	LREAL
  *  Speicher :	Bedarf fuer "res" in Bytes:
  *		  sizeof (LREALHDR) + 2	* sizeof (LDIGIT)
  *  Ergebnis :	LR_ZERO, LR_NORMAL
  *  Info     :	Es wird	die kuerzeste LREAL-Zahl geliefert, die	"s" exakt
  *		darstellt.
  */
{
  if (s	> 0) {
    memcpy ((PCHAR) res, (PCHAR) PosSmall, sizeof (PosSmall));
    LrPMSD (res) [0] = s;
    return (LR_NORMAL);
  }
  else if (s < 0) {
    memcpy ((PCHAR) res, (PCHAR) NegSmall, sizeof (NegSmall));
    LrPMSD (res) [0] = -s;
    return (LR_NORMAL);
  }
  LrSetPosZero (res);
  return (LR_ZERO);
} /* stolr */

INT FAR	CALL ultolr (LREAL res,	ULONG ul)
 /***********************************************************************
  *  Funktion :	Konvertierung: ULONG ->	LREAL
  *  Speicher :	Bedarf fuer "res" in Bytes:
  *		  sizeof (LREALHDR) + 2	* sizeof (LDIGIT)
  *  Ergebnis :	LR_ZERO, LR_NORMAL
  *  Info     :	Es wird	die kuerzeste LREAL-Zahl geliefert, die	"ul" exakt
  *		darstellt.
  */
{
  LDIGIT Digit;

  if ((Digit = ((LDIGIT	*) &ul)	[D1OF2]) != 0) {
    memcpy ((PCHAR) res, (PCHAR) PosBig, sizeof	(PosBig));
    LrPMSD (res) [0] = Digit;
    LrPMSD (res) [1] = (LDIGIT)	ul;
    return (LR_NORMAL);
  }
  else if ((Digit = (LDIGIT) ul) != 0) {
    memcpy ((PCHAR) res, (PCHAR) PosSmall, sizeof (PosSmall));
    LrPMSD (res) [0] = (LDIGIT)	ul;
    return (LR_NORMAL);
  }
  LrSetPosZero (res);
  return (LR_ZERO);
} /* ultolr */

INT FAR	CALL ltolr (LREAL res, LONG l)
 /***********************************************************************
  *  Funktion :	Konvertierung: LONG -> LREAL
  *  Speicher :	Bedarf fuer "res" in Bytes:
  *		  sizeof (LREALHDR) + 2	* sizeof (LDIGIT)
  *  Ergebnis :	LR_ZERO, LR_NORMAL
  *  Info     :	Es wird	die kuerzeste LREAL-Zahl geliefert, die	"l" exakt
  *		darstellt.
  */
{
  LDIGIT Digit;

  if (l	> 0) {
    if ((Digit = ((LDIGIT *) &l) [D1OF2]) != 0)	{
      memcpy ((PCHAR) res, (PCHAR) PosBig, sizeof (PosBig));
      LrPMSD (res) [0] = Digit;
      LrPMSD (res) [1] = (LDIGIT) l;
    }
    else {
      memcpy ((PCHAR) res, (PCHAR) PosSmall, sizeof (PosSmall));
      LrPMSD (res) [0] = (LDIGIT) l;
    }
    return (LR_NORMAL);
  }
  else if (l < 0) {
    l =	-l;
    if ((Digit = ((LDIGIT *) &l) [D1OF2]) != 0)	{
      memcpy ((PCHAR) res, (PCHAR) NegBig, sizeof (NegBig));
      LrPMSD (res) [0] = Digit;
      LrPMSD (res) [1] = (LDIGIT) l;
    }
    else {
      memcpy ((PCHAR) res, (PCHAR) NegSmall, sizeof (NegSmall));
      LrPMSD (res) [0] = (LDIGIT) l;
    }
    return (LR_NORMAL);
  }
  LrSetPosZero (res);
  return (LR_ZERO);
} /* ltolr */

#if !defined (__ASSEMBLER__)

INT FAR	CALL dtolr (LREAL res, PDOUBLE dble)
 /***********************************************************************
  *  Funktion :	Konvertierung: DOUBLE -> LREAL
  *  Speicher :	Bedarf fuer "res" in Bytes:
  *		  sizeof (LREALHDR) + 6	* sizeof (LDIGIT)
  *  Ergebnis :	LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL
  *  Info     :	Es wird	die kuerzeste LREAL-Zahl geliefert, die	"dble" exakt
  *		darstellt.
  */
{
 /*
  *  IEEE-long-real-Format:
  *
  *		 sign |	biased exponent	| significant
  *		 1 bit|	11 bits		| 52 bits
  *		 msb  |			|	    lsb
  *		 -----+-----------------+-------------------------------
  *   normal   : 0/1  |	11...10		| [1.]11...11 *2^(exponent-1023)
  *		      |	 :		|      :
  *		      |	00...01		| [1.]00...00
  *   denormal : 0/1  |	00...00		| [0.]11...11 *2^-1022
  *		      |	 :		|      :
  *		      |	00...00		| [0.]00...01
  *   0	       : 0/1  |	00...00		|     00...00
  *  +oo       :  0   |	11...11		|     00...00
  *  -oo       :  1   |	11...11		|     00...00
  *   nan      : 0/1  |	11...11		|     11...11
  *		      |	 :		|      :
  *		      |	11...11		|     00...01
  */
#define	EXPONENT	0x7FF0
#define	BIAS		0x03FF

#define	ELMT(d)		((PUSHORT) d)

  USHORT  Digit	= ELMT (dble) [D3OF4],
	  Temp	= Digit	& EXPONENT;
  PLDIGIT MSD	= LrPMSD (res);

  LrSetSign (res, Digit	& LR_NEGATIVE);
  Digit	&= 0x000F;

  memset ((PCHAR) MSD, 0, 6 * sizeof (LDIGIT));

  if (Temp == 0) {				/* 0 oder Denormal	*/
    if (Digit || ELMT (dble) [D2OF4] ||	ELMT (dble) [D1OF4]
	|| ELMT	(dble) [D0OF4])	{		/* denormalisierte Zahl	*/
     /*
      *	 Eine denormalisierte Zahl besitzt die Form (x,	y bezeichnen Bits):
      *	   0.xxyy..yy*2^-1022 =	xx.yy..yy*2^-1024 = xx.yy..yy*(2^16)^-64;
      *	 Die Mantisse der IEEE-long-real-Zahl beginnt im hoechstwertigen Wort
      *	 auf den niederwertigsten 4 Bits. Loescht man die hoeherwertigen 12
      *	 Bits dieses Wortes, ergeben 2 Rechtsshifts die	signifikanteste	Ziffer
      *	 der entsprechenden Langzahl. Die Normalisierung muss noch die ver-
      *	 schwindenden fuehrenden Ziffern beruecksichtigen.
      */
      SHORT i, k;

      k	= (MSD [i = 0] = Digit >> 2) !=	0;

      Temp = Digit; Digit = ELMT (dble)	[D2OF4];
      if ((MSD [i += k]	= (Temp	<< 14) | (Digit	>> 2)) != 0) k = 1;

      Temp = Digit; Digit = ELMT (dble)	[D1OF4];
      if ((MSD [i += k]	= (Temp	<< 14) | (Digit	>> 2)) != 0) k = 1;

      Temp = Digit; Digit = ELMT (dble)	[D0OF4];
      if ((MSD [i += k]	= (Temp	<< 14) | (Digit	>> 2)) != 0) k = 1;

      MSD [i +=	k] = Digit << 14;

      LrExponent (res) = -63 - (4 - i);
      LrSetDigits (res, i+1);
      return (LR_NORMAL);
    }
    else {
      LrSetZero	(res); return (LR_ZERO);
    }
  }
  else if (Temp	== EXPONENT) {			/* Infinity oder NaN	*/
    if (Digit || ELMT (dble) [D2OF4] ||	ELMT (dble) [D1OF4]
	|| ELMT	(dble) [D0OF4])
      LrSetNaN (res);
    else if (LrSign (res))
      LrSetNegInfinity (res);
    else
      LrSetPosInfinity (res);
    return (LrSpecial (res));
  } /* Im folgenden liegt keine	spezielle IEEE-long-real-Zahl vor. */

  {/*
    *  Die Mantisse der	IEEE-long-real-Zahl beginnt im hoechstwertigen Wort
    *  auf den niederwertigsten	4 Bits (mit vorangestelltem implicit-integer-
    *  Bit (Bit	4)). Loescht man die hoechstwertigen 12	Bits dieses Wortes und
    *  setzt Bit 4, kann das resultierende Wort	als hoechstwertige Ziffer der-
    *  jenigen Langzahl	aufgefasst werden, die die urspruengliche IEEE-long-real-
    *  Zahl multipliziert mit 2^-12 darstellt.
    *  Dementsprechend wird der	Exponent der IEEE-long-real-Zahl um +12
    *  inkrementiert.
    */
    Digit |= 0x0010;				/* implicit-integer-Bit	*/

    Temp >>= 4;	Temp +=	(USHORT) (12 - BIAS);
    LrExponent (res) = ((SHORT)	Temp) >> 4;
   /*
    *  Der Exponent der	Langzahl bezieht sich auf die Basis LR_BASE == 2^16;
    *  LrExponent (res)	ist so bestimmt, dass der Exponent um Temp & 0x000F
    *  zu klein	ist, also Temp & 0x000F	Linksshifts erforderlich sind.
    */
    if ((Temp &= 0x000F) >= 12)	{			/* Rechtsshift	*/
      SHORT Shift   = 16 - Temp;
      SHORT HiShift = Temp;

      MSD [0] =			     Digit >> Shift;

      Temp = Digit; Digit = ELMT (dble)	[D2OF4];
      MSD [1] =	(Temp << HiShift) | (Digit >> Shift);

      Temp = Digit; Digit = ELMT (dble)	[D1OF4];
      MSD [2] =	(Temp << HiShift) | (Digit >> Shift);

      Temp = Digit; Digit = ELMT (dble)	[D0OF4];
      MSD [3] =	(Temp << HiShift) | (Digit >> Shift);

      MSD [4] =	Digit << HiShift;

      LrExponent (res)++;
    }
    else if (Temp) {					/* Linksshift	*/
      SHORT Shift   = Temp;
      SHORT LoShift = 16 - Temp;

      Temp = Digit; Digit = ELMT (dble)	[D2OF4];
      MSD [0] =	(Temp << Shift)	| (Digit >> LoShift);

      Temp = Digit; Digit = ELMT (dble)	[D1OF4];
      MSD [1] =	(Temp << Shift)	| (Digit >> LoShift);

      Temp = Digit; Digit = ELMT (dble)	[D0OF4];
      MSD [2] =	(Temp << Shift)	| (Digit >> LoShift);

      MSD [3] =	Digit << Shift;
    }
    else {
      MSD [0] =	Digit;
      MSD [1] =	ELMT (dble) [D2OF4];
      MSD [2] =	ELMT (dble) [D1OF4];
      MSD [3] =	ELMT (dble) [D0OF4];
    }
  }

  { /* 0-Ziffern am Ende entfernen */
    SHORT k = 6;

    while (MSD [--k] ==	0)
      ;
    LrSetDigits	(res, k+1);
  }

  return (LR_NORMAL);

#undef EXPONENT
#undef BIAS
} /* dtolr */

INT FAR CALL ddtolr (DOUBLE a, DOUBLE b, LREAL lr)
  /**********************************************************************
  *  Funktion : Exakte Multiplikation zweier DOUBLE-Zahlen und Speicherung
  *		als Langzahl.
  *  Speicher : Bedarf fuer "lr" in Bytes:
  *		  sizeof (LREALHDR) + 8 * sizeof (LDIGIT)
  *  Ergebnis : LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL
  *  Info     : Es wird die kuerzeste LREAL-Zahl berechnet, die das Produkt
  *		exakt darstellt.
  */
{
#define NaN_oo		0x7FF0
#define lrDigit(i)	(LrPMSD (lr)[i])

  USHORT aExp, bExp, abExp;
  ULONG  aDigit;
  ULONG  Accu;
  USHORT Sign;
  INT	 nDigits, nDigits0;

  /* Behandlung von Vorzeichen und Exponent */
  aExp = ((PUSHORT) & a) [D3OF4];
  ((PUSHORT) & a) [D3OF4] &= 0x000F;

  bExp = ((PUSHORT) & b) [D3OF4];
  ((PUSHORT) & b) [D3OF4] &= 0x000F;

  Sign = (aExp ^ bExp) & 0x8000;
  aExp &= 0x7FF0;
  bExp &= 0x7FF0;

  /* Behandlung spezieller Eingabewerte */
  if (aExp == NaN_oo || bExp == NaN_oo) {
    if (aExp == NaN_oo) {
      if ( (((PULONG) & a) [D0OF2] | ((PULONG) & a) [D1OF2])
	|| ((bExp | ((PULONG) & b) [D0OF2] | ((PULONG) & b) [D1OF2]) == 0)) {
	/* a == NaN || a == oo && b == 0 */
	LrSetNaN (lr);
	return (LR_NAN);
      }
    }
    /* a != NaN, oo || a == oo, b != 0 */

    if (bExp == NaN_oo) {
      if ( (((PULONG) & b) [D0OF2] | ((PULONG) & b) [D1OF2])
	|| ((aExp | ((PULONG) & a) [D0OF2] | ((PULONG) & a) [D1OF2]) == 0)) {
	/* b == NaN || b == oo && a == 0 */
	LrSetNaN (lr);
	return (LR_NAN);
      }
    }
    /* b != NaN, oo || b == oo, a != 0 */

    if (Sign)
      LrSetNegInfinity (lr);
    else
      LrSetPosInfinity (lr);

    return (LrSpecial (lr));
  } /* im folgenden sind a und b weder NaN noch oo */

  if (aExp && bExp) {
    ((PUSHORT) & a) [D3OF4] |= 0x0010;
    ((PUSHORT) & b) [D3OF4] |= 0x0010;
  }
  else {
    if ( (aExp | ((PULONG) & a) [D0OF2] | ((PULONG) & a) [D1OF2]) == 0
      || (bExp | ((PULONG) & b) [D0OF2] | ((PULONG) & b) [D1OF2]) == 0) {
      if (Sign)
	LrSetNegZero (lr);
      else
	LrSetPosZero (lr);

      return (LrSpecial (lr));
    }
    if (aExp)
      ((PUSHORT) & a) [D3OF4] |= 0x0010;
    else
      aExp += 0x0010;
    if (bExp)
      ((PUSHORT) & b) [D3OF4] |= 0x0010;
    else
      bExp += 0x0010;
  } /* im folgenden sind a und b normale DOUBLE-Zahlen */

  abExp = (((USHORT) (aExp + bExp)) >> 4) - (0x03FF + 0x03FF - 24);

  LrExponent (lr) = ((SHORT) abExp) >> 4;

  /* a und b skalieren */
  if (abExp & 0x0008) {
    /* b mit 2^8 skalieren */
#if defined (__32BIT__)
    ((PULONG) & b)[D1OF2] <<= 8;
    ((PUCHAR) & b)[D4OF8] = ((PUCHAR) & b)[D3OF8];
    ((PULONG) & b)[D0OF2] <<= 8;
#else
    ((PUCHAR) & b)[D7OF8] = ((PUCHAR) & b)[D6OF8];
    ((PUCHAR) & b)[D6OF8] = ((PUCHAR) & b)[D5OF8];
    ((PUCHAR) & b)[D5OF8] = ((PUCHAR) & b)[D4OF8];
    ((PUCHAR) & b)[D4OF8] = ((PUCHAR) & b)[D3OF8];
    ((PUCHAR) & b)[D3OF8] = ((PUCHAR) & b)[D2OF8];
    ((PUCHAR) & b)[D2OF8] = ((PUCHAR) & b)[D1OF8];
    ((PUCHAR) & b)[D1OF8] = ((PUCHAR) & b)[D0OF8];
    ((PUCHAR) & b)[D0OF8] = 0;
#endif
  }
  if (abExp & 0x0007) {
    /* a mit 2^abExp skalieren */
    abExp &= 0x0007;
#if defined (__32BIT__)
    ((PULONG) & a)[D1OF2] <<= abExp;
    ((PUCHAR) & a)[D4OF8]  |= ((PUCHAR) & a)[D3OF8] >> (8 - abExp);
    ((PULONG) & a)[D0OF2] <<= abExp;
#else
    ((PUSHORT) & a)[D3OF4] <<= abExp;
    ((PUCHAR)  & a)[D6OF8]  |= ((PUCHAR) & a)[D5OF8] >> (8 - abExp);
    ((PUSHORT) & a)[D2OF4] <<= abExp;
    ((PUCHAR)  & a)[D4OF8]  |= ((PUCHAR) & a)[D3OF8] >> (8 - abExp);
    ((PUSHORT) & a)[D1OF4] <<= abExp;
    ((PUCHAR)  & a)[D2OF8]  |= ((PUCHAR) & a)[D1OF8] >> (8 - abExp);
    ((PUSHORT) & a)[D0OF4] <<= abExp;
#endif
  }

  /* 64x64-Bit Integer-Multiplikation */
  aDigit = (ULONG) ((PUSHORT) & a)[D0OF4];

  Accu = aDigit * ((PUSHORT) & b)[D0OF4];
  lrDigit (7) = (USHORT) (Accu);
  Accu >>= 16; Accu += aDigit * ((PUSHORT) & b)[D1OF4];
  lrDigit (6) = (USHORT) (Accu);
  Accu >>= 16; Accu += aDigit * ((PUSHORT) & b)[D2OF4];
  lrDigit (5) = (USHORT) (Accu);
  Accu >>= 16; Accu += aDigit * ((PUSHORT) & b)[D3OF4];
  lrDigit (4) = (USHORT) (Accu);
  lrDigit (3) = (USHORT) (Accu >> 16);

  aDigit = (ULONG) ((PUSHORT) & a)[D1OF4];

  Accu	= aDigit * ((PUSHORT) & b)[D0OF4] + lrDigit (6);
  lrDigit (6) = (USHORT) (Accu); Accu >>= 16;
  Accu += aDigit * ((PUSHORT) & b)[D1OF4] + lrDigit (5);
  lrDigit (5) = (USHORT) (Accu); Accu >>= 16;
  Accu += aDigit * ((PUSHORT) & b)[D2OF4] + lrDigit (4);
  lrDigit (4) = (USHORT) (Accu); Accu >>= 16;
  Accu += aDigit * ((PUSHORT) & b)[D3OF4] + lrDigit (3);
  lrDigit (3) = (USHORT) (Accu);
  lrDigit (2) = (USHORT) (Accu >> 16);

  aDigit = (ULONG) ((PUSHORT) & a)[D2OF4];

  Accu	= aDigit * ((PUSHORT) & b)[D0OF4] + lrDigit (5);
  lrDigit (5) = (USHORT) (Accu); Accu >>= 16;
  Accu += aDigit * ((PUSHORT) & b)[D1OF4] + lrDigit (4);
  lrDigit (4) = (USHORT) (Accu); Accu >>= 16;
  Accu += aDigit * ((PUSHORT) & b)[D2OF4] + lrDigit (3);
  lrDigit (3) = (USHORT) (Accu); Accu >>= 16;
  Accu += aDigit * ((PUSHORT) & b)[D3OF4] + lrDigit (2);
  lrDigit (2) = (USHORT) (Accu);
  lrDigit (1) = (USHORT) (Accu >> 16);

  aDigit = (ULONG) ((PUSHORT) & a)[D3OF4];

  Accu	= aDigit * ((PUSHORT) & b)[D0OF4] + lrDigit (4);
  lrDigit (4) = (USHORT) (Accu); Accu >>= 16;
  Accu += aDigit * ((PUSHORT) & b)[D1OF4] + lrDigit (3);
  lrDigit (3) = (USHORT) (Accu); Accu >>= 16;
  Accu += aDigit * ((PUSHORT) & b)[D2OF4] + lrDigit (2);
  lrDigit (2) = (USHORT) (Accu); Accu >>= 16;
  Accu += aDigit * ((PUSHORT) & b)[D3OF4] + lrDigit (1);
  lrDigit (1) = (USHORT) (Accu);
  lrDigit (0) = (USHORT) (Accu >> 16);

  /* abschliessende 0-Ziffern entfernen */
  nDigits = 8;
  if (lrDigit (7)) goto CHOPPED;
  nDigits--;
  if (lrDigit (6)) goto CHOPPED;
  nDigits--;
  if (lrDigit (5)) goto CHOPPED;
  nDigits--;
  if (lrDigit (4)) goto CHOPPED;
  nDigits--;
  if (lrDigit (3)) goto CHOPPED;
  nDigits--;
  if (lrDigit (2)) goto CHOPPED;
  nDigits--;
  if (lrDigit (1)) goto CHOPPED;
  nDigits--;

CHOPPED:

  /* Normalisierung (bei Denormal-Operanden) */
  if (lrDigit (0) == 0) {
    nDigits0 = 1;
    if (lrDigit (1)) goto NORMALIZE;
    nDigits0++;
    if (lrDigit (2)) goto NORMALIZE;
    nDigits0++;
    if (lrDigit (3)) goto NORMALIZE;
    nDigits0++;
    if (lrDigit (4)) goto NORMALIZE;
    nDigits0++;
    if (lrDigit (5)) goto NORMALIZE;
    nDigits0++;
    if (lrDigit (6)) goto NORMALIZE;
    nDigits0++;

NORMALIZE:

    nDigits	    -= nDigits0;
    LrExponent (lr) -= nDigits0;

    MoveFarLeft (& lrDigit (0), & lrDigit (nDigits0), nDigits, LDIGIT);
  }

  LrSetSignedDigits (lr, Sign, nDigits);

  return (LR_NORMAL);

#undef	NaN_oo
#undef	lrDigit
} /* ddtolr */

INT FAR	CALL lrtod (PDOUBLE dble, LREAL	lr, INT	Rnd)
 /***********************************************************************
  *  Funktion :	Konvertierung: LREAL ->	DOUBLE
  *  Eingaben :	Rnd	: Rundung
  *		    LR_RND_NEAR	: zur naechsten		 DOUBLE-Zahl
  *		    LR_RND_DOWN	: zur naechst-kleineren	 DOUBLE-Zahl
  *		    LR_RND_UP	: zur naechst-groesseren DOUBLE-Zahl
  *		    LR_RND_CHOP	: Rundung durch	Abschneiden
  *  Ergebnis :	(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS bei Genauigkeitsverlust
  */
{
 /*
  *  IEEE-long-real-Format:
  *
  *		 sign |	biased exponent	| significant
  *		 1 bit|	11 bits		| 52 bits
  *		 -----+-----------------+-------------------------------
  *   normal   : 0/1  |	11...10		| [1.]11...11 *2^(exponent-1023)
  *		      |	 :		|      :
  *		      |	00...01		| [1.]00...00
  *   denormal : 0/1  |	00...00		| [0.]11...11 *2^-1022
  *		      |	 :		|      :
  *		      |	00...00		| [0.]00...01
  *   0	       : 0/1  |	00...00		|     00...00
  *  +oo       :  0   |	11...11		|     00...00
  *  -oo       :  1   |	11...11		|     00...00
  *   nan      : 0/1  |	11...11		|     11...11
  *		      |	 :		|      :
  *		      |	11...11		|     00...01
  */
#define	BIAS		0x03FF

#define	ELMT(d)		((PUSHORT) d)

  static USHORT SpecialId [] = {
    0x0000,	/* ZERO		*/
    0x7FF0,	/* POS_INFINITY	*/
    0xFFF0,	/* NEG_INFINITY	*/
    0x7FFF	/* NAN		*/
  };
  static SHORT RndTable	[2][4] = {
		/* near	down up	chop */
    /* pos */	{  -1,	 0,   1,   0 },
    /* neg */	{  -1,	 1,   0,   0 }
  };
  USHORT d0, d1, d2, d3;
  USHORT Sign	= LrSign (lr);
  INT	 Exponent;
  USHORT Rest;

  { /* Behandlung der Spezialfaelle */
    if (LrIsSpecial (lr)) {
      SHORT Info = LrSpecial (lr);

      ELMT (dble) [D3OF4] = SpecialId [Info] | Sign;
      ELMT (dble) [D2OF4] =
      ELMT (dble) [D1OF4] =
      ELMT (dble) [D0OF4] = 0;

      return (Info);
    }
  } /* Im folgenden liegt keine	spezielle Langzahl vor.	*/

  {
    SHORT   Len		= LrDigits (lr);
    PLDIGIT MSD		= LrPMSD (lr);
    LDIGIT  wrk	[6];
    SHORT   Shift;
    SHORT   BigShift	= 0;

    if (Len < 5) {
      memset ((PCHAR) wrk, 0, 5	* sizeof (LDIGIT));
      memcpy ((PCHAR) wrk, (PCHAR) MSD,	Len * sizeof (LDIGIT));
      MSD = wrk;
    }

    Exponent = LrExponent (lr);
    if (Exponent >= 65)			/* 2^-16*(2^16)^65 >= 2*2^1023	*/
      raise OVERFLOW;
    else if (Exponent <= -68)		/* (2^16)^-68 <	2^-52*2^-1022	*/
      raise UNDERFLOW;

    { /* Bit-Position (0..15) des signifikantesten Bits	bestimmen;	*/
      /* Bit-Position -	4 gibt die Anzahl noetiger Rechtsshifts		*/
      static CHAR BitPos [] = {
       /* 0000 0001 0010 0011 0100 0101	0110 0111 */
	  0,   0,   1,	 1,   2,   2,	2,   2,
       /* 1000 1001 1010 1011 1100 1101	1110 1111 */
	  3,   3,   3,	 3,   3,   3,	3,   3
      };
      USHORT Digit = *MSD;

      if (Digit	& 0xFF00)
	Shift =	(Digit & 0xF000) ? BitPos [Digit >> 12]	+ 12 - 4
				 : BitPos [Digit >>  8]	+  8 - 4;
      else
	Shift =	(Digit & 0x00F0) ? BitPos [Digit >>  4]	+  4 - 4
				 : BitPos [Digit      ]	     - 4;
    }

    Exponent = (Exponent << 4) + Shift - 12;
    if (Exponent > 1023)
      raise OVERFLOW;
    else if (Exponent <	-1022) { /* denormalisierte Zahl oder Unterlauf	*/
      if (Exponent < -1075)
	raise UNDERFLOW;
      else if (Exponent	== -1075) {
	if (RndTable [Sign != 0][Rnd])
	  raise	MINREAL;
	else
	  raise	UNDERFLOW;
      }
      else {
	Shift -= (Exponent + 1022); Exponent = - BIAS;
	if (Shift > 0) {
	  BigShift = Shift & 0xFFF0; Shift &= 0x000F;
	}
      }
    }

    if (Shift >	0) {
      SHORT HiShift = 16 - Shift;

      d0 =			    (MSD [0] >>	Shift) & 0x000F;
      d1 =   (MSD [0] << HiShift) | (MSD [1] >>	Shift);
      d2 =   (MSD [1] << HiShift) | (MSD [2] >>	Shift);
      d3 =   (MSD [2] << HiShift) | (MSD [3] >>	Shift);
      Rest = (MSD [3] << HiShift) | (USHORT) (MSD [4] != 0);
    }
    else if (Shift < 0)	{
      SHORT LoShift = 16 + Shift; Shift	= -Shift;

      d0 = ((MSD [0] <<	Shift) | (MSD [1] >> LoShift)) & 0x000F;
      d1 =  (MSD [1] <<	Shift) | (MSD [2] >> LoShift);
      d2 =  (MSD [2] <<	Shift) | (MSD [3] >> LoShift);
      d3 =  (MSD [3] <<	Shift) | (MSD [4] >> LoShift);
      Rest = MSD [4] <<	Shift;
    }
    else {
      d0 = MSD [0] & 0x000F;
      d1 = MSD [1];
      d2 = MSD [2];
      d3 = MSD [3];
      Rest = MSD [4];
    }

    switch (BigShift) {
      case 0: break;
      case 1: Rest = d3	| (USHORT) (Rest != 0);
	      d3 = d2; d2 = d1;	d1 = d0; d0 = 0;
	      break;
      case 2: Rest = d2	| (USHORT) ((d3	| Rest)	!= 0);
	      d3 = d1; d2 = d0;	d1 = d0	= 0;
	      break;
      case 3: Rest = d1	| (USHORT) ((d2	| d3 | Rest) !=	0);
	      d3 = d0; d2 = d1 = d0 = 0;
	      break;
    }

    { /* Rundung */
      SHORT Rounding = RndTable	[Sign != 0][Rnd];

      if (Rest == 0 && Len > 5)
	Rest++;

      if (Rounding < 0)
	Rounding = Rest	& 0x8000;
      else if (Rounding)
	Rounding = Rest;

      if (Rounding) {
	if (++d3 == 0 && ++d2 == 0 && ++d1 == 0	&& ++d0	> 0x000F) {
	  d0 &=	0x000F;
	  if (++Exponent > 1023) raise OVERFLOW;
	}
      }
    } /* Rundung */
  }
  ELMT (dble) [D3OF4] =	Sign | ((Exponent + BIAS) << 4)	| d0;
  ELMT (dble) [D2OF4] =	d1;
  ELMT (dble) [D1OF4] =	d2;
  ELMT (dble) [D0OF4] =	d3;
  return (Rest ? (LR_LOSS | LR_NORMAL) : LR_NORMAL);

OVERFLOW:
  if (RndTable [Sign !=	0][Rnd]) {
    ELMT (dble)	[D3OF4]	= Sign | SpecialId [LR_POS_INFINITY];
    ELMT (dble)	[D2OF4]	=
    ELMT (dble)	[D1OF4]	=
    ELMT (dble)	[D0OF4]	= 0;
    return (LR_LOSS | (LR_POS_INFINITY + (Sign != 0)));
  }

/* MAXREAL: */
  ELMT (dble) [D3OF4] =	Sign | 0x7FFE;
  ELMT (dble) [D2OF4] =
  ELMT (dble) [D1OF4] =
  ELMT (dble) [D0OF4] =	0xFFFF;
  return (LR_LOSS | LR_NORMAL);

UNDERFLOW:
  if (RndTable [Sign !=	0][Rnd]	<= 0) {
    ELMT (dble)	[D3OF4]	= Sign | SpecialId [LR_ZERO];
    ELMT (dble)	[D2OF4]	=
    ELMT (dble)	[D1OF4]	=
    ELMT (dble)	[D0OF4]	= 0;
    return (LR_LOSS | LR_ZERO);
  }

MINREAL:
  ELMT (dble) [D3OF4] =	Sign;
  ELMT (dble) [D2OF4] =
  ELMT (dble) [D1OF4] =	0;
  ELMT (dble) [D0OF4] =	1;
  return (LR_LOSS | LR_NORMAL);

#undef BIAS
} /* lrtod */

#endif /* !__ASSEMBLER__ */
