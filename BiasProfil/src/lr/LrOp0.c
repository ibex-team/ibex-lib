/************************************************************************
*  $Id:	LrOp0.c			Langzahlarithmetik, Operationen, Level 0
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
*  Version 1.1	26.06.90	Dirk Husung
*	korrigiert: LrAssign()
*  Letzte Aenderung:		Wed 14-Oct-1992 11:37:11
*************************************************************************/

#include "Portab.h"

#if defined (__ANSI__)
#include <string.h>
#else
#include <memory.h>
#include <malloc.h>
#endif

#include "Lr.h"

/************************************************************************
*  Prototypen lokaler Funktionen
*************************************************************************/

#define	CHOP 0
#define	INCR 1

#if defined (__ASSEMBLER__)
#define	LrShl	LrAsmShl
#define	LrShr	LrAsmShr
#else
static INT NEAR	LrShl (PUSHORT Data, USHORT Size, USHORT Shift);
static INT NEAR	LrShr (PUSHORT Data, USHORT Size, USHORT Shift);
#endif

static INT NEAR	LrIncrMantissa (LREAL a, SHORT Digits);
static INT NEAR	LrDecrMantissa (LREAL a, SHORT Digits);

/************************************************************************
*  Implementierung globaler Funktionen
*************************************************************************/

INT FAR	CALL LrUnderflow (LREAL	a, SHORT Sign)
 /***********************************************************************
  *  Funktion :	Behandlung eines Unterlaufs.
  *  Eingaben :	a	: Langzahl
  *		Sign	: Vorzeichen
  *		    < 0	: negativ
  *		    = 0	: ungeaendert
  *		    > 0	: positiv
  *  Ergebnis :	LR_LOSS	+ LR_ZERO
  */
{
  if (Sign == 0)
    LrSetZero (a);
  else if (Sign	< 0)
    LrSetSignedZero (a,	LR_NEGATIVE);
  else
    LrSetSignedZero (a,	LR_POSITIVE);

  return (LR_LOSS | LR_ZERO);
} /* LrUnderflow */

INT FAR	CALL LrOverflow	(LREAL a, SHORT	Sign)
 /***********************************************************************
  *  Funktion :	Behandlung eines Ueberlaufs.
  *  Eingaben :	a	: Langzahl
  *		Sign	: Vorzeichen
  *		    < 0	: negativ
  *		    = 0	: ungeaendert
  *		    > 0	: positiv
  *  Ergebnis :	LR_LOSS	+ (LR_POS_INFINITY, LR_NEG_INFINITY)
  */
{
  if (Sign < 0 || (Sign	== 0 &&	LrSign (a))) {
    LrSetNegInfinity (a); return (LR_LOSS | LR_NEG_INFINITY);
  }
  else {
    LrSetPosInfinity (a); return (LR_LOSS | LR_POS_INFINITY);
  }
} /* LrOverflow	*/

INT FAR	CALL LrRoundingOverflow	(LREAL a)
 /***********************************************************************
  *  Funktion :	Behandlung eines Uebertrags in der signifikantesten Ziffer
  *		beim Runden.
  *  Ergebnis :	LR_LOSS	+ (LR_POS_INFINITY, LR_NEG_INFINITY, LR_NORMAL)
  */
{
  if (LrExponent (a)++ == LR_MAX_EXP) {			/* Ueberlauf	*/
    if (LrSign (a)) {
      LrSetNegInfinity (a); return (LR_LOSS | LR_NEG_INFINITY);
    }
    else {
      LrSetPosInfinity (a); return (LR_LOSS | LR_POS_INFINITY);
    }
  }
  else {
    LrPMSD(a) [0] = 1;
    LrSetDigits	(a, 1);
    return (LR_LOSS | LR_NORMAL);
  }
} /* LrRoundingOverflow	*/

INT FAR	CALL LrAssign (LREAL a,	LREAL b, INT Level)
 /***********************************************************************
  *  Funktion :	Zuweisung einer	Langzahl "b": a	= b.
  *  Speicher :	Bedarf fuer "a"	in Bytes:
  *		Level =	0 && LrDigits (a) < LrDigits (b):
  *		  LrDigits (a) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 2 *	sizeof (LDIGIT)
  *		sonst:
  *		  LrDigits (a) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *  Eingaben :	Level:
  *		    = 0	: ggf. Anfuegen	von 0-Ziffern am Ende.
  *		    > 0	: Runden zur naechsten Langzahl,
  *			  Entfernen von	0-Ziffern am Ende.
  *  Ergebnis :	(LR_ZERO, LR_NORMAL, LR_POS_INFINITY, LR_NEG_INFINITY)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  */
{
  { /* Behandlung der Spezialfaelle *************************************/
    if (LrIsSpecial (b)) {
      LrSetPosSpecial (a); if (LrSign (b)) LrSetNegative (a);
      return (LrSpecial	(a) = LrSpecial	(b));
    }
  } /* Im folgenden liegt keine	spezielle Langzahl vor.	*/

  { /* Normalfall *******************************************************/
    SHORT  bLen	= LrDigits (b);
    SHORT  aLen = LrDigits (a) + ((Level == 0) * 2);  /* 2 Schutzziffern*/
    SHORT  Len	= min (aLen, bLen);
    USHORT Loss	= 0;

    memcpy ((PCHAR) a, (PCHAR) b, Len *	sizeof (LDIGIT)	+ sizeof (LREALHDR));

    if (Level) {
      PLDIGIT aMSD = LrPMSD (a);

      if (bLen > aLen) {
	LDIGIT aDig = LrPMSD (b) [aLen];

	if (aDig >= LR_ROUND) {				/* Runden	*/
	  while	(aLen--	&& (LDIGIT) ++(aMSD [aLen]) == 0)
	    ;
	  if (aLen < 0)	return (LrRoundingOverflow (a));
	  Loss = LR_LOSS;
	} /* Runden */
	else if	(aDig || bLen >	aLen+1)
	  Loss = LR_LOSS;
      }
      /* sonst:	bLen <=	aLen; bLen wurde mitkopiert */

      while (aMSD [--Len] == 0)		/* 0-Ziffern am	Ende entfernen	*/
	;
      LrSetDigits (a, Len+1);
    }
    else /* if (Level == 0) */ {
      LrSetDigits (a, aLen-2);
      if (bLen <= aLen)	{
	if ((aLen -= Len) > 0)
	  memset ((PCHAR) (LrPMSD (a) + Len), 0, aLen * sizeof (LDIGIT));
      }
      else
	Loss = LR_LOSS;
    }
    return (Loss | LR_NORMAL);
  } /* Normalfall */
} /* LrAssign */

INT FAR	CALL LrPred (LREAL a, SHORT Digits)
 /***********************************************************************
  *  Funktion :	Liefert	den Vorgaenger einer Langzahl ohne Schutzziffer.
  *  Eingaben :	a	: Langzahl
  *		Digits	: # Ziffern
  *		    = 0	: # Ziffern = LrDigits (a)
  *		    > 0	: Digits   >= LrDigits (a),
  *			  0-Ziffern am Ende werden entfernt.
  *  Ergebnis :	LR_NORMAL,
  *		LR_LOSS	+ LR_ZERO	   bei Unterlauf,
  *		LR_LOSS	+ LR_NEG_INFINITY  bei Ueberlauf.
  */
{
  { /* Behandlung der Spezialfaelle *************************************/
    if (LrIsSpecial (a))
      if (LrIsZero (a))	{
	PLDIGIT	aMSD = LrPMSD (a);

	LrSetNegative (a);
	LrExponent (a) = LR_MIN_EXP;

	if (Digits)
	  LrSetDigits (a, 1);
	else
	  memset ((PCHAR) aMSD,	0, LrDigits (a)	* sizeof (LDIGIT));
	*aMSD =	1;
	return (LR_NORMAL);
      }	/* if (LrIsZero	...) */
      else
	return (LrSpecial (a));
  } /* Im folgenden liegt keine	spezielle Langzahl vor.	*/

  { /* Normalfall *******************************************************/
    if (LrSign (a))
      return (LrIncrMantissa (a, Digits));
    else
      return (LrDecrMantissa (a, Digits));
  } /* Normalfall */
} /* LrPred */

INT FAR	CALL LrSucc (LREAL a, SHORT Digits)
 /***********************************************************************
  *  Funktion :	Liefert	den Nachfolger einer Langzahl ohne Schutzziffer.
  *  Eingaben :	a	: Langzahl
  *		Digits	: # Ziffern
  *		    = 0	: # Ziffern = LrDigits (a)
  *		    > 0	: Digits   >= LrDigits (a),
  *			  0-Ziffern am Ende werden entfernt.
  *  Ergebnis :	LR_NORMAL,
  *		LR_LOSS	+ LR_ZERO	   bei Unterlauf,
  *		LR_LOSS	+ LR_POS_INFINITY  bei Ueberlauf.
  */
{
  { /* Behandlung der Spezialfaelle *************************************/
    if (LrIsSpecial (a))
      if (LrIsZero (a))	{
	PLDIGIT	aMSD = LrPMSD (a);

	LrSetPosNormal (a);
	LrExponent (a) = LR_MIN_EXP;

	if (Digits)
	  LrSetDigits (a, 1);
	else
	  memset ((PCHAR) aMSD,	0, LrDigits (a)	* sizeof (LDIGIT));
	*aMSD =	1;
	return (LR_NORMAL);
      }	/* if (LrIsZero	...) */
      else
	return (LrSpecial (a));
  } /* Im folgenden liegt keine	spezielle Langzahl vor.	*/

  { /* Normalfall *******************************************************/
    if (LrSign (a))
      return (LrDecrMantissa (a, Digits));
    else
      return (LrIncrMantissa (a, Digits));
  } /* Normalfall */
} /* LrSucc */

INT FAR	CALL LrShift (LREAL a, LONG Shift, INT Level)
 /***********************************************************************
  *  Funktion :	Multiplikation von "a" mit 2^Shift durch bitweises Schieben.
  *  Eingaben :	a	: zu shiftende Langzahl.
  *		Shift	: # Bitpositionen fuer Linksshift (Rechtsshift mit
  *			  negativen Werten).
  *		Level	:
  *		    = 0	: Verwendung einer Schutzziffer	(kein Genauigkeits-
  *			  verlust)
  *		    > 0	: keine	Schutzziffer (Genauigkeitsverlust moeglich)
  *  Ergebnis :	(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, bei Genauigkeitsverlust.
  */
{
  if (LrIsSpecial (a)) return (LrSpecial (a));

  { /* Normalfall *******************************************************/
    PLDIGIT aMSD = LrPMSD (a);
    USHORT  aLen = LrDigits (a);
    USHORT  rc	 = 0;

    if (Level == 0) aMSD [aLen++] = 0;

    if (Shift >	0) {					/* Linksshift */
      SHORT FineShift =	((SHORT) Shift)	& 0x000F;

      if ((Shift = (Shift >> 4)	+ LrExponent (a)) > LR_MAX_EXP)
	return (LrOverflow (a, 0));
     /*
      *	 Linksshift um k Bits:	k = -k = 16-k -16 = 16-k +16; also:
      *			       <-   ->	  ->   ->    ->	  <-
      *	 k = 0	   : nichts zu tun.
      *	 k = 1..7  : Linksshift	um k Bits.
      *	 k = 8..15 : Exponent++, Rechtsshift um	16-k Bits.
      */
      if (FineShift) {

#if defined (__HIGHBYTEFIRST__)
	if ((FineShift & 7) == 0) {		/* byteweise shiften	*/
	  PCHAR  Base = (PCHAR) aMSD;
	  USHORT Size = aLen * sizeof (LDIGIT);

	  if (Base [0]) {
	    USHORT Count = --Size;
	    PCHAR  Src	 = Base	+ Count;
	    PCHAR  Dest	 = Base	+ Count;

	    if (Base [Size]) rc	= 0x0100;
	    while (Count--) *Dest-- = *--Src;
	    Base [0] = 0;
	    Shift++;
	  }
	  else {
	    USHORT Count = --Size;
	    PCHAR  Src	 = Base;
	    PCHAR  Dest	 = Base;

	    while (Count--) *Dest++ = *++Src;
	    Base [Size] = 0;
	  }
	}
	else
#endif

	if (FineShift &	8)
	  Shift	+= ((CHAR) (rc = LrShr (aMSD, aLen, 16 - FineShift)) + 1);
	else
	  Shift	+=  (CHAR) (rc = LrShl (aMSD, aLen, FineShift));
      }
      if (Shift	> LR_MAX_EXP) return (LrOverflow (a, 0));

      LrExponent (a) = (SHORT) Shift;
    } /* if (Shift > 0)	*/

    else if (Shift < 0)	{				/* Rechtsshift */
      SHORT FineShift;

      Shift	= -Shift;
      FineShift	= ((SHORT) Shift) & 0x000F;

      if ((Shift = LrExponent (a) - (Shift >> 4)) < LR_MIN_EXP)
	return (LrUnderflow (a,	0));
     /*
      *	 Rechtsshift um	k Bits:	 k = -k	= 16-k -16 = 16-k +16; also:
      *				->   <-	   <-	<-    <-   ->
      *	 k = 0	   : nichts zu tun.
      *	 k = 1..7  : Rechtsshift um k Bits.
      *	 k = 8..15 : Exponent--, Linksshift um 16-k Bits.
      */
      if (FineShift) {

#if defined (__HIGHBYTEFIRST__)
	if ((FineShift & 7) == 0) {		/* byteweise shiften	*/
	  PCHAR  Base = (PCHAR) aMSD;
	  USHORT Size = aLen * sizeof (LDIGIT);

	  if (Base [0]) {
	    USHORT Count = --Size;
	    PCHAR  Src	 = Base	+ Count;
	    PCHAR  Dest	 = Base	+ Count;

	    if (Base [Size]) rc = 0x0100;
	    while (Count--) *Dest-- = *--Src;
	    Base [0] = 0;
	  }
	  else {
	    USHORT Count = --Size;
	    PCHAR  Src	 = Base;
	    PCHAR  Dest	 = Base;

	    while (Count--) *Dest++ = *++Src;
	    Base [Size]	= 0;
	    Shift--;
	  }
	}
	else
#endif

	if (FineShift &	8)
	  Shift	+= ((CHAR) (rc = LrShl (aMSD, aLen, 16 - FineShift)) - 1);
	else
	  Shift	+=  (CHAR) (rc = LrShr (aMSD, aLen, FineShift));
      }
      if (Shift	< LR_MIN_EXP) return (LrUnderflow (a, 0));

      LrExponent (a) = (SHORT) Shift;
    } /* if (Shift < 0)	*/

    if (Level && (rc & 0xFF00))
      return (LR_LOSS |	LR_NORMAL);
    else
      return (LR_NORMAL);
  } /* Normalfall */
} /* LrShift */

/************************************************************************
*  Implementierung lokaler Funktionen
************************************************************************/

static INT NEAR	LrIncrMantissa (LREAL a, SHORT Digits)
 /***********************************************************************
  *  Funktion :	Inkrementiert die Mantisse einer Langzahl um 1 Einheit.
  *  Eingaben :	a	: Langzahl
  *		Digits	: # Ziffern
  *		    = 0	: # Ziffern = LrDigits (a)
  *		    > 0	: Digits   >= LrDigits (a),
  *			  0-Ziffern am Ende werden entfernt.
  *  Ergebnis :	LR_NORMAL,
  *		LR_LOSS	+ (LR_NEG_INFINITY, LR_POS_INFINITY)  bei Ueberlauf.
  */
{
  PLDIGIT aMSD = LrPMSD	(a);
  SHORT	  aLen = LrDigits (a);

  if (Digits > aLen) {
    memset ((PCHAR) (aMSD + aLen), 0, (Digits - aLen) * sizeof (LDIGIT));
    aMSD [Digits-1] = 1;
    LrSetDigits	(a, Digits);
  }
  else {
    SHORT Len =	aLen;

    while (aLen-- > 0 && (LDIGIT) ++(aMSD [aLen]) == 0)
      ;
    if (aLen < 0) {
      if (LrExponent (a)++ == LR_MAX_EXP)
	return (LrOverflow (a, 0));

      if (Digits)
	LrSetDigits (a,	1);
      else
	memset ((PCHAR) aMSD, 0, Len * sizeof (LDIGIT));
      *aMSD = 1;
    } /* if (aLen ...) */
    else if (Digits) {			/* 0-Ziffern am	Ende entfernen	*/
      aLen = Len;
      while (aMSD [--aLen] == 0)
	;
      LrSetDigits (a, aLen+1);
    }
  } /* else */

  return (LR_NORMAL);
} /* LrIncrMantissa */

static INT NEAR	LrDecrMantissa (LREAL a, SHORT Digits)
 /***********************************************************************
  *  Funktion :	Dekrementiert die Mantisse einer Langzahl um 1 Einheit.
  *  Eingaben :	a	: Langzahl
  *		Digits	: # Ziffern
  *		    = 0	: # Ziffern = LrDigits (a)
  *		    > 0	: Digits   >= LrDigits (a),
  *			  0-Ziffern am Ende werden entfernt.
  *  Ergebnis :	LR_NORMAL,
  *		LR_LOSS	+ LR_ZERO bei Unterlauf.
  */
{
  PLDIGIT aMSD = LrPMSD	(a);
  SHORT	  aLen = LrDigits (a);
  SHORT	  Len  = aLen;

  if (Digits > aLen) {
    memset ((PCHAR) (aMSD + aLen), 0xFF, (Digits - aLen) * sizeof (LDIGIT));
    LrSetDigits	(a, Digits);
  }

  while	((aMSD [--aLen])-- == 0)
    ;
  if (*aMSD == 0) {
    if (LrExponent (a)-- == (SHORT) LR_MIN_EXP)
      return (LrUnderflow (a, 0));

    memset ((PCHAR) aMSD, 0xFF,	Len * sizeof (LDIGIT));
  }
  else if (Digits) {			/* 0-Ziffern am	Ende entfernen	*/
    aLen = Digits;
    while (aMSD	[--aLen] == 0)
      ;
    LrSetDigits	(a, aLen+1);
  }

  return (LR_NORMAL);
} /* LrDecrMantissa */

#if !defined (__ASSEMBLER__)

static INT NEAR	LrShl (PUSHORT Data, USHORT Size, USHORT Shift)
 /***********************************************************************
  *  Funktion :	Verschiebt ein Feld von	16-Bit-Worten um 1..8 Bitpositionen
  *		nach LINKS.
  *		Bei Ueberlauf (eine der	fuehrenden "Shift" Bitpositionen ist
  *		urspruenglich 1) wird um 1 Wort	nach rechts verschoben.
  *  Eingaben :	Data	: Zeiger auf zu	verschiebendes Feld von	16-Bit-Worten
  *		Size	: # Worte ( >= 1)
  *		Shift	: # Bitpositionen (1..8), um die verschoben werden
  *			  soll.
  *  Ergebnis :	0x0000,	falls ohne Ueberlauf verschoben	werden konnte.
  *		0x0001,	falls Ueberlauf	eintrat	und die	deshalb	nach rechts
  *			herausgeschobenen letzten 16 - "Shift" Bits 0 waren.
  *		0x0101,	falls Ueberlauf	eintrat	und eines der deshalb nach
  *			rechts herausgeschobenen letzten 16 - "Shift" Bits 0
  *			war.
  */
{
  static USHORT	Mask []	= {
    0x0000, 0x8000, 0xC000, 0xE000, 0xF000, 0xF800, 0xFC00, 0xFE00, 0xFF00
  };
  INT rc;
  USHORT HiShift = Shift;
  USHORT LoShift = 16 -	Shift;
  USHORT t	 = Mask	[HiShift];

  if (*Data & t) {				/* Ueberlauf		*/
    USHORT d = Data [--Size];

    rc = (d & ~t) ? 0x0101 : 0x0001;
    d >>= LoShift;
    while (Size--) {
      t	= d; d = Data [Size];
      Data [Size+1] = (d << HiShift) | t;
      d	>>= LoShift;
    }
    *Data = d;
  }
  else {					/* KEIN	Ueberlauf	*/
    USHORT d = 0;

    while (Size--) {
      t	= d; d = Data [Size];
      Data [Size] = (d << HiShift) | t;
      d	>>= LoShift;
    }
    rc = 0x0000;
  }
  return (rc);
} /* LrShl */

static INT NEAR	LrShr (PUSHORT Data, USHORT Size, USHORT Shift)
 /***********************************************************************
  *  Funktion :	Verschiebt ein Feld von	16-Bit-Worten um 1..8 Bitpositionen
  *		nach RECHTS.
  *		Bei Unterlauf (alle fuehrenden 16 - "Shift" Bitpositionen sind
  *		urspruenglich 0) wird um 1 Wort	nach links verschoben.
  *  Eingaben :	Data	: Zeiger auf zu	verschiebendes Feld von	16-Bit-Worten
  *		Size	: # Worte ( >= 1)
  *		Shift	: # Bitpositionen (1..8), um die verschoben werden
  *			  soll.
  *  Ergebnis :	0x0000,	falls ohne Unterlauf verschoben	werden konnte und
  *			alle herausgeschobenen letzten "Shift" Bits 0 waren.
  *		0x0100,	falls ohne Unterlauf verschoben	werden konnte und
  *			eines der herausgeschobenen letzten "Shift" Bits 1 war.
  *		0x00FF,	falls Unterlauf	eintrat.
  */
{
  static USHORT	Mask []	= {
    0xFFFF, 0xFFFE, 0xFFFC, 0xFFF8, 0xFFF0, 0xFFE0, 0xFFC0, 0xFF80, 0xFF00
  };
  INT rc;
  USHORT LoShift = Shift;
  USHORT HiShift = 16 -	Shift;
  USHORT t	 = Mask	[LoShift];

  if (*Data & t) {				/* KEIN	Unterlauf	*/
    USHORT d = Data [--Size];

    rc = (d & ~t) ? 0x0100 : 0x0000;
    d >>= LoShift;
    while (Size--) {
      t	= d; d = Data [Size];
      Data [Size+1] = (d << HiShift) | t;
      d	>>= LoShift;
    }
    *Data = d;
  }
  else {					/* Unterlauf		*/
    USHORT d = 0;

    while (Size--) {
      t	= d; d = Data [Size];
      Data [Size] = (d << HiShift) | t;
      d	>>= LoShift;
    }
    rc = 0x00FF;
  }
  return (rc);
} /* LrShr */

#endif /* !__ASSEMBLER__ */
