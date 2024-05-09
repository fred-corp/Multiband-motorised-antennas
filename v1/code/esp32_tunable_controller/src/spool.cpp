#include <Arduino.h>
#include <TMCStepper.h>
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
  this->spoolDiameter = 100;     // defaults to 100mm
  this->spoolWidth = 20;        // defaults to 20mm
  this->driver = new TMC2130Stepper(cs_pin, r_sense, sw_mosi, sw_miso, sw_sck);
  this->step_pin = step_pin;
  this->dir_pin = dir_pin;
  this->en_pin = en_pin;
  this->stall_pin = stall_pin;
}

// Initialize the spool
void Spool::begin() {
  this->driver->begin();
  this->driver->en_pwm_mode(1);

  pinMode(this->step_pin, OUTPUT);
  pinMode(this->dir_pin, OUTPUT);
  pinMode(this->en_pin, OUTPUT);
  pinMode(this->stall_pin, INPUT);
}

// Set current of the stepper
// ```int current``` : current in mA
void Spool::setCurrent(int current) {
  this->driver->rms_current(current);
}

// Enable the stepper
void Spool::enable() {
  digitalWrite(this->en_pin, LOW);
}

// Disable the stepper
void Spool::disable() {
  digitalWrite(this->en_pin, HIGH);
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

// Set the direction of the stepper
// ```bool dir``` : direction (0 counter-clockwise, 1 clockwise)
void Spool::setDir(bool dir) {
  this->driver->shaft(dir);
}

// Perform a single step
// ```int delay``` : delay in microseconds
void Spool::singleStep(int delay) {
  digitalWrite(this->step_pin, HIGH);
  delayMicroseconds(delay);
  digitalWrite(this->step_pin, LOW);
  delayMicroseconds(delay);
}

// Destructor
Spool::~Spool() {
  delete this->driver;
}

