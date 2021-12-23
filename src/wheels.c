#include <stdio.h>
#include <string.h>
#include <unistd.h> //sleep

#include "filewriter.h"

#define RIGHT_FORWARD_PIN 22  //BCM pin number, corresponds 15 physical pin number
#define RIGHT_BACKWARD_PIN 23 //BCM pin number, corresponds 16 physical pin number
#define LEFT_FORWARD_PIN 26   //BCM pin number, corresponds 37 physical pin number
#define LEFT_BACKWARD_PIN 12  //BCM pin number, corresponds 32 physical pin number

#define PINS_COUNT 4
#define PINS (int[PINS_COUNT]){ RIGHT_FORWARD_PIN, RIGHT_BACKWARD_PIN, LEFT_FORWARD_PIN, LEFT_BACKWARD_PIN }

#define OUT_DIRECTION "out"

#define LOW  "0" //0.0V at pin output
#define HIGH "1" //3.3V at pin output

#define GPIO_PATH_PREFIX "/sys/class/gpio/gpio"
#define VALUE_PATH_POSTFIX "/value"

void exportPins(); 
void setPinDirections();

void initWheels() {
    exportPins();
    sleep(0.5);
    setPinDirections(OUT_DIRECTION);
}

void moveForward() {
    char rightForwardValueFile[255];
    char leftForwardValueFile[255];
    sprintf(rightForwardValueFile, "%s%d%s", GPIO_PATH_PREFIX, RIGHT_FORWARD_PIN, VALUE_PATH_POSTFIX);
    sprintf(leftForwardValueFile, "%s%d%s", GPIO_PATH_PREFIX, LEFT_FORWARD_PIN, VALUE_PATH_POSTFIX);
    writeToFile(rightForwardValueFile, HIGH);
    writeToFile(leftForwardValueFile, HIGH);
}

void stopWheels() {
    for (int i = 0; i < PINS_COUNT; i++) {
        char pathToValueFile[255];
        sprintf(pathToValueFile, "%s%d%s", GPIO_PATH_PREFIX, PINS[i], VALUE_PATH_POSTFIX);
        writeToFile(pathToValueFile, LOW);
    }
}

void exportPins() {
    printf("Exporting pins...\n");

    for (int i = 0; i < PINS_COUNT; i++) {
        char pinNumber[2];
        sprintf(pinNumber, "%d", PINS[i]);
        writeToFile("/sys/class/gpio/export", pinNumber);
    }

    printf("Pins export complete...\n");
}

void setPinDirections() {
    printf("Setting directions...\n");

    for (int i = 0; i < PINS_COUNT; i++) {
        char directionFile[255];
        sprintf(directionFile, "%s%d%s", GPIO_PATH_PREFIX, PINS[i], "/direction");
        writeToFile(directionFile, OUT_DIRECTION);
    }

    printf("Directions definition complete...\n");
}
