#include "encoder_base.h"
#include "Ctrl/Utils/foc_utils.h"

inline float abs(float _v)
{
    return _v >= 0 ? _v : -_v;
}

void EncoderBase::update()
{
    float val = GetSensorAngle();
    angle_prev_ts = micros();
    float d_angle = val - angle_prev;
    // if overflow happened track it as full rotation
    if (abs(d_angle) > (0.8f * _2PI))
        full_rotations += (d_angle > 0) ? -1 : 1;
    angle_prev = val;
}


/** get current angular velocity (rad/s) */
float EncoderBase::getVelocity()
{
    // calculate sample time
    float Ts = (float) (angle_prev_ts - vel_angle_prev_ts) * 1e-6f;
    // quick fix for strange cases (micros overflow)
    if (Ts <= 0) Ts = 1e-3f;
    // velocity calculation
    float vel = ((float) (full_rotations - vel_full_rotations) * _2PI + (angle_prev - vel_angle_prev)) / Ts;
    // save variables for future pass
    vel_angle_prev = angle_prev;
    vel_full_rotations = full_rotations;
    vel_angle_prev_ts = angle_prev_ts;
    return vel;
}

void EncoderBase::InitVars()
{
    // initialize all the internal variables of EncoderBase to ensure a "smooth" startup (without a 'jump' from zero)
    GetSensorAngle(); // call once
    delayMicroseconds(1);
    vel_angle_prev = GetSensorAngle(); // call again
    vel_angle_prev_ts = micros();
    delay(1);
    GetSensorAngle(); // call once
    delayMicroseconds(1);
    angle_prev = GetSensorAngle(); // call again
    angle_prev_ts = micros();
}


float EncoderBase::getMechanicalAngle()
{
    return angle_prev;
}


float EncoderBase::GetAngle()
{
    return (float) full_rotations * _2PI + angle_prev;
}


int32_t EncoderBase::getFullRotations()
{
    return full_rotations;
}
