/*****************************************************************************
*  $Id: LiMem.c 		Intervall-Langzahlarithmetik
*				Speicherverwaltung
*
*  Copyright (C) 1990, 1992 Olaf Knueppel
*
*  This file is part of PROFIL/BIAS.
* 
*  PROFIL/BIAS is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
* 
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
* 
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
*  USA.
* 
*  Version 1.0	25.07.90	Olaf Knueppel
*  Letzte Aenderung:		Sun 31-May-1992 17:26:02
*****************************************************************************/

#include "Portab.h"

#if defined (__ANSI__)
#include <string.h>
#else
#include <memory.h>
#include <malloc.h>
#endif

#include "Li.h"
#include "DynMem.h"

LIREAL FAR CALL LiNew (USHORT Prec, register USHORT Size)
 /***********************************************************************
  *  Funktion : Speicherallokierung fuer eine Intervall-Langzahl.
  *  Eingaben : Prec	: Genauigkeit.
  *		Size	: Speicherbedarf in Bytes.
  *  Ergebnis : Zeiger auf Intervall-Langzahl.
  *  Info     : Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */
{
  register PPVOID Memory;

  if (Size <= MEM_MAXSIZE
      && (Memory = (PPVOID) *((PPVOID) ((PCHAR) MemStore + Size))) != NULL) {
    *((PPVOID) ((PCHAR) MemStore + Size)) = *Memory;
  }
  else if ((Memory = (PPVOID) malloc (Size)) == NULL) {
    INT ErrorClass = 0;

    do {
      (*MemErrorHandler) (ErrorClass++);
    } while ((Memory = (PPVOID) malloc (Size)) == NULL);
  }
  LiInfo ((LIREAL) Memory) = Prec;

  return (LIREAL) Memory;
} /* LiNew */
