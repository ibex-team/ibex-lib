/************************************************************************
 *
 * Definition of REAL test matrices (C routines)
 * ---------------------------------------------
 *
 * Copyright (C) 1993 Thomas Simenec
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
 * $Id: BaseTestMatrices.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __BASETESTMATRICES__
#define __BASETESTMATRICES__

#include <Configuration.h>

#if defined (__cplusplus)
extern "C" {
#endif

VOID BaseLietzke (REAL *, INT);

VOID BaseIntPascal (REAL *, INT);

VOID BasePascal (REAL *, INT, REAL);

VOID BaseHilbert (REAL *, INT);

INT BaseExactHilbert (REAL *, INT);

VOID BaseInverseHilbert (REAL *, INT);

VOID BaseLotkin (REAL *, INT);

VOID BaseWestlake (REAL *, INT);

VOID BaseNewman (REAL *, INT);

VOID BaseFrank (REAL *, INT);

VOID BaseBoothroydMax (REAL *, INT);

VOID BaseHadamard (REAL *, INT);

VOID BaseBinomial (REAL *, INT);

VOID BaseAergerter (REAL *, INT);

VOID BaseTodd (REAL *, INT);

VOID BaseMilnes (REAL *, REAL *, INT, INT);

VOID BaseCombinatorial (REAL *, REAL, REAL, INT, INT);

VOID BaseCauchy (REAL *, REAL *, REAL *, INT, INT);

VOID BaseVandermonde (REAL *, REAL *, INT);

VOID BaseBoothroyd (REAL *, REAL *, REAL *, INT);

VOID BaseInverseBoothroyd (REAL *, REAL *, REAL *, INT);

VOID BaseRandomM_Matrix (REAL *, REAL *, REAL *, INT);

VOID BaseRandomPD (REAL *, REAL *, REAL *, REAL *, REAL, REAL, INT, INT);

VOID BaseRandomSPD (REAL *, REAL *, REAL *, REAL, REAL, INT, INT);

VOID BaseRandomToeplitz (REAL *, REAL *, INT);

VOID BaseRandomMatrix (REAL *, INT);

VOID BaseRectRandomMatrix (REAL *, INT, INT);

VOID BaseRandomSymmetric (REAL *, INT);

VOID BaseRandomPersymmetric (REAL *, INT);

#if defined (__cplusplus)
}
#endif

#endif /* __BASETESTMATRICES__ */
