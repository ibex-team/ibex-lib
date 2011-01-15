/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines Test Program
 * --------------------------------------------------
 *
 * Copyright (C) 1997 Olaf Knueppel
 *
 * This file is part of PROFIL/BIAS.
 *
 * PROFIL/BIAS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 * $Id: BiasTest.c 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <BIAS/Bias0.h>
#include <stdio.h>
#include <stdlib.h>

static void error (char *s)
{
  fprintf (stderr, "ERROR: %s\n", s);
}

static void Dump (char *s, double Result)
{
  int i;
  unsigned char *p = (unsigned char *) & Result;

  fprintf (stderr, "%+2.2f %-8.8s: ", Result, s);
  for (i = 0; i < sizeof (double); i++)
    fprintf (stderr, "%02X ", *p++);
  fprintf (stderr, "\n");
}

static void DumpValues (void)
{
  static volatile double One = 1.0;
  static volatile double Ten = 10.0;

  volatile double Result;

  BiasRoundNear ();
  Result = One / Ten;
  Dump ("Nearest", Result);
  BiasRoundUp ();
  Result = One / Ten;
  Dump ("Up", Result);
  BiasRoundDown ();
  Result = One / Ten;
  Dump ("Down", Result);

  BiasRoundNear ();
  Result = -One / Ten;
  Dump ("Nearest", Result);
  BiasRoundUp ();
  Result = -One / Ten;
  Dump ("Up", Result);
  BiasRoundDown ();
  Result = -One / Ten;
  Dump ("Down", Result);
}

int main (int argc, char *argv[])
{
  int rc;
  BIASINTERVAL x, y;
  static double One      =  1.0;
  static double MinusOne = -1.0;
  static double Three    =  3.0;
  static double OneThird;

  BiasInit ();
  OneThird = One / Three;

  BiasHullRR (& x, & MinusOne, & One);
  BiasDivIR (& y, & x, & Three);

  rc = 0;

  if (BiasInf (& y) >= -OneThird) {
    error ("illegal downward rounding");
    rc = 1;
  }
  if (BiasSup (& y) <=  OneThird) {
    error ("illegal upward rounding");
    rc = 1;
  }

  if (argc > 1) DumpValues ();

  return rc;
}
