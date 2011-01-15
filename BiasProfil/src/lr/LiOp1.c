/************************************************************************
*  $Id: LiOp1.c 		Intervall-Langzahlarithmetik
*				Grundrechenarten und Hilfsroutinen
*
*  Copyright (C) 1990, 1996 Olaf Knueppel
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
*  Version 1.0	27.08.90	Olaf Knueppel
*  Letzte Aenderung:		Mon 4-Mar-1996 12:58:46
*  Korrektur: mul_bnd_diam: 2. do_round: cc statt c
*************************************************************************/

#include "Portab.h"

#if defined (__ANSI__)
#include <string.h>
#else
#include <memory.h>
#endif

#include "Li.h"

/************************************************************************
*
*  Intervall-Langzahlarithmetik
*  ----------------------------
*
*  Es werden die Langzahl-Punktarithmetik-Routinen (siehe "Lr_op1.c")
*  verwendet. Daher wurde die Darstellungsart der Intervalle so gewaehlt,
*  dass moeglichst wenig zeitaufwendige Konvertierungen noetig sind. Ebenso
*  wie bei den Punktarithmetik-Routinen darf auch bei den Intervall-Routinen
*  das Ergebnis nicht im gleichen Speicherbereich wie die Operanden liegen!
*  Es ist darauf zu achten, dass fuer das Ergebnis genuegend Speicherplatz
*  (es werden fuer eine Langzahl Schutzziffern benoetigt) zur Verfuegung
*  gestellt wird.
*
*  Fuer die Darstellung der Intervall-Langzahlen werden 2 verschiedene
*  Darstellungsarten verwendet:
*
*  1. (0-Grenze, Durchmesser): (a, d) =
*    | d_len | a_info | a_exp | a[1] |...| a[a_len] | d[1] |...| d[d_len] |
*      word    word	word	word	   word       word	 word
*
*    "a_len" ergibt sich aus LrDigits (a).
*    Die von Null weiter entfernte Grenze ergibt sich durch rechtsbuendige
*    Addition der Ziffern von "d" zu "a":
*      b = | a_info | a_exp | a[1] | a[2] |...| a[a_len] |
*				 + | d[1] |...| d[d_len] |
*    Damit lauten die Grenzen fuer das Intervall,
*      falls a > 0: [	      a, a+d]
*      falls a < 0: [-(|a|+|d|), a  ]
*
*    Voraussetzungen:
*      - "a" darf keine spezielle Langzahl (z.B. Null) sein.
*      - d_len <= a_len
*      - bei der Addition darf kein Uebertrag aus der hoechstwertigen
*	 Ziffer von a entstehen, d.h. die Addition darf keine Exponenten-
*	 aenderung bewirken.
*
*    Konsequenzen:
*      - ein Intervall in dieser Darstellung kann die Null nicht enthalten
*      - fuer bestimmte Intervalle (a[1] = a[2] = ... = 0xFFFF) kann der
*	 Durchmesser nur sehr kleine Werte annehmen.
*      - die Berechnung der von Null entfernten Grenze ist einfach und
*	 schnell durchzufuehren.
*
*  2. (untere Grenze, obere Grenze): (a, b) =
*    |0|a_info|a_exp | a[1]|...|a[a_len] | b_info|b_exp | b[1]|...|b[b_len]|
*	word   word    word	word	   word   word   word    word
*
*    "a_len" und "b_len" ergeben sich aus "a_info" bzw. "b_info".
*
*    Voraussetzungen:
*      - a < b (d.h. "a" ist immer die untere Grenze)
*
*    Konsequenzen:
*      - diese Darstellung wird fuer alle Intervalle gewaehlt, die spezielle
*	 Langzahlen (insbesondere die Null) enthalten, sowie fuer Intervalle
*	 mit Grenzen stark unterschiedlicher Groessenordnung
*
*  Die Darstellung einer Intervall-Langzahl wird damit im ersten Feld
*  ("d_len") festgelegt (0 = Darstellung in (untere Grenze, obere Grenze),
*  ansonsten Darstellung in (0-Grenze, Durchmesser).
*
*  Alle Routinen berechnen zunaechst das Ergebnis in der 2. Darstellungsart
*  (untere Grenze, obere Grenze) und waehlen dann die jeweils speicheroptimale
*  Darstellungsart. Dabei werden nachfolgende Nullen einer Zahl entfernt,
*  d.h. aus "1.20000" wird "1.2". Die aufrufende Routine hat gegebenenfalls
*  fuer die Freigabe des nicht mehr benoetigten Speichers zu sorgen.
*  Die maximale Genauigkeit des Ergebnisses wird durch die aufrufende
*  Routine im 1. Info-Feld des Ergebnisses festgelegt.
*
*  Als Ergebnis geben alle Routinen sowohl das Ergebnis der Berechnung der
*  unteren als auch der oberen Grenze zurueck. Dabei gilt:
*  Ergebnis = (Ergebnis der oberen Grenze)<<8 + (Ergebnis der unteren Grenze)
*
*************************************************************************/

/************************************************************************
*  Konstanten
*************************************************************************/

#define POS	0
#define NEG	1

/************************************************************************
*  Makro-Funktionen
*************************************************************************/

#define chk_inner(c, rc) \
	{							\
	  if (LrCmp (LiPlr (c), LiUpperBound (c)) == LR_GT)	\
	    rc = set_nan_nan (c);				\
	}

/************************************************************************
*  Prototypen lokaler Funktionen
*************************************************************************/

static INT  NEAR mul_ival_ival (LREAL a, LREAL aa, LREAL b, LREAL bb,
				 LIREAL ci, LREAL wrk, SHORT inner);
static INT  NEAR mul_ival_bnd	(LREAL a, LREAL aa, LREAL b,
				 LIREAL ci, SHORT inner);
static INT  NEAR mul_ival_diam (LREAL a, LREAL aa, LIREAL b,
				 LIREAL ci, SHORT inner);
static INT  NEAR mul_bnd_bnd	(LREAL a, LREAL aa, LREAL b, LREAL bb,
				 LIREAL ci, SHORT inner);
static INT  NEAR mul_bnd_diam	(INT sign, LREAL a, LREAL aa, LIREAL b,
				 LIREAL ci, SHORT inner);
static INT  NEAR mul_diam_diam (INT sign, LIREAL a, LIREAL b,
				 LIREAL ci, SHORT inner);

static INT  NEAR set_nan_nan	(LIREAL ci);
static INT  NEAR set_inf_inf	(LIREAL ci);

static INT  NEAR div_bnd_ival	(LREAL a, LIREAL ci, SHORT inner);
static INT  NEAR div_ival_bnd	(LREAL a, LREAL aa, LREAL b,
				 LIREAL ci, PUSHORT wrk, SHORT inner);
static INT  NEAR div_ival_diam (LREAL a, LREAL aa, LIREAL b,
				 LIREAL ci, PUSHORT wrk, SHORT inner);
static INT  NEAR div_bnd_bnd	(LREAL a, LREAL aa, LREAL b, LREAL bb,
				 LIREAL ci, PUSHORT wrk, SHORT inner);
static INT  NEAR div_bnd_diam	(INT sign, LREAL a, LREAL aa, LIREAL b,
				 LIREAL ci, PUSHORT wrk, SHORT inner);
static INT  NEAR div_diam_bnd	(INT sign, LIREAL a, LREAL b, LREAL bb,
				 LIREAL ci, PUSHORT wrk, SHORT inner);
static INT  NEAR div_diam_diam (INT sign, LIREAL a, LIREAL b,
				 LIREAL ci, PUSHORT wrk, LIREAL wrk_ival,
				 SHORT inner);

static VOID NEAR pack_inum	(LIREAL c);
static VOID NEAR far_bnd	(LIREAL c);
static VOID NEAR near_bnd	(LIREAL c);
static INT  NEAR do_round	(LREAL a, INT dir, INT rc, LDIGIT bound,
				 SHORT inner);

/************************************************************************
*  Implementierung globaler Funktionen
*************************************************************************/

INT FAR CALL LiAdd (LIREAL a, LIREAL b, LIREAL c, SHORT inner)
 /***********************************************************************
  *  Funktion : Addition zweier Intervall-Langzahlen: c = a + b.
  *		Es wird die jeweils guenstigere Darstellungsart automatisch
  *		gewaehlt.
  *  Speicher : Bedarf fuer "c" in Bytes:
  *		  2 * LrDigits (c) * sizeof (LDIGIT)
  *		+ 2 * sizeof (LDIGIT)
  *		+ sizeof (LIREALHDR) + sizeof (LREALHDR)
  *  Eingaben : inner:
  *		  = 0 : Es wird die Ausseneinschliessung berechnet. Bei nicht
  *			exakten Ergebnissen betraegt die maximal moegliche
  *			Ueberschaetzung je Grenze 1 Einheit in der nieder-
  *			wertigsten Ziffer, d.h. es kann maximal zu der dem
  *			exakten Wert uebernaechsten Langzahl gerundet worden
  *			sein.
  *		 != 0 : Es wird die Inneneinschliessung berechnet. Bei nicht
  *			exakten Ergebnissen betraegt die maximal moegliche
  *			Ueberschaetzung je Grenze 1 Einheit in der nieder-
  *			wertigsten Ziffer, d.h. es kann maximal zu der dem
  *			exakten Wert uebernaechsten Langzahl gerundet worden
  *			sein.
  *  Ergebnis : Ergebnis der oberen Grenze im hoeher- und der unteren Grenze
  *		im niederwertigen Byte. Pro Grenze sind moeglich:
  *		(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls das Ergebnis gerundet werden musste.
  *  Info     : - Es wird mit max. 2 Schutzziffern gerechnet.
  *		- Die Schutzziffern sind nach Ausfuehrung der Operation nicht
  *		  mehr vorhanden!
  */
{
  LREAL ao, bo, cu, co;
  INT	a_sign, a_bounds,
	b_sign, b_bounds;
  INT	rc, d;

  /* Ermittlung der Darstellungsart von "a" */
  a_bounds = LiBounds (a);
  if (a_bounds) {		/* "a": (untere Grenze, obere Grenze) */
    if (LrIsSpecial (a))
      if (LrSpecial (a) == LR_NAN) { /* Ergebnis: NaN */
	LrSetNaN (c);
	pack_inum (c);
	return ((LR_NAN << 8) + LR_NAN);
      }
      else
	a_sign = LrIsNegInfinity (a);
    else
      a_sign = LrSign (a);
  }
  else {			/* "a": (0-Grenze, Durchmesser) */
    a_sign = LrSign (a);
  }

  /* Ermittlung der Darstellungsart von "b" */
  b_bounds = LiBounds (b);
  if (b_bounds) {		/* "b": (untere Grenze, obere Grenze) */
    if (LrIsSpecial (b))
      if (LrSpecial (b) == LR_NAN) { /* Ergebnis: NaN */
	LrSetNaN (c);
	pack_inum (c);
	return ((LR_NAN << 8) + LR_NAN);
      }
      else
	b_sign = LrIsNegInfinity (b);
    else
      b_sign = LrSign (b);
  }
  else {			/* "b": (0-Grenze, Durchmesser) */
    b_sign = LrSign (b);
  }

  cu = LiPlr (c);
  d = LrDigits (cu);

  if (!a_bounds && !b_bounds && a == b) {
    /* Sonderbehandlung fuer den Fall gleicher Operanden in
       (0-Grenze, Durchmesser)-Darstellung */
    rc = do_round (
	cu, LI_RND_DOWN, LrAdd (LiPlr (a), LiPlr (a), cu, 0), 2, inner);
    co = LiUpperBound (c);
    LrSetPrec (co, d);
    far_bnd (a);
    rc += do_round (
	co, LI_RND_UP, LrAdd (LiPlr (a), LiPlr (a), co, 0), 2, inner) << 8;
    near_bnd (a);
    pack_inum (c);
    return rc;
  }

  /* Bestimmung der unteren Grenze von "a" */
  if (!a_bounds && a_sign)		/* [-(|a|+|da|),-|a|] */
    far_bnd (a);

  /* Bestimmung der unteren Grenze von "b" */
  if (!b_bounds && b_sign)		/* [-(|b|+|db|),-|b|] */
    far_bnd (b);

  /* Berechnung der unteren Grenze der Summe */
  rc = do_round (
      cu, LI_RND_DOWN, LrAdd (LiPlr (a), LiPlr (b), cu, 0), 2, inner);

  /* Bestimmung der oberen Grenze von "a" */
  if (a_bounds) 			/* [au, ao] */
    ao = LiUpperBound (a);
  else {
    if (a_sign)
      near_bnd (a);			/* [-(|a|+|da|), -|a|] */
    else
      far_bnd (a);			/* [a, a+da] */
    ao = LiPlr (a);
  }

  /* Bestimmung der oberen Grenze von "b" */
  if (b_bounds) 			/* [bu, bo] */
    bo = LiUpperBound (b);
  else {
    if (b_sign)
      near_bnd (b);			/* [-(|b|+|db|), -|b|] */
    else
      far_bnd (b);			/* [b, b+db] */
    bo = LiPlr (b);
  }

  /* Berechnung der oberen Grenze der Summe */
  co = LiUpperBound (c);
  LrSetPrec (co, d);
  rc += do_round (co, LI_RND_UP, LrAdd (ao, bo, co, 0), 2, inner) << 8;

  /* Wiederherstellung der urspruenglichen Werte, falls "a" oder "b"
     in der Darstellung [a, a+da] vorlagen */
  if (!a_bounds && (a_sign == 0)) near_bnd (a);
  if (!b_bounds && (b_sign == 0)) near_bnd (b);

  /* Ueberpruefung, ob Inneneinschliessung ueberhaupt moeglich war */
  if (inner) chk_inner (c, rc);

  /* Ueberpruefung, ob (0-Grenze, Durchmesser)-Darstellung moeglich ist;
     Entfernung von ueberfluessigen Nullen */
  pack_inum (c);
  return rc;
} /* LiAdd */

INT FAR CALL LiSub (LIREAL a, LIREAL b, LIREAL c, PVOID wrk, SHORT inner)
 /***********************************************************************
  *  Funktion : Subtraktion zweier Intervall-Langzahlen: c = a - b.
  *  Speicher : Bedarf fuer "c" in Bytes:
  *		  2 * LrDigits (c) * sizeof (LDIGIT)
  *		+ 2 * sizeof (LDIGIT)
  *		+ sizeof (LIREALHDR) + sizeof (LREALHDR)
  *		Bedarf fuer "wrk" in Bytes:
  *		  LiSize (a)
  *		  (Platz fuer "a" bei (0-Grenze, Durchmesser)-Darstellung)
  *  Eingaben : inner:
  *		  = 0 : Es wird die Ausseneinschliessung berechnet. Bei nicht
  *			exakten Ergebnissen betraegt die maximal moegliche
  *			Ueberschaetzung je Grenze 1 Einheit in der nieder-
  *			wertigsten Ziffer, d.h. es kann maximal zu der dem
  *			exakten Wert uebernaechsten Langzahl gerundet worden
  *			sein.
  *		 != 0 : Es wird die Inneneinschliessung berechnet. Bei nicht
  *			exakten Ergebnissen betraegt die maximal moegliche
  *			Ueberschaetzung je Grenze 1 Einheit in der nieder-
  *			wertigsten Ziffer, d.h. es kann maximal zu der dem
  *			exakten Wert uebernaechsten Langzahl gerundet worden
  *			sein.
  *  Ergebnis : Ergebnis der oberen Grenze im hoeher- und der unteren Grenze
  *		im niederwertigen Byte. Pro Grenze sind moeglich:
  *		(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls das Ergebnis gerundet werden musste.
  *  Info     : - Es wird mit max. 2 Schutzziffern gerechnet.
  *		- "wrk" wird nur im Falle gleicher Operanden in (0-Grenze,
  *		  Durchmesser)-Darstellung verwendet.
  *		- "wrk" muss nicht vorbesetzt werden und ist nach Ausfuehrung
  *		  der Funktion undefiniert.
  *		- Die Schutzziffern sind nach Ausfuehrung der Operation nicht
  *		  mehr vorhanden!
  */
{
  LREAL ao, bo, cu, co;
  INT	a_sign, a_bounds,
	b_sign, b_bounds;
  INT	rc, rc2, d;

  /* Ermittlung der Darstellungsart von "a" */
  a_bounds = LiBounds (a);
  if (a_bounds) {		/* "a": (untere Grenze, obere Grenze) */
    if (LrIsSpecial (a))
      if (LrSpecial (a) == LR_NAN) { /* Ergebnis: NaN */
	LrSetNaN (c);
	pack_inum (c);
	return ((LR_NAN << 8) + LR_NAN);
      }
      else
	a_sign = LrIsNegInfinity (a);
    else
      a_sign = LrSign (a);
  }
  else {			/* "a": (0-Grenze, Durchmesser) */
    a_sign = LrSign (a);
  }

  /* Ermittlung der Darstellungsart von "b" */
  b_bounds = LiBounds (b);
  if (b_bounds) {		/* "b": (untere Grenze, obere Grenze) */
    if (LrIsSpecial (b))
      if (LrSpecial (b) == LR_NAN) { /* Ergebnis: NaN */
	LrSetNaN (c);
	pack_inum (c);
	return ((LR_NAN << 8) + LR_NAN);
      }
      else
	b_sign = LrIsNegInfinity (b);
    else
      b_sign = LrSign (b);
  }
  else {			/* "b": (0-Grenze, Durchmesser) */
    b_sign = LrSign (b);
  }

  cu = LiPlr (c);
  d = LrDigits (cu);

  /* Sonderbehandlung fuer den Fall gleicher Operanden in
     (0-Grenze, Durchmesser)-Darstellung */
  if (!a_bounds && !b_bounds && a == b) {
    memcpy (wrk, (PCHAR) a, LiSize (a));
    far_bnd ((LIREAL) wrk);
    rc = a_sign ? LrSub (LiPlr ((LIREAL)wrk), LiPlr (a), cu, 0)
		: LrSub (LiPlr (a), LiPlr ((LIREAL)wrk), cu, 0);
    rc = do_round (cu, LI_RND_DOWN, rc, 2, inner);
    co = LiUpperBound (c);
    LrSetPrec (co, d);
    rc2 = a_sign ? LrSub (LiPlr (a), LiPlr ((LIREAL)wrk), co, 0)
		 : LrSub (LiPlr ((LIREAL)wrk), LiPlr (a), co, 0);
    rc += do_round (co, LI_RND_UP, rc2, 2, inner) << 8;
    pack_inum (c);
    return rc;
  }

  /* Bestimmung der unteren Grenze von "a" */
  if (!a_bounds && a_sign)		/* [-(|a|+|da|), -|a|] */
    far_bnd (a);

  /* Bestimmung der oberen Grenze von "b" */
  if (b_bounds) 			/* [bu, bo] */
    bo = LiUpperBound (b);
  else {
    if (b_sign == 0)			/* [b, b+db] */
      far_bnd (b);
    bo = LiPlr (b);
  }

  /* Berechnung der unteren Grenze der Differenz */
  rc = do_round (cu, LI_RND_DOWN, LrSub (LiPlr (a), bo, cu, 0), 2, inner);

  /* Bestimmung der oberen Grenze von "a" */
  if (a_bounds) 			/* [au, ao] */
    ao = LiUpperBound (a);
  else {
    if (a_sign) 			/* [-(|a|+|da|), -|a|] */
      near_bnd (a);
    else				/* [a, a+da] */
      far_bnd (a);
    ao = LiPlr (a);
  }

  /* Bestimmung der unteren Grenze von "b" */
  if (!b_bounds)
    if (b_sign) 			 /* [-(|b|+|db|), -|b|] */
      far_bnd (b);
    else
      near_bnd (b);			 /* [b, b+db] */

  /* Berechnung der oberen Grenze der Differenz */
  co = LiUpperBound (c);
  LrSetPrec (co, d);
  rc += do_round (co, LI_RND_UP, LrSub (ao, LiPlr (b), co, 0), 2, inner) << 8;

  /* Wiederherstellung der urspruenglichen Werte, falls "a" in der
     Darstellung [a, a+da] oder "b" in der Darstellung [-(|b|+|db|),-|b|]
     vorlagen */
  if (!a_bounds && (a_sign == 0)) near_bnd (a);
  if (!b_bounds &&  b_sign)	  near_bnd (b);

  /* Ueberpruefung, ob Inneneinschliessung ueberhaupt moeglich war */
  if (inner) chk_inner (c, rc);

  /* Ueberpruefung, ob (0-Grenze, Durchmesser)-Darstellung moeglich ist;
     Entfernung von ueberfluessigen Nullen */
  pack_inum (c);
  return rc;
} /* LiSub */

INT FAR CALL LiMul (LIREAL a, LIREAL b, LIREAL c, PVOID wrk, SHORT inner)
 /***********************************************************************
  *  Funktion : Multiplikation zweier Intervall-Langzahlen: c = a * b.
  *  Speicher : Bedarf fuer "c" in Bytes:
  *		  2 * LrDigits (c) * sizeof (LDIGIT)
  *		+ 4 * sizeof (LDIGIT)
  *		+ sizeof (LIREALHDR) + sizeof (LREALHDR)
  *		Bedarf fuer "wrk" in Bytes:
  *		  LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		+ 4 * sizeof (LDIGIT)
  *  Eingaben : inner:
  *		  = 0 : Es wird die Ausseneinschliessung berechnet. Bei nicht
  *			exakten Ergebnissen betraegt die maximal moegliche
  *			Ueberschaetzung je Grenze 1 Einheit in der nieder-
  *			wertigsten Ziffer, d.h. es kann maximal zu der dem
  *			exakten Wert uebernaechsten Langzahl gerundet worden
  *			sein.
  *		 != 0 : Es wird die Inneneinschliessung berechnet. Bei nicht
  *			exakten Ergebnissen betraegt die maximal moegliche
  *			Ueberschaetzung je Grenze 1 Einheit in der nieder-
  *			wertigsten Ziffer, d.h. es kann maximal zu der dem
  *			exakten Wert uebernaechsten Langzahl gerundet worden
  *			sein.
  *  Ergebnis : Ergebnis der oberen Grenze im hoeher- und der unteren Grenze
  *		im niederwertigen Byte. Pro Grenze sind moeglich:
  *		(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls das Ergebnis gerundet werden musste.
  *  Info     : - Es wird mit max. 4 Schutzziffern gerechnet.
  *		- "wrk" muss nicht vorbesetzt werden und ist nach Ausfuehrung
  *		  der Funktion undefiniert.
  *		- Die Schutzziffern sind nach Ausfuehrung der Operation nicht
  *		  mehr vorhanden!
  */
{
  LREAL au, ao, bu, bo;
  INT	a_sign, aa_sign, a_bounds,
	b_sign, bb_sign, b_bounds;
  INT	rc;

  /* Ermittlung der Darstellungsart von "a" */
  a_bounds = LiBounds (a);
  if (a_bounds) {		/* "a": (untere Grenze, obere Grenze) */
    if (LrIsSpecial (a))
      if (LrSpecial (a) == LR_NAN) { /* Ergebnis: NaN */
	LrSetNaN (c);
	pack_inum (c);
	return ((LR_NAN << 8) + LR_NAN);
      }
      else
	a_sign = LrIsNegInfinity (a);
    else
      a_sign = LrSign (a);
    au = LiPlr (a);
    ao = LiUpperBound (a);
    aa_sign = LrIsSpecial (ao) ? LrIsNegInfinity (ao) : LrSign (ao);
  }
  else {			/* "a": (0-Grenze, Durchmesser) */
    a_sign = LrSign (a);
  }

  /* Ermittlung der Darstellungsart von "b" */
  b_bounds = LiBounds (b);
  if (b_bounds) {		/* "b": (untere Grenze, obere Grenze) */
    if (LrIsSpecial (b))
      if (LrSpecial (b) == LR_NAN) { /* Ergebnis: NaN */
	LrSetNaN (c);
	pack_inum (c);
	return ((LR_NAN << 8) + LR_NAN);
       }
      else
	b_sign = LrIsNegInfinity (b);
    else
      b_sign = LrSign (b);
    bu = LiPlr (b);
    bo = LiUpperBound (b);
    bb_sign = LrIsSpecial (bo) ? LrIsNegInfinity (bo) : LrSign (bo);
  }
  else {			/* "b": (0-Grenze, Durchmesser) */
    b_sign = LrSign (b);
  }

  /* Fallunterscheidung */
  if (a_sign)
    if (a_bounds)
      if (aa_sign == 0)
	if (b_sign)
	 if (b_bounds)
	   if (bb_sign == 0)		 /* au <= 0 <= ao, bu <= 0 <= bo */
	     rc = mul_ival_ival (au, ao, bu, bo, c, (LREAL) wrk, inner);
	   else 			 /* au <= 0 <= ao, bu < bo <= 0 */
	     rc = mul_ival_bnd (ao, au, bu, c, inner);
	 else				 /* au <= 0 <= ao, b+db < b < 0 */
	   rc = mul_ival_diam (ao, au, b, c, inner);
	else
	  if (b_bounds) 		 /* au <= 0 <= ao, 0 <= bu < bo */
	    rc = mul_ival_bnd (au, ao, bo, c, inner);
	  else 			 /* au <= 0 <= ao, 0 < b < b+db */
	    rc = mul_ival_diam (au, ao, b, c, inner);
      else
	if (b_sign)
	  if (b_bounds)
	    if (bb_sign == 0)		 /* au < ao <= 0, bu <= 0 <= bo */
	      rc = mul_ival_bnd (bo, bu, au, c, inner);
	    else			 /* au < ao <= 0, bu < bo <= 0 */
	      rc = mul_bnd_bnd (ao, au, bo, bu, c, inner);
	  else 			 /* au < ao <= 0, b+db < b < 0 */
	    rc = mul_bnd_diam (POS, ao, au, b, c, inner);
	else
	  if (b_bounds) 		 /* au < ao <= 0, 0 <= bu < bo */
	    rc = mul_bnd_bnd (au, ao, bo, bu, c, inner);
	  else 			 /* au < ao <= 0, 0 < b < b+db */
	    rc = mul_bnd_diam (NEG, au, ao, b, c, inner);
    else
      if (b_sign)
	if (b_bounds)
	  if (bb_sign == 0)		 /* a+da < a < 0, bu <= 0 <= bo */
	    rc = mul_ival_diam (bo, bu, a, c, inner);
	  else 			 /* a+da < a < 0, bu < bo <= 0 */
	    rc = mul_bnd_diam (POS, bo, bu, a, c, inner);
	else				 /* a+da < a < 0, b+db < b < 0 */
	    rc = mul_diam_diam (POS, a, b, c, inner);
      else
	if (b_bounds)			 /* a+da < a < 0, 0 <= bu < bo */
	  rc = mul_bnd_diam (NEG, bo, bu, a, c, inner);
	else				 /* a+da < a < 0, 0 < b < b+db */
	  rc = mul_diam_diam (NEG, a, b, c, inner);
  else
    if (b_sign)
      if (b_bounds)
	if (bb_sign == 0)
	  if (a_bounds) 		 /* 0 <= au < ao, bu <= 0 <= bo */
	    rc = mul_ival_bnd (bu, bo, ao, c, inner);
	  else 			 /* 0 < a < a+da, bu <= 0 <= bo */
	    rc = mul_ival_diam (bu, bo, a, c, inner);
	else
	  if (a_bounds) 		 /* 0 <= au < ao, bu < bo <= 0 */
	    rc = mul_bnd_bnd (ao, au, bu, bo, c, inner);
	  else 			 /* 0 < a < a+da, bu < bo <= 0 */
	    rc = mul_bnd_diam (NEG, bu, bo, a, c, inner);
      else
	if (a_bounds)			 /* 0 <= au < ao, b+db < b < 0 */
	  rc = mul_bnd_diam (NEG, ao, au, b, c, inner);
	else				 /* 0 < a < a+da, b+db < b < 0 */
	  rc = mul_diam_diam (NEG, a, b, c, inner);
    else
      if (b_bounds)
	if (a_bounds)			 /* 0 <= au < ao, 0 <= bu < bo */
	  rc = mul_bnd_bnd (au, ao, bu, bo, c, inner);
	else				 /* 0 < a < a+da, 0 <= bu < bo */
	 rc = mul_bnd_diam (POS, bu, bo, a, c, inner);
      else
	if (a_bounds)			 /* 0 <= au < ao, 0 < b < b+db */
	  rc = mul_bnd_diam (POS, au, ao, b, c, inner);
	else				 /* 0 < a < a+da, 0 < b < b+db */
	  rc = mul_diam_diam (POS, a, b, c, inner);

  /* Ueberpruefung, ob Inneneinschliessung ueberhaupt moeglich war */
  if (inner) chk_inner (c, rc);

  /* Ueberpruefung, ob (0-Grenze, Durchmesser)-Darstellung moeglich ist;
     Entfernung von ueberfluessigen Nullen */
  pack_inum (c);
  return rc;
} /* LiMul */

INT FAR CALL LiDiv (
  LIREAL a, LIREAL b, LIREAL c, PVOID wrk, PVOID wrk_ival, SHORT inner)
 /***********************************************************************
  *  Funktion : Division zweier Intervall-Langzahlen: c = a / b.
  *  Speicher : Bedarf fuer "c" in Bytes:
  *		  2 * LrDigits (c) * sizeof (LDIGIT)
  *		+ 2 * sizeof (LDIGIT)
  *		+ sizeof (LIREALHDR) + sizeof (LREALHDR)
  *		Bedarf fuer "wrk" in Bytes:
  *		  (LrDigits (c) + 32) * sizeof (LDIGIT)
  *		Bedarf fuer "wrk_ival" in Bytes:
  *		  LiSize (a)
  *		  (Platz fuer "a" bei (0-Grenze, Durchmesser)-Darstellung)
  *  Eingaben : inner:
  *		  = 0 : Es wird die Ausseneinschliessung berechnet. Bei nicht
  *			exakten Ergebnissen betraegt die maximal moegliche
  *			Ueberschaetzung je Grenze 1 Einheit in der nieder-
  *			wertigsten Ziffer, d.h. es kann maximal zu der dem
  *			exakten Wert uebernaechsten Langzahl gerundet worden
  *			sein.
  *		 != 0 : Es wird die Inneneinschliessung berechnet. Bei nicht
  *			exakten Ergebnissen betraegt die maximal moegliche
  *			Ueberschaetzung je Grenze 1 Einheit in der nieder-
  *			wertigsten Ziffer, d.h. es kann maximal zu der dem
  *			exakten Wert uebernaechsten Langzahl gerundet worden
  *			sein.
  *  Ergebnis : Ergebnis der oberen Grenze im hoeher- und der unteren Grenze
  *		im niederwertigen Byte. Pro Grenze sind moeglich:
  *		(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls das Ergebnis gerundet werden musste.
  *  Info     : - Es wird mit max. 2 Schutzziffern gerechnet.
  *		- "wrk" ist nach Ausfuehrung der Funktion undefiniert.
  *		- "wrk_ival" wird nur im Fall gleicher Operanden in (0-Grenze,
  *		  Durchmesser)-Darstellung verwendet.
  *		- "wrk_ival" ist nach Ausfuehrung der Funktion undefiniert.
  *		- Die Schutzziffern sind nach Ausfuehrung der Operation nicht
  *		  mehr vorhanden!
  */
{
  LREAL au, ao, bu, bo;
  INT	a_sign, aa_sign, a_bounds,
	b_sign, bb_sign, b_bounds;
  INT	rc;

  /* Ermittlung der Darstellungsart von "a" */
  a_bounds = LiBounds (a);
  if (a_bounds) {		/* "a": (untere Grenze, obere Grenze) */
    if (LrIsSpecial (a))
      if (LrSpecial (a) == LR_NAN) { /* Ergebnis: NaN */
	LrSetNaN (c);
	pack_inum (c);
	return ((LR_NAN<<8)+LR_NAN);
      }
      else
	a_sign = LrIsNegInfinity (a);
    else
      a_sign = LrSign (a);
    au = LiPlr (a);
    ao = LiUpperBound (a);
    aa_sign = LrIsSpecial (ao) ? LrIsNegInfinity (ao) : LrSign (ao);
  }
  else {			/* "a": (0-Grenze, Durchmesser) */
    a_sign = LrSign (a);
  }

  /* Ermittlung der Darstellungsart von "b" */
  b_bounds = LiBounds (b);
  if (b_bounds) {		/* "b": (untere Grenze, obere Grenze) */
    if (LrIsSpecial (b))
      if (LrSpecial (b) == LR_NAN) { /* Ergebnis: NaN */
	LrSetNaN (c);
	pack_inum (c);
	return ((LR_NAN<<8)+LR_NAN);
      }
      else
	b_sign = LrIsNegInfinity (b);
    else
      b_sign = LrSign (b);
    bu = LiPlr (b);
    bo = LiUpperBound (b);
    bb_sign = LrIsSpecial (bo) ? LrIsNegInfinity (bo) : LrSign (bo);
  }
  else {			/* "b": (0-Grenze, Durchmesser) */
    b_sign = LrSign (b);
  }

  /* Fallunterscheidung */
  if (a_sign)
    if (a_bounds)
      if (aa_sign == 0)
	if (b_sign)
	  if (b_bounds)
	    if (bb_sign == 0)		 /* au <= 0 <= ao, bu <= 0 <= bo */
	      rc = set_nan_nan (c);
	    else			 /* au <= 0 <= ao, bu < bo <= 0 */
	      rc = div_ival_bnd (ao, au, bo, c, (PUSHORT) wrk, inner);
	  else 			 /* au <= 0 <= ao, b+db < b < 0 */
	    rc = div_ival_diam (ao, au, b, c, (PUSHORT) wrk, inner);
	else
	  if (b_bounds) 		 /* au <= 0 <= ao, 0 <= bu < bo */
	    rc = div_ival_bnd (au, ao, bu, c, (PUSHORT) wrk, inner);
	  else 			 /* au <= 0 <= ao, 0 < b < b+db */
	    rc = div_ival_diam (au, ao, b, c, (PUSHORT) wrk, inner);
      else
	if (b_sign)
	  if (b_bounds)
	    if (bb_sign == 0)		 /* au < ao <= 0, bu <= 0 <= bo */
	      rc = div_bnd_ival (ao, c, inner);
	    else			 /* au < ao <= 0, bu < bo <= 0 */
	      rc = div_bnd_bnd (ao, au, bo, bu, c, (PUSHORT) wrk, inner);
	  else 			 /* au < ao <= 0, b+db < b < 0 */
	    rc = div_bnd_diam (POS, ao, au, b, c, (PUSHORT) wrk, inner);
	else
	  if (b_bounds) 		 /* au < ao <= 0, 0 <= bu < bo */
	    rc = div_bnd_bnd (au, ao, bo, bu, c, (PUSHORT) wrk, inner);
	  else 			 /* au < ao <= 0, 0 < b < b+db */
	    rc = div_bnd_diam (NEG, au, ao, b, c, (PUSHORT) wrk, inner);
    else
      if (b_sign)
	if (b_bounds)
	  if (bb_sign == 0)		 /* a+da < a < 0, bu <= 0 <= bo */
	    if (inner)
	      rc = set_nan_nan (c);
	    else
	      rc = set_inf_inf (c);
	  else 			 /* a+da < a < 0, bu < bo <= 0 */
	    rc = div_diam_bnd (POS, a, bu, bo, c, (PUSHORT) wrk, inner);
	else				 /* a+da < a < 0, b+db < b < 0 */
	  rc = div_diam_diam (POS, a, b, c, (PUSHORT) wrk, (LIREAL) wrk_ival, inner);
      else
	if (b_bounds)			 /* a+da < a < 0, 0 <= bu < bo */
	  rc = div_diam_bnd (NEG, a, bu, bo, c, (PUSHORT) wrk, inner);
	else				 /* a+da < a < 0, 0 < b < b+db */
	  rc = div_diam_diam (NEG, a, b, c, (PUSHORT) wrk, (LIREAL) wrk_ival, inner);
  else
    if (b_sign)
      if (b_bounds)
	if (bb_sign == 0)
	  if (a_bounds) 		 /* 0 <= au < ao, bu <= 0 <= bo */
	    rc = div_bnd_ival (au, c, inner);
	  else 			 /* 0 < a < a+da, bu <= 0 <= bo */
	    if (inner)
	      rc = set_nan_nan (c);
	    else
	      rc = set_inf_inf (c);
	else
	  if (a_bounds) 		 /* 0 <= au < ao, bu < bo <= 0 */
	    rc = div_bnd_bnd (ao, au, bu, bo, c, (PUSHORT) wrk, inner);
	  else 			 /* 0 < a < a+da, bu < bo <= 0 */
	    rc = div_diam_bnd (NEG, a, bo, bu, c, (PUSHORT) wrk, inner);
      else
	if (a_bounds)			 /* 0 <= au < ao, b+db < b < 0 */
	  rc = div_bnd_diam (NEG, ao, au, b, c, (PUSHORT) wrk, inner);
	else				 /* 0 < a < a+da, b+db < b < 0 */
	  rc = div_diam_diam (NEG, a, b, c, (PUSHORT) wrk, (LIREAL) wrk_ival, inner);
    else
      if (b_bounds)
	if (a_bounds)			 /* 0 <= au < ao, 0 <= bu < bo */
	  rc = div_bnd_bnd (au, ao, bu, bo, c, (PUSHORT) wrk, inner);
	else				 /* 0 < a < a+da, 0 <= bu < bo */
	  rc = div_diam_bnd (POS, a, bo, bu, c, (PUSHORT) wrk, inner);
      else
	if (a_bounds)			 /* 0 <= au < ao, 0 < b < b+db */
	  rc = div_bnd_diam (POS, au, ao, b, c, (PUSHORT) wrk, inner);
	else				 /* 0 < a < a+da, 0 < b < b+db */
	  rc = div_diam_diam (POS, a, b, c, (PUSHORT) wrk, (LIREAL) wrk_ival, inner);

  /* Ueberpruefung, ob Inneneinschliessung ueberhaupt moeglich war */
  if (inner) chk_inner (c, rc);

  /* Ueberpruefung, ob (0-Grenze, Durchmesser)-Darstellung moeglich ist;
     Entfernung von ueberfluessigen Nullen */
  pack_inum (c);
  return rc;
} /* LrDiv */

INT FAR CALL LiNeg (LIREAL a, LIREAL b)
 /***********************************************************************
  *  Funktion : Negation eines Langzahl-Intervalls: b = - a.
  *  Speicher : Bedarf fuer "b" in Bytes:
  *		  Platz fuer "a".
  *  Ergebnis : Ergebnis der oberen Grenze im hoeher- und der unteren Grenze
  *		im niederwertigen Byte. Pro Grenze sind moeglich:
  *		(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  */
{
  LREAL real, real_a;
  INT	rc, rc2;

  if (LiBounds (a)) { /* "a" liegt als [untere Grenze, obere Grenze] vor */
    /* Grenzen vertauschen und einzeln negieren */
    LiDiamLength (b) = 0;

    /* untere Grenze (b) = - obere Grenze (a) */
    real_a = LiUpperBound (a);
    real   = LiPlr (b);
    memcpy ((PCHAR) real, (PCHAR) real_a, LrSize (real_a));
    LrNegative (real);
    if (LrIsSpecial (real)) { /* Sonderbehandlung fuer +oo <-> -oo */
      if (LrIsPosInfinity (real))
	rc = LrSpecial (real) = LR_NEG_INFINITY;
      else if (LrIsNegInfinity (real))
	rc = LrSpecial (real) = LR_POS_INFINITY;
      else
	rc = LrSpecial (real);
    }
    else
      rc = LR_NORMAL;

    /* obere Grenze (b) = - untere Grenze (a) */
    real_a = LiPlr (a);
    real   = LiUpperBound (b);
    memcpy ((PCHAR) real, (PCHAR) real_a, LrSize (real_a));
    LrNegative (real);
    if (LrIsSpecial (real)) { /* Sonderbehandlung fuer +oo <-> -oo */
      if (LrIsPosInfinity (real))
	rc2 = LrSpecial (real) = LR_NEG_INFINITY;
      else if (LrIsNegInfinity (real))
	rc2 = LrSpecial (real) = LR_POS_INFINITY;
      else
	rc2 = LrSpecial (real);
    }
    else
      rc2 = LR_NORMAL;
  }
  else { /* "a" liegt in (0-Grenze, Durchmesser)-Darstellung vor */
    memcpy ((PCHAR) b, (PCHAR) a, LiSize (a));
    LrNegative (b);
    rc = rc2 = LR_NORMAL;
  }
  return rc + (rc2 << 8);
} /* LrNeg */

/*****************************************************************************
*  globale Aufrufe lokal definierter Funktionen
*****************************************************************************/

VOID FAR CALL LiPack (LIREAL a)
{
  pack_inum (a);
} /* LiPack */

VOID FAR CALL LiFar (LIREAL a)
{
  far_bnd (a);
} /* LiFar */

VOID FAR CALL LiNear (LIREAL a)
{
  near_bnd (a);
} /* LiNear */

INT FAR CALL LrRound (LREAL a, INT dir, INT rc, LDIGIT bound, SHORT inner)
{
  return do_round (a, dir, rc, bound, inner);
} /* LrRound */

/************************************************************************
*  Implementierung lokaler Funktionen
*************************************************************************/

/********************************
*  Multiplikation
*********************************/

static INT NEAR abs_gtr (LREAL a, LREAL b)
 /***********************************************************************
  *  Funktion : Vergleich der Betraege von a und b.
  *  Ergebnis : 1, falls |a| > |b|.
  *		0  sonst.
  */
{
  USHORT a_sign, b_sign, a_spec, b_spec;
  INT	 rc;

  /* Bildung der Betraege von "a" und "b" */
  if (LrIsSpecial (a)) {
    a_spec = LrSpecial (a);
    if (LrSpecial (a) == LR_NEG_INFINITY)
      LrSpecial (a) = LR_POS_INFINITY;
  }
  a_sign = LrSign (a); LrSetPositive (a);

  if (LrIsSpecial (b)) {
    b_spec = LrSpecial (b);
    if (LrSpecial (b) == LR_NEG_INFINITY)
      LrSpecial (b) = LR_POS_INFINITY;
  }
  b_sign = LrSign (b); LrSetPositive (b);

  rc = LrCmp (a, b);

  /* Rueckgaengigmachung der Betragsbildung */
  if (LrIsSpecial (a)) LrSpecial (a) = a_spec;
  LrSetSign (a, a_sign);

  if (LrIsSpecial (b)) LrSpecial (b) = b_spec;
  LrSetSign (b, b_sign);

  return (rc == LR_GT);
} /* abs_gtr */

static INT NEAR mul_ival_ival (
  LREAL a, LREAL aa, LREAL b, LREAL bb, LIREAL ci, LREAL wrk, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, aa] * [b, bb]
  *		mit  c = min (a*bb, aa*b), cc = max (a*b, aa*bb).
  *  Speicher : Bedarf fuer "wrk" in Bytes:
  *		  LrDigits (ci) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		+ 4 * sizeof (LDIGIT)
  *  Annahmen : a <= 0 <= aa, b <= 0 <= bb.
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : stets in (untere Grenze, obere Grenze)-Darstellung.
  */
{
  LREAL cc, c = LiPlr (ci);
  INT	rc, rc_wrk, rc_c, cmp;
  INT	d;

  LrSetPrec (wrk, d = LrDigits (c));
  /* Bestimmung der Groessenverhaeltnisse der Betraege, es gilt:
     |a| >  |aa| und |b| >  |bb|: cmp = 3
     |a| >  |aa| und |b| <= |bb|: cmp = 2
     |a| <= |aa| und |b| >  |bb|: cmp = 1
     |a| <= |aa| und |b| <= |bb|: cmp = 0 */
  cmp = (abs_gtr (a, aa) << 1) + abs_gtr (b, bb);
  if ((cmp == 3) || (cmp == 0)) {
    rc    = do_round (c, LI_RND_DOWN, LrMul (a, bb, c, 0), 2, inner);
    rc_wrk = do_round (wrk, LI_RND_DOWN, LrMul (aa, b, wrk, 0), 2, inner);
    if (LrCmp (c, wrk) == LR_GT) { /* "wrk" ist die untere Grenze */
      memcpy ((PCHAR) c, (PCHAR) wrk, LrSize (c));
      rc = rc_wrk;
    }
    cc = LiUpperBound (ci); LrSetPrec (cc, d);
    if (cmp == 3)
      rc_wrk = LrMul (a, b, cc, 0);
    else
      rc_wrk = LrMul (aa, bb, cc, 0);

    return ((do_round (cc, LI_RND_UP, rc_wrk, 2, inner) << 8) + rc);
  }
  else {
    if (cmp == 1)
      rc = LrMul (aa, b, c, 0);
    else
      rc = LrMul (a, bb, c, 0);
    rc = do_round (c, LI_RND_DOWN, rc, 2, inner);

    cc = LiUpperBound (ci); LrSetPrec (cc, d);
    rc_c   = do_round (cc, LI_RND_UP, LrMul (a, b, cc, 0), 2, inner);
    rc_wrk = do_round (wrk, LI_RND_UP, LrMul (aa, bb, wrk, 0), 2, inner);

    if (LrCmp (wrk, cc) == LR_GT) { /* "wrk" ist die obere Grenze */
      memcpy ((PCHAR) cc, (PCHAR) wrk, LrSize (wrk));
      return ((rc_wrk << 8) + rc);
    }
    else
      return ((rc_c << 8) + rc);
  }
} /* mul_ival_ival */

static INT NEAR mul_ival_bnd (
  LREAL a, LREAL aa, LREAL b, LIREAL ci, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, aa] * b
  *		darin: c = a*b <= 0 <= cc = aa*b
  *  Annahmen : 0 <= |a| <= |aa|, 0 <= |b|.
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : stets in (untere Grenze, obere Grenze)-Darstellung.
  */
{
  LREAL cc, c = LiPlr (ci);
  INT	rc, d;

  d  = LrDigits (c);
  rc = do_round (c, LI_RND_DOWN, LrMul (a, b, c, 0), 2, inner);
  cc = LiUpperBound (ci); LrSetPrec (cc, d);
  return ((do_round (cc, LI_RND_UP, LrMul (
      aa, b, cc, 0), 2, inner) << 8) + rc);
} /* mul_ival_bnd */

static INT NEAR mul_ival_diam (
  LREAL a, LREAL aa, LIREAL b, LIREAL ci, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, aa] * [b, b+db]
  *		darin: c = a*(b+db) <= 0 <= cc = aa*(b+db)
  *  Annahmen : 0 <= |a|,|aa|  und  0 < |b+db|
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : stets in (untere Grenze, obere Grenze)-Darstellung.
  */
{
  LREAL cc, c = LiPlr (ci);
  INT	rc, rc2, d;

  d = LrDigits (c);
  far_bnd (b);

  rc  = do_round (c, LI_RND_DOWN, LrMul (a, LiPlr (b), c, 0), 2, inner);
  cc  = LiUpperBound (ci); LrSetPrec (cc, d);
  rc2 = LrMul (aa, LiPlr (b), cc, 0);

  near_bnd (b);
  return ((do_round (cc, LI_RND_UP, rc2, 2, inner) << 8) + rc);
} /* mul_ival_diam */

static INT NEAR mul_bnd_bnd (
  LREAL a, LREAL aa, LREAL b, LREAL bb, LIREAL ci, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, aa] * [b, bb]
  *		darin: c = a*b, cc = aa*bb
  *  Annahmen : 0 <= |a|,|aa|  und  0 <= |b|,|bb|
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : beide Intervall-Darstellungsarten moeglich.
  */
{
  LREAL cc, c = LiPlr (ci);
  INT	rc, d;

  d  = LrDigits (c);
  rc = do_round (c, LI_RND_DOWN, LrMul (a, b, c, 0), 2, inner);
  cc = LiUpperBound (ci); LrSetPrec (cc, d);
  return ((do_round (cc, LI_RND_UP, LrMul (
      aa, bb, cc, 0), 2, inner) << 8) + rc);
} /* mul_bnd_bnd */

static INT NEAR mul_bnd_diam (INT sign, LREAL a, LREAL aa, LIREAL b,
			     LIREAL ci, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, aa] * [b, b+db]
  *		darin: 0 < c = a*b < cc = aa*(b+db) , falls sign == POS
  *		       c = a*(b+db) < cc = aa*b < 0 , falls sign == NEG
  *  Annahmen : 0 <= |a|,|aa|  und  0 < |b|,|b+db|
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : beide Intervall-Darstellungsarten moeglich.
  */
{
  LREAL cc, c = LiPlr (ci);
  INT	rc, d;

  d = LrDigits (c);
  if (sign == NEG) far_bnd (b);

  rc = do_round (c, LI_RND_DOWN, LrMul (a, LiPlr (b), c, 0), 2, inner);
  if (sign == POS)
    far_bnd (b);
  else
    near_bnd (b);
  cc = LiUpperBound (ci); LrSetPrec (cc, d);
  rc += do_round (cc, LI_RND_UP, LrMul (aa, LiPlr (b), cc, 0), 2, inner) << 8;

  if (sign == POS) near_bnd (b);
  return rc;
} /* mul_bnd_diam */

static INT NEAR mul_diam_diam (
  INT sign, LIREAL a, LIREAL b, LIREAL ci, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, a+da] * [b, b+db]
  *		darin: 0 < c = a*b < cc = (a+da)*(b*db) , falls sign == POS
  *		       c = (a+da)*(b+db) < cc = a*b < 0 , falls sign == NEG
  *  Annahmen : 0 < |a|,|a+da| und  0 < |b|,|b+db|
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : beide Intervall-Darstellungsarten moeglich.
  */
{
  LREAL cc, c = LiPlr (ci);
  INT	rc, d;

  d = LrDigits (c);
  if ((sign == POS) && (a == b)) {
    /* Sonderbehandlung fuer identische Operanden */
    rc = do_round (c, LI_RND_DOWN, LrMul (
	LiPlr (a), LiPlr (a), c, 0), 2, inner);
    far_bnd (a);

    cc = LiUpperBound (ci); LrSetPrec (cc, d);
    rc += do_round (cc, LI_RND_UP, LrMul (
	LiPlr (a), LiPlr (a), cc, 0), 2, inner) << 8;

    near_bnd (a);
    return rc;
  }

  if (sign == NEG) {
    far_bnd (a); far_bnd (b);
  }
  rc = do_round (
    c, LI_RND_DOWN, LrMul (LiPlr (a), LiPlr (b), c, 0), 2, inner);

  if (sign == POS) {
    far_bnd (a); far_bnd (b);
  }
  else {
    near_bnd (a); near_bnd (b);
  }
  cc = LiUpperBound (ci); LrSetPrec (cc, d);
  rc += do_round (cc, LI_RND_UP, LrMul (
      LiPlr (a), LiPlr (b), cc, 0), 2, inner) << 8;

  if (sign == POS) {
    near_bnd (a); near_bnd (b);
  }
  return rc;
} /* mul_diam_diam */

/********************************
*  Division
*********************************/

static INT NEAR set_nan_nan (LIREAL ci)
 /***********************************************************************
  *  Funktion : Rueckgabe von [LR_NAN, LR_NAN].
  *  Ergebnis : [LR_NAN, LR_NAN].
  */
{
  LrSetNaN (LiPlr (ci));
  LrSetNaN (LiUpperBound (ci));
  return ((LR_NAN << 8) + LR_NAN);
} /* set_nan_nan */

static INT NEAR set_inf_inf (LIREAL ci)
 /***********************************************************************
  *  Funktion : Rueckgabe von [LR_NEG_INFINITY, LR_POS_INFINITY].
  *  Ergebnis : [LR_NEG_INFINITY, LR_POS_INFINITY].
  */
{
  LrSetNegInfinity (LiPlr (ci));
  LrSetPosInfinity (LiUpperBound (ci));
  return ((LR_POS_INFINITY << 8) + LR_NEG_INFINITY);
} /* set_inf_inf */

static INT NEAR div_bnd_ival (LREAL a, LIREAL ci, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, aa] / 0
  *		darin ist a die Nullgrenze des Dividenden
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : [NAN, NAN] oder [NEG_INFINITY, POS_INFINITY]
  *		(untere Grenze, obere Grenze)-Darstellung.
  */
{
 if (inner || (LrIsSpecial (a) && (LrSpecial (a) == LR_ZERO)))
   return set_nan_nan (ci);
 else
   return set_inf_inf (ci);
} /* div_bnd_ival */

static INT NEAR div_ival_bnd (
  LREAL a, LREAL aa, LREAL b, LIREAL ci, PUSHORT wrk, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, aa] / b
  *		darin: c = a/b <= 0 <= cc = aa/b
  *		falls b == 0, ist [c, cc] = [NEG_INFINITY, POS_INFINITY].
  *  Annahmen : 0 <= |a| <= |aa|  und  0 <= |b|.
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : stets (untere Grenze, obere Grenze)-Darstellung.
  */
{
  LREAL cc, c = LiPlr (ci);
  INT  rc, d;

  if (LrIsSpecial (b) && (LrSpecial (b) == LR_ZERO))
    return set_nan_nan (ci);
  d  = LrDigits (c);
  rc = do_round (c, LI_RND_DOWN, LrDiv (a, b, c, wrk, 0), 2, inner);
  cc = LiUpperBound (ci);

  LrSetPrec (cc, d);
  return ((do_round (cc, LI_RND_UP, LrDiv (
      aa, b, cc, wrk, 0), 2, inner) << 8) + rc);
} /* div_ival_bnd */

static INT NEAR div_ival_diam (
  LREAL a, LREAL aa, LIREAL b, LIREAL ci, PUSHORT wrk, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, aa] / [b, b+db]
  *		darin: c = a/b <= 0 <= cc = aa/b.
  *  Annahmen : 0 <= |a|,|aa|  und  0 <= |b|.
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : stets (untere Grenze, obere Grenze)-Darstellung.
  */
{
  LREAL cc, c = LiPlr (ci);
  INT	rc, d;

  d  = LrDigits (c);
  rc = do_round (c, LI_RND_DOWN, LrDiv (a, LiPlr (b), c, wrk, 0), 2, inner);
  cc = LiUpperBound (ci);

  LrSetPrec (cc, d);
  return ((do_round (cc, LI_RND_UP, LrDiv (
      aa, LiPlr (b), cc, wrk, 0), 2, inner) << 8) + rc);
} /* div_ival_diam */

static INT NEAR div_bnd_bnd (
  LREAL a, LREAL aa, LREAL b, LREAL bb, LIREAL ci, PUSHORT wrk, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, aa] / [b, bb]
  *		darin: c = a/bb, cc = aa/b
  *		falls b, bb == 0, ist [c, cc] = [NEG_INFINITY, POS_INFINITY].
  *  Annahmen : 0 <= |a|,|aa|  und  0 <= |b|,|bb|.
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : beide Intervall-Darstellungsarten moeglich;
  *		berechnet wird (untere Grenze, obere Grenze)-Darstellung.
  */
{
  INT rc, d;
  LREAL cc, c = LiPlr (ci);

  if (  (LrIsSpecial (b)  && (LrSpecial (b)  == LR_ZERO))
      || (LrIsSpecial (bb) && (LrSpecial (bb) == LR_ZERO)))
    if (   (LrIsSpecial (a)  && (LrSpecial (a) == LR_ZERO))
	|| (LrIsSpecial (aa) && (LrSpecial (aa) == LR_ZERO)))
      return set_nan_nan (ci);
    else
      return inner ? set_nan_nan (ci) : set_inf_inf (ci);

  d  = LrDigits (c);
  rc = do_round (c, LI_RND_DOWN, LrDiv (a, bb, c, wrk, 0), 2, inner);
  cc = LiUpperBound (ci);

  LrSetPrec (cc, d);
  return ((do_round (cc, LI_RND_UP, LrDiv (
      aa, b, cc, wrk, 0), 2, inner) << 8) + rc);
} /* div_bnd_bnd */

static INT NEAR div_bnd_diam (
  INT sign, LREAL a, LREAL aa, LIREAL b, LIREAL ci, PUSHORT wrk, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, aa] / [b, b+db]
  *		darin: 0 < c = a/(b+db) < cc = aa/b , falls sign == POS.
  *		       c = a/b < cc = aa/(b+db) < 0 , falls sign == NEG.
  *  Annahmen : 0 <= |a|,|aa|, 0 < |b|,|b+db|.
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : beide Intervall-Darstellungsarten moeglich;
  *		berechnet wird (untere Grenze, obere Grenze)-Darstellung.
  */
{
  LREAL cc, c = LiPlr (ci);
  INT	rc, d;

  d = LrDigits (c);
  if (sign == POS) far_bnd (b);
  rc = do_round (c, LI_RND_DOWN, LrDiv (a, LiPlr (b), c, wrk, 0), 2, inner);

  if (sign == POS)
    near_bnd (b);
  else
    far_bnd (b);

  cc = LiUpperBound (ci); LrSetPrec (cc, d);
  rc += do_round (cc, LI_RND_UP, LrDiv (
      aa, LiPlr (b), cc, wrk, 0), 2, inner) << 8;

  if (sign == NEG) near_bnd (b);
  return rc;
} /* div_bnd_diam */

static INT NEAR div_diam_bnd (
  INT sign, LIREAL a, LREAL b, LREAL bb, LIREAL ci, PUSHORT wrk, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, a+da] / [b, bb]
  *		darin: 0 < c = a/b < cc = (a+da)/bb , falls sign == POS
  *		       c = (a+da)/b < cc = a/bb < 0 , falls sign == NEG
  *		falls b, bb == 0, ist [c, cc] = [NEG_INFINITY, POS_INFINITY].
  *  Annahmen : 0 < |a|,|a+da|, 0 <= |b|,|bb|.
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : beide Intervall-Darstellungsarten moeglich;
  *		berechnet wird (untere Grenze, obere Grenze)-Darstellung.
  */
{
  LREAL cc, c = LiPlr (ci);
  INT	rc, d;

  if (  (LrIsSpecial (b)  && (LrSpecial (b)  == LR_ZERO))
      || (LrIsSpecial (bb) && (LrSpecial (bb) == LR_ZERO)))
    return inner ? set_nan_nan (ci) : set_inf_inf (ci);

  d = LrDigits (c);
  if (sign == NEG) far_bnd (a);
  rc = do_round (c, LI_RND_DOWN, LrDiv (LiPlr (a), b, c, wrk, 0), 2, inner);

  if (sign == POS)
    far_bnd (a);
  else
    near_bnd (a);

  cc = LiUpperBound (ci); LrSetPrec (cc, d);
  rc += do_round (cc, LI_RND_UP, LrDiv (
      LiPlr (a), bb, cc, wrk, 0), 2, inner) << 8;

  if (sign == POS) near_bnd (a);
  return rc;
} /* div_diam_bnd */

static INT NEAR div_diam_diam (INT sign, LIREAL a, LIREAL b,
  LIREAL ci, PUSHORT wrk, LIREAL wrk_ival, SHORT inner)
 /***********************************************************************
  *  Funktion : ci = [c, cc] = [a, a+da] / [b, b+db]
  *		darin: 0 < c = a/(b+db) < cc = (a+da)/b , falls sign == POS
  *		       c = (a+da)/b < cc = a/(b+db) < 0 , falls sign == NEG.
  *  Annahmen : 0 < |a|,|a+da|, 0 < |b|,|b+db|.
  *  Eingaben : inner:
  *		  == 0 : Ausseneinschliessung.
  *		  != 0 : Inneneinschliessung.
  *  Ergebnis : beide Intervall-Darstellungsarten moeglich;
  *		berechnet wird (untere Grenze, obere Grenze)-Darstellung.
  */
{
  INT rc, d;
  LREAL cc, c = LiPlr (ci);

  d = LrDigits (c);
  if ((sign == POS) && (a == b)) {
    /* Sonderbehandlung fuer identische Operanden */
    memcpy ((PCHAR) wrk_ival, (PCHAR) a, LiSize (a));
    far_bnd (wrk_ival);

    rc = do_round (c, LI_RND_DOWN, LrDiv (
	LiPlr (a), LiPlr (wrk_ival), c, wrk, 0), 2, inner);

    cc = LiUpperBound (ci); LrSetPrec (cc, d);
    return ((do_round (cc, LI_RND_UP, LrDiv (
	LiPlr (wrk_ival), LiPlr (a), cc, wrk, 0), 2, inner) << 8) + rc);
  }

  if (sign == POS)
    far_bnd (b);
  else
    far_bnd (a);

  rc = do_round (c, LI_RND_DOWN, LrDiv (
      LiPlr (a), LiPlr (b), c, wrk, 0), 2, inner);

  if (sign == POS) {
    near_bnd (b); far_bnd (a);
  }
  else {
    far_bnd (b); near_bnd (a);
  }

  cc = LiUpperBound (ci); LrSetPrec (cc, d);
  rc += do_round (cc, LI_RND_UP, LrDiv (
      LiPlr (a), LiPlr (b), cc, wrk, 0), 2, inner);

  if (sign == POS)
    near_bnd (a);
  else
    near_bnd (b);
  return rc;
} /* div_diam_diam */

/********************************
*  Diverses
*********************************/

static VOID NEAR pack_inum (LIREAL c)
 /***********************************************************************
  *  Funktion : Entfernung ueberfluessiger Nullen aus einer Intervall-Lang-
  *		zahl in (untere Grenze, obere Grenze)-Darstellung und evtl.
  *		Konvertierung in (0-Grenze, Durchmesser)-Darstellung.
  *  Annahmen : "c" liegt in (untere Grenze, obere Grenze)-Darstellung vor.
  *		Bei "NaN" muss nur das info-Feld der unteren Grenze korrekt
  *		gesetzt worden sein, da ein Intervall als "NaN" gilt, wenn
  *		eine der Grenzen diese Bedingung erfuellt.
  *		Falls LrDigits (untere Grenze) != LrDigits (obere Grenze) ist,
  *		wird keine Darstellung als (0-Grenze, Durchmesser) versucht
  *		(dieser Fall tritt nur bei Ueberlauf/ Unterlauf auf).
  *  Ergebnis : ---
  */
{
  LREAL  cu, co;
  LDIGIT *cu_msd,*co_msd;
  USHORT c_len, c_prec, c_start;
  LDIGIT c_digit;

  LiDiamLength (c) = 0;

  /* Bestimmung der Adressen der unteren und oberen Grenze von "c" */
  cu = LiPlr (c);
  co = LiUpperBound (c);

  /* Behandlung der Sonderfaelle: Fuer spezielle Langzahlen werden keinerlei
     Ziffern benoetigt; ausserdem ist fuer sie nur eine Darstellung der Form
     (untere Grenze, obere Grenze) moeglich. */

  if (LrIsSpecial (cu)) {
    if (LrSpecial (cu) == LR_NAN)
      /* eine Grenze NaN => gesamtes Intervall NaN */
      LrSetNaN (co);
#ifdef LR_SPECIAL
    LrSetDigits (cu, 0);
#endif
  }

  if (LrIsSpecial (co)) {
    if (LrSpecial (co) == LR_NAN)
      /* eine Grenze NaN => gesamtes Intervall NaN */
      LrSetNaN (cu);
#ifdef LR_SPECIAL
    LrSetDigits (cu, 0); LrSetDigits (co, 0);
#endif
  }

  cu_msd = LrPMSD (cu);
  co_msd = LrPMSD (co);
  if (!LrIsSpecial (cu) && !LrIsSpecial (co)) {
    /* Es liegt keine spezielle Langzahl vor. Zuerst werden alle gemeinsamen
       "Nachnullen" der unteren und oberen Grenze entfernt, d.h. aus z.B.
       [1.2000, 1.2300] wird [1.20, 1.23]. */
    if (LrDigits (cu) == LrDigits (co)) {
      c_len = LrDigits (cu) - 1;
      while (!(cu_msd[c_len] || co_msd[c_len])) c_len--;
#ifdef LR_SPECIAL
      if ((c_len & 1) == 0) c_len++; /* nur gerade Ziffernanzahl erlaubt,
					falls LR_SPECIAL-Bit vorhanden */
#endif
      c_prec = c_len + 1;
      LrSetDigits (cu, c_prec);
      LrSetDigits (co, c_prec);
    }
    /* Eine Umwandlung des Intervalls in die (0-Grenze, Durchmesser)-
       Darstellung ist nur dann moeglich, wenn die beiden Grenzen das
       gleiche Vorzeichen, den gleichen Exponenten und gleiche
       Ziffernanzahl besitzen. */
    if (   (LrSign (cu)     == LrSign (co))
	&& (LrExponent (cu) == LrExponent (co))
	&& (LrDigits (cu)   == LrDigits (co))) {
      /* Da auch Punktintervalle, die nicht in (0-Grenze, Durchmesser)-
	 Darstellung abgespeichert werden koennen, moeglich sind, werden
	 die Ziffern der beiden Langzahlen von vorne her bis zum Auftreten
	 einer Ungleichheit oder dem Ende der Langzahl(en) verglichen.
	 Diese identischen Stellen ergeben bei der Bestimmung des Durch-
	 messers nur Vornullen, die nicht mit abgespeichert und damit auch
	 nicht berechnet werden muessen. */
      c_start = 0;
      while ((c_start <= c_len) && (cu_msd[c_start] == co_msd[c_start]))
	c_start++;
      if (c_start <= c_len) {
	/* Die beiden Langzahlen sind nicht identisch, d.h. eine Darstellung
	   der Form (0-Grenze, Durchmesser) ist moeglich. */
	if (LrSign (cu)) {
	  /* Falls die Grenzen negativ sind, muessen die Langzahlen vertauscht
	     werden, da die 0-Grenze in diesem Fall die Obergrenze ist. Da
	     sich die Langzahlen nur in den Ziffern unterscheiden, muessen
	     auch nur diese getauscht werden. */
	  for (c_prec = c_start; c_prec <= c_len; c_prec++) {
	    c_digit = cu_msd[c_prec];
	    cu_msd[c_prec] = co_msd[c_prec];
	    co_msd[c_prec] = c_digit;
	  }
	}
	/* Berechnung des Durchmessers durch Subtraktion der betragsmaessig
	   kleineren Grenze von der betragsmaessig groesseren. */
	c_prec = c_len - c_start;
#if defined (__ASSEMBLER__)
	LrAsmSub (co_msd + c_start, c_prec, cu_msd + c_start, c_prec, 0);
#else
	{
	  ULONG t;
	  register USHORT borrow;

	  borrow = 0;
	  while ((SHORT)c_prec >= 0) {
	    t = LR_BASE + (ULONG) co_msd[c_start+c_prec]
			- (ULONG) cu_msd[c_start+c_prec] - borrow;
	    co_msd[c_start+c_prec--] = ((PUSHORT) &t)[LO];
	    borrow		     = ((PUSHORT) &t)[HI] == 0;
	  }
	}
#endif
	/* Entfernen von evtl. vorhandenen Vornullen des Durchmessers */
	while (co_msd[c_start] == 0) c_start++;

	/* Verschieben des Durchmessers an die 0-Grenze; Setzen der
	   Laenge des Durchmessers. */
	LiDiamLength (c) = c_len - c_start + 1;
	MoveFarLeft (cu_msd + LrDigits (c), co_msd + c_start,
	    LiDiamLength (c) * sizeof (LDIGIT), CHAR);
      } /* if (c_start > c_len) */
    }
    else {
      /* Entfernung von "Nachnullen" fuer untere und obere Grenze
	 getrennt. */
      c_len = LrDigits (cu) - 1;
      while (!(cu_msd[c_len])) c_len--;
      LrSetDigits (cu, c_len+1);

      c_len = LrDigits (co) - 1;
      while (!(co_msd[c_len])) c_len--;
      LrSetDigits (co, c_len+1);
    }
  }
  else { /* mindestens eine Grenze ist eine spezielle Langzahl */
    /* Entfernung von "Nachnullen" fuer untere und obere Grenze getrennt. */
    if (!LrIsSpecial (cu)) {
      c_len = LrDigits (cu) - 1;
      while (!(cu_msd[c_len])) c_len--;
      LrSetDigits (cu, c_len+1);
    }
    if (!LrIsSpecial (co)) {
      c_len = LrDigits (co) - 1;
      while (!(co_msd[c_len])) c_len--;
      LrSetDigits (co, c_len+1);
    }
  }
  if (LiBounds (c)) {
    /* "Komprimierung" eines Intervalls in der (untere Grenze, obere Grenze)-
       Darstellung. Die info-Felder der beiden Grenzen sind bereits entspre-
       chend der Laenge der Zahlen gesetzt, es ist also nur noch eine Ver-
       schiebung der oberen Grenze in Richtung auf die untere Grenze hin
       durchzufuehren. */
    MoveFarLeft (LrPMSD (c)+LrDigits (c), co, LrSize (co), CHAR);
  }
} /* pack_inum */

static VOID NEAR far_bnd (LIREAL a)
 /***********************************************************************
  *  Funktion : Berechnung der von Null weiter entfernten Grenze einer Inter-
  *		vall-Langzahl in (0-Grenze, Durchmesser)-Darstellung. Die
  *		berechnete Grenze ueberschreibt die 0-Grenze (der Aufruf der
  *		LiNear() stellt den urspruenglichen Zustand wieder her).
  *  Annahmen : "a" muss in (0-Grenze, Durchmesser)-Darstellung vorliegen.
  *  Ergebnis : ---
  */
{
  INT n_cnt = LrDigits (a) - 1;
  INT d_cnt = LiDiamLength (a) - 1;

#if defined (__ASSEMBLER__)
  LrAsmAdd (LrPMSD (a), n_cnt, LrPMSD (a) + n_cnt + 1, d_cnt);
#else
  ULONG t;
  PLDIGIT  n_msd = LrPMSD (a);
  PLDIGIT  d_msd = n_msd + n_cnt + 1;
  register USHORT carry;

  carry = 0;
  while (d_cnt >= 0) {
    t = (ULONG) n_msd[n_cnt] + (ULONG) d_msd[d_cnt--] + carry;
    n_msd[n_cnt--] = ((PUSHORT) &t)[LO];
    carry	   = ((PUSHORT) &t)[HI];
  }
  while (carry && n_cnt >= 0)
    carry = ++(n_msd[n_cnt--]) == 0;
#endif
} /* far_bnd */

static VOID NEAR near_bnd (LIREAL a)
 /***********************************************************************
  *  Funktion : Wiederherstellung der urspruenglichen Werte, nachdem die
  *		vorhergehende Routine aufgerufen wurde.
  *  Annahmen : "a" muss in (0-Grenze, Durchmesser)-Darstellung vorliegen und
  *		LiFar() bereits aufgerufen worden sein.
  */
{
  INT n_cnt = LrDigits (a) - 1;
  INT d_cnt = LiDiamLength (a) - 1;

#if defined (__ASSEMBLER__)
  LrAsmSub (LrPMSD (a), n_cnt, LrPMSD (a) + n_cnt + 1, d_cnt, 0);
#else
  ULONG t;
  PLDIGIT n_msd = LrPMSD (a);
  PLDIGIT d_msd = n_msd + n_cnt + 1;
  register USHORT borrow;

  borrow = 0;
  while (d_cnt >= 0) {
    t = LR_BASE + (ULONG) n_msd[n_cnt] - (ULONG) d_msd[d_cnt--] - borrow;
    n_msd[n_cnt--] = ((PUSHORT) &t)[LO];
    borrow	   = ((PUSHORT) &t)[HI] == 0;
  }
  while (borrow && n_cnt >= 0)
    borrow = n_msd[n_cnt--]-- == 0;
#endif
} /* near_bnd */

static INT NEAR do_round (LREAL a, INT dir, INT rc, LDIGIT bound, SHORT inner)
 /***********************************************************************
  *  Funktion : Rundung der Langzahl "a".
  *  Eingaben : dir  : Rundungsrichtung (LI_RND_DOWN, LI_RND_UP)
  *		inner:
  *		  == 0 fuer Ausseneinschliessung.
  *		  != 0 fuer Inneneinschliessung (=> dir = !dir).
  *		rc   : Ergebnis der Operation, durch die "a" entstanden ist.
  *  Annahmen : Falls ein Unterlauf erfolgte ("a" == LR_ZERO und LR_LOSS
  *		gesetzt), muss das Vorzeichen von "a" mit der Richtung des
  *		Ueberlaufs uebereinstimmen.
  *  Ergebnis : Falls (rc & LR_LOSS) == 0 ist, wird nur dann gerundet, wenn
  *		die Schutzziffer != 0 ist ("bound" ist in diesem Fall == 0,
  *		die Rundung ist in diesem Fall optimal).
  *		Ansonsten wird nur gerundet, falls (rc & LR_LOSS) != 0 ist.
  *		Die Rundung betragsmaessig nach unten erfolgt durch einfaches
  *		Abschneiden der Schutzziffern. Bei der Rundung betragsmaessig
  *		nach oben werden "bound" Einheiten auf die erste Schutzziffer
  *		addiert. Die Ueberschaetzung betraegt maximal eine Einheit
  *		in der niederwertigsten Ziffer.
  *		Sonderfaelle:
  *		  1. a == LR_ZERO und betragsmaessige Rundung nach oben:
  *		     Es wird zur betragsmaessig kleinsten darstellbaren
  *		     Zahl gerundet (0x0.0001*B^LR_MIN_EXP).
  *		  2. a == LR_NEG_INFINITY , LR_POS_INFINITY und betrags-
  *		     maessige Rundung nach unten: Es wird zur betragsmaessig
  *		     groessten darstellbaren Zahl mit einer Stelle gerundet
  *		     (0x0.FFFF*B^LR_MAX_EXP).
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Rundung erfolgte.
  */
{
  PLDIGIT  a_msd = LrPMSD (a);
  INT	   a_len = LrDigits (a);
  register USHORT carry;

  if (inner) dir = !dir; /* Richtungsumkehr bei Inneneinschliessung */
  if (LrIsSpecial (a)) {
    if ((rc & LR_LOSS) == 0) return rc; /* Ergebnis exakt */

    switch (LrSpecial (a)) {
      case LR_ZERO:
	  if (  ((dir == LI_RND_DOWN) &&  LrIsNegative (a))
	      || ((dir == LI_RND_UP)   && !LrIsNegative (a))) {
	    /* Rundung zur betragsmaessig kleinsten Zahl */
#ifdef LR_SPECIAL
	    LiInfo (a) &= ~LR_SPECIAL;
#endif
	    LrSetDigits (a, 1);
	    LrExponent (a) = LR_MIN_EXP;
	    a_msd[0] = 1;
#ifdef LR_SPECIAL
	    a_msd[1] = 0;
#endif
	    return LR_NORMAL | LR_LOSS;
	  }
	  return rc;
	  /* break; */

      case LR_POS_INFINITY:
      case LR_NEG_INFINITY:
	  if ((dir == LI_RND_DOWN) && LrIsNegInfinity (a)) return rc;
	  if ((dir == LI_RND_UP)   && LrIsPosInfinity (a)) return rc;

	  /* Rundung zur betragsmaessig groessten Zahl */
#ifdef LR_SPECIAL
	  LiInfo (a) &= ~LR_SPECIAL;
#endif
	  LrSetDigits (a, 1);
	  LrExponent (a) = LR_MAX_EXP;
	  a_msd[0] = (LDIGIT)(-1);
#ifdef LR_SPECIAL
	  a_msd[1] = (LDIGIT)(-1);
#endif
	  return LR_NORMAL | LR_LOSS;

      default:
	  return LrSpecial (a);
	  /* break; */
    }
  }

  /* Im folgenden liegt keine spezielle Langzahl vor */
  if (  ((dir == LI_RND_DOWN) && !LrIsNegative (a))
      || ((dir == LI_RND_UP)   &&  LrIsNegative (a)))
    return rc | ((a_msd[a_len] == 0) ? 0 : LR_LOSS);

  a_len--;
  if ((rc & LR_LOSS) == 0) {
    /* Addition von einer Einheit zur letzten Ziffer, falls die Schutzziffer
       ungleich Null ist. */
    if (a_msd[a_len+1] == 0) return LR_NORMAL;

    carry = (++(a_msd[a_len]) == 0);
  }
  else {
    /* Die Mantisse von "a" wird um "bound" Einheiten in der 1. Schutzziffer
       inkrementiert. In jedem Fall werden maximal 2 Einheiten zur
       letzten Ziffer addiert. */
    carry = (++(a_msd[a_len]) == 0);
    if (a_msd[a_len+1] > (USHORT) (-bound) && ++(a_msd[a_len]) == 0) carry++;
  }

  while (carry && a_len--)
    carry = ++(a_msd[a_len]) == 0;
  if (carry) {
    LDIGIT t;

    if (LrExponent (a)++ == LR_MAX_EXP) { /* Exponentenueberlauf */
      if (LrSign (a))
	LrSetNegInfinity (a);
      else
	LrSetPosInfinity (a);
      return LrSpecial (a) | LR_LOSS;
    }
    a_len = LrDigits (a) - 1;
    t = a_msd[a_len]; /* 0 oder 1 */
    memset ((PCHAR) a_msd, 0, a_len*sizeof (LDIGIT));
    a_msd[0] = 1; a_msd[a_len] = t;
  }
  return (LR_NORMAL | LR_LOSS);
} /* do_round */
