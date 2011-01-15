/************************************************************************
*  $Id: LiTest0.c		Intervall-Langzahlarithmetik, Test
*
*  Copyright (C) 1991, 1992 Dirk Husung
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
*  Version 1.0	17.06.91	Dirk Husung
*  Letzte Änderung:		Fri 15-May-1992 15:48:55
*************************************************************************/

#include "Portab.h"

#if defined (__ANSI__)
#include <stdio.h>
#include <string.h>
#else
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#endif

#include "Li.h"
#include "LrIntern.h"

/************************************************************************
*  Konstanten-Definitionen
*************************************************************************/

#define UPPER_BND	20
#define DIGITS		20

/************************************************************************
*  Implementierung lokaler Funktionen
*************************************************************************/

static VOID LiWrite (FILE * File, PCHAR Name, LIREAL li,
  USHORT FracDigits, USHORT ExpDigits)
 /***********************************************************************
  *  Funktion : Ausgabe einer Intervall-Langzahl in Exponentialdarstellung
  *		zur Basis 10.
  *  Eingaben : File	   : Ausgabedatei.
  *		Name	   : Ausgabetext "Name = " oder "", NULL
  *		lr	   : Langzahl.
  *		FracDigNum : # Nachkommastellen.
  *		ExpDigNum  : # Exponentenstellen.
  *  Ergebnis : ---
  *  Info     : Bei zu wenig Speicher erfolgt Programmabbruch (exit-Code -1).
  */
{
  PCHAR   res;
  PCHAR   s1, s2;
  PVOID   wrk1, wrk2, wrk3;
  USHORT  Len;
  USHORT  Size1, Size2;

  if (FracDigits < 1)
    FracDigits = 1;
  if (ExpDigits  < 1)
    ExpDigits = 1;
  else if (ExpDigits > 6)
    ExpDigits = 6;

  Len	= (USHORT) (( ((((LONG) FracDigits + 9) >> 2) << 16) + 83845L) / 78913L);
  Size1 = (Len + 14) * sizeof (LDIGIT) + sizeof (LREALHDR);
  Size2 = (Len + 33) * sizeof (LDIGIT);

  if (	 (res  = (PCHAR) malloc ((FracDigits+14)*2+3)) == NULL
      || (s1   = (PCHAR) malloc (FracDigits+14)) == NULL
      || (s2   = (PCHAR) malloc (FracDigits+14)) == NULL
      || (wrk1 = (PVOID) malloc (Size1)) == NULL
      || (wrk2 = (PVOID) malloc (Size1)) == NULL
      || (wrk3 = (PVOID) malloc (Size2)) == NULL)
    exit (-1);

  litosze (li, res, FracDigits, ExpDigits,
      s1, s2, wrk1, wrk2, wrk3, LI_ESTR_FRACCOM | LI_STR_SINGLE);

  if (Name && *Name) fprintf (File, "%s = ", Name);

  fprintf (File, "%s\n", res);

  free ((PCHAR) res);
  free ((PCHAR) s1);
  free ((PCHAR) s2);
  free ((PCHAR) wrk1);
  free ((PCHAR) wrk2);
  free ((PCHAR) wrk3);
} /* LiWrite */

static VOID NEAR LrEuler (VOID)
 /***********************************************************************
  *  Funktion : Berechnung der Eulerschen Zahl "e" mittels Taylor-Entwicklung
  *		(Punkt)
  *  Ergebnis : ---
  */
{
  static USHORT sum   [DIGITS+ 2] = { 0x0002, 0x0001, 0x0002, 0x8000 };
  static USHORT addto [DIGITS+ 2] = { 0x0002, 0x0000, 0x8000, 0x0000 };
  static USHORT count [	4] 	  = { 0x0002, 0x0001, 0x0003, 0x0000 };

  USHORT wrk   [DIGITS+ 4];
  USHORT wrk1  [DIGITS+32];
  INT	 i;

  /* Rechnung:
       for count = 3 to n step 1 do {
	 addto /= count; sum += addto;
       } */

  for (i = 3; i < UPPER_BND; i++) {
    count [2] = i;

    wrk [0] = DIGITS;
    LrDiv ((LREAL) addto, (LREAL) count, (LREAL) wrk, wrk1, 1);
    memcpy ((PCHAR) addto, (PCHAR) wrk, DIGITS * sizeof (LDIGIT));

    wrk [0] = DIGITS;
    LrAdd ((LREAL) sum, (LREAL) addto, (LREAL) wrk, 1);
    memcpy ((PCHAR) sum, (PCHAR) wrk, DIGITS * sizeof (LDIGIT));
  } /* Ergebnis in "sum" */

  LrWrite (stdout, "e (LrEuler)", (LREAL) sum, 30, 3, LR_RND_NEAR);
} /* LrEuler */

static VOID NEAR LiEuler (VOID)
 /***********************************************************************
  *  Funktion : Berechnung der Eulerschen Zahl "e" mittels Taylor-Entwicklung
  *		(Intervall)
  *  Ergebnis : ---
  */
{
  static USHORT sum [DIGITS*2+5] = {
    0x0000, 0x0002, 0x0001, 0x0002, 0x8000, 0x0002, 0x0001, 0x0002, 0x8000
  };
  static USHORT addto [DIGITS*2+5] = {
    0x0000, 0x0002, 0x0000, 0x8000, 0x0000, 0x0002, 0x0000, 0x8000, 0x0000
  };
  static USHORT count [9] = {
    0x0000, 0x0002, 0x0001, 0x0003, 0x0000, 0x0002, 0x0001, 0x0003, 0x0000
  };
  USHORT wrk  [DIGITS*2+2+5];
  USHORT wrk1 [DIGITS+32];
  USHORT wrk2 [DIGITS*2+5];
  INT	 i;

  /* Initialisierung:
       sum = 2.5; addto = 0.5; */

  LiPack ((LIREAL) sum);
  LiPack ((LIREAL) addto);

  /* Rechnung:
       for count = 3 to n step 1 do {
	 addto /= count; sum += addto;
       } */

  for (i = 3; i < UPPER_BND; i++) {
    count [3] = count [7] = i;

    wrk [1] = DIGITS;
    LiDiv ((LIREAL) addto, (LIREAL) count, (LIREAL) wrk, wrk1, wrk2, 0);
    memcpy ((PCHAR) addto, (PCHAR) wrk, (DIGITS*2+5) * sizeof (LDIGIT));

    wrk [1] = DIGITS;
    LiAdd ((LIREAL) sum, (LIREAL) addto, (LIREAL) wrk, 0);
    memcpy ((PCHAR) sum, (PCHAR) wrk, (DIGITS*2+5) * sizeof (LDIGIT));
  } /* Ergebnis in "sum" */

  LiWrite (stdout, "e (LiEuler)", (LIREAL) sum, 30, 3);
} /* LiEuler */

/************************************************************************
*  Hauptprogramm
*************************************************************************/

INT main(VOID)
{
  LrEuler ();
  LiEuler ();

  return (0);
} /* main */
