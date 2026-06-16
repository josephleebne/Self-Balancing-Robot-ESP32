#include "communication.h"
#include "config.h"
#include "balancing.h"

// BLE Services & Characteristics
static BLEService robotService(CONTROL_SERVICE_UUID);

// Characteristic for receiving PID/Commands (String based)
static BLEStringCharacteristic cmdChar(CMD_CHARACTERISTIC_UUID, BLEWrite, 20);

// Characteristic for sending Telemetry (String based)
static BLEStringCharacteristic telemetChar(TELEMETRY_CHARACTERISTIC_UUID, BLENotify, 50);

float targetVelocity = 0;
float targetTurn = 0;

void COMM_init() {
    if (!BLE.begin()) {
        Serial.println("BLE Failed");
        while (1);
    }

    BLE.setLocalName("BalanceBot");
    BLE.setAdvertisedService(robotService);

    robotService.addCharacteristic(cmdChar);
    robotService.addCharacteristic(telemetChar);
    
    BLE.addService(robotService);
    BLE.advertise();
}

void parseCommand(String cmd) {
    if (cmd.length() < 3) return;
    char type = cmd.charAt(0);

    if (type == 'M') {
        char moveDir = cmd.charAt(2);
        targetVelocity = 0;
        targetTurn = 0;

        if (moveDir == 'F') targetVelocity = TARGET_VELOCITY;
        else if (moveDir == 'B') targetVelocity = -TARGET_VELOCITY;
        else if (moveDir == 'L') targetTurn = TARGET_TURN;
        else if (moveDir == 'R') targetTurn = -TARGET_TURN;
        else if (moveDir == 'S') { targetVelocity = 0; targetTurn = 0; }
        
        // Serial Reporting for Movement
        Serial.print("Movement Command: ");
        Serial.println(moveDir);
        
    } else {
        float value = cmd.substring(2).toFloat();
        
        if (type == 'P') {
            balancePID.kp = value;
            Serial.print("New Kp: ");
        } 
        else if (type == 'I') {
            balancePID.ki = value;
            Serial.print("New Ki: ");
        } 
        else if (type == 'D') {
            balancePID.kd = value;
            Serial.print("New Kd: ");
        }
        
        Serial.println(value);
    }
}

void COMM_update() {
    BLE.poll();

    if (cmdChar.written()) {
        parseCommand(cmdChar.value());
    }
}

void COMM_send_telemetry(float angle, float battery) {
    char buffer[50];
    sprintf(buffer, "A:%.1f|V:%.1f", angle, battery);
    telemetChar.writeValue(buffer);
}