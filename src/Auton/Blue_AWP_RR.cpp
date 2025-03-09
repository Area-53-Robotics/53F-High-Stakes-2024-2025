#include "main.h"
#include "variables.h"
#include <sys/_intsup.h>

void Blue_AWP_RR() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(0, 16, 0, 1000, {.earlyExitRange = 1});
    intake.move(127);
    pros::delay(250);
    intake.move(0);
    pros::delay(450);
    arm.set_value(HIGH);
    pros::delay(100);
    chassis.waitUntilDone();
    chassis.moveToPose(0, 6, 0, 1000, {.forwards = false, .earlyExitRange = 1});
    pros::delay(300);
    arm.set_value(LOW);

    chassis.moveToPose(0, 6, 20, 1000, {.earlyExitRange = 1});
    chassis.moveToPose(5, 17, 23, 1000, {.earlyExitRange = 1});
    intake.move(127);
    pros::delay(1150);
    intake.move(0);
    chassis.waitUntilDone();

    wall.set_value(HIGH);
    chassis.moveToPose(5, 17, -55, 1000, {.earlyExitRange = 1});
    chassis.moveToPose(-9, 26, -60, 1000, {.earlyExitRange = 1});
    pros::delay(800);
    wall.set_value(LOW);
    //chassis.waitUntilDone();
    pros::delay(300);

    clamp.set_value(HIGH);
    chassis.moveToPose(1, 20, -62, 750, {.forwards = false, .earlyExitRange = 5});
    chassis.moveToPose(4, 22, -55, 500, {.forwards = false, .earlyExitRange = 5});
    chassis.moveToPose(23, 9, -63.5, 2250, {.forwards = false, .maxSpeed = 75, .earlyExitRange = 1});
    pros::delay(1200);
    clamp.set_value(LOW);
    pros::delay(100);
    intake.move(127);
    pros::delay(1000);
    intake.move(0);

    chassis.moveToPose(27, 6, -65, 1000, {.forwards = false, .earlyExitRange = 1});
    chassis.moveToPose(22, 9, -180, 950, {.forwards = false, .earlyExitRange = 1});
    intake.move(127);
    chassis.moveToPose(24, -9, -180, 1000, {.earlyExitRange = 1});
    chassis.moveToPose(25, -9, -266, 1000, {.earlyExitRange = 1});
    pros::delay(500);
    chassis.moveToPose(35, -11, -270, 1000, {.earlyExitRange = 1});
    pros::delay(2100);
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
