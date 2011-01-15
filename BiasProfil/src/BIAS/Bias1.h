/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines Level 1
 * ---------------------------------------------
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
 * $Id: Bias1.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __BIAS1__
#define __BIAS1__

#include <BIAS/Bias0.h>

/************************************************************************
 *  Function Prototypes
 ************************************************************************/

#if defined (__cplusplus)
extern "C" {
#endif

VOID    BiasPredVR      (REAL * const  pr,
			 const REAL * const  pa,
                         const INT dim);
INT     BiasPredVI      (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pA,
                         const INT dim);

VOID    BiasSuccVR      (REAL * const  pr,
                         const REAL * const  pa,
                         const INT dim);
VOID    BiasSuccVI      (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pA,
                         const INT dim);

VOID    BiasAddVRVR     (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const REAL * const pb,
                         const INT dim);
VOID    BiasAddVRVI     (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const BIASINTERVAL * const pB,
                         const INT dim);
VOID    BiasAddVIVI     (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pa,
			 const BIASINTERVAL * const pB,
                         const INT dim);

VOID    BiasSubVRVR     (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const REAL * const pb,
                         const INT dim);
VOID    BiasSubVRVI     (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const BIASINTERVAL * const pB,
                         const INT dim);
VOID    BiasSubVIVR     (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pA,
			 const REAL * const pb,
                         const INT dim);
VOID    BiasSubVIVI     (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pa,
			 const BIASINTERVAL * const pB,
                         const INT dim);

VOID    BiasMulRVR      (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const REAL * const pb,
                         const INT dim);
VOID    BiasMulIVR      (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pA,
                         const REAL * const pb,
                         const INT dim);
VOID    BiasMulRVI      (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const BIASINTERVAL * const pB,
                         const INT dim);
VOID    BiasMulIVI      (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pa,
                         const BIASINTERVAL *pB,
                         const INT dim);

VOID    BiasMacRVR      (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const REAL * const pb,
                         const INT dim);
VOID    BiasMacIVR      (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pA,
                         const REAL * const pb,
                         const INT dim);
VOID    BiasMacRVI      (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const BIASINTERVAL * const pB,
                         const INT dim);
VOID    BiasMacIVI      (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pA,
                         const BIASINTERVAL *pB,
                         const INT dim);

VOID    BiasMacRVIs     (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const BIASINTERVAL * const pB,
                         const INT dim,
                         const INT Bstep);
VOID    BiasMacsRVIs    (BIASINTERVAL * const pR,
			 const REAL * const pa,
                         const BIASINTERVAL * const pB,
			 const INT dim,
                         const INT Rstep,
                         const INT Bstep);

VOID    BiasDivVRR      (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const REAL * const pb,
                         const INT dim);
VOID    BiasDivVIR      (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pA,
			 const REAL * const pb,
                         const INT dim);
VOID    BiasDivVRI      (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const BIASINTERVAL * const pB,
                         const INT dim);
VOID    BiasDivVII      (BIASINTERVAL * const pR,
                         const BIASINTERVAL *pA,
			 const BIASINTERVAL * const pB,
                         const INT dim);

VOID    BiasMacVRVR     (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const REAL * const pb,
                         const INT dim);
VOID    BiasMacVRVI     (BIASINTERVAL * const pR,
                         const REAL * const pa,
			 const BIASINTERVAL * const pB,
                         const INT dim);
VOID    BiasMacVIVI     (BIASINTERVAL * const pR,
                         const BIASINTERVAL *pA,
                         const BIASINTERVAL *pB,
                         const INT dim);

VOID    BiasInfV        (REAL * const pr,
                         const BIASINTERVAL * const pA,
                         const INT dim);
VOID    BiasSupV        (REAL * const pr,
                         const BIASINTERVAL * const pA,
                         const INT dim);

VOID    BiasNegV        (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pA,
                         const INT dim);

VOID    BiasMidV        (REAL * const pr,
                         const BIASINTERVAL * const pA,
                         const INT dim);
VOID    BiasMidRadV     (REAL * const pm,
                         REAL * const pr,
                         const BIASINTERVAL * const pA,
                         const INT dim);
VOID    BiasDiamV       (REAL * const pr,
                         const BIASINTERVAL * const pA,
                         const INT dim);

VOID    BiasAbsV        (REAL * const pr,
                         const BIASINTERVAL * const pA,
                         const INT dim);

INT     BiasIntersectionV (BIASINTERVAL * const pR,
                           const BIASINTERVAL * const pA,
                           const BIASINTERVAL * const pB,
                           const INT dim);

VOID    BiasHullVR      (BIASINTERVAL * const pR,
                         const REAL * const pa,
                         const INT dim);
VOID    BiasHullVRVR    (BIASINTERVAL * const pR,
                         const REAL * const pa,
                         const REAL * const pb,
                         const INT dim);
VOID    BiasHullVRVI    (BIASINTERVAL * const pR,
                         const REAL * const pa,
                         const BIASINTERVAL * const pB,
                         const INT dim);
VOID    BiasHullVIVI    (BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB,
                         const INT dim);

INT     BiasInVR        (const REAL * const pa,
                         const BIASINTERVAL * const pB,
                         const INT dim);
INT     BiasInVI        (const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB,
                         const INT dim);

INT     BiasInInteriorVR(const REAL * const pa,
                         const BIASINTERVAL * const pB,
                         const INT dim);
INT     BiasInInteriorVI(const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB,
                         const INT dim);

INT     BiasIsEqualV    (const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB,
                         const INT dim);

VOID    BiasSetToZeroV  (BIASINTERVAL * const pR,
                         const INT dim);
VOID    BiasSetToZeroVs (BIASINTERVAL * const pR,
                         const INT dim,
                         const INT step);

#define BiasAddVIVR(pR, pA, pb, d)   BiasAddVRVI (pR, pb, pA, d)
#define BiasMacVIVR(pR, pA, pb, d)   BiasMacVRVI (pR, pb, pA, d)
#define BiasHullVIVR(pR, pA, pb, d)  BiasHullVRVI (pR, pb, pA, d)

#if defined (__cplusplus)
}
#endif

#endif /* __BIAS1__ */
