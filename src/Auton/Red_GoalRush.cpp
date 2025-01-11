#include "main.h"
#include "pros/device.hpp"
#include "pros/rtos.hpp"
#include "variables.h"

void auton_redGoal() {
  chassis.setPose(0, 0, 0);
   if (auton == 4) {
      clamp.set_value(HIGH);
      chassis.moveToPoint(0, -16, 1000, {.forwards = false, .earlyExitRange = 0.01});
      chassis.waitUntilDone();
      chassis.turnToHeading(30, 500);
      chassis.waitUntilDone();
      chassis.setPose(0, -16, 30);
      chassis.moveToPose(-10, -29, 30, 1000, {.forwards = false, .maxSpeed = 40});
      chassis.waitUntilDone();
      clamp.set_value(LOW);
      pros::delay(100);
      intake.move(127);
      pros::delay(1000);
      intake.move(-127);
      pros::delay(500);
      intake.move(0);
      chassis.turnToHeading(270, 750);
      chassis.waitUntilDone();
      chassis.setPose(-10, -29, 270);
      clamp.set_value(HIGH);
      chassis.moveToPose(-26, -29, 270, 500);
      intake.move(127);
      pros::delay(1250);
      intake.move(0);
      chassis.moveToPose(-20, -29, 270, 1000, {.forwards = false});
      chassis.waitUntilDone();
      chassis.turnToHeading(30, 750);
      chassis.waitUntilDone();
      chassis.setPose(-20, -29, 30);
      chassis.moveToPose(-27, -42, 30, 1000, {.forwards = false});
      chassis.waitUntilDone();
      clamp.set_value(HIGH);
      pros::delay(100);
      intake.move(127);
      pros::delay(1000);
      intake.move(-127);
      pros::delay(500);
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