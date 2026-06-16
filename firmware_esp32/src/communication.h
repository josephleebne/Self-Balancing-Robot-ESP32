#pragma once
#include <ArduinoBLE.h>
#include <balancing.h>

extern float targetVelocity;
extern float targetTurn;

void COMM_init();
void COMM_update();
void COMM_send_telemetry(float angle, float battery);