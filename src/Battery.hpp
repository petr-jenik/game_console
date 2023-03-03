#pragma once

#ifdef BUILD_FOR_X86
#include <ArduinoAPI.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

class Battery 
{
private:
    static const pin_size_t ADC_PIN = 26;
public:
    Battery(){}

    // Get mean value from ADC calculated over N samples
    int readAdcMeanValue(int nSample)
    {
        long adc = 0;
        int count = 0;
        for (count = 0; count < nSample; count++)
        {
            adc += analogRead(ADC_PIN);
        }
        return adc / count;
    }


    // Measure voltage using ADC
    // Compensate for ADC error as decribed in https://w4krl.com/esp32-analog-to-digital-conversion-accuracy/
    int voltageMilliV()
    {
        // read mean value from ADC over 10 samples
        int adc = this->readAdcMeanValue(10);

        // TODO Remove usage of float
        float volts = 0;

        // Compensate for an error of used ADC on ESP32
        if ( adc > 3000 )
        {
            volts = 0.0005 * adc + 1.0874;
        }
        else
        {
            volts = 0.0008 * adc + 0.1372;
        }

        // Compensate for a voltage divider connected between battery voltage and ADC input.
        volts = 5 * volts / 3;

        // Convert voltage to millivolts
        return static_cast<int>(volts * 1000);
    }
};