/************************************************************************
*  $Id: LrSc.c			Langzahl-Arithemtik: Skalarprodukt
*
*  Copyright (C) 1991, 1992 Dirk Husung
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
*  Version 1.0	29.07.91	Dirk Husung
*  Letzte Aenderung:		Tue 06-Oct-1992 01:02:30
*************************************************************************/

#include "Portab.h"

#include <string.h>

#include "Lr.h"
#include "DynMem.h"

/************************************************************************
*  Konstanten- und Variablen-Definitionen
*************************************************************************/

#define ACCU_INIT		0
#define ACCU_POS		1
#define ACCU_NEG		2
#define ACCU_POS_NEG		3
#define ACCU_POS_INFINITY	4
#define ACCU_NEG_INFINITY	5
#define ACCU_NAN		6

static INT aAccuState [] = {
  LR_ZERO,		/* ACCU_INIT	     */
  LR_NORMAL,		/* ACCU_POS	     */
  LR_NORMAL,		/* ACCU_NEG	     */
  LR_NORMAL,		/* ACCU_POS_NEG      */
  LR_POS_INFINITY,	/* ACCU_POS_INFINITY */
  LR_NEG_INFINITY,	/* ACCU_NEG_INFINITY */
  LR_NAN		/* ACCU_NAN	     */
};

/************************************************************************
*  Prototypen lokaler Funktionen
*************************************************************************/

static INT NEAR _Inc (LREAL a, LREAL b, USHORT aMaxLen);

/************************************************************************
*  Implementierung globaler Funktionen
*************************************************************************/

INT FAR CALL LrScalp (INT n, PDOUBLE a, PDOUBLE b, LREAL c, INT Level)
  /**********************************************************************
  *  Funktion : Berechnung des Skalarprodukts zweier DOUBLE-Vektoren.
  *  Speicher : Bedarf fuer c in Bytes:
  *		  LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 2 * sizeof (LDIGIT)
  *		Fuer LrDigits (c) >= 266 wird das Skalarprodukt exakt
  *		berechnet.
  *  Eingaben : n	: Anzahl der DOUBLE-Elemente der Vektoren.
  *		a, b	: DOUBLE-Vektoren.
  *		c	: Langzahl-Ergebnis.
  *		Level
  *		    = 0	: Berechnung des Skalarprodukts mit 1 Schutzziffer
  *			  (intern wird mit bis zu 2 Schutzziffern gearbeitet).
  *			  LR_LOSS wird zum Funktionsergebnis addiert, wenn
  *			  das Skalarprodukt mit Schutzziffer moeglicherweise
  *			  nicht exakt ist.
  *			  Das Skalarprodukt ist	garantiert betragskleiner oder
  *			  -gleich dem exakten; im Fall von LR_LOSS liefert
  *			  Addition von 2 Einheiten zu der Schutzziffer ein
  *			  Skalarprodukt, das garantiert betragsgroesser ist
  *			  als das exakte.
  *		    > 0	: zusaetzlich: Addition	des halben moeglichen Fehlers,
  *			  Runden zur naechsten Langzahl,
  *			  Entfernen von	0-Ziffern am Ende.
  *			  Zwischen dem berechneten Skalarprodukt und dem
  *			  exakten liegt keine weitere Langzahl der geforderten
  *			  Genauigkeit. Es wird ein korrekt gerundetes Skalar-
  *			  produkt geliefert, ausgenommen das exakte Skalar-
  *			  produkt liegt "in der Mitte" zwischen zwei Lang-
  *			  zahlen.
  *  Ergebnis :	(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  */

{
 /*
  *  DOUBLE-Zahlenbereich:
  *
  *  min = 0.0 ... 0 1 * 2^-1022 = 2^-1074
  *	     1	  51 52
  *  max = 1.1 ... 1 1 * 2^+1023 < 2^+1024
  *
  *  min*min = 2^-2148 > (2^16)^-135
  *  max*max < 2^+2048 = (2^16)^+128
  */
#define ACCUDIGITS	(128+135+3)		/* 3 fuer Uebertraege	*/
#define lrPosAccu	((LREAL) PosAccu)
#define lrNegAccu	((LREAL) NegAccu)
#define lrProd		((LREAL) Prod)

  LDIGIT PosAccu [2 + ACCUDIGITS + 1];
  LDIGIT NegAccu [2 + ACCUDIGITS + 1];
  LDIGIT Prod	 [2 + 8];

  /**** lrPosAccu == lrNegAccu == 0 *************************************/

  while (--n >= 0) {
    if ((((PSHORT) a) [D3OF4] ^ ((PSHORT) b) [D3OF4]) >= 0)
      switch (ddtolr (*a, *b, lrPosAccu)) {
	case LR_ZERO: ++a; ++b; continue;
	case LR_POS_INFINITY:	goto STATE4;
	case LR_NEG_INFINITY:	goto STATE5;
	case LR_NAN:		goto STATE6;
	default:		goto STATE1;
      } /* switch */
    else
      switch (ddtolr (*a, *b, lrNegAccu)) {
	case LR_ZERO: ++a; ++b; continue;
	case LR_POS_INFINITY:	goto STATE4;
	case LR_NEG_INFINITY:	goto STATE5;
	case LR_NAN:		goto STATE6;
	default:		goto STATE2;
      } /* switch */
  } /* while */
  LrSetPosZero (c);
  return (LR_ZERO);

STATE1: /**** lrPosAccu != 0, lrNegAccu == 0 ****************************/

  while (--n >= 0) {
    switch (ddtolr (*++a, *++b, lrNegAccu)) {
      case LR_ZERO:		continue;
      case LR_POS_INFINITY:	goto STATE4;
      case LR_NEG_INFINITY:	goto STATE5;
      case LR_NAN:		goto STATE6;
    } /* switch */
    if (LrSign (lrNegAccu)) goto STATE3;

    _Inc (lrPosAccu, lrNegAccu, ACCUDIGITS);
  } /* while */
  return (LrAssign (c, lrPosAccu, Level));

STATE2: /**** lrPosAccu == 0, lrNegAccu != 0 ****************************/

  while (--n >= 0) {
    switch (ddtolr (*++a, *++b, lrPosAccu)) {
      case LR_ZERO:		continue;
      case LR_POS_INFINITY:	goto STATE4;
      case LR_NEG_INFINITY:	goto STATE5;
      case LR_NAN:		goto STATE6;
    } /* switch */
    if (LrSign (lrPosAccu) == 0) goto STATE3;

    _Inc (lrNegAccu, lrPosAccu, ACCUDIGITS);
  } /* while */
  return (LrAssign (c, lrNegAccu, Level));

STATE3: /**** lrPosAccu != 0, lrNegAccu != 0 ****************************/

  while (--n >= 0) {
    switch (ddtolr (*++a, *++b, lrProd)) {
      case LR_ZERO:		continue;
      case LR_POS_INFINITY:	goto STATE4;
      case LR_NEG_INFINITY:	goto STATE5;
      case LR_NAN:		goto STATE6;
    } /* switch */
    if (LrSign (lrProd))
      _Inc (lrNegAccu, lrProd, ACCUDIGITS);
    else
      _Inc (lrPosAccu, lrProd, ACCUDIGITS);
  }
  return (LrAdd (lrPosAccu, lrNegAccu, c, Level));

STATE4: /**** +oo *******************************************************/

  while (--n >= 0) {
    if (ddtolr (*++a, *++b, lrProd) == LR_NEG_INFINITY) goto STATE6;
  }
  LrSetPosInfinity (c); return (LR_POS_INFINITY);

STATE5: /**** -oo *******************************************************/

  while (--n >= 0) {
    if (ddtolr (*++a, *++b, lrProd) == LR_POS_INFINITY) goto STATE6;
  }
  LrSetNegInfinity (c); return (LR_NEG_INFINITY);

STATE6: /**** NaN *******************************************************/

  LrSetNaN (c); return (LR_NAN);

#undef ACCUDIGITS
#undef lrPosAccu
#undef lrNegAccu
#undef lrProd
} /* LrScalp */

PLRACCU FAR CALL LrNewAccu (USHORT nDigits)
  /**********************************************************************
  *  Funktion : Allokiert einen Langzahl-Akkumulator.
  *  Eingaben : nDigits : # Langzahlziffern des Akkumulators zur Basis 2^16.
  *			  Zusaetzlich werden 3 Schutzziffern gegen Ueberlaeufe
  *			  allokiert.
  *  Ergebnis : Zeiger auf den Akkumulator.
  *  Info     :	Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */
{
  PLRACCU pAccu = (PLRACCU) MemNew (sizeof (LRACCU));

  pAccu -> nAccuDigits	= nDigits + 3;
  pAccu -> State	= ACCU_INIT;
  pAccu -> lrPosAccu	= LrNewTemp (pAccu -> nAccuDigits, 1);
  pAccu -> lrNegAccu	= LrNewTemp (pAccu -> nAccuDigits, 1);

  return (pAccu);
} /* LrNewAccu */

VOID FAR CALL LrDeleteAccu (PLRACCU pAccu)
  /**********************************************************************
  *  Funktion : Gibt einen Langzahl-Akkumulator frei.
  */
{
  LrDeleteTemp (pAccu -> lrPosAccu, pAccu -> nAccuDigits, 1);
  LrDeleteTemp (pAccu -> lrNegAccu, pAccu -> nAccuDigits, 1);

  MemDelete (pAccu, sizeof (LRACCU));
} /* LrDeleteAccu */

INT FAR CALL LrSetAccu (PLRACCU pAccu, LREAL lr)
  /**********************************************************************
  *  Funktion : Initialisiert einen Langzahl-Akkumulator mit einer Langzahl.
  *  Ergebnis :	(LR_ZERO, LR_NORMAL, LR_POS_INFINITY, LR_NEG_INFINITY)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  *  Info     : Ggf. erfolgt Rundung zur naechsten Langzahl.
  */
{
  INT Loss;

  if (LrIsSpecial (lr)) {
    switch (LrSpecial (lr)) {
      case LR_ZERO:	    pAccu -> State = ACCU_INIT;		break;
      case LR_POS_INFINITY: pAccu -> State = ACCU_POS_INFINITY; break;
      case LR_NEG_INFINITY: pAccu -> State = ACCU_NEG_INFINITY; break;
      default:		    pAccu -> State = ACCU_NAN;		break;
    } /* switch */

    return (aAccuState [pAccu -> State]);
  }

  if (LrSign (lr) == 0) {
    LrSetPosDigits (pAccu -> lrPosAccu, pAccu -> nAccuDigits);
    Loss = LrAssign (pAccu -> lrPosAccu, lr, 1);
    pAccu -> State = ((Loss & ~LR_LOSS) == LR_POS_INFINITY) ?
       ACCU_POS_INFINITY : ACCU_POS;
  }
  else {
    LrSetPosDigits (pAccu -> lrNegAccu, pAccu -> nAccuDigits);
    Loss = LrAssign (pAccu -> lrNegAccu, lr, 1);
    pAccu -> State = ((Loss & ~LR_LOSS) == LR_NEG_INFINITY) ?
       ACCU_NEG_INFINITY : ACCU_NEG;
  }

  return (Loss);
} /* LrSetAccu */

INT FAR CALL LrIncAccu (PLRACCU pAccu, LREAL lr)
  /**********************************************************************
  *  Funktion : Addiert eine Langzahl auf einen Langzahl-Akkumulator.
  *  Ergebnis :	(LR_ZERO, LR_NORMAL, LR_POS_INFINITY, LR_NEG_INFINITY)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  *  Info     : Ggf. erfolgt Rundung zur naechsten Langzahl.
  */
{
  INT Loss;

  if (LrIsSpecial (lr)) {
    switch (LrSpecial (lr)) {
      case LR_POS_INFINITY:
	  switch (pAccu -> State) {
	    case ACCU_INIT:
	    case ACCU_POS:
	    case ACCU_NEG:
	    case ACCU_POS_NEG:	    pAccu -> State = ACCU_POS_INFINITY;	break;
	    case ACCU_NEG_INFINITY: pAccu -> State = ACCU_NAN;		break;
	  } /* switch */
	  break;

      case LR_NEG_INFINITY:
	  switch (pAccu -> State) {
	    case ACCU_INIT:
	    case ACCU_POS:
	    case ACCU_NEG:
	    case ACCU_POS_NEG:	    pAccu -> State = ACCU_NEG_INFINITY;	break;
	    case ACCU_POS_INFINITY: pAccu -> State = ACCU_NAN;		break;
	  } /* switch */
	  break;

      case LR_NAN:
	  pAccu -> State = ACCU_NAN;
	  break;
    } /* switch */
  }
  else if (LrSign (lr) == 0) {
    switch (pAccu -> State) {
      case ACCU_INIT:		/* lrPosAccu == 0, lrNegAccu == 0 */
      case ACCU_NEG:		/* lrPosAccu == 0, lrNegAccu != 0 */
	  LrSetPosDigits (pAccu -> lrPosAccu, pAccu -> nAccuDigits);
	  Loss = LrAssign (pAccu -> lrPosAccu, lr, 1);
	  if ((Loss & ~LR_LOSS) == LR_POS_INFINITY)
	    pAccu -> State = ACCU_POS_INFINITY;
	  else
	    pAccu -> State |= ACCU_POS;
	  return (Loss);

      case ACCU_POS:		/* lrPosAccu != 0, lrNegAccu == 0 */
      case ACCU_POS_NEG:	/* lrPosAccu != 0, lrNegAccu != 0 */
	  Loss = _Inc (pAccu -> lrPosAccu, lr, pAccu -> nAccuDigits);
	  if ((Loss & ~LR_LOSS) == LR_POS_INFINITY)
	    pAccu -> State = ACCU_POS_INFINITY;
	  return (Loss);
    } /* switch */
  }
  else {
    switch (pAccu -> State) {
      case ACCU_INIT:		/* lrPosAccu == 0, lrNegAccu == 0 */
      case ACCU_POS:		/* lrPosAccu != 0, lrNegAccu == 0 */
	  LrSetPosDigits (pAccu -> lrNegAccu, pAccu -> nAccuDigits);
	  Loss = LrAssign (pAccu -> lrNegAccu, lr, 1);
	  if ((Loss & ~LR_LOSS) == LR_NEG_INFINITY)
	    pAccu -> State = ACCU_NEG_INFINITY;
	  else
	    pAccu -> State |= ACCU_NEG;
	  return (Loss);

      case ACCU_NEG:		/* lrPosAccu == 0, lrNegAccu != 0 */
      case ACCU_POS_NEG:	/* lrPosAccu != 0, lrNegAccu != 0 */
	  Loss = _Inc (pAccu -> lrNegAccu, lr, pAccu -> nAccuDigits);
	  if ((Loss & ~LR_LOSS) == LR_NEG_INFINITY)
	    pAccu -> State = ACCU_NEG_INFINITY;
	  return (Loss);
    } /* switch */
  }

  return (aAccuState [pAccu -> State]);
} /* LrIncAccu */

INT FAR CALL LrDecAccu (PLRACCU pAccu, LREAL lr)
  /**********************************************************************
  *  Funktion : Subtrahiert eine Langzahl von einem Langzahl-Akkumulator.
  *  Ergebnis :	(LR_ZERO, LR_NORMAL, LR_POS_INFINITY, LR_NEG_INFINITY)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  *  Info     : Ggf. erfolgt Rundung zur naechsten Langzahl.
  */
{
  INT Loss;

  if (LrIsSpecial (lr)) {
    switch (LrSpecial (lr)) {
      case LR_POS_INFINITY:
	  switch (pAccu -> State) {
	    case ACCU_INIT:
	    case ACCU_POS:
	    case ACCU_NEG:
	    case ACCU_POS_NEG:	    pAccu -> State = ACCU_NEG_INFINITY;	break;
	    case ACCU_POS_INFINITY: pAccu -> State = ACCU_NAN;		break;
	  } /* switch */
	  break;

      case LR_NEG_INFINITY:
	  switch (pAccu -> State) {
	    case ACCU_INIT:
	    case ACCU_POS:
	    case ACCU_NEG:
	    case ACCU_POS_NEG:	    pAccu -> State = ACCU_POS_INFINITY;	break;
	    case ACCU_NEG_INFINITY: pAccu -> State = ACCU_NAN;		break;
	  } /* switch */
	  break;

      case LR_NAN:
	  pAccu -> State = ACCU_NAN;
	  break;
    } /* switch */
  }
  else if (LrSign (lr)) {
    switch (pAccu -> State) {
      case ACCU_INIT:		/* lrPosAccu == 0, lrNegAccu == 0 */
      case ACCU_NEG:		/* lrPosAccu == 0, lrNegAccu != 0 */
	  LrSetPosDigits (pAccu -> lrPosAccu, pAccu -> nAccuDigits);
	  LrSetPositive (lr);
	  Loss = LrAssign (pAccu -> lrPosAccu, lr, 1);
	  LrSetNegative (lr);
	  if ((Loss & ~LR_LOSS) == LR_POS_INFINITY)
	    pAccu -> State = ACCU_POS_INFINITY;
	  else
	    pAccu -> State |= ACCU_POS;
	  return (Loss);

      case ACCU_POS:		/* lrPosAccu != 0, lrNegAccu == 0 */
      case ACCU_POS_NEG:	/* lrPosAccu != 0, lrNegAccu != 0 */
	  Loss = _Inc (pAccu -> lrPosAccu, lr, pAccu -> nAccuDigits);
	  if ((Loss & ~LR_LOSS) == LR_POS_INFINITY)
	    pAccu -> State = ACCU_POS_INFINITY;
	  return (Loss);
    } /* switch */
  }
  else {
    switch (pAccu -> State) {
      case ACCU_INIT:		/* lrPosAccu == 0, lrNegAccu == 0 */
      case ACCU_POS:		/* lrPosAccu != 0, lrNegAccu == 0 */
	  LrSetPosDigits (pAccu -> lrNegAccu, pAccu -> nAccuDigits);
	  LrSetNegative (lr);
	  Loss = LrAssign (pAccu -> lrNegAccu, lr, 1);
	  LrSetPositive (lr);
	  if ((Loss & ~LR_LOSS) == LR_NEG_INFINITY)
	    pAccu -> State = ACCU_NEG_INFINITY;
	  else
	    pAccu -> State |= ACCU_NEG;
	  return (Loss);

      case ACCU_NEG:		/* lrPosAccu == 0, lrNegAccu != 0 */
      case ACCU_POS_NEG:	/* lrPosAccu != 0, lrNegAccu != 0 */
	  Loss = _Inc (pAccu -> lrNegAccu, lr, pAccu -> nAccuDigits);
	  if ((Loss & ~LR_LOSS) == LR_NEG_INFINITY)
	    pAccu -> State = ACCU_NEG_INFINITY;
	  return (Loss);
    } /* switch */
  }

  return (aAccuState [pAccu -> State]);
} /* LrDecAccu */

INT FAR CALL LrReadAccu (PLRACCU pAccu, LREAL res, INT Level)
  /**********************************************************************
  *  Funktion : Liest einen Akkumulator in eine Langzahl aus.
  *  Info     : Der Akkumulator verwendet eine positive und eine neagtive
  *		Teilsumme; diese Funktion berechnet die Gesamtsumme.
  *  Speicher : Bedarf fuer res in Bytes:
  *		  LrDigits (res) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 2 * sizeof (LDIGIT)
  *  Eingaben : Level
  *		    = 0	: Berechnung der Gesamtsumme mit 1 Schutzziffer
  *			  (intern wird mit bis zu 2 Schutzziffern gearbeitet).
  *			  LR_LOSS wird zum Funktionsergebnis addiert, wenn
  *			  die Summe aus positiver und negativer Teilsumme
  *			  moeglicherweise nicht exakt ist.
  *			  Unter der Voraussetzung, dass beide Teilsummen
  *			  ohne Genauigkeitsverlust berechnet wurden, ist
  *			  der gelieferte Wert garantiert betragskleiner oder
  *			  -gleich dem exakten; im Fall von LR_LOSS liefert
  *			  Addition von 2 Einheiten zu der Schutzziffer einen
  *			  Wert, der garantiert betragsgroesser ist als der
  *			  exakte.
  *		    > 0	: zusaetzlich: Addition	des halben moeglichen Fehlers,
  *			  Runden zur naechsten Langzahl,
  *			  Entfernen von	0-Ziffern am Ende.
  *  Ergebnis :	(LR_ZERO, LR_NORMAL, LR_POS_INFINITY, LR_NEG_INFINITY)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  */
{
  switch (pAccu -> State) {
    case ACCU_INIT:
	LrSetPosZero (res);
	return (LR_ZERO);
    case ACCU_POS:
	return (LrAssign (res, pAccu -> lrPosAccu, Level));
    case ACCU_NEG:
	return (LrAssign (res, pAccu -> lrNegAccu, Level));
    case ACCU_POS_NEG:
	return (LrAdd (pAccu -> lrPosAccu, pAccu -> lrNegAccu, res, Level));
    case ACCU_POS_INFINITY:
	LrSetPosInfinity (res);
	return (LR_POS_INFINITY);
    case ACCU_NEG_INFINITY:
	LrSetNegInfinity (res);
	return (LR_NEG_INFINITY);
    default:
	LrSetNaN (res);
	return (LR_NAN);
  } /* switch */
} /* LrReadAccu */

/************************************************************************
*  Implementierung lokaler Funktionen
*************************************************************************/

static INT NEAR _Inc (LREAL a, LREAL b, USHORT aMaxLen)
  /**********************************************************************
  *  Funktion : Berechnet a += b auf bis zu aMaxLen Stellen.
  *  Annahmen : a, b normale Langzahlen.
  *		a*b > 0 (gleiche Vorzeichen).
  *  Speicher : Bedarf fuer a in Bytes:
  *		  aMaxLen * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 1 * sizeof (LDIGIT)
  *  Eingaben : a, b	: Langzahlen
  *		aMaxLen : maximale Stellenzahl der Summe.
  *		Level
  *		    = 0 : Rundung zu 0.
  *		   != 0 : Rundung zur naechsten Langzahl.
  *  Ergebnis : (LR_POS_INFINITY, LR_NEG_INFINITY, LR_NORMAL)
  *		+ LR_LOSS bei Genauigkeitsverlust).
  */
{
#define INC_GUARDS	1

  PLDIGIT aMSD	= LrPMSD (a);
  PLDIGIT bMSD	= LrPMSD (b);
  INT	  Loss	= 0;
  INT	  aLen, bLen, Len;

  if (LrExponent (a) >= LrExponent (b)) {
    register USHORT ExpDiff = LrExponent (a) - LrExponent (b);
					/* a: |------------|		*/
					/* b:			|----|	*/
    if (ExpDiff >= (aMaxLen += INC_GUARDS))
      return (LR_LOSS | LR_NORMAL);	/* a sehr viel groesser als b	*/

					/* ExpDiff + bLen darstellbar!	*/
    if ((USHORT) (ExpDiff + (bLen = LrDigits (b))) <= (USHORT) (aLen = LrDigits (a))) {
      Len  = aLen;			/* a: |------------|		*/
      aLen = ExpDiff + bLen;		/* b:	  |----|		*/
    }
    else /* if (ExpDiff + bLen > aLen) */ {
      if ((USHORT) (Len = ExpDiff + bLen) > aMaxLen) {
	bLen = (Len = aMaxLen) - ExpDiff; /* > 0 ! */
	Loss = LR_LOSS;
      }
      if (ExpDiff >= (USHORT) aLen) {	/* a: |------------|		*/
					/* b:			|----|	*/
	memset ((PCHAR) (aMSD + aLen), 0, (ExpDiff - aLen) * sizeof (LDIGIT));
	memcpy ((PCHAR) (aMSD + ExpDiff), (PCHAR) bMSD, bLen * sizeof (LDIGIT));
	goto ROUND;
      }
					/* a: |------------|		*/
					/* b:	       |--------|	*/
      memcpy ((PCHAR) (aMSD + aLen), (PCHAR) (bMSD + (aLen - ExpDiff)),
	  (bLen - (aLen - ExpDiff)) * sizeof (LDIGIT));
      bLen = aLen - ExpDiff;
    }
  }
  else /* if (LrExponent (a) < LrExponent (b)) */ {
    register USHORT ExpDiff;		/* a:	    |---------| 	*/
					/* b: |--------------------|	*/
    if ((USHORT) (bLen = LrDigits (b)) > (aMaxLen += INC_GUARDS)) {
      bLen = aMaxLen;
      Loss = LR_LOSS;
    }

    ExpDiff = LrExponent (b) - LrExponent (a);
    LrExponent (a) = LrExponent (b);

    if (ExpDiff >= aMaxLen) {		/* b sehr viel groesser als a	*/
      memcpy ((PCHAR) aMSD, (PCHAR) bMSD, (Len = bLen) * sizeof (LDIGIT));
      Loss = LR_LOSS;
      goto ROUND;
    }
					/* ExpDiff + aLen darstellbar!	*/
    if ((USHORT) (ExpDiff + (aLen = LrDigits (a))) <= aMaxLen) {
      MoveFarRight (aMSD + ExpDiff, aMSD, aLen, LDIGIT);
      Len = aLen += ExpDiff;
    }
    else {
      MoveFarRight (aMSD + ExpDiff, aMSD, aMaxLen - ExpDiff, LDIGIT);
      Len = aLen = aMaxLen;
      Loss = LR_LOSS;
    }

    if ((USHORT) bLen <= ExpDiff) {	/* a:	      |----------|	*/
					/* b: |----|			*/
      memcpy ((PCHAR) aMSD, (PCHAR) bMSD, bLen * sizeof (LDIGIT));
      memset ((PCHAR) (aMSD + bLen), 0, (ExpDiff - bLen) * sizeof (LDIGIT));
      goto ROUND;
    }

    memcpy ((PCHAR) aMSD, (PCHAR) bMSD, ExpDiff * sizeof (LDIGIT));
    if (bLen <= aLen)
      aLen = bLen;
    else {				/* a:	    |--------|		*/
					/* b: |----------------------|	*/
      memcpy ((PCHAR) (aMSD + aLen), (PCHAR) (bMSD + aLen), 
          (bLen - aLen) * sizeof (LDIGIT));
      Len  = bLen;
      bLen = aLen;
    }
    bMSD += ExpDiff; bLen -= ExpDiff;
  }

#if defined (__INLINE__)

  _asm {
    push	si
    push	di
    push	bp
    push	ds

    mov		si, word ptr bLen
    mov		di, word ptr aLen
    les		bx, dword ptr bMSD	; /* b = es:[bx+si]		*/
    lds		bp, dword ptr aMSD	; /* c = ds:[bp+di]		*/

    dec 	di
    shl 	di, 1
    dec 	si
    shl		si, 1
    clc
    jz		_Inc_cont1		; /* moeglicherweise si == 0	*/
  } /* _asm */

_Inc_loop1:
  _asm {
    mov		ax, word ptr es:[bx+si]
    adc		word ptr ds:[bp+di], ax
    dec		di			; /* aendert Carry-Flag NICHT	*/
    dec		di
    dec		si
    dec		si
    jnz		_Inc_loop1
  } /* _asm */

_Inc_cont1:
  _asm {
    mov		ax, word ptr es:[bx]
    adc		word ptr ds:[bp+di], ax
    jnc		_Inc_done_no_carry	; /* Carry-Flag der letzten	*/
  } /* _asm */				; /*   Addition 		*/

_Inc_carry:
  _asm {
    sub 	di, 2
    jb		_Inc_done_carry
    inc		word ptr ds:[bp+di]
    jz		_Inc_carry
  } /* _asm */

_Inc_done_no_carry:
  _asm {
    pop 	ds
    pop 	bp
    pop 	di
    pop 	si
    jmp 	short _Inc_done
  } /* _asm */

_Inc_done_carry:
  _asm {
    pop 	ds
    pop 	bp
    pop 	di
    pop 	si
  } /* _asm */

  if (LrExponent (a)++ == LR_MAX_EXP) return (LrOverflow (a, 0));

  if ((USHORT) Len < aMaxLen)
    MoveRight (aMSD, Len++, LDIGIT);
  else {				/* Annahme: INC_GUARDS == 1	*/
    if (aMSD [aMaxLen - 1]) Loss = LR_LOSS;
    MoveRight (aMSD, aMaxLen - 1, LDIGIT);
  }
  *aMSD = 1;

_Inc_done:

#elif defined (__ASSEMBLER__)

  if (LrAsmAdd (aMSD, aLen-1, bMSD, bLen-1)) {	/* Uebertrag		*/
    if (LrExponent (a)++ == LR_MAX_EXP) return (LrOverflow (a, 0));

    if ((USHORT) Len < aMaxLen)
      MoveRight (aMSD, Len++, LDIGIT);
    else {				/* Annahme: INC_GUARDS == 1	*/
      if (aMSD [aMaxLen - 1]) Loss = LR_LOSS;
      MoveRight (aMSD, aMaxLen - 1, LDIGIT);
    }
    *aMSD = 1;
  } /* if (LrAsmAdd ...) */

#else

  {
    ULONG  t;
    USHORT Carry = 0;

    while (bLen) {
      t = (ULONG) aMSD [--aLen] + (ULONG) bMSD [--bLen] + Carry;
      aMSD [aLen] = ((USHORT *) &t)[D0OF2];
      Carry	  = ((USHORT *) &t)[D1OF2];
    }
    if (Carry) {
      while (aLen-- > 0 && (LDIGIT) ++(aMSD [aLen]) == 0)
	;
      if (aLen < 0) {
	if (LrExponent (a)++ == LR_MAX_EXP) return (LrOverflow (a, 0));

	if ((USHORT) Len < aMaxLen) {	/* GNU C braucht die Klammern!	*/
	  MoveRight (aMSD, Len++, LDIGIT);
	}
	else {				/* Annahme: INC_GUARDS == 1	*/
	  if (aMSD [aMaxLen - 1]) Loss = LR_LOSS;
	  MoveRight (aMSD, aMaxLen - 1, LDIGIT);
	}
	*aMSD = 1;
      }
    } /* if (Carry) */
  }

#endif

ROUND:
  if ((USHORT) Len > (aMaxLen -= INC_GUARDS)) {
    Len = aMaxLen;
    if (aMSD [Len] >= (LDIGIT) (LR_ROUND - 1)) {
      while (aMaxLen-- && (LDIGIT) ++(aMSD [aMaxLen]) == 0)
	;
      if (++aMaxLen == 0) return (LrRoundingOverflow (a));
      Loss = LR_LOSS;
    }
    else if (aMSD [Len])
      Loss = LR_LOSS;
  }

  while (aMSD [--Len] == 0)		/* 0-Ziffern am Ende entfernen	*/
    ;
  LrSetDigits (a, Len+1);

  return (Loss | LR_NORMAL);
} /* _Inc */
