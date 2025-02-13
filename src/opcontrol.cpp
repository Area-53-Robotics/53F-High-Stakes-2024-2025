#include "pros/misc.h"
#include "variables.h"

// Open Control (Driver Control)
void opcontrol() {
  clamp.set_value(HIGH);
  while (true) {

    // -- Getting Tracking Wheel Postion -- //
    lemlib::Pose pose = chassis.getPose();

    // Display raw values on the VEX controller
    pros::lcd::print(0, "X: %f", pose.x); // Display X value on line 0
    pros::lcd::print(1, "Y: %f", pose.y); // Display Y value on line 1
    pros::lcd::print(2, "Theta: %f", pose.theta); // Display Theta value on line 3

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
    } else if (reverseDT == true && arcade == false) {
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
    } else if (arcade == true && reverseDT == true) {
      controller.print(1, 0, "Rev(A)&Cade(B):ON__________");
      pros::lcd::print(4, "/ Arcade Drive and Reverse Drive ON /");
      chassis.arcade(-rightY, rightX);
    }

    // -- Intake function (Hold) -- //
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      intake.move(127);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      intake.move(-127);
    } else {
      intake.move(0);
    }

    // -- Intake function (Toggle) -- //
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
    } else {
      clamp.set_value(LOW);
      pros::lcd::print(5, "/ Clamp Deactivated /");
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      hangON = !hangON;
    }

    if (hangON == true) {
      hang.set_value(HIGH);
      pros::lcd::print(6, "/ Hang Activated /");
    } else {
      hang.set_value(LOW);
      pros::lcd::print(6, "/ Hang Deactivated /");
    }

    // Delay to save resources
    pros::delay(25);
  }
}
