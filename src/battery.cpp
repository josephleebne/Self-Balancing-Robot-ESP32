#include "battery.h"
#include "config.h"

void battery_init() {
    pinMode(BATTERY_PIN, ANALOG);
}

float get_voltage() {
    float sum = 0;

    for (int i = 0; i < NO_OF_SAMPLES; i++) {
        sum += analogRead(BATTERY_PIN);
    }

    float rawAverage = sum / NO_OF_SAMPLES;

    float dividerRatio = (float)(BIG_RESISTOR + SMALL_RESISTOR) / SMALL_RESISTOR;
    float voltage = (VREF / ADC_RESOLUTION) * rawAverage * dividerRatio + CALIBRATION_CONSTANT;

    return voltage;
}

float calculate_battery_level(float voltage) {
    float percentage = (voltage - MIN_VOLTS) * (MAX_BATTERY_PERCENTAGE - MIN_BATTERY_PERCENTAGE) / (MAX_VOLTS - MIN_VOLTS);
    percentage = (int)constrain(percentage, MIN_BATTERY_PERCENTAGE, MAX_BATTERY_PERCENTAGE);
    return percentage;
}


