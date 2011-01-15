/************************************************************************
*  $Id: LrIntern.h		Langzahl-Interna
*
*  Copyright (C) 1990, 1992 Dirk Husung
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
*  Version 1.0	21.05.90	Dirk Husung
*  Letzte Aenderung:		Tue 06-Oct-1992 18:35:50
************************************************************************/

#ifndef __LRINTERN__
#define __LRINTERN__

#if defined (__cplusplus)
extern "C" {
#endif

/************************************************************************
*  Funktionsprototypen
************************************************************************/

extern VOID dInput ARGS ((PCHAR sz, PDOUBLE pd));
  /**********************************************************************
  *  Funktion : Einlesen einer DOUBLE-Zahl im Format
  *		[+-]?"0x1."[0-9A-Fa-f']+("Q"[+-]?[0-9]+)?
  *		[+-]?"0x0."[0-9A-Fa-f']+"Q-1022"?
  *  Eingaben : sz	: Zeichenketten-Darstellung der Zahl
  *  Ausgaben : pd	: Zeiger auf DOUBLE-Zahl
  *  Ergebnis : ---
  */

extern VOID dOutput ARGS ((FILE * File, PCHAR Name, DOUBLE d));
  /**********************************************************************
  *  Funktion : Ausgabe einer DOUBLE-Zahl im Format
  *		+0x0.1234'5678'9ABC'DQ-1022
  *		-0x1.1234'5678'9ABC'DQ+0001
  *  Eingaben : File	: Ausgabedatei
  *		Name	: Variablenname oder "", NULL
  *		d	: DOUBLE-Zahl
  *  Ergebnis : ---
  */

extern VOID dRand ARGS ((PDOUBLE pd));
  /**********************************************************************
  *  Funktion :	Erzeugt	eine "Zufalls"-Mantisse einer DOUBLE-Zahl.
  *  Eingaben :	pd	: DOUBLE-Zahl.
  *  Ergebnis :	---
  */

extern INT LrInput ARGS ((PCHAR sz, LREAL lr, USHORT Level));
  /**********************************************************************
  *  Funktion : Einlesen einer LREAL-Zahl im Format
  *		[+-]?"0x0."[0-9A-Fa-f']+("L"[+-]?[0-9]+)?
  *  Speicher : Bedarf fuer "lr" in Bytes:
  *		  lrDigits (lr) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *		  + 1 * sizeof (LDIGIT)
  *  Eingaben : sz	: 0-terminierte Zeichenkette
  *		lr	: LREAL-Zahl
  *		Level	:
  *		    = 0 : Einlesen von bis zu lrDigits (lr) + 1 LREAL-Ziffer
  *		    > 0 : zusaetzlich: Runden zur naechsten Langzahl,
  *			  Entfernen von 0-Ziffern am Ende.
  *  Ergebnis : LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NORMAL,
  *		LR_NORMAL + LR_LOSS, falls zu viele Ziffern angegeben wurden.
  */

extern VOID LrOutput ARGS ((
  FILE * File, PCHAR Name, LREAL lr, USHORT Guards));
  /**********************************************************************
  *  Funktion : Ausgabe einer LREAL-Zahl im Format
  *		+0x0.1234'5678'9ABC'DEF0'L+00001
  *  Eingaben : File	: Ausgabedatei
  *		Name	: Ausgabetext "Name = " oder "", NULL
  *		lr	: LREAL-Zahl
  *		Guards	: # auszugebender Schutzziffern
  *  Ergebnis : ---
  */

extern INT LrRead ARGS ((PCHAR sz, LREAL lr, SHORT Level));
  /**********************************************************************
  *  Funktion : Konvertierung Dezimalzahl-Zeichenkette -> Langzahl,
  *  Eingaben : sz	: Dezimalzahl-Zeichenkette.
  *		lr	: Langzahl.
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
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *		+ LR_LOSS, falls Genauigkeitsverlust moeglich.
  *  Info     : Bei zu wenig Speicher erfolgt Programmabbruch (exit-Code -1).
  */

extern VOID LrWrite ARGS ((FILE * File, PCHAR Name, LREAL lr,
  USHORT FracDigits, USHORT ExpDigits, INT Rnd));
  /**********************************************************************
  *  Funktion : Ausgabe einer Langzahl in Exponentialdarstellung zur Basis 10.
  *  Eingaben : File	   : Ausgabedatei.
  *		Name	   : Ausgabetext "Name = " oder "", NULL
  *		lr	   : Langzahl.
  *		FracDigNum : # Nachkommastellen.
  *		ExpDigNum  : # Exponentenstellen.
  *		Rnd	   : Rundungsmodus:
  *			     LR_RND_NEAR, LR_RND_DOWN, LR_RND_UP, LR_RND_CHOP.
  *  Ergebnis : ---
  *  Info     : Bei zu wenig Speicher erfolgt Programmabbruch (exit-Code -1).
  */

extern VOID LrRand ARGS ((LREAL lr));
  /**********************************************************************
  *  Funktion : Erzeugt eine "Zufalls"-Langzahl gegebener Laenge.
  *  Eingaben : lr	: Langzahl
  *  Ergebnis : ---
  */

#if defined (__cplusplus)
}
#endif
#endif /* !__LRINTERN__ */
