/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines MIPS-Rounding (IRIX)
 * ----------------------------------------------------------
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
 * $Id: fpRound.s 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

		.globl BiasRoundUp
		.ent BiasRoundUp
BiasRoundUp:
	        cfc1 $10, $31
	        and $10, 0xfffffffc
	        or $10, 0x00000002
	        ctc1 $10, $31
		j $31
		.end BiasRoundUp

		.globl BiasRoundDown
		.ent BiasRoundDown
BiasRoundDown:
	        cfc1 $10, $31
	        and $10, 0xfffffffc
	        or $10, 0x00000003
	        ctc1 $10, $31
		j $31
		.end BiasRoundDown

		.globl BiasRoundNear
		.ent BiasRoundNear
BiasRoundNear:
	        cfc1 $10, $31
	        and $10, 0xfffffffc
	        ctc1 $10, $31
		j $31
		.end BiasRoundNear
