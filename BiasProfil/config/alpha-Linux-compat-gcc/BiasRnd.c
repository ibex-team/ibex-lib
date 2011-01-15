/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines DEC-alpha-Rounding (DEC Alpha)
 * --------------------------------------------------------------------
 *
 * Copyright (C) 1997 Olaf Knueppel
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
 * $Id: BiasRnd.c 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <BiasRnd.h>

void BiasRoundInit (void) {}

#define RND_MASK 0x0C00000000000000
#define RND_DOWN 0x0400000000000000
#define RND_NEAR 0x0800000000000000
#define RND_UP   0x0C00000000000000

void BiasRoundNear (void)
{
  register long int fpcontrol;

  asm volatile ("mf_fpcr %0" : "=f" (fpcontrol));
  fpcontrol &= ~RND_MASK;
  fpcontrol |= RND_NEAR;
  asm volatile ("mt_fpcr %0" : : "f" (fpcontrol));
}

void BiasRoundUp (void)
{
  register long int fpcontrol;

  asm volatile ("mf_fpcr %0" : "=f" (fpcontrol));
  fpcontrol &= ~RND_MASK;
  fpcontrol |= RND_UP;
  asm volatile ("mt_fpcr %0" : : "f" (fpcontrol));
}

void BiasRoundDown (void)
{
  register long int fpcontrol;

  asm volatile ("mf_fpcr %0" : "=f" (fpcontrol));
  fpcontrol &= ~RND_MASK;
  fpcontrol |= RND_DOWN;
  asm volatile ("mt_fpcr %0" : : "f" (fpcontrol));
}
