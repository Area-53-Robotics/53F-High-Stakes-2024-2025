#include "main.h"
#include "pros/adi.h"
#include "pros/device.hpp"
#include "pros/rtos.hpp"
#include "variables.h"

void auton_redGoal() {
    chassis.setPose(0, 0, 0);


    chassis.moveToPose(0, 0, -20, 300, {.earlyExitRange = 1});
    chassis.moveToPose(-13, 34, -20, 1200, {.earlyExitRange = 1});
    pros::delay(600);
    intake.move(127);
    pros::delay(450);
    intake.move(0);
    pros::delay(100);
    arm.set_value(HIGH);
    pros::delay(100);
    chassis.moveToPose(-9.6, 23.8, -22, 500, {.forwards = false, .earlyExitRange = 1});
    pros::delay(490);
    chassis.moveToPose(-10, 25, 88, 1000, {.earlyExitRange = 1});
    pros::delay(1000);
    arm.set_value(LOW);
    pros::delay(50);
    clamp.set_value(HIGH);
    pros::delay(100);
    chassis.moveToPose(-30, 25, 90, 1000, {.forwards = false, .earlyExitRange = 1});
    pros::delay(1100);
    clamp.set_value(LOW);
    intake.move(127);
    chassis.waitUntilDone();
    pros::delay(1000);
    intake.move(0);

    chassis.moveToPose(-30, 25, 220, 1000, {.forwards = true, .earlyExitRange = 1});
    wall.set_value(HIGH);
    chassis.moveToPose(-46, -9, 215, 2000, {.forwards = true, .earlyExitRange = 1});

    /*chassis.moveToPoint(0, 23, 1000, {.earlyExitRange = 1});
    pros::delay(490);
    intake.move(127);
    pros::delay(360);
    intake.move(0);
    chassis.moveToPose(-30, 36, -41, 2000, {.earlyExitRange = 1});
    pros::delay(500);
    arm.set_value(HIGH);*/
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
