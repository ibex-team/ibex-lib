/*****************************************************************************
*  $Id: LiCvt1.c		Intervall-Langzahlarithmetik
*				Langzahl-Intervall -> Zeichenkette
*
*  Copyright (C) 1990, 1993 Olaf Knueppel
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
*  Version 1.0	19.08.90	Olaf Knueppel
*  Letzte Aenderung:		Mon 7-Jun-1993 10:47:42
*****************************************************************************/

#include "Portab.h"

#if defined (__ANSI__)
#include <string.h>
#else
#include <string.h>
#include <memory.h>
#endif

#include "Li.h"

/************************************************************************
*  Prototypen lokaler Funktionen
*************************************************************************/

static INT   NEAR abscmp	(LREAL a, LREAL b);
static PCHAR NEAR lrtof 	(LREAL a, PCHAR s, USHORT IntDigNum,
				 USHORT FracDigNum, SHORT rnd,
				 PVOID wrk1, PVOID wrk2, PVOID wrk3,
				 PLONG exp, PSHORT spaces, PINT is_zero);

/************************************************************************
*  Implementierung globaler Funktionen
*************************************************************************/

USHORT FAR CALL litosze (
  LIREAL a, PCHAR s, USHORT FracDigNum, USHORT ExpDigNum,
  PCHAR s1, PCHAR s2, PVOID wrk1, PVOID wrk2, PVOID wrk3, USHORT mode)
 /***********************************************************************
  *  Funktion : Konvertierung: Langzahl-Intervall -> Intervall-Zeichenkette
  *		zur Basis 10 in eines der folgenden E-Formate:
  *		  *a.aa...a[u..u,o..o]E*e...e		(LI_ESTR_FRACCOM)
  *		  [*u.uu...u,*o.oo...o]E*e...e 	(LI_ESTR_EXPCOM)
  *		  [*u.uu...uE*e...e,*o.oo...oE*e...e]	(LI_ESTR_EXPSEP)
  *  Annahmen : FracDigNum >= 1.
  *		ExpDigNum  >= 1.
  *  Speicher : Bedarf fuer "s" in Bytes:
  *		  2 * (FracDigNum + 14) + 3
  *		Bedarf fuer "s1", "s2" in Bytes (jeweils):
  *		  FracDigNum + 14
  *		Bedarf fuer "wrk1", "wrk2" in Bytes (jeweils):
  *		  ((M << 16 + 83845) div 78913) * sizeof (LDIGIT)
  *		  + 14 * sizeof (LDIGIT)
  *		  + sizeof (LREALHDR)
  *		  mit M = (FracDigNum+9) >> 2
  *		Bedarf fuer "wrk3" in Bytes:
  *		  ((M << 16 + 83845) div 78913) * sizeof (LDIGIT)
  *		  + 33 * sizeof (LDIGIT)
  *		  mit M = (FracDigNum+9) >> 2
  *  Eingaben : a : zu konvertierendes Langzahl-Intervall.
  *		s : Zeichenketten-Darstellung des Intervalls.
  *		FracDigNum : # Nachkommastellen.
  *		ExpDigNum  : # Exponentenstellen.
  *		wrk1/2/3   : Arbeitsspeicher.
  *		s1/2	   : Arbeitsspeicher fuer Langzahl-Zeichenketten
  *		mode	   : gewuenschte Darstellungsart (LI_ESTR_FRACCOM,
  *			     LI_ESTR_EXPCOM, LI_ESTR_EXPSEP). Falls zusaetz-
  *			     lich LI_STR_SINGLE gesetzt ist, wird - sofern
  *			     die Darstellung im gewuenschten Format nicht
  *			     moeglich ist - die Darstellung in den anderen
  *			     Formaten versucht (Reihenfolge s.o.).
  *  Ergebnis : Laenge der Intervallzeichenkette (0, falls Konvertierung im
  *		gewuenschten Format nicht moeglich)
  */
{
 /*
  *  Konvertierung:
  *    LI_ESTR_EXPSEP:
  *	 stets moeglich!
  *    LI_ESTR_EXPCOM:
  *	 nur bei gleichen Exponenten moeglich.
  *    LI_ESTR_FRACCOM:
  *	 nur bei gleichen Exponenten, und Vorzeichen sowie "FracDigNum" >= 2
  *	 moeglich, bzw. im Sonderfall [9.9...9aa..aEx, 1.0...00bb..bE(x+1)].
  *	 Ausserdem darf sich die Mantissendifferenz maximal auf die 2. Nach-
  *	 kommastelle ausdehnen. Ausgegeben werden die gemeinsamen Stellen,
  *	 die weiteren Stellen der betragsmaessig kleineren sowie die weiteren
  *	 Stellen der betragsmaessig groesseren Grenze. Ist die Anzahl der
  *	 Stellen der betragsmaessig groesseren Grenze groesser als die
  *	 Stellenanzahl der betragsmaessig kleineren Grenze, so ist die 1.
  *	 Stelle der weiteren Ziffern der betragsmaessig groesseren Grenze
  *	 (immer 1) als Uebertrag zur niederwertigsten gemeinsamen Ziffer zu
  *	 sehen: 1.99[9,10] = [1.99+0.009,1.99+0.010] = [1.999,2.000]
  */
  LREAL  au, ao;
  PCHAR  su, so, smin, smax, s_act;
  LONG  eu, eo;
  USHORT su_len, so_len, MantSize;
  USHORT n_equal, n_diff, i;
  INT	 same_exp, one_exp_diff;

  MantSize = FracDigNum + 3;
  s_act = s;

  /* Konvertierung der beiden Intervallgrenzen in Zeichenketten */
  au = LiPlr (a);

  if (LiBounds (a)) {			/* (untere Grenze, obere Grenze) */
    ao = LiUpperBound (a);
    su = lrtosze (
	au, s1, FracDigNum, ExpDigNum, LR_RND_DOWN, wrk1, wrk2, wrk3, &eu);
    so = lrtosze (
	ao, s2, FracDigNum, ExpDigNum, LR_RND_UP, wrk1, wrk2, wrk3, &eo);
    if (abscmp (au, ao) == LR_GT) {
      smin = so; smax = su;
    }
    else {
      smin = su; smax = so;
    }
    if (LrIsSpecial (au) || LrIsSpecial (ao)) {
      /* Vorzeichen bei Null als Grenze entfernen, da es bei der Intervall-
	 Arithmetik im Gegensatz zu "normalen" Langzahlen keine Unterschei-
	 dung zwischen +0 und -0 gibt. */

      if (LrIsSpecial (au) && (LrSpecial (au) == LR_ZERO)) su[0] = ' ';
      if (LrIsSpecial (ao) && (LrSpecial (ao) == LR_ZERO)) so[0] = ' ';
      same_exp = one_exp_diff = 0;
    }
    else {
      same_exp = (eu == eo);
      one_exp_diff = ((eu + 1 == eo) || (eo + 1 == eu));
    }
  }
  else {				/* (0-Grenze, Durchmesser) */
    if (LrIsNegative (a)) LiFar (a);
    su = lrtosze (
	au, s1, FracDigNum, ExpDigNum, LR_RND_DOWN, wrk1, wrk2, wrk3, &eu);
    if (LrIsNegative (a))
      LiNear (a);
    else
      LiFar (a);
    so = lrtosze (
      au, s2, FracDigNum, ExpDigNum, LR_RND_UP, wrk1, wrk2, wrk3, &eo);
    if (!LrIsNegative (a)) {
      LiNear (a); smin = su; smax = so;
    }
    else {
      smin = so; smax = su;
    }
    same_exp = (eu == eo);
    one_exp_diff = ((eu + 1 == eo) || (eo + 1 == eu));
  }

  /* Fallunterscheidung */
  switch (mode & ~LI_STR_SINGLE) {
    case LI_ESTR_FRACCOM:
	if ((FracDigNum >= 2) && (*su == *so)) {
	  n_equal = n_diff = 0;
	    if (same_exp) {
	      /* 1. Anzahl der identischen Mantissenstellen ermitteln */
	      i = 1;
	      if (smin[1] == smax[1]) {
		n_equal++;
		for (i = 3; smin[i] == smax[i]; i++, n_equal++) {
		  if (smin[i] == 'E') break;
		 }
	      }

	      /* 2. Anzahl der Mantissenstellen ermitteln, die sich in der
		 ersten Stelle um 1 unterscheiden und in allen weiteren
		 Stellen die betragsmaessig kleinere Grenze nur aus "9" und
		 die betragsmaessig groessere Grenze nur aus "0" besteht */
	      if (smin[i] + 1 == smax[i]) {
		if (i++ == 1) i++;	/* Dezimalpunkt ueberspringen */
		while ((smin[i] == '9') && (smax[i] == '0')) {
		  i++; n_diff++;
		}
		if (n_diff > 0) n_diff++;
	      }
	    } /* if (same_exp) */
	    else if (one_exp_diff && (smin[1] == '9') && (smax[1] == '1')
		&& (smax[3] == '0')) {
	      /* Sonderfall: smin == 9.9...9aaaE(x),
			     smax == 1.0...00bbE(x+1)
		 n_equal = 0, nur n_diff muss ermittelt werden (vgl. oben) */
	      i = 3;
	      while ((smin[i] == '9') && (smax[i+1] == '0')) {
		i++; n_diff++;
	      }
	      if (n_diff > 0) n_diff++;
	    }
	    /* "n_equal": Anzahl gleicher Stellen (ohne Dezimalpunkt)
	       "n_diff" : Anzahl der Stellen, deren Differenz 1 in der
			  letzten gezaehlten Stelle ist.
			  [1.999,2.000] => n_diff = 4 */
	    if ((USHORT) (n_equal + n_diff) > 1) {
	      su_len = n_equal + n_diff + 2;
	      if (su_len == MantSize) su_len--;
	      memcpy (s_act, smin, su_len);
	      s_act += su_len;

	      *s_act++ = '[';
	      memcpy (s_act, smin + su_len, MantSize - su_len);
	      s_act += MantSize - su_len;

	      *s_act++ = ',';
	      if (n_diff) *s_act++ = '1';
	      if (same_exp) {
		memcpy (s_act, smax + su_len, MantSize - su_len);
		s_act += MantSize - su_len;
	      }
	      else {
		memcpy (s_act, smax + su_len + 1, MantSize - su_len - 1);
		s_act += MantSize - su_len - 1;
		*s_act++ = '0';
	      }

	      *s_act++ = ']';
	      memcpy (s_act, smin + MantSize,
		  so_len = strlen (smin + MantSize));
	      s_act += so_len;

	      *s_act = '\0';
	      return su_len + so_len + ((MantSize - su_len) << 1) + (n_diff ? 4 : 3);
	    } /* if (n_equal ...) */
	} /* if (FracDigNum ...) */

	/* Format nicht moeglich */
	if ((mode & LI_STR_SINGLE) == 0) return 0;
	/* else: naechste Darstellungsart versuchen */

    case LI_ESTR_EXPCOM:
	if (same_exp) {
	  *s_act++ = '[';
	  memcpy (s_act, su, MantSize);
	  s_act += MantSize;

	  *s_act++ = ',';
	  memcpy (s_act, so, MantSize);
	  s_act += MantSize;

	  *s_act++ = ']';
	  memcpy (s_act, su + MantSize, su_len = strlen (su + MantSize));
	  s_act += su_len;

	  *s_act = '\0';
	  return su_len + (MantSize << 1) + 3;
	}

	/* Format nicht moeglich */
	if ((mode & LI_STR_SINGLE) == 0) return 0;
	/* else: naechste Darstellungsart versuchen */

    case LI_ESTR_EXPSEP:
	*s_act++ = '[';
	memcpy (s_act, su, su_len = strlen (su));
	s_act += su_len;

	*s_act++ = ',';
	memcpy (s_act, so, so_len = strlen (so));
	s_act += so_len;

	*s_act++ = ']';
	*s_act  = '\0';
	return su_len + so_len + 3;
  } /* switch */

  return 0; /* "mode" fehlerhaft! */
} /* litosze */

USHORT FAR CALL litoszf (
  LIREAL a, PCHAR s, USHORT IntDigNum, USHORT FracDigNum,
  PCHAR s1, PCHAR s2, PVOID wrk1, PVOID wrk2, PVOID wrk3, USHORT mode)
 /***********************************************************************
  *  Funktion : Konvertierung: Langzahl-Intervall -> Intervall-Zeichenkette
  *		zur Basis 10 in eines der folgenden F-Formate:
  *		  [*uu...u,*oo...o]			(LI_FSTR_INT)
  *		  *aa...a.aa...a[u...u,o...o]		(LI_FSTR_FRACCOM)
  *		  [*uu...uu.uu...u,*oo...o.oo...o]	(LI_FSTR_FRACSEP)
  *  Annahmen : FracDigNum >= 1 (nicht fuer LI_FSTR_INT).
  *		IntDigNum  >= 1.
  *  Speicher : Bedarf fuer "s" in Bytes:
  *		  2 * (FracDigNum + IntDigNum + 2) + 3
  *		Bedarf fuer "s1", "s2" in Bytes (jeweils):
  *		  FracDigNum + IntDigNum + 14
  *		Bedarf fuer "wrk1", "wrk2" in Bytes (jeweils):
  *		  ((M << 16 + 83845) div 78913) * sizeof (LDIGIT)
  *		  + 14 * sizeof (LDIGIT)
  *		  + sizeof (LREALHDR)
  *		  mit M = (FracDigNum + IntDigNum + 9) >> 2
  *		Bedarf fuer "wrk3" in Bytes:
  *		  ((M << 16 + 83845) div 78913) * sizeof (LDIGIT)
  *		  + 33 * sizeof (LDIGIT)
  *		  mit M = (FracDigNum + IntDigNum + 9) >> 2
  *  Eingaben : a	   : zu konvertierendes Langzahl-Intervall.
  *		s	   : Zeichenketten-Darstellung des Intervalls.
  *		IntDigNum  : # Vorkommastellen.
  *		FracDigNum : # Nachkommastellen.
  *		wrk1/2/3   : Arbeitsspeicher.
  *		s1/2	   : Arbeitsspeicher fuer Langzahl-Zeichenketten
  *		mode	   : gewuenschte Darstellungsart (LI_FSTR_INT,
  *			     LI_FSTR_FRACCOM, LI_FSTR_FRACSEP). Falls zusaetz-
  *			     lich LI_STR_SINGLE gesetzt ist, wird - sofern die
  *			     Darstellung im gewuenschten Format nicht moeglich
  *			     ist - die Darstellung in den anderen Formaten
  *			     versucht (Reihenfolge s.o.). Sollte kein F-Format
  *			     moeglich sein, so wird in diesem Fall auf das E-
  *			     Format umgeschaltet.
  *  Ergebnis : Laenge der Intervallzeichenkette (0, falls Konvertierung im
  *		gewuenschten Format nicht moeglich)
  */
{
 /*
  *  Konvertierung:
  *    LI_FSTR_INT:
  *	 nur fuer ganze Zahlen, die die max. Stellenanzahl nicht ueber-
  *	 schreiten, moeglich.
  *    LI_FSTR_FRACSEP:
  *	 nur fuer Intervalle, deren Vorkommastellen die max. Stellenanzahl
  *	 ("IntDigNum") nicht ueberschreiten, moeglich.
  *    LI_FSTR_FRACCOM:
  *	 nur bei gleichen Exponenten, gleichen Vorzeichen, genuegend kurzem
  *	 Vorkommateil (< "IntDigNum") sowie "FracDigNum" >= 2 moeglich.
  *	 Exponentengleichheit ist fuer Zahlen < 1 nicht umbedingt erforder-
  *	 lich. Auch der folgende Sonderfall wird korrekt erkannt:
  *	 [9..9.9...9aa..a, 10..0.0...0bb..b].
  *	 Die Mantissendifferenz darf sich ausserdem maximal auf die 2. Nach-
  *	 kommastelle ausdehnen. Ausgegeben werden die gemeinsamen Stellen,
  *	 die weiteren Stellen der betragsmaessig kleineren sowie die weiteren
  *	 Stellen der betragsmaessig groesseren Grenze. Ist die Anzahl der
  *	 Stellen der betragsmaessig groesseren Grenze groesser als die
  *	 Stellenanzahl der betragsmaessig kleineren Grenze, so ist die 1.
  *	 Stelle der weiteren Ziffern der betragsmaessig groesseren Grenze
  *	 (immer 1) als Uebertrag zur niederwertigsten gemeinsamen Ziffer zu
  *	 sehen: 1.99[9,10] = [1.99+0.009,1.99+0.010] = [1.999,2.000]
  */
  LREAL  au, ao;
  PCHAR  su, so, smin, smax, s_act;
  LONG  eu, eo;
  SHORT  su_spaces, so_spaces;
  INT	 su_zero, so_zero;
  USHORT su_len, so_len, IntRealNum, MantSize;
  USHORT n_equal, n_diff, i;
  INT	 same_exp, same_sign, one_exp_diff, is_int;

  s_act = s;

  /* Konvertierung der beiden Intervallgrenzen in Zeichenketten und Setzen
     der folgenden Flags:
       same_sign    == beide Grenzen haben das gleiche Vorzeichen (Null wird
		       entsprechend der zweiten Grenze als positiv oder
		       negativ angesehen).
       same_exp     == beide Grenzen haben den gleichen Exponenten bzw.
		       sind betragsmaessig kleiner als 1.
       one_exp_diff == die Exponenten der beiden Grenzen unterscheiden sich
		       genau um eine Einheit.
       is_int	    == beide Grenzen sind ganze Zahlen (Bedingung fuer das
		       korrekte Erkennen von ganzen Zahlen ist, dass die
		       niederwertigste Mantissenstelle nicht gleich Null
		       sein darf (erfuellt durch das automatische Entfernen
		       von "ueberfluessigen" Nullen durch LiPack() )). */
  au = LiPlr (a);

  if (LiBounds (a)) {			/* (untere Grenze, obere Grenze) */
    ao = LiUpperBound (a);
    su = lrtof (au, s1, IntDigNum, FracDigNum, LR_RND_DOWN, wrk1, wrk2, wrk3,
	&eu, &su_spaces, &su_zero);
    so = lrtof (ao, s2, IntDigNum, FracDigNum, LR_RND_UP, wrk1, wrk2, wrk3,
	&eo, &so_spaces, &so_zero);
    if (abscmp (au, ao) == LR_GT) {
      smin = so; smax = su;
    }
    else {
      smin = su; smax = so;
    }
    if (LrIsSpecial (au) || LrIsSpecial (ao))
      same_exp = one_exp_diff = 0;
    else {
      same_exp = ((eu == eo)
	  || ((LrExponent (au) <= 0) && (LrExponent (ao) <= 0)));
      one_exp_diff = ((eu + 1 == eo) || (eo + 1 == eu));
    }
    same_sign = su_zero || so_zero || (LrSign (au) == LrSign (ao));
    if (LrIsSpecial (au))
      is_int = 1;
    else
      is_int = ((LrExponent (au) > 0) && (LrExponent (au) >= LrDigits (au)));
    if (!LrIsSpecial (ao))
     is_int &= ((LrExponent (ao) > 0) && (LrExponent (ao) >= LrDigits (ao)));
  }
  else {				/* (0-Grenze, Durchmesser) */
    ao = au;
    if (LrIsNegative (a)) LiFar (a);
    su = lrtof (au, s1, IntDigNum, FracDigNum, LR_RND_DOWN, wrk1, wrk2, wrk3,
	&eu, &su_spaces, &su_zero);
    if (LrIsNegative (a))
      LiNear (a);
    else
      LiFar (a);
    so = lrtof (au, s2, IntDigNum, FracDigNum, LR_RND_UP, wrk1, wrk2, wrk3,
	&eo, &so_spaces, &so_zero);
    if (!LrIsNegative (a)) {
      LiNear (a); smin = su; smax = so;
    }
    else {
      smin = so; smax = su;
    }
    same_exp = ((eu == eo) || (LrExponent (au) <= 0));
    one_exp_diff = ((eu + 1 == eo) || (eo + 1 == eu));
    is_int = ((LrExponent (au) > 0) && (LrExponent (au) >= LrDigits (au)));
    same_sign = 1;
  }

  /* Falls eine der beiden Grenzen gleich "0.00...0" und die zweite Grenze
     betragsmaessig kleiner als 1 ist, wird eine Darstellung mit gemeinsamen
     Nachkommastellen prinzipiell moeglich, d.h. "same_exp" wird auf 1
     gesetzt. */

  if (su_zero && so_zero)
    same_exp = 1;
  else if (su_zero && (!LrIsSpecial (ao) && (LrExponent (ao) <= 0)))
    same_exp = 1;
  else if (so_zero && (!LrIsSpecial (au) && (LrExponent (au) <= 0)))
    same_exp = 1;

  /* Falls kein F-Format moeglich ist, u.U. auf E-Format ausweichen */
  if ((su_spaces < 0) || (so_spaces < 0))
    if (mode & LI_STR_SINGLE)
      return litosze (a, s, IntDigNum + FracDigNum, 1, s1, s2,
	  wrk1, wrk2, wrk3, LI_ESTR_FRACCOM | LI_STR_SINGLE);
    else
      return 0; 			/* kein F-Format moeglich */

  /* Fallunterscheidung */
  switch (mode & ~LI_STR_SINGLE) {
    case LI_FSTR_INT:
	if (is_int) {
	  *s_act++ = '[';
	  if (su_spaces > 0) {
	    memset (s_act,' ', su_spaces); s_act += su_spaces;
	  }
	  memcpy (s_act, su, su_len = IntDigNum + 1 - su_spaces);
	  s_act += su_len;

	  *s_act++ = ',';
	  if (so_spaces > 0) {
	    memset (s_act,' ', so_spaces); s_act += so_spaces;
	  }
	  memcpy (s_act, so, so_len = IntDigNum + 1 - so_spaces);
	  s_act += so_len;

	  *s_act++ = ']';
	  *s_act   = '\0';
	  return (IntDigNum << 1) + 3;
	}

	/* Format nicht moeglich */
	if ((mode & LI_STR_SINGLE) == 0) return 0;
	/* else: naechste Darstellungsart versuchen */

    case LI_FSTR_FRACCOM:
	if ((FracDigNum >= 2) && same_sign && (same_exp || one_exp_diff)) {
	  /* # wirklicher Vorkommastellen berechnen */
	  IntRealNum = IntDigNum - max (su_spaces, so_spaces);
	  MantSize = IntRealNum + FracDigNum + 2;
	  n_equal = n_diff = 0;
	  if (su_spaces == so_spaces) {
	    one_exp_diff = 0;		/* fuer unten: Kennung zur Unterschei-
					   dung der beiden Faelle */

	    /* 1. Anzahl der identischen Ziffern ermitteln */
	    for (i = 1; smin[i] == smax[i]; i++) {
	      if (smin[i] == '\0')
		break;
	      if (smin[i] != '.')	/* Dezimalpunkt nicht mitzaehlen */
		n_equal++;
	     }

	    /* 2. Anzahl der Ziffern ermitteln, die sich in der ersten Stelle
	       um 1 unterscheiden und in allen weiteren Stellen die betrags-
	       maessig kleinere Grenze nur aus "9" und die betragsmaessig
	       groessere Grenze nur aus "0" besteht */

	    if (smin[i] + 1 == smax[i]) {
	      if (smin[++i] == '.') i++;/* Dezimalpunkt ueberspringen */
	      while ((smin[i] == '9') && (smax[i] == '0')) {
		n_diff++;
		if (smin[++i] == '.')	/* Dezimalpunkt ueberspringen */
		  i++;
	      }
	      if (n_diff > 0) n_diff++;
	    }
	  } /* if (su_spaces ...) */
	  else if ((smax[1] == '1') && (smax[2] == '0') && (smin[1] == '9')) {
	    /* Sonderfall: smin =  99..9.9..9aaa, smin >=  9
			   smax = 100..0.0..0bbb, smax >= 10
	       n_equal = 0, one_exp_diff = 1, Bestimmung von n_diff s.o.
	       Die Faelle [0.9...,1.0...] und [0.0...09...,0.0...10...]
	       sind bereits oben abgehandelt worden, da in diesen Faellen
	       su_spaces == so_spaces ist. */

	    if (so_spaces > su_spaces) /* su_spaces = "smin_spaces" */
	      su_spaces = so_spaces;
	    i = 2;
	    if (smin[i] == '.') i++;	/* Dezimalpunkt ueberspringen */
	    while ((smin[i] == '9') && (smax[i+1] == '0')) {
	      n_diff++;
	      if (smin[++i] == '.') i++;/* Dezimalpunkt ueberspringen */
	    }
	    if (n_diff > 0) n_diff++;
	  }
	  /* "n_equal": Anzahl gleicher Stellen (ohne Dezimalpunkt)
	     "n_diff" : Anzahl der Stellen, deren Differenz 1 in der letzten
			gezaehlten Stelle ist.
			[1.999, 2.000] => n_diff = 4 */

	  if ((USHORT) (n_equal + n_diff) > IntRealNum) {
	    su_len = n_equal + n_diff + 2;
	    if (su_len == MantSize) su_len--;
	    if (su_spaces > 0) {
	      memset (s_act,' ', su_spaces); s_act += su_spaces;
	    }
	    memcpy (s_act, smin, su_len);
	    *s_act = smax[0];	/* Vorzeichen von betragsmaessig groesserer
				   Grenze verwenden (nur entscheidend, falls
				   "smin" == LR_ZERO ist) */
	    s_act += su_len;

	    *s_act++ = '[';
	    memcpy (s_act, smin + su_len, MantSize - su_len);
	    s_act += MantSize - su_len;

	    *s_act++ = ',';
	    if (n_diff) *s_act++ = '1';
	    if (one_exp_diff)		/* strlen (smax) > strlen (smin) */
	      memcpy (s_act, smax + su_len + 1, MantSize - su_len);
	    else			/* Grenzen gleich lang */
	      memcpy (s_act, smax + su_len, MantSize - su_len);
	    s_act += MantSize - su_len;

	    *s_act++ = ']';
	    *s_act   = '\0';
	    return (su_spaces + su_len + ((MantSize - su_len) << 1)
		+ (n_diff ? 4 : 3));
	  } /* if (n_equal ...) */
	} /* if (FracDigNum ...) */

	/* Format nicht moeglich */
	if ((mode & LI_STR_SINGLE) == 0) return 0;
	/* else: naechste Darstellungsart versuchen */

    case LI_FSTR_FRACSEP:
	*s_act++ = '[';
	if (su_spaces > 0) {
	  memset (s_act,' ', su_spaces); s_act += su_spaces;
	}
	memcpy (s_act, su, su_len = IntDigNum + FracDigNum + 2 - su_spaces);
	s_act += su_len;

	*s_act++ = ',';
	if (so_spaces > 0) {
	  memset (s_act,' ', so_spaces); s_act += so_spaces;
	}
	memcpy (s_act, so, so_len = IntDigNum + FracDigNum + 2 - so_spaces);
	s_act += so_len;

	*s_act++ = ']';
	*s_act  = '\0';
	return ((IntDigNum + FracDigNum + 2) << 1) + 3;
  } /* switch */

  return 0; /* "mode" fehlerhaft! */
} /* litoszf */

/************************************************************************
*  Implementierung lokaler Funktionen
*************************************************************************/

static INT NEAR abscmp (LREAL a, LREAL b)
 /***********************************************************************
  *  Funktion : Vergleich der Betraege zweier Langzahlen
  *  Ergebnis : LR_GT, falls a >  b oder
  *			falls a == b und a < 0 oder
  *			falls b == 0
  *		LR_LT, falls a <  b oder
  *			falls a == b und a > 0 oder
  *			falls a == 0
  *		LR_NAN, falls a == LR_NAN oder
  *			falls b == LR_NAN
  */
{
  /* Da "a" und "b" fuer die Ausgabe gerundet werden, liefert diese Funktion
     eine Groessenabschaetzung der gerundeten Werte.
     LR_GT ist daher als "groesser oder gleich" und LR_LT als "kleiner oder
     gleich" zu verstehen. */

  INT cmp;

  if (LrIsSpecial (a))
    switch (LrSpecial (a)) {
      case LR_ZERO:
	  return LR_LT;
      case LR_POS_INFINITY:
      case LR_NEG_INFINITY:
	  return LR_GT;
      default:
	  return LR_NAN;
    } /* switch */

  if (LrIsSpecial (b))
    switch (LrSpecial (b)) {
      case LR_ZERO:
	  return LR_GT;
      case LR_POS_INFINITY:
      case LR_NEG_INFINITY:
	  return LR_LT;
      default:
	  return LR_NAN;
    } /* switch */

  if (LrExponent (a) != LrExponent (b))
    cmp = (LrExponent (a) < LrExponent (b)) ? LR_LT : LR_GT;
  else if (*LrPMSD (a) != *LrPMSD (b))
    cmp = (*LrPMSD (a) < *LrPMSD (b)) ? LR_LT : LR_GT;
  else
    cmp = _LrAbsCmp (a, b);

  if (cmp == LR_EQ) return LrIsNegative (a) ? LR_GT : LR_LT;
  return cmp;
} /* abscmp */

static PCHAR NEAR lrtof (
  LREAL a, PCHAR s, USHORT IntDigNum, USHORT FracDigNum, SHORT rnd,
  PVOID wrk1, PVOID wrk2, PVOID wrk3, PLONG exp, PSHORT spaces, PINT is_zero)
 /***********************************************************************
  *  Funktion : Konvertierung: Langzahl -> Zeichenkette zur Basis 10 im Fest-
  *		punktformat:
  *		  *aa...a.aa...a
  *  Annahmen : FracDigNum >= 1
  *		IntDigNum  >= 1
  *  Speicher : Bedarf fuer "s" in Bytes:
  *		  FracDigNum + IntDigNum + 14
  *		Bedarf fuer "wrk1", "wrk2" in Bytes (jeweils):
  *		  ((M << 16 + 83845) div 78913) * sizeof (LDIGIT)
  *		  + 14 * sizeof (LDIGIT)
  *		  + sizeof (LREALHDR)
  *		  mit M = (FracDigNum + IntDigNum + 9) >> 2.
  *		Bedarf fuer "wrk3" in Bytes:
  *		  ((M << 16 + 83845) div 78913) * sizeof (LDIGIT)
  *		  + 33 * sizeof (LDIGIT)
  *		  mit M = (FracDigNum + IntDigNum + 9) >> 2.
  *  Eingaben : a	   : zu konvertierende Langzahl.
  *		s	   : Zeichenketten-Darstellung der Langzahl.
  *		IntDigNum  : # Vorkommastellen.
  *		FracDigNum : # Nachkommastellen.
  *		rnd	   : Rundungsmodus (LR_RND_DOWN, LR_RND_UP).
  *		wrk1/2/3   : Arbeitsspeicher.
  *		exp	   : Zeiger auf Variable fuer Exponentenrueckgabe.
  *  Ausgaben : spaces    : Anzahl der vor der Zeichenkette einzufuegenden
  *			     Leerzeichen, um auf "IntDigNum" Vorkommastellen
  *			     zu kommen (< 0, falls Konvertierung nicht moeg-
  *			     lich war).
  *		is_zero    : != 0, falls die Zeichenkette "0.00...0" ist.
  *  Ergebnis : Zeiger auf Zeichenkette im Format:
  *		  *aa...a.aa...a
  */
{
 /*
  *  Algorithmus:
  *  Es werden zunaechst soviele Ziffern berechnet, wie maximal zur Dar-
  *  stellung noetig sein koennten. Danach wird anhand des Exponenten
  *  (zur Basis 10) entschieden, ob die Darstellung im Festkommaformat ueber-
  *  haupt moeglich ist. Falls ja, so wird die Zahl auf die erforderliche
  *  Anzahl von Nachkommastellen gerundet (entweder durch Abschneiden oder
  *  durch Addition von einer Einheit zur letzten Nachkommastelle). Das derart
  *  gerundete Ergebnis ist die bestmoegliche Naeherung des wahren Wertes.
  */
  PCHAR  sbeg;
  SHORT  e, frac;
  USHORT FracDigits, i;
  INT	 do_round;

  *is_zero = 0;
  FracDigits = IntDigNum + FracDigNum - 1;
  sbeg = lrtosze (a, s, FracDigits, 1, rnd, wrk1, wrk2, wrk3, exp);
  if (LrIsSpecial (a)) {
    if (LrSpecial (a) == LR_ZERO) {
      *spaces = IntDigNum - 1;
      sbeg[0] = ' ';			/* Null ist vorzeichenlos */
      sbeg[FracDigNum + 3] = '\0';	/* Endekennung */
      *is_zero = 1;
    }
    else {				/* +oo, -oo, NaN */
      *spaces = IntDigNum - 2;
      sbeg[FracDigNum + 4] = '\0';	/* Endekennung */
    }
  }
  else if (*exp >= (LONG) IntDigNum)	/* "normale" Zahl */
     *spaces = -1;
  else {
    /* Rundung auf erforderliche Nachkommastellenanzahl */
    do_round = (rnd == LR_RND_UP) ^ LrIsNegative (a);
    e = (SHORT) *exp;
    MoveLeft (sbeg + 2, FracDigits, CHAR); /* Dezimalpunkt entfernen */
    frac = e + FracDigNum + 1; /* Index auf letzte Nachkommastelle */
    if (frac < 1) {
      /* Die Zahl ist so klein, dass sie zu 0.00...00 bzw. 0.00...01
	 gerundet wird. */

      memset (sbeg + 1,'0', FracDigNum + 2);
      sbeg[FracDigNum + 3] = '\0'; /* Endekennung */
      sbeg[2] = '.';
      if (do_round)
	sbeg[FracDigNum + 2] = '1';
      else {				/* Null ist vorzeichenlos */
	*is_zero = 1; sbeg[0] = ' ';
      }
      *spaces = IntDigNum - 1;
    } /* if (frac ...) */
    else {
      if (do_round) {
	/* Ist eine der abgeschnittenen Nachkommastellen != '0', d.h.
	   ist eine Rundung notwendig? */

	do_round = 0;
	for (i = frac + 1; i < (USHORT) (FracDigits + 2); i++)
	  if (sbeg[i] != '0') {
	    do_round = 1; break;
	  }
      }
      else
	/* Dieser Zweig kann entfallen, falls lrtosze() ganze Zahlen bei
	   zur Null gerichteter Rundung exakt (und nicht als .9999..)
	   berechnet. */
	if (   (LrExponent (a) >= 0)
	    && (LrExponent (a) >= LrDigits (a)) /* ganze Zahl? */
	    && (sbeg[e+2] == '9')) {	/* 1. Nachkommastelle == '9' */
	  do_round = 1;
	  frac = e + 1; 		/* 1 Einheit zur niederwertigsten
					   Vorkommastelle */
	  /* Nachkommastellen auf Null setzen */
	  memset (sbeg + frac + 1,'0', FracDigNum);
	} /* Ende der Ganzzahlkorrektur (n.99...9 -> (n+1).00...0) */

      if (do_round) {
	/* Addition von einer Einheit zur letzten gueltigen Nachkomma-
	   stelle */
	register USHORT carry;

	carry = 1; /* nur notwendig, falls Ganzzahlkorrektur (s.o.) erfolgt */
	for (i = frac; i > 0; i--) {
	  carry = ((sbeg[i])++ == '9');
	  if (carry)
	    sbeg[i] = '0';
	  else
	    break;
	}
	if (carry) {			/* Es ist ein Uebertrag aufgetreten */
	  e++; (*exp)++;
	  MoveRight (sbeg + 1, e + FracDigNum + 1, CHAR);
	  sbeg[1] = '1';
	  frac++;
	}
      }
      /* Dezimalpunkt setzen (bei negativem Exponenten Nachkommanullen
	 und Vorkommanull einfuegen) */

      if (e >= 0) {
	MoveRight (sbeg + e + 2, FracDigNum, CHAR);
	sbeg[e+2] = '.';
	*spaces = IntDigNum - e - 1;
      }
      else { /* e < 0 */
	MoveFarLeft (sbeg - e + 2, sbeg + 1, frac, CHAR);
	memset (sbeg + 1,'0', 1 - e);
	sbeg[2] = '.';
	*spaces = IntDigNum - 1;
      }

      if (*spaces >= 0)
	sbeg[IntDigNum - *spaces + FracDigNum + 2] = '\0'; /* Endekennung */
    } /* else (frac ... ) */
  } /* else */

  return (sbeg);
} /* lrtof */
