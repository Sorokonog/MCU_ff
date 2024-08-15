#include <stdio.h>
#include <iostream>
#include <bitset>
#include <chrono>
#include <cstdio>
#include <vector>

#include "main.h"

#include "trackerinterface.h"

using namespace std::chrono_literals; //для возможностей использования литералов типа 1s, 5m и т.д.


int main(int, char**){

    int retVal;
    
    waiting_transmit_to_start(); //Ждем начала передачи пакета 
    printf("0"); //0 для коррекции , это обусловлено алгоритмом работы функции waiting_transmit_to_start()

    std::chrono::system_clock::time_point prevReading{std::chrono::system_clock::now()};
    std::chrono::system_clock::time_point start{std::chrono::system_clock::now()};

    //цикл вызывающий метод библиотеки get_signal_state()
    while((start + 2s) > std::chrono::system_clock::now()) //читаем сигнал 2 секунды
    {
        if(is_time_for_signal_reading(prevReading))
        {
            prevReading = std::chrono::system_clock::now();
            retVal = get_signal_state();
            printf("%d", retVal);
        }
    }
};


/**
 * @brief Функция проверяет пора ли читать следующий бит
 *
 * Частота чтения задается константой UART_SIGNAL_INTERVAL (1/1200 секунды)
 * 
 * @return true/false в зависимости от текущего и предыдущего момента чтения
 */
bool is_time_for_signal_reading(std::chrono::system_clock::time_point prevReading)
{
    return((std::chrono::system_clock::now() - prevReading) > UART_SIGNAL_INTERVAL);
}


/**
 * @brief Функция актуализирует момент изменения фронта сигнала  
 *
 * @arg std::chrono::system_clock::time_point & signalFrontTimePoint - ссылка на временную метку начала фронта, int signalLevel уровень сигнала по которому мерить измнения
 * 
 * Потенциально данная функция может использоваться для синхронизации по стоповым битам
 * Частота чтения задается константой SIGNAL_DETECTION_INTERVAL (1/12000 секунды)
 * т.е. в 10 раз чаще чем обычная скорость UART. 
 * Это позволяет точнее знать момент начала передачи следующего байта.
 * 
 * @return true/false в зависимости от успеха определения времени фронта
 */
bool detect_signal_front_time_point(std::chrono::system_clock::time_point & signalFrontTimePoint, int signalLevel)
{
    bool signalFrontDetected = false;
    int retVal;
    int counter = 0;
    std::chrono::system_clock::time_point prevReading = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point nowTimePoint;

    while(!signalFrontDetected) //потенциально можно использовать более сложную логику, но можно и заменить на просто true
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

/**
 * @brief Функция ждет начала передачи пакета отбрасывая единички в сигнале. 
 *
 * Блокирующая функция специально не дающая работать основному циклу, пока не началась передача посылки
 * 
 * @return true/false в зависимости от успеха определения начала посылки
 */
bool waiting_transmit_to_start()
{
    int counter = 0;
    int retVal;
    bool txStarted = false;
    std::chrono::system_clock::time_point prevReading = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point nowTimePoint;
    while(!txStarted) //потенциально можно использовать более сложную логику, но можно и заменить на просто true
    {
        nowTimePoint = std::chrono::system_clock::now();
        if((nowTimePoint - prevReading) > SIGNAL_DETECTION_INTERVAL) //Использую SIGNAL_DETECTION_INTERVAL, но можно и UART_SIGNAL_INTERVAL, есть мнение что с SIGNAL_DETECTION_INTERVAL результат получше
        {
            retVal = get_signal_state();    
        if(retVal)
        {
            counter++;   
        }
        //магическо эмприческое значение SOME_VALUE_OF_BITS_FOR_TRX_WAITING == 90. Если насчитали такое количество единиц подряд, значит мы в режиме ожидания.
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