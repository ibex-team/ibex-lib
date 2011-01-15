/************************************************************************
*  $Id: Lr.h			Langzahlarithmetik
*
*  Copyright (C) 1989, 1991, 1992, 1993 Dirk Husung
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
*  Version 1.2	06.06.91	Dirk Husung
*	umbenannt: alt: sztoaus()  neu: lrsztoaus()
*			austolr()	lraustolr()
*			sztolr()	lrsztolr()
*			lrtosz()	lrtosze()
*	geaendert: LrDiv(), lraustolr(), lrtosze(), lrsztolr(): erhoehter
*	Bedarf an Arbeitsspeicher.
*  Version 1.3	01.08.91	Dirk Husung
*	entfernt : LrAbsCmp()
*	neu	 : _LrAbsCmp(): frueheres LrAbsCmp() fuer gleiche Exponenten
*  Version 1.4	03.10.92	Dirk Husung
*	neu	 : ddtolr()
*	neu	 : LrScalp()
*	neu	 : LrNewAccu(), LrDeleteAccu(), LrResetAccu(), LrSetAccu(),
*		   LrIncAccu(), LrDecAccu(), LrReadAccu().
*	geaendert: ARGS()-Makro eingefuehrt.
*  Letzte Aenderung:		Wed 06-Oct-1993 13:59:00
************************************************************************/

#ifndef	__LR__
#define	__LR__

#if defined (__cplusplus)
extern "C" {
#endif

#include <lr/LrDefs.h>

#define	CALL

/************************************************************************
*  Konstanten- und Typ-Definitionen
************************************************************************/

#define	 LR_RND_NEAR	0
#define	 LR_RND_DOWN	1
#define	 LR_RND_UP	2
#define	 LR_RND_CHOP	3

#define	 LR_LT	       -1
#define	 LR_EQ		0
#define	 LR_EQ_INFINITY	256
#define	 LR_GT		1

#define	 LR_ZERO	0x0000
#define	 LR_POS_INFINITY 0x0001
#define	 LR_NEG_INFINITY (LR_POS_INFINITY+1)
#define	 LR_NAN		0x0003

#define	 LR_NORMAL	0x0004
#define	 LR_LOSS	0x0008

#define	 LR_POSITIVE	0x0000
#define	 LR_NEGATIVE	0x8000
#define	 LR_PRECISION	0x7FFF

#define	 LR_MIN_EXP    -0x7FFF	/* == -32767 Basis 2^16; -157821 Basis 10  */
#define	 LR_MAX_EXP	0x7FFF	/* ==  32767 Basis 2^16;  157821 Basis 10  */
#define	 LR_MAX_DIGITS	0x7FD6	/* Gesamtspeicherbedarf	<= 0xFFF0 Bytes	   */
				/* - sizeof (LREALHDR)			   */
				/* - 32*sizeof (LDIGIT)	(32 Schutzziffern) */
				/* entspricht etwa 157624 Dezimalziffern   */
#define	 LR_BASE	0x10000L
#define	 LR_BASE_1	0xFFFF
#define	 LR_ROUND	((USHORT) 0x8000)
#define  BITS_PER_LDIGIT 16

typedef	USHORT		LDIGIT;
typedef	LDIGIT FAR *	PLDIGIT;

typedef	struct {
  USHORT info;
  SHORT	 exponent;
} LREALHDR, FAR	*LREAL;

typedef LREAL FAR *	PLREAL;

typedef struct LRACCU_ {
  USHORT	nAccuDigits;
  SHORT 	State;
  LREAL 	lrPosAccu;
  LREAL 	lrNegAccu;
} LRACCU;

typedef LRACCU FAR *	PLRACCU;

/************************************************************************
*  Funktions-Deklarationen
************************************************************************/

/**************************************
*  Makro-Definitionen
**************************************/

#define LrSize(lr)		(LrDigits (lr) * sizeof (LDIGIT) \
				+ sizeof (LREALHDR))		      /* 2 */
				/* Speicherbedarf in Bytes */

#define	LrPMSD(lr)		((PLDIGIT) ((lr)+1))
				/* Zeiger auf signifikanteste Ziffer	*/

#define	LrIsNegative(lr)	(((lr) -> info & LR_NEGATIVE) != 0)
#define	LrIsSpecial(lr)		(((lr) -> info & LR_PRECISION) == 0)

#define	LrIsZero(lr)		((lr) -> exponent == LR_ZERO)	      /* 1 */
#define	LrIsPosInfinity(lr)	((lr) -> exponent == LR_POS_INFINITY) /* 1 */
#define	LrIsNegInfinity(lr)	((lr) -> exponent == LR_NEG_INFINITY) /* 1 */
#define	LrIsNaN(lr)		((lr) -> exponent == LR_NAN)	      /* 1 */

#define	LrSign(lr)		((lr) -> info &	LR_NEGATIVE)
#define	LrDigits(lr)		((SHORT) ((lr) -> info & LR_PRECISION))
#define	LrSpecial(lr)		((lr) -> exponent)		      /* 1 */
#define	LrExponent(lr)		((lr) -> exponent)		      /* 2 */

#define	LrSetPositive(lr)	((lr) -> info &= ~LR_NEGATIVE)
#define	LrSetNegative(lr)	((lr) -> info |=  LR_NEGATIVE)
#define	LrSetSign(lr, s)	((lr) -> info &= ~LR_NEGATIVE, \
				 (lr) -> info |=  (s))

#define	LrSetPosNormal(lr)	((lr) -> info &= ~LR_NEGATIVE)
				/* loescht das Vorzeichenbit */
#define LrSetPosSpecial(lr)	((lr) -> info = 0)
				/* loescht das Vorzeichenbit */
#define LrSetSignedSpecial(lr, s)	((lr) -> info = s)

#define	LrSetZero(lr)		((lr) -> info &= LR_NEGATIVE, \
				 (lr) -> exponent = LR_ZERO)
#define	LrSetPosZero(lr)	((lr) -> info =	0, \
				 (lr) -> exponent = LR_ZERO)
#define	LrSetNegZero(lr)	((lr) -> info =	LR_NEGATIVE, \
				 (lr) -> exponent = LR_ZERO)
#define	LrSetSignedZero(lr, s)	((lr)	-> info	= s,	\
				 (lr) -> exponent = LR_ZERO)

#define	LrSetPosInfinity(lr)	((lr) -> info =	0, \
				 (lr) -> exponent = LR_POS_INFINITY)
#define	LrSetNegInfinity(lr)	((lr) -> info =	LR_NEGATIVE, \
				 (lr) -> exponent = LR_NEG_INFINITY)

#define	LrSetNaN(lr)		((lr) -> info =	0, \
				 (lr) -> exponent = LR_NAN)

#define	LrSetDigits(lr,	d)	((lr) -> info &= LR_NEGATIVE, \
				 (lr) -> info |= (d))
#define	LrSetPosDigits(lr, d)	((lr) -> info =	(d))
				/* loescht das Vorzeichenbit */

#define	LrSetSignedDigits(lr, s, d)  ((lr) -> info = (s) | (d))

#define LrNegative(lr)		((lr) -> info ^= LR_NEGATIVE)

/********************
*  1 : nur definiert, falls LrIsSpecial() == TRUE
*  2 : nur definiert, falls LrIsSpecial() == FALSE
********************/

/**************************************
*  Assembler-Routinen
**************************************/

#if defined (__ASSEMBLER__)

/**************************************
*  LrOpa.asm
**************************************/

extern INT FAR LrAsmAdd	(PUSHORT a, USHORT a_idx, PUSHORT b, USHORT b_idx);
  /**********************************************************************
  *  Funktion :	Addiert	Vektor "b" auf Vektor "a", behandelt Uebertraege.
  *  Annahmen :	a_len >= b_len
  *  Ergebnis :	Gesamtuebertrag
  */

extern INT FAR LrAsmSub	(
  PUSHORT a, USHORT a_idx, PUSHORT b, USHORT b_idx, USHORT borrow);
  /**********************************************************************
  *  Funktion :	Subtrahiert Vektor "b" von Vektor "a", behandelt Borgen.
  *  Annahmen :	a_len >= b_len,	a > b
  *  Ergebnis :	Gesamtborgen
  */

extern USHORT FAR LrAsmScale (USHORT na, PUSHORT a, USHORT b);
  /**********************************************************************
  *  Funktion :	Multipliziert Vektor "a" mit Skalar "b"
  *  Eingaben :	na : Anzahl der	Woerter	des Vektors a
  *		a  : Adresse des Vektors a
  *		b  : Skalar b
  *  Ergebnis :	Fuehrende Ziffer des Produkts (wird nicht gespeichert)
  */

extern VOID FAR	LrAsmAddScale (USHORT na, PUSHORT a, USHORT b, PUSHORT c);
  /**********************************************************************
  *  Funktion :	Addiert	Vektor "a" multipliziert mit Skalar "b"	auf Vektor "c"
  *  Ergebnis :	---
  */

extern INT FAR LrAsmSubScale (
  INT na, PUSHORT a, INT incr_a, USHORT	b, PUSHORT c);
  /**********************************************************************
  *  Funktion :	c -= (a	+ (incr_a != 0))*b; rc = 0;
  *		loop {
  *		  if (c	>= 0) return (rc);
  *		  c += a + (incr_a != 0); rc++;
  *		}
  *  Ergebnis :	rc
  */

extern USHORT FAR LrAsmDiv (
  USHORT a3, USHORT a2,	USHORT a1, USHORT b2, USHORT b1);
  /**********************************************************************
  *  Funktion :	Berechnet q = (a1*B^2+a2*B+a3)/(b1*B+b2), B = 2^16
  *  Annahmen :	0 <= q <= B
  *  Ergebnis :	q < B ?	q : q-1
  */

extern VOID FAR	LrAsmHorner10000 (USHORT na, PUSHORT a);
  /**********************************************************************
  *  Funktion :	Konvertiert einen Vektor "a" mit "na" Ziffern zur Basis	10^4
  *		in einen Vektor	mit Ziffern zur	Basis 2^16
  *  Annahmen :	na >= 1
  *  Ergebnis :	---
  */

extern INT FAR LrAsmShl	(PUSHORT Data, USHORT Size, USHORT Shift);
  /**********************************************************************
  *  Funktion :	Verschiebt ein Feld von	16-Bit-Worten um 1..7 Bitpositionen
  *		nach LINKS.
  *		Bei Ueberlauf (eine der	fuehrenden "Shift" Bitpositionen ist
  *		urspruenglich 1) wird um 1 Wort	nach rechts verschoben.
  *  Eingaben :	Data	: Zeiger auf zu	verschiebendes Feld von	16-Bit-Worten
  *		Size	: # Worte
  *		Shift	: # Bitpositionen (1..7), um die verschoben werden
  *			  soll.
  *  Ergebnis :	0x00, falls ohne Ueberlauf verschoben werden konnte.
  *		0x01, falls Ueberlauf eintrat und die deshalb nach rechts
  *		      herausgeschobenen	letzten	16 - "Shift" Bits == 0 waren.
  *   LR_LOSS +	0x01, falls Ueberlauf eintrat und eines	der deshalb nach rechts
  *		      herausgeschobenen	letzte 16 - "Shift" Bits != 0 war.
  *  Info     :	LR_LOSS	>= 0x0100
  */

extern INT FAR LrAsmShr	(PUSHORT Data, USHORT Size, USHORT Shift);
  /**********************************************************************
  *  Funktion :	Verschiebt ein Feld von	16-Bit-Worten um 1..7 Bitpositionen
  *		nach RECHTS.
  *		Bei Unterlauf (alle fuehrenden 16 - "Shift" Bitpositionen sind
  *		urspruenglich 0) wird um 1 Wort	nach links verschoben.
  *  Eingaben :	Data	: Zeiger auf zu	verschiebendes Feld von	16-Bit-Worten
  *		Size	: # Worte
  *		Shift	: # Bitpositionen (1..7), um die verschoben werden
  *			  soll.
  *  Ergebnis :	0x00, falls ohne Unterlauf verschoben werden konnte und
  *		      alle herausgeschobenen letzten "Shift" Bits 0 waren.
  *   LR_LOSS +	0x00, falls ohne Unterlauf verschoben werden konnte und
  *		      eines der	herausgeschobenen letzten "Shift" Bits 1 war.
  *		0xFF, falls Unterlauf eintrat.
  *  Info     :	LR_LOSS	>= 0x0100
  */

/**************************************
*  LrCvta.asm
**************************************/

extern INT FAR dtolr (LREAL lr, PDOUBLE a);
  /**********************************************************************
  *  Funktion :	Konvertierung: DOUBLE -> LREAL.
  *  Speicher :	Bedarf fuer "lr" in Bytes:
  *		  sizeof (LREALHDR) + 6	* sizeof (LDIGIT)
  *  Ergebnis :	LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL.
  *  Info     :	Es wird	die kuerzeste LREAL-Zahl geliefert, die	"a" exakt
  *		darstellt.
  */

extern INT FAR lrtod (PDOUBLE a, LREAL lr, USHORT rnd);
  /**********************************************************************
  *  Funktion :	Konvertierung: LREAL ->	DOUBLE.
  *  Eingaben :	rnd: Rundung
  *		  LR_RND_NEAR	: zur naechsten		 DOUBLE-Zahl
  *		  LR_RND_DOWN	: zur naechst-kleineren	 DOUBLE-Zahl
  *		  LR_RND_UP	: zur naechst-groesseren DOUBLE-Zahl
  *		  LR_RND_CHOP	: Rundung durch	Abschneiden
  *  Ergebnis :	(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS bei Genauigkeitsverlust.
  */

extern INT FAR ddtolr (DOUBLE a, DOUBLE b, LREAL lr);
  /**********************************************************************
  *  Funktion : Exakte Multiplikation zweier DOUBLE-Zahlen und Speicherung
  *		als Langzahl.
  *  Speicher : Bedarf fuer "lr" in Bytes:
  *		  sizeof (LREALHDR) + 8 * sizeof (LDIGIT)
  *  Ergebnis : LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL
  *  Info     : Es wird die kuerzeste LREAL-Zahl berechnet, die das Produkt
  *		exakt darstellt.
  */

#endif /* __ASSEMBLER__	*/

/**************************************
*  C-Routinen
**************************************/

/**************************************
*  LrMem.c
**************************************/

extern VOID FAR CALL LrInit ARGS ((VOID));
  /**********************************************************************
  *  Funktion :	Initialisierung	bei Programmstart.
  *  Ergebnis :	---
  */

extern VOID FAR CALL LrReset ARGS ((VOID));
  /**********************************************************************
  *  Funktion :	Reinitialisierung.
  *  Ergebnis :	---
  */

#define LrNewVar(p)		LrNew (p, ((((p) + 1) & ~1) << 1) + 4)
  /**********************************************************************
  *  Funktion :	Speicherallokierung fuer eine Langzahl ohne Schutzziffern.
  *  Eingaben : p	: # Langzahlziffern zur Basis 2^16. (Wird "p" um 1
  *			  inkrementiert, entspricht dies in etwa der Ver-
  *			  laengerung der Mantisse um 4.816 Dezimalstellen.)
  *  Ergebnis :	Zeiger auf Speicherbereich.
  *  Info     :	Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */

#define LrNewTemp(p, g)		LrNew (p, ((((p)+(g)+1) & ~1) << 1) + 4)
  /**********************************************************************
  *  Funktion :	Speicherallokierung fuer eine Langzahl mit Schutzziffern.
  *  Eingaben : p	: # Langzahlziffern zur Basis 2^16. (Wird "p" um 1
  *			  inkrementiert, entspricht dies in etwa der Ver-
  *			  laengerung der Mantisse um 4.816 Dezimalstellen.)
  *		g	: # Schutzziffern.
  *  Ergebnis :	Zeiger auf Speicherbereich.
  *  Info     :	Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */

LREAL FAR CALL LrNew ARGS ((SIZE Digits, SIZE Size));
  /**********************************************************************
  *  Funktion :	Speicherallokierung fuer eine Langzahl.
  *  Eingaben : Digits	: # Ziffern zu Basis 2^16.
  *		Size	: Speicherbedarf in Bytes.
  *  Ergebnis :	Zeiger auf Langzahl.
  *  Info     :	Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */

#define LrDeleteVar(lr, p)	_MemDelete (lr, ((((p) + 1) & ~1) << 1) + 4)
  /**********************************************************************
  *  Funktion :	Freigabe einer Langzahl ohne Schutzziffern.
  *  Eingaben : lr	: Langzahl.
  *		p	: # Langzahlziffern zur Basis 2^16.
  *  Ergebnis : ---
  */

#define LrDeleteTemp(lr, p, g)	_MemDelete (lr, ((((p)+(g)+1) & ~1) << 1) + 4)
  /**********************************************************************
  *  Funktion :	Freigabe einer Langzahl mit Schutzziffern.
  *  Eingaben : lr	: Langzahl.
  *		p	: # Langzahlziffern zur Basis 2^16.
  *		g	: # Schutzziffern.
  *  Ergebnis : ---
  */

#define	LrDelete(lr, Size)	MemDelete (lr, Size)
  /**********************************************************************
  *  Funktion :	Freigabe einer Langzahl.
  *  Eingaben : lr	: Langzahl.
  *		Size	: Speicherbedarf in Bytes.
  *  Ergebnis :	---
  */

/**************************************
*  LrCvt0.c
**************************************/

extern INT FAR CALL ustolr ARGS ((LREAL res, USHORT us));
  /**********************************************************************
  *  Funktion :	Konvertierung: USHORT -> LREAL
  *  Speicher :	Bedarf fuer "res" in Bytes:
  *		  sizeof (LREALHDR) + 2	* sizeof (LDIGIT)
  *  Ergebnis :	LR_ZERO, LR_NORMAL
  *  Info     :	Es wird	die kuerzeste LREAL-Zahl geliefert, die	"us" exakt
  *		darstellt.
  */

extern INT FAR CALL stolr ARGS ((LREAL res, SHORT s));
  /**********************************************************************
  *  Funktion :	Konvertierung: SHORT ->	LREAL
  *  Speicher :	Bedarf fuer "res" in Bytes:
  *		  sizeof (LREALHDR) + 2	* sizeof (LDIGIT)
  *  Ergebnis :	LR_ZERO, LR_NORMAL
  *  Info     :	Es wird	die kuerzeste LREAL-Zahl geliefert, die	"s" exakt
  *		darstellt.
  */

extern INT FAR CALL ultolr ARGS ((LREAL res, ULONG ul));
  /**********************************************************************
  *  Funktion :	Konvertierung: ULONG ->	LREAL
  *  Speicher :	Bedarf fuer "res" in Bytes:
  *		  sizeof (LREALHDR) + 2	* sizeof (LDIGIT)
  *  Ergebnis :	LR_ZERO, LR_NORMAL
  *  Info     :	Es wird	die kuerzeste LREAL-Zahl geliefert, die	"ul" exakt
  *		darstellt.
  */

extern INT FAR CALL ltolr ARGS ((LREAL res, LONG l));
  /**********************************************************************
  *  Funktion :	Konvertierung: LONG -> LREAL
  *  Speicher :	Bedarf fuer "res" in Bytes:
  *		  sizeof (LREALHDR) + 2	* sizeof (LDIGIT)
  *  Ergebnis :	LR_ZERO, LR_NORMAL
  *  Info     :	Es wird	die kuerzeste LREAL-Zahl geliefert, die	"l" exakt
  *		darstellt.
  */

#if !defined (__ASSEMBLER__)

extern INT FAR CALL dtolr ARGS ((LREAL res, PDOUBLE dble));
  /**********************************************************************
  *  Funktion :	Konvertierung: DOUBLE -> LREAL
  *  Speicher :	Bedarf fuer "res" in Bytes:
  *		  sizeof (LREALHDR) + 6	* sizeof (LDIGIT)
  *  Ergebnis :	LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL
  *  Info     :	Es wird	die kuerzeste LREAL-Zahl geliefert, die	"dble" exakt
  *		darstellt.
  */

extern INT FAR ddtolr ARGS ((DOUBLE a, DOUBLE b, LREAL lr));
  /**********************************************************************
  *  Funktion : Exakte Multiplikation zweier DOUBLE-Zahlen und Speicherung
  *		als Langzahl.
  *  Speicher : Bedarf fuer "lr" in Bytes:
  *		  sizeof (LREALHDR) + 8 * sizeof (LDIGIT)
  *  Ergebnis : LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL
  *  Info     : Es wird die kuerzeste LREAL-Zahl berechnet, die das Produkt
  *		exakt darstellt.
  */

extern INT FAR CALL lrtod ARGS ((PDOUBLE dble, LREAL lr, INT Rnd));
  /**********************************************************************
  *  Funktion :	Konvertierung: LREAL ->	DOUBLE
  *  Eingaben :	Rnd	: Rundung
  *		    LR_RND_NEAR	: zur naechsten		 DOUBLE-Zahl
  *		    LR_RND_DOWN	: zur naechst-kleineren	 DOUBLE-Zahl
  *		    LR_RND_UP	: zur naechst-groesseren DOUBLE-Zahl
  *		    LR_RND_CHOP	: Rundung durch	Abschneiden
  *  Ergebnis :	(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS bei Genauigkeitsverlust
  */

#endif /* !__ASSEMBLER__ */

/**************************************
*  LrCvt1.c
**************************************/

extern INT FAR CALL lrsztoaus ARGS ((
  PCHAR pchIn, PPCHAR pchOut, PUSHORT aus, USHORT na));
  /**********************************************************************
  *  Funktion :	Konvertierung: Dezimalzahl-Zeichenkette	-> Langzahlfeld.
  *		Formate der Dezimalzahl-Zeichenkette:
  *		  longreal
  *		    : mantissa exponent?
  *		    .
  *		  mantissa
  *		    : {+-}? (digit+ ("." digit+)?
  *		    | {+-}? ("." digit+ ({eE} {+-}? digit+)?)
  *		    .
  *		  exponent
  *		    : ({eE} {+-}? digit+)?)
  *		    .
  *		Format des Langzahlfeldes:
  *		  info | exponent (Basis 10) | a[1] | a[2] | ... | a[na']
  *		  USHORT LONG		       USHORT USHORT	   USHORT
  *		Bedeutung des Langzahlfeldes (B	= 2^16):
  *		info-Bitfeld wie bei Langzahlen:
  *		  Vorzeichenbit (LR_NEGATIVE) und na' = # tatsaechlich
  *		  bestimmter Ziffern zur Basis B (bei exakter Darstellung
  *		  ist na' < na moeglich).
  *		  Eine spezielle Zahl (0, +oo, -oo, NaN) ist auf den ersten
  *		  vier Bytes des Langzahlfeldes wie eine spezielle Langzahl
  *		  kodiert.
  *		Zahlwert, falls keine spezielle Zahl vorliegt:
  *		  (sum (k = 1..na') a[k]*B^(na'-k)) * 10^exponent
  *  Speicher :	Bedarf fuer "aus" in Bytes:
  *		Falls na = (strlen (pchIn) + 3) >> 2:
  *		  na *	sizeof (USHORT)	+ sizeof (USHORT) + sizeof (LONG)
  *		sonst:
  *		  m * sizeof (USHORT) +	sizeof (USHORT) + sizeof (LONG)  mit
  *		  m = (na * 78913 + 49152) >> 16
  *  Eingaben :	pchIn	: Zeiger auf Dezimalzahl-Zeichenkette.
  *		na	: # zu bestimmender Ziffern zur	Basis B; eine exakte
  *			  Darstellung der Langzahl im Langzahlfeld erfolgt
  *			  stets	fuer
  *			    na = (strlen (pchIn) + 3) >> 2 <= LR_MAX_DIGITS
  *		aus	: Langzahlfeld.
  *  Ausgaben :	pchOut	: Zeiger auf das erste nicht mehr bearbeitete Zeichen
  *			  der Langzahlzeichenkette.
  *  Ergebnis :	LR_ZERO
  *		LR_LOSS	+ LR_ZERO		bei Unterlauf
  *		LR_LOSS	+ LR_POS_INFINITY	bei Ueberlauf
  *		LR_NAN				bei fehlerhafter Eingabe
  *		LR_NORMAL (+ LR_LOSS, falls Genauigkeitsverlust	moeglich)
  */

extern INT FAR CALL lraustolr ARGS ((
  PUSHORT aus, LREAL c, PVOID wrk1, PVOID wrk2, PVOID wrk3, INT Level));
  /**********************************************************************
  *  Funktion :	Konvertierung: Langzahlfeld -> Langzahl.
  *		Format des Langzahlfeldes:
  *		  info | exponent (Basis 10) | a[1] | a[2] | ... | a[na]
  *		  USHORT LONG		       USHORT USHORT	   USHORT
  *		Bedeutung des Langzahlfeldes (B	= 2^16):
  *		info-Bitfeld wie bei Langzahlen:
  *		  Vorzeichenbit (LR_NEGATIVE) und na = # Ziffern zur Basis B
  *		Zahlwert:
  *		  (sum (k = 1..na) a[k]*B^(na-k)) * 10^exponent
  *  Speicher :	Bedarf fuer "c"	in Bytes:
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 16 * sizeof	(LDIGIT)
  *		Bedarf fuer "wrk1", "wrk2" in Bytes (jeweils):
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 16 * sizeof	(LDIGIT)
  *		Bedarf fuer "wrk3" in Bytes:
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 32 * sizeof	(LDIGIT)
  *  Eingaben : aus	: Langzahlfeld, sollte mit nIn >= LrDigits (c) + 4
  *			  bestimmt worden sein.
  *		c	: Langzahl.
  *		wrk1,
  *		wrk2,
  *		wrk3	: Arbeitsspeicher.
  *		Level:
  *		    = 0	: Berechnung der Langzahl mit 1	Schutzziffer.
  *			  LR_LOSS wird zum Funktionsergebnis addiert, wenn
  *			  die Langzahl mit Schutzziffer	moeglicherweise	nicht
  *			  exakt	ist.
  *			  Die berechnete Langzahl ist garantiert betrags-
  *			  kleiner oder gleich der exakten; im Fall von LR_LOSS
  *			  liefert Addition von 4 Einheiten zu der 1. Schutz-
  *			  ziffer ein Ergebnis, das garantiert betragsgroesser
  *			  ist als das exakte.
  *		    > 0	: zusaetzlich: Addition	des halben moeglichen Fehlers,
  *			  Runden zur naechsten Langzahl,
  *			  Entfernen von	0-Ziffern am Ende.
  *			  Zwischen der berechneten Langzahl und	der exakten
  *			  liegt	keine weitere Langzahl der geforderten
  *			  Genauigkeit. Es wird eine korrekt gerundete Langzahl
  *			  geliefert, ausgenommen die exakte Langzahl liegt "in
  *			  der Mitte" zwischen zwei Langzahlen.
  *  Ergebnis :	LR_ZERO
  *		LR_LOSS	+ LR_ZERO		bei Unterlauf
  *		LR_LOSS	+ LR_POS_INFINITY	bei Ueberlauf
  *		LR_NORMAL (+ LR_LOSS, falls Genauigkeitsverlust	moeglich)
  */

extern INT FAR CALL lrsztolr ARGS ((PCHAR pchIn, PPCHAR pchOut, LREAL c,
  PVOID	wrk1, PVOID wrk2, PVOID	wrk3, PVOID wrk4, INT Level));
  /**********************************************************************
  *  Funktion :	Konvertierung: Dezimalzahl-Zeichenkette	-> Langzahl.
  *  Speicher :	Bedarf fuer "c"	in Bytes:
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 16 * sizeof	(LDIGIT)
  *		Bedarf fuer "wrk1" in Bytes:
  *		  m * sizeof (USHORT) +	 sizeof	(LONG) mit
  *		  m = (LrDigits	(c) * 78913 + 364804) >> 16
  *		Bedarf fuer "wrk2", "wrk3" in Bytes:
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 16 * sizeof	(LDIGIT)
  *		Bedarf fuer "wrk4" in Bytes:
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 32 * sizeof	(LDIGIT)
  *  Eingaben :	pchIn	: Zeiger auf Dezimalzahl-Zeichenkette.
  *		c	: Langzahl
  *		wrk1,
  *		wrk2,
  *		wrk3,
  *		wrk4,	: Arbeitsspeicher
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
  *  Ausgaben :	pchOut	: Zeiger auf das erste nicht mehr bearbeitete Zeichen
  *			  der Langzahlzeichenkette.
  *  Ergebnis :	(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  */

/**************************************
*  LrCvt2.c
**************************************/

extern PCHAR FAR CALL lrtosze ARGS ((
  LREAL	a, PCHAR s, USHORT FracDigNum, USHORT ExpDigNum, INT Rnd,
  PVOID	wrk1, PVOID wrk2, PVOID	wrk3, PLONG Exp10));
  /***************************************************************************
  *  Funktion :	Konvertierung: Langzahl	-> Langzahlzeichenkette	zur Basis 10
  *		im Format *d[0].d[1]d[2]...d[m]E*e[1]...e[n]
  *  Speicher :	Bedarf fuer "s"	in Bytes:
  *		  FracDigNum + 14 (FragDigNum =	0 moeglich)
  *		Bedarf fuer "wrk1", "wrk2" in Bytes (jeweils):
  *		  ((M << 16 + 83845) div 78913) * sizeof (LDIGIT)
  *		  + sizeof (LREALHDR) + 14 * sizeof (LDIGIT)
  *		  mit M = (FracDigNum+9) >> 2
  *		Bedarf fuer "wrk3" in Bytes:
  *		  ((M << 16 + 83845) div 78913) * sizeof (LDIGIT)
  *		  + 33 * sizeof (LDIGIT), mit M = (FracDigNum+9) >> 2
  *  Eingaben :	a	   : zu	konvertierende Langzahl.
  *		s	   : Zeichenketten-Darstellung der Langzahl.
  *		FracDigNum : # Nachkommastellen.
  *		ExpDigNum  : # Exponentenstellen.
  *		Rnd	   : Rundungsmodus:
  *			     LR_RND_NEAR, LR_RND_DOWN, LR_RND_UP, LR_RND_CHOP.
  *		wrk1/2/3   : Arbeitsspeicher.
  *  Ausgaben : Exp10	   : Exponent der Langzahl zur Basis 10.
  *  Ergebnis :	Zeiger auf Beginn der Langzahlzeichenkette.
  *  Info     :	Falls FracDigNum == 0, wird  1 Nachkommastelle ausgegeben,
  *		falls ExpDigNum	 == 0, mind. 1 Exponentenstelle.
  */

/**************************************
*  LrOp0.c
**************************************/

extern INT FAR CALL LrUnderflow ARGS ((LREAL a, SHORT Sign));
  /**********************************************************************
  *  Funktion :	Behandlung eines Unterlaufs.
  *  Eingaben :	a	: Langzahl
  *		Sign	: Vorzeichen
  *		    < 0	: negativ
  *		    = 0	: ungeaendert
  *		    > 0	: positiv
  *  Ergebnis :	LR_LOSS	+ LR_ZERO
  */

extern INT FAR CALL LrOverflow ARGS ((LREAL a, SHORT Sign));
  /**********************************************************************
  *  Funktion :	Behandlung eines Ueberlaufs.
  *  Eingaben :	a	: Langzahl
  *		Sign	: Vorzeichen
  *		    < 0	: negativ
  *		    = 0	: ungeaendert
  *		    > 0	: positiv
  *  Ergebnis :	LR_LOSS	+ (LR_POS_INFINITY, LR_NEG_INFINITY)
  */

extern INT FAR CALL LrRoundingOverflow ARGS ((LREAL a));
  /**********************************************************************
  *  Funktion :	Behandlung eines Uebertrags in der signifikantesten Ziffer
  *		beim Runden.
  *  Ergebnis :	LR_LOSS	+ (LR_POS_INFINITY, LR_NEG_INFINITY, LR_NORMAL)
  */

extern INT FAR CALL LrAssign ARGS ((LREAL a, LREAL b, INT Level));
  /**********************************************************************
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

extern INT FAR CALL LrPred ARGS ((LREAL a, SHORT Digits));
  /**********************************************************************
  *  Funktion :	Liefert	den Vorgaenger einer Langzahl ohne Schutzziffer.
  *  Eingaben :	a	: Langzahl
  *		Digits	: # Ziffern
  *		    = 0	: # Ziffern = LrDigits (a)
  *		    > 0	: Digits   >= LrDigits (a); 0-Ziffern am Ende sind
  *			  entfernt, werden ggf.	abschliessend entfernt.
  *  Ergebnis :	LR_NORMAL,
  *		LR_LOSS	+ LR_ZERO	   bei Unterlauf,
  *		LR_LOSS	+ LR_NEG_INFINITY  bei Ueberlauf.
  */

extern INT FAR CALL LrSucc ARGS ((LREAL a, SHORT Digits));
  /**********************************************************************
  *  Funktion :	Liefert	den Nachfolger einer Langzahl ohne Schutzziffer.
  *  Eingaben :	a	: Langzahl
  *		Digits	: # Ziffern
  *		    = 0	: # Ziffern = LrDigits (a)
  *		    > 0	: Digits   >= LrDigits (a); 0-Ziffern am Ende sind
  *			  entfernt, werden ggf.	abschliessend entfernt.
  *  Ergebnis :	LR_NORMAL,
  *		LR_LOSS	+ LR_ZERO	   bei Unterlauf,
  *		LR_LOSS	+ LR_POS_INFINITY  bei Ueberlauf.
  */

extern INT FAR CALL LrShift ARGS ((LREAL lr, LONG Shift, INT Level));
  /**********************************************************************
  *  Funktion :	Multiplikation von lr mit 2^Shift durch	bitweises Schieben.
  *  Eingaben :	lr	: zu shiftende Langzahl.
  *		Shift	: # Bitpositionen fuer Linksshift (Rechtsshift mit
  *			  negativen Werten).
  *		Level	:
  *		    = 0	: Verwendung einer Schutzziffer	(kein Genauigkeits-
  *			  verlust)
  *		    > 0	: keine	Schutzziffer (Genauigkeitsverlust moeglich)
  *  Ergebnis :	(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, bei Genauigkeitsverlust.
  */

/**************************************
*  LrOp1.c
**************************************/

extern INT FAR CALL _LrAbsCmp ARGS ((LREAL a, LREAL b));
  /**********************************************************************
  *  Funktion :	Vergleich der Absolutbetraege zweier Langzahlen	"a", "b"
  *		mit gleichen Exponenten.
  *  Annahmen :	Weder "a" noch "b" sind	spezielle Langzahlen.
  *  Ergebnis :	LR_LT	: |a| <	 |b|
  *		LR_EQ	: |a| == |b|
  *		LR_GT	: |a| >	 |b|
  */

extern INT FAR CALL LrCmp ARGS ((LREAL a, LREAL b));
  /**********************************************************************
  *  Funktion :	Vergleich zweier Langzahlen "a", "b"
  *  Ergebnis :	LR_EQ, LR_EQ_INFINITY, (& 0x00FF == 0)
  *		LR_LT, LR_GT, LR_NAN
  */

extern INT FAR CALL LrAdd ARGS ((LREAL a, LREAL b, LREAL c, INT Level));
  /**********************************************************************
  *  Funktion :	Addition zweier	Langzahlen "a",	"b": c = a+b.
  *  Speicher :	Bedarf fuer "c"	in Bytes:
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 2 *	sizeof (LDIGIT)
  *  Eingaben :	Level:
  *		    = 0	: Berechnung der Summe mit 1 Schutzziffer (intern
  *			  wird mit bis zu 2 Schutzziffern gearbeitet).
  *			  LR_LOSS wird zum Funktionsergebnis addiert, wenn die
  *			  Summe	mit Schutzziffer moeglicherweise nicht exakt
  *			  ist.
  *			  Die Summe ist	garantiert betragskleiner oder -gleich
  *			  der exakten; im Fall von LR_LOSS liefert Addition
  *			  von 2	Einheiten zu der Schutzziffer eine Summe, die
  *			  garantiert betragsgroesser ist als die exakte.
  *		    > 0	: zusaetzlich: Addition	des halben moeglichen Fehlers,
  *			  Runden zur naechsten Langzahl,
  *			  Entfernen von	0-Ziffern am Ende.
  *			  Zwischen der berechneten Summe und der exakten liegt
  *			  keine	weitere	Langzahl der geforderten Genauigkeit.
  *			  Es wird eine korrekt gerundete Summe geliefert,
  *			  ausgenommen die exakte Summe liegt "in der Mitte"
  *			  zwischen zwei	Langzahlen.
  *  Ergebnis :	(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  */

extern INT FAR CALL LrSub ARGS ((LREAL a, LREAL b, LREAL c, INT Level));
  /**********************************************************************
  *  Funktion :	Subtraktion zweier Langzahlen "a", "b":	c = a-b.
  *  Speicher :	Bedarf fuer "c"	in Bytes:
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 2 *	sizeof (LDIGIT)
  *  Eingaben :	Level:
  *		    = 0	: Berechnung der Differenz mit 1 Schutzziffer (intern
  *			  wird mit bis zu 2 Schutzziffern gearbeitet).
  *			  LR_LOSS wird zum Funktionsergebnis addiert, wenn die
  *			  Differenz mit	Schutzziffer moeglicherweise nicht
  *			  exakt	ist.
  *			  Die Differnz ist garantiert betragskleiner oder
  *			  -gleich der exakten; im Fall von LR_LOSS liefert
  *			  Addition von 2 Einheiten zu der Schutzziffer eine
  *			  Differenz, die garantiert betragsgroesser ist	als
  *			  die exakte.
  *		    > 0	: zusaetzlich: Addition	des halben moeglichen Fehlers,
  *			  Runden zur naechsten Langzahl,
  *			  Entfernen von	0-Ziffern am Ende.
  *			  Zwischen der berechneten Differenz und der exakten
  *			  liegt	keine weitere Langzahl der geforderten
  *			  Genauigkeit. Es wird eine korrekt gerundete Differenz
  *			  geliefert, ausgenommen die exakte Differenz liegt
  *			  "in der Mitte" zwischen zwei Langzahlen.
  *  Ergebnis :	(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  */

extern INT FAR CALL LrMul ARGS ((LREAL a, LREAL b, LREAL c, INT Level));
  /**********************************************************************
  *  Funktion :	Multiplikation zweier Langzahlen "a", "b": c = a*b.
  *  Speicher :	Bedarf fuer "c"	in Bytes:
  *		Level <	0:
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 2 *	sizeof (LDIGIT)
  *		Level >= 0:
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 4 *	sizeof (LDIGIT)
  *  Eingaben :	Level:
  *		    < 0	: Berechnung des Produkts ohne Schutzziffer (intern
  *			  wird mit bis zu 2 Schutzziffern gearbeitet), Ent-
  *			  fernen von 0-Ziffern am Ende.
  *			  LR_LOSS wird zum Funktionsergebnis addiert, wenn das
  *			  Produkt moeglicherweise nicht	exakt ist.
  *			  Das Produkt ist garantiert betragskleiner oder
  *			  -gleich dem exakten; im Fall von LR_LOSS liefert
  *			  Addition von	LrDigits (c) + 3  Einheiten zu der
  *			  letzten Ziffer ein Produkt, das garantiert betrags-
  *			  groesser ist als das exakte.
  *		    = 0	: Berechnung des Produkts mit 1	Schutzziffer (intern
  *			  wird mit bis zu 4 Schutzziffern gearbeitet).
  *			  LR_LOSS wird zum Funktionsergebnis addiert, wenn das
  *			  Produkt mit Schutzziffer moeglicherweise nicht
  *			  exakt	ist.
  *			  Das Produkt ist garantiert betragskleiner oder
  *			  -gleich dem exakten; im Fall von LR_LOSS liefert
  *			  Addition von 2 Einheiten zu der Schutzziffer ein
  *			  Produkt, das garantiert betragsgroesser ist als das
  *			  exakte.
  *		    > 0	: zusaetzlich: Addition	des halben moeglichen Fehlers,
  *			  Runden zur naechsten Langzahl,
  *			  Entfernen von	0-Ziffern am Ende.
  *			  Zwischen dem berechneten Produkt und dem exakten
  *			  liegt	keine weitere Langzahl der geforderten
  *			  Genauigkeit. Es wird ein korrekt gerundetes Produkt
  *			  geliefert, ausgenommen das exakte Produkt liegt
  *			  "in der Mitte" zwischen zwei Langzahlen.
  *  Ergebnis :	(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  */

extern INT FAR CALL LrDiv ARGS ((
  LREAL a, LREAL b, LREAL c, PVOID wrk, INT Level));
  /**********************************************************************
  *  Funktion :	Division zweier	Langzahlen "a",	"b": c = a/b.
  *  Speicher :	Bedarf fuer "c"	in Bytes:
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 2 *	sizeof (LDIGIT)
  *		Bedarf fuer "wrk" in Bytes:
  *		  (LrDigits (c) + 32) * sizeof (LDIGIT)
  *  Eingaben :	Level:
  *		    = 0	: Berechnung des Quotienten mit	1 Schutzziffer (intern
  *			  wird mit bis zu 2 Schutzziffern gearbeitet).
  *			  LR_LOSS wird zum Funktionsergebnis addiert, wenn der
  *			  Quotient mit Schutzziffer moeglicherweise nicht
  *			  exakt	ist.
  *			  Der Quotient ist garantiert betragskleiner oder
  *			  -gleich dem exakten; im Fall von LR_LOSS liefert
  *			  Addition von 2 Einheiten zu der Schutzziffer einen
  *			  Quotienten, der garantiert betragsgroesser ist als
  *			  der exakte.
  *		    > 0	: zusaetzlich: Addition	des halben moeglichen Fehlers,
  *			  Runden zur naechsten Langzahl,
  *			  Entfernen von	0-Ziffern am Ende.
  *			  Zwischen dem berechneten Quotienten und dem exakten
  *			  liegt	keine weitere Langzahl der geforderten
  *			  Genauigkeit. Es wird ein korrekt gerundeter Quotient
  *			  geliefert, ausgenommen der exakte Quotient liegt
  *			  "in der Mitte" zwischen zwei Langzahlen.
  *  Ergebnis :	(LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  *  Info     :	"wrk" enthaelt nach Beendigung der Division undefinierte Werte!
  */

/**************************************
*  LrOp2.c
**************************************/

extern INT FAR CALL LrPow10 ARGS ((
  LONG exp10, LREAL res, PVOID wrk1, PVOID wrk2, INT Level));
  /**********************************************************************
  *  Funktion :	Langzahl-Berechnung einer 10er-Potenz:
  *		  res =	10^Exp10, Exp10	>= 0.
  *  Speicher :	Bedarf fuer "res" in Bytes:
  *		  LrDigits (res) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 12 * sizeof	(LDIGIT)
  *		Bedarf fuer "wrk1" in Bytes:
  *		  LrDigits (res) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 12 * sizeof	(LDIGIT)
  *		Bedarf fuer "wrk2" in Bytes:
  *		falls Exp10 < 1024:
  *		  NULL
  *		sonst:
  *		  LrDigits (res) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 12 * sizeof	(LDIGIT)
  *  Eingaben :	Level:
  *		    = 0	: Berechnung der Potenz	mit 2 Schutzziffern (intern
  *			  wird mit bis zu 12 Schutzziffern gearbeitet).
  *			  LR_LOSS wird zum Funktionsergebnis addiert, wenn die
  *			  Potenz mit Schutzziffern moeglicherweise nicht exakt
  *			  ist.
  *			  Die Potenz ist garantiert betragskleiner oder	-gleich
  *			  der exakten; im Fall von LR_LOSS liefert Addition
  *			  von 3/4*LR_BASE Einheiten zu der 2. Schutzziffer eine
  *			  Potenz, die garantiert betragsgroesser ist als die
  *			  exakte.
  *		    > 0	: zusaetzlich: Addition	des halben moeglichen Fehlers,
  *			  Runden zur naechsten Langzahl,
  *			  Entfernen von	0-Ziffern am Ende.
  *			  Zwischen der berechneten Potenz und der exakten liegt
  *			  keine	weitere	Langzahl der geforderten Genauigkeit.
  *			  Es wird eine korrekt gerundete Potenz	geliefert,
  *			  ausgenommen die exakte Potenz	liegt "in der Mitte"
  *			  zwischen zwei	Langzahlen.
  *  Ergebnis :	LR_NORMAL		   bei exakter Potenzberechnung,
  *		LR_LOSS	+ LR_NORMAL,	   falls Genauigkeitsverlust moeglich,
  *		LR_LOSS	+ LR_POS_INFINITY, bei Ueberlauf.
  */

extern VOID FAR CALL LrIncrPow10 ARGS ((LREAL a));
  /**********************************************************************
  *  Funktion :	Aufrunden einer	mit LrPow10() berechneten 10er-Potenz:
  *		  2 Schutzziffern mitverwenden,
  *		  3/4*B	Einheiten zur letzten Ziffer addieren (bei Gesamt-
  *		  uebertrag 1 zusaetzliche Ziffer verwenden),
  *		  0-Ziffern am Ende entfernen.
  *  Speicher :	Bedarf fuer "a"	in Bytes:
  *		  LrDigits (a) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 4 *	sizeof (LDIGIT)
  *  Eingaben :	Mit LrPow10() berechnete 10er-Potenz.
  *  Annahmen :	a ist normale Langzahl (nicht LR_POS_INFINITY).
  *  Ergebnis :	---
  */

/**************************************
*  LrSc.c
**************************************/

extern INT FAR CALL LrScalp ARGS ((
  INT n, PDOUBLE a, PDOUBLE b, LREAL c, INT Level));
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

PLRACCU FAR CALL LrNewAccu ARGS ((USHORT nDigits));
  /**********************************************************************
  *  Funktion : Allokiert einen Langzahl-Akkumulator.
  *  Eingaben : nDigits : # Langzahlziffern des Akkumulators zur Basis 2^16.
  *			  Zusaetzlich werden 3 Schutzziffern gegen Ueberlaeufe
  *			  allokiert.
  *  Ergebnis : Zeiger auf den Akkumulator.
  *  Info     :	Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */

VOID FAR CALL LrDeleteAccu ARGS ((PLRACCU pAccu));
  /**********************************************************************
  *  Funktion : Gibt einen Langzahl-Akkumulator frei.
  */

#define LrResetAccu(pAccu)	(pAccu -> State = 0)
  /**********************************************************************
  *  Funktion : Setzt einen Langzahl-Akkumulator in den Ausgangszustand
  *		zurueck.
  */

INT FAR CALL LrSetAccu ARGS ((PLRACCU pAccu, LREAL lr));
  /**********************************************************************
  *  Funktion : Initialisiert einen Langzahl-Akkumulator mit einer Langzahl.
  *  Ergebnis :	(LR_ZERO, LR_NORMAL, LR_POS_INFINITY, LR_NEG_INFINITY)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  *  Info     : Ggf. erfolgt Rundung zur naechsten Langzahl.
  */

INT FAR CALL LrIncAccu ARGS ((PLRACCU pAccu, LREAL lr));
  /**********************************************************************
  *  Funktion : Addiert eine Langzahl auf einen Langzahl-Akkumulator.
  *  Ergebnis :	(LR_ZERO, LR_NORMAL, LR_POS_INFINITY, LR_NEG_INFINITY)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  *  Info     : Ggf. erfolgt Rundung zur naechsten Langzahl.
  */

INT FAR CALL LrDecAccu ARGS ((PLRACCU pAccu, LREAL lr));
  /**********************************************************************
  *  Funktion : Subtrahiert eine Langzahl von einem Langzahl-Akkumulator.
  *  Ergebnis :	(LR_ZERO, LR_NORMAL, LR_POS_INFINITY, LR_NEG_INFINITY)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  *  Info     : Ggf. erfolgt Rundung zur naechsten Langzahl.
  */

INT FAR CALL LrReadAccu ARGS ((PLRACCU pAccu, LREAL res, INT Level));
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

#if defined (__cplusplus)
}
#endif
#endif /* !__LR__ */
