/************************************************************************
 *
 * General Configuration Parameters
 * --------------------------------
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
 * $Id: Configuration.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __CONFIG__
#define __CONFIG__

/************************************************************************
 *  If the following macro is defined, a special strategy avoiding
 *  copies of temporary structures is used. Due to the fact, that C++
 *  only supports a single copy constructor, some restrictions occur when
 *  using this strategy:
 *    - expressions must not be used as reference parameters
 *    - expressions or single variables must not be used as value parameters
 *    - initialization without using the assignment operator must not
 *      be used.
 *  The above restrictions are valid for all structured types as VECTOR,
 *  MATRIX, INTERVAL_VECTOR, ...
 *  If any of the above cases can't be avoided, a MakePermanent(...)
 *  command must be used before any use of the appropriate variable.
 *  For testing purposes, the macro should be left undefined.
 ************************************************************************/

#undef __DONTCOPY__

/************************************************************************
 *  If the following macro is defined, an index check is performed prior
 *  any vector or matrix element access. This results in reduced execution
 *  speed and therefore should only be used in the developement phase
 * of the program.
 *************************************************************************/

#undef __INDEXCHECK__

/************************************************************************
 *  If the following macro is defined, the keyword 'const' is used.
 *  If you have an older compiler which gets into trouble with this,
 *  replace the definition below by:
 *  #undef __USECONST__
 *************************************************************************/

#define __USECONST__

/************************************************************************
 *  Include some general definitions
 *************************************************************************/

#define __SIMPLEINOUT__

#include <Definitions.h>

#endif /* __CONFIG__ */
