// MultiStepper.pde
// -*- mode: C++ -*-
//
// Shows how to multiple simultaneous steppers
// Runs one stepper forwards and backwards, accelerating and decelerating
// at the limits. Runs other steppers at the same time
//
// Copyright (C) 2009 Mike McCauley
// $Id: MultiStepper.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

// Define some steppers and the pins the will use
AccelStepper stepper1(AccelStepper::DRIVER,30,31,32);
AccelStepper stepper2(AccelStepper::DRIVER,36,37,38);
AccelStepper stepper3(AccelStepper::DRIVER,33,34,35);
AccelStepper stepper4(AccelStepper::DRIVER,39,40,41);
AccelStepper stepper5(AccelStepper::DRIVER,42,43,44);

void setup()
{
    // wrist tilt
    stepper1.setMaxSpeed(500.0);
    stepper1.setAcceleration(3000.0);

    // wrist rotate
    stepper2.setMaxSpeed(500.0);
    stepper2.setAcceleration(3000.0);

    // elbow
    stepper3.setMaxSpeed(500.0);
    stepper3.setAcceleration(3000.0);

    // shoulder tilt
    stepper4.setMaxSpeed(500.0);
    stepper4.setAcceleration(3000.0);

    // shoulder rotate
    stepper5.setMaxSpeed(500.0);
    stepper5.setAcceleration(3000.0);

}

int pos = 2000;

void loop()
{
    if (stepper1.distanceToGo() == 0)
  {
    delay(500);
    pos = -pos;
    stepper1.moveTo(pos);\
  }

  stepper1.run();
  stepper2.run();
  stepper3.run();
  stepper4.run();
  stepper5.run();
}
