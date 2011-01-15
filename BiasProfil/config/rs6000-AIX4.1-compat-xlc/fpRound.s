#############################################################################
#
# Basic Interval Arithmetic Subroutines Rounding Control (RS/6000)
# ----------------------------------------------------------------
#
# Copyright (C) 1997 Olaf Knueppel
#
# This file is part of PROFIL/BIAS.
#
# PROFIL/BIAS is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
# USA.
#
# $Id: fpRound.s 478 2006-08-09 13:13:30Z keil $
#
#############################################################################

		.toc
		.csect	[PR]
		.align	2
		.globl	BiasRoundUp
		.globl	.BiasRoundUp
		.csect	BiasRoundUp[DS]
BiasRoundUp:
		.long 	.BiasRoundUp, TOC[tc0], 0
		.csect [PR]
.BiasRoundUp:
		mtfsfi	7,2
		br
LT..BiasRoundUp:
		.long 	0
		.byte 	0,0,32,64,0,0,0,0
		.long	LT..BiasRoundUp-.BiasRoundUp
		.short	11
		.byte	"BiasRoundUp"
		.align	2

		.globl	BiasRoundDown
		.globl	.BiasRoundDown
		.csect	BiasRoundDown[DS]
BiasRoundDown:
		.long	.BiasRoundDown, TOC[tc0], 0
		.csect 	[PR]
.BiasRoundDown:
		mtfsfi	7,3
		br
LT..BiasRoundDown:
		.long	0
		.byte	0,0,32,64,0,0,0,0
		.long	LT..BiasRoundDown-.BiasRoundDown
		.short	13
		.byte	"BiasRoundDown"
		.align	2

		.globl	BiasRoundNear
		.globl	.BiasRoundNear
		.csect	BiasRoundNear[DS]
BiasRoundNear:
		.long	.BiasRoundNear, TOC[tc0], 0
		.csect	[PR]
.BiasRoundNear:
		mtfsfi	7,0
		br
LT..BiasRoundNear:
		.long	0
		.byte	0,0,32,64,0,0,0,0
		.long	LT..BiasRoundNear-.BiasRoundNear
		.short	13
		.byte	"BiasRoundNear"
_section_.text:
		.csect	.data[RW]
		.long _section_.text
