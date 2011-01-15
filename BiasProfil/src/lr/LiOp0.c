/*****************************************************************************
*  $Id: LiOp0.c 		Intervall-Langzahlarithmetik
*				Konvertierungsroutinen, Zuweisung
*
*  Copyright (C) 1990, 1994 Olaf Knueppel
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
*  Version 1.0	30.08.90	Olaf Knueppel
*  Letzte Aenderung:		Fri 04-Mar-1994 16:16:38
*****************************************************************************/

#include "Portab.h"

#if defined (__ANSI__)
#include <string.h>
#else
#include <memory.h>
#endif

#include "Li.h"

/************************************************************************
*  Implementierung globaler Funktionen
*************************************************************************/

INT FAR CALL LiHull (LREAL lb, LREAL hb, LIREAL a, SHORT inner)
 /***********************************************************************
  *  Funktion : Erzeugung der Huelle: a = hull(lb,hb).
  *  Annahmen : "lb" muss nicht zwingend kleiner als "hb" sein.
  *  Speicher : Bedarf fuer "a" in Bytes:
  *		  2 * LrDigits (a) * sizeof (LDIGIT)
  *		+ 1 * sizeof (LDIGIT)
  *		+ sizeof (LIREALHDR) + sizeof (LREALHDR)
  *  Eingaben : inner:
  *		  = 0 : Es wird eine Ausseneinschliessung berechnet.
  *		 != 0 : Es wird eine Inneneinschliessung berechnet.
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich (jeweils fuer
  *		untere Grenze (niederwertiges Byte) und obere Grenze (hoeher-
  *		wertiges Byte)).
  */
{
  INT	 rc, rc2;
  INT	 lb_len, hb_len;
  INT	 a_len = LrDigits (a);
  USHORT a_size;
  LREAL  a_real;

  /* Evtl. "lb" und "hb" tauschen, so dass "lb" <= "hb" wird */
  if (LrCmp (lb, hb) == LR_GT) {
    a_real = lb; lb = hb; hb = a_real;
  }
  lb_len = LrDigits (lb);
  hb_len = LrDigits (hb);

  /* maximale Genauigkeit bestimmen */
  if (a_len > max (lb_len, hb_len)) a_len = max (lb_len, hb_len);

  /* Kopieren der unteren Grenze
     (evtl. mit Nullen auffuellen oder runden) */
  a_real = LiPlr (a);
  a_size = LrSize (a_real);
  if (LrIsSpecial (lb)) {
    rc = LrSpecial (lb);
    memcpy ((PCHAR) a_real, (PCHAR) lb, sizeof (LREALHDR));
  }
  else {
    rc = LR_NORMAL;
    if (a_len < lb_len) { 		/* Rundung notwendig		*/
      rc |= LR_LOSS;
      memcpy ((PCHAR) a_real, (PCHAR) lb, a_size + sizeof (LDIGIT));
      LrSetDigits (a_real, a_len);
      rc = LrRound (a_real, LI_RND_DOWN, rc, 1, inner);
    }
    else {				/* Mit Nullen auffuellen	*/
      memcpy ((PCHAR) a_real, (PCHAR) lb, LrSize (lb));
      memset ((PCHAR) (LrPMSD (a_real) + lb_len), 0,
	      (a_len - lb_len) * sizeof (LDIGIT));
      LrSetDigits (a_real, a_len);
    }
  }

  /* Kopieren der oberen Grenze
     (evtl. mit Nullen auffuellen oder runden) */
  a_real = LiUpperBound (a);
  if (LrIsSpecial (hb)) {
    rc2 = LrSpecial (hb);
    memcpy ((PCHAR) a_real, (PCHAR) hb, sizeof (LREALHDR));
  }
  else {
    rc2 = LR_NORMAL;
    if (a_len < hb_len) { 		/* Rundung notwendig		*/
      rc2 |= LR_LOSS;
      memcpy ((PCHAR) a_real, (PCHAR) hb, a_size + sizeof (LDIGIT));
      LrSetDigits (a_real, a_len);
      rc2 = LrRound (a_real, LI_RND_UP, rc2, 1, inner);
    }
    else {				/* Mit Nullen auffuellen	*/
      memcpy ((PCHAR) a_real, (PCHAR) hb, LrSize (hb));
      memset ((PCHAR) (LrPMSD (a_real) + hb_len), 0,
	      (a_len - hb_len) * sizeof (LDIGIT));
      LrSetDigits (a_real, a_len);
    }
  }

  LiPack (a);
  return rc + (rc2 << 8);
} /* LiHull */

INT FAR CALL LiBound (LREAL t, LIREAL a, INT rc, LDIGIT bound)
 /***********************************************************************
  *  Funktion : Intervalleinschluss einer Langzahl mit mind. 1 Schutzziffer
  *  Speicher : Bedarf fuer "a" in Bytes:
  *		  2 * (LrDigits (a) * sizeof (LDIGIT) + sizeof (LREALHDR))
  *		+ 1 * sizeof (LDIGIT)
  *  Eingaben : t     : die einzuschliessende Langzahl mit mind. 1 Schutz-
  *			ziffer
  *		rc    : Returncode, entstanden bei der Berechnung von "t"
  *		bound : # ggf. in 1. Schutzziffer zu addierende Einheiten
  *  Ausgaben : a     : einschliessendes Langzahl-Intervall
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich (jeweils fuer
  *		untere Grenze (niederwertiges Byte) und obere Grenze (hoeher-
  *		wertiges Byte)).
  */
{
  SHORT  len, a_digits;
  INT	 rclow, rchigh;
  LREAL  a_real;

  /* Kopieren von "t" in die untere Grenze von "a" unter evtl.
     Abschneiden von Ziffern. Es wird immer eine Schutzziffer mitkopiert. */
  if ((a_digits = LrDigits (a)) < LrDigits (t)) {
    /* Zielgenauigkeit kleiner als Quellgenauigkeit. Es sind maximal
       2 Einheiten auf die neue Schutzziffer zu addieren, da durch Addition
       von "bounds" Einheiten auf die alte Schutzziffer maximal ein
       Uebertrag von 2 Einheiten auftreten kann. */
    if (bound > 2) bound = 2;
    rc |= LR_LOSS;
  }
  else {
    /* Die Zielgenauigkeit ist groesser als die Quellgenauigkeit, daher
       kann die komplette Zahl kopiert werden. */
    a_digits = LrDigits (t);
  }
  len	= a_digits + 1;
  a_real = LiPlr (a);
  memcpy ((PCHAR) a_real, (PCHAR) t, len * sizeof (LDIGIT) + sizeof (LREALHDR));
  LrSetDigits (a_real, a_digits);

  rclow = LrRound (a_real, LI_RND_DOWN, rc, bound, 0);

  /* Kopieren von "t" in die obere Grenze von "a" unter evtl. Abschneiden
     von Ziffern. Es wird eine Schutzziffer mitkopiert. */
  a_real = LiUpperBound (a);
  memcpy ((PCHAR) a_real, (PCHAR) t, len * sizeof (LDIGIT) + sizeof (LREALHDR));
  LrSetDigits (a_real, a_digits);

  rchigh = LrRound (a_real, LI_RND_UP, rc, bound, 0);

  LiPack (a);
  return (rclow + (rchigh << 8));
} /* LiBound */

INT FAR CALL LiInfSup (LIREAL a, LREAL is, INT is_sup, SHORT inner)
 /***********************************************************************
  *  Funktion : Bestimmung der unteren bzw. oberen Grenze eines Intervalls:
  *		is = inf(a) bzw. is = sup(a).
  *  Speicher : Bedarf fuer "is" in Bytes:
  *		  LrDigits (a) * sizeof (LDIGIT) + sizeof (LREALHDR))
  *		+ 1 * sizeof (LDIGIT)
  *  Eingaben : is_sup:
  *		   = 0 : Es wird die untere Grenze bestimmt.
  *		  != 0 : Es wird die obere  Grenze bestimmt.
  *		inner:
  *		   = 0 : Es wird eine "Ausseneinschliessung" berechnet, d.h.
  *			 die berechnete Grenze ist betragsmaessig groesser
  *			 oder gleich der Intervallgrenze.
  *		  != 0 : Es wird eine "Inneneinschliessung" berechnet, d.h.
  *			 die berechnete Grenze ist betragsmaessig kleiner
  *			 oder gleich der Intervallgrenze.
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  */
{
  LREAL   a_real;
  INT	  rc;
  INT	  use_far;
  USHORT  a_size, is_size;
  INT	  is_len;
  PLDIGIT is_msd;

  /* Bestimmung des Zeigers auf entsprechenden Grenzwert */
  if (LiDiamLength (a) == 0) {
    /* Intervall liegt als (untere Grenze, obere Grenze) vor. */
    a_real = is_sup ? LiUpperBound (a) : LiPlr (a);
    use_far = 0;
  }
  else {
    /* Intervall liegt als (0-Grenze, Durchmesser) vor. */
    use_far = ((is_sup && !LrIsNegative(a)) || (!is_sup && LrIsNegative(a)));
    if (use_far) LiFar (a);
    a_real = LiPlr (a);
  }

  /* Kopieren der Grenze (evtl. runden) */
  a_size  = LrSize (a_real);
  is_size = LrSize (is);
  is_len  = LrDigits (is);
  if (LrIsSpecial (a))
    rc = LrSpecial (a);
  else
    rc = LR_NORMAL;
  if (is_size < a_size) {
    /* Rundung notwendig */
    rc |= LR_LOSS;
    memcpy ((PCHAR) is, (PCHAR) a_real, is_size + sizeof (LDIGIT));
    LrSetDigits (is, is_len);
    rc = LrRound (is, is_sup ? LI_RND_UP : LI_RND_DOWN, rc, 1, inner);
  }
  else {
    /* Komplett kopieren */
    memcpy ((PCHAR) is, (PCHAR) a_real, a_size);
  }

  /* Evtl. Rueckgaengigmachung der Berechnung der "far"-Grenze */
  if (use_far) LiNear (a);

  /* Entfernen von ueberfluessigen Nullen / Mantissenstellen */
#ifdef LR_SPECIAL
  if (LrIsSpecial (is))
    LrSetDigits (is, 0);
  else
#endif
  {
    is_len = LrDigits (is);
    is_msd = LrPMSD (is);
    while ((--is_len >= 0) && (is_msd [is_len] == 0));
    LrSetDigits (is, is_len+1);
  }
  return rc;
} /* LiInfSup */

INT FAR CALL LiDiam (LIREAL a, LREAL d, SHORT inner)
 /**************************************************************************
  *  Funktion : Bestimmung des Intervall-Durchmessers: d = diam(a).
  *  Speicher : Bedarf fuer "d" in Bytes:
  *		  LrDigits (d) * sizeof (LDIGIT) + sizeof (LREALHDR))
  *		+ 1 * sizeof (LDIGIT)
  *  Eingaben : inner:
  *		 = 0 : Es wird eine "Ausseneinschliessung" berechnet, d.h.
  *		       der berechnete Durchmesser ist betragsmaessig groesser
  *		       oder gleich dem wahren Intervalldurchmesser.
  *		!= 0 : Es wird eine "Inneneinschliessung" berechnet, d.h. der
  *		       berechnete Durchmesser ist betragsmaessig kleiner oder
  *		       gleich dem wahren Intervalldurchmesser.
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  */
{
  PLDIGIT d_msd;
  SHORT   d_len;
  SHORT   exp_korr;
  INT	  rc;

  if (LiDiamLength (a) == 0) {
    /* "a" liegt als (untere Grenze, obere Grenze) vor.
       diam([au,ao]) = (ao-au) */
    rc = LrRound(d,LI_RND_UP,LrSub(LiUpperBound(a),LiPlr(a),d,0),2,inner);
  }
  else {
    /* "a" liegt als (0-Grenze, Durchmesser) vor.
       diam([a,a+da]) = da */
    if ((exp_korr = LrDigits (a) - LiDiamLength (a)) + LR_MIN_EXP
	> LrExponent (a)) {
      /* Durchmesser zu klein => diam = LR_ZERO */
      LrSetSignedZero (d, 0);
      rc = LrRound (d, LI_RND_UP, LR_ZERO | LR_LOSS, 2, inner);
    }
    else {
      LrSetPositive (d); /* evtl. ueberfluessig ??? */
      LrExponent (d) = LrExponent (a) - exp_korr;
      d_len = LrDigits (d);
      if (d_len < LiDiamLength (a)) {
	/* Ergebnis hat geringere Stellenzahl */
	memcpy ((PCHAR) LrPMSD (d), (PCHAR) (LrPMSD (a) + LrDigits (a)),
	    (d_len + 1) * sizeof (LDIGIT));
	rc = LrRound (d, LI_RND_UP, LR_NORMAL | LR_LOSS, 1, inner);
      }
      else {
	/* Ergebnis kann direkt kopiert werden */
	rc = LR_NORMAL;
	memcpy ((PCHAR) LrPMSD (d), (PCHAR) (LrPMSD (a) + LrDigits (a)),
	    LiDiamLength (a) * sizeof (LDIGIT));
	LrSetDigits (d, LiDiamLength (a));
      }
    }
  }

  /* Entfernen von ueberfluessigen Nullen / Mantissenstellen */
#ifdef LR_SPECIAL
  if (LrIsSpecial (d))
    LrSetDigits (d, 0);
  else
#endif
  {
    d_len = LrDigits (d);
    d_msd = LrPMSD (d);
    while ((--d_len >= 0) && (d_msd [d_len] == 0));
    LrSetDigits (d, d_len+1);
  }
  return rc;
} /* LiDiam */

INT FAR CALL LiAssign (LIREAL a, LIREAL b, SHORT inner)
 /***********************************************************************
  *  Funktion : Zuweisung: b = a.
  *  Speicher : Bedarf fuer "b" in Bytes:
  *		  2 * LrDigits (b) * sizeof (LDIGIT)
  *		+ 1 * sizeof (LDIGIT)
  *		+ sizeof (LIREALHDR) + sizeof (LREALHDR)
  *  Eingaben : inner:
  *		  = 0 : Es wird eine Ausseneinschliessung berechnet.
  *		 != 0 : Es wird eine Inneneinschliessung berechnet.
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich (jeweils fuer
  *		untere Grenze (niederwertiges Byte) und obere Grenze (hoeher-
  *		wertiges Byte)).
  */
{
  INT	 rc, rc2;
  LREAL  a_real = LiPlr (a),
	 b_real = LiPlr (b),
	 aa_real;
  USHORT a_size = LrSize (a),
	 b_size = LrSize (b),
	 aa_size;
  INT	 a_len = LrDigits (a),
	 b_len = LrDigits (b);

  if (LiBounds (a)) {	/* (untere Grenze, obere Grenze)-Darstellung	*/
    /* Bestimmung des Returncodes der beiden Grenzen (ohne Rundung)	*/
    aa_real = LiUpperBound (a);
    aa_size = LrSize (aa_real);
    if (LrIsSpecial (a_real))
      rc = LrSpecial (a_real);
    else
      rc = LR_NORMAL;
    if (LrIsSpecial (aa_real))
      rc2 = LrSpecial (aa_real);
    else
      rc2 = LR_NORMAL;
    /* Falls die Zielgenauigkeit >= der beiden Quellgenauigkeiten ist,
       kann direkt kopiert werden. */
    if ((b_size >= a_size) && (b_size >= aa_size)) {
      memcpy ((PCHAR) b, (PCHAR) a, a_size + aa_size + sizeof (USHORT));
      /* USHORT: LIREAL -> d_len */
      return rc + (rc2 << 8);
    }

    /* Im folgenden ist mindestens eine der beiden Grenzen zu runden */
    /* Zuweisung der unteren Grenze */
    if (b_size < a_size) {
      /* Das Zielintervall hat eine geringere Stellenanzahl */
      memcpy ((PCHAR) b_real, (PCHAR) a_real, b_size + sizeof (LDIGIT));
      LrSetDigits (b_real, b_len);
      rc = LrRound (b_real, LI_RND_DOWN, rc | LR_LOSS, 1, inner);
    }
    else {
      /* Das Zielintervall wird kopiert und mit Nullen aufgefuellt */
      memcpy ((PCHAR) b_real, (PCHAR) a_real, a_size);
      memset ((PCHAR) (LrPMSD (b_real) + a_len), 0, b_size - a_size);
      LrSetDigits (b_real, b_len);
    }
    /* Zuweisung der oberen Grenze */
    b_real = LiUpperBound (b);
    a_len  = LrDigits (aa_real);
    if (b_size < aa_size) {
      /* Das Zielintervall hat eine geringere Stellenanzahl */
      memcpy ((PCHAR) b_real, (PCHAR) aa_real, b_size + sizeof (LDIGIT));
      LrSetDigits (b_real, b_len);
      rc2 = LrRound (b_real, LI_RND_UP, rc2 | LR_LOSS, 1, inner);
    }
    else {
      /* Das Zielintervall wird kopiert und mit Nullen aufgefuellt */
      memcpy ((PCHAR) b_real, (PCHAR) aa_real, aa_size);
      memset ((PCHAR) (LrPMSD (b_real) + a_len), 0, b_size - aa_size);
      LrSetDigits (b_real,b_len);
    }
  }
  else { /* (0-Grenze, Durchmesser)-Darstellung */
    if (b_size >= a_size) {
      /* Zielgenauigkeit ist groesser
	 => Intervall kann direkt kopiert werden */
      memcpy ((PCHAR) b, (PCHAR) a, LiSize (a));
      return (LR_NORMAL + (LR_NORMAL << 8));
    }
    /* Die Zielgenauigkeit ist geringer als die Quellgenauigkeit
       => Rundung notwendig. */

    /* Zuweisung der unteren Grenze */
    if (LrIsNegative (a_real)) LiFar(a);
    memcpy ((PCHAR) b_real, (PCHAR) a_real, b_size + sizeof (LDIGIT));
    LrSetDigits (b_real, b_len);
    rc = LrRound (b_real, LI_RND_DOWN, LR_NORMAL | LR_LOSS, 1, inner);

    /* Zuweisung der oberen Grenze */
    if (LrIsNegative (a_real))
      LiNear (a);
    else
      LiFar(a);
    b_real = LiUpperBound(b);
    memcpy ((PCHAR) b_real, (PCHAR) a_real, b_size + sizeof (LDIGIT));
    LrSetDigits (b_real, b_len);
    rc2 = LrRound (b_real, LI_RND_UP, LR_NORMAL | LR_LOSS, 1, inner);
    if (!LrIsNegative (a)) LiNear (a);
  }

  /* Inneneinschliessung moeglich? */
  if (inner)
    if (LrCmp (LiPlr (b), b_real) == LR_GT) {
     LrSetNaN (LiPlr (b));
     rc = rc2 = LR_NAN;
    }

  LiPack (b);
  return rc + (rc2 << 8);
} /* LiAssign */

INT FAR CALL LiMidRad (LREAL a, LREAL d, LIREAL b, SHORT inner)
 /***********************************************************************
  *  Funktion : Zuweisung: b = a +/- d = [a - d, a + d].
  *  Speicher : Bedarf fuer "b" in Bytes:
  *		  2 * LrDigits (b) * sizeof (LDIGIT)
  *		+ 2 * sizeof (LDIGIT)
  *		+ sizeof (LIREALHDR) + sizeof (LREALHDR)
  *  Eingaben : inner:
  *		  = 0 : Es wird eine Ausseneinschliessung berechnet.
  *		 != 0 : Es wird eine Inneneinschliessung berechnet.
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich (jeweils fuer
  *		untere Grenze (niederwertiges Byte) und obere Grenze (hoeher-
  *		wertiges Byte)).
  */
{
  INT	 rc, rc2;
  USHORT prec;
  LREAL  b_real = LiPlr (b);

  LrSetPositive (d);
  prec   = LrDigits (b_real);
  rc	 = LrRound (b_real, LI_RND_DOWN, LrSub (a, d, b_real, 0), 2, inner);
  b_real = LiUpperBound (b);
  LrSetDigits (b_real, prec);
  rc2	 = LrRound (b_real, LI_RND_UP, LrAdd (a, d, b_real, 0), 2, inner);
  LiPack (b);
  return rc + (rc2 << 8);
} /* LiMidRad */
