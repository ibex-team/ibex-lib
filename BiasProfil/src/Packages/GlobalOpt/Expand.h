/************************************************************************
 *
 * Definition of Expansion Strategy
 * --------------------------------
 *
 * Copyright (C) 1995, 1997 Olaf Knueppel
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
 * $Id: Expand.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __EXPAND__
#define __EXPAND__

#include <IntervalMatrix.h>
#include <Boolean.h>
#include <NLF/NLF.h>

BOOL ExpansionStep (FUNCTION & f, CONST INTERVAL_VECTOR & MaxRange,
                    CONST VECTOR & x, INTERVAL_VECTOR & Enclosure, INTERVAL_VECTOR & Expansion);
// Expansion Strategy
// Input:  x: approximation of a stationary point of f
// Output: Enclosure: narrow enclosure of a stationary point of f
//         Expansion: expansion around a  stationary point of f
// Return: TRUE, if enclosure was possible

#endif /* __EXPAND__ */
