/************************************************************************
*  $Id: LiCvt2.c		Intervall-Langzahlarithmetik
*				Zeichenkette -> Langzahl-Intervall
*
*  Copyright (C) 1990, 1992 Olaf Knueppel
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
*  Version 1.0	25.08.90	Olaf Knueppel
*  Letzte Aenderung:		Fri 15-May-1992 15:47:51
*************************************************************************/

#include "Portab.h"

#if defined (__ANSI__)
#include <string.h>
#else
#include <memory.h>
#endif

#include "Li.h"

/************************************************************************
*  Prototypen lokaler Funktionen
*************************************************************************/

static INT NEAR SetaustoNaN (PUSHORT ausle, PUSHORT ausri);

/************************************************************************
*  Implementierung globaler Funktionen
*************************************************************************/

INT FAR CALL lisztoaus (
  PCHAR str, PCHAR lrstr, PUSHORT ausle, PUSHORT ausri, USHORT na)
 /***********************************************************************
  *  Funktion : Konvertierung: Dezimalzahl-Intervall-Zeichenkette
  *			    -> Langzahlfelder (untere und obere Grenze).
  *		Format der Dezimalzahl-Intervall-Zeichenkette:
  *		  longinterval
  *		    : "[" longreal "," longreal "]"
  *		    | "[" mantissa "," mantissa "]" exponent
  *		    | mantissa "[" digit+ "," digit+ "]" exponent?
  *		    .
  *		  longreal
  *		    : mantissa exponent?
  *		    .
  *		  mantissa
  *		    : {+-}? digit+ ("." digit+)?
  *		    | {+-}? "." digit+
  *		    .
  *		  exponent
  *		    : {eE} {+-}? digit+)?
  *		    .
  *		Format des Langzahlfeldes:
  *		  info | exponent (Basis 10) | a[1] | a[2] | ... | a[na']
  *		  USHORT LONG		       USHORT USHORT	   USHORT
  *		Bedeutung des Langzahlfeldes (B = 2^16):
  *		info-Bitfeld wie bei Langzahlen:
  *		  Vorzeichenbit (LR_NEGATIVE) und na' = # tatsaechlich
  *		  bestimmter Ziffern zur Basis B (bei exakter Darstellung
  *		  ist na' < na moeglich).
  *		  Eine spezielle Zahl (0, +oo, -oo, NaN) ist auf den ersten
  *		  vier Bytes des Langzahlfeldes wie eine spezielle Langzahl
  *		  kodiert.
  *		Zahlwert, falls keine spezielle Zahl vorliegt:
  *		  (sum (k = 1..na') a[k]*B^(na'-k)) * 10^exponent
  *  Speicher : Bedarf fuer "ausle", "ausri" in Bytes (jeweils):
  *		Falls na = (strlen (str) - 4 + 3) >> 2:
  *		  na * sizeof (USHORT) + sizeof (USHORT) + sizeof (LONG)
  *		sonst:
  *		  m * sizeof (USHORT) + sizeof (USHORT) + sizeof (LONG)
  *		  mit  m = (na * 78913 + 49152) >> 16
  *		Bedarf fuer "lrstr" in Bytes:
  *		  strlen (str) - 3
  *  Eingaben : str	: Zeiger auf Dezimalzahl-Intervallzeichenkette.
  *		lrstr	: Zeiger auf Zeichenkettenfeld fuer temporaere Zwecke
  *		na	: # zu bestimmender Ziffern zur Basis B; eine exakte
  *			  Darstellung der Langzahl im Langzahlfeld erfolgt
  *			  stets fuer
  *			    na = (strlen (str) - 4 + 3) >> 2 <= LR_MAX_DIGITS
  *		ausle	: Langzahlfeld fuer linke Grenze.
  *		ausri	: Langzahlfeld fuer rechte Grenze.
  *  Ergebnis : LR_ZERO
  *		LR_LOSS + LR_ZERO		bei Unterlauf
  *		LR_LOSS + LR_POS_INFINITY	bei Ueberlauf
  *		LR_NAN 			bei fehlerhafter Eingabe
  *		LR_NORMAL (+ LR_LOSS, falls Genauigkeitsverlust moeglich)
  *		jeweils fuer linke und rechte Grenze getrennt.
  *		(Ergebnis = (Erg. rechte Grenze) << 8 + (Erg. linke Grenze))
  */
{
 /*
  *  Algorithmus:
  *  Zunaechst werden die Positionen (relativ zum Zeichenkettenanfang) von:
  *    - eigentlicher Beginn der Zeichenkette (== 0, falls keine fuehrenden
  *	 Leerzeichen vorhanden sind) "numbeg"
  *    - Position der linken und rechten Intervallklammer "lbrackpos",
  *	 "rbrackpos"
  *    - Position des Kommas "commapos"
  *    - Gesamtlaenge der Zeichenkette (== strlen (str)) "numlen"
  *	 bestimmt.
  *  Anhand der Positionen wird entschieden, welches Eingabeformat vorliegt.
  *  Danach werden die Zeichenkettendarstellungen fuer linke und rechte
  *  Grenze getrennt nach "lrstr" kopiert und mittels lrsztoaus() in Lang-
  *  zahlfelder umgewandelt.
  *  Fuer "lrstr" werden 3 Zeichen weniger als fuer "str" benoetigt, da
  *  '[', ',', ']' sowie mindestens eine Stelle der jeweils anderen Grenze
  *  nicht, ein Nullbyte zur Endekennung jedoch zusaetzlich benoetigt wird.
  *  Aus dem gleichen Grund erfolgt eine exakte Darstellung bereits fuer
  *  na =  (strlen (str) - 4 + 3) >> 2.
  */
  INT	 rc, rc2;
  INT	 sign;
  USHORT i, msize, expsize, lowsize, highsize;
  USHORT numbeg, lbrackpos, rbrackpos, commapos, numlen;
  PCHAR  dummyOut;

  /**** 1. Bestimmung von "numbeg", "lbrackpos", "rbrackpos", "commapos"
	   sowie "numlen". */

  i = 0;
  while ((str[i] == ' ') || (str[i] == '\t'))
    i++;				/* Leerzeichen ueberlesen */
  numbeg = i;
  while ((str[i] != '[') && (str[i] != '\0'))
    i++;				/* '[' suchen */
  if (str[i] == '\0')			/* '[' nicht vorhanden */
    return SetaustoNaN (ausle, ausri);

  lbrackpos = i++;
  while ((str[i] != ',') && (str[i] != '\0'))
    i++;				/* ',' suchen */
  if (str[i] == '\0')			/* ',' nicht vorhanden */
    return SetaustoNaN (ausle, ausri);

  commapos = i++;
  while ((str[i] != ']') && (str[i] != '\0'))
    i++;				/* ']' suchen */
  if (str[i] == '\0')			/* ']' nicht vorhanden */
    return SetaustoNaN (ausle, ausri);

  rbrackpos = i++;
  while (str[i] != '\0')
    i++;				/* Zeichenkettenende suchen */
  numlen = i;
  if ((lbrackpos + 1 == commapos) || (commapos + 1 == rbrackpos))
    return SetaustoNaN (ausle, ausri); /* leere linke oder rechte Grenze */

  expsize = numlen - rbrackpos - 1;	/* # Zeichen des gemeinsamen
					   Exponenten */

  /**** 2. Eingabeformat feststellen und Grenzen in Langzahlfelder
	   umwandeln */

  if (numbeg == lbrackpos) {
    /* linke und rechte Grenze liegen getrennt (evtl. mit gemeinsamem
       Exponenten) vor. */

    /* linke Grenze in "lrstr" zusammensetzen und in Langzahlfeld
       umwandeln */

    memcpy (lrstr, str + lbrackpos + 1, msize = commapos - lbrackpos - 1);
    if (expsize)			/* gemeinsamer Exponent */
      memcpy (lrstr + msize, str + rbrackpos + 1, expsize);
    lrstr[msize + expsize] = '\0';
    rc = lrsztoaus (lrstr, &dummyOut, ausle, na);

    /* rechte Grenze in "lrstr" zusammensetzen und in Langzahlfeld
       umwandeln */

    memcpy (lrstr, str + commapos + 1, msize = rbrackpos - commapos - 1);
    if (expsize)			/* gemeinsamer Exponent */
      memcpy (lrstr + msize, str + rbrackpos + 1, expsize);
    lrstr[msize + expsize] = '\0';
    rc2 = lrsztoaus (lrstr, &dummyOut, ausri, na);
    return rc + (rc2 << 8);
  }
  else {
    /* Die Grenzen besitzen gemeinsame Ziffern und (falls vorhanden)
       einen gemeinsamen Exponenten. */

    sign = str[numbeg] == '-';

    /* betragsmaessig kleinere Grenze in "lrstr" zusammensetzen und in
       Langzahlfeld umwandeln. Dazu werden die gemeinsamen Stellen kopiert,
       daran die fuer die Grenze spezifischen Stellen angehaengt und (falls
       vorhanden) noch die Exponentenangabe darangehaengt. */

    memcpy (lrstr, str + numbeg, msize = lbrackpos - numbeg);
    memcpy (lrstr + msize, str + lbrackpos + 1,
	lowsize = commapos - lbrackpos - 1);
    if (expsize)			/* gemeinsamer Exponent */
      memcpy (lrstr + msize + lowsize, str + rbrackpos + 1, expsize);
    lrstr[msize + lowsize + expsize] = '\0';
    rc = lrsztoaus (lrstr, &dummyOut, sign ? ausri : ausle, na);

    /* betragsmaessig groessere Grenze in "lrstr" zusammensetzen und in
       Langzahlfeld umwandeln. Dazu werden die gemeinsamen Stellen kopiert,
       die fuer die Grenze spezifischen Stellen angehaengt und (falls vor-
       handen) noch die Exponentenangabe darangehaengt.
       Ist die Anzahl der fuer diese Grenze spezifischen Stellen um genau
       1 groesser (Ziffer muss "1" sein) als die Stellenanzahl der anderen
       Grenze, so werden die gemeinsamen Stellen um eine Einheit in der
       letzten Ziffer erhoeht (vgl. LiCvt1.c). */

    lrstr++;				/* vorderstes Zeichen von "lrstr"
					   fuer evtl. Ueberlauf reservieren */
    memcpy (lrstr, str + numbeg, msize);/* gemeinsame Ziffern */
    highsize = rbrackpos - commapos - 1;
    if ((highsize == lowsize + 1) && (str[commapos+1] == '1')) {
      INT ovfl;

      memcpy (lrstr + msize, str + commapos + 2, --highsize);
      /* eine Einheit zu den gemeinsamen Ziffern addieren */
      i = msize;
      ovfl = 1;
      do {
	i--;
	if (lrstr[i] == '.')
	  continue;
	if ((lrstr[i] < '0') || (lrstr[i] > '9')) {
	  i++; break;
	}
	if (lrstr[i]++ != '9') {
	  ovfl = 0; break;
	}
	lrstr[i] = '0';
      } while (i);
      if (ovfl) {
	/* Es ist ein Uebertrag aufgetreten ("i" zeigt auf MSD der
	   Zeichenkette) */

	lrstr--;
	if (i) {			/* Vorzeichen */
	  *lrstr = lrstr[1]; lrstr[1] = ' ';
	}
	highsize++;
	lrstr[i] = '1';
      }
    }
    else if (highsize == lowsize)
      memcpy (lrstr + msize, str + commapos + 1, highsize);
    else
      return SetaustoNaN (ausle, ausri);/* Stellenanzahl falsch */

    if (expsize)			/* gemeinsamer Exponent */
      memcpy (lrstr + msize + highsize, str + rbrackpos + 1, expsize);
    lrstr[msize + highsize + expsize] = '\0';
    rc2 = lrsztoaus (lrstr,&dummyOut, sign ? ausle : ausri, na);

    return sign ? ((rc << 8) + rc2) : ((rc2 << 8) + rc);
  }
} /* lisztoaus */

INT FAR CALL liaustoli (
  PUSHORT ausle, PUSHORT ausri, INT rc, LIREAL c, PVOID wrk1, PVOID wrk2,
  PVOID wrk3)
 /***********************************************************************
  *  Funktion : Konvertierung: 2 Langzahlfelder -> Langzahl-Intervall.
  *		Format des Langzahlfeldes:
  *		  info | exponent (Basis 10) | a[1] | a[2] | ... | a[na']
  *		  USHORT  LONG 		USHORT USHORT	    USHORT
  *		Bedeutung des Langzahlfeldes (B = 2^16):
  *		info-Bitfeld wie bei Langzahlen:
  *		  Vorzeichenbit (LR_NEGATIVE) und na' = # Ziffern zur Basis B
  *		Zahlwert:
  *		  (sum (k = 1..na') a[k]*B^(na'-k)) * 10^exponent
  *  Speicher : Bedarf fuer "c" in Bytes:
  *		  2 * LrDigits (c) * sizeof (LDIGIT)
  *		  + 16 * sizeof (LDIGIT)
  *		  + sizeof (LIREALHDR) + sizeof (LREALHDR)
  *		Bedarf fuer "wrk1", "wrk2" in Bytes (jeweils):
  *		  LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 16 * sizeof (LDIGIT)
  *		Bedarf fuer "wrk3" in Bytes:
  *		  LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 32 * sizeof (LDIGIT)
  *  Eingaben : ausle	: Langzahlfeld der linken Grenze, sollte mit
  *			  na >= LrDigits (c) + 4 bestimmt worden sein.
  *		ausri	: Langzahlfeld der rechten Grenze, sollte mit
  *			  na >= LrDigits (c) + 4 bestimmt worden sein.
  *		rc	: Returncode von lisztoaus(). Es wird jeweils
  *			  nur LR_LOSS verwendet.
  *		c	: Langzahl-Intervall.
  *		wrk1/2/3: Arbeitsspeicher.
  *  Ergebnis : LR_ZERO
  *		LR_LOSS + LR_ZERO		bei Unterlauf
  *		LR_LOSS + LR_POS_INFINITY	bei Ueberlauf
  *		LR_NORMAL (+ LR_LOSS, falls Genauigkeitsverlust moeglich)
  *		jeweils fuer linke und rechte Grenze getrennt.
  *		(Ergebnis = (Erg. rechte Grenze) << 8 + (Erg. linke Grenze))
  */
{
  INT	 rcle, rcri;
  USHORT prec;
  LREAL  real = LiPlr (c);

  LiDiamLength (c) = 0;
  prec = LrDigits (real);
  rcle = lraustolr (ausle, real, wrk1, wrk2, wrk3, 0);
  rcle = LrRound (real, LI_RND_DOWN, rcle | (rc & LR_LOSS), 4, 0);

  real = LiUpperBound (c); LrSetDigits (real, prec);
  rcri = lraustolr (ausri, real, wrk1, wrk2, wrk3, 0);
  rcri = LrRound (real, LI_RND_UP, rcri | ((rc >> 8) & LR_LOSS), 4, 0);

  if (LrCmp (LiPlr (c), real) == LR_GT) /* linke Grenze > rechte Grenze */
    LrSetNaN (LiPlr (c));		/* => [NaN, NaN]		*/
  LiPack (c);

  return ((rcri << 8) + rcle);
} /* liaustoli */

INT FAR CALL lisztoli (
  PCHAR str, PCHAR lrstr, LIREAL c, PVOID wrk1, PVOID wrk2, PVOID wrk3,
  PVOID wrk4, PVOID wrk5)
 /***********************************************************************
  *  Funktion : Konvertierung: Dezimalzahl-Zeichenkette -> Langzahl.
  *  Speicher : Bedarf fuer "c" in Bytes:
  *		  2 * LrDigits (c) * sizeof (LDIGIT)
  *		  + 16 * sizeof (LDIGIT)
  *		  + sizeof (LIREALHDR) + sizeof (LREALHDR)
  *		Bedarf fuer "wrk1", "wrk2" in Bytes (jeweils):
  *		  m * sizeof (USHORT) + sizeof (USHORT) + sizeof (LONG)
  *		  mit  m = (LrDigits (c) * 78913 + 364804) >> 16
  *		Bedarf fuer "wrk3", "wrk4" in Bytes (jeweils):
  *		  LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 16 * sizeof (LDIGIT)
  *		Bedarf fuer "wrk5" in Bytes:
  *		  LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 32 * sizeof (LDIGIT)
  *		Bedarf fuer "lrstr" in Bytes:
  *		  strlen (str) - 3
  *  Eingaben : str	     : Zeiger auf Dezimalzahl-Intervallzeichenkette.
  *		lrstr	     : Zeiger auf Zeichenkettenfeld fuer temporaere
  *			       Zwecke.
  *		c	     : Langzahl-Intervall.
  *		wrk1/2/3/4/5 : Arbeitsspeicher.
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  *		jeweils fuer linke und rechte Grenze getrennt.
  *		(Ergebnis = (Erg. rechte Grenze) << 8 + (Erg. linke Grenze))
  */
{
  INT rc;

  rc = lisztoaus (str, lrstr, (PUSHORT) wrk1, (PUSHORT) wrk2, LrDigits (c) + 4);
  return (liaustoli ((PUSHORT) wrk1, (PUSHORT) wrk2, rc, c, wrk3, wrk4, wrk5));
} /* lisztoli */

/************************************************************************
*  Implementierung lokaler Funktionen
*************************************************************************/

static INT NEAR SetaustoNaN (PUSHORT ausle, PUSHORT ausri)
 /***********************************************************************
  *  Funktion : Langzahlfelder (untere und obere Grenze) auf "NaN" setzen
  *		(wird von lisztoaus() verwendet).
  *  Eingaben : Zeiger auf die beiden Langzahlfelder
  *  Ergebnis : (LR_NAN << 8) + LR_NAN
  */
{
  LrSetNaN ((LREAL) ausle);
  LrSetNaN ((LREAL) ausri);
  return (LR_NAN << 8) + LR_NAN;
} /* SetaustoNaN */
