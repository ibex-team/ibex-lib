/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines 80x86-Rounding (Watcom)
 * -------------------------------------------------------------
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
 * $Id: BiasRnd.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __BIASRND__
#define __BIASRND__

#include <BIAS/BiasType.h>

#undef __BIASINLINEROUNDING__

/*
 * If the following macro is defined,
 * division by zero aborts with an error message;
 * otherwise, bounds containing +/-oo
 * or NaN are computed.
 */
#undef __BIASRAISEDIVIDEBYZERO__

/*
 * If the following macro is defined,
 * the rounding mode is set to nearest after
 * each interval operation.
 */
#define  __BIASSETROUNDTONEAREST__

/*
 * The following macro defines the number of invalid bits
 * obtained by the standard C library functions (e.g. sin(), cos(), ...)
 */
#define __BIASSTDFUNCINVALIDBITS__      0

#ifdef __cplusplus
extern "C" {
#endif

void BiasRoundInit (void);
void BiasRoundUp   (void);
void BiasRoundDown (void);
void BiasRoundNear (void);

#ifdef __cplusplus
}
#endif

#endif /* __BIASRND__ */
