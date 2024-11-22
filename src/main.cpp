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

// --- VARIABLES --- //
// -- Motors & Controllers -- //
bool reverseDT = false;
bool arcade = false;
bool clampON = false;
bool intakeToggle = false;
bool outtakeToggle = false;
int stopIntake = 0; 

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::MotorGroup leftMotors({-6, 7, -11}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({20, 4, -5}, pros::MotorGearset::blue);

pros::MotorGroup intake({18, -15});

pros::adi::DigitalOut clamp ('C');


// -- Imu -- //
pros::Imu imu(10);
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
lemlib::ControllerSettings angular_controller(5, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              26, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(15, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              30, // derivative gain (kD)
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
   chassis.calibrate();
   pros::lcd::print(3, "//----- Initialize Complete -----//");
}

//ASSET(testing_txt);

void autonomous() {
   clamp.set_value(HIGH);
   int auton = 2;
   chassis.setPose(0, 0, 0);
   if (auton == 1 /*Right side*/) {
   chassis.moveToPoint(0, -20.5, 1000, {.forwards = false});
   chassis.waitUntilDone();
   chassis.turnToHeading(27, 500);
   chassis.waitUntilDone();
   chassis.setPose(0, 0, 0);
   chassis.moveToPoint(0, -12, 2000, {.forwards = false, .maxSpeed = 40, .earlyExitRange = 0.1});
   chassis.waitUntilDone();
   clamp.set_value(LOW);
   intake.move(127);
   pros::delay(1500);
   intake.move(-127);
   pros::delay(500);
   intake.move(0);
   chassis.setPose(0.00, 0.00, 0.00);
   chassis.turnToHeading(-125, 1000);
   intake.move(127);
   chassis.moveToPose(0, -15, -135, 2000);
   chassis.moveToPose(12, 10, 125, 1000, {.forwards = false, .maxSpeed = 127});
   pros::delay(4000);
   intake.move(-127);
   pros::delay(500);
   intake.move(0);
}else if (auton == 2 /*left Side*/) {
   chassis.moveToPoint(0, -20.5, 1000, {.forwards = false, .earlyExitRange = 0.1});
   chassis.waitUntilDone();
   chassis.turnToHeading(-27, 500);
   chassis.waitUntilDone();
   chassis.setPose(0, 0, 0);
   chassis.moveToPoint(0, -12, 3000, {.forwards = false, .maxSpeed = 40, .earlyExitRange = 0.1});
   chassis.waitUntilDone();
   clamp.set_value(LOW);
   intake.move(127);
   pros::delay(1500);
   intake.move(-127);
   pros::delay(500);
   intake.move(0);
   chassis.setPose(0.00, 0.00, 0.00);
   chassis.turnToHeading(130, 1000);
   intake.move(127);
   chassis.moveToPose(27, 4, 125, 5000, {.maxSpeed = 127, .earlyExitRange = 0.1});
   chassis.moveToPose(12, 10, 125, 1000, {.forwards = false, .maxSpeed = 127});
   pros::delay(4000);
   intake.move(-127);
   pros::delay(500);
   intake.move(0);
   //chassis.moveToPose(18, 7, 115, 20000, {.forwards = false});
   //intake.move(127);
   
}else if (auton == 3){
   clamp.set_value(HIGH);
}
   while (true) {
      lemlib::Pose pose = chassis.getPose();

      // Display raw values on the VEX controller
      controller.set_text(0, 0, "X: ");
      controller.set_text(0, 3, std::to_string(pose.x).c_str());

      controller.set_text(1, 0, "Y: ");
      controller.set_text(1, 3, std::to_string(pose.y).c_str());

      controller.set_text(2, 0, "Theta: ");
      controller.set_text(2, 7, std::to_string(pose.theta).c_str());

      /*pros::lcd::print(0, "X: %f", pose.x);  // Display X value on line 0
      pros::lcd::print(1, "Y: %f", pose.y);  // Display Y value on line 1
      pros::lcd::print(2, "Theta: %f", pose.theta);  // Display Theta value on line 2*/
   }
}

//Open Control (Driver Control)
void opcontrol() {
   clamp.set_value(HIGH);
	while (true) {

      // -- Getting Tracking Wheel Postion -- //
      lemlib::Pose pose = chassis.getPose();

      // Display raw values on the VEX controller
      pros::lcd::print(0, "X: %f", pose.x);  // Display X value on line 0
      pros::lcd::print(1, "Y: %f", pose.y);  // Display Y value on line 1
      pros::lcd::print(2, "Theta: %f", pose.theta);  // Display Theta value on line 2

      // -- Getting Y position for Left and Right joysticks -- //
      int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
      int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
      // -- Getting X position for Right joystick (Arcade Drive) -- //
      int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

      // -- Toggle for Reverse Drivetrain and funtion for Drivetrain -- //
      if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
         reverseDT = !reverseDT;
      }

     	if (reverseDT == false && arcade == false) {
         controller.print(1, 0, "Reverse(A):_OFF_________");
         pros::lcd::print(4, "/ Arcade Drive and Reverse Drive OFF /");
     		chassis.tank(leftY, rightY);
     	}else if (reverseDT == true && arcade == false) {
     		controller.print(1, 0, "Reverse(A):_ON__________");
         pros::lcd::print(4, "/ Reverse DriveTrain ON /");
     		chassis.tank(-rightY, -leftY);
     	}

      // -- Toggle for Arcade Drivetrain and funtion for Arcade Drive -- //
      if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
         arcade = !arcade;
      }

     	if (arcade == true && reverseDT == false) {
			controller.print(1, 0, "Arcade(B):_ON__________");
         pros::lcd::print(4, "/ Arcade Drive ON /");
     		chassis.arcade(rightY, rightX);
     	}else if (arcade == true && reverseDT == true) {
     		controller.print(1, 0, "Rev(A)&Cade(B):ON__________");
         pros::lcd::print(4, "/ Arcade Drive and Reverse Drive ON /");
     		chassis.arcade(-rightY, rightX);
     	}

      // -- Intake funtion -- //

      if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
         intake.move(127);
      }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
         intake.move(-127);
      }else {
         intake.move(0);
      }

      /*if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
         //intakeToggle = true;
         //outtakeToggle = false;
         stopIntake = (stopIntake + 1);
         pros::lcd::print(5, "/ Intaking /");
      }else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
         //intakeToggle = false;
         //outtakeToggle = true;
         stopIntake = (stopIntake - 1);
         pros::lcd::print(5, "/ Outtaking /");
      }else {
         intake.move(0);
      }

      if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
         stopIntake = 0;
      }

      if(stopIntake == 1){
         intake.move(127);
         std::cout << stopIntake;
      }else if(stopIntake == -1){
         intake.move(-127);
         std::cout << stopIntake;
      }else{
         //intakeToggle = false;
         //outtakeToggle = false;
         stopIntake = 0;
         intake.move(0);
         std::cout << stopIntake;
      }*/


      // -- Clamp -- //
     	if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
     		clampON = !clampON;
     	}

     	if (clampON == true) {
     		clamp.set_value(HIGH);
         pros::lcd::print(5, "/ Clamp Activated /");
     	}else {
     		clamp.set_value(LOW);
         pros::lcd::print(5, "/ Clamp Deactivated /");
     	}
    
      // Delay to save resources
      pros::delay(25);
   }//*/
}