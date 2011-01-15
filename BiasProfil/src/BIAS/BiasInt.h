/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines Internal Definitions
 * ----------------------------------------------------------
 *
 * Copyright (C) 1993, 1994, 1997 Olaf Knueppel
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
 * $Id: BiasInt.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __BIASINTERN__
#define __BIASINTERN__

/************************************************************************
 *  Macros
 ************************************************************************/

#if defined (__INTEGERCOMPARE__)
#if defined (__BIGENDIAN__)
#define SIGNIDX 0
#else
#define SIGNIDX (sizeof (REAL) / sizeof (unsigned long) - 1)
#endif
#define SIGNBIT (1L << (8 * sizeof (unsigned long) - 1))

#define Sign(r) \
        (((unsigned long *) & (r)) [SIGNIDX] & SIGNBIT)

#define Zero(r) \
        ((sizeof (REAL) == sizeof (unsigned long) ?              \
        (((unsigned long *) & (r)) [SIGNIDX] & ~ SIGNBIT) :      \
        (((unsigned long *) & (r)) [SIGNIDX] & ~ SIGNBIT) |      \
        ((unsigned long *) & (r)) [1 - SIGNIDX]) == 0)
#else /* __INTEGERCOMPARE__ */
#define Sign(r) ((r) < 0.0)
#define Zero(r) ((r) == 0.0)
#endif /* __INTEGERCOMPARE__ */

#if defined (__BIASSETROUNDTONEAREST__)
#define _SetRoundToNearest()    _BiasRoundNear ()
#else
#define _SetRoundToNearest()
#endif

#if !defined (__BIASINLINEROUNDING__)
#define _BiasRoundInit()        BiasRoundInit ()
#define _BiasRoundUp()          BiasRoundUp ()
#define _BiasRoundDown()        BiasRoundDown ()
#define _BiasRoundNear()        BiasRoundNear ()
#endif

/************************************************************************
 *  Subroutines
 ************************************************************************/

VOID _BiasError (const char * const s);

#endif /* __BIASINTERN__ */
