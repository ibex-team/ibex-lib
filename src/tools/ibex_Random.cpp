/*-------------------------------------------------------------------------------------*/
/*  NOMAD - Nonsmooth Optimization by Mesh Adaptive Direct search - version 3.5.1        */
/*                                                                                     */
/*  Copyright (C) 2001-2010  Mark Abramson        - the Boeing Company, Seattle        */
/*                           Charles Audet        - Ecole Polytechnique, Montreal      */
/*                           Gilles Couture       - Ecole Polytechnique, Montreal      */
/*                           John Dennis          - Rice University, Houston           */
/*                           Sebastien Le Digabel - Ecole Polytechnique, Montreal      */
/*                           Christophe Tribes    - Ecole Polytechnique, Montreal      */
/*                                                                                     */
/*  funded in part by AFOSR and Exxon Mobil                                            */
/*  Contact information:                                                               */
/*    Ecole Polytechnique de Montreal - GERAD                                          */
/*    C.P. 6079, Succ. Centre-ville, Montreal (Quebec) H3C 3A7 Canada                  */
/*    e-mail: nomad@gerad.ca                                                           */
/*    phone : 1-514-340-6053 #6928                                                     */
/*    fax   : 1-514-340-5665                                                           */
/*                                                                                     */
/*  This program is free software: you can redistribute it and/or modify it under the  */
/*  terms of the GNU Lesser General Public License as published by the Free Software   */
/*  Foundation, either version 3 of the License, or (at your option) any later         */
/*  version.                                                                           */
/*                                                                                     */
/*  This program is distributed in the hope that it will be useful, but WITHOUT ANY    */
/*  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A    */
/*  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.   */
/*                                                                                     */
/*  You should have received a copy of the GNU Lesser General Public License along     */
/*  with this program. If not, see <http://www.gnu.org/licenses/>.                     */
/*                                                                                     */
/*  You can find information on the NOMAD software at www.gerad.ca/nomad               */
/*-------------------------------------------------------------------------------------*/
/**
 \file   RNG.cpp
 \brief  Custom class for random number generator (implementation)
 \author Christophe Tribes and Sebastien Le Digabel and Jordan Ninin
 \date   2011-09-28
 \see    rng.hpp
 */

#include "ibex_Random.h"


#ifdef  _IBEX_WITH_DIRECT_

namespace ibex {
//** Default values for the random number seed  */
uint32_t RNG::x = 123456789;
uint32_t RNG::y = 362436069;
uint32_t RNG::z = 521288629;

bool RNG::srand()
{
	/** This function sets the seed for random number generation \c
	 \return A boolean if the seed is acceptable, that is in [0,UINT32_MAX].
	 */
	//srand(times(&t)+time(NULL));

	if(x<UINT32_MAX)
	{
		x++;
		return true;
	}
	else {
		x  = 123456789; // the default value
		return false;
	}
}

bool RNG::srand(unsigned long s)
{
	/** This function sets the seed for random number generation \c 
	 \return A boolean if the seed is acceptable, that is in [0,UINT32_MAX].
	 */
	if(s<=UINT32_MAX)
	{
		x=s;
		return true;
	}
	else 
		return false;
}

uint32_t RNG::rand () { //period 2^96-1
	/** This function serves to obtain a random number \c 
	 \return An integer in the interval [0,UINT32_MAX].
	 */
 	uint32_t t;
 	x ^= x << 16;
 	x ^= x >> 5;
 	x ^= x << 1;
 	
 	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;      
	
	return z;
}


} // end namespace ibex
#else
#include <stdlib.h>

bool ibex::RNG::srand(){ ::srand(::rand()+10);  return true; }

bool ibex::RNG::srand(unsigned long s){  ::srand(s); return true; }

uint32_t ibex::RNG::rand () { return ::rand(); }


#endif
