#include <config.h>
#include "communication.h"
#include "motor_control.h"
#include "balancing.h"
#include "battery.h"
#include "communication.h"

void setup() {
  Serial.begin(BAUD_RATE);

  Serial.println("Beginning Setup");

  COMM_init();

  if (!IMU_init()) {
    Serial.println("IMU setup went wrong!");
  }
  motors_init();
  battery_init();

  Serial.println("Setup finished");
}

unsigned long lastTelemetTime = 0;

void loop() {
  //Update MPU9250
  IMU_update();

  //Check for BLE updates
  COMM_update();

  if (millis() - lastTelemetTime > TELEMETRY_INTERVAL) {
    float currentAngle = get_IMU_angle_y();
    float currentBattery = get_voltage();
    
    COMM_send_telemetry(currentAngle, currentBattery);
    lastTelemetTime = millis();
  }

  //Turn the motors
  float effort = calculate_effort();
  motors_update(effort);

  //print_effort();
}