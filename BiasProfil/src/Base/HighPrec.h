/************************************************************************
 *
 * Definition of operations with exact scalar product
 * --------------------------------------------------
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
 * $Id: HighPrec.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __HIGHPREC__
#define __HIGHPREC__

#include <IntervalMatrix.h>
#include <lr/Li.h>

REAL   HighPrecMul (CONST VECTOR &, CONST VECTOR &);
VECTOR HighPrecMul (CONST MATRIX &, CONST VECTOR &);
MATRIX HighPrecMul (CONST MATRIX &, CONST MATRIX &);

INTERVAL HighPrecMulBounds (CONST VECTOR &         , CONST VECTOR &         );
INTERVAL HighPrecMul       (CONST VECTOR &         , CONST INTERVAL_VECTOR &);
INTERVAL HighPrecMul       (CONST INTERVAL_VECTOR &, CONST VECTOR &         );
INTERVAL HighPrecMul       (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);

INTERVAL_VECTOR HighPrecMulBounds (CONST MATRIX &         , CONST VECTOR &         );
INTERVAL_VECTOR HighPrecMul       (CONST MATRIX &         , CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR HighPrecMul       (CONST INTERVAL_MATRIX &, CONST VECTOR &         );
INTERVAL_VECTOR HighPrecMul       (CONST INTERVAL_MATRIX &, CONST INTERVAL_VECTOR &);

INTERVAL_MATRIX HighPrecMulBounds (CONST MATRIX &         , CONST MATRIX &         );
INTERVAL_MATRIX HighPrecMul       (CONST MATRIX &         , CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX HighPrecMul       (CONST INTERVAL_MATRIX &, CONST MATRIX &         );
INTERVAL_MATRIX HighPrecMul       (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);

INTERVAL_MATRIX IdMinusHighPrecMul (CONST MATRIX &         , CONST MATRIX &         );
INTERVAL_MATRIX IdMinusHighPrecMul (CONST MATRIX &         , CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX IdMinusHighPrecMul (CONST INTERVAL_MATRIX &, CONST MATRIX &         );
INTERVAL_MATRIX IdMinusHighPrecMul (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);

// the following functions are for internal use only!
LREAL  HighPrecScalpLr (CONST REAL *, CONST REAL *, INT, INT);
LIREAL HighPrecScalpLi (CONST REAL *, CONST REAL *, INT, INT);
LIREAL HighPrecScalpLi (CONST REAL *, CONST BIASINTERVAL *, INT, INT);
LIREAL HighPrecScalpLi (CONST BIASINTERVAL *, CONST REAL *, INT, INT);
LIREAL HighPrecScalpLi (CONST BIASINTERVAL *, CONST BIASINTERVAL *, INT, INT);

#endif /* __HIGHPREC__ */
