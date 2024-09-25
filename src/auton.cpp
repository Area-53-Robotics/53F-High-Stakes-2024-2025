#include "main.h"
#include "variables.h"

ASSET(path_jerryio_txt);

void autonomous() {
   chassis.follow(path_jerryio_txt, 15, 2000);
}