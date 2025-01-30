#include "variables.h"

void WSP_RedLeft() {
  chassis.setPose(0, 0, 0);
   if (auton == 9) {
	    leftMotors.move(-55);
      rightMotors.move(-55);
      pros::delay(635);
      leftMotors.move(0);
      rightMotors.move(0);
      chassis.turnToHeading(91, 10000, {.maxSpeed = 55});
      chassis.waitUntilDone();
      leftMotors.move(-55);
      rightMotors.move(-55);
      pros::delay(665);
      intake.move(-127);
      pros::delay(100);
      leftMotors.move(0);
      rightMotors.move(0);
      pros::delay(1000);
      intake.move(0);

      chassis.setPose(0, 0, 0);
      chassis.moveToPoint(0, 7, 750, {.earlyExitRange = 0.01});
      chassis.waitUntilDone();
      chassis.setPose(0, 7, 0);
      clamp.set_value(HIGH);
      chassis.moveToPose(-14.5, 50, 135, 3000, {.forwards = false, .maxSpeed = 75, .earlyExitRange = 0.01});
      chassis.waitUntilDone();
      pros::delay(200);
      clamp.set_value(LOW);
      pros::delay(500);
      chassis.setPose(-12.5, 50, 135);

      

      //chassis.turnToHeading(260, 750);
      

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