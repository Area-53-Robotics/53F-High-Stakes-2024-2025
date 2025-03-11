#include "main.h"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "variables.h"
#include <iomanip>
#include <sstream>

bool showKeybinds = false;

void coordinates() {
    lemlib::Pose pose = chassis.getPose();

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "X: " << pose.x << " Y: " << pose.y << " Theta: " << pose.theta;

    pros::lcd::print(0, "X: %.2f; Y: %.2f; Theta: %.2f", pose.x, pose.y, pose.theta);
    //controller.set_text(1, 0, ss.str());
}

void displayInfo() {
    std::stringstream status;
    status << (reverseDT ? "Rev " : "") << (arcade ? "Arc " : "") << (clampON ? "Clamp " : "")
           << (wallON ? "Wall " : "") << (armON ? "Arm " : "");
    //controller.set_text(1, 0, status.str());
}

void displayKeybinds() {
    //controller.set_text(1, 0, "A: Rev | B: Arc | UP: Clamp");
    //controller.set_text(2, 0, "L1: Wall | LEFT: Arm");
}

// Open Control (Driver Control)
void opcontrol() {
    //pros::delay(100);
    //controller.clear();

    while (true) {
        int battery = pros::battery::get_capacity();
        //controller.print(0, 0, "Bat: %d%%", battery);
        pros::lcd::print(1, "Battery: %d%%", battery);

        coordinates();

        double leftSpeed = leftMotors.get_actual_velocity();
        double rightSpeed = rightMotors.get_actual_velocity();
        //controller.print(2, 0, "L: %.1f  R: %.1f", leftSpeed, rightSpeed);
        pros::lcd::print(2, "L: %.1f  R: %.1f", leftSpeed, rightSpeed);

        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
            reverseDT = !reverseDT;
        }
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            arcade = !arcade;
        }
        if (!reverseDT && !arcade) {
            chassis.tank(leftY, rightY);
        } else if (reverseDT && !arcade) {
            chassis.tank(-rightY, -leftY);
        } else if (arcade && !reverseDT) {
            chassis.arcade(rightY, rightX);
        } else if (arcade && reverseDT) {
            chassis.arcade(-rightY, rightX);
        }

        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intake.move(127);
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            intake.move(-127);
        } else {
            intake.move(0);
        }

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
            clampON = !clampON;
        }
        clamp.set_value(clampON ? HIGH : LOW);

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
            wallON = !wallON;
        }
        wall.set_value(wallON ? HIGH : LOW);

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            armON = !armON;
        }
        arm.set_value(armON ? HIGH : LOW);

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            showKeybinds = !showKeybinds;
        }

        if (showKeybinds) {
            displayKeybinds();
        } else {
            displayInfo();
        }

        pros::delay(25);
        // Done
    }
}
