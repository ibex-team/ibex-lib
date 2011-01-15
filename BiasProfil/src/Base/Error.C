/************************************************************************
 *
 * Implementation of error handler and misc. checkings
 * ---------------------------------------------------
 *
 * Copyright (C) 1993, 1997 Olaf Knueppel
 *               2005 Christian Keil
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
 * $Id: Error.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static char rcs_id[] = "$Id: Error.C 478 2006-08-09 13:13:30Z keil $";

#include <stdlib.h>
#include <Error.h>
#include <iostream>
using namespace std;

INT ErrorHandler::LastErrorCode = 0;
INT ErrorHandler::MaxErrorLevel = 0;
INT ErrorHandler::WarningLevel = ErrorHandler::Warning;
INT ErrorHandler::SevereLevel  = ErrorHandler::SevereError;

ErrorHandler MyErrorHandler; // destructor should be called automatically at the end

VOID ErrorReport (INT DoReportSuccess)
{
  if (ErrorHandler::MaxErrorLevel > 0) {
    cerr << "*** Errors occured:" << endl
         << "    Max. error level: " << ErrorHandler::MaxErrorLevel << endl
         << "    Last error code:  " << ErrorHandler::LastErrorCode << endl;
    ErrorHandler::MaxErrorLevel = 0; // to prevent this report to occur twice
  }
  else if (DoReportSuccess)
    cerr << "*** No errors occured." << endl;
}

VOID Error (CONST char *s, INT ErrNo, ErrorHandler::ErrorLevel Priority)
{
  INT ErrorType;
  INT isWarning;

  ErrorHandler::LastErrorCode = ErrNo;
  if (Priority > ErrorHandler::MaxErrorLevel)
    ErrorHandler::MaxErrorLevel = Priority;
  if (Priority == ErrorHandler::FatalError) ErrorType = 0;
  else if (Priority < ErrorHandler::SevereLevel) ErrorType = 1;
  else ErrorType = 2;
  if (ErrorType && (Priority < ErrorHandler::WarningLevel)) return;
  isWarning = (Priority < ErrorHandler::SevereError);
  if (ErrorType)
    cerr << "*** "
         << (isWarning ? "Warning" : "Error")
         << (((ErrorType == 1) && !isWarning) ? "(ignored)" : "")
         << ErrNo << '/' << int (Priority) << ": " << s << endl;
  else
    cerr << "*** Fatal Error: " << s << endl;
  switch (ErrorType) {
  case 0:
    abort ();
  case 1:
    return;
  case 2:
    exit (1);
  }
}

VOID FatalError (CONST char *s)
{
  Error (s, 1, ErrorHandler::FatalError);
}

VOID _CheckDimensions (INT aDim, INT bDim)
{
  if (aDim != bDim) FatalError ("Dimensions differ");
}

VOID _CheckMatrixDimensions (INT aRowDim, INT bRowDim, INT aColDim, INT bColDim)
{
  if (aRowDim != bRowDim) FatalError ("Row dimensions differ");
  if (aColDim != bColDim) FatalError ("Column dimensions differ");
}

VOID _CheckQuadratic (INT rDim, INT cDim)
{
  if (rDim != cDim) FatalError ("Non square matrix");
}

VOID _CheckMemory (CONST VOID *p)
{
  if (p == NULL) FatalError ("Out of Memory");
}

#ifdef __INDEXCHECK__
VOID _CheckVectorRange (INT n)
{
  if (n < 0) FatalError ("Negative vector dimension");
}

VOID _CheckMatrixRange (INT n, INT m)
{
  if (n < 0) FatalError ("Negative matrix row dimension");
  if (m < 0) FatalError ("Negative matrix column dimension");
}

VOID _CheckVectorIndex (INT i, INT n)
{
  if ((i <= 0) || (i > n))
    FatalError ("Vector index out of range");
}

VOID _CheckMatrixRowIndex (INT i, INT n)
{
  if ((i <= 0) || (i > n))
    FatalError ("Matrix row index out of range");
}

VOID _CheckMatrixColIndex (INT i, INT n)
{
  if ((i <= 0) || (i > n))
    FatalError ("Matrix column index out of range");
}
#endif
