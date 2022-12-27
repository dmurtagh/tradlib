//
//  StandardDeviationCalculator.hpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#pragma once

#include <stdio.h>
#include <vector>
#include "TradlibCommon.hpp"

namespace tradlib
{
    class StandardDeviationCalculator
    {
        
        /**
         * Calculates the sample standard deviation of an array
         * of numbers, when the number are obtained by a random sampling.
         * Code must be modified if you have a complete set of data.
         * To get estimate of a complete data sampling, use n instead of n-1 in last line.
         *
         * see http://davidmlane.com/hyperstat/A16252.html
         *
         * @param data Numbers to compute the standard deviation of.
         * Array must contain two or more numbers.
         * @return standard deviation estimate of population
         */
    public:
        static float sdFast (const SharedFloatVec & data);
        
        /**
         * Calculates the sample standard deviation of an array
         * of numbers,  when the number are obtained by a random sampling.
         * Code must be modified if you have a complete set of data.
         * To get estimate of a complete sample, use n instead of n-1 in last line.
         *
         * see Knuth's The Art Of Computer Programming
         * Volume II: Seminumerical Algorithms
         * This algorithm is slower, but more resistant to error propagation.
         *
         * @param data Numbers to compute the standard deviation of.
         * Array must contain two or more numbers.
         * @return standard deviation estimate of population
         */
        static float sdKnuth (const SharedFloatVec & data);
        
        /**
         * Test driver
         *
         */
        static void test ();
    };
}
