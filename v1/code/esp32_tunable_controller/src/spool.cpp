#include <Arduino.h>
#include <TMCStepper.h>
#include "spool.h"

Spool::Spool(TMC2130Stepper *driver, char step_pin, char dir_pin, char en_pin, char stall_pin) {
  this->driver = driver;
  this->step_pin = step_pin;
  this->dir_pin = dir_pin;
  this->en_pin = en_pin;
  this->stall_pin = stall_pin;
}

void Spool::begin() {
  this->driver->begin();
  this->driver->en_pwm_mode(1);

  pinMode(this->step_pin, OUTPUT);
  pinMode(this->dir_pin, OUTPUT);
  pinMode(this->en_pin, OUTPUT);
  pinMode(this->stall_pin, INPUT);
}

void Spool::setCurrent(int current) {
  this->driver->rms_current(current);
}

void Spool::enable() {
  digitalWrite(this->en_pin, LOW);
}

void Spool::disable() {
  digitalWrite(this->en_pin, HIGH);
}

uint32_t Spool::getDRVStatus() {
  return this->driver->DRV_STATUS();
}

uint8_t Spool::testConnection() {
  return this->driver->test_connection();
}

void Spool::setDir(bool dir) {
  this->driver->shaft(dir);
}

void Spool::singleStep(int delay) {
  digitalWrite(this->step_pin, HIGH);
  delayMicroseconds(delay);
  digitalWrite(this->step_pin, LOW);
  delayMicroseconds(delay);
}