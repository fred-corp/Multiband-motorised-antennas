#include <Arduino.h>
#include <TMCStepper.h>
#include <AccelStepper.h>
#include "spool.h"

// Constructor
// ```uint16_t cs_pin```    : Chip Select pin
// ```float r_sense```      : sense resistor value
// ```uint16_t sw_mosi```   : software SPI MOSI pin
// ```uint16_t sw_miso```   : software SPI MISO pin
// ```uint16_t sw_sck```    : software SPI SCK pin
// ```uint16_t step_pin```  : step pin
// ```uint16_t dir_pin```   : direction pin
// ```uint16_t en_pin```    : enable pin
// ```uint16_t stall_pin``` : stall pin
Spool::Spool(uint16_t cs_pin, float r_sense, uint16_t sw_mosi, uint16_t sw_miso, uint16_t sw_sck, uint16_t step_pin, uint16_t dir_pin, uint16_t en_pin, uint16_t stall_pin) {
  this->driver = new TMC2130Stepper(cs_pin, r_sense, sw_mosi, sw_miso, sw_sck);
  this->step_pin = step_pin;
  this->dir_pin = dir_pin;
  this->en_pin = en_pin;
  this->stall_pin = stall_pin;
  this->stepper = new AccelStepper(AccelStepper::DRIVER, this->step_pin, this->dir_pin);
}

// Initialize the spool
void Spool::begin() {
  this->driver->begin();
  this->driver->en_pwm_mode(1);
  this->driver->shaft(0);
  this->invDir = false;

  pinMode(this->step_pin, OUTPUT);
  pinMode(this->dir_pin, OUTPUT);
  pinMode(this->en_pin, OUTPUT);
  pinMode(this->stall_pin, INPUT);

  this->stepper->setEnablePin(this->en_pin);
  this->stepper->setPinsInverted(false, false, true);
  this->stepper->setMaxSpeed(2000);
  this->stepper->setAcceleration(1600);
}

// Set current of the stepper
// ```int current``` : current in mA
void Spool::setCurrent(int current) {
  this->driver->rms_current(current);
}

// Enable the stepper
void Spool::enable() {
  this->stepper->enableOutputs();
}

// Disable the stepper
void Spool::disable() {
  this->stepper->disableOutputs();
}

// Get the raw status of the driver
uint32_t Spool::getDRVStatus() {
  return this->driver->DRV_STATUS();
}

// Test the connection to the driver
// returns raw status
uint8_t Spool::testConnection() {
  return this->driver->test_connection();
}

void Spool::invertDir() {
  this->invDir = !this->invDir;
  this->driver->shaft(this->invDir);
}

// Set the diameter of the spool
// ```float diameter``` : diameter in mm
void Spool::setDiameter(float diameter) {
  this->spoolDiameter = diameter;
}

// Get the diameter of the spool
// returns the diameter of the spool in mm
float Spool::getDiameter() {
  return this->spoolDiameter;
}

// Set the width of the spool
// ```float width``` : width in mm
void Spool::setWidth(float width) {
  this->spoolWidth = width;
}

// Get the width of the spool
// returns the width of the spool in mm
float Spool::getWidth() {
  return this->spoolWidth;
}

// Set the microsteps of the stepper
// ```int microSteps``` : microsteps
void Spool::setMicroSteps(int microSteps) {
  this->microSteps = microSteps;
  this->microStepsPerRevolution = this->stepperStepsPerRevolution * this->microSteps;
  this->driver->microsteps(this->microSteps);
}

// Set the steps per revolution of the stepper
// ```int steps``` : steps per revolution
void Spool::setStepperStepsPerRevolution(int steps) {
  this->stepperStepsPerRevolution = steps;
  this->microStepsPerRevolution = this->stepperStepsPerRevolution * this->microSteps;
}

// Get the steps per revolution of the stepper
// returns the steps per revolution of the stepper
int Spool::getStepsPerRevolution() {
  return this->microStepsPerRevolution;
}

// Set the direction of the stepper
// ```bool dir``` : direction (0 counter-clockwise, 1 clockwise)
void Spool::setDir(bool dir) {
  if (dir) {
    this->stepper->setSpeed(-abs(this->speed));
  } else {
    this->stepper->setSpeed(abs(this->speed));
  }
}

// Perform a single step
// ```int delay``` : delay in microseconds
void Spool::singleStep(int delay) {
  this->stepper->move(1);
  this->stepper->runToPosition();
}

// Destructor
void Spool::end() {
  delete this->driver;
  delete this->stepper;
}

// Set the speed of the spool
// ```int speed``` : speed rpm
void Spool::setSpeed(float speed) {
  this->speed = speed;
  this->stepper->setSpeed((this->speed/60) * (this->microStepsPerRevolution));
}

// Set the acceleration of the spool
// ```int acceleration``` : acceleration in rpm/s
void Spool::setAcceleration(int acceleration) {
  this->acceleration = acceleration;
  this->stepper->setAcceleration((this->acceleration/60)*this->microStepsPerRevolution);
}

// Rotate the spool by a number of steps
// ```int steps``` : steps to rotate
// ```bool accel``` : whether to accelerate or not
void Spool::rotateSteps(int steps) {
  this->stepper->move(steps);
  this->stepper->runToPosition();
}

// Rotate the spool by a number of degrees
// ```float degrees``` : degrees to rotate
void Spool::rotateDegrees(float degrees) {
  float steps = (degrees/360.0) * float(this->microStepsPerRevolution);
  this->rotateSteps(steps);
}

// Rotate the spool by a distance
// ```float distance``` : distance to rotate
void Spool::rotateDistance(float distance) {
  float steps = (distance / (this->spoolDiameter * PI)) * float(this->microStepsPerRevolution);
  this->rotateSteps(steps);
}