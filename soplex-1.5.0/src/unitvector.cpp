/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the class library                   */
/*       SoPlex --- the Sequential object-oriented simPlex.                  */
/*                                                                           */
/*    Copyright (C) 1996      Roland Wunderling                              */
/*                  1996-2010 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SoPlex is distributed under the terms of the ZIB Academic Licence.       */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License              */
/*  along with SoPlex; see the file COPYING. If not email to soplex@zib.de.  */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#pragma ident "@(#) $Id: unitvector.cpp,v 1.12 2010/09/16 17:45:04 bzfgleix Exp $"

#include <assert.h>

#include "unitvector.h"
#include "message.h"

namespace soplex
{

#ifndef NO_CONSISTENCY_CHECKS
bool UnitVector::isConsistent() const
{
   if (mem() != themem)
      return MSGinconsistent("UnitVector");
   if (mem() + 1 != &themem[1])
      return MSGinconsistent("UnitVector");
   if (size() != 1)
      return MSGinconsistent("UnitVector");
   if (max() != 1)
      return MSGinconsistent("UnitVector");

   return SVector::isConsistent();
}
#endif
} // namespace soplex

//-----------------------------------------------------------------------------
//Emacs Local Variables:
//Emacs mode:c++
//Emacs c-basic-offset:3
//Emacs tab-width:8
//Emacs indent-tabs-mode:nil
//Emacs End:
//-----------------------------------------------------------------------------
