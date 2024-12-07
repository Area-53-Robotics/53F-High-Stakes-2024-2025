#include "main.h"
#include "variables.h"

void auton_blueGold() {
  chassis.setPose(0, 0, 0);
   if (auton == 6) {
      leftMotors.move(-25);
      rightMotors.move(-25);
      pros::delay(1650);
      leftMotors.move(0);
      rightMotors.move(0);
      chassis.turnToHeading(90, 10000, {.maxSpeed = 25});
      chassis.waitUntilDone();
      leftMotors.move(-25);
      rightMotors.move(-25);
      pros::delay(750);
      leftMotors.move(0);
      rightMotors.move(0);
      intake.move(-127);
      pros::delay(1000);
      intake.move(0);
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