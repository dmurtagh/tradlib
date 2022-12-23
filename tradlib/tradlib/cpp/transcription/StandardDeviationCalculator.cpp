//
//  StandardDeviationCalculator.cpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#include <cmath>
#include "StandardDeviationCalculator.hpp"
#include "Logger.hpp"

using namespace tradlib;
 
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
float StandardDeviationCalculator::sdFast(const std::vector<float> & data)
{
    // sd is sqrt of sum of (values-mean) squared divided by n - 1
    // Calculate the mean
    float mean = 0;
    const size_t n = data.size();
    if ( n < 2 )
    {
        return nanf("NAN");
    }
    for ( int i=0; i<n; i++ )
    {
        mean += data[i];
    }
    mean /= n;
    // calculate the sum of squares
    float sum = 0;
    for ( int i=0; i<n; i++ )
    {
        const float v = data[i] - mean;
        sum += v * v;
    }
    // Change to ( n - 1 ) to n if you have complete data instead of a sample.
    return sqrt( sum / ( n - 1 ) );
}

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
float StandardDeviationCalculator::sdKnuth(const std::vector<float> & data)
{
    const size_t n = data.size();
    if ( n < 2 )
    {
        return nanf("NAN");
    }
    float avg = data[0];
    float sum = 0;
    for ( int i = 1; i < n; i++ )
    {
        float newavg = avg + ( data[i] - avg ) / ( i + 1 );
        sum += ( data[i] - avg ) * ( data [i] -newavg ) ;
        avg = newavg;
    }
    // Change to ( n - 1 ) to n if you have complete data instead of a sample.
    return sqrt( sum / ( n - 1 ) );
}

/**
 * Test driver
 *
 */
void StandardDeviationCalculator::test()
{
    std::vector<float> data = { 10, 100 , 50};
    Logger::log("sdFast result = " + std::to_string(sdFast( data )));
    Logger::log("sdKnuth result = " + std::to_string(sdKnuth( data )));
}
