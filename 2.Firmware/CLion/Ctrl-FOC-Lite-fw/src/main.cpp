#include <Arduino.h>
#include <SimpleFOC.h>

// Motor instance
BLDCMotor motor = BLDCMotor(14);
BLDCDriver3PWM driver = BLDCDriver3PWM(PB_4, PB_5, PB_0, PB_12);
Encoder encoder = Encoder(PA_8, PA_9, 4096);

// Interrupt routine intialisation
// channel A and B callbacks
void doA()
{ encoder.handleA(); }

void doB()
{ encoder.handleB(); }

void doI()
{ encoder.handleIndex(); }


// angle set point variable
float target_angle = 0;
// instantiate the commander
Commander command = Commander(Serial);

void doTarget(char *cmd)
{ command.scalar(&target_angle, cmd); }


void setup()
{

    // initialize encoder sensor hardware
    encoder.init();
    encoder.enableInterrupts(doA, doB, doI);
    // link the motor to the sensor
    motor.linkSensor(&encoder);

    // driver config
    // power supply voltage [V]
    driver.voltage_power_supply = 12;
    driver.init();
    // link the motor and the driver
    motor.linkDriver(&driver);

    // aligning voltage [V]
    motor.voltage_sensor_align = 3;
    // index search velocity [rad/s]
    motor.velocity_index_search = 3;

    // set motion control loop to be used
    motor.controller = MotionControlType::velocity;

    // contoller configuration
    // default parameters in defaults.h

    // velocity PI controller parameters
    motor.PID_velocity.P = 0.1;
    motor.PID_velocity.I = 20;
    // default voltage_power_supply
    motor.voltage_limit = 1;
    // jerk control using voltage voltage ramp
    // default value is 300 volts per sec  ~ 0.3V per millisecond
    motor.PID_velocity.output_ramp = 1000;

    // velocity low pass filtering time constant
    motor.LPF_velocity.Tf = 0.01;

    // angle P controller
    motor.P_angle.P = 20;
    //  maximal velocity of the position control
    motor.velocity_limit = 4;


    // use monitoring with serial
    Serial.begin(115200);
    // comment out if not needed
    motor.useMonitoring(Serial);

    // initialize motor
    motor.init();
    // align encoder and start FOC
    motor.initFOC();

    // add target command T
    command.add('T', doTarget, "target angle");

    Serial.println("Motor ready.");
    Serial.println("Set the target angle using serial terminal:");
    _delay(1000);
}

void loop()
{
    // main FOC algorithm function
    // the faster you run this function the better
    // Arduino UNO loop  ~1kHz
    // Bluepill loop ~10kHz
    motor.loopFOC();

    // Motion control function
    // velocity, position or voltage (defined in motor.controller)
    // this function can be run at much lower frequency than loopFOC() function
    // You can also use motor.move() and set the motor.target in the code
    motor.move(target_angle);

    // function intended to be used with serial plotter to monitor motor variables
    // significantly slowing the execution down!!!!
    // motor.monitor();

    // user communication
    command.run();
}