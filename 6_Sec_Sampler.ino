
#ifndef WINDOW6_SAMPLER_H
#define WINDOW6_SAMPLER_H

#define ENABLE_MR60BHA2 1    // 0 = disabled, 1 = enabled
#define ENABLE_XM125 1       // 0 = disabled, 1 = enabled
#define ENABLE_SEN0344 1     // Temperature/Oximeter (optional)

#include <Arduino.h>
#include <math.h>
#include <QuickStats.h>
#include "Buffer_manager.h"


// Work in Progress, Looking to automate finding length. 
QuickStats stats; // Initialise

// Takes a 6 Second Segmentof Data then derives its statistics
float* Window_6(float* buffer, int length) {

    static float features[8]; 

    float maxVal = stats.max(buffer, length);
    float minVal = stats.min(buffer, length);
    
    // Amplitute
    features[0] = stats.stdev(buffer, length);     // std
    features[1] = calculateRMS(buffer, length);    // rms
    features[2] = stats.SNR(buffer, length);       // SNR
    features[3] = stats.mode(buffer, length, 0.05); // mode
    features[4] = maxVal - minVal;                 // p2p
    
    // 2. Geometric Mean
    features[5] = stats.g_average(buffer, length); // gmean

    // 3. Shape Analysis
    features[6] = skewness(buffer, length);        // skew
    features[7] = kurtosis(buffer, length);        // kurt

}


// https://github.com/RobTillaart/Kurtosis/tree/master
//
//                size * sum( (arr[i] - average)^4 )
//  kurtosis() = ------------------------------------
//                   sum( (arr[i] - average)^2 )^2
//

float kurtosis(float *arr, uint16_t size)
{
  // Average
  float sum = 0;
  for (uint16_t i = 0; i < size; i++) sum += arr[i];
  float average = sum / size;

  //  sum((x - average)^2)
  //  sum((x - average)^4)
  float a = 0;
  float b = 0;
  for (uint16_t i = 0; i < size; i++)
  {
    float val = arr[i] - average;
    val *= val;  //  (arr[i] - average)^2
    b   += val;
    val *= val;  //  (arr[i] - average)^4
    a   += val;
  }

  return (size * a) / (b * b);
}


//                   sum( (arr[i] - average)^3 )
//                   ---------------------------
//                              size
//  skewness() = ------------------------------------
//                   sum( (arr[i] - average)^2 )^1.5
//                   ---------------------------
//                             size^1.5
//
//  simplified
//
//                sqrt(size) * sum( (arr[i] - average)^3 )
//  skewness() = ------------------------------------------
//                   sum( (arr[i] - average)^2 )^1.5
//
float skewness(float *arr, uint16_t size)
{
  //  average
  float sum = 0;
  for (uint16_t i = 0; i < size; i++) sum += arr[i];
  float average = sum / size;

  //  a = sum((x - average)^2)
  //  b = sum((x - average)^3)
  float a = 0;
  float b = 0;
  for (uint16_t i = 0; i < size; i++)
  {
    float val = arr[i] - average;
    float val2 = val * val;   //  (arr[i] - average)^2
    float val3 = val2 * val;  //  (arr[i] - average)^3
    b += val2;
    a += val3;
  }

  return sqrtf(1.0 * size) * a * powf(b, -1.5);
}

float calculateRMS(float* buffer, int length) {
    if (length <= 0) return 0;

    float sumSquares = 0;

    for (int i = 0; i < length; i++) {
        sumSquares += buffer[i] * buffer[i];
    }

    return sqrt(sumSquares / length);


#endif