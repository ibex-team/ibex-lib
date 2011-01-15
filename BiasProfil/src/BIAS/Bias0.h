/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines Level 0
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
 * $Id: Bias0.h 560 2008-10-14 04:56:19Z keil $
 *
 ************************************************************************/

#ifndef __BIAS0__
#define __BIAS0__

#include <BIAS/BiasType.h>

#define __BIASINFSUP__
#define __BIASIEEE__

/************************************************************************
 *  Definition of Constants, Types, and Variables
 ************************************************************************/

extern REAL     BiasEpsilon;            /* min {x >= 0 : 1 + x > 1}     */
extern REAL     BiasEta;                /* min {x > 0}                  */
extern REAL     BiasNaN;                /* NaN                          */
extern REAL     BiasPosInf;             /* +oo                          */
extern REAL     BiasNegInf;             /* -oo                          */

/************************************************************************
 *  Function Prototypes
 ************************************************************************/
#if defined (__cplusplus)
extern "C" {
#endif

VOID    BiasRoundUp     (void);
VOID    BiasRoundDown   (void);
VOID    BiasRoundNear   (void);

VOID    BiasInit        (void);

REAL    BiasPredR       (const REAL         * const pa);
INT     BiasPredI       (      BIASINTERVAL * const pR,
                         const BIASINTERVAL * const pA);

REAL    BiasSuccR       (const REAL         * const pa);
VOID    BiasSuccI       (BIASINTERVAL       * const pR,
                         const BIASINTERVAL * const pA);

VOID    BiasAddRR       (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const REAL         * const pb);
VOID    BiasAddRI       (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const BIASINTERVAL * const pB);
VOID    BiasAddII       (BIASINTERVAL       * const pR,
			 const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB);

VOID    BiasSubRR       (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const REAL         * const pb);
VOID    BiasSubRI       (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const BIASINTERVAL * const pB);
VOID    BiasSubIR       (BIASINTERVAL       * const pR,
			 const BIASINTERVAL * const pA,
                         const REAL         * const pb);
VOID    BiasSubII       (BIASINTERVAL       * const pR,
			 const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB);

VOID    BiasMulRR       (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const REAL         * const pb);
VOID    BiasMulRI       (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const BIASINTERVAL * const pB);
VOID    BiasMulII       (BIASINTERVAL       * const pR,
			 const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB);

VOID    BiasDivRR       (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const REAL         * const pb);
VOID    BiasDivRI       (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const BIASINTERVAL * const pB);
VOID    BiasDivIR       (BIASINTERVAL       * const pR,
			 const BIASINTERVAL * const pA,
                         const REAL         * const pb);
VOID    BiasDivII       (BIASINTERVAL       * const pR,
			 const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB);

VOID    BiasMacRR       (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const REAL         * const pb);
VOID    BiasMacRI       (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const BIASINTERVAL * const pB);
VOID    BiasMacII       (BIASINTERVAL       * const pR,
			 const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB);

VOID    BiasNeg         (BIASINTERVAL       * const pR,
                         const BIASINTERVAL * const pA);

REAL    BiasMid         (const BIASINTERVAL * const pA);
VOID    BiasMidRad      (REAL               * const pm,
                         REAL               * const pr,
			 const BIASINTERVAL * const pA);
REAL    BiasDiam        (const BIASINTERVAL * const pA);

REAL    BiasAbs         (const BIASINTERVAL * const pA);

REAL    BiasDistRI      (const REAL         * const pa,
                         const BIASINTERVAL * const pB);
REAL    BiasDistII      (const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB);

INT     BiasIntersection(BIASINTERVAL       * const pR,
			 const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB);

VOID    BiasHullR       (BIASINTERVAL       * const pR,
                         const REAL         * const pa);
VOID    BiasHullRR      (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const REAL         * const pb);
VOID    BiasHullRI      (BIASINTERVAL       * const pR,
			 const REAL         * const pa,
                         const BIASINTERVAL * const pB);
VOID    BiasHullII      (BIASINTERVAL       * const pR,
			 const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB);

INT     BiasInR         (const REAL         * const pa,
                         const BIASINTERVAL * const pB);
INT     BiasInI         (const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB);

INT     BiasInInteriorR (const REAL         * const pa,
                         const BIASINTERVAL * const pB);
INT     BiasInInteriorI (const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB);

INT     BiasIsEqual     (const BIASINTERVAL * const pA,
                         const BIASINTERVAL * const pB);

#define BiasInf(pA)     ((pA)->inf)
#define BiasSup(pA)     ((pA)->sup)

#define BiasAddIR(pR, pA, pb)    BiasAddRI (pR, pb, pA)
#define BiasMulIR(pR, pA, pb)    BiasMulRI (pR, pb, pA)
#define BiasMacIR(pR, pA, pb)    BiasMacRI (pR, pb, pA)
#define BiasDistIR(pA, pb)       BiasDistRI (pb, pA)
#define BiasHullIR(pR, pA, pb)   BiasHullRI (pR, pb, pA)

#if defined (__cplusplus)
}
#endif

#endif /* __BIAS0__ */
