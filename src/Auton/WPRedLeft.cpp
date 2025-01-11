#include "variables.h"

void WPRedLeft() {
  chassis.setPose(0, 0, 0);
   if (auton == 9) {
	    leftMotors.move(-50);
      rightMotors.move(-50);
      pros::delay(640);
      leftMotors.move(0);
      rightMotors.move(0);
      chassis.turnToHeading(91, 10000, {.maxSpeed = 50});
      chassis.waitUntilDone();
      leftMotors.move(-25);
      rightMotors.move(-25);
      pros::delay(650);
      intake.move(-127);
      pros::delay(100);
      leftMotors.move(0);
      rightMotors.move(0);
      pros::delay(1000);
      intake.move(0);

      chassis.setPose(0, 0, 0);
      chassis.moveToPoint(0, 7, 750, {.earlyExitRange = 0.01});
      chassis.waitUntilDone();
      chassis.turnToHeading(150, 1000);
      chassis.waitUntilDone();
      chassis.setPose(0, 7, 150);
      clamp.set_value(HIGH);
      chassis.moveToPose(-22.5, 35, 149, 1500, {.forwards = false, .maxSpeed = 75, .earlyExitRange = 0.01});
      chassis.waitUntilDone();
      pros::delay(200);
      clamp.set_value(LOW);
      pros::delay(500);

      chassis.moveToPose(-22.5, 37, 150, 750, {.forwards = false, .earlyExitRange = 0.01});
      chassis.waitUntilDone();
      chassis.turnToHeading(268, 750);
      chassis.waitUntilDone();
      chassis.setPose(-22.5, 37, 270);
      intake.move(127);
      chassis.moveToPose(-33, 37, 273, 10000, {.minSpeed = 115, .earlyExitRange = 0.1});
      chassis.waitUntilDone();
      pros::delay(1750);
      intake.move(-100);
      pros::delay(250);
      intake.move(0);

      chassis.turnToHeading(90, 750);
      chassis.waitUntilDone();
      chassis.setPose(-33, 37, 90);
      chassis.moveToPose(-10, 37, 90, 1000, {.maxSpeed = 127, .earlyExitRange = 2});
      chassis.waitUntilDone();
      chassis.moveToPose(0, 37, 90, 5000, {.maxSpeed = 50});

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