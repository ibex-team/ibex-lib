/************************************************************************
*  $Id:	LrIntern.c		Langzahl-Interna
*
*  Copyright (C) 1990, 1991, 1993 Dirk Husung
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
*  Version 1.0	21.05.90	Dirk Husung
*  Version 1.1	18.07.91	Dirk Husung
*	geändert: LrRead(): Allokierung von Arbeitsspeicher;
*		  dInput(), LrInput(): Erkennung des Exponenten
*  Letzte Aenderung:		Thu 15-Apr-1993 14:01:28
************************************************************************/

#include "Portab.h"

#if defined (__STDC__) || defined (__ANSI__)
#	include <stdio.h>
#	include	<string.h>
#else
#	include <stdio.h>
#	include <memory.h>
#	include <malloc.h>
#if defined (sparc)
#	include <rand48.h>
#endif
#endif

#include "Lr.h"
#include "LrIntern.h"

#if defined (sparc)
#define rand()	 lrand48()
#endif

/************************************************************************
*  Implementierung der Funktionen
************************************************************************/

VOID dInput (PCHAR sz, PDOUBLE pd)
 /***********************************************************************
  *  Funktion :	Einlesen einer DOUBLE-Zahl im Format
  *		  {+-}? "0x1." { 0-9 A-F a-f' }+ ( {Ee} {+-}? { 0-9 }+)?
  *		  {+-}? "0x0." { 0-9 A-F a-f' }+ ( {Ee} "-1022" )?
  *  Eingaben :	sz	: Zeichenketten-Darstellung der	Zahl
  *  Ausgaben :	pd	: Zeiger auf DOUBLE-Zahl
  *  Ergebnis :	---
  *  Info     :	IEEE-DOUBLE-Format:
  *
  *		sign | biased exponent | significant
  *		1 bit| 11 bits	       | 52 bits
  *		msb  |		       |	   lsb
  *		-----+-----------------+------------
  *  normal   :	0/1  | 11...10	       | [1.]11...11 *2^(exponent-1023)
  *		     |	:	       |      :
  *		     | 00...01	       | [1.]00...00
  *  denormal :	0/1  | 00...00	       | [0.]11...11 *2^-1022
  *		     |	:	       |      :
  *		     | 00...00	       | [0.]00...01
  *  0	      :	0/1  | 00...00	       |     00...00
  *  +oo      :	 0   | 11...11	       |     00...00
  *  -oo      :	 1   | 11...11	       |     00...00
  *  nan      :	0/1  | 11...11	       |     11...11
  *		     |	:	       |      :
  *		     | 11...11	       |     00...01
  */
{
#define ExpChar1	'E'
#define ExpChar2	'e'

  static USHORT	Zero []	= {
    0x0000, 0x0000, 0x0000, 0x0000
  };
  static USHORT	Infinity [] = {
    0x0000, 0x0000, 0x0000, 0x0000
  };
  static USHORT	NaN [] = {
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
  };

  register CHAR	  c;
  register USHORT dig;
  BOOL		  Sign = 0;
  BOOL		  Denormal;
  SHORT		  Exponent;
  USHORT	  Mantissa [4];
  SHORT		  i, k;

  memset ((PCHAR) Mantissa, 0, sizeof (Mantissa));

  switch (c = *sz++) {
    case '-' : Sign++;
    case '+' : c = *sz++;
  }
  if (c	!= '0' || *sz++	!= 'x' || ((c =	*sz++) != '0' && c != '1')
      || *sz++ != '.')
    raise ERR_NAN;

  Denormal = (BOOL) (c == '0');

  dig =	0; i = 0; k = 1;
  while	((c = *sz++) !=	0) {
    if (c >= '0' && c <= '9') {
      dig <<= 4; dig +=	c - '0'; k--;
    }
    else if (*sz == '+' || *sz == '-')
      break;
    else if (c >= 'A' && c <= 'F') {
      dig <<= 4; dig +=	c - 'A'	+ 10; k--;
    }
    else if (c >= 'a' && c <= 'f') {
      dig <<= 4; dig +=	c - 'a'	+ 10; k--;
    }
    else if (c != '\'')
      break;
    if (k == 0 && i < 4) {
      Mantissa [i++] = dig; dig	= 0; k = 4;
    }
  } /* while */
  if (k	< 4 && i < 4) {
    if (i) dig <<= k*4;
    Mantissa [i] = dig;
  }

  if (Denormal && Mantissa [0] == 0)
    raise ERR_ZERO;

  if (c == ExpChar1 || c == ExpChar2) {
    BOOL ExpSign = 0;

    switch (c =	*sz++) {
      case '-' : ExpSign++;
      case '+' : c = *sz++;
    }
    if (Denormal) {
      if (ExpSign == 0 || c != '1' || *sz++ != '0' || *sz++ != '2'
	  || *sz++ != '2' || (c	= *sz) != 0)
	raise ERR_NAN;
      Exponent = 0;
    }
    else {
      Exponent = 0;
      while (c >= '0' && c <= '9') {
	if ((Exponent =	Exponent * 10 +	c - '0') > 1023)
	  if (ExpSign)
	    raise ERR_ZERO;
	  else
	    raise ERR_OVFL;
	c = *sz++;
      }
      if ((Exponent = ExpSign ?	0x03FF - Exponent : 0x03FF + Exponent) == 0)
	raise ERR_ZERO;
    }
  }
  else if (Denormal)
    Exponent = 0;
  else
    Exponent = 0x03FF;

  if (c	!= 0)
    raise ERR_NAN;

  ((PUSHORT) pd) [D3OF4] = (Sign ? 0x8000 : 0) | (Exponent << 4) | Mantissa [0];
  ((PUSHORT) pd) [D2OF4] = Mantissa [1];
  ((PUSHORT) pd) [D1OF4] = Mantissa [2];
  ((PUSHORT) pd) [D0OF4] = Mantissa [3];

  return;

ERR_ZERO:
  Zero [D3OF4] = Sign ?	0x8000 : 0x0000;
  *pd =	*((PDOUBLE) Zero);
  return;

ERR_OVFL:
  Infinity [D3OF4] = Sign ? 0xFFF0 : 0x7FF0;
  *pd =	*((PDOUBLE) Infinity);
  return;

ERR_NAN:
  NaN [D3OF4] =	Sign ? 0xFFFF :	0x7FFF;
  *pd =	*((PDOUBLE) NaN);
} /* dInput */

VOID dOutput (FILE * File, PCHAR Name, DOUBLE d)
 /***********************************************************************
  *  Funktion :	Ausgabe	einer DOUBLE-Zahl im Format
  *		+0x0.1234'5678'9ABC'DE-1022
  *		-0x1.1234'5678'9ABC'DE+0001
  *  Eingaben :	File	: Ausgabedatei
  *		Name	: Variablenname	oder "", NULL
  *		d	: Zeichen
  *  Ergebnis :	---
  *  Info     :	IEEE-DOUBLE-Format:
  *
  *		sign | biased exponent | significant
  *		1 bit| 11 bits	       | 52 bits
  *		msb  |		       |	   lsb
  *		-----+-----------------+------------
  *  normal   :	0/1  | 11...10	       | [1.]11...11 *2^(exponent-1023)
  *		     |	:	       |      :
  *		     | 00...01	       | [1.]00...00
  *  denormal :	0/1  | 00...00	       | [0.]11...11 *2^-1022
  *		     |	:	       |      :
  *		     | 00...00	       | [0.]00...01
  *  0	      :	0/1  | 00...00	       |     00...00
  *  +oo      :	 0   | 11...11	       |     00...00
  *  -oo      :	 1   | 11...11	       |     00...00
  *  nan      :	0/1  | 11...11	       |     11...11
  *		     |	:	       |      :
  *		     | 11...11	       |     00...01
  */
{
#define PlusChar	' '
#define ExpChar 	'E'

#define	ELMT(d)	((USHORT *) &d)
  USHORT Sign	  =  ELMT (d) [D3OF4] &	0x8000;
  SHORT	 Exponent =  (SHORT) ((USHORT) (ELMT (d) [D3OF4] & 0x7FF0) >> 4);
  USHORT d0	  =  ELMT (d) [D3OF4] &	0x000F;
  USHORT d1	  =  ELMT (d) [D2OF4];
  USHORT d2	  =  ELMT (d) [D1OF4];
  USHORT d3	  =  ELMT (d) [D0OF4];
#undef ELMT

  if (Name && *Name) fprintf (File, "%s = ", Name);

  if (Exponent == 0x7FF) {			/* +oo,	-oo oder NaN	*/
    if (d0 || d1 || d2 || d3)
      fprintf (File, "NaN\n");
    else
      fprintf (File, "%coo\n", Sign ? '-' : '+');
  }
  else if (Exponent)				/* normal		*/
    fprintf (File, "%c0x1.%04X\'%04X\'%04X'%01X%c%+05d\n",
	Sign ? '-' : PlusChar,
	(USHORT) ((d0 << 12) | (d1 >> 4)),
	(USHORT) ((d1 << 12) | (d2 >> 4)),
	(USHORT) ((d2 << 12) | (d3 >> 4)),
	 d3 & 0x000F,
	ExpChar,
	Exponent - 1023);
  else {					/* denormal oder 0	*/
    if (d0 || d1 || d2 || d3)
      fprintf (File, "%c0x0.%04X\'%04X\'%04X'%01X%c-1022\n",
	  Sign ? '-' : PlusChar,
	  (USHORT) ((d0 << 12) | (d1 >> 4)),
	  (USHORT) ((d1 << 12) | (d2 >> 4)),
	  (USHORT) ((d2 << 12) | (d3 >> 4)),
	   d3 & 0x000F,
	  ExpChar);
    else
      fprintf (File, "%c0\n", Sign ? '-' : '+');
  }
} /* dOutput */

VOID dRand (PDOUBLE pd)
 /***********************************************************************
  *  Funktion :	Erzeugt	eine "Zufalls"-Mantisse einer DOUBLE-Zahl.
  *  Eingaben :	pd	: DOUBLE-Zahl.
  *  Ergebnis :	---
  */
{
  ((PUSHORT) pd) [D0OF4] = (USHORT) rand() + (USHORT) rand();
  ((PUSHORT) pd) [D1OF4] = (USHORT) rand() + (USHORT) rand();
  ((PUSHORT) pd) [D2OF4] = (USHORT) rand() + (USHORT) rand();
  ((PUSHORT) pd) [D3OF4] =
 (((PUSHORT) pd) [D3OF4] & 0xFFF0) | ((USHORT) rand() & 0x000F);
} /* dRand */

INT LrInput (PCHAR sz, LREAL lr, USHORT	Level)
 /***********************************************************************
  *  Funktion :	Einlesen einer LREAL-Zahl im Format
  *		  {+-}? "0x0." { 0-9 A-F a-f' }+ ( {Ee} {+-}? { 0-9}+ )?
  *  Speicher :	Bedarf fuer "lr" in Bytes:
  *		  LrDigits (lr)	* sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 1 *	sizeof (LDIGIT)
  *  Eingaben :	sz	: 0-terminierte	Zeichenkette
  *		lr	: LREAL-Zahl
  *		Level	:
  *		    = 0	: Einlesen von bis zu LrDigits (lr) + 1	LREAL-Ziffer
  *		    > 0	: zusaetzlich: Runden zur naechsten Langzahl,
  *			  Entfernen von	0-Ziffern am Ende.
  *  Ergebnis :	LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NORMAL,
  *		LR_NORMAL + LR_LOSS, falls zu viele Ziffern angegeben wurden.
  */
{
#define ExpChar1	'E'
#define ExpChar2	'e'

  register CHAR	  c;
  register USHORT dig;
  BOOL		  ExpSign  = 0;
  LONG		  Exponent = 0L;

  PLDIGIT	  msd	   = LrPMSD (lr);
  SHORT		  Len;
  USHORT	  Loss	   = 0;
  SHORT		  k, l;

  Len =	LrDigits (lr) +	1;
  memset ((PCHAR) msd, 0, Len *	sizeof (LDIGIT));

  LrSetPosNormal (lr);
  switch (c = *sz++) {
    case '-' : LrSetNegative (lr);
    case '+' : c = *sz++;
  }
  if (c	!= '0' || *sz++	!= 'x' || *sz++	!= '0' || *sz++	!= '.')
    raise ERR_NAN;

  dig =	0; k = 4; l = 0;
  while	((c = *sz++) !=	0) {
    if (c >= '0' && c <= '9') {
      dig <<= 4; dig +=	c - '0'; k--;
    }
    else if (*sz == '+' || *sz == '-')
      break;
    else if (c >= 'A' && c <= 'F') {
      dig <<= 4; dig +=	c - 'A'	+ 10; k--;
    }
    else if (c >= 'a' && c <= 'f') {
      dig <<= 4; dig +=	c - 'a'	+ 10; k--;
    }
    else if (c != '\'')
      break;
    if (k == 0)	{
      if (l < Len)
	msd [l++] = dig;
      else if (dig)
	Loss = LR_LOSS;
      dig = 0; k = 4;
    }
  } /* while */
  if (k	< 4)
    if (l < Len)
      msd [l] =	dig << k*4;
    else if (dig)
      Loss = LR_LOSS;

  if (LrPMSD (lr)[0] ==	0)
    raise ERR_ZERO;

  if (c == ExpChar1 || c == ExpChar2) {
    switch (c =	*sz++) {
      case '-' : ExpSign++;
      case '+' : c = *sz++;
    }

    while (c >=	'0' && c <= '9') {
      if ((Exponent = Exponent * 10 + c	- '0') > LR_MAX_EXP)
	if (ExpSign)
	  raise	ERR_ZERO;
	else
	  raise	ERR_OVFL;
      c	= *sz++;
    }
  }
  if (c	!= 0)
    raise ERR_NAN;

  LrExponent (lr) = (SHORT) (ExpSign ? -Exponent : Exponent);

  if (Level) {
    if (msd [Len] >= LR_ROUND) {
      l	= Len;
      while (l-- && (LDIGIT) ++(msd [l]) == 0)
	;
      if (l < 0) {
	if (LrExponent (lr)++ == LR_MAX_EXP)
	  raise	ERR_OVFL;
	else {
	  *msd = 1;
	  LrSetDigits (lr, 1);
	  return (LR_LOSS | LR_NORMAL);
	}
      }	/* if (Len ...)	*/
    }
    else if (msd [Len])
      Loss = LR_LOSS;

    Len--;				/* Schutzziffer	uebergehen	*/
    while (msd [--Len] == 0)
      ;
    LrSetDigits	(lr, Len+1);
  }
  return (Loss | LR_NORMAL);

ERR_ZERO:
  LrSetZero (lr);
  return (LR_ZERO);

ERR_OVFL:
  if (LrSign (lr)) {
    LrSetNegInfinity (lr); return (LR_NEG_INFINITY);
  }
  else {
    LrSetPosInfinity (lr); return (LR_POS_INFINITY);
  }

ERR_NAN:
  LrSetNaN (lr);
  return (LR_NAN);
} /* LrInput */

VOID LrOutput (FILE * File, PCHAR Name,	LREAL lr, USHORT Guards)
 /***********************************************************************
  *  Funktion :	Ausgabe	einer LREAL-Zahl im Format
  *		  -0x0.1234'5678'9ABC'DEF0'E+00001
  *  Eingaben :	File	: Ausgabedatei
  *		Name	: Ausgabetext "Name = "	oder "", NULL
  *		lr	: LREAL-Zahl
  *		Guards	: # auszugebender Schutzziffern
  *  Ergebnis :	---
  */
{
#define PlusChar	' '
#define ExpChar 	'E'

  if (Name && *Name) fprintf (File, "%s = ", Name);

  if (LrIsSpecial (lr))	{
    switch (LrSpecial (lr)) {
      case LR_ZERO	   : fprintf (File, "%c0\n", LrSign (lr) ? '-' : '+');
			     break;
      case LR_POS_INFINITY : fputs ("+oo\n", File); break;
      case LR_NEG_INFINITY : fputs ("-oo\n", File); break;
      case LR_NAN	   : fputs ("NaN\n", File); break;
    }
  }
  else {
    PUSHORT msd	   = LrPMSD (lr);
    USHORT  Digits = LrDigits (lr);

    fprintf (File, "%c0x0.", LrSign (lr) ? '-' : PlusChar);
    while (Digits--)
      fprintf (File, "%04X\'", *msd++);
    if (Guards)	{
      fputc ('\'', File);
      do {
	fprintf	(File, "%04X\'", *msd++);
      }	while (--Guards);
    }
    fprintf (File, "%c%+06d\n", ExpChar, LrExponent (lr));
  }
} /* LrOutput */

INT LrRead (PCHAR sz, LREAL lr, SHORT Level)
 /***********************************************************************
  *  Funktion : Konvertierung Dezimalzahl-Zeichenkette -> Langzahl,
  *  Eingaben : sz	: Dezimalzahl-Zeichenkette.
  *		lr	: Langzahl.
  *		Level:
  *		    = 0	: Berechnung der Langzahl mit einer Schutzziffer.
  *			  LR_LOSS wird zum Funktionsergebnis addiert, wenn die
  *			  Langzahl mit Schutzziffer moeglicherweise nicht exakt
  *			  ist.
  *			  Die berechnete Langzahl ist garantiert betragskleiner
  *			  oder gleich der exakten; im Fall von LR_LOSS liefert
  *			  Addition von 4 Einheiten zu der 1. Schutzziffer ein
  *			  Ergebnis, das	garantiert betragsgroesser ist als das
  *			  exakte.
  *		    > 0	: zusaetzlich: Addition	des halben moeglichen Fehlers,
  *			  Runden zur naechsten Langzahl,
  *			  Entfernen von	0-Ziffern am Ende.
  *			  Zwischen der berechneten Langzahl und	der exakten
  *			  liegt	keine weitere Langzahl der geforderten
  *			  Genauigkeit. Es wird eine korrekt gerundete Langzahl
  *			  geliefert, ausgenommen die exakte Langzahl liegt "in
  *			  der Mitte" zwischen zwei Langzahlen.
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  *  Info     : Bei zu wenig Speicher erfolgt Programmabbruch (exit-Code -1).
  */
{
  PCHAR  szOut;
  PVOID  wrk1,
	 wrk2,
	 wrk3,
	 wrk4;
  SHORT  rc;

  ULONG  uu	= (ULONG) LrDigits (lr) * 78913L + 364804L;
  USHORT Size1	= ((SHORT *) &uu) [D1OF2] * sizeof (USHORT) + sizeof (LONG);
  USHORT Size2 = (LrDigits (lr) + 16) * sizeof (LDIGIT) + sizeof (LREALHDR);
  USHORT Size3 = (LrDigits (lr) + 32) * sizeof (LDIGIT) + sizeof (LREALHDR);

  if (	 (wrk1 = (PVOID) malloc (Size1)) == NULL
      || (wrk2 = (PVOID) malloc (Size2)) == NULL
      || (wrk3 = (PVOID) malloc (Size2)) == NULL
      || (wrk4 = (PVOID) malloc (Size3)) == NULL)
    exit (-1);

  rc = lrsztolr (sz, &szOut, lr, wrk1, wrk2, wrk3, wrk4, Level);

  free ((PCHAR) wrk1);
  free ((PCHAR) wrk2);
  free ((PCHAR) wrk3);
  free ((PCHAR) wrk4);

  return (rc);
} /* LrRead */

VOID LrWrite (FILE * File, PCHAR Name, LREAL lr,
  USHORT FracDigits, USHORT ExpDigits, INT Rnd)
 /***********************************************************************
  *  Funktion : Ausgabe einer Langzahl in Exponentialdarstellung zur Basis 10.
  *  Eingaben : File	   : Ausgabedatei.
  *		Name	   : Ausgabetext "Name = " oder "", NULL
  *		lr	   : Langzahl.
  *		FracDigNum : # Nachkommastellen.
  *		ExpDigNum  : # Exponentenstellen.
  *		Rnd	   : Rundungsmodus:
  *			     LR_RND_NEAR, LR_RND_DOWN, LR_RND_UP, LR_RND_CHOP.
  *  Ergebnis : ---
  *  Info     : Bei zu wenig Speicher erfolgt Programmabbruch (exit-Code -1).
  */
{
  PCHAR   res;
  PVOID   wrk1,
	  wrk2,
	  wrk3;
  USHORT  Len;
  USHORT  Size1;
  USHORT  Size2;
  LONG	  Exp10;

  if (FracDigits < 1) FracDigits = 1;
  if (ExpDigits  < 1)
    ExpDigits = 1;
  else if (ExpDigits > 6)
    ExpDigits = 6;

  Len	= (USHORT) (( ((((LONG) FracDigits + 9) >> 2) << 16) + 83845L) / 78913L);
  Size1 = (Len + 14) * sizeof (LDIGIT) + sizeof (LREALHDR);
  Size2 = (Len + 33) * sizeof (LDIGIT);

  if (   (res  = (PCHAR) malloc (FracDigits + 13)) == NULL
      || (wrk1 = (PVOID) malloc (Size1)) == NULL
      || (wrk2 = (PVOID) malloc (Size1)) == NULL
      || (wrk3 = (PVOID) malloc (Size2)) == NULL)
    exit (-1);

  if (Name && *Name) fprintf (File, "%s = ", Name);

  fprintf (File, "%s\n",
      lrtosze (lr, res, FracDigits, ExpDigits, Rnd, wrk1, wrk2, wrk3, &Exp10));

  free ((PCHAR) res);
  free ((PCHAR) wrk1);
  free ((PCHAR) wrk2);
  free ((PCHAR) wrk3);
} /* LrWrite */

VOID LrRand (LREAL lr)
 /***********************************************************************
  *  Funktion :	Erzeugt	eine "Zufalls"-Langzahl	gegebener Laenge.
  *  Eingaben :	lr	: Langzahl
  *  Ergebnis :	---
  */
{
  LrSetPosNormal (lr);
  if (rand() & 1)
    LrSetNegative (lr);
  if (rand() ==	0)
    LrSetZero (lr);
  else {
    PLDIGIT msd	= LrPMSD (lr);
    USHORT  Len	= LrDigits (lr);

    LrExponent (lr) = (SHORT) (rand() &	0x0007)	* ((rand() & 1)	? -1 : 1);
    while ((*msd = (LDIGIT) rand() + (LDIGIT) rand()) == 0)
      ;
    while (--Len)
      *++msd = (LDIGIT)	rand() + (LDIGIT) rand();
  }
} /* LrRand */
