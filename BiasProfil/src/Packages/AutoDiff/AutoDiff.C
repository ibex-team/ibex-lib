/************************************************************************
 *
 * Implementation of Real Automatic Differentiation Type
 * -----------------------------------------------------
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
 * $Id: AutoDiff.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: AutoDiff.C 478 2006-08-09 13:13:30Z keil $";

#include <AutoDiff/AutoDiff.h>

#define AUTODIFF_TYPE          AUTODIFF
#define AUTODIFF_SIMPLE_TYPE   REAL
#define AUTODIFF_VECTOR_TYPE   VECTOR
#define AUTODIFF_MATRIX_TYPE   MATRIX
#include <AutoDiff/AutoDiff.Cgen>
#undef AUTODIFF_MATRIX_TYPE
#undef AUTODIFF_VECTOR_TYPE
#undef AUTODIFF_SIMPLE_TYPE
#undef AUTODIFF_TYPE
