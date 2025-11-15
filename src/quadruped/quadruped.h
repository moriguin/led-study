// src/quadruped/quadruped.h
#ifndef QUADRUPED_QUADRUPED_H
#define QUADRUPED_QUADRUPED_H

#include <Adafruit_PWMServoDriver.h>

// 外部参照
extern Adafruit_PWMServoDriver pwm;

void trotGaitStep();

void initState();

#endif