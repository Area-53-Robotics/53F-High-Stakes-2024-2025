#include "main.h"
#include "lemlib/api.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

//-------------------------VARIABLES---------------------------//
bool reverseDT = false;
bool arcade = false;
bool clampON = false;

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::MotorGroup leftMotors({11, -12, 20}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({-1, 2, -10}, pros::MotorGearset::blue);

pros::Imu imu(17);

pros::adi::Encoder horizontal_encoder('A', 'B');
pros::adi::Encoder vertical_encoder('G', 'H');

pros::adi::DigitalOut clamp ('C');
//-------------------------VARIABLES---------------------------//
//----------------------------PID------------------------------//
// horizontal tracking wheel
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_275, 1.75);
// vertical tracking wheel
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, 4.75);

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

// angular PID controller
lemlib::ControllerSettings angular_controller(3, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in degrees
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in degrees
                                              0, // large error range timeout, in milliseconds
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

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     5, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                  5, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors, // odometry sensors
                        &throttle_curve, 
                        &steer_curve
);
//----------------------------PID------------------------------//


void initialize() {
	pros::lcd::initialize();
	pros::lcd::print(3, "//----- 53F -----/");
   chassis.calibrate();
	/*while (true) { // infinite loop
        // print measurements from the adi encoder
        pros::lcd::print(0, "ADI Encoder H: %i", horizontal_encoder.get_value());
        pros::lcd::print(1, "ADI Encoder V: %i", vertical_encoder.get_value());
        // print measurements from the rotation sensor
        pros::delay(10); // delay to save resources. DO NOT REMOVE
    }*/
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

//--------------------------------------------------------------------------------------//

void autonomous() {
	// set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(90, 100000);
    //lemlib::Pose poseVal = chassis.getPose(false);
    //printf("Theta: %f\n", poseVal.theta);

}

void opcontrol() {
	while (true) {


      // get left y and right y positions
      int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
      int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
   //---------------------------------------------------------------------------//
      int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        
		pros::lcd::print(0, "ADI Encoder H: %i", horizontal_encoder.get_value());
      pros::lcd::print(1, "ADI Encoder V: %i", vertical_encoder.get_value());

      if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
     		reverseDT = !reverseDT;
     	}

     	if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
     		arcade = !arcade;
     	}

     	if (reverseDT == false && arcade == false) {
     		controller.print(0, 0, "Arcade(B):_OFF__________");
     		chassis.tank(leftY, rightY);
     	}else if (reverseDT == true && arcade == false) {
     		controller.print(0, 0, "Reverse(A):_ON__________");
     		chassis.tank(-leftY, -rightY);
     	}

     	if (arcade == true && reverseDT == false) {
			controller.print(0, 0, "Arcade(B):_ON__________");
     		chassis.arcade(rightY, rightX);
     	}else if (arcade == true && reverseDT == true) {
     		controller.print(0, 0, "Rev(A)_&_cade(B):_ON__________");
     		chassis.arcade(-rightY, rightX);
     	}

     	if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
     		clampON = !clampON;
     	}

     	if (clampON == true) {
     		clamp.set_value(HIGH);
     	}else {
     		clamp.set_value(LOW);
     	}



      // delay to save resources
      pros::delay(25);
   }
}