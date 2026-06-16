#pragma once

/*
FOR IMU
*/

#include <MPU6050_light.h>
#include <Wire.h>

extern MPU6050 mpu;

bool IMU_init();
void IMU_update();
float get_IMU_angle_y();

/*
   =============================
   For PID Loop
   =============================
*/
float calculate_effort();
void print_effort();

struct PIDController {
    float kp, ki, kd;
    float setPoint, errorSum, lastError, effort, dt;
    unsigned long lastCalculationTime;
    void reset();
};

extern PIDController balancePID;