/************************************************************************
*  $Id:	LrCvt1.c		Langzahlarithmetik, Konvertierungen, Level 1
*
*  Copyright (C) 1989, 1991 Dirk Husung
*                1993 Olaf Knueppel
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
*	geaendert: lraustolr(), lrsztolr(): erhoehter Bedarf an Arbeitsspeicher
*  Version 1.3  12.10.93        Olaf Knueppel
*       geaendert: lrsztoaus(): temporaere Aenderungen an Parametern entfernt
*                               (siehe OK-Kommentare)
*       geaendert: ltsztoaus(): temporaere Aenderungen an Parametern etwas
*                               modifiziert (muss noch getestet werden)
*  Letzte Aenderung:		Tue 12-Oct-1993 15:40:12
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
*  Implementierung globaler Funktionen
*************************************************************************/

#define SkipWhitespace()	while (*pchIn == ' ' || *pchIn == '\t') pchIn++
#define	IsDigit(c)		(c >= '0' && c <= '9')

INT FAR	CALL lrsztoaus (PCHAR pchIn, PPCHAR pchOut, PUSHORT aus, USHORT na)
 /***********************************************************************
  *  Funktion :	Konvertierung: Dezimalzahl-Zeichenkette	-> Langzahlfeld.
  *		Format der Langzahlzeichenkette:
  *		    {+-}? (digit+ ("." digit+)? ({eE} {+-}? digit+)?)
  *		  | {+-}? ("." digit+ ({eE} {+-}? digit+)?)
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
{
 /*
  *  Fehlerabschaetzung:
  *
  *  Bezeichne d die durch die Langzahlzeichenkette dargestellte Dezimalzahl:
  *    d = (sum	(k = 1..n) d[k]*10^-k) * 10^ed (, d[1] != 0)
  *  Seien d', a' (mit B = 2^16) erklaert durch:
  *    d' := (sum (k = 1..nd) d[k]*10^-k) * 10^nd
  *    a' := (sum (k = 1..na) a[k]* B^-k) *  B^na  mit	a' == d'
  *  Dann gilt stets:
  *    a'*10^(ed-nd) <=	d < (a'+1)*10^(ed-nd)
  *
  *  Es	ist lediglich sicherzustellen, dass nd so gwaehlt wird,	dass a'	auf
  *  genau na Stellen darstellbar ist; um a' ==	d' und a[1] != 0 zu erreichen,
  *  muss gelten:
  *    B^(na-1)	<= d' <	B^na
  *  Hinreichend dafuer	ist (log : Logarithmus zur Basis 10):
  *	 B^(na-1) <= 10^(nd-1)	und  10^nd <= B^na
  *  <=> (na-1)*log(B) + 1 <= nd  und  nd <= na*log(B);	log(B) = 4.816479..
  *
  *  Es	gilt: 0	<= k <=	32769, k ganzzahlig =>
  *    k * log(B) - 1.5	< (k * 78913) >> 14 <= k * log(B)
  *    (kann auf dem Rechner ueberprueft werden)
  *
  *  Waehle also etwa  nd = (na	* 78913) >> 14.
  *  (klar: nd = (na * 78913) >> 14 <= na * log(B)
  *				(!)
  *   z.z.: (na-1) * log(B) + 1	<= na *	log(B) - 1.5 < (na * 78913) >> 14 = nd
  *   <=>	    -log(B) + 1	<= -1.5
  *   <=>		    2.5	<= log(B)   ok)
  *
  *  Der implementierte	Algorithmus konvertiert	zunaechst zur Basis 10000;
  *  der Speicherbedarf	zur Berechnung von na Ziffern betraegt damit in	Bytes:
  *    ((nd + 3) >> 2)			 * sizeof (USHORT) + sizeof (LONG)
  *  = ((((na *	78913) >> 14) +	3) >> 2) * sizeof (USHORT) + sizeof (LONG)
  *  = ((na * 78913 + 49152) >>	16)	 * sizeof (USHORT) + sizeof (LONG)
  */
#define	ExpChar1	'E'	/* Exponentzeichen			*/
#define	ExpChar2	'e'	/* Exponentzeichen			*/

  PUSHORT aMSD		= aus+3;
  USHORT  aLen		= 0;

  LONG	  Exponent	= 0L;
  LONG	  ExpOffset	= 0L;

  LONG	  n10;			/* # zu	beruecksichtigender Dezimal-	*/
				/* ziffern				*/
  USHORT  n10000;		/* # zu	beruecksichtigender Ziffern zur	*/
				/* Basis 10000				*/

  PCHAR	  pch0		= NULL;	/* zeigt auf die erste 0-Ziffer	einer	*/
				/* Folge von 0-Ziffern am Ende der	*/
				/* Dezimaldarstellung.			*/
  PCHAR	  pch0x		= NULL;	/* zeigt auf das erste Zeichen hinter	*/
				/* einer Folge fuehrender 0-Ziffern,	*/
				/* die den Dezimalpunkt	einschliesst.	*/
  PCHAR	  pchCont;
  USHORT  Loss		= 0;
  USHORT  Sign		= 0;

  SkipWhitespace ();
  switch (*pchIn) {
    case '-' : Sign = LR_NEGATIVE;
    case '+' : pchIn++;
	       SkipWhitespace ();
  }

  /* das erste Zeichen muss eine Ziffer	oder ein '.' sein */
  if (!IsDigit (*pchIn)	&& *pchIn != '.') {
    *pchOut = pchIn; LrSetNaN ((LREAL) aus); return (LR_NAN);
  }

 /*
  *  normierte Zahldarstellung erzeugen: Dezimalpunkt sowie 0-Ziffern
  *  am	Anfang und am Ende entfernen, verbleibende Ganzzahl konvertieren,
  *  Exponenten	geeignet korrigieren.
  */

  while	(*pchIn	== '0')		/* fuehrende 0-Ziffern ueberlesen	*/
    pchIn++;

  { /* 1. Durchgang */
    LONG cNulls		= 0L;
    LONG cDigits	= 0L;
    BOOL DecimalPoint	= FALSE;
    BOOL ResetExpOffset	= FALSE;/* == FALSE : anfangs und nach Dezimal-	*/
				/*	      punkt			*/
				/* == TRUE  : nach einer Folge der	*/
				/*	      Ziffern 1..9		*/

    pchCont = pchIn;		/* Gehen dem Dezimalpunkt nur 0-Ziffern	*/
				/* voran, wird mit dem Dezimalpunkt be-	*/
				/* gonnen.				*/
    if (*pchCont == '.') {
     /*
      *	 Faelle: 00..00.00..00xx..xx	ExpOffset <  0,	pch0x != NULL
      *	   oder	 00..00.xx..xx		ExpOffset == 0,	pch0x == NULL
      */
      DecimalPoint = TRUE; pchCont++;
      if (*pchCont == '0') {
	pch0 = pchCont;
	do {
	  ExpOffset--;
	} while	(*++pchCont == '0');
	pch0x =	pchCont;
      }
      else if (!IsDigit	(*pchCont)) {
	*pchOut	= pchCont; LrSetNaN ((LREAL) aus); return (LR_NAN);
      }
    }

    loop {
      if (*pchCont >= '1' && *pchCont <= '9') {
	ResetExpOffset = TRUE;
	cDigits	+= cNulls; cNulls = 0;
	pch0 = NULL;
	do {
	  cDigits++;
	} while	(*++pchCont >= '1' && *pchCont <= '9');
      }
      else if (*pchCont	== '0')	{
	if (cNulls == 0) pch0 =	pchCont;/* Folge von 0-Ziffern kann	*/
					/* durch Dezimalpunkt unter-	*/
					/* brochen werden		*/
	do {
	  cNulls++;
	} while	(*++pchCont == '0');
      }
      else if (*pchCont	== '.')	{
	pchCont++;
	if (DecimalPoint++ == FALSE) {	/* noch: ExpOffset == 0		*/
					/*	 pch0x	   == NULL	*/
	  ExpOffset = cNulls; ResetExpOffset = FALSE;
	}
	else {
	  *pchOut = pchCont; LrSetNaN ((LREAL) aus); return (LR_NAN);
	}
      }
      else {				/* Exponententeil oder Ende	*/
	if (DecimalPoint == FALSE)
	  ExpOffset = cNulls;
	else if ((pch0x == NULL && ResetExpOffset) || cDigits == 0)
	 /*
	  *  Faelle:		 00..00.00..00
	  *			 00..00.xx..xx
	  *    oder  00..00xx..xx00..00.00..00xx..xx
	  */
	  ExpOffset = 0L;
	break;
      }
    } /* loop */
   /*
    *  Jetzt: ExpOffset	gibt eine Exponentenkorrektur fuer folgende Faelle:
    *	 00..00xx..xx00..00			DecimalPoint   == FALSE
    *		     00..00.00..00		pch0x	       != NULL
    *		     00..00.00..00xx..xx	pch0x	       != NULL
    *	 00..00xx..xx00..00.00..00		ResetExpOffset == FALSE
    *  In folgenden Faellen gilt ExpOffset == 0:
    *		     00..00			DecimalPoint   == FALSE
    *	       00..00xx..xx			DecimalPoint   == FALSE
    *		     00..00.xx..xx		   pch0x == NULL
    *	 00..00xx..xx00..00.00..00xx..xx	&& ResetExpOffset == TRUE
    *	       00..00xx..xx.00..00		wie oben, aber ExpOffset == 0
    *  Andere Faelle gibt es nicht.
    */

    if ((n10 = (LONG) (((ULONG)	na * 78913L) >> 14)) >= cDigits)
      n10 = cDigits;			/* exakte Darstellung moeglich	*/
    else
      Loss = LR_LOSS;
  } /* 1. Durchgang */

#if 0 /* OK */
  if (pch0 != NULL)			/* 0-Ziffern am	Ende entfernen	*/
    *pch0 = 0;
#else
  if (ExpOffset > 0) ExpOffset = 0;
#endif /* OK */

  { /* 2. Durchgang */
    USHORT Digit = 0;
    SHORT  Count = (((USHORT) n10 + 3) & 3) + 1; /* # Dezimalziffern,	*/
					/* die noch in Digit einzutra-	*/
					/* gen sind.			*/
    n10000 = (USHORT) ((n10 + 3) >> 2);
    if (IsDigit (*pchIn))		/* Vorkommastellen		*/
      do {
	if (Count) {
	  Count--; Digit *= 5; Digit <<= 1;
	}
	else {
	  aMSD [aLen++]	= Digit; Digit = 0;
	  if (--n10000 == 0) {
	    do {			/* folgende Ziffern weglesen,	*/
	      ExpOffset++; pchIn++;	/* Exponent anpassen		*/
	    } while (IsDigit (*pchIn));
	    break;
	  }
	  Count	= 3;
	}
	Digit += (*pchIn++ - '0');
      }	while (IsDigit (*pchIn));

    if (*pchIn == '.') {		/* Nachkommastellen		*/
      pchIn++;
      if (!IsDigit (*pchIn) && pch0 == NULL) {
	*pchOut	= pchIn; LrSetNaN ((LREAL) aus); return (LR_NAN);
      }
      if (n10000 == 0)
	pchIn =	pchCont;
      else {
	if (pch0x != NULL) pchIn = pch0x;

	while (IsDigit (*pchIn)) {
	  if (Count) {
	    Count--; Digit *= 5; Digit <<= 1;
	  }
	  else {
	    aMSD [aLen++] = Digit; Digit = 0;
	    if (--n10000 == 0) {
	      pchIn = pchCont;
	      break;
	    }
	    Count = 3;
	  }
	  Digit	+= (*pchIn++ - '0'); ExpOffset--;
	} /* while */
      }	/* else	*/
    } /* if (*pchIn == '.') */

    if (n10000)				/* letzte Ziffer speichern	*/
      aMSD [aLen++] = Digit;
  } /* 2. Durchgang */

  if (pch0 != NULL) {
#if 0 /* OK */
    *pch0 = '0';
#endif /* OK */
    pchIn = pchCont;
  }

  SkipWhitespace ();
  if (*pchIn == ExpChar1 || *pchIn == ExpChar2) { /* Exponent		*/
    BOOL ExpSign = FALSE;

    pchIn++;
    SkipWhitespace ();
    switch (*pchIn) {
      case '-' : ExpSign++; /* no break	*/
      case '+' : pchIn++;
		 SkipWhitespace	();
    }
    if (!IsDigit (*pchIn)) {
      *pchOut =	pchIn; LrSetNaN ((LREAL) aus); return (LR_NAN);
    }
    while (*pchIn == '0')
      pchIn++;
    while (IsDigit (*pchIn)) {
      Exponent *= 10;
      if ((Exponent += (*pchIn++ - '0')) > 999999L) {
	while (IsDigit (*pchIn))
	  pchIn++;
	break;
      }
    }
    if (ExpSign) Exponent = -Exponent;

    SkipWhitespace ();
  }

  *pchOut = pchIn;

  if (aLen > 1)	{	/* Konvertierung: Basis	10000 -> Basis 2^16	*/
#if defined (__ASSEMBLER__)
    LrAsmHorner10000 (aLen, aMSD);
#else
    {
      ULONG uu;
      USHORT n = 1;

      do {
	USHORT k = n;

	((USHORT *) &uu) [D1OF2] = aMSD	[k];
	do {
	  aMSD [k] = (USHORT) (uu =
	      aMSD [k-1] * 10000L + ((USHORT *)	&uu) [D1OF2]);
	} while	(--k);
	aMSD [0] = ((USHORT *) &uu) [D1OF2];
      }	while (++n < aLen);
    }
#endif

    if (*aMSD == 0) {
      do {			/* '0' Ziffern am Anfang entfernen	*/
	aLen--;
      }	while (*++aMSD == 0);

      MoveFarLeft (aus+3, aMSD, aLen, USHORT);
    } /* if (*aMSD == 0) */
  }
  else if (aLen == 0 || aMSD [0] == 0) {
    LrSetSignedZero ((LREAL) aus, Sign); return (LR_ZERO);
  }

  if ((Exponent	+= ExpOffset) <	-999999L) {
    LrSetSignedZero ((LREAL) aus, Sign); return (LR_LOSS | LR_ZERO);
  }
  if (Exponent > 999999L) {
    if (Sign) {
      LrSetNegInfinity ((LREAL) aus); return (LR_LOSS | LR_NEG_INFINITY);
    }
    else {
      LrSetPosInfinity ((LREAL) aus); return (LR_LOSS | LR_POS_INFINITY);
    }
  }

  aus [0] = Sign | aLen;
  aus [1] = ((USHORT *) &Exponent) [D0OF2];
  aus [2] = ((USHORT *) &Exponent) [D1OF2];

  return (Loss | LR_NORMAL);
} /* lrsztoaus */

INT FAR CALL lraustolr (PUSHORT aus, LREAL c,
  PVOID	wrk1, PVOID wrk2, PVOID	wrk3, INT Level)
 /***********************************************************************
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
  *		Bedarf fuer "wrk1", "wrk2" in Bytes:
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
{
 /*
  *  Das Langzahlfeld wird durch voruebergehendes Ueberschreiben des Expo-
  *  nenten in eine Langzahl a umgewandelt. Mit	LrPow10() wird die benoetigte
  *  10er-Potenz  b = 10^|exponent|  berechnet;	je nach	exponent-Vorzeichen
  *  ergibt sich die gesuchte Langzahl c als a*b oder a/b.
  *
  *  Fehlerabschaetzung:
  *
  *    a' <= a < a' + da*B^-na;	a' = sum (k = 1..na) a[k]*B^-k
  *    b' <= b < b' + db*B^-nb;	b' = sum (k = 1..nb) b[k]*B^-k
  *
  *  1.	Fall: a*b
  *
  *  Berechnet wird c' = sum (k	= 1..nc) c[k]*B^-k mit
  *    c' <= a'*b' <= c' + dc
  *  Bei Rechnung mit 4	zusaetzlichen Schutzziffern, ergibt sich im unguen-
  *  stigsten Fall eine	Schranke fuer dc von
  *    2 Einheiten in der 1. Schutzziffer von c'
  *
  *  Seien na =	nc+2, nb = nc+4	gewaehlt, dann folgt mit da = 1, db < 3/4*B:
  *    a'*b' <=	a*b <= (a'+B^-na)*(b'+db*B^-nb)
  *		     =	a'*b' +	a'*db*B^-nb + b'*B^-na + db*B^-(na+nb)
  *		    <	a'*b' +	db*B^-nb + B^-na + db*B^-(na+nb)
  *		    <	a'*b' +	3/4*B *	B^-  (nc+4)
  *			      +		B^-  (nc+2)
  *			      +	3/4*B *	B^-2*(nc+3)
  *		    <	a'*b' +	2*B^-(nc+2)	(vor Normalisierung)
  *
  *  D.h.: Der Gesamtfehler kann nach Normalisierung und Abschneiden hinter
  *  der 1. Schutzziffer 2 + 2 Einheiten in der	1. Schutzziffer	abgeschaetzt
  *  werden.
  *
  *  2.	Fall: a/b
  *
  *  Berechnet wird c' = sum (k	= 1..nc) c[k]*B^-k mit
  *    c' <= a'/ (b'+db*B^-nb) <= c' + dc
  *  Bei Rechnung mit 2	zusaetzlichen Schutzziffern, ergibt sich im unguen-
  *  stigsten Fall eine	Schranke fuer dc von
  *    2 Einheiten in der 1. Schutzziffer von c'
  *
  *  Sei d erklaert durch:
  *
  *    a'		      !	a'+da*B^-na
  *    ----------- + d*B^-nc >=	-----------
  *    b'+db*B^-nb		b'
  *
  *  Dann ist fuer d hinreichend:
  *
  *	    !  a'+da*B^-na   a'
  *	 d >= (----------- - -----------) * B^nc
  *	       b'	     b'+db*B^-nb
  *
  *	      (a'+da*B^-na)*(b'+db*B^-nb) - a'*b'
  *	    = ----------------------------------- * B^nc
  *	       b'*(b'+db*B^-nb)
  *
  *	       a'*b' + a'*db*B^-nb + da*B^-na*(b'+db*B^-nb) - a'*b'
  *	    = ----------------------------------------------------- * B^nc
  *	       b'*(b'+db*B^-nb)
  *
  *	      a'    1				  1
  *	    = -- * ------------- * db*B^(nc-nb)	+ -- * da*B^(nc-na)
  *	      b'   (b'+db*B^-nb)		  b'
  *
  *	      1	     1			   1
  *  <=	 d >= ---- * ---- * db*B^(nc-nb) + ----	* da*B^(nc-na)
  *	      B^-1   B^-1		   B^-1
  *
  *	    = db*B^(nc-nb+2) + da*B^(nc-na+1)
  *
  *  Seien na =	nc+2, nb = nc+4	gewaehlt, dann ist mit da = 1, db < 3/4*B
  *  hinreichend:
  *	 d >= 3/4*B * B^-2 + B^-1
  *
  *  D.h.: Der Gesamtfehler kann durch 2 + 2 Einheiten in der 1. Schutzziffer
  *  nach oben abgeschaetzt werden.
  *
  *  Problem: Die benoetigte 10er-Potenz 10^|exponent| kann so gross sein,
  *  dass sie als Langzahl nicht darstellbar ist.
  *  In	diesem Fall wird die 10er-Potenz in zwei 10er-Potenzen b1 und b2 zer-
  *  legt, es werden zwei Divisionen durchgefuehrt: Die	erste Division
  *  liefert c1'= (a'/b1'+db*B^-nb1) wie oben, die zweite liefert das
  *  endgueltige c. Eine Abschaetzung des Fehleranteils	aufgrund der Fehler
  *  in	den Eingangsdaten der zweiten Division ergibt mit c1, b2 statt a, b
  *  in	obiger Formel fuer nc1 = nc+3, nb2 = nc+4, dc1 = 4, db < 3/4*B:
  *	 d >= 3/4*B * B^-2 + 4 * B^-2 tut's.
  *
  *  Zusammenfassend: Ist die benoetigte 10er-Potenz als Langzahl nicht	dar-
  *  stellbar, setze
  *    na  = nc+4
  *    nb1 = nc+4 (+2 Schutzziffern).
  *  Berechne damit c1'	= (a'/b1'+db*B^-nb1) mit zunaechst nc1 = nc+2; es er-
  *  gibt sich ein Gesamtfehler	d < 4*B^-(nc1+1). Verwende die 1. Schutz-
  *  ziffer von	c1 mit und setze
  *    nb2 = nc+2 (+2 Schutzziffern).
  *  Dies liefert c mit	einem Gesamtfehler d < 3 Einheiten in der 1. Schutz-
  *  ziffer.
  */
#define	MAX_EXP		157821L
#define	a		((LREAL) (aus))

  LONG	  Exponent;
  SHORT	  cLen;
  INT	  rc;
  INT	  Loss;

  if (LrIsSpecial (a)) {
    memcpy ((PCHAR) c, (PCHAR) a, 4);
    return (LrSpecial (a));
  }

  ((USHORT *) &Exponent) [D0OF2] = aus [1];
  ((USHORT *) &Exponent) [D1OF2] = aus [2];

  aus [1] = aus [0]; aus++;		/* beachte!			*/
  {
    PLDIGIT MSD = LrPMSD (a);
    SHORT   na	= LrDigits (a);

    LrExponent (a) = na;		/* Exponent zur Basis 2^16	*/
					/* eintragen			*/
    while (MSD [--na] == 0)		/* 0-Ziffern am Ende entfernen	*/
      ;
    LrSetDigits (a, ++na);		/* # Ziffern eintragen		*/
  }

  if (Exponent == 0) {
    rc = LrAssign (c, a, Level);
					/* aus wieder herstellen	*/
    aus [0] = ((USHORT *) &Exponent) [D0OF2];
    aus [1] = ((USHORT *) &Exponent) [D1OF2];
    return (rc);
  }

  cLen = LrDigits (c);

  if (Exponent < 0) {
    if (Exponent < -MAX_EXP*2) raise UNDERFLOW;

    if (Exponent < -MAX_EXP) {
      LONG Exp1	= (-Exponent) >> 1;
      LONG Exp2	= (-Exponent) -	Exp1;

      LrSetPosDigits ((LREAL) wrk1, cLen+4);	/* wrk1	= 10^Exp1	*/
      Loss = LrPow10 (Exp1, (LREAL) wrk1, wrk2, wrk3, 0) & LR_LOSS;
      LrIncrPow10 ((LREAL) wrk1);

      LrSetPosDigits ((LREAL) wrk2, cLen+2);	/* wrk2	= a / wrk1	*/
      Loss |= (LrDiv (a, (LREAL) wrk1, (LREAL) wrk2, wrk3, 0) & LR_LOSS);

      {	/* 1 Schutzziffern mitverwenden, 0-Ziffern am Ende entfernen	*/
	PLDIGIT	MSD = LrPMSD ((LREAL) wrk2);
	SHORT	Len = cLen+3;

	while (MSD [--Len] == 0)
	  ;
	LrSetPosDigits ((LREAL)	wrk2, Len+1);
      }

      LrSetPosDigits ((LREAL) wrk1, cLen+2);	/* wrk1	= 10^Exp2	*/
      rc = LrPow10 (Exp2, (LREAL) wrk1, c, wrk3, 0);
      if (rc & LR_LOSS)	{
	Loss |=	LR_LOSS;
	LrIncrPow10 ((LREAL) wrk1);
      }
      else {
	PLDIGIT	MSD = LrPMSD ((LREAL) wrk1);
	SHORT	Len = cLen+4;

	while (MSD [--Len] == 0)
	  ;
	LrSetPosDigits ((LREAL)	wrk1, Len+1);
      }

      LrSetPosDigits (c, cLen);			/* c = wrk2 / wrk1	*/
      if ((rc = LrDiv (
	  (LREAL) wrk2, (LREAL) wrk1, c, wrk3, 0)) == (LR_LOSS | LR_ZERO))
	raise UNDERFLOW;
    }
    else /* if (-MAX_EXP <= Exponent < 0) */ {
      LrSetPosDigits ((LREAL) wrk1, cLen+2);	/* wrk1	= 10^Exp1	*/
      if ((Loss = LrPow10 (
	  -Exponent, (LREAL) wrk1, c, wrk2, 0) & LR_LOSS) != 0)
	LrIncrPow10 ((LREAL) wrk1);
      else {
	PLDIGIT	MSD = LrPMSD ((LREAL) wrk1);
	SHORT	Len = cLen+4;

	while (MSD [--Len] == 0)
	  ;
	LrSetPosDigits ((LREAL)	wrk1, Len+1);
      }

      LrSetPosDigits (c, cLen);			/* c = a / wrk1		*/
      rc = LrDiv (a, (LREAL) wrk1, c, wrk3, 0);
    }
  }
  else /* if (Exponent > 0) */ {
    if (Exponent > MAX_EXP) raise OVERFLOW;

    LrSetPosDigits ((LREAL) wrk1, cLen+2);	/* wrk1	= 10^Exponent	*/
    Loss = LrPow10 (Exponent, (LREAL) wrk1, c, wrk2, 0) & LR_LOSS;

    { /* 2 Schutzziffern mitverwenden, 0-Ziffern am Ende entfernen	*/
      PLDIGIT MSD = LrPMSD ((LREAL) wrk1);
      SHORT   Len = cLen+4;

      while (MSD [--Len] == 0)
	;
      LrSetPosDigits ((LREAL) wrk1, Len+1);
    }

    LrSetPosDigits (c, cLen);
    if ((rc = LrMul (
	(LREAL) aus, (LREAL) wrk1, c, 0)) == (LR_LOSS | LR_POS_INFINITY))
      raise OVERFLOW;
  }

  if (Level > 0) {
    PLDIGIT cMSD = LrPMSD (c);

    if (cMSD [cLen] >= (LDIGIT) (LR_ROUND - 2)) { /* Runden		*/
      SHORT Len	= cLen;

      while (Len-- && (LDIGIT) ++(cMSD [Len]) == 0)
	;
      if (Len <	0) {
	LrSetSign (c, LrSign (a));
					/* aus wieder herstellen	*/
	aus [0] = ((USHORT *) &Exponent) [D0OF2];
	aus [1] = ((USHORT *) &Exponent) [D1OF2];

	return (LrRoundingOverflow (c));
      }
      Loss |= LR_LOSS;
    }
    else if (cMSD [cLen])
      Loss |= LR_LOSS;

    while (cMSD	[--cLen] == 0)		/* 0-Ziffern am	Ende entfernen	*/
      ;
    LrSetPosDigits (c, cLen+1);
  }
  LrSetSign (c, LrSign (a));
					/* aus wieder herstellen	*/
  aus [0] = ((USHORT *) &Exponent) [D0OF2];
  aus [1] = ((USHORT *) &Exponent) [D1OF2];

  return (Loss | rc);

UNDERFLOW:
  LrSetSignedZero (c, LrSign (a));
					/* aus wieder herstellen	*/
  aus [0] = ((USHORT *) &Exponent) [D0OF2];
  aus [1] = ((USHORT *) &Exponent) [D1OF2];
  return (LR_LOSS | LR_ZERO);

OVERFLOW:
  {
    USHORT Sign = LrSign (a);
					/* aus wieder herstellen	*/
    aus [0] = ((USHORT *) &Exponent) [D0OF2];
    aus [1] = ((USHORT *) &Exponent) [D1OF2];

    if (Sign) {
      LrSetNegInfinity (c); return (LR_LOSS | LR_NEG_INFINITY);
    }
    else {
      LrSetPosInfinity (c); return (LR_LOSS | LR_POS_INFINITY);
    }
  }
} /* lraustolr */

INT FAR	CALL lrsztolr (PCHAR pchIn, PPCHAR pchOut, LREAL c,
  PVOID	wrk1, PVOID wrk2, PVOID	wrk3, PVOID wrk4, INT Level)
 /***********************************************************************
  *  Funktion :	Konvertierung: Dezimalzahl-Zeichenkette	-> Langzahl.
  *  Speicher :	Bedarf fuer "c"	in Bytes:
  *		  LrDigits (c) * sizeof	(LDIGIT) + sizeof (LREALHDR)
  *		  + 16 * sizeof	(LDIGIT)
  *		Bedarf fuer "wrk1" in Bytes:
  *		  m * sizeof (USHORT) + sizeof (USHORT) + sizeof (LONG) mit
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
{
 /*
  *  Speicherbedarf fuer "wrk1":
  *    m = ((((LrDigits	(c) + 4) * 78913) >> 14) + 3) >> 2
  *	 =   ((LrDigits	(c) + 4) * 78913 + 49152) >> 16
  *	 =    (LrDigits	(c) * 78913 + 364804) >> 16
  */
  INT	 Loss;

  Loss = lrsztoaus (pchIn, pchOut, (PUSHORT) wrk1, LrDigits (c) + 4) & LR_LOSS;

  return (Loss | lraustolr ((PUSHORT) wrk1, c, wrk2, wrk3, wrk4, Level));
} /* lrsztolr */
