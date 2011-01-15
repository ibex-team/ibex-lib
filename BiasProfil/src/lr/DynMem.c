/************************************************************************
*  $Id:DynMem.c			Speicherallokierung /-freigabe
*
*  Copyright (C) 1990, 1993 Dirk Husung
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
*  Version 1.0	05.06.90	Dirk Husung
*  Version 1.1	05.12.90	Dirk Husung
*	ueberarbeitet
*  Letzte Aenderung:		Sun 28-Feb-1993 09:19:06
*************************************************************************/

#include <stdio.h>

#include "Portab.h"

#if defined (__STDC__) || defined (__cplusplus)
#include <string.h>
#else
#include <memory.h>
#include <malloc.h>
#endif

#include "DynMem.h"

/************************************************************************
*  Variablen-Definitionen
*************************************************************************/

PVOID MemStore [(MEM_MAXSIZE >> 2) + 1];

PMEM_ERRORHANDLER MemErrorHandler;

#if defined (__MEMTEST__)
ULONG	ulMemUsed;
#endif

/************************************************************************
*  Implementierung der Funktionen
*************************************************************************/

static VOID FAR MemError (INT ErrorClass)
  /**********************************************************************
  *  Funktion : Standard-Fehlerbehandlung, falls Speicher erschoepft:
  *		  ErrorCode = 0: Aufruf von MemRelease().
  *		  ErrorCode > 0: kurze Meldung,
  *				 Programmabbruch mit Exit-Code 1.
  */
{
  switch (ErrorClass) {
    case 0:
	MemRelease ();
	break;
    default:
	fprintf (stderr, "Zu wenig Speicher.\n");
	exit (1);
  } /* switch */
} /* MemError */

VOID MemInit (VOID)
  /**********************************************************************
  *  Funktion : Initialisierung bei Programmstart.
  *  Ergebnis : ---
  */
{
  static BOOL IsInitialized = FALSE;

  if (IsInitialized == FALSE) {
    MemErrorHandler = MemError;
    memset ((PCHAR) MemStore, 0, sizeof (MemStore));
#if defined (__MEMTEST__)
    ulMemUsed = 0;
#endif
    IsInitialized = TRUE;
  }
} /* MemInit */

VOID MemReset (VOID)
  /**********************************************************************
  *  Funktion : Reinitialisierung.
  */
{
  MemErrorHandler = MemError;
  MemRelease ();
#if defined (__MEMTEST__)
  ulMemUsed = 0;
#endif
} /* MemReset */

VOID MemRelease (VOID)
  /**********************************************************************
  *  Funktion : Gibt den internen Speicher-Puffer frei.
  */
{
  PVOID  Memory, LastMemory;
  USHORT Size;

  for (Size = 0; Size <= MEM_MAXSIZE; Size += 4) {
    Memory = *((PPVOID) ((PCHAR) MemStore + Size));
    while (Memory) {
      Memory = *((PPVOID) (LastMemory = Memory));
      free ((PCHAR) LastMemory);
    }
  }
  memset ((PCHAR) MemStore, 0, sizeof (MemStore));
} /* MemRelease */

PVOID _MemNew (SIZE Size)
  /**********************************************************************
  *  Funktion : Allokiert Size Bytes Speicher.
  *  Annahmen : Size ist ohne Rest durch 4 teilbar.
  *  Eingaben : Size	: Groesse des Speicherbereichs.
  *  Ergebnis : Zeiger auf Speicherbereich.
  *  Info     : Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */
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
#if defined (__MEMTEST__)
  ulMemUsed += Size;
#endif
  return (PVOID) Memory;
} /* _MemNew */

VOID _MemDelete (PVOID Memory, SIZE Size)
  /**********************************************************************
  *  Funktion : Gibt einen mit _MemNew() allokierten Speicherbereich frei.
  *  Annahmen : Size ist ohne Rest durch 4 teilbar.
  *  Eingaben : Memory	: Zeiger auf freizugebenden Speicherbereich.
  *		Size	: Groesse des Speicherbereichs.
  */
{
  if (Size <= MEM_MAXSIZE) {
    *((PPVOID) Memory) = *((PPVOID) ((PCHAR) MemStore + Size));
    *((PPVOID) ((PCHAR) MemStore + Size)) = Memory;
  }
  else
    free ((PCHAR) Memory);
#if defined (__MEMTEST__)
  ulMemUsed -= Size;
#endif
} /* _MemDelete */

PVOID _MemResize (PVOID Memory, SIZE OldSize, SIZE NewSize)
  /**********************************************************************
  *  Funktion : Aendert die Groesse eines mit _MemNew() allokierten
  *		Speicherbereichs.
  *  Annahmen : OldSize und NewSize sind ohne Rest durch 4 teilbar.
  *  Eingaben : Memory	: Zeiger auf zu aendernden Speicherbereich.
  *		OldSize : urspr. Groesse des Speicherbereichs.
  *		NewSize : neue Groesse des Speicherbereichs.
  *  Ergebnis : Zeiger auf Speicherbereich.
  *  Info     : Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */
{
  PVOID NewMemory;

  if (OldSize <= MEM_MAXSIZE || NewSize <= MEM_MAXSIZE) {
    NewMemory = (PVOID) _MemNew (NewSize);
    memcpy (NewMemory, Memory, min (OldSize, NewSize));
    _MemDelete (Memory, OldSize);
  }
  else {
    if ((NewMemory = (PVOID) realloc ((PCHAR) Memory, NewSize)) == NULL) {
      INT ErrorClass = 0;

      do {
	(*MemErrorHandler) (ErrorClass++);
	NewMemory = (PVOID) realloc ((PCHAR) Memory, NewSize);
      } while (NewMemory == NULL);
    }
#if defined (__MEMTEST__)
    ulMemUsed += NewSize - OldSize;
#endif
  }
  return (NewMemory);
} /* _MemResize */

PVOID MemExtend (PVOID Memory, PSIZE Size)
  /**********************************************************************
  *  Funktion : Verdoppelt die Groesse eines mit _MemNew() oder MemNew()
  *		allokierten Speicherbereichs.
  *  Eingaben : Memory	: Zeiger auf zu erweiternden Speicherbereich.
  *		Size	: urspr. Groesse des Speicherbereichs.
  *  Ausgaben : Size	: neue Groesse des Speicherbereichs.
  *  Ergebnis : Zeiger auf Speicherbereich.
  *  Info     : Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */
{
  PVOID NewMemory;
  SIZE	OldSize = (*Size + 3) & ~3;
  SIZE	NewSize = *Size == 0 ? (*Size = 4) : ((*Size <<= 1) + 3) & ~3;

  if (OldSize <= MEM_MAXSIZE) {
    NewMemory = (PVOID) _MemNew (NewSize);
    memcpy (NewMemory, Memory, OldSize);
    _MemDelete (Memory, OldSize);
  }
  else {
    if ((NewMemory = (PVOID) realloc ((PCHAR) Memory, NewSize)) == NULL) {
      INT ErrorClass = 0;

      do {
	(*MemErrorHandler) (ErrorClass++);
	NewMemory = (PVOID) realloc ((PCHAR) Memory, NewSize);
      } while (NewMemory == NULL);
    }
#if defined (__MEMTEST__)
    ulMemUsed += NewSize - OldSize;
#endif
  }
  return (NewMemory);
} /* MemExtend */

PVOID MemExtendList (PVOID List, PSIZE nElmts, SIZE ElmtSize)
  /**********************************************************************
  *  Funktion : Erweitert eine Liste auf die doppelte Elemntanzahl.
  *  Eingaben : List	 : Zeiger auf zu erweiternde Liste.
  *		nElmts	 : urspr. # Elemente.
  *		ElmtSize : Elementgroesse in Bytes.
  *  Ausgaben : nElmts	 : neue # Elemente.
  */
{
  PVOID NewList;
  SIZE	OldSize, NewSize;

  ElmtSize = AlignedSize (ElmtSize);
  OldSize  = (( *nElmts	* ElmtSize) + 3) & ~3;
  NewSize  = (((*nElmts == 0 ? (*nElmts = 1) : (*nElmts <<= 1))
      * ElmtSize) + 3) & ~3;

  if (OldSize <= MEM_MAXSIZE) {
    NewList = (PVOID) _MemNew (NewSize);
    memcpy (NewList, List, OldSize);
    _MemDelete (List, OldSize);
  }
  else {
    if ((NewList = (PVOID) realloc ((PCHAR) List, NewSize)) == NULL) {
      INT ErrorClass = 0;

      do {
	(*MemErrorHandler) (ErrorClass++);
	NewList = (PVOID) realloc ((PCHAR) List, NewSize);
      } while (NewList == NULL);
    }
#if defined (__MEMTEST__)
    ulMemUsed += NewSize - OldSize;
#endif
  }
  return (NewList);
} /* MemExtendList */

PPVOID MemNewArray (USHORT nRows, USHORT nColumns, SIZE ElmtSize)
  /**********************************************************************
  *  Funktion : Allokiert eine Tabelle.
  *  Eingaben : nRows	 : # Zeilen.
  *		nColumns : # Spalten.
  *		ElmtSize : Elementgroesse in Bytes.
  *  Ergebnis : Zeiger auf Tabelle.
  *  Info     : Falls nicht genug Speicher zur Verfuegung steht, wird
  *		(*MemErrorHandler) () aufgerufen.
  */
{
  PPVOID Array;
  USHORT i;

  if ((Array = (PPVOID) _MemNew (nRows * sizeof (PVOID))) != NULL) {
    SIZE Size = ((nColumns * AlignedSize (ElmtSize)) + 3) & ~3;

    for (i = 0; i < nRows; i++)
      if ((Array [i] = (PVOID) _MemNew (Size)) == NULL) {
	while (i--) _MemDelete (Array [i], Size);
	_MemDelete (Array, nRows * sizeof (PVOID));
	return (NULL);
      }
  }
  return (Array);
} /* MemNewArray */

VOID MemDeleteArray (
  PPVOID Array, USHORT nRows, USHORT nColumns, SIZE ElmtSize)
  /**********************************************************************
  *  Funktion : Gibt eine Tabelle frei.
  *  Eingaben : Array	 : Zeiger auf freizugebende Tabelle.
  *		nRows	 : # Zeilen.
  *		nColumns : # Spalten.
  *		ElmtSize : Elementgroesse in Bytes.
  */
{
  SIZE	 Size = ((nColumns * AlignedSize (ElmtSize)) + 3) & ~3;
  USHORT i    = nRows;

  while (i--) _MemDelete (Array [i], Size);
  _MemDelete (Array, nRows * sizeof (PVOID));
} /* MemDeleteArray */
