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
/*                                                                                     */
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
 \file   RNG.hpp
 \brief  Custom class for random number generator
 \author Christophe Tribes and Sebastien Le Digabel 
 \date   2011-09-28
 \see    RNG.cpp
 */

#ifndef __IBEX_RNG__
#define __IBEX_RNG__

#include "stdint.h"

// Max uint32 for random number generator
#if !defined(UINT32_MAX)
typedef unsigned int uint32_t;
#define UINT32_MAX	0xffffffff
#endif

namespace ibex {

	class RNG {
		
	public:

		/** \brief Set seed with random number
		 *  This function sets the seed for random number generation \c
		 *  \return A boolean if the seed is acceptable, that is in [0,UINT32_MAX].
		 */
		static void srand();

		/** \brief Set seed
		 *  This function sets the seed for random number generation \c
		 *  \return A boolean if the seed is acceptable, that is in [0,UINT32_MAX].
		 */
		static void srand(int s);

		/** \brief Get a random integer as uint32
		 * This function serves to obtain a random number \c
		 * \return An integer in the interval [0,UINT32_MAX].
		 */
		static uint32_t rand();

		/** \brief Get a random number having a normal distribution as double
         * \param a Lower bound  -- \b IN.
         * \param b Upper bound  -- \b IN.
         * \return A double in the interval [a,b].
		 */
		static double rand(double a, double b){return a+((double)(b-a)*RNG::rand())/UINT32_MAX;}
		
		/** \brief  Get a random number approaching a normal distribution ( N(Mean,Var) ) as double
         * \param mean : Mean of the target normal distribution		-- \b IN.
         * \param var :	Variance of the target normal distribution	-- \b IN.
         * \return A random number.
		 */
        static double normal_rand( double mean=0 , double var=1 ) ;

	private:
		static const uint32_t x0,y0,z0;
		static uint32_t x,y,z,seed;
	};
}

#endif
