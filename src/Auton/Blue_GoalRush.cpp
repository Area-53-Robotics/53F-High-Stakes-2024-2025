#include "main.h"
#include "variables.h"
#include <sys/_intsup.h>

void auton_blueGoal() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(1, 33, 6, 15000, {.earlyExitRange = 1});
    pros::delay(490);
    intake.move(127);
    pros::delay(340);
    intake.move(0);
    pros::delay(390);
    arm.set_value(HIGH);

    chassis.moveToPose(0, 25, 1, 900, {.forwards = false, .earlyExitRange = 1});
    chassis.moveToPose(2, 25, -60, 900, {.forwards = false});
    pros::delay(1400);
    arm.set_value(LOW);
    clamp.set_value(HIGH);
    chassis.setPose(2, 25, -60);

    chassis.moveToPose(16.5, 16, -60, 9900, {.forwards = false, .maxSpeed = 95.25, .earlyExitRange = 3});
    chassis.waitUntilDone();
    clamp.set_value(LOW);
    pros::delay(490);
    intake.move(127);
    pros::delay(990);
    intake.move(0);

    chassis.moveToPose(16, 17, -150, 1400, {.forwards = true, .earlyExitRange = 1});
    chassis.moveToPose(8, 2, -150, 1400, {.forwards = true, .earlyExitRange = 1});
    chassis.moveToPose(6, 3, -240, 1400, {.forwards = true, .earlyExitRange = 1});
    chassis.waitUntilDone();
    chassis.setPose(0, 0, 0);

    chassis.moveToPose(0, 9, 0, 1400, {.forwards = true});
    chassis.waitUntilDone();
    pros::delay(300);
    arm.set_value(HIGH);
    pros::delay(100);
    chassis.moveToPose(0, 0, 0, 1500, {.forwards = false});
    pros::delay(700);
    arm.set_value(LOW);


    chassis.moveToPose(5, 10, 36, 1000, {.forwards = true});
    intake.move(127);
    pros::delay(1500);
    chassis.moveToPose(21, 20, 68, 2000, {.forwards = true, .maxSpeed = 110});
    wall.set_value(HIGH);
    pros::delay(800);
    wall.set_value(LOW);
    intake.move(0);
    chassis.waitUntilDone();

    leftMotors.move(-127);
    rightMotors.move(-127);
    pros::delay(250);
    leftMotors.move(0);
    rightMotors.move(0);
    intake.move(127);
    pros::delay(2000);
    intake.move(0);


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
