/************************************************************************
 *
 * Implementation of machine dependant constants
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
 * $Id: Constants.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static char rcs_id[] = "$Id: Constants.C 478 2006-08-09 13:13:30Z keil $";

#include <Constants.h>
#include <BIAS/Bias0.h>

REAL Machine::Epsilon            = 0.0;
REAL Machine::MinPositive        = 0.0;
REAL Machine::PosInfinity        = 0.0;
REAL Machine::NegInfinity        = 0.0;
REAL Machine::NaN                = 0.0;

Machine MachineConstants; // constructor is called automatically

Machine::Machine ()
{
  BiasInit ();
  Machine::Epsilon            = BiasEpsilon;
  Machine::MinPositive        = BiasEta;
  Machine::PosInfinity        = BiasPosInf;
  Machine::NegInfinity        = BiasNegInf;
  Machine::NaN                = BiasNaN;
}

// The following routine is only used for reference in Interval.C and Vector.C
// in order to force the loader to bind this file into the executable code.

VOID RegisterConstants () { }
