/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines Sparc-Rounding (Solaris2.x)
 * -----------------------------------------------------------------
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

		.data
		.align	4
_FSR:
		.word	0

_CwRoundUp	= 0x80000000
_CwRoundDown	= 0xC0000000
_CwRoundNear	= 0x00000000
_CwMask		= 0xC0000000

		.text

		.align	4
		.global	BiasRoundUp
		.proc	020
BiasRoundUp:
		sethi	%hi(_FSR),%g2
		st	%fsr,[%g2+%lo(_FSR)]
		ld	[%g2+%lo(_FSR)],%g4
		sethi	%hi(_CwMask),%g3
		andn	%g4,%g3,%g4
		sethi	%hi(_CwRoundUp),%g3
		or	%g4,%g3,%g4
		st	%g4,[%g2+%lo(_FSR)]
		retl
		ld	[%g2+%lo(_FSR)],%fsr

		.align	4
		.global	BiasRoundDown
		.proc	020
BiasRoundDown:
		sethi	%hi(_FSR),%g2
		st	%fsr,[%g2+%lo(_FSR)]
		ld	[%g2+%lo(_FSR)],%g4
		sethi	%hi(_CwMask),%g3
		andn	%g4,%g3,%g4
		sethi	%hi(_CwRoundDown),%g3
		or	%g4,%g3,%g4
		st	%g4,[%g2+%lo(_FSR)]
		retl
		ld	[%g2+%lo(_FSR)],%fsr

		.align	4
		.global	BiasRoundNear
		.proc	020
BiasRoundNear:
		sethi	%hi(_FSR),%g2
		st	%fsr,[%g2+%lo(_FSR)]
		ld	[%g2+%lo(_FSR)],%g4
		sethi	%hi(_CwMask),%g3
		andn	%g4,%g3,%g4
		sethi	%hi(_CwRoundNear),%g3
		or	%g4,%g3,%g4
		st	%g4,[%g2+%lo(_FSR)]
		retl
		ld	[%g2+%lo(_FSR)],%fsr
