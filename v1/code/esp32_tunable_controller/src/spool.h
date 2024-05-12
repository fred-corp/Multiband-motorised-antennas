#ifndef SPOOL_H
#define SPOOL_H

#include <Arduino.h>
#include <TMCStepper.h>
#include <AccelStepper.h>


class Spool
{
  public:
    Spool(uint16_t cs_pin, float r_sense, uint16_t sw_mosi, uint16_t sw_miso, uint16_t sw_sck, uint16_t step_pin, uint16_t dir_pin, uint16_t en_pin, uint16_t stall_pin);
    void begin();
    void end();
    void setCurrent(int current);
    void enable();
    void disable();
    virtual uint32_t getDRVStatus();
    uint8_t testConnection();
    void invertDir();
    void setDiameter(float diameter);
    float getDiameter();
    void setWidth(float width);
    float getWidth();
    void setMicroSteps(int microSteps);
    void setStepperStepsPerRevolution(int steps);
    int getStepsPerRevolution();
    void setDir(bool dir);
    void singleStep(int delay);
    void setSpeed(float speed);
    void setAcceleration(int acceleration);
    void rotateSteps(int steps);
    void rotateDegrees(float degrees);
    void rotateDistance(float distance);
  private:
    bool invDir;
    float spoolDiameter;
    float spoolWidth;
    int microSteps;
    int stepperStepsPerRevolution;
    int microStepsPerRevolution;
    TMC2130Stepper *driver;
    AccelStepper *stepper;
    uint16_t step_pin;
    uint16_t dir_pin;
    uint16_t en_pin;
    uint16_t stall_pin;
    bool dir;
    int speed;
    float acceleration;
};

#endif