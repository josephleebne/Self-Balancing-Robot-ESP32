#include "config.h"
#include "balancing.h"
#include "communication.h"

struct MotorState
{
    bool moving = false;
};

static MotorState motors;

void motors_init()
{
    pinMode(EN_A, OUTPUT);
    pinMode(EN_B, OUTPUT);
    pinMode(INT_1, OUTPUT);
    pinMode(INT_2, OUTPUT);
    pinMode(INT_3, OUTPUT);
    pinMode(INT_4, OUTPUT);

    ledcSetup(CHANNEL_0, PWM_FREQUENCY, PWM_RESOLUTION); // Channel 0
    ledcSetup(CHANNEL_1, PWM_FREQUENCY, PWM_RESOLUTION); // Channel 1

    ledcAttachPin(EN_A, 0); 
    ledcAttachPin(EN_B, 1);
}

int sign(float effort)
{
    if (effort > 0) return 1;
    else if (effort < 0) return -1;
    else return 0;
}

float effort_to_speed(float effort)
{
    float magnitude = abs(effort);
    
    if (magnitude < 0.1f) return 0; 

    float speed = MIN_SPEED + (magnitude * (float)(MAX_SPEED - MIN_SPEED) / MAX_EFFORT);

    if (speed > MAX_SPEED) speed = MAX_SPEED;
    
    return speed;
}

void set_left_direction_forward() {
    digitalWrite(INT_1, HIGH);
    digitalWrite(INT_2, LOW);
}

void set_left_direction_backward() {
    digitalWrite(INT_1, LOW);
    digitalWrite(INT_2, HIGH);
}

void set_right_direction_forward() {
    digitalWrite(INT_3, LOW);
    digitalWrite(INT_4, HIGH);
}

void set_right_direction_backward() {
    digitalWrite(INT_3, HIGH);
    digitalWrite(INT_4, LOW);
}

void motors_stop()
{
    digitalWrite(INT_1, LOW);
    digitalWrite(INT_2, LOW);
    digitalWrite(INT_3, LOW);
    digitalWrite(INT_4, LOW);
    ledcWrite(CHANNEL_0, 0);
    ledcWrite(CHANNEL_1, 0);
}

void motors_update(float effort) {
    if (abs(get_IMU_angle_y()) > FAIL_ANGLE){
        motors_stop(); 
        return;
    }

    // Apply turning offset to the raw effort
    float leftEffort = effort + targetTurn;
    float rightEffort = effort - targetTurn;

    if (abs(leftEffort) < NEUTRAL_EFFORT && abs(rightEffort) < NEUTRAL_EFFORT) {
        motors_stop();
        return;
    }

    float leftSpeed = effort_to_speed(leftEffort);
    float rightSpeed = effort_to_speed(rightEffort);
    
    // Set Left Motor Direction
    if (leftEffort > 0) {
        set_left_direction_forward(); 
    } else {
        set_left_direction_backward();
    }

    // Set Right Motor Direction
    if (rightEffort > 0) {
        set_right_direction_forward();
    } else {
        set_right_direction_backward();
    }

    // Write PWM
    ledcWrite(CHANNEL_0, (int)(RIGHT_MOTOR_SLOW_FACTOR * rightSpeed));
    ledcWrite(CHANNEL_1, (int)leftSpeed);
}