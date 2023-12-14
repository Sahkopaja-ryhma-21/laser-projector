//Parameters identify various variables or functions in the system.
enum Parameter : uint8_t {
    //The following parameters can be loaded or stored as configuration data
    PARAM_CURRENT_GAIN_P    =  0, //Coil current PID controller - proportional term gain
    PARAM_CURRENT_GAIN_I    =  1, //Coil current PID controller - integral term gain
    PARAM_CURRENT_GAIN_D    =  2, //Coil current PID controller - derivative term gain
    PARAM_VELOCITY_GAIN_P   =  3, //Angular velocity PID controller - proportional term gain
    PARAM_VELOCITY_GAIN_I   =  4, //Angular velocity PID controller - integral term gain
    PARAM_VELOCITY_GAIN_D   =  5, //Angular velocity PID controller - derivative term gain
    PARAM_ANGLE_GAIN_P      =  6, //Angle PID controller - proportional term gain
    PARAM_ANGLE_GAIN_I      =  7, //Angle PID controller - integral term gain
    PARAM_ANGLE_GAIN_D      =  8, //Angle PID controller - derivative term gain
    PARAM_ANGLE_CENTER      =  9, //Center position of projected image
    PARAM_ANGLE_SCALE       = 10, //How big the projected image is around the center position
    PARAM_INVERTED          = 11, //If true (non-zero), flips the sign of all setpoints (angle,
                                  //..angular velocity and current) and all actual values.

    //The following parameters are never loaded or stored as part of a configuration
    PARAM_VOLTAGE_OUTPUT    = 12, //Output voltage. Read only if init state > INIT_INPUT_CAL
    PARAM_CURRENT_SETPOINT  = 13, //Desired velocity. Read only if init state > INIT_CURRENT_CTRL
    PARAM_VELOCITY_SETPOINT = 14, //Desired velocity. Read only if init state > INIT_VELOCITY_CTRL
    PARAM_ANGLE_SETPOINT    = 15, //Desired angle. Read/write. 
    PARAM_CURRENT_ACTUAL    = 16, //Actual value of current. Read only.
    PARAM_VELOCITY_ACTUAL   = 17, //Actual value of angular velocity. Read only.
    PARAM_ANGLE_ACTUAL      = 18, //Actual value of angle. Read only.

    //... reserved for future use

    PARAM_STORE_CONFIG      = 29, //Writing to this stores configuration. The value written 
                                  //..determines which slot is overwritten. 
    PARAM_LOAD_CONFIG       = 30, //Writing to this loads configuration. The value written
                                  //..determines which slot is read.
    PARAM_INIT_STATE        = 31  //Set the initialization state of the system. See enum InitState
                                  //..and setInitState() for details.
};

//InitState defines the behavior of setInitState, which can be used to turn off or turn on the
//..system in granular fashion.
enum InitState : uint8_t {
    INIT_OFF            = 0, //Disable everything. Coil drivers off.
    INIT_OUTPUT_ON      = 1, //Initialize controllers and hardware peripherals. Coil drivers on, start ADC.
    INIT_INPUT_CAL      = 2, //Perform calibration procedures.
    INIT_CURRENT_CTRL   = 3, //Enable closed loop current control.
    INIT_VELOCITY_CTRL  = 4, //Enable closed loop angular velocity control.
    INIT_ANGLE_CTRL     = 5  //Enable closed loop angle control. System fully active.
};

//There are four configuration slots that can be read from or written to on the fly using storeConfig
//..or loadConfig
enum ConfigSlot : uint8_t {
    CONFIG_DEFAULT      = 0, //default configuration; cannot be written to
    CONFIG_SLOT_1       = 1, 
    CONFIG_SLOT_2       = 2,
    CONFIG_SLOT_3       = 3,
    CONFIG_SLOT_4       = 4
};