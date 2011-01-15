/************************************************************************
*  $Id:	LrCvt2.c		Langzahlarithmetik, Konvertierungen, Level 2
*
*  Copyright (C) 1989, 1990, 1991, 1992 Dirk Husung
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
*  Version 1.1	06.08.90	Dirk Husung
*	geaendert: lrtosze(): zusaetzlicher Parameter Exp10
*  Version 1.2	06.06.91	Dirk Husung
*	geaendert: lrtosze(): erhoehter Bedarf an Arbeitsspeicher
*  Letzte Aenderung:		Tue 06-Oct-1992 14:43:26
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

static VOID NEAR AsciiDig	(PCHAR s, USHORT d);

#if !defined (__ASSEMBLER__)
static USHORT NEAR LrAsmScale	(USHORT	na, PUSHORT a, USHORT b);
#endif

/************************************************************************
*  Implementierung globaler Funktionen
*************************************************************************/

PCHAR FAR CALL lrtosze (
  LREAL	a, PCHAR s, USHORT FracDigNum, USHORT ExpDigNum, INT Rnd,
  PVOID	wrk1, PVOID wrk2, PVOID	wrk3, PLONG Exp10)
 /***********************************************************************
  *  Funktion :	Konvertierung: Langzahl	-> Langzahlzeichenkette	zur Basis 10
  *		im Format *d[0].d[1]d[2]...d[m]E*e[1]...e[n]
  *  Speicher :	Bedarf fuer "s"	in Bytes:
  *		  FracDigNum + 14 (FragDigNum =	0 moeglich)
  *		Bedarf fuer "wrk1", "wrk2" in Bytes jeweils:
  *		  sizeof (LREALHDR)
  *		  + ((M	<< 16 +	83845) div 78913) * sizeof (LDIGIT)
  *		  + 14 * sizeof (LDIGIT), mit M = (m+9) >> 2
  *		Bedarf fuer "wrk3" in Bytes jeweils:
  *		    ((M	<< 16 +	83845) div 78913) * sizeof (LDIGIT)
  *		  + 33 * sizeof (LDIGIT), mit M = (m+9) >> 2
  *  Eingaben :	a	   : zu	konvertierende Langzahl.
  *		s	   : Zeichenketten-Darstellung der Langzahl.
  *		FracDigNum : m = # Nachkommastellen.
  *		ExpDigNum  : n = # Exponentenstellen.
  *		Rnd	   : Rundungsmodus:
  *			     LR_RND_NEAR, LR_RND_DOWN, LR_RND_UP, LR_RND_CHOP.
  *		wrk1/2/3   : Arbeitsspeicher.
  *  Ausgaben : Exp10	   : Exponent der Langzahl zur Basis 10.
  *  Ergebnis :	Zeiger auf Beginn der Langzahlzeichenkette.
  *  Info     :	Falls FracDigNum == 0, wird  1 Nachkommastelle ausgegeben,
  *		falls ExpDigNum	 == 0, mind. 1 Exponentenstelle.
  */
{
 /*
  *  Eingabe:
  *  Langzahl a	= sum (k = 1..na) a[k]*B^-k * B^-ea
  *
  *  Ausgabe:
  *  Zeichenkette mit Langzahl-Darstellung der Form
  *    *d[0].d[1]d[2]...d[m]E*e[1]...e[n]
  *
  *  Algorithmus (B = 2^16, log	= Logarithmus zur Basis	10)
  *  Ges.: m+1 Dezimalstellen, 2 Schutzziffern.
  *  Lsg.: Die Langzahl	a wird zunaechst durch Multiplikation mit bzw.
  *	   Division durch eine Zehnerpotenz b =	10^E so	skaliert, dass fuer
  *	   die skalierte Langzahl c gilt:
  *	     10000^-1 <= c < 1
  *	   Durch wiederholte Multiplikation mit	10000 werden die fuehrenden
  *	   Ziffern einer Darstellung zur Basis 10000 berechnet,	diese
  *	   Darstellung wird dann in die	endgueltige Dezimaldarstellung
  *	   konvertiert.
  *
  *  Im	einzelnen:
  *  m+3 Dezimalstellen	erfordern die Berechnung von M = (m+3+6) >> 2 Ziffern
  *  zur Basis 10000 (betrachte: [...d][dddd][d...]).
  *  Die durch die M Ziffern dargestellte Zahl soll betragsmaessig zu klein
  *  sein, 2 Einheiten,	zu der letzten berechneten Ziffer addiert, sollen eine
  *  betragsmaessig zu grosse Zahl ergeben; wird c angenaehert durch
  *    c' = sum	(k = 1..nc) c[k]*B^-k  mit  c' <= c <= c' + dc * B^-nc,
  *  so	ist hinreichend
  *	 dc * B^-nc * 10000^M <= 1	(m-fache Multiplikation	mit 10000)
  *  <=> dc * 10000^M	      <= B^nc
  *  <=> log(dc) + 4*M	      <= nc * log(B)
  *  <=> nc		      >= (4*M +	log(dc)) / log(B)
  *  <=	 nc = ((4*M + log(dc)) * 2^14 +	78912) div 78913
  *  fuer dc = 2:
  *    nc = (M*2^16 + 83845) div 78913
  *
  *  Im	folgenden wird eine Zehnerpotenz b = 10^E zur Skalierung von a auf c
  *  mit 10000^-1 <= c < 1 hergeleitet.
  *
  *  Vorueberlegung:
  *  (a) Es gilt: 0 <= k <= 32769, k ganzzahlig	=>
  *	   k * log(B) -	1.5 < (k * 78913) >> 14	<= k * log(B)
  *	 (kann auf dem Rechner ueberprueft werden)
  *  (b) Es gilt: 0 < k	< 65536, k ganzzahlig,
  *	   L(k)	:= 1, falls   1	<= k <	  10  =>       L(k) <= log(k)+1
  *		   2	     10	<= k <	 100	       L(k) >= log(k+1)
  *		   3	    100	<= k <	1000	  also:
  *		   4	   1000	<= k < 10000	      -L(k) >= -log(k)-1
  *		   5	  10000	<= k < 65536	      -L(k) <= -log(k+1)
  *
  *  Beh.: Es gilt:
  *  (1) ea < 0: 10000^-1 <= c = a*10^E	< 1 mit
  *		 E = ((-ea+1) *	78913) >> 14 - (1 + L(a[1]))
  *  (2) ea = 0: 10000^-1 <= c = a*b < 1 mit
  *		 b = 1000, falls a[1] <= 64
  *			1, sonst
  *  (3) ea > 0: 10000^-1 <= c = a/10^E	< 1 mit
  *		 E = ((ea-1) * 78913) >> 14 + (2 + L(a[1]))
  *
  *  Bew.:
  *  Es	gilt: a[1] * B^-1 * B^ea <= a <	(a[1]+1) * B^-1	* B^ea;	damit
  *
  *  (1):
  *	   10000^-1 <= c = a*10^E
  *    <=  10^-4    <= a[1] * B^(ea-1) * 10^E
  *    <=> -4	    <= log(a[1]) + (ea-1) * log(B) + E
  *    <=> E	    >= (-ea+1) * log(B)	- log(a[1]) - 4	=: RS
  *  Es	gilt:
  *    E =  ((-ea+1) * 78913) >> 14 - (1 + L(a[1]))
  *	 >  (-ea+1) * log(B) - 1.5 - 1 - log(a[1]) - 1
  *	 =  (-ea+1) * log(B) - log(a[1]) - 3.5
  *	 >= RS.		    (E koennte um h <= 0.5 kleiner gewaehlt werden!)
  *
  *	   c = a*10^E <	1
  *    <=  (a[1]+1) * B^(ea-1) * 10^E <	1
  *    <=> log(a[1]+1) + (ea-1)	* log(B) + E < 0
  *    <=> E < (-ea+1) * log(B)	- log(a[1]+1)		=: RS
  *  Es	gilt:
  *    E  = ((-ea+1) * 78913) >> 14 - (1 + L(a[1]))
  *	 <= (-ea+1) * log(B) - 1 - log(a[1]+1)
  *	 <  RS.		    (E koennte um h < 1	groesser gewaehlt werden!)
  *
  *  (2): klar.
  *
  *  (3):
  *	   10000^-1 <= c = a/10^E
  *    <=  10^-4    <= a[1] * B^(ea-1) * 10^-E
  *    <=> -4	    <= log(a[1]) + (ea-1) * log(B) - E
  *    <=> E	    <= (ea-1) *	log(B) + log(a[1]) + 4	=: RS
  *  Es	gilt:
  *    E =  ((ea-1) * 78913) >>	14 + (2	+ L(a[1]))
  *	 <= (ea-1) * log(B) + 2	+ log(a[1]) + 1
  *	 <= RS.		    (E koennte um h <= 1 groesser gewaehlt werden!)
  *
  *	   c = a/10^E <	1
  *    <=  (a[1]+1) * B^(ea-1) * 10^-E < 1
  *    <=> log(a[1]+1) + (ea-1)	* log(B) - E < 0
  *    <=> E > (ea-1) *	log(B) + log(a[1]+1)		=: RS
  *  Es	gilt:
  *    E =  ((ea-1) * 78913) >>	14 + (2	+ L(a[1]))
  *	 >  (ea-1) * log(B) - 1.5 + 2 +	log(a[1]+1)
  *	 >= RS.		    (E koennte um h <= 0.5 kleiner gewaehlt werden!)
  *
  *  Um	die geforderte Skalierung zu erreichen,	darf in	den Faellen (1)	und (3)
  *  der Naeherungswert	fuer die Zehnerpotzenz um den Faktor 3 zu klein	oder
  *  zu	gross sein!.
  *
  *  Nachfolgend werden	Schranken fuer die Genauigkeit angegeben, mit der die
  *  Zehnerpotenz zu berechnen ist.
  *
  *  (1) (ea < 0):
  *  Bestimme 10^E auf nb = nc Stellen mit 2 Schutzziffern und verwende	die
  *  Schutzziffern in der folgenden Rechnung mit. Berechnet wird b' mit
  *    b'*B^eb <= 10^E <= (b' +	db*B^-(nb+2))*B^eb, db <= 3/4*B
  *
  *  Weiter gilt (, da sicher 3*b'*B^eb	>= 10^E):
  *	  B^-1 <= a*B^ea * b'*B^eb < 1	und  B^-1 <= a,	b' < 1
  *    => ea + eb = 0 oder 1
  *  und:
  *    a*B^ea *	db*B^-(nb+2)*B^eb < 1*db*B^(ea+eb-nb-2)	<= db*B^-(nc+1)
  *
  *  Bestimme a*b'*B^(ea+eb) mit 4 Schutzziffern. Berechnet wird c' mit
  *    c' <= a*b'*B^(ea+eb) <= c' + 2*B^-(nc+1)
  *
  *  D.h.: Insgesamt wird ein betragsmaessig zu	kleiner	Wert berechnet,	B Ein-
  *  heiten zur	1. Schutzziffer	addiert, liefern ein betragsmaessig zu grosses
  *  Ergebnis.
  *
  *  Problem: a	kann so	klein werden, dass die 10er-Potenz 10^E	zur Skalierung
  *  nicht mehr	als Langzahl darstellbar ist.
  *  In	diesem Fall wird zur Skalierung	die groesste darstellbare 10er-Potenz
  *  verwendet.
  *  Insbesondere gilt dann ea + eb <= 1, d.h. die Schranke fuer den Fehler-
  *  anteil, der aus dem Fehler	in den Eingangsdaten der Multiplikation
  *  resultiert, bleibt	unveraendert gueltig, ebenso die Abschaetzung des
  *  Gesamtfehlers. Wird nc um 1 inkrementiert,	verschiebt sich	der Fehler in
  *  die naechst-niederwertigere Ziffer.
  *  Bei Skalierung der	kleinsten positiven Langzahl B^-32768 >	0.385E-157826
  *  mit der groessten darstellbaren 10er-Potenz, 1E+157821, liefert hoechstens
  *  die erste Multiplikation mit 10000	eine fuehrende 0-Ziffer. Der moegliche
  *  Fehler in der Langzahldarstellung zur Basis 10000 vergroessert sich also
  *  hoechstens	um den Faktor 10000; dies wird durch Inkrementieren von	nc um 1
  *  mehr als ausgeglichen.
  *
  *  (2) (ea = 0):
  *  Falls mit 1000 multipliziert werden soll, in der (nc+1)-ten Stelle
  *  beginnen; der Gesamtfehler	wird durch 2 Einheiten in der nc-ten Stelle
  *  erfasst.
  *
  *  (3) (ea > 0):
  *  Bestimme 10^E auf nb = nc+1 Stellen mit 2 Schutzziffern und verwende die
  *  Schutzziffern in der folgenden Rechnung mit. Berechnet wird b' mit
  *    b'*B^eb <= 10^E <= (b' +	db*B^-(nb+2))*B^eb, db <= 3/4*B
  *
  *  Weiter gilt (, da sicher 3*b'*B^eb	>= 10^E):
  *	  B^-1 <= a*B^ea / (b'*B^eb) < 1  und  B^-1 <= a, b' < 1
  *    => ea - eb = -1 oder 0
  *
  *  Bestimme a/(b' + db*B^-nb)*B^(ea-eb) mit 2	Schutzziffern. Berechnet wird
  *  c'	mit
  *    c' <= a/(b' + db*B^-(nb+2))*B^(ea-eb) <=	c' + 2*B^-(nc+1)
  *
  *  Sei d erklaert durch:
  *
  *	a				       ! a
  *	---------------	* B^(ea-eb) + d*B^-nc >= -- * B^(ea-eb)
  *	b'+db*B^-(nb+2)				 b'
  *
  *  Dann ist fuer d hinreichend:
  *
  *	   !  a	   a
  *	d >= (-- - ---------------) * B^(ea-eb+nc)
  *	      b'   b'+db*B^-(nb+2)
  *
  *	      a*(b'+db*B^-(nb+2)) - a*b'
  *	   = --------------------------- * B^(ea-eb+nc)
  *	      b'*(b'+db*B^-(nb+2))
  *
  *	      a*b' + a*db*B^-(nb+2) - a*b'
  *	   = ------------------------------ * B^(ea-eb+nc)
  *	      b'*(b'+db*B^-(nb+2))
  *
  *	     a	  1
  *	   = --	* ----------------- * db * B^(ea-eb+nc-(nb+2))
  *	     b'	  (b'+db*B^-(nb+2))
  *
  *	     1	    1
  *  <=	d >= ---- * ---- * db *	B^(ea-eb+nc-(nb+2))
  *	     B^-1   B^-1
  *
  *	   = db*B^(ea-eb+nc-(nb+2)+2) ;	mit ea-eb <= 0,	nb = nc+1:
  *
  *  <=	d >= db*B^-1  tut's!
  *
  *  D.h.: Insgesamt wird ein betragsmaessig zu	kleiner	Wert berechnet,	B Ein-
  *  heiten zur	1. Schutzziffer	addiert, liefern ein betragsmaessig zu grosses
  *  Ergebnis.
  *
  *  Problem: a	kann so	gross werden, dass die 10er-Potenz 10^E	zur Skalierung
  *  nicht mehr	als Langzahl darstellbar ist.
  *  In	diesem Fall wird zur Skalierung	die groesste darstellbare 10er-Potenz
  *  verwendet.
  *  Insbesondere gilt dann ea <= eb = LR_MAX_EXP, d.h.	die Schranke fuer den
  *  Fehleranteil, der aus dem Fehler in den Eingangsdaten der Division
  *  resultiert, bleibt	unveraendert gueltig, ebenso die Abschaetzung des
  *  Gesamtfehlers. Wird nc um 1 inkrementiert,	verschiebt sich	der Fehler in
  *  die naechst-niederwertigere Ziffer.
  *  Bei Skalierung der	groessten positiven Langzahl < B^32767 < 3.962E+157821
  *  mit der groessten darstellbaren 10er-Potenz, 1E+157821, ergibt sich die
  *  fuehrende Ziffer der Langzahldarstellung zur Basis	10000 unmittelbar. Im
  *  folgenden steht eine Ziffer zur Basis B weniger zur Verfuegung; dies wird
  *  durch Inkrementieren von nc um 1 ausgeglichen.
  */
#define	MAX_EXP			157821L
#define	PlusChar		' '	/* fuehrendes Pluszeichen	*/
#define	ExpChar			'E'	/* Exponentzeichen		*/

#define	PosInfinityStr		"+oo"
#define	NegInfinityStr		"-oo"
#define	NaNStr			"NaN"

#define	PosInfinityStrLen	3
#define	NegInfinityStrLen	3
#define	NaNStrLen		3

  USHORT	DigNum,			/* # Ziffern zur Basis 10000	*/
		cLen;			/* # Ziffern zur Basis B = 2^16	*/
  PLDIGIT	cMSD;
  LONG		Exponent;
  PCHAR		DigStrg;
  USHORT	Digit;

  { /* Behandlung der Spezialfaelle */
    if (LrIsSpecial (a)) {
      switch (LrSpecial	(a)) {
	case LR_ZERO :
	    memset (s+1, '0', FracDigNum + ExpDigNum + 4);
	    s [0] = (CHAR) (LrSign (a) ? '-' : PlusChar);
	    s [2] = '.';
	    s [FracDigNum + 3] = ExpChar;
	    s [FracDigNum + 4] = '+';
	    break;
	case LR_POS_INFINITY :
	    memcpy (s, PosInfinityStr, PosInfinityStrLen);
	    memset (s +	PosInfinityStrLen, ' ',
		FracDigNum + ExpDigNum + 5 - PosInfinityStrLen);
	    break;
	case LR_NEG_INFINITY :
	    memcpy (s, NegInfinityStr, NegInfinityStrLen);
	    memset (s +	NegInfinityStrLen, ' ',
		FracDigNum + ExpDigNum + 5 - NegInfinityStrLen);
	    break;
	case LR_NAN :
	    memcpy (s, NaNStr, NaNStrLen);
	    memset (s +	NaNStrLen, ' ',
		FracDigNum + ExpDigNum + 5 - NaNStrLen);
	    break;
      }	/* switch */
      s	[FracDigNum + ExpDigNum	+ 5] = 0;
      return (s);
    }
  } /* Im folgenden liegt keine	spezielle Langzahl vor.	*/

  if (FracDigNum == 0) FracDigNum++;
  if (ExpDigNum	 == 0)
    ExpDigNum =	1;
  else if (ExpDigNum > 6)
    ExpDigNum =	6;

  DigNum = (USHORT) (FracDigNum + 9) >> 2;
  cLen	 = (USHORT) ((((ULONG) DigNum << 16) + 83845L) / 78913L);
  cMSD	 = LrPMSD ((LREAL) wrk2);

  { /* skalierte Langzahl berechnen *************************************/
    LDIGIT  aMSD	= *(LrPMSD (a));
    SHORT   aLogMSD;

    { /* "Logarithmus" der fuehrenden Ziffer der Langzahl bilden ********/
      if (aMSD < 100)
	aLogMSD	= 1 + (aMSD >= 10);
      else if (aMSD < 1000)
	aLogMSD	= 3;
      else
	aLogMSD	= 4 + (aMSD >= 10000);
    }

    if (LrExponent (a) < 0) {
      ULONG E =
	  (((ULONG) (1L	- LrExponent (a)) * 78913L) >> 14) - (1	+ aLogMSD);

      if (E > MAX_EXP) {
	SHORT	Len = ++cLen;		/* # Ziffern um 1 erhoehen,	*/
					/* weil E > MAX_EXP		*/
	PLDIGIT	MSD = LrPMSD ((LREAL) wrk1);

	LrSetPosDigits ((LREAL)	wrk1, Len);	/* wrk1	= 10^MAX_EXP	*/
	LrPow10 (MAX_EXP, (LREAL) wrk1, wrk2, wrk3, 0);
	Len += 2;			/* 2 Schutzziffern mitverwenden	*/
	while (MSD [--Len] == 0)	/* 0-Ziffern am	Ende entfernen	*/
	  ;
	LrSetPosDigits ((LREAL)	wrk1, Len+1);

	LrSetPosDigits ((LREAL)	wrk2, cLen);	/* wrk2	= a / wrk1	*/
	LrMul (a, (LREAL) wrk1, (LREAL) wrk2, 0);

	if (LrExponent ((LREAL)	wrk2)) {
	  /* LrExponent	== -1: Exponent	durch 0-Ziffer ersetzen,	*/
	  /* erste Multiplikation mit 10000 liefert fuehrende 0-Ziffer.	*/
	  *--cMSD = 0;
	  while	(cMSD [--cLen] == 0)	/* 0-Ziffern am	Ende entfernen	*/
	    ;
	  LrAsmScale (++cLen, cMSD, 10000);
	  Digit	= LrAsmScale (cLen, cMSD, 10000);
	  Exponent = (LONG) -MAX_EXP - 4;
	}
	else /*	if (LrExponent == 0) */	{
	  /* LrExponent	== 0: erste Multiplikation mit 10000 kann	*/
	  /* fuehrende 0-Ziffer	liefern.				*/
	  while	(cMSD [--cLen] == 0)	/* 0-Ziffern am	Ende entfernen	*/
	    ;
	  if ((Digit = LrAsmScale (++cLen, cMSD, 10000)) == 0) {
	    Digit = LrAsmScale (cLen, cMSD, 10000);
	    Exponent = (LONG) -MAX_EXP - 4;
	  }
	  else {
	    cLen--;			/* # Ziffern um	1 verkleinern	*/
	    Exponent = -MAX_EXP;
	  }
	}
      }
      else /* if (E <= MAX_EXP)	*/ {
	SHORT	Len = cLen;
	PLDIGIT	MSD = LrPMSD ((LREAL) wrk1);

	LrSetPosDigits ((LREAL) wrk1, Len);	/* wrk1 = 10^E		*/
	LrPow10 (E, (LREAL) wrk1, wrk2, wrk3, 0);
	Len += 2;			/* 2 Schutzziffern mitverwenden	*/
	while (MSD [--Len] == 0)	/* 0-Ziffern am	Ende entfernen	*/
	  ;
	LrSetPosDigits ((LREAL)	wrk1, Len+1);

	LrSetPosDigits ((LREAL)	wrk2, cLen);	/* wrk2	= a * wrk1	*/
	LrMul (a, (LREAL) wrk1, (LREAL) wrk2, 0);

	while (cMSD [--cLen] ==	0)	/* 0-Ziffern am	Ende entfernen	*/
	  ;
	Digit =	LrAsmScale (++cLen, cMSD, 10000);
	Exponent = -E;
      }
    } /* if (LrExponent	(a) < 0) */
    else if (LrExponent	(a) > 0) {	/* cLen besitzt Originalwert	*/
      ULONG E	 =
	  (((ULONG) (LrExponent	(a) - 1L) * 78913L) >> 14) + (2	+ aLogMSD);

      if (E > MAX_EXP) {
	SHORT	Len = ++cLen+2; 	/* # Ziffern um 1 erhoehen,	*/
					/* weil E > MAX_EXP		*/
	PLDIGIT	MSD = LrPMSD ((LREAL) wrk1);

	LrSetPosDigits ((LREAL)	wrk1, cLen+1);	/* wrk1	= 10^MAX_EXP	*/
	LrPow10 (MAX_EXP, (LREAL) wrk1, wrk2, wrk3, 0);

	/* 2 Schutzziffern mitverwenden,				*/
	/* 3/4*B Einheiten zur letzten Ziffer addieren,			*/
	/* 0-Ziffern am	Ende entfernen.					*/
	if (MSD	[Len] <	0x4000)	{	/* Addition von	3/4*B		*/
	  MSD [Len] += 0xC000;		/* ohne	Uebertrag moeglich	*/
	  LrSetPosDigits ((LREAL) wrk1,	Len+1);
	}
	else {
	  LDIGIT Dig = MSD [Len] += 0xC000;

	  if (Dig)
	    LrSetPosDigits ((LREAL) wrk1, cLen+3);
	  else {
	    while ((LDIGIT) ++(MSD [--Len]) == 0)
	      ;
	    LrSetPosDigits ((LREAL) wrk1,  Len+1);
	  }
	}

	LrSetPosDigits ((LREAL)	wrk2, cLen);	/* wrk2	= a / wrk1	*/
	LrDiv (a, (LREAL) wrk1, (LREAL) wrk2, wrk3, 0);

	if (LrExponent ((LREAL)	wrk2)) {
	  /* Exponent == 1: fuehrende Ziffer ist fuehrende Ziffer zur	*/
	  /* Basis 10000; im folgenden werden die cLen-1 verbleibenden	*/
	  /* Ziffern verwendet.						*/
	  Digit	= *cMSD++; cLen--;
	  while	(cLen && cMSD [--cLen] == 0)
	    ;				/* 0-Ziffern am	Ende entfernen	*/
	  ++cLen;			/* mindestens 1 Ziffer fuer	*/
					/* LrAsmScale			*/
	  Exponent = MAX_EXP + 4;
	}
	else /*	if (LrExponent == 0) */	{
	  cLen--;			/* # Ziffern um	1 verkleinern	*/
	  while	(cMSD [--cLen] == 0)	/* 0-Ziffern am	Ende entfernen	*/
	    ;
	  Digit	= LrAsmScale (++cLen, cMSD, 10000);
	  Exponent = MAX_EXP;
	}
      }
      else /* if (E <= MAX_EXP)	*/ {
	LrSetPosDigits ((LREAL)	wrk1, cLen+1);	/* wrk1	= 10^E		*/
	LrPow10 (E, (LREAL) wrk1, wrk2, wrk3, 0);
	LrIncrPow10 ((LREAL) wrk1);

	LrSetPosDigits ((LREAL)	wrk2, cLen);	/* wrk2	= a / wrk1	*/
	LrDiv (a, (LREAL) wrk1, (LREAL) wrk2, wrk3, 0);
	while (cMSD [--cLen] ==	0)	/* 0-Ziffern am	Ende entfernen	*/
	  ;
	Digit =	LrAsmScale (++cLen, cMSD, 10000);
	Exponent = E;
      }
    }
    else {
      if (aMSD <= 64) {
	LrSetPosDigits ((LREAL)	wrk2, cLen+1);
	LrAssign ((LREAL) wrk2, a, 0);
	LrAsmScale (cLen+1, cMSD, 1000);
	LrSetPosDigits ((LREAL)	wrk2, cLen);
	Exponent = -3;
      }
      else {
	LrSetPosDigits ((LREAL)	wrk2, cLen);
	LrAssign ((LREAL) wrk2, a, 0);
	Exponent = 0;
      }
      while (cMSD [--cLen] == 0)
	;
      Digit = LrAsmScale (++cLen, cMSD,	10000);
    }
  } /* wrk2 haelt die skalierte	Langzahl; es gilt: 10000^-1 <= wrk2 < 1	*/

  {/* Dezimalziffern berechnen ******************************************
    *
    *  DigNum Ziffern zur Basis	10000 abspalten
    *
    *	*x.yyyE*zzzz
    *  012345678901234
    *  ...xyyyhhkkkkkk
    *			fuehrende Ziffer     Offset  Exponentkorrektur
    *  ...xyyy		1000 <=	. < 10000   3	    *	 10 E-1
    *  ..0xyy		 100 <=	. < 1000    2	    *	100 E-2
    *  .00xy		  10 <=	. < 100	    1	    *  1000 E-3
    *  000x		   1 <=	. < 10	    0	    * 10000 E-4
    */
    SHORT  Offset;

    if (Digit <	100)
      Offset = (Digit >= 10);
    else
      Offset = (Digit >= 1000) + 2;

    Exponent +=	(Offset	- 4);
    DigStrg = s	+ Offset;

    AsciiDig (DigStrg, Digit);
    while (--DigNum) 
      AsciiDig (DigStrg	+= 4, LrAsmScale (cLen,	cMSD, 10000));
    DigStrg += 4;
  } /* Dezimalziffern berechnen	*/

  { /* Rundung **********************************************************/
    USHORT Incr;

    switch (Rnd) {
      case LR_RND_NEAR : Incr =	s [FracDigNum +	4] >= '5'; break;
      case LR_RND_DOWN : Incr =	LrSign (a) ? 2 : 0;	   break;
      case LR_RND_UP   : Incr =	LrSign (a) ? 0 : 2;	   break;
      default	       : Incr =	0;			   break;
    }
    if (Incr ==	2) {
      PCHAR t =	DigStrg;
      SHORT k =	(SHORT)	(t - s)	- (FracDigNum +	4);

      while (k-- && *--t == '9')
	;
      if (k >= 0) Incr = 1;
    }
    if (Incr) {
      PCHAR  t = s + FracDigNum	+ 3;

      if ((*t += (CHAR)	Incr) >	'9') {
	SHORT k	= FracDigNum;

	*t-- -=	10;
	while (k--)
	  if (*t == '9')
	    *t-- = '0';
	  else {
	    (*t)++; k =	0; break;
	  }
	Incr = k;
      }
      else
	Incr = 0;
    }
    if (Incr) {
      s	[1] = '1'; Exponent++;
    }
    else {
      s++; s [1] = s [2];
    }
    s [0] = (CHAR) (LrSign (a) ? '-' : PlusChar);
    s [2] = '.';
  } /* Rundung ausgefuehrt */

  { /* Exponenten eintragen *********************************************/
    PCHAR	t = s +	(FracDigNum + 3);
    CHAR	ExpStrg	[9];
    INT		i, k;

    *Exp10 = Exponent;

    *t++ = ExpChar;
    *t++ = (CHAR) ((Exponent < 0) ? (Exponent =	- Exponent, '-') : '+');

    AsciiDig (ExpStrg  , (USHORT) (Exponent / 10000));
    AsciiDig (ExpStrg+4, (USHORT) (Exponent % 10000));
    ExpStrg [8]	= 0;

    for	(i = 0,	k = 8 -	ExpDigNum; k; i++, k--)	/* 01234567. */
      if (ExpStrg [i] != '0')			/* 87654321  */
	break;
    memcpy (t, ExpStrg + i, 9 -	i);
  } /* Exponent	eingetragen */

  return (s);
} /* lrtosze */

/************************************************************************
*  Implementierung lokaler Funktionen
*************************************************************************/

static VOID NEAR AsciiDig (PCHAR s, USHORT d)
 /***********************************************************************
  *  Funktion :	Konvertierung einer Ziffer zur Basis 10000 in 4	Dezimalziffern
  *  Eingaben :	d : Ziffer zur Basis 10000
  *  Ausgaben :	s : Ascii-Darstellung der Eingabeziffer	zur Basis 10
  *  Ergebnis :	---
  */
{
  UCHAR	Digit;

  Digit	= (UCHAR) (d / 100); *s++ = (CHAR) (Digit / 10 + 0x30);
			     *s++ = (CHAR) (Digit % 10 + 0x30);
  Digit	= (UCHAR) (d % 100); *s++ = (CHAR) (Digit / 10 + 0x30);
			     *s	  = (CHAR) (Digit % 10 + 0x30);
} /* AsciiDig */

#if !defined (__ASSEMBLER__)

static USHORT NEAR LrAsmScale (USHORT na, PUSHORT a, USHORT b)
 /***********************************************************************
  *  Funktion :	Multipliziert Vektor "a" mit Skalar "b"
  *  Eingaben :	na : Anzahl der	Woerter	des Vektors a
  *		a  : Adresse des Vektors a
  *		b  : Skalar b
  *  Ergebnis :	Fuehrende Ziffer des Produkts (wird nicht gespeichert)
  */
{
  ULONG	bb = b,
	uu = 0;

  while	(na--)
    a [na] = (USHORT) (uu = a [na] * bb	+ ((USHORT *) &uu) [D1OF2]);
  return ((USHORT *) &uu) [D1OF2];
} /* LrAsmScale	*/

#endif /* !__ASSEMBLER__ */
