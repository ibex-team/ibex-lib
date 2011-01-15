/************************************************************************
 *
 * Multiple Precision Arithmetic Basic Definitions (Implementation)
 * ----------------------------------------------------------------
 *
 * Copyright (C) 1998 Olaf Knueppel
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
 * $Id: Portab.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __PORTAB__
#define __PORTAB__

#include <lr/LrDefs.h>

#ifndef __ANSI__
#define __ANSI__
#endif
#define __32BIT__

#if defined (__LITTLEENDIAN__)

#define LO		0
#define HI		1

#define D0OF2		0
#define D1OF2		1

#define D0OF4		0
#define D1OF4		1
#define D2OF4		2
#define D3OF4		3

#define D0OF8		0
#define D1OF8		1
#define D2OF8		2
#define D3OF8		3
#define D4OF8		4
#define D5OF8		5
#define D6OF8		6
#define D7OF8		7

#elif defined (__BIGENDIAN__)

#define LO		1
#define HI		0

#define D0OF2		1
#define D1OF2		0

#define D0OF4		3
#define D1OF4		2
#define D2OF4		1
#define D3OF4		0

#define D0OF8		7
#define D1OF8		6
#define D2OF8		5
#define D3OF8		4
#define D4OF8		3
#define D5OF8		2
#define D6OF8		1
#define D7OF8		0

#endif /* !__LITTLEENDIAN__ && !__BIGENDIAN__ */

#define MoveFarLeft(pDest, pSrc, Count, Type) \
	memmove ((pDest), (pSrc), (Count) * sizeof (Type))
#define MoveFarRight(pDest, pSrc, Count, Type) \
	memmove ((pDest), (pSrc), (Count) * sizeof (Type))
#define	MoveLeft(pType,	Count, Type) \
	memmove	((pType), (pType)+1, (Count) * sizeof (Type))
#define	MoveRight(pType, Count,	Type) \
	memmove	((pType)+1, (pType), (Count) * sizeof (Type))

#define raise		goto
#define loop		for (;;)
#define min(a,b)        (((a) < (b)) ? (a) : (b))
#define max(a,b)        (((a) > (b)) ? (a) : (b))
#define dimof(array)	(sizeof(array) / sizeof(*(array)))

#if defined (__BORLANDC__)
#define _asm		asm
#endif

#endif /* !__PORTAB__ */
