// variables.h
#pragma once

#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "lemlib/api.hpp"

// Function declarations
void on_center_button();
void initialize();

// --- VARIABLES --- //

// -- Motors & Controllers -- //
extern bool reverseDT;
extern bool arcade;
extern bool clampON;

extern pros::Controller controller;

extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

extern pros::Motor intake;

extern pros::adi::DigitalOut clamp;

// -- IMU -- //
extern pros::Imu imu;

// -- Horizontal Tracking Wheel -- //
extern pros::adi::Encoder horizontal_encoder;
extern lemlib::TrackingWheel horizontal_tracking_wheel;

// -- Vertical Tracking Wheel -- //
extern pros::adi::Encoder vertical_encoder;
extern lemlib::TrackingWheel vertical_tracking_wheel;

// -- Lemlib Configuration -- //
extern lemlib::Drivetrain drivetrain;
extern lemlib::OdomSensors sensors;

// --- PID Controllers --- //
// Angular PID controller
extern lemlib::ControllerSettings angular_controller;

// Lateral PID controller
extern lemlib::ControllerSettings lateral_controller;

// --- Drive Curve --- //
// -- Throttle Curve -- //
extern lemlib::ExpoDriveCurve throttle_curve;

// -- Steer Curve -- //
extern lemlib::ExpoDriveCurve steer_curve;

// -- Chassis (Drivetrain for lemlib) -- //
extern lemlib::Chassis chassis;
