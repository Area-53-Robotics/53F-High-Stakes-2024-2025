#include "main.h"
#include "pros/rtos.hpp"
#include "variables.h"
#include <sys/_intsup.h>

void Red_AWP_RR() {
    chassis.setPose(0, 0, 0);
    intake.move(127);
    pros::delay(250);
    intake.move(0);
    wall.set_value(HIGH);
    chassis.moveToPose(9, 9, 69, 1000, {.minSpeed = 60, .earlyExitRange = 1});
    pros::delay(900);
    wall.set_value(LOW);
    //chassis.setPose(9, 9, 65);
    pros::delay(300);

    chassis.moveToPose(-8, 0, 69, 2000, {.forwards = false, .earlyExitRange = 1});
    chassis.moveToPose(-21, -5, 69, 2000, {.forwards = false, .earlyExitRange = 1});
    clamp.set_value(HIGH);
    pros::delay(1500);
    clamp.set_value(LOW);

    chassis.moveToPose(-26, -9, 65, 1000, {.forwards = false, .earlyExitRange = 1});
    chassis.moveToPose(-27, -9, 177, 1000, {.forwards = false, .earlyExitRange = 1});
    chassis.moveToPose(-22, -20, 180, 1000, {.forwards = true, .earlyExitRange = 1});
    intake.move(127);
    pros::delay(3000);
    intake.move(0);

    chassis.moveToPose(-22, -25, 180, 1000, {.forwards = false, .earlyExitRange = 1});
    chassis.moveToPose(-22, -25, 262, 1000, {.forwards = false, .earlyExitRange = 1});
    chassis.moveToPose(-33, -27, 262, 1000, {.forwards = true});
    intake.move(127);
    chassis.waitUntilDone();

    chassis.moveToPose(-22, -27, 262, 1000, {.forwards = false, .earlyExitRange = 1});
    chassis.waitUntilDone();

    chassis.moveToPose(-36, -30, 250, 1000, {.forwards = true, .earlyExitRange = 1});

    pros::delay(6000);
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
