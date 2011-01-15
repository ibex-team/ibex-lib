/************************************************************************
*  $Id: Li.h			Intervall-Langzahlarithmetik
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
*  Version 1.0	25.08.90	Olaf Knueppel
*  Letzte Aenderung:		Wed 06-Oct-1993 13:58:00
*************************************************************************/

#ifndef __LI__
#define __LI__

#include <lr/Lr.h>

#if defined (__cplusplus)
extern "C" {
#endif

/************************************************************************
*  Konstanten
*************************************************************************/

#define LI_RND_DOWN	0
#define LI_RND_UP	1

#define LI_ESTR_FRACCOM 0
#define LI_ESTR_EXPCOM 1
#define LI_ESTR_EXPSEP 2

#define LI_FSTR_INT	0
#define LI_FSTR_FRACCOM 1
#define LI_FSTR_FRACSEP 2

#define LI_STR_SINGLE	0x10

/************************************************************************
*  Typ-Deklarationen
*************************************************************************/

typedef struct {
  SHORT  d_len;
  USHORT info;
  SHORT  exponent;
} LIREALHDR, FAR *LIREAL;

/************************************************************************
*  Makro-Definitionen
*************************************************************************/

#define LiPlr(li)		((LREAL) &((li) -> info))
				/* Zeiger auf untere bzw. Nullgrenze	*/

#define LiUpperBound(li)	((LREAL) (LrPMSD (li) + LrDigits (li)))
				/* 2 */

#define LiInfo(li)		((li) -> info)
#define LiDiamLength(li)	((li) -> d_len)
#define LiBounds(li)		(LiDiamLength (li) == 0)
				/* wahr fuer (untere, obere Grenze)-	*/
				/* Darstellung 			*/
#define LiSize(li) \
	(sizeof (LIREALHDR) + (LrDigits (li) + (li) -> d_len)*sizeof (LDIGIT))
				/* 1 */

#define LrCopyPrec(l1,l2)	((l1) -> info = (l2) -> info & LR_PRECISION)
#define LrSetPrec(lr,d) 	((lr) -> info = 0, LrSetDigits (lr, d))

/******************************
*  1 : nur definiert, falls LiBounds() == FALSE
*  2 : nur definiert, falls LiBounds() == TRUE
******************************/

/************************************************************************
*  Funktions-Prototypen
*************************************************************************/

/**************************************
*  LiMem.c
**************************************/

#define LiNewVar(p) \
	LiNew (p, (((p) << 2) + sizeof (LIREALHDR) + sizeof (LREALHDR) + 3) & ~3)
 /***********************************************************************
  *  Funktion : Speicherallokierung fuer eine Intervall-Langzahl ohne
  *		Schutzziffern.
  *  Eingaben : p	: p ergibt die Anzahl der Ziffern einer Grenze zur
  *			  Basis 2^16. (Wird "p" um 1 inkrementiert, entspricht
  *			  dies in etwa der Verlaengerung der Mantisse um
  *			  4.816 Dezimalstellen.)
  *  Ergebnis : Zeiger auf Speicherbereich.
  *  Info     : Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */

#define LiNewTemp(p, g) \
	LiNew (p, (((((p) << 1) + (g)) << 1) + sizeof (LIREALHDR) \
	+ sizeof (LREALHDR) + 3) & ~3)
 /***********************************************************************
  *  Funktion : Speicherallokierung fuer eine Langzahl mit Schutzziffern.
  *  Eingaben : p	: p ergibt die Anzahl der Ziffern der Langzahl zur
  *			  Basis 2^16. (Wird "p" um 1 inkrementiert, entspricht
  *			  dies in etwa der Verlaengerung der Mantisse um
  *			  4.816 Dezimalstellen.)
  *		g	: # Schutzziffern.
  *  Ergebnis : Zeiger auf Speicherbereich.
  *  Info     : Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */

LIREAL FAR CALL LiNew (USHORT Prec, register USHORT Size);
 /***********************************************************************
  *  Funktion : Speicherallokierung fuer eine Intervall-Langzahl.
  *  Eingaben : Prec	: Genauigkeit.
  *		Size	: Speicherbedarf in Bytes.
  *  Ergebnis : Zeiger auf Intervall-Langzahl.
  *  Info     : Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */

#define LiDeleteVar(li, p) \
        _MemDelete (li, (((p) << 2) + sizeof (LIREALHDR) \
                        + sizeof (LREALHDR) + 3) & ~3)
  /**********************************************************************
  *  Funktion :	Freigabe einer Intervall-Langzahl ohne Schutzziffern.
  *  Eingaben : li	: Intervall-Langzahl.
  *		p	: # Langzahlziffern zur Basis 2^16.
  *  Ergebnis : ---
  */

#define LiDeleteTemp(li, p, g) \
        _MemDelete (li, (((((p) << 1) + (g)) << 1) + sizeof (LIREALHDR) \
		        + sizeof (LREALHDR) + 3) & ~3)
  /**********************************************************************
  *  Funktion :	Freigabe einer Intervall-Langzahl mit Schutzziffern.
  *  Eingaben : li	: Intervall-Langzahl.
  *		p	: # Langzahlziffern zur Basis 2^16.
  *		g	: # Schutzziffern.
  *  Ergebnis : ---
  */

#define LiDelete(LiReal, Size) \
	MemDelete (LiReal, Size)
 /***********************************************************************
  *  Funktion : Freigabe einer Intervall-Langzahl.
  *  Eingaben : Size	: Speicherbedarf in Bytes.
  *		LiReal	: Langzahl.
  *  Ergebnis : ---
  */

/**************************************
*  LiOp0.c
**************************************/

INT FAR CALL LiHull (LREAL lb, LREAL hb, LIREAL a, SHORT inner);
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

INT FAR CALL LiBound (LREAL t, LIREAL a, INT rc, LDIGIT bound);
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

INT FAR CALL LiInfSup (LIREAL a, LREAL is, INT is_sup, SHORT inner);
 /***********************************************************************
  *  Funktion : Bestimmung der unteren bzw. oberen Grenze eines Intervalls:
  *		is = inf(a) bzw. is = sup(a).
  *  Speicher : Bedarf fuer "is" in Bytes:
  *		  LrDigits (is) * sizeof (LDIGIT) + sizeof (LREALHDR))
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

INT FAR CALL LiDiam (LIREAL a, LREAL d, SHORT inner);
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

INT FAR CALL LiAssign (LIREAL a, LIREAL b, SHORT inner);
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

INT FAR CALL LiMidRad (LREAL a, LREAL d, LIREAL b, SHORT inner);
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

/**************************************
*  LiOp1.c
**************************************/

INT FAR CALL LiAdd (LIREAL a, LIREAL b, LIREAL c, SHORT inner);
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

INT FAR CALL LiSub (LIREAL a, LIREAL b, LIREAL c, PVOID wrk, SHORT inner);
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

INT FAR CALL LiMul (LIREAL a, LIREAL b, LIREAL c, PVOID wrk, SHORT inner);
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

INT FAR CALL LiDiv (
  LIREAL a, LIREAL b, LIREAL c, PVOID wrk, PVOID wrk_ival, SHORT inner);
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

INT FAR CALL LiNeg (LIREAL a, LIREAL b);
 /***********************************************************************
  *  Funktion : Negation eines Langzahl-Intervalls: b = - a.
  *  Speicher : Bedarf fuer "b" in Bytes:
  *		  Platz fuer "a".
  *  Ergebnis : Ergebnis der oberen Grenze im hoeher- und der unteren Grenze
  *		im niederwertigen Byte. Pro Grenze sind moeglich:
  *		(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  */

VOID FAR CALL LiPack (LIREAL c);
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

VOID FAR CALL LiFar (LIREAL a);
 /***********************************************************************
  *  Funktion : Berechnung der von Null weiter entfernten Grenze einer Inter-
  *		vall-Langzahl in (0-Grenze, Durchmesser)-Darstellung. Die
  *		berechnete Grenze ueberschreibt die 0-Grenze (der Aufruf der
  *		LiNear() stellt den urspruenglichen Zustand wieder her).
  *  Annahmen : "a" muss in (0-Grenze, Durchmesser)-Darstellung vorliegen.
  *  Ergebnis : ---
  */

VOID FAR CALL LiNear (LIREAL a);
 /***********************************************************************
  *  Funktion : Wiederherstellung der urspruenglichen Werte, nachdem die
  *		vorhergehende Routine aufgerufen wurde.
  *  Annahmen : "a" muss in (0-Grenze, Durchmesser)-Darstellung vorliegen und
  *		LiFar() bereits aufgerufen worden sein.
  */

INT FAR CALL LrRound (LREAL a, INT dir, INT rc, LDIGIT bound, SHORT inner);
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

/**************************************
*  LiCvt1.c
**************************************/

USHORT FAR CALL litosze (
  LIREAL a, PCHAR s, USHORT FracDigNum, USHORT ExpDigNum,
  PCHAR s1, PCHAR s2, PVOID wrk1, PVOID wrk2, PVOID wrk3, USHORT mode);
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

USHORT FAR CALL litoszf (
  LIREAL a, PCHAR s, USHORT IntDigNum, USHORT FracDigNum,
  PCHAR s1, PCHAR s2, PVOID wrk1, PVOID wrk2, PVOID wrk3, USHORT mode);
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

/**************************************
*  LiCvt2.c
**************************************/

INT FAR CALL lisztoaus (
  PCHAR str, PCHAR lrstr, PUSHORT ausle, PUSHORT ausri, USHORT na);
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
  *  Speicher : Bedarf fuer "ausle", "ausri" in Bytes:
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

INT FAR CALL liaustoli (
  PUSHORT ausle, PUSHORT ausri, INT rc, LIREAL c, PVOID wrk1, PVOID wrk2,
  PVOID wrk3);
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

INT FAR CALL lisztoli (
  PCHAR str, PCHAR lrstr, LIREAL c, PVOID wrk1, PVOID wrk2, PVOID wrk3,
  PVOID wrk4, PVOID wrk5);
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

#if defined (__cplusplus)
}
#endif
#endif /* __LI__ */
