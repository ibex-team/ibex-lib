/************************************************************************
*  $Id: LrPi.c			Test der Langzahlarithmetik: Berechnung von Pi
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
*  Version 1.1	05.07.90	Dirk Husung
*  Letzte Aenderung:		Wed 14-Oct-1992 13:40:10
*************************************************************************/

#include "Portab.h"

#include <stdio.h>
#include <malloc.h>
#if defined (__I386__)
#include <time.h>
#else
#include <sys/times.h>
#ifdef m68000
#define TIMES_TICKS 50
#else
#define TIMES_TICKS 60
#endif
#endif

#include "Lr.h"
#include "LrIntern.h"
#include "DynMem.h"

/************************************************************************
*  Konstanten-Definitionen
*************************************************************************/

#define Guards	4

/************************************************************************
*  Implementierung der Funktionen
*************************************************************************/

DOUBLE ElapsedTime (VOID) /* in sec */
{
  static DOUBLE LastTime = -1.0;
  DOUBLE Time, DiffTime;

#if defined (__I386__)
  Time = (DOUBLE) clock () / CLOCKS_PER_SEC;
#else
  {
    struct tms buffer;
    times (&buffer);
    Time = (DOUBLE) (buffer.tms_utime + buffer.tms_stime) / TIMES_TICKS;
  }
#endif

  if (LastTime == -1.0) LastTime = Time;

  DiffTime = Time - LastTime;
  LastTime = Time;

  return (DiffTime);
} /* ElapsedTime */

VOID LrArctan (LREAL lnum, LREAL arg)
 /***********************************************************************
  *  Funktion: naeherungsweise Berechnung von LrArctan (x)
  *	       durch Auswerten der ersten Glieder der Potenzreihe
  *	       arctan (x) = x - x^3/3 + x^5/5 - x^7/7 +- ...
  */
{
  USHORT Digits  = LrDigits (lnum);
  USHORT Size	 = ((Digits + Guards) * sizeof (LDIGIT) + sizeof (LREALHDR)
		   + 3) & ~3;
  USHORT wrkSize = ((Digits + 32) * sizeof (LDIGIT) + 3) & ~3;

  LREAL  sum	 = LrNew (Digits, Size);
  LREAL  term	 = LrNew (Digits, Size);
  LREAL  potenz	 = LrNew (Digits, Size);
  LREAL  quad	 = LrNew (Digits, Size); /* AT&T C++ 2.0 erfordert	*/
  LREAL  temp	 = LrNew (Digits, Size); /* getrennte Variablendef.	*/
  LREAL  lnum_k	 = LrNew (2, 8);
  LREAL  t;

  PVOID  wrk	 = MemNew (wrkSize);
  ULONG  k	 = 1L;
  BOOL	 plus	 = FALSE;

  LrAssign (sum, arg, 1);
  LrAssign (potenz, arg, 1);
  LrMul (arg, arg, quad, 1);

  do {
    LrSetDigits (temp, Digits); LrMul (potenz, quad, temp, 1);
    t = potenz; potenz = temp; temp = t;
    ultolr (lnum_k, k += 2);
    LrSetDigits (term, Digits); LrDiv (potenz, lnum_k, term, wrk, 1);
    if (plus) {
      plus = FALSE; LrSetDigits (temp, Digits); LrAdd (sum, term, temp, 1);
    }
    else {
      plus++;	    LrSetDigits (temp, Digits); LrSub (sum, term, temp, 1);
    }
    t = sum; sum = temp; temp = t;
  } while (LrCmp (sum, temp));

  LrSetDigits (lnum, Digits); LrAssign (lnum, sum, 1);

  MemDelete (wrk, wrkSize);

  LrDelete (sum,    Size);
  LrDelete (term,   Size);
  LrDelete (potenz, Size);
  LrDelete (quad,   Size);
  LrDelete (temp,   Size);
  LrDelete (lnum_k, 8);
} /* LrArctan */

VOID Pi (USHORT Decimals)
 /***********************************************************************
  *  Funktion : naeherungsweise Berechnung von pi nach folgenden Formeln:
  *		GAUSS:
  *		  pi/4 = 12*LrArctan (1/18) + 8*LrArctan (1/57) - 5*LrArctan (1/239)
  *		MACHIN:
  *		  pi/4 =  4*LrArctan (1/5) - LrArctan (1/239)
  *  Eingaben : Decimals: # gewuenschter Nachkommastellen.
  *  Ergebnis : ---
  */
{
  USHORT Digits  = (Decimals >> 2) + 1;
  USHORT Size	 = ((Digits + Guards) * sizeof (LDIGIT) + sizeof (LREALHDR)
		   + 3) & ~3;
  USHORT wrkSize = ((Digits + 32) * sizeof (LDIGIT) + 3) & ~3;

  LREAL  gauss	 = LrNew (Digits, Size),
	 machin  = LrNew (Digits, Size),
	 arg	 = LrNew (Digits, Size),
	 atan	 = LrNew (Digits, Size),
	 temp	 = LrNew (Digits, Size),
	 lnum	 = LrNew (Digits, Size),
	 one	 = LrNew (2, 8),
	 t;

  PVOID  wrk	 = MemNew (wrkSize);

  ustolr (one, 1);

 /*
  *  Formel von GAUSS:
  *    pi = 48*LrArctan (1/18) + 32*LrArctan (1/57) - 20*LrArctan (1/239)
  */
  ustolr (lnum, 18);
  LrSetDigits (arg  , Digits); LrDiv (one, lnum, arg, wrk, 1);
  LrSetDigits (atan , Digits); LrArctan (atan, arg);
  ustolr (lnum, 48);
  LrSetDigits (gauss, Digits); LrMul (lnum, atan, gauss, 1);

  ustolr (lnum, 57);
  LrSetDigits (arg  , Digits); LrDiv (one, lnum, arg, wrk, 1);
  LrSetDigits (atan , Digits); LrArctan (atan, arg);
  ustolr (lnum, 32);
  LrSetDigits (temp , Digits); LrMul (lnum, atan, temp, 1);
  LrSetDigits (lnum , Digits); LrAdd (gauss, temp, lnum, 1);
  t = gauss; gauss = lnum; lnum = t;

  ustolr (lnum, 239);
  LrSetDigits (arg  , Digits); LrDiv (one, lnum, arg, wrk, 1);
  LrSetDigits (atan , Digits); LrArctan (atan, arg);
  ustolr (lnum, 20);
  LrSetDigits (temp , Digits); LrMul (lnum, atan, temp, 1);
  LrSetDigits (lnum , Digits); LrSub (gauss, temp, lnum, 1);
  t = gauss; gauss = lnum; lnum = t;
 /*
  LrOutput (stdout, "Pi (Gauss) ", gauss, 1);
  */
 /*
  *  Formel von MACHIN:
  *    pi = 16*LrArctan (1/5) - 4*LrArctan (1/239)
  */
  ustolr (lnum, 5);
  LrSetDigits (arg  , Digits); LrDiv (one, lnum, arg, wrk, 1);
  LrSetDigits (atan , Digits); LrArctan (atan, arg);
  ustolr (lnum, 16);
  LrSetDigits (machin, Digits); LrMul (lnum, atan, machin, 1);

  ustolr (lnum, 239);
  LrSetDigits (arg  , Digits); LrDiv (one, lnum, arg, wrk, 1);
  LrSetDigits (atan , Digits); LrArctan (atan, arg);
  ustolr (lnum, 4);
  LrSetDigits (temp , Digits); LrMul (lnum, atan, temp, 1);
  LrSetDigits (lnum , Digits); LrSub (machin, temp, lnum, 1);
  t = machin; machin = lnum; lnum = t;
 /*
  LrOutput (stdout, "Pi (Machin)", machin, 1);
  */
  LrWrite (stdout, "Pi (Gauss) ", gauss,  Decimals, 3, LR_RND_NEAR);
  LrWrite (stdout, "Pi (Machin)", machin, Decimals, 3, LR_RND_NEAR);

  MemDelete (wrk, wrkSize);

  LrDelete (gauss,  Size);
  LrDelete (machin, Size);
  LrDelete (arg,    Size);
  LrDelete (atan,   Size);
  LrDelete (temp,   Size);
  LrDelete (lnum,   Size);
  LrDelete (one,    8);
} /* Pi */

/************************************************************************
*  Hauptprogramm
*************************************************************************/

INT main (VOID)
{
  USHORT Decimals = 0;
  INT	 c;
  DOUBLE Time;

  LrInit ();

  puts ("---- Berechnung von Pi ----");
  printf ("gewuenschte Zahl an Nachkommastellen [16..10000]: ");

  while ((c = getc (stdin)) >= '0' && c <= '9')
    Decimals = Decimals * 10 + (c - '0');

  if (Decimals < 16)
    Decimals = 16;
  else if (Decimals > 10000)
    Decimals = 10000;

  puts ("");

  ElapsedTime ();
  Pi (Decimals);
  Time = ElapsedTime ();
  printf ("ElapsedTime = %7.3lfms\n", Time*1.0e3);

  return (0);
} /* main */
