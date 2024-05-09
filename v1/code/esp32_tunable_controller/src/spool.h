#ifndef SPOOL_H
#define SPOOL_H

#include <Arduino.h>
#include <TMCStepper.h>


class Spool
{
  public:
    Spool(TMC2130Stepper *driver, char step_pin, char dir_pin, char en_pin, char stall_pin);
    void begin();
    void setCurrent(int current);
    void enable();
    void disable();
    virtual uint32_t getDRVStatus();
    uint8_t testConnection();
    void setDir(bool dir);
    void singleStep(int delay);
  private:
    TMC2130Stepper *driver;
    char step_pin;
    char dir_pin;
    char en_pin;
    char stall_pin;
};

#endif