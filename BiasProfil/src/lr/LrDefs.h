/************************************************************************
 *
 * Multiple Precision Arithmetic Basic Definitions (Header)
 * --------------------------------------------------------
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
 * $Id: LrDefs.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __LRDEFS__
#define __LRDEFS__

#include <stdlib.h>

#define NEAR
#define FAR

#define ARGS(parameters)	parameters
#define NOARGS			VOID

#define TRUE		1
#define FALSE		0

#define SUCCESS 	0
#define FAILURE 	(-1)

#define VOID		void

#define INT		int
#define UNSIGNED	unsigned

typedef char		CHAR;		/*  8 bits			*/
typedef unsigned char	UCHAR;		/*  8 bits			*/
typedef short		SHORT;		/* 16 bits			*/
typedef unsigned short	USHORT;		/* 16 bits			*/
typedef unsigned short	CARDINAL;	/* 16 bits			*/
typedef int		LONG;		/* 32 bits			*/
typedef unsigned int	ULONG;		/* 32 bits			*/

typedef unsigned char	BOOL;
typedef char *		STRING;

typedef VOID	FAR *	PVOID;
typedef PVOID	FAR *	PPVOID;
typedef CHAR	FAR *	PCHAR;
typedef UCHAR	FAR *	PUCHAR;
typedef SHORT	FAR *	PSHORT;
typedef USHORT	FAR *	PUSHORT;
typedef INT	FAR *	PINT;
typedef LONG	FAR *	PLONG;
typedef ULONG	FAR *	PULONG;

#define SIZE		size_t
typedef SIZE	FAR *	PSIZE;

#define FLOAT		float
#define DOUBLE		double
typedef FLOAT	FAR *	PFLOAT;
typedef DOUBLE	FAR *	PDOUBLE;

typedef PCHAR	FAR *	PPCHAR;
typedef PUCHAR	FAR *	PPUCHAR;
typedef PSHORT	FAR *	PPSHORT;
typedef PUSHORT FAR *	PPUSHORT;
typedef PLONG	FAR *	PPLONG;
typedef PULONG	FAR *	PPULONG;

#endif /* __LRDEFS__ */
