#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include "Config.h"
#include "CircularBuffer.hpp"

// MR60BHA2
#if ENABLE_MR60BHA2
config.SensorConfig.
CircularBuffer<float, 1200> bufferBreathingPhase;
CircularBuffer<float, 1200> bufferHeartPhase;
uint16_t sizeMR60; // Keeps track on how many values concurrently
float rateMR60;
#endif

// XM125 enabled for Distance
#if ENABLE_XM125
CircularBuffer<float, 1200> bufferPeak0Distance;
CircularBuffer<float, 1200> bufferPeak1Distance;
CircularBuffer<float, 1200> bufferPeak2Distance;
CircularBuffer<float, 1200> bufferPeak3Distance;
CircularBuffer<float, 1200> bufferPeak4Distance;
CircularBuffer<float, 1200> bufferPeak5Distance;

CircularBuffer<float, 1200> bufferPeak0Strength;
CircularBuffer<float, 1200> bufferPeak1Strength;
CircularBuffer<float, 1200> bufferPeak2Strength;
CircularBuffer<float, 1200> bufferPeak3Strength;
CircularBuffer<float, 1200> bufferPeak4Strength;
CircularBuffer<float, 1200> bufferPeak5Strength;
uint16_t sizeXM125;
float rateXM125;
#endif

// SEN0344
#if ENABLE_SEN0344
CircularBuffer<float, 1200> bufferTemperature;
CircularBuffer<float, 1200> bufferOxygenSaturation;
CircularBuffer<float, 1200> bufferHeartRate;
uint16_t sizeSEN0344;
float rateSEN0344;
#endif

// MR60BHA2 push
#if ENABLE_MR60BHA2
void pushMR60(float breathing, float heart) {
    bufferBreathingPhase.push(breathing);
    bufferHeartPhase.push(heart);
}
#endif

// XM125 push
#if ENABLE_XM125
void pushXM125(float peak0, float peak1, float peak2, 
               float peak3, float peak4, float peak5, 
               float str0, float str1, float str2, 
               float str3, float str4, float str5) {
    bufferPeak0Distance.push(peak0);
    bufferPeak1Distance.push(peak1);
    bufferPeak2Distance.push(peak2);
    bufferPeak3Distance.push(peak3);
    bufferPeak4Distance.push(peak4);
    bufferPeak5Distance.push(peak5);

    bufferPeak0Distance.push(str0);
    bufferPeak1Distance.push(str1);
    bufferPeak2Distance.push(str2);
    bufferPeak3Distance.push(str3);
    bufferPeak4Distance.push(str4);
    bufferPeak5Distance.push(str5);
}
#endif

// SEN0344 push
#if ENABLE_SEN0344
void pushSEN0344(float temp, float o2, float heart) {
    bufferTemperature.push(temp);
    bufferOxygenSaturation.push(o2);
    bufferHeartRateSEN.push(heart);
}
#endif

// Check if buffers  are lagging
#if ENABLE_MR60BHA2
bool isMR60Ready() {
    return bufferBreathingPhase.size() >= sizeMR60;
}
#endif

#if ENABLE_XM125
bool isXM125Ready() {
    return bufferPeak0Distance.size() >= sizeXM125;
}
#endif

#if ENABLE_SEN0344
bool isSEN0344Ready() {
    return bufferTemperature.size() >= sizeSEN0344;
}
#endif


#endif