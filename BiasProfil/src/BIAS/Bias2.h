/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines Level 2
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
 * $Id: Bias2.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __BIAS2__
#define __BIAS2__

#include <BIAS/Bias1.h>

/* #define __BIAS2USEVECTOROPERATIONS__ */

/************************************************************************
 *  Function Prototypes
 ************************************************************************/

#if defined (__cplusplus)
extern "C" {
#endif

VOID    BiasMulRMR      (BIASINTERVAL *pR,
                         const REAL * const pa,
                         const REAL *pb,
			 const INT rows,
                         const INT cols);
VOID    BiasMulRMI      (BIASINTERVAL *pR,
                         const REAL * const pa,
                         const BIASINTERVAL *pB,
			 const INT rows,
                         const INT cols);
VOID    BiasMulIMR      (BIASINTERVAL *pR,
                         const BIASINTERVAL * const pA,
                         const REAL *pb,
			 const INT rows,
                         const INT cols);
VOID    BiasMulIMI      (BIASINTERVAL *pR,
                         const BIASINTERVAL * const pA,
                         const BIASINTERVAL *pB,
			 const INT rows,
                         const INT cols);

VOID    BiasDivMRR      (BIASINTERVAL *pR,
                         const REAL *pa,
                         const REAL * const pb,
			 const INT rows,
                         const INT cols);
VOID    BiasDivMRI      (BIASINTERVAL *pR,
                         const REAL *pa,
                         const BIASINTERVAL * const pB,
			 const INT rows,
                         const INT cols);
VOID    BiasDivMIR      (BIASINTERVAL *pR,
                         const BIASINTERVAL *pA,
                         const REAL * const pb,
			 const INT rows,
                         const INT cols);
VOID    BiasDivMII      (BIASINTERVAL *pR,
                         const BIASINTERVAL *pA,
                         const BIASINTERVAL * const pB,
			 const INT rows,
                         const INT cols);

VOID    BiasMulMRVR     (BIASINTERVAL *pR,
                         const REAL *pa,
                         const REAL * const pb,
			 const INT rows,
                         const INT cols);
VOID    BiasMulMRVI     (BIASINTERVAL *pR,
                         const REAL *pa,
                         const BIASINTERVAL * const pB,
			 const INT rows,
                         const INT cols);
VOID    BiasMulMIVR     (BIASINTERVAL *pR,
                         const BIASINTERVAL *pA,
                         const REAL *pb,
			 const INT rows,
                         const INT cols);
VOID    BiasMulMIVI     (BIASINTERVAL *pR,
                         const BIASINTERVAL *pA,
                         const BIASINTERVAL * const pB,
			 const INT rows,
                         const INT cols);

#if defined (__BIAS2USEVECTOROPERATIONS__)
#define BiasAddMRMR(pR, pa, pb, r, c)      BiasAddVRVR (pR, pa, pb, (r)*(c))
#define BiasAddMRMI(pR, pa, pB, r, c)      BiasAddVRVI (pR, pa, pB, (r)*(c))
#define BiasAddMIMR(pR, pA, pb, r, c)      BiasAddVIVR (pR, pA, pb, (r)*(c))
#define BiasAddMIMI(pR, pA, pB, r, c)      BiasAddVIVI (pR, pA, pB, (r)*(c))
#define BiasSubMRMR(pR, pa, pb, r, c)      BiasSubVRVR (pR, pa, pb, (r)*(c))
#define BiasSubMRMI(pR, pa, pB, r, c)      BiasSubVRVI (pR, pa, pB, (r)*(c))
#define BiasSubMIMR(pR, pA, pb, r, c)      BiasSubVIVR (pR, pA, pb, (r)*(c))
#define BiasSubMIMI(pR, pA, pB, r, c)      BiasSubVIVI (pR, pA, pB, (r)*(c))
#else

VOID    BiasAddMRMR     (BIASINTERVAL *pR,
                         const REAL *pa,
                         const REAL *pb,
			 const INT rows,
                         const INT cols);
VOID    BiasAddMRMI     (BIASINTERVAL *pR,
                         const REAL *pa,
                         const BIASINTERVAL *pB,
			 const INT rows,
                         const INT cols);
#define BiasAddMIMR(pR, pA, pb, r, c)      BiasAddMRMI (pR, pb, pA, r, c)
VOID    BiasAddMIMI     (BIASINTERVAL *pR,
                         const BIASINTERVAL *pA,
                         const BIASINTERVAL *pB,
			 const INT rows,
                         const INT cols);

VOID    BiasSubMRMR     (BIASINTERVAL *pR,
                         const REAL *pa,
                         const REAL *pb,
			 const INT rows,
                         const INT cols);
VOID    BiasSubMRMI     (BIASINTERVAL *pR,
                         const REAL *pa,
                         const BIASINTERVAL *pB,
			 const INT rows,
                         const INT cols);
VOID    BiasSubMIMR     (BIASINTERVAL *pR,
                         const BIASINTERVAL *pA,
                         const REAL *pb,
			 const INT rows,
                         const INT cols);
VOID    BiasSubMIMI     (BIASINTERVAL *pR,
                         const BIASINTERVAL *pA,
                         const BIASINTERVAL *pB,
			 const INT rows,
                         const INT cols);
#endif

VOID    BiasMulMRMR     (BIASINTERVAL *pR,
                         const REAL *pa,
                         const REAL *pb,
			 const INT arows,
                         const INT acols,
                         const INT bcols);
VOID    BiasMulMRMI     (BIASINTERVAL *pR,
                         const REAL *pa,
                         const BIASINTERVAL * const pB,
			 const INT arows,
                         const INT acols,
                         const INT Bcols);
VOID    BiasMulMIMR     (BIASINTERVAL *pR,
                         const BIASINTERVAL * const pA,
                         const REAL *pb,
			 const INT Arows,
                         const INT Acols,
                         const INT bcols);
VOID    BiasMulMIMI     (BIASINTERVAL *pR,
                         const BIASINTERVAL *pA,
                         const BIASINTERVAL * const pB,
			 const INT Arows,
                         const INT Acols,
                         const INT Bcols);

#define BiasPredMR(pr, pa, r, c)           BiasPredVR (pr, pa, (r)*(c))
#define BiasPredMI(pR, pA, r, c)           BiasPredVI (pR, pA, (r)*(c))
#define BiasSuccMR(pr, pa, r, c)           BiasSuccVR (pr, pa, (r)*(c))
#define BiasSuccMI(pR, pA, r, c)           BiasSuccVI (pR, pA, (r)*(c))

#define BiasInfM(pr, pA, r, c)             BiasInfV (pr, pA, (r)*(c))
#define BiasSupM(pr, pA, r, c)             BiasSupV (pr, pA, (r)*(c))

#define BiasNegM(pR, pA, r, c)             BiasNegV (pR, pA, (r)*(c))

#define BiasMidM(pr, pA, r, c)             BiasMidV (pr, pA, (r)*(c))
#define BiasMidRadM(pm, pr, pA, r, c)      BiasMidRadV (pm, pr, pA, (r)*(c))

#define BiasDiamM(pr, pA, r, c)            BiasDiamV (pr, pA, (r)*(c))

#define BiasAbsM(pr, pA, r, c)             BiasAbsV (pr, pA, (r)*(c))

#define BiasIntersectionM(pR, pA, pB, r, c)   BiasIntersectionV (pR, pA, pB, (r)*(c))

#define BiasHullMR(pR, pa, r, c)           BiasHullVR (pR, pa, (r)*(c))
#define BiasHullMRMR(pR, pa, pb, r, c)     BiasHullVRVR (pR, pa, pb, (r)*(c))
#define BiasHullMRMI(pR, pa, pB, r, c)     BiasHullVRVI (pR, pa, pB, (r)*(c))
#define BiasHullMIMR(pR, pA, pb, r, c)     BiasHullVIVR (pR, pA, pb, (r)*(c))
#define BiasHullMIMI(pR, pA, pB, r, c)     BiasHullVIVI (pR, pA, pB, (r)*(c))

#define BiasInMR(pa, pB, r, c)             BiasInVR (pa, pB, (r)*(c))
#define BiasInMI(pA, pB, r, c)             BiasInVI (pA, pB, (r)*(c))

#define BiasInInteriorMR(pa, pB, r, c)     BiasInInteriorVR (pa, pB, (r)*(c))
#define BiasInInteriorMI(pA, pB, r, c)     BiasInInteriorVI (pA, pB, (r)*(c))

#define BiasIsEqualM(pA, pB, r, c)         BiasIsEqualV (pA, pB, (r)*(c))

#define BiasSetToZeroM(pR, r, c)           BiasSetToZeroV (pR, (r)*(c))

#if defined (__cplusplus)
}
#endif

#endif /* __BIAS2__ */
