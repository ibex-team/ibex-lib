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
#pragma ident "@(#) $Id: updatevector.cpp,v 1.13 2010/09/16 17:45:04 bzfgleix Exp $"

#include "updatevector.h"
#include "message.h"

namespace soplex
{

UpdateVector& UpdateVector::operator=(const UpdateVector& rhs)
{
   if (this != &rhs)
   {
      theval   = rhs.theval;
      thedelta = rhs.thedelta;
      DVector::operator=(rhs);

      assert(UpdateVector::isConsistent());
   }
   return *this;
}

UpdateVector::UpdateVector( const UpdateVector& base)
   : DVector( base )
   , theval(base.theval)
   , thedelta(base.thedelta)
{
   assert(UpdateVector::isConsistent());
}

#ifndef NO_CONSISTENCY_CHECKS
bool UpdateVector::isConsistent() const
{
   if (dim() != thedelta.dim())
      return MSGinconsistent("UpdateVector");

   return DVector::isConsistent() && thedelta.isConsistent();
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
