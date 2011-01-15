/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines Timing Measurement Program
 * ----------------------------------------------------------------
 *
 * Copyright (C) 1998 Olaf Knueppel
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
 * $Id: BiasTimes.c 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <BIAS/Bias0.h>
#include <BIAS/Bias1.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VECTORSIZE  1000
#define VECTORSIZE_STR "1000"

#define Ticks2Seconds(Ticks) (((double) (Ticks)) / CLOCKS_PER_SEC)

static unsigned long ElapsedTicks (void)
{
  return (unsigned long) clock ();
}

static double a = 1.234567890123456789;
static double b = 5.678901234567890123;
static BIASINTERVAL B;
static BIASINTERVAL VB[VECTORSIZE];

static double c;
static BIASINTERVAL C;
static BIASINTERVAL VC[VECTORSIZE];

static void Empty (void)
{
}

static void RoundingModeSwitch (void)
{
  BiasRoundNear ();
}

static void FloatAdd (void)
{
  c = a + b;
}

static void FloatMul (void)
{
  c = a * b;
}

static void FloatMac (void)
{
  c += a * b;
}

static void FloatDiv (void)
{
  c = a / b;
}

static void IntervalAdd (void)
{
  BiasAddRR (& C, & a, & b);
}

static void IntervalMul1 (void)
{
  BiasMulRR (& C, & a, & b);
}

static void IntervalMul2 (void)
{
  BiasMulRI (& C, & a, & B);
}

static void IntervalMac1 (void)
{
  BiasMacRR (& C, & a, & b);
}

static void IntervalMac2 (void)
{
  BiasMacRI (& C, & a, & B);
}

static void IntervalDiv1 (void)
{
  BiasDivRR (& C, & a, & b);
}

static void IntervalDiv2 (void)
{
  BiasDivRI (& C, & a, & B);
}

static void IntervalVectorMul (void)
{
  BiasMulRVI (VC, & a, VB, VECTORSIZE);
}

static void IntervalVectorMac (void)
{
  BiasMacRVI (VC, & a, VB, VECTORSIZE);
}

static void IntervalVectorDiv (void)
{
  BiasDivVIR (VC, VB, & a, VECTORSIZE);
}

static double TestLoop (unsigned long nScale, void (*Subroutine) (void))
{
#define NLOOPS 1000000
  unsigned int i;
  unsigned long n = NLOOPS / nScale;
  unsigned long StartTicks, StopTicks;

  Subroutine ();
  StartTicks = ElapsedTicks ();
  for (i = 0; i < n; i++) Subroutine ();
  StopTicks = ElapsedTicks ();

  return Ticks2Seconds (StopTicks - StartTicks) / n * 1e9;
}

static void Testing (char *Name, unsigned long nScale,
                     void (*Subroutine) (void))
{
  double UsedTime;
  static double EmptyLoop = -1.0;

  if (EmptyLoop < 0.0) EmptyLoop = TestLoop (1, Empty);

  UsedTime = TestLoop (nScale, Subroutine);
  printf ("%-48.48s: %8.0f ns\n", Name, UsedTime - EmptyLoop);
}

static void TestInit (void)
{
  int i;

  BiasHullRR (& B, & a, & b);
  for (i = 0; i < VECTORSIZE; i++) VB[i] = B;
}

int main ()
{
  printf ("\nBIAS Timing Tests\n=================\n\n");
  printf ("Please note: The tests are quite simple and do not care\n");
  printf ("             about cache usage, good accuracy of the clock\n");
  printf ("             nor any thing else. They are provided just as\n");
  printf ("             a simple method to determine the order of magnitude\n");
  printf ("             of the used time of some basic functions.\n\n");

  TestInit ();

  Testing ("Rounding Mode Switch", 1, RoundingModeSwitch);

  Testing ("Floating Point Addition", 1, FloatAdd);
  Testing ("Floating Point Multiplication", 1, FloatMul);
  Testing ("Floating Point Multiply/Accumulate", 1, FloatMac);
  Testing ("Floating Point Division", 1, FloatDiv);

  Testing ("BiasAddRR", 1, IntervalAdd);
  Testing ("BiasMulRR", 1, IntervalMul1);
  Testing ("BiasMacRR", 1, IntervalMac1);
  Testing ("BiasDivRR", 1, IntervalDiv1);

  Testing ("BiasMulRI", 1, IntervalMul2);
  Testing ("BiasMacRI", 1, IntervalMac2);
  Testing ("BiasDivRI", 1, IntervalDiv2);

  Testing ("BiasMulRVI (n = " VECTORSIZE_STR ")", VECTORSIZE, IntervalVectorMul);
  Testing ("BiasMacRVI (n = " VECTORSIZE_STR ")", VECTORSIZE, IntervalVectorMac);
  Testing ("BiasDivVIR (n = " VECTORSIZE_STR ")", VECTORSIZE, IntervalVectorDiv);

  return 0;
}
