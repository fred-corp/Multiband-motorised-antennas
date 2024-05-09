#include <Arduino.h>
#include <TMCStepper.h>
#include "spool.h"

Spool::Spool(uint16_t cs_pin, float r_sense, uint16_t sw_mosi, uint16_t sw_miso, uint16_t sw_sck, uint16_t step_pin, uint16_t dir_pin, uint16_t en_pin, uint16_t stall_pin) {
  this->spoolDiameter = 100;     // defaults to 100mm
  this->spoolWidth = 200;
  this->driver = new TMC2130Stepper(cs_pin, r_sense, sw_mosi, sw_miso, sw_sck);
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

void Spool::setDiameter(float diameter) {
  this->spoolDiameter = diameter;
}

float Spool::getDiameter() {
  return this->spoolDiameter;
}

void Spool::setWidth(float width) {
  this->spoolWidth = width;
}

float Spool::getWidth() {
  return this->spoolWidth;
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