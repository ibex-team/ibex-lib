/************************************************************************
 *
 * Definition of commonly used real operations
 * -------------------------------------------
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
 * $Id: RealOp.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __REALOP__
#define __REALOP__

#include <Configuration.h>

#if defined (__cplusplus)
extern "C" {
#endif

VOID RealOpNeg (REAL *pr, CONST REAL *pa, INT dim);

VOID RealOpAdd (REAL *pr, CONST REAL *pa, CONST REAL *pb, INT dim);
VOID RealOpSub (REAL *pr, CONST REAL *pa, CONST REAL *pb, INT dim);
VOID RealOpMul (REAL *pr, REAL  a,        CONST REAL *pb, INT dim);
VOID RealOpDiv (REAL *pr, CONST REAL *pa, REAL  b,        INT dim);

VOID RealOpAddTo   (REAL *pr, CONST REAL *pa, INT dim);
VOID RealOpSubFrom (REAL *pr, CONST REAL *pa, INT dim);
VOID RealOpMulWith (REAL *pr, REAL  a,        INT dim);
VOID RealOpDivBy   (REAL *pr, REAL  a,        INT dim);

REAL RealOpScalp (CONST REAL *pa, CONST REAL *pb, INT dim);

VOID RealOpVecMul (REAL *pr, CONST REAL *pa, CONST REAL *pb,
                   INT rows, INT cols);
VOID RealOpMatMul (REAL *pr, CONST REAL *pa, CONST REAL *pb,
		   INT arows, INT acols, INT bcols);

INT RealOpLessThan     (CONST REAL *pa, CONST REAL *pb, INT dim);
INT RealOpLessEqual    (CONST REAL *pa, CONST REAL *pb, INT dim);
INT RealOpGreaterThan  (CONST REAL *pa, CONST REAL *pb, INT dim);
INT RealOpGreaterEqual (CONST REAL *pa, CONST REAL *pb, INT dim);

#if defined (__cplusplus)
}
#endif

#endif /* __REALOP__ */
