#include "balancing.h"
#include "communication.h"
#include "config.h"

// Function declarations
float get_IMU_angle_y();
void IMU_update();


MPU6050 mpu(Wire);
bool imuOk = false;

PIDController balancePID;

void PIDController::reset() {
    kp = INITIAL_KP; 
    ki = INITIAL_KI;
    kd = INITIAL_KD;
    setPoint = SET_POINT;

    IMU_update(); 
    errorSum = 0.0f;

    float currentAngle = get_IMU_angle_y();
    lastError = setPoint - currentAngle;

    effort = 0.0f;

    // Fixed double assignment typo
    lastCalculationTime = millis();
    
    Serial.println("PID Controller Reset. Gains Loaded:");
    Serial.print("P: "); Serial.print(kp);
    Serial.print(" I: "); Serial.print(ki);
    Serial.print(" D: "); Serial.println(kd);
}

bool IMU_init()
{
    Wire.begin(SDA_PIN, SCL_PIN);
    byte status = mpu.begin();
    if (status != 0)
    {
        imuOk = false;
        return imuOk;
    }
    imuOk = true;
    mpu.calcOffsets(true, true);
    mpu.setFilterGyroCoef(GYRO_GAIN);
    
    Serial.println("Calibrating IMU...");
    for(int i = 0; i < 200; i++) {
        mpu.update();
        delay(10);
    }
    balancePID.reset();
    Serial.println("IMU is Ready.");
    return imuOk;
}

float get_IMU_angle_x() { return mpu.getAngleX(); }
float get_IMU_angle_y() { return mpu.getAngleY(); }
float get_IMU_angle_z() { return mpu.getAngleZ(); }

void IMU_update()
{
    if (imuOk) mpu.update();
}

float calculate_effort() {
    unsigned long now = millis();
    float dt = (now - balancePID.lastCalculationTime) / 1000.0f;

    if (abs(get_IMU_angle_y()) > FAIL_ANGLE) {
        balancePID.errorSum = 0;
        balancePID.effort = 0;
        return 0;
    }

    if (dt >= PID_RATE_SECONDS) { 
        balancePID.lastCalculationTime = now; 
        
        IMU_update();
        float currentAngle = get_IMU_angle_y();
        
        // Incorporate targetVelocity into the error calculation
        float error = (balancePID.setPoint + targetVelocity) - currentAngle;
        
        balancePID.errorSum += (error * dt);
        balancePID.errorSum = constrain(balancePID.errorSum, -20.0f, 20.0f);

        static float lastFilteredD = 0;
        float rawD = (error - balancePID.lastError) / dt;
        float filteredD = (0.5f * lastFilteredD) + (0.5f * rawD);
        lastFilteredD = filteredD;

        float pTerm = error * balancePID.kp;
        float iTerm = balancePID.errorSum * balancePID.ki;
        float dTerm = filteredD * balancePID.kd;

        balancePID.effort = pTerm + iTerm + dTerm;
        balancePID.effort = constrain(balancePID.effort, MIN_EFFORT, MAX_EFFORT);
        
        balancePID.lastError = error;
    }

    return balancePID.effort;
}

void print_effort() {
    unsigned long now = millis();
    static unsigned long lastPrint = 0;
    
    if ((now - lastPrint) >= LOG_DELAY) {
        float currentAngle = get_IMU_angle_y();
        float error = balancePID.setPoint - currentAngle;
        
        float pTerm = error * balancePID.kp;
        float iTerm = balancePID.errorSum * balancePID.ki;
        float dTerm = balancePID.effort - (pTerm + iTerm);

        Serial.print("Angle: ");  Serial.print(currentAngle, 2);
        Serial.print(" | Error: "); Serial.print(error, 2);
        Serial.print(" | Effort Required: "); Serial.println(balancePID.effort, 2);
        
        lastPrint = now;
    }
}