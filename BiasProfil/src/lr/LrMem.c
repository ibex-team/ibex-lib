/************************************************************************
*  $Id:	LrMem.c			Langzahlarithmetik, Speicherverwaltung
*
*  Copyright (C) 1989, 1992 Dirk Husung
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
*  Version 1.0	10.10.89	Dirk Husung
*  Letzte Aenderung:		Thu 14-May-1992 17:28:55
************************************************************************/

#include "Portab.h"

#if defined (__ANSI__)
#include <string.h>
#else
#include <memory.h>
#include <malloc.h>
#endif

#include "Lr.h"
#include "DynMem.h"

/************************************************************************
* Implementierung globaler Funktionen
************************************************************************/

VOID FAR CALL LrInit (VOID)
 /***********************************************************************
  *  Funktion :	Initialisierung	bei Programmstart.
  *  Ergebnis :	---
  */
{
  static USHORT IsInitialized = FALSE;

  if (IsInitialized++) return;

  MemInit ();
} /* LrInit */

VOID FAR CALL LrReset (VOID)
 /***********************************************************************
  *  Funktion :	Reinitialisierung.
  *  Ergebnis :	---
  */
{
  MemReset ();
} /* LrReset */

LREAL FAR CALL LrNew (SIZE Digits, SIZE Size)
{
  PPVOID Memory;

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
  LrSetPosDigits ((LREAL) Memory, Digits);

  return (LREAL) Memory;
} /* LrNew */
