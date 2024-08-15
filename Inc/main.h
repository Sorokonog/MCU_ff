#define SOME_VALUE_OF_BITS_FOR_TRX_WAITING 90

bool waiting_transmit_to_start();
bool signal_read(std::chrono::system_clock::time_point prev_reading, int * ret_val);
bool detect_signal_front_time_point(std::chrono::system_clock::time_point & signalFrontTimePoint, int signalLevel);