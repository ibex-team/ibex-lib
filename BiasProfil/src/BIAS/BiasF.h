/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines Standard Functions
 * --------------------------------------------------------
 *
 * Copyright (C) 1993, 1997 Olaf Knueppel
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
 * $Id: BiasF.h 560 2008-10-14 04:56:19Z keil $
 *
 ************************************************************************/

#ifndef __BIASF__
#define __BIASF__

#include <BIAS/Bias0.h>

/************************************************************************
 *  Definition of Constants, Types, and Variables
 ************************************************************************/

extern REAL     BiasPi;                 /* 3.141...                     */
extern REAL     BiasTwoPi;
extern REAL     BiasPiHalf;
extern REAL     BiasPiQuarter;
extern REAL     BiasE;
extern REAL     BiasSqrt2;
extern REAL     BiasInvSqrt2;           /* 1/ Sqrt(2)                   */
extern REAL     BiasLn10;

/************************************************************************
 *  Function Prototypes
 ************************************************************************/

#if defined (__cplusplus)
extern "C" {
#endif

VOID    BiasFuncInit    (void);

VOID    BiasSin    (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasCos    (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasTan    (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasCot    (BIASINTERVAL * const, const BIASINTERVAL * const);

VOID    BiasArcSin (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasArcCos (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasArcTan (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasArcCot (BIASINTERVAL * const, const BIASINTERVAL * const);

VOID    BiasSinh   (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasCosh   (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasTanh   (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasCoth   (BIASINTERVAL * const, const BIASINTERVAL * const);

VOID    BiasArSinh (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasArCosh (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasArTanh (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasArCoth (BIASINTERVAL * const, const BIASINTERVAL * const);

VOID    BiasExp    (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasLog    (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasLog10  (BIASINTERVAL * const, const BIASINTERVAL * const);

VOID    BiasIAbs   (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasSqr    (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasSqrt   (BIASINTERVAL * const, const BIASINTERVAL * const);
VOID    BiasRoot   (BIASINTERVAL * const,
                    const BIASINTERVAL * const, const INT);

VOID    BiasPowerN (BIASINTERVAL * const,
                    const BIASINTERVAL * const, const INT);
VOID    BiasPowerI (BIASINTERVAL * const,
                    const BIASINTERVAL * const,
                    const BIASINTERVAL * const);

#if defined (__cplusplus)
}
#endif

#endif /* __BIASF__ */
