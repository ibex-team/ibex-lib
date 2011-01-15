/************************************************************************
 *
 * Definitions of Misc. Functions
 * ------------------------------
 *
 * Copyright (C) 1993, 1994 Thomas Simenec
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
 * $Id: MiscFunctions.h 560 2008-10-14 04:56:19Z keil $
 *
 ************************************************************************/

#ifndef __MISCFUNCTIONS__
#define __MISCFUNCTIONS__ 

#include <Configuration.h>

#ifdef __16BIT__
  #define PROFIL_RAND_MAX 65535.0
#else
  #define PROFIL_RAND_MAX 4294967296.0
#endif

#if defined (__cplusplus)
extern "C" {
#endif

REAL Random (void);

REAL Rand01 (void);

unsigned long Rand (void);

VOID Randomize (void);

INT Binom (INT, INT);

INT Legendre (INT, INT);

#if defined (__cplusplus)
}
#endif

#endif /* __MISCFUNCTIONS__ */


