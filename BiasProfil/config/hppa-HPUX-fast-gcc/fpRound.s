#############################################################################
#
# Basic Interval Arithmetic Subroutines Rounding Control (HPPA)
# -------------------------------------------------------------
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

	.VERSION	"Rounding Mode Switch for HPPA"
	
        .SPACE $PRIVATE$
        .SUBSPA $DATA$,QUAD=1,ALIGN=8,ACCESS=31
        .SUBSPA $BSS$,QUAD=1,ALIGN=8,ACCESS=31,ZERO,SORT=82
        .SPACE $TEXT$
        .SUBSPA $LIT$,QUAD=0,ALIGN=8,ACCESS=44
        .SUBSPA $CODE$,QUAD=0,ALIGN=8,ACCESS=44,CODE_ONLY
        .IMPORT $global$,DATA
        .IMPORT $$dyncall,MILLICODE

        .SPACE $TEXT$
        .SUBSPA $LIT$

CwRoundDown
	.ALIGN	4
	.WORD	0x81e00
CwRoundUp
	.ALIGN	4
	.WORD	0x81c00
CwRoundNear
	.ALIGN	4
	.WORD	0x81800

        .SPACE $TEXT$
        .SUBSPA $CODE$

        .align 4
        .EXPORT BiasRoundDown,ENTRY,PRIV_LEV=3
BiasRoundDown
        .PROC
        .CALLINFO FRAME=0,NO_CALLS
        .ENTRY
	LDIL	LR'CwRoundDown,%r23
	LDO	RR'CwRoundDown+16(%r23),%r31
	BV	%r0(%r2)
	.EXIT
	FLDWS	-16(0,%r31),%fr0L
        .PROCEND

        .align 4
        .EXPORT BiasRoundUp,ENTRY,PRIV_LEV=3
BiasRoundUp
        .PROC
        .CALLINFO FRAME=0,NO_CALLS
        .ENTRY
	LDIL	LR'CwRoundUp,%r23
	LDO	RR'CwRoundUp+16(%r23),%r31
	BV	%r0(%r2)
	.EXIT
	FLDWS	-16(0,%r31),%fr0L
        .PROCEND

        .align 4
        .EXPORT BiasRoundNear,ENTRY,PRIV_LEV=3
BiasRoundNear
        .PROC
        .CALLINFO FRAME=0,NO_CALLS
        .ENTRY
	LDIL	LR'CwRoundNear,%r23
	LDO	RR'CwRoundNear+16(%r23),%r31
	BV	%r0(%r2)
	.EXIT
	FLDWS	-16(0,%r31),%fr0L
        .PROCEND
