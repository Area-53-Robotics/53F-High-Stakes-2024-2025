#include "main.h"
#include "pros/misc.h"
#include "variables.h"

//Testing 1

void disabled() {}

void competition_initialize() {}

void opcontrol() {
	while (true) {

      // -- Getting Tracking Wheel Postion -- //
      pros::lcd::print(0, "ADI Encoder H: %i", horizontal_encoder.get_value());
      pros::lcd::print(1, "ADI Encoder V: %i", vertical_encoder.get_value());

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
     		controller.print(0, 0, "Arcade(B):_OFF__________");
     		chassis.tank(leftY, rightY);
     	}else if (reverseDT == true && arcade == false) {
     		controller.print(0, 0, "Reverse(A):_ON__________");
     		chassis.tank(-rightY, -leftY);
     	}

      // -- Toggle for Arcade Drivetrain and funtion for Arcade Drive -- //
      if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
         arcade = !arcade;
      }

     	if (arcade == true && reverseDT == false) {
			controller.print(0, 0, "Arcade(B):_ON__________");
     		chassis.arcade(rightY, rightX);
     	}else if (arcade == true && reverseDT == true) {
     		controller.print(0, 0, "Rev(A)_&_cade(B):_ON__________");
     		chassis.arcade(-rightY, rightX);
     	}

      // -- Intake funtion -- //
      if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
         intake.move(127);
      }else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
         intake.move(-127);
      }else {
         intake.move(0);
      }

      // -- Clamp -- //
     	if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
     		clampON = !clampON;
     	}

     	if (clampON == true) {
     		clamp.set_value(HIGH);
     	}else {
     		clamp.set_value(LOW);
     	}



      // Delay to save resources
      pros::delay(25);
   }
}