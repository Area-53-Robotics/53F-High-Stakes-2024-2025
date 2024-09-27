#include "main.h"
#include "variables.h"

// -- No idea what it's for but pros needs this --//
void on_center_button() {
   static bool pressed = false;
   pressed = !pressed;
   if (pressed) {
      pros::lcd::set_text(2, "I was pressed!");
   } else {
      pros::lcd::clear_line(2);
   }
}

// --- VARIABLES --- //
// -- Motors & Controllers -- //
bool reverseDT = false;
bool arcade = false;
bool clampON = false;

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::MotorGroup leftMotors({11, -12, 20}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({-1, 2, -10}, pros::MotorGearset::blue);

pros::Motor intake(16);

pros::adi::DigitalOut clamp ('C');

// -- Imu -- //
pros::Imu imu(17);
// -- Horizontal Tracking Wheel -- //
pros::adi::Encoder horizontal_encoder('A', 'B');
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_275, 1.75);

// -- Vertical Tracking Wheel -- //
pros::adi::Encoder vertical_encoder('G', 'H');
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, 4.75);


// -- Lemlib Configuration -- //
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              13.5, // 10 inch track width
                              lemlib::Omniwheel::NEW_325,
                              360, // drivetrain rpm is 360
                              2 // horizontal drift is 2 (for now)
);
lemlib::OdomSensors sensors(&vertical_tracking_wheel, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontal_tracking_wheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);


// --- Pain Insanity Disaster (PID) ---//
// angular PID controller
lemlib::ControllerSettings angular_controller(6, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              46, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// --- I think this is drive curve... idk --- //
// -- Throttle Curve -- //
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     5, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// -- Steer Curve -- //
lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                  5, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// -- Chassis (Drivetrain for lemlib) -- //
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors, // odometry sensors
                        &throttle_curve, 
                        &steer_curve
);


// -- Initialize -- //
void initialize() {
	pros::lcd::initialize();
	pros::lcd::print(3, "//----- 53F -----/");
    chassis.calibrate();
}