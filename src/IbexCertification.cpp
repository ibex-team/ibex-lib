/*---------------------------------------------------------------------------------
 * 
 * Certification (based on the strict contraction of a contractor)
 * ---------------------------------------------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#include "IbexCertification.h"

namespace ibex {

void Certification::contract() {

  if (space.box.max_diameter()>ceil) return;

  INTERVAL_VECTOR init(space.box);
  INTERVAL_VECTOR inflat(space.box);

  //cout.precision(16);
  //cout << "init=" << init << endl;

  //bool success=true;

  //REAL fac=1;  
  for (int i=0; i<space.nb_var(); i++) {
    //inflat(i+1)=Mid(init(i+1))+fac*INTERVAL(-Diam(init(i+1)),Diam(init(i+1))); //ceil/4.0,+ceil/4.0);
    inflat(i+1)=init(i+1)+INTERVAL(-ceil/4.0,+ceil/4.0);
  }

  space.box = inflat;
  //cout << "inflat=" << inflat << endl;
  
  try {

    prover->contract(space); // may throw EmptyBoxException

  } catch(EmptyBoxException) {    
    //cout << "no solution\n";
    goto restore_and_abort;
  }

  //cout << "tmp=" << space.box << endl;
  
  if (!space.box.interior(inflat)) {
    //cout << "no strict contraction.\n";
    goto restore_and_abort;
  }
  
  // We have proven that a solution exists inside [inflat].
  // We must prove now that this solution is inside [init]. 
  try {

    funnel->contract(space);

  } catch(EmptyBoxException) {    
    //cout << "no solution\n";
    goto restore_and_abort;
  }

    
  if (!space.box.included(init) ) {

    // Try to eliminate what juts out from the initial box 
    INTERVAL_VECTOR tmp=space.box;

    for (int i=0; i<space.nb_var(); i++) {

      if (Inf(tmp(i+1))<Inf(init(i+1))) {
	space.box(i+1)=INTERVAL(Inf(tmp(i+1)),Inf(init(i+1)));
	//cout << "try to eliminate slice inf (var:" << i+1 << ")" << space.box << " --> ";
	try {
	  prover->contract(space);
	  funnel->contract(space);
	  //cout << Inf(space.box(i+1)) << endl;
	  if (Inf(space.box(i+1))<Inf(init(i+1))) { // we have not reached the lower bound of [init]
	    //cout << "failed.\n";
	    goto restore_and_abort;
	  }
	}
	catch (EmptyBoxException) { }
	tmp(i+1)=INTERVAL(Inf(init(i+1)),Sup(tmp(i+1)));
	//cout << "ok.\n";
	space.box = tmp; // reinitialize to (new) [tmp]
      }

      if (Sup(tmp(i+1))>Sup(init(i+1))) {
	space.box(i+1)=INTERVAL(Sup(init(i+1)),Sup(tmp(i+1)));
	//cout << "try to eliminate slice sup (var:" << i+1 << ")" << space.box << " --> ";
	try {
	  prover->contract(space);
	  funnel->contract(space);
	  if (Sup(space.box(i+1))!=Sup(init(i+1))) { // we have not reached the lower bound of [init]
	    //cout << "failed.\n";
	    goto restore_and_abort;
	  }
	}
	catch (EmptyBoxException) { }
	tmp(i+1)=INTERVAL(Inf(tmp(i+1)),Sup(init(i+1)));
	//cout << "ok.\n";
	space.box = tmp; // reinitialize to (new) [tmp]
      }
    }

    //cout << "final=" << space.box << " sucess=" << success << endl;

  }

  //-------- success --------------
  throw EmptyBoxException();
  //-------------------------------

  restore_and_abort:
  
  space.box = init;  

  //space.box &= init; // forbidden: otherwise, the removed part of space.box would be considered as certified!
}

} // end namespace
