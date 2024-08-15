#include <stdio.h>
#include <iostream>
#include <bitset>
#include <chrono>
#include <cstdio>

#include "main.h"

#include "trackerinterface.h"

using namespace std::chrono_literals;



std::chrono::nanoseconds UART_SIGNAL_INTERVAL{833333};
std::chrono::nanoseconds SIGNAL_DETECTION_INTERVAL{UART_SIGNAL_INTERVAL/10};

//std::bitset<10> byte_to_fill_from_single_bits;

int main(int, char**){



    int retVal = 4;

    //printf("%d\n", std::chrono::nanoseconds(UART_SIGNAL_INTERVAL_IN_MC).count());
    
    waiting_transmit_to_start();

    // std::chrono::system_clock::time_point signalToDetect;
    // signalToDetect = std::chrono::system_clock::now();
    // 
    // printf("1st %d\n", std::chrono::nanoseconds(std::chrono::system_clock::now() - signalToDetect).count());
    // detect_signal_front_time_point(signalToDetect, 1);
    // printf("2nd %d\n", std::chrono::nanoseconds(signalToDetect - start).count());

    std::chrono::system_clock::time_point readingTime{std::chrono::system_clock::now()};
    std::chrono::system_clock::time_point start{std::chrono::system_clock::now()};

    while((start + std::chrono::seconds{2}) > std::chrono::system_clock::now())
    {
        
        if(signal_read(readingTime, &retVal))
        {
            readingTime = std::chrono::system_clock::now();
            printf("%d",retVal);
        }
        else
        {
        }
    }
    //printf("%d\n", std::chrono::nanoseconds(std::chrono::system_clock::now() - prev_reading).count());
};


bool signal_read(std::chrono::system_clock::time_point prevReading, int * retVal)
{
if((std::chrono::system_clock::now() - prevReading) > UART_SIGNAL_INTERVAL)
{
*retVal = get_signal_state();
return true;
}
else
{
return false;
}
};

bool detect_signal_front_time_point(std::chrono::system_clock::time_point & signalFrontTimePoint, int signalLevel)
{
    bool signalFrontDetected = false;
    int retVal;
    int counter = 0;
    std::chrono::system_clock::time_point prevReading = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point nowTimePoint;

    while(!signalFrontDetected)
    {
        nowTimePoint = std::chrono::system_clock::now();
        if((nowTimePoint - prevReading) > SIGNAL_DETECTION_INTERVAL)
        {
            retVal = get_signal_state();    
            if(retVal == signalLevel)
            {
                counter++;
            }
            else if(retVal != signalLevel && counter != 0)
            {
                signalFrontTimePoint = nowTimePoint;        //TODO (nowTimePoint - prevReading)/2; 
                return true;
            }
            else
            {
                return false;
            }
        prevReading = std::chrono::system_clock::now();
        }
    }
}

bool waiting_transmit_to_start()
{
    int counter = 0;
    int retVal;
    bool txStarted = false;
    std::chrono::system_clock::time_point prevReading = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point nowTimePoint;
    while(!txStarted)
    {
        nowTimePoint = std::chrono::system_clock::now();
        if((nowTimePoint - prevReading) > SIGNAL_DETECTION_INTERVAL)
        {
            retVal = get_signal_state();    
        if(retVal)
        {
            counter++;   
        }
        else if(!retVal && counter > SOME_VALUE_OF_BITS_FOR_TRX_WAITING)
        {
            return true;
        }
        else
        {
            counter = 0;
        }
        prevReading = std::chrono::system_clock::now();
        }
    }
};