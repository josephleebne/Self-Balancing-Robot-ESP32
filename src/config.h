#pragma once
#include <Arduino.h>

/*
   =============================
   Motor Driver (L298N)
   =============================
*/

//25, 26, 27, 32, 33, 14
#define EN_A 32
#define INT_1 33
#define INT_2 25
#define INT_3 26
#define INT_4 27
#define EN_B 14

#define CHANNEL_0 0
#define CHANNEL_1 1
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8

#define MAX_SPEED 255.0f
#define MIN_SPEED 120.0f
#define NEUTRAL_EFFORT 0.2f
#define MAX_EFFORT 100.0f
#define MIN_EFFORT -100.0f
#define MIN_EFFORT_MAGNITUDE 0.1f
#define RIGHT_MOTOR_SLOW_FACTOR 0.97f
#define FAIL_ANGLE 45.0f

#define TARGET_VELOCITY 2.0f
#define TARGET_TURN 50.0f

#define SET_POINT -1.0f

/*
   =============================
   MPU6050
   =============================
*/
#define MPU_ADDRESS 0x68
#define GYRO_GAIN 0.98
#define ACCEL_GAIN 0.02
#define SCL_PIN 18
#define SDA_PIN 5

/*
   =============================
   Battery Monitoring
   =============================
*/
#define BATTERY_PIN 35
#define BIG_RESISTOR 9010
#define SMALL_RESISTOR 983
#define VREF 3.3f
#define ADC_RESOLUTION 4095
#define CALIBRATION_CONSTANT 6.89f
#define NO_OF_SAMPLES 50
#define MIN_VOLTS 7.0f
#define MAX_VOLTS 8.4f
#define MIN_BATTERY_PERCENTAGE 0
#define MAX_BATTERY_PERCENTAGE 100

/*
   =============================
   Bluetooth
   =============================
*/
#define CONTROL_SERVICE_UUID "12345678-1234-1234-1234-1234567890ab"
#define CMD_CHARACTERISTIC_UUID "abcdefab-1234-1234-1234-abcdefabcdef"
#define TELEMETRY_CHARACTERISTIC_UUID "00002a51-1234-5678-1234-567812345678"
#define TELEMETRY_INTERVAL 100

/*
   =============================
   PID Loop
   =============================
*/
#define INITIAL_KP 37.0f
#define INITIAL_KI 6.0f
#define INITIAL_KD 0.5f
#define PID_RATE_SECONDS 0.005f
#define LEAKINESS_FACTOR 1.0f

/*
   =============================
   LED
   =============================
*/
#define LED_PIN 13

/*

   =============================
   GENERAL
   =============================
*/
#define BAUD_RATE 115200
#define LOG_DELAY 50
#define STARTUP_DELAY 1000