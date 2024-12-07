#include "variables.h"

void auton_blueLeft() {
  chassis.setPose(0, 0, 0);
   if (auton == 4) {
      clamp.set_value(HIGH);
      chassis.moveToPoint(0, -20.5, 1000, {.forwards = false, .earlyExitRange = 0.1});
      chassis.waitUntilDone();
      chassis.turnToHeading(-27, 500);
      chassis.waitUntilDone();
      chassis.setPose(0, 0, 0);
      chassis.moveToPoint(0, -12, 1000, {.forwards = false, .maxSpeed = 40, .earlyExitRange = 0.1});
      chassis.waitUntilDone();
      clamp.set_value(LOW);
      intake.move(127);
      pros::delay(2000);
      intake.move(-127);
      pros::delay(1000);
      intake.move(0);

      chassis.setPose(0.00, 0.00, 0.00);
      chassis.turnToHeading(125, 1000);
      intake.move(127);
      chassis.moveToPose(27, 4, 125, 1500, {.maxSpeed = 127, .earlyExitRange = 0.1});
      chassis.waitUntilDone();
      pros::delay(3000);
      intake.move(0);
      chassis.waitUntilDone();
      chassis.turnToHeading(200, 500);
      chassis.waitUntilDone();
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