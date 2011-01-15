/************************************************************************
 *
 * Definition of error handler and misc. checkings
 * -----------------------------------------------
 *
 * Copyright (C) 1993, 1995, 1997 Olaf Knueppel
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
 * $Id: Error.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __ERROR__
#define __ERROR__

#include <Configuration.h>

#define CheckDimensions(a,b)        _CheckDimensions (Dimension(a), Dimension(b))
#define CheckColumnDimensions(a,b)  _CheckDimensions (ColDimension(a), Dimension(b))
#define CheckRowDimensions(a,b)     _CheckDimensions (RowDimension(a), Dimension(b))
#define CheckMatrixMatrixDimensions(a,b) \
  _CheckDimensions (ColDimension(a), RowDimension(b))
#define CheckMatrixDimensions(a,b)  \
  _CheckMatrixDimensions (RowDimension(a), RowDimension(b), \
			  ColDimension(a), ColDimension(b))
#define CheckQuadratic(a)           _CheckQuadratic(RowDimension(a), ColDimension(a))
#define CheckMemory(p)              _CheckMemory     ((CONST VOID *) (p))

#ifdef __INDEXCHECK__
#define CheckVectorRange(n)         _CheckVectorRange (n)
#define CheckMatrixRange(n,m)       _CheckMatrixRange (n, m)
#define CheckVectorIndex(i,v)       _CheckVectorIndex (i, Dimension(v))
#define CheckMatrixRowIndex(i,m)    _CheckMatrixRowIndex (i, RowDimension(m))
#define CheckMatrixColIndex(i,m)    _CheckMatrixColIndex (i, ColDimension(m))
#else
#define CheckVectorRange(n)
#define CheckMatrixRange(n,m)
#define CheckVectorIndex(i,v)
#define CheckMatrixRowIndex(i,m)
#define CheckMatrixColIndex(i,m)
#endif
#define CheckMatrixIndex(i,j,m)     CheckMatrixRowIndex(i,m); CheckMatrixColIndex(j,m)

VOID _CheckDimensions (INT, INT);
VOID _CheckMatrixDimensions (INT, INT, INT, INT);
VOID _CheckQuadratic (INT, INT);
VOID _CheckMemory (CONST VOID *);

#ifdef __INDEXCHECK__
VOID _CheckVectorRange (INT);
VOID _CheckMatrixRange (INT, INT);
VOID _CheckVectorIndex (INT, INT);
VOID _CheckMatrixRowIndex (INT, INT);
VOID _CheckMatrixColIndex (INT, INT);
#endif

class ErrorHandler {
public:
  static INT LastErrorCode;
  static INT MaxErrorLevel;
  static INT WarningLevel;
  static INT SevereLevel;
  enum ErrorLevel { FatalError = -1, Ignore = 0, Warning = 1000, SevereError = 2000 };
  ErrorHandler () { }
  ~ErrorHandler ();
  friend VOID Error (CONST char *, INT, ErrorLevel);
  friend VOID FatalError (CONST char *);
  friend VOID ErrorReport (INT);
};

VOID Error (CONST char *, INT, ErrorHandler::ErrorLevel);
VOID FatalError (CONST char *);
VOID ErrorReport (INT DoReportSuccess = 1);

inline ErrorHandler::~ErrorHandler () {
  ErrorReport (0); 
}

#endif /* __ERROR__ */
