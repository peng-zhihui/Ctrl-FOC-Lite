#ifndef SENSOR_H
#define SENSOR_H

#include <cinttypes>

/**
 *  Direction structure
 */
enum Direction
{
    CW = 1,  //clockwise
    CCW = -1, // counter clockwise
    UNKNOWN = 0   //not yet known or invalid state
};


/**
 * EncoderBase abstract class defintion
 * 
 * This class is purposefully kept simple, as a base for all kinds of sensors. Currently we have
 * Encoders, Magnetic Encoders and Hall EncoderBase implementations. This base class extracts the
 * most basic common features so that a FOC driver can obtain the data it needs for operation.
 * 
 * To implement your own sensors, create a sub-class of this class, and implement the getSensorAngle()
 * method. getSensorAngle() returns a float value, in radians, representing the current shaft angle in the
 * range 0 to 2*PI (one full turn). 
 * 
 * To function correctly, the sensor class update() method has to be called sufficiently quickly. Normally,
 * the BLDCMotor's loopFOC() function calls it once per iteration, so you must ensure to call loopFOC() quickly
 * enough, both for correct motor and sensor operation.
 * 
 * The EncoderBase base class provides an implementation of getVelocity(), and takes care of counting full
 * revolutions in a precise way, but if you wish you can additionally override these methods to provide more
 * optimal implementations for your hardware.
 * 
 */
class EncoderBase
{
public:
    /**
     * Get mechanical shaft angle in the range 0 to 2PI. This value will be as precise as possible with
     * the hardware. Base implementation uses the values returned by update() so that
     * the same values are returned until update() is called again.
     */
    virtual float getMechanicalAngle();

    /**
     * Get current position (in rad) including full rotations and shaft angle.
     * Base implementation uses the values returned by update() so that the same
     * values are returned until update() is called again.
     * Note that this value has limited precision as the number of rotations increases,
     * because the limited precision of float can't capture the large angle of the full
     * rotations and the small angle of the shaft angle at the same time.
     */
    virtual float GetAngle();

    /**
     * Get current angular velocity (rad/s)
     * Can be overridden in subclasses. Base implementation uses the values
     * returned by update() so that it only makes sense to call this if update()
     * has been called in the meantime.
     */
    virtual float getVelocity();

    /**
     * Get the number of full rotations
     * Base implementation uses the values returned by update() so that the same
     * values are returned until update() is called again.
     */
    virtual int32_t getFullRotations();

    /**
     * Updates the sensor values by reading the hardware sensor.
     * Some implementations may work with interrupts, and not need this.
     * The base implementation calls getSensorAngle(), and updates internal
     * fields for angle, timestamp and full rotations.
     * This method must be called frequently enough to guarantee that full
     * rotations are not "missed" due to infrequent polling.
     * Override in subclasses if alternative behaviours are required for your
     * sensor hardware.
     */
    virtual void update();


protected:
    /**
     * Get current shaft angle from the sensor hardware, and
     * return it as a float in radians, in the range 0 to 2PI.
     *
     * This method is pure virtual and must be implemented in subclasses.
     * Calling this method directly does not update the base-class internal fields.
     * Use update() when calling from outside code.
     */
    virtual float GetSensorAngle() = 0;
    /**
     * Call EncoderBase::init() from your sensor subclass's init method if you want smoother startup
     * The base class init() method calls getSensorAngle() several times to initialize the internal fields
     * to current values, ensuring there is no discontinuity ("jump from zero") during the first calls
     * to sensor.getAngle() and sensor.getVelocity()
     */
    virtual void InitVars();

    // velocity calculation variables
    float angle_prev = 0; // result of last call to getSensorAngle(), used for full rotations and velocity
    uint64_t angle_prev_ts = 0; // timestamp of last call to getAngle, used for velocity
    float vel_angle_prev = 0; // angle at last call to getVelocity, used for velocity
    uint64_t vel_angle_prev_ts = 0; // last velocity calculation timestamp
    int32_t full_rotations = 0; // full rotation tracking
    int32_t vel_full_rotations = 0; // previous full rotation value for velocity calculation
};

#endif
