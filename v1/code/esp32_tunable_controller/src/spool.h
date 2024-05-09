#ifndef SPOOL_H
#define SPOOL_H

#include <Arduino.h>
#include <TMCStepper.h>


class Spool
{
  public:
    Spool(uint16_t cs_pin, float r_sense, uint16_t sw_mosi, uint16_t sw_miso, uint16_t sw_sck, uint16_t step_pin, uint16_t dir_pin, uint16_t en_pin, uint16_t stall_pin);
    void begin();
    void setCurrent(int current);
    void enable();
    void disable();
    virtual uint32_t getDRVStatus();
    uint8_t testConnection();
    void setDiameter(float diameter);
    float getDiameter();
    void setWidth(float width);
    float getWidth();
    void setDir(bool dir);
    void singleStep(int delay);
  private:
    float spoolDiameter;
    float spoolWidth;
    TMC2130Stepper *driver;
    uint16_t step_pin;
    uint16_t dir_pin;
    uint16_t en_pin;
    uint16_t stall_pin;
};

#endif