#include "variables.h"
#include "main.h"
#include "pros/adi.h"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "pros/rtos.hpp"
#include "lemlib/api.hpp"
#include <algorithm>
#include <cstddef>
#include <optional>
#include <string>

void disabled() {}

void competition_initialize() {}

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

// -- Auton -- //
int auton = 7;
/*
            1 = Blue Auto WP - Ladder
            2 = Blue Auto WP - Ring Rush
            3 = Blue GoalRush
            4 = Blue Right
            5 = Red Auto WP - Ladder
            6 = Red Auto WP - Ring Rush
            7 = Red GoalRush
            8 = Red Left
            9 = Skills
*/

void autonomous() {
   if (auton == 1) {
    Blue_AWP_L();
   }else if (auton == 2) {
    Blue_AWP_RR();
   }else if (auton == 3) {
    auton_blueGoal();
   }else if (auton == 4) {
    auton_blueRight();
   }else if (auton == 5) {
    Red_AWP_L();
   }else if (auton == 6) {
    Red_AWP_RR();
   }else if (auton == 7) {
    auton_redGoal();
   }else if (auton == 8) {
    auton_redLeft();
   }else if (auton == 9) {
    auton_skills();
   }
}

// --- VARIABLES --- //
// -- Motors & Controllers -- //
bool reverseDT = false;
bool arcade = false;
bool intakeToggle = false;
bool outtakeToggle = false;
int stopIntake = 0;

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::MotorGroup leftMotors({-6, 7, -11}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({20, 4, -5}, pros::MotorGearset::blue);

pros::MotorGroup intake({18, -15});

bool clampON = false;
pros::adi::DigitalOut clamp ('C');
bool wallON = false;
pros::adi::DigitalOut wall ('D'/* As in Diddy -Josh */);
bool armON = false;
pros::adi::DigitalOut arm ('E');

// -- Imu -- //
pros::Imu imu(10);
// -- Horizontal Tracking Wheel -- //
pros::adi::Encoder horizontal_encoder('A', 'B');
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_275, -1.825000);

// -- Vertical Tracking Wheel -- //
pros::adi::Encoder vertical_encoder('G', 'H');
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, -4.390180);


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
// angular PID controller ()
lemlib::ControllerSettings angular_controller(5, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              26, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              50, // small error range timeout, in milliseconds
                                              1.5, // large error range, in inches
                                              250, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(16, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              50, // small error range timeout, in milliseconds
                                              1.5, // large error range, in inches
                                              250, // large error range timeout, in milliseconds
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
   chassis.calibrate();
   controller.clear();
}






































