#ifndef VARIABLES_H
#define VARIABLES_H

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

// Function prototypes
void disabled();
void competition_initialize();
void on_center_button();

// Auton
extern int auton;

/* 1 */ void Blue_AWP_L();
/* 2 */ void Blue_AWP_RR();
/* 3 */ void auton_blueGoal();
/* 4 */ void auton_blueRight();
/* 5 */ void Red_AWP_L();
/* 6 */ void Red_AWP_RR();
/* 7 */ void auton_redGoal();
/* 8 */ void auton_redLeft();
/* 9 */ void auton_skills();


// Variables
extern bool reverseDT;
extern bool arcade;
extern bool clampON;
extern bool wallON;
extern bool armON;
extern bool intakeToggle;
extern bool outtakeToggle;
extern int stopIntake;

extern pros::Controller controller;
extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;
extern pros::MotorGroup intake;
extern pros::adi::DigitalOut clamp;
extern pros::adi::DigitalOut wall;
extern pros::adi::DigitalOut arm;

extern pros::Imu imu;
extern pros::adi::Encoder horizontal_encoder;
extern pros::adi::Encoder vertical_encoder;

extern lemlib::TrackingWheel horizontal_tracking_wheel;
extern lemlib::TrackingWheel vertical_tracking_wheel;
extern lemlib::Drivetrain drivetrain;
extern lemlib::OdomSensors sensors;

extern lemlib::ControllerSettings angular_controller;
extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ExpoDriveCurve throttle_curve;
extern lemlib::ExpoDriveCurve steer_curve;
extern lemlib::Chassis chassis;

#endif // MY_ROBOT_H
