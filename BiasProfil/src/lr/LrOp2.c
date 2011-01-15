/************************************************************************
*  $Id:	LrOp2.c			Langzahlarithmetik, Operationen, Level 2
*
*  Copyright (C) 1989, 1992 Dirk Husung
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
*  Letzte Aenderung:		Sun 04-Oct-1992 11:07:21
*************************************************************************/

#include "Portab.h"

#include <string.h>

#include "Lr.h"

/************************************************************************
*  Implementierung globaler Funktionen
*************************************************************************/

INT FAR	CALL LrPow10 (
  LONG Exp10, LREAL res, PVOID wrk1, PVOID wrk2, INT Level)
 /***********************************************************************
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
{
 /*
  *  Die gesuchte 10er-Potenz wird durch Multiplikation	der 10er-Potenzen
  *  10^1, 10^2, 10^(2^2), ...,	10^(2^17) berechnet. Dabei sind	die 10er-
  *  Potenzen 10^1, ..., 10^(2^9) exakt	gespeichert, die Zehnerpotenzen
  *  10^(2^10),	..., 10^(2^17) werden -	falls benoetigt	- durch	wiederholtes
  *  Quadrieren	berechnet.
  *
  *  Fehlerabschaetzung	(B = 2^16):
  *
  *  Ann.: Der Rechenfehler bei	der Multiplikation mit 2 Schutzziffern
  *	   betraegt nach evtl. Normalisierung und Abschneiden der Schutz-
  *	   ziffern weniger als 7/8*B Einheiten in der Endziffer.
  *	   (Diese Ann. wird in den mit "!*!" statt "*" gekennzeichneten
  *	   Zeilen verwendet.)
  *
  *  Wird die Quadratur	von "a"	als Multiplikation mit 2 Schutzziffern aus-
  *  gefuehrt, ergibt sich mit
  *    a' <= a < a' + da*B^-na;	 a' = sum (k = 1..na) a[k]*B^-k
  *
  *    a'*a' <=	a*a < (a' + da*B^-na)^2
  *		    <  a'*a' + 2*(a[1]+1)*da*B^-(na+1) + da*da*B^-(2*na)
  *
  *  Die Multiplikations-Routine berechnet
  *    c' <= a'*a' <= c' + dc
  *  Bei 2 Schutzziffern ergibt	sich als eine grobe obere Fehlerschranke
  *  fuer dc (nach Abschneiden der Schutzziffern),
  *    falls keine Normalisierung erforderlich:	2 Einheiten in der Endziffer
  *    andernfalls:				B Einheiten in der Endziffer
  *
  *  Zusammenfassend: Der Gesamtfehler jeder Quadratur (d.i. die Anzahl
  *  Einheiten,	die, zu	der Endziffer des korrekten oder betragsmaessig	zu
  *  kleinen Ergebnisses addiert, ein betragsmaessig zu	grosses	Ergebnis
  *  liefert) kann abgeschaetzt	werden durch:
  *    falls keine Normalisierung erforderlich:	2*da*(a[1]+1)*B^-1 + 2
  *    andernfalls:				2*da*(a[1]+1)	   + B
  *						Einheiten in der Endziffer
  *  (Obige Schranken fuer dc sind so grob, dass auch der Term da*da*B^-(2*na)
  *  mit abgeschaetzt wird, falls "da" hinr. klein und "na" hinr. gross	sind,
  *  z.B.: da <	B^2, na	>= 6.)
  *
  *  Rechnung liefert:
  *
  *		 a[1]+1	      Normalisierung Fehlerschranke
  *					     in	Einheiten der Endziffer
  *  10^(2^10) : 0326 =	 806  ja				   <=	 1*B
  *  10^(2^11) : 000A =	  10  nein		1*B * 2* 806/B + 2 <=	 1*B
  *  10^(2^12) : 0063 =	  99  ja		1*B * 2*  10   + B <=	21*B
  *  10^(2^13) : 25A9 =	9641  ja	       21*B * 2*  99   + B <= 4159*B
  *  10^(2^14) : 058B =	1419  nein	     4159*B * 2*9641/B + 2 <= 1224*B
  *  10^(2^15) : 001F =	  31  nein	     1224*B * 2*1419/B + 2 <=	54*B
  *  10^(2^16) : 03AF =	 943  ja	       54*B * 2*  31   + B <= 3349*B
  *  10^(2^17) : 000E	      nein	     3349*B * 2* 943/B + 2 <=	97*B
  *
  *  Zusammenfassend: Die berechneten 10er-Potenzen 10^(2^10), ..., 10^(2^17)
  *  sind stets	korrekt	oder betragsmaessig um weniger als (1/8*B - 1)*B Ein-
  *  heiten in der Endziffer zu	klein.
  *
  *  Nachfolgend wird die Berechnung der gesuchten 10er-Potenz durch Multi-
  *  plikation der Potenzen 10^1, 10^2,	10^(2^2), ..., 10^(2^17) betrachtet:
  *
  *  Ein Teilprodukt "a" und ein Faktor	"b" seien bestimmt durch:
  *    a' <= a < a' + da*B^-na;	 a' = sum (k = 1..na) a[k]*B^-k
  *    b' <= b < b' + db*B^-nb;	 b' = sum (k = 1..nb) b[k]*B^-k
  *
  *  Die Multiplikations-Routine berechnet c' =	sum (k = 1..nc)	c[k]*B^-k mit:
  *    c' <= a'*b' <= c' + dc
  *  Bei 2 Schutzziffern ergibt	sich im	unguenstigsten Fall (nach Normali-
  *  sierung und Abschneiden der Schutzziffern)	eine obere Schranke fuer "dc"
  *  von
 !*!   7/8*B  Einheiten	in der Endziffer von c'
  *
  *  Weiter gilt:
  *    a'*b' <=	a*b < (a'+ da*B^-na)*(b'+ db*B^-nb)
  *		    = a'*b' + a'*db*B^-nb + b'*da*B^-na	+ da*db*B^-(na+nb)
  *		    < a'*b' + 1	*db*B^-nb + (b[1]+1)*da*B^-(na+1)
  *			    + da*db*B^-(na+nb)
  *
  *  Seien na =	nc, nb = nc+2 gewaehlt,	dann folgt mit db < (1/8*B - 1)*B :
  *    o.B.d.A.: da*db*B^-(na+nb) < B^-(nc+1)  (, wenn nur  da*B^-na < B^-1 )
  *  und weiter:
  *    a'*b' <=	a*b < a'*b' + db*B^-(nc+2) + da*(b[1]+1)*B^-(nc+1) + B^-(nc+1)
  *		    < a'*b' + (1/8*B + da*(b[1]+1)) * B^-(nc+1)
  *
  *  Zusammenfassend: Der Gesamtfehler des Produkts c' (d.i. die Anzahl
  *  Einheiten,	die, zu	der Endziffer von c' addiert, ein betragsmaessig zu
  *  grosses Ergebnis liefert) kann im unguenstigsten Fall (nach Normalisierung
  *  und Abschneiden der Schutzziffern)	abgeschaetzt werden durch:
 !*!   7/8*B + 1/8*B + da*(b[1]+1)
  *  = da*(b[1]+1) + B		    Einheiten in der Endziffer
  *
  *  Die gesuchte 10er-Potenz wird als Produkt aller Elemente einer Teilmenge T
  *  der Potenzen 10^1,	10^2, 10^(2^2),	..., 10^(2^17) bestimmt. Der Fehler
  *  des Gesamtprodukts	(d.i. die Anzahl Einheiten, die, zu der	Endziffer des
  *  berechneten Produkts addiert, ein Ergebnis	liefert, das betragsmaessig
  *  groesser ist als die gesuchte 10er-Potenz)	kann vorteilhaft aufgeschlues-
  *  selt werden nach:
  *    1. der Anzahl N der Elemente 10^(2^k) der Teilmenge T mit k >= 5,
  *    2. der Groesse der gesuchten 10er-Potenz	10^Exp10; die Teilmenge	T kann
  *	  nur Potenzen	10^(2^k) mit 0 <= k <= m, 2^m <= Exp10	enthalten.
  *
  *  Der Fehler	des Gesamtprodukts wird	gemaess	obiger Formel abgeschaetzt:
  *  Beachte: Die Potenzen 10^1, 10^2, 10^4, 10^8, 10^16 koennen bei minde-
  *  stens 8-ziffriger Rechnung	exakt miteinander multipliziert	werden.	Das
  *  erste nicht exakt berechenbare Produkt ist	mit einem Fehler < B Einheiten
  *  in	der Endziffer behaftet.
  *  Dementsprechend werden die	N (verschiedenen) Potenzen 10^(2^k1), ...,
  *  10^(2^kN),	5 <= k1, ..., kN <= m  mit den groessten fuehrenden Ziffern
  *  zugrunde gelegt. Diese Potenzen werden der	Groesse	ihrer fuehrenden
  *  Ziffer nach aufmultipliziert, beginnend mit der Potenz mit	kleinster
  *  fuehrender	Ziffer.	Gestartet wird mit einem exakten Teilprodukt a der
  *  Potenzen 10^0, 10^1, 10^2,	..., 10^16, d.h. mit dem Eingangsfehler	da = 0
  *  Einheiten.	(Begruendung der Multiplikations-Reihenfolge; Beispiel:
  *    da0 =   0*b0 + B	= B
  *    da1 = da0*b1 + B	= B * (b1 + 1)
  *    da2 = da1*b2 + B	= B * (b1*b2 + b2 + 1)		 (!)
  *    da3 = da2*b3 + B	= B * (b1*b2*b3	+ b2*b3	+ b3 + 1) = max.
  *  Innerhalb des Produkts b1*b2*b3 ist die Wahl von b1, b2 und b3 beliebig,
  *  der Rest, b2*b3 + b3 + 1, wird maximal, wenn b2 und b3 maximal (und folg-
  *  lich b1 minimal) gewaehlt werden. Unter b2	und b3 ist analog b2 minimal
  *  zu	waehlen. Insgesamt sind	die Potenzen der Groesse ihrer fuehrenden
  *  Ziffer nach, beginnend mit	der Potenz mit kleinster fuehrender Ziffer
  *  aufzumultiplizieren.)
  *
  *  Rechnung liefert folgende Tabelle der maximalen Anzahlen fehlerbehafteter
  *  Endziffern	(fehlerbehaftet	in obigem Sinn):
  *
  *    N \ Exp10 < 2^m
  *	   m =	 6   7	 8   9	10  11	12  13	14  15	16  17	18
  *
  *    0 :	 0   0	 0   0	 0   0	 0   0	 0   0	 0   0	 0
  *    1 :	 2   2	 2   2	 2   2	 2   2	 2   2	 2   2	 2 ')
  *    2 :	 .   2	 2   2	 2   2	 2   2	 2   2	 2   2	 2
  *    3 :	 .   .	 3   3	 3   3	 3   3	 3   3	 3   3	 3
  *    4 :	 .   .	 .   3	 3   3	 3   3	 4   4	 4   4	 4
  *    5 :	 .   .	 .   .	 3   4	 4   4	 4   4	 4   4	 4
  *    6 :	 .   .	 .   .	 .   4	 4   4	 5   5	 5   5	 5
  *    7 :	 .   .	 .   .	 .   .	 4   4	 5   5	 5   5	 5
  *    8 :	 .   .	 .   .	 .   .	 .   5	 5   6	 6   6	 6
  *    9 :	 .   .	 .   .	 .   .	 .   .	 5   6	 6   6	 6
  *   10 :	 .   .	 .   .	 .   .	 .   .	 .   6	 6   6	 6
  *   11 :	 .   .	 .   .	 .   .	 .   .	 .   .	 6   7	 7
  *   12 :	 .   .	 .   .	 .   .	 .   .	 .   .	 .   7	 7
  *
  *  ')	Die vorletzte Endziffer	ist mit	hoechstens 1 Einheit fehlerbehaftet.
  *
  *  Addition von 3/4*B	Einheiten zur fuehrenden fehlerbehafteten Endziffer
  *  liefert in	jedem Fall ein betragsmaessig zu grosses Ergebnis.
  *
  *  Bei einem Tabelleneintrag von k (k	> 0) wird die Ziffernzahl von "res",
  *  "wrk1" und	"wrk2" voruebergehend um k+1 erhoeht. Die Potenzen 10^(2^k),
  *  10	<= k <=	17 werden - falls benoetigt - mit nochmals 2 zusaetzlichen
  *  Ziffern berechnet.	Saemtliche Multiplikationen werden mit 2 (statt	der
  *  ueblichen 4) Schutzziffern	ausgefuehrt.
  */
  static USHORT	LrConst1E0 [] =	{
    0x0001, 0x0001, 0x0001
  };
  static USHORT	LrConst1E1 [] =	{
    0x0001, 0x0001, 0x000A
  };
  static USHORT	LrConst1E2 [] =	{
    0x0001, 0x0001, 0x0064
  };
  static USHORT	LrConst1E3 [] =	{
    0x0001, 0x0001, 0x03E8
  };
  static USHORT	LrConst1E4 [] =	{
    0x0001, 0x0001, 0x2710
  };
  static USHORT	LrConst1E5 [] =	{
    0x0002, 0x0002, 0x0001, 0x86A0
  };
  static USHORT	LrConst1E6 [] =	{
    0x0002, 0x0002, 0x000F, 0x4240
  };
  static USHORT	LrConst1E7 [] =	{
    0x0002, 0x0002, 0x0098, 0x9680
  };
  static USHORT	LrConst1E8 [] =	{
    0x0002, 0x0002, 0x05F5, 0xE100
  };
  static USHORT	LrConst1E9 [] =	{
    0x0002, 0x0002, 0x3B9A, 0xCA00
  };
  static USHORT	LrConst1E10 [] = {
    0x0003, 0x0003, 0x0002, 0x540B, 0xE400
  };
  static USHORT	LrConst1E11 [] = {
    0x0003, 0x0003, 0x0017, 0x4876, 0xE800
  };
  static USHORT	LrConst1E12 [] = {
    0x0003, 0x0003, 0x00E8, 0xD4A5, 0x1000
  };
  static USHORT	LrConst1E13 [] = {
    0x0003, 0x0003, 0x0918, 0x4E72, 0xA000
  };
  static USHORT	LrConst1E14 [] = {
    0x0003, 0x0003, 0x5AF3, 0x107A, 0x4000
  };
  static USHORT	LrConst1E15 [] = {
    0x0004, 0x0004, 0x0003, 0x8D7E, 0xA4C6, 0x8000
  };
  static LREAL LrConst1E [] = {
    (LREAL) LrConst1E0,
    (LREAL) LrConst1E1,
    (LREAL) LrConst1E2,
    (LREAL) LrConst1E3,
    (LREAL) LrConst1E4,
    (LREAL) LrConst1E5,
    (LREAL) LrConst1E6,
    (LREAL) LrConst1E7,
    (LREAL) LrConst1E8,
    (LREAL) LrConst1E9,
    (LREAL) LrConst1E10,
    (LREAL) LrConst1E11,
    (LREAL) LrConst1E12,
    (LREAL) LrConst1E13,
    (LREAL) LrConst1E14,
    (LREAL) LrConst1E15
  };
  static USHORT	LrConst1E16 [] = {
    0x0003, 0x0004, 0x0023, 0x86F2, 0x6FC1
  };
  static USHORT	LrConst1E32 [] = {
    0x0005, 0x0007, 0x04EE, 0x2D6D, 0x415B, 0x85AC, 0xEF81
  };
  static USHORT	LrConst1E64 [] = {
    0x000A, 0x000E, 0x0018, 0x4F03, 0xE93F, 0xF9F4, 0xDAA7, 0x97ED,
    0x6E38, 0xED64, 0xBF6A, 0x1F01
  };
  static USHORT	LrConst1E128 []	= {
    0x0013, 0x001B, 0x024E, 0xE91F, 0x2603, 0xA633, 0x7F19, 0xBCCD,
    0xB0DA, 0xC404, 0xDC08, 0xD3CF, 0xF5EC, 0x2374, 0xE42F, 0x0F15,
    0x38FD, 0x03DF, 0x9909, 0x2E95, 0x3E01
  };
  static USHORT	LrConst1E256 []	= {
    0x0026, 0x0036, 0x0005, 0x53F7, 0x5FDC, 0xEFCE, 0xF46E, 0xEDDC,
    0x80DC, 0xC7F7, 0x55BC, 0x28F2, 0x65F9, 0xEF17, 0xCC55, 0x73C0,
    0x63FF, 0x540E, 0x3C42, 0xD35A, 0x1D15, 0x3624, 0xADC6, 0x66B0,
    0x26B2, 0x716E, 0xD595, 0xD80F, 0xCF4A, 0x6E70, 0x6BDE, 0x50C6,
    0x1215, 0x2F87, 0xD8D9, 0x9F72, 0xBED3, 0x875B, 0x982E, 0x7C01
  };
  static USHORT	LrConst1E512 []	= {
    0x004B, 0x006B, 0x001C, 0x6334, 0x15D4, 0xC1D2, 0x38D9, 0x8CAB,
    0x8A97, 0x8A0B, 0x1F13, 0x8CB0, 0x7303, 0xA269, 0x9748, 0x45A7,
    0x1D46, 0xB099, 0xBC81, 0x7343, 0xAFAC, 0x69BE, 0x5B0E, 0x9449,
    0x775C, 0x1366, 0x732A, 0x93AB, 0xADE4, 0xB290, 0x8EE0, 0xF95F,
    0x635E, 0x85A9, 0x1924, 0xC3FC, 0x0695, 0xE7FC, 0x7153, 0x329C,
    0x57AE, 0xBFA3, 0xEDAC, 0x96E1, 0x4F5D, 0xBC51, 0xFB2E, 0xB21A,
    0x2F22, 0x1E25, 0xCFEA, 0x703E, 0xD321, 0xAA1D, 0xA1BF, 0x28F8,
    0x733B, 0x4475, 0xB579, 0xC889, 0x76C1, 0x94E6, 0x5747, 0x46C4,
    0x0513, 0xC31E, 0x1AD9, 0xB83A, 0x8A97, 0x5D96, 0x976F, 0x8F95,
    0x46DC, 0x77F2, 0x7267, 0xFC6C, 0xF801
  };
  static LREAL LrConstPow [] = {
    (LREAL) LrConst1E16,
    (LREAL) LrConst1E32,
    (LREAL) LrConst1E64,
    (LREAL) LrConst1E128,
    (LREAL) LrConst1E256,
    (LREAL) LrConst1E512
  };

  USHORT Loss =	0;
  USHORT Exp0,		/* Bits	 0.. 3 des Exponenten	*/
	 Exp1,		/* Bits	 4.. 9 des Exponenten	*/
	 Exp2;		/* Bits	10..17 des Exponenten	*/
  SHORT	 BitsSet0_9,	/* #(gesetzte Bits) der	Bits 0..9; die Gruppe	*/
			/* der Bits 0..3 wird dabei als	1 Bit gezaehlt	*/
	 BitsSet5_9,	/* #(gesetzte Bits) der	Bits 5..9		*/
	 oldLen;

  if (Exp10 >= 157822L /* > log	(LR_BASE)*32767	*/ )
    return (LrOverflow (res, 1));

  /* Exponent in Bitgruppen aufspalten */
  Exp0 = ((USHORT) Exp10) & 0x000F;
  Exp2 = (Exp1 = (USHORT) (Exp10 >> 4))	>> 6;

  if ((Exp1 &= 0x003F) == 0) {
    if (Exp2 ==	0)
      return (LrAssign (res, LrConst1E [Exp0], Level));

    BitsSet0_9 = Exp0 != 0;
    BitsSet5_9 = 0;
  }
  else {
    USHORT Exp = Exp1;

    BitsSet0_9 = (Exp0 != 0) + (Exp & 1);
    BitsSet5_9 = 0;
    for	(Exp >>= 1; Exp; Exp >>= 1)
      BitsSet5_9 += (Exp & 1);
    BitsSet0_9 += BitsSet5_9;
  }

  oldLen = LrDigits (res);

  if (Exp2 == 0) {
    LREAL  r, q, s, t;
    BOOL   rDefined = FALSE,
	   tDefined = FALSE;
    USHORT Len;
    INT	   i;

   /*
    *  Initialisierung von "r",	"q", so	dass das Endergebnis in	"res" steht
    *  ("p", "t" bezeichnen gespeicherte 10er-Potenzen):
    *	 2 gesetzte Bits: p * t	-> q  (gestartet wird mit t statt r)
    *	 3 gesetzte Bits: p * q	-> r
    *	 4 gesetzte Bits: p * r	-> q
    *	 :			:
    */
    if (BitsSet0_9 & 1)	{		/* #(gesetzte Bits) ungerade	*/
      r = res; q = (LREAL) wrk1;
    }
    else {				/* #(gesetzte Bits) gerade	*/
      r = (LREAL) wrk1; q = res;
    }

    /* Genauigkeit festlegen */
    if ((Len = oldLen +	(BitsSet5_9 ? 4	: 2)) <	8) Len = 8;

    /* Multiplikationen	*/
    if (Exp0) {
      tDefined++; t = LrConst1E	[Exp0];
    }
    for	(i = 0;	Exp1; Exp1 >>= 1, i++)
      if (Exp1 & 1) {
	if (tDefined ==	FALSE) {
	  tDefined++; t	= LrConstPow [i];
	}
	else {
	  LrSetPosDigits (q, Len);
	  Loss |=
	      LrMul (rDefined ?	r : (rDefined++, t), LrConstPow	[i], q,	-1);
	  s = q; q = r;	r = s;
	}
      }

    /* Ergebnis	*/
    if (rDefined == FALSE)	/* Genauigkeit noch unveraendert	*/
      return (LrAssign (res, t , Level));
  }
  else /* if (Exp2 != 0) */ {
    LREAL r, p,	q;
    SHORT BitsSet10_17 = 0,	/* #(gesetzte Bits) in Exp2		*/
	  MSB10_17     = 0,	/* Nummer des hoechstwertigen gesetzten	*/
				/* Bits	in Exp2	(von 0 an gezaehlt)	*/
	  resLen,
	  facLen;

    {/*	 Initialisierung von "r", "p", "q", so dass das	Endergebnis in "res"
      *	 steht:
      *	 Idee: "r", "p", "q" zeigen auf	virtuelle Speicherbereiche; dabei
      *	 haelt nach jedem Schritt der Potenzberechnung
      *	   *r :	das bis	dahin berechnete Teilprodukt des Endergebnisses,
      *	   *p :	die 10er-Potenz, die ggf. im letzten Schritt in	*r einging,
      *	   *q :	freien Arbeitsspeicher fuer den	naechsten Schritt.
      *	 Anschaulich koennen die virtuellen Speicherbereiche den Ecken eines
      *	 Dreiecks zugeordnet werden, anfangs z.B.:    q
      *						    p	r
      *
      *	 Im Lauf der Potenzberechnung tauschen die virtuellen Speicherbereiche
      *	 ihre Rollen, dem entsprechen folgende moegliche Zustaende:
      *
      *	  [0]:	q    [1]:  p	[2]:  r	   [3]:	 p    [4]:  q	 [5]:  r
      *	      p	  r	 r   q	    q	p      q   r	  r   p	     p	 q
      *
      *	 Im Endzustand haelt *r	das Endergebnis, "res" ist diesem virtuellen
      *	 Speicherbereich (der betreffenden Ecke	des Dreiecks) zuzuordnen;
      *	 im Fall eines Endzustands:   r	     muesste also anfangs "q" mit
      *				    q	p
      *
      *	 "res" initialisiert werden.
      *
      *	 Jeder Schritt der Potenzberechnung umfasst grundsaetzlich folgende
      *	 Aktionen:
      *	   (i)	*p * *p	-> *q, p <-> q
      *	   (ii)	falls entsprechendes Bit im Exponenten gesetzt,
      *		*p * *r	-> *q, q <-> r
      *
      *	 Fuer den Fall BitsSet0_9 > 0 stellt nachstehende Uebersicht Zustaende
      *	 und Zustandsuebergaenge dar (die Zustaende werden durch einen Index
      *	 gekennzeichnet):
      *
      *		      Bit == 1 :Bit == 0: Bit == 1
      *		       +----+  :	:  +----+
      *		       |    v  :	:  v	|
      *		       |    q  :	:  p	|
      *	 [0]: (0,1,2)  |  p   r:<------>:q   r	|  [3]:	(0,2,1)
      *		       |    |  :	:  |	|
      *		       |    p  :<---+	:  r	|
      *	 [1]: (2,0,1)  |  r   q:   +|-->:p   q	|  [4]:	(2,1,0)
      *		       |    |  :   ||	:  |	|
      *		       |    r  :   |+-->:  q	|
      *	 [2]: (1,2,0)  |  q   p:<--+	:r   p	|  [5]:	(1,0,2)
      *		       +----+  :	:  +----+
      *
      *	 Falls BitsSet0_9 == 0,	erfolgen modifizierte Aktionen,	wenn ERSTMALIG
      *	 das entsprechende Bit im Exponenten gesetzt ist:
      *	   (i)	 *p * *p -> *q,	p <-> q
      *	   falls kein weiteres Bit im Exponenten gesetzt, Ergebnis in q
      *	   sonst
      *	   (ii)	 *p * *p -> *q,	p <-> q
      *	   (iii) falls folgendes Bit im	Exponenten gesetzt,
      *		 *p * *q -> *r
      *		 sonst
      *		 q <-> r
      *
      *	 Die Aktionen im einzelnen (es liegen entweder Zustand 0 oder 3	vor):
      *
      *	 [0]:  q      (i)      p(r)   (ii)     q      (iii,0) [4]:  r
      *	     p	 r    -->    q	 r    -->    p	 r    -->	  p   q
      *
      *						      (iii,1) [0]:  q
      *						      -->	  p   r
      *
      *	 [3]:  p      (i)      q      (ii)     p      (iii,0) [1]:  p
      *	     q	 r    --> (r)p	 r    -->    q	 r    -->	  r   q
      *
      *						      (iii,1) [3]:  p
      *						      -->	  q   r
      *
      *	 Die Zustaende nach Teilschritt	(i) hinsichtlich "r", falls kein
      *	 weiteres Bit im Exponenten gesetzt ist, und nach Teilschritt (iii)
      *	 lassen	sich herleiten,	indem aus Zustand 0 (3)	in Zustand 2 (5)
      *	 uebergegangen wird, wenn erstmalig das	entsprechende Bit im Expo-
      *	 nenten	gesetzt	ist; im	folgenden wird dann wie	oben weiter verfahren:
      *
      *	 [0]:  q      (i)  [2]:	 r	(iii,0)	[4]:  r
      *	     p	 r    -->      q   p	-->	    p	q
      *
      *					(iii,1)	[0]:  q
      *					-->	    p	r
      *
      *	 [3]:  p      (i)  [5]:	 q	(iii,0)	[1]:  p
      *	     q	 r    -->      r   p	-->	    r	q
      *
      *					(iii,1)	[3]:  p
      *					-->	    q	r
      */
      static CHAR Status [6][6]	= {
       /* 0  1	2  3  4	 5 */	/* Status-Uebergang,			*/
	{ 3, 5,	4, 0, 2, 1 },	/*   falls Bit in Exp2 == 0		*/
	{ 1, 2,	0, 4, 5, 3 },	/*   falls Bit in Exp2 == 1		*/
	{ 2,-1,-1, 5,-1,-1 },	/*   falls BitsSet0_9 == 0 und		*/
				/*	   Bit in Exp2 erstmalig == 1	*/
				/* Index zur Initialisierung		*/
	{ 0, 2,	1, 0, 2, 1 },	/*   von r				*/
	{ 1, 0,	2, 2, 1, 0 },	/*   von p				*/
	{ 2, 1,	0, 1, 0, 2 }	/*   von q				*/
       /* Index	0 (1, 2) bedeutet Initialisierung mit res (wrk1, wrk2)	*/
      };

      LREAL  lr	[3];
      BOOL   tDefined  = (BOOL)	BitsSet0_9;
      INT    Index     = 0;
      USHORT Exp       = Exp2;

      loop {
	if (Exp	& 1) {
	  BitsSet10_17++;
	  if (tDefined)
	    Index = Status [1][Index];
	  else {
	    tDefined++;
	    Index = Status [2][Index];
	  }
	}
	else
	  Index	= Status [0][Index];

	if ((Exp >>= 1)	== 0) break;

	MSB10_17++;
      }	/* loop	*/

      lr [0] = res; lr [1] = (LREAL) wrk1; lr [2] = (LREAL) wrk2;
      r	= lr [Status [3][Index]];
      p	= lr [Status [4][Index]];
      q	= lr [Status [5][Index]];
    } /* Initialisierung von "r", "p", "q" */

    { /* Genauigkeiten festlegen */
      static CHAR GuardDigits [13][8] =	{
	{ -1, -1, -1, -1, -1, -1, -1, -1 },
	{  3,  3,  3,  3,  3,  3,  3,  3 },
	{  3,  3,  3,  3,  3,  3,  3,  3 },
	{  4,  4,  4,  4,  4,  4,  4,  4 },
	{  4,  4,  4,  5,  5,  5,  5,  5 },
	{  5,  5,  5,  5,  5,  5,  5,  5 },
	{  5,  5,  5,  6,  6,  6,  6,  6 },
	{ -1,  5,  5,  6,  6,  6,  6,  6 },
	{ -1, -1,  6,  6,  7,  7,  7,  7 },
	{ -1, -1, -1,  6,  7,  7,  7,  7 },
	{ -1, -1, -1, -1,  7,  7,  7,  7 },
	{ -1, -1, -1, -1, -1,  7,  8,  8 },
	{ -1, -1, -1, -1, -1, -1,  8,  8 }
      };

      if ((resLen =
	  oldLen + GuardDigits [BitsSet5_9 + BitsSet10_17][MSB10_17]) <	8)
	resLen = 8;
      facLen = resLen +	2;
    } /* Genauigkeiten festlegen */

    if (BitsSet0_9 == 0) {
      BOOL  rDefined = FALSE,
	    pDefined = FALSE;
      LREAL s;

      do {
	LrSetPosDigits (q, facLen);
	if (pDefined)
	  Loss |= LrMul	(p, p, q, -1);
	else {
	  pDefined++;
	  Loss |= LrMul	((LREAL) LrConst1E512, (LREAL) LrConst1E512, q,	-1);
	}
	if ((Exp2 & 1) == 0) {
	  s = p; p = q;	q = s;
	}
	else if	(rDefined) {
	  LrSetPosDigits (p, resLen);
	  if ((Loss |= LrMul (r, q, p, -1)) & LR_POS_INFINITY)
	    raise OVERFLOW;
	  s = r; r = p;	p = q; q = s;
	}
	else if	(Exp2 >>= 1) {
	  rDefined++;
	  LrSetPosDigits (p, facLen);
	  Loss |= LrMul	(q, q, p, -1);
	  if (Exp2 & 1)	{
	    LrSetPosDigits (r, resLen);
	    if ((Loss |= LrMul (p, q, r, -1)) &	LR_POS_INFINITY)
	      raise OVERFLOW;
	  }
	  else {
	    s =	q; q = r; r = s;
	  }
	}
	/* sonst besitzt die zu	berechnende 10er Potenz	die Form 10^(2^k) */
      }	while (Exp2 >>=	1);
    }
    else /* if (BitsSet0_9 != 0) */ {
      BOOL  rDefined = FALSE,
	    pDefined = FALSE,
	    tDefined = FALSE;
      LREAL s, t;
      INT   i;

      if ((BitsSet0_9 &	1) == 0) { /* Anzahl gesetzter Bits gerade */
	s = q; q = r; r	= s;
      }
      if (Exp0)	{
	tDefined++; t =	LrConst1E [Exp0];
      }
      for (i = 0; Exp1;	Exp1 >>= 1, i++)
	if (Exp1 & 1) {
	  if (tDefined == FALSE) {
	    tDefined++;	t = LrConstPow [i];
	  }
	  else {
	    LrSetPosDigits (q, resLen);
	    Loss |=
		LrMul (rDefined	? r : (rDefined++, t), LrConstPow [i], q, -1);
	    s =	q; q = r; r = s;
	  }
	}

      do {
	LrSetPosDigits (q, facLen);
	if (pDefined)
	  Loss |= LrMul	(p, p, q, -1);
	else {
	  pDefined++;
	  Loss |= LrMul	((LREAL) LrConst1E512, (LREAL) LrConst1E512, q,	-1);
	}
	if ((Exp2 & 1) == 0) {
	  s = p; p = q;	q = s;
	}
	else {
	  LrSetPosDigits (p, resLen);
	  if ((Loss |= LrMul (rDefined ? r : (rDefined++, t), q, p, -1))
	      &	LR_POS_INFINITY)
	    raise OVERFLOW;
	  s = r; r = p;	p = q; q = s;
	}
      }	while (Exp2 >>=	1);
    } /* else if (BitsSet0_9 !=	0) */
  } /* else if (Exp2 !=	0) */

  { /* Ergebnis	*/
    PLDIGIT rMSD = LrPMSD (res);
    SHORT   rLen = LrDigits (res);

    if (Level) {
      if (rLen > oldLen) {
	if (rMSD [oldLen] >= (LDIGIT) (LR_ROUND - 1)) {	/* Runden	*/
	  rLen = oldLen;
	  while	(rLen--	&& (LDIGIT) ++(rMSD [rLen]) == 0)
	    ;
	  if (rLen < 0)	{
	    LrSetPosNormal (res);
	    return (LrRoundingOverflow (res));
	  }
	}
	rLen = oldLen;			/* 0-Ziffern am	Ende entfernen	*/
	while (rMSD [--rLen] ==	0)
	  ;
	LrSetPosDigits (res, rLen+1);
	Loss = LR_LOSS;
      }	/* if (Len ...)	*/
    }
    else /* if (Level == 0) */ {
      LrSetPosDigits (res, oldLen);
      if (rLen > oldLen	+ 2) Loss = LR_LOSS;
    }
  } /* Ergebnis	*/

  return ((Loss	& LR_LOSS) | LR_NORMAL);

OVERFLOW:
  LrSetPosDigits (res, oldLen);
  return (LrOverflow (res, 1));
} /* LrPow10 */

VOID FAR CALL LrIncrPow10 (LREAL a)
 /***********************************************************************
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
{
  PLDIGIT aMSD = LrPMSD	(a);
  SHORT	  aLen = LrDigits (a) +	1;	/* 2 Schutzziffern mitverwenden	*/

  if (aMSD [aLen] < 0x4000) {		/* Addition von	3/4*B		*/
    aMSD [aLen] += 0xC000;		/* ohne	Uebertrag moeglich	*/
    LrSetPosDigits (a, aLen+1);
  }
  else {
    LDIGIT Dig = aMSD [aLen] += 0xC000;
    SHORT  Len = ++aLen;

    while (Len-- && (LDIGIT) ++(aMSD [Len]) == 0)
      ;
    if (Len < 0) {			/* tritt nur auf, wenn		*/
      LrExponent (a)++;			/* LrExponent (a) < LR_MAX_EXP	*/
      if (Dig) {
	LrSetPosDigits (a, aLen+1);
	MoveRight (aMSD, aLen, LDIGIT);
      }
      else
	LrSetPosDigits (a, 1);
      aMSD [0] = 1;
    }
    else if (Dig)
      LrSetPosDigits (a, aLen);
    else
      LrSetPosDigits (a, Len+1);
  }
} /* LrIncrPow10 */
