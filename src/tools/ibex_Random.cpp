/*------------------------------------------------------------------------------*/
/*  NOMAD - Nonlinear Optimization by Mesh Adaptive Direct search -             */
/*          version 3.8.1                                                       */
/*                                                                              */
/*  NOMAD - version 3.8.1 has been created by                                   */
/*                 Charles Audet        - Ecole Polytechnique de Montreal       */
/*                 Sebastien Le Digabel - Ecole Polytechnique de Montreal       */
/*                 Christophe Tribes    - Ecole Polytechnique de Montreal       */
/*                                                                              */
/*  The copyright of NOMAD - version 3.8.1 is owned by                          */
/*                 Sebastien Le Digabel - Ecole Polytechnique de Montreal       */
/*                 Christophe Tribes    - Ecole Polytechnique de Montreal       */
/*                                                                              */
/*  NOMAD v3 has been funded by AFOSR, Exxon Mobil, Hydro Québec, Rio Tinto     */
/*  and IVADO.                                                                  */
/*                                                                              */
/*  NOMAD v3 is a new version of NOMAD v1 and v2. NOMAD v1 and v2 were created  */
/*  and developed by Mark Abramson, Charles Audet, Gilles Couture, and John E.  */
/*  Dennis Jr., and were funded by AFOSR and Exxon Mobil.                       */
/*                                                                              */
/*  Contact information:                                                        */
/*    Ecole Polytechnique de Montreal - GERAD                                   */
/*    C.P. 6079, Succ. Centre-ville, Montreal (Quebec) H3C 3A7 Canada           */
/*    e-mail: nomad@gerad.ca                                                    */
/*    phone : 1-514-340-6053 #6928                                              */
/*    fax   : 1-514-340-5665                                                    */
/*                                                                              */
/*  This program is free software: you can redistribute it and/or modify it     */
/*  under the terms of the GNU Lesser General Public License as published by    */
/*  the Free Software Foundation, either version 3 of the License, or (at your  */
/*  option) any later version.                                                  */
/*                                                                              */
/*  This program is distributed in the hope that it will be useful, but WITHOUT */
/*  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       */
/*  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License */
/*  for more details.                                                           */
/*                                                                              */
/*  You should have received a copy of the GNU Lesser General Public License    */
/*  along with this program. If not, see <http://www.gnu.org/licenses/>.        */
/*                                                                              */
/*  You can find information on the NOMAD software at www.gerad.ca/nomad        */
/*------------------------------------------------------------------------------*/


/**
 \file   RNG.cpp
 \brief  Custom class for random number generator (implementation)
 \author Christophe Tribes and Sebastien Le Digabel
 \date   2011-09-28
 \see    rng.hpp
 */

#include <math.h>
#include "ibex_Random.h"


namespace ibex {
//** Default values for the random number seed  */
const uint32_t RNG::x0 = 123456789;
const uint32_t RNG::y0 = 362436069;
const uint32_t RNG::z0 = 521288629;
uint32_t RNG::x = 123456789;
uint32_t RNG::y = 362436069;
uint32_t RNG::z = 521288629;
uint32_t RNG::seed = 0;

void RNG::srand()
{
	/** This function sets the seed for random number generation \c
	 \return A boolean if the seed is acceptable, that is in [0,UINT32_MAX].
	 */

	if(x<UINT32_MAX)
	{
		seed = x;
    	x = x0;
    	y = y0;
    	z = z0;
		for ( uint32_t i=0 ; i<seed ; i++)
			RNG::rand();
	}
	else {
		seed = 0;
    	x = x0;
    	y = y0;
    	z = z0;
	}
}

void RNG::srand(int s)
{
	/** This function sets the seed for random number generation \c 
	 \return A boolean if the seed is acceptable, that is in [0,UINT32_MAX].
	 */

    if( 0<=s && s<UINT32_MAX ) {
    	seed = s;
    	x = x0;
    	y = y0;
    	z = z0;
		for ( uint32_t i=0 ; i<seed ; i++)
			RNG::rand();
    }
    else if (0>s)  {
    	RNG::srand(-s);
    } else {
    	RNG::srand(s%UINT32_MAX);
    }
}

uint32_t RNG::rand () { //period 2^96-1
	/** This function serves to obtain a random number \c 
	 \return An integer in the interval [0,UINT32_MAX].
	 */
	// http://madrabbit.org/~ray/code/xorshf96.c
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


double RNG::normal_rand( double mean , double var )
{
    // Box-Muller transformation~\cite{BoMu58}

    double x1 , x2 , w;

    do
    {
        x1 = RNG::rand(-1.0,1.0);
        x2 = RNG::rand(-1.0,1.0);
        w  = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );

    w = ::sqrt( (-2.0 * ::log( w ) ) / w );

    return ::pow ( var,0.5) * x1 * w + mean;
}

} // end namespace ibex

