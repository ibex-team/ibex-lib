#############################################################################
#
# Basic Interval Arithmetic Subroutines Rounding Control (80x86 Win32)
# --------------------------------------------------------------------
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

		.file		"fpRound.s"
		.version	"$Revision: 478 $"

		.text

		.align		4
		.globl		_BiasRoundUp
_BiasRoundUp:
		fstcw		_fpcw
		movw		_fpcw,%ax
		andb		$0xf3,%ah
		orb		$0x08,%ah
		movw		%ax,_fpcw
		fldcw		_fpcw
		ret
.LUp:

		.align		4
		.globl		_BiasRoundDown
_BiasRoundDown:
		fstcw		_fpcw
		movw		_fpcw,%ax
		andb		$0xf3,%ah
		orb		$0x04,%ah
		movw		%ax,_fpcw
		fldcw		_fpcw
		ret
.LDown:

		.align		4
		.globl		_BiasRoundNear
_BiasRoundNear:
		fstcw		_fpcw
		movw		_fpcw,%ax
		andb		$0xf3,%ah
		movw		%ax,_fpcw
		fldcw		_fpcw
		ret
.LNear:

		.lcomm		_fpcw,4

		.ident		"$Id: fpRound.s 478 2006-08-09 13:13:30Z keil $"
