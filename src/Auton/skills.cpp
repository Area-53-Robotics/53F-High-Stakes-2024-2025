#include "main.h"
#include "pros/adi.h"
#include "variables.h"

void auton_skills() {
  chassis.setPose(0, 0, 0);
      intake.move(-127);
      pros::delay(500);
      intake.move(0);

      pros::delay(100);
      chassis.setPose(0, 0, 0);
      chassis.moveToPoint(0, 13, 1000);
      chassis.waitUntilDone();
      chassis.turnToHeading(90, 750);
      chassis.waitUntilDone();
      clamp.set_value(HIGH);
      chassis.setPose(0, 12, 90);
      chassis.moveToPose(-16, 12, 90, 1000, {.forwards = false, .maxSpeed = 50});
      chassis.waitUntilDone();
      pros::delay(100);
      clamp.set_value(LOW);
      chassis.moveToPose(-24, 12, 90, 1000, {.forwards = false});
      chassis.waitUntilDone();

      chassis.turnToHeading(0, 700);
      chassis.waitUntilDone();
      chassis.setPose(-24, 12, 0);
      chassis.moveToPose(-24, 33, 0, 1000);
      intake.move(127);
      chassis.waitUntilDone();
      pros::delay(1000);
      intake.move(-127);
      pros::delay(200);
      intake.move(0);

      chassis.turnToHeading(268, 700);
      chassis.waitUntilDone();
      chassis.setPose(-24, 33, 270);
      chassis.moveToPose(-45, 33, 270, 1000);
      intake.move(127);
      chassis.waitUntilDone();
      pros::delay(1000);
      intake.move(-127);
      pros::delay(200);
      intake.move(0);
      chassis.turnToHeading(190, 500);
      chassis.waitUntilDone();
      chassis.setPose(-45, 33, 180);
      chassis.moveToPose(-45, 5, 180, 10000, {.maxSpeed = 50, .earlyExitRange = 0.01});
      intake.move(127);
      pros::delay(2000);
      intake.move(-127);
      pros::delay(500);
      intake.move(127);
      pros::delay(2000);
      intake.move(-127);
      pros::delay(500);
      intake.move(127);
      pros::delay(2000);
      intake.move(0);
      chassis.waitUntilDone();
      chassis.turnToHeading(35, 1000);
      chassis.waitUntilDone();
      chassis.setPose(-45, 5, 35);
      chassis.moveToPoint(-52, -2, 1000, {.forwards = false});
      chassis.waitUntilDone();
      clamp.set_value(HIGH);
      chassis.moveToPose(-20, 20, 90, 3000);
      chassis.waitUntilDone();
      chassis.moveToPose(29, 0, 90, 2000, {.forwards = false});

   while (true) {
      lemlib::Pose pose = chassis.getPose();

      // Display raw values on the VEX controller
      controller.set_text(0, 0, "X: ");
      controller.set_text(0, 3, std::to_string(pose.x).c_str());

      controller.set_text(1, 0, "Y: ");
      controller.set_text(1, 3, std::to_string(pose.y).c_str());

      controller.set_text(2, 0, "Theta: ");
      controller.set_text(2, 7, std::to_string(pose.theta).c_str());

      pros::lcd::print(0, "X: %f", pose.x);  // Display X value on line 0
      pros::lcd::print(1, "Y: %f", pose.y);  // Display Y value on line 1
      pros::lcd::print(2, "Theta: %f", pose.theta);  // Display Theta value on line 2
   }
}
