#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif 

#define SOME_VALUE_OF_BITS_FOR_TRX_WAITING 90

const std::chrono::nanoseconds UART_SIGNAL_INTERVAL{833333};
const std::chrono::nanoseconds SIGNAL_DETECTION_INTERVAL{UART_SIGNAL_INTERVAL/10};

bool waiting_transmit_to_start();
bool is_time_for_signal_reading(std::chrono::system_clock::time_point prevReading);
bool detect_signal_front_time_point(std::chrono::system_clock::time_point & signalFrontTimePoint, int signalLevel);

#ifdef __cplusplus
}
#endif

#endif