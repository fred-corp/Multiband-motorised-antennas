/*
 * @file esp32_tunable_controller.ino
 * @brief Control script for ON4PFD's tunable dipole antenna
 * 
 * This script is used to control the ON4PFD's tunable dipole antenna.
 * It supports the TMC2130 stepper driver and the ESP32 microcontroller.
 * Control is done via the web interface, or can automatically fetch a rig's frequency
 * by using the CAT protocol of an FLRig server.
 * 
 * 
 * @author ON4PFD
 * @date 2024-05-08
 * @version 1.0
 */


// TODO : dynamics handling (speedup, slowdown, ...)
// TODO : homing/retract sequence
// TODO : state machine
// TODO : web interface
// TODO : CAT protocol

#include "./src/spool.h"

#define EN_PIN           15
#define DIR_PIN          32
#define STEP_PIN         33
#define CS_PIN           17
#define SW_MOSI          14
#define SW_MISO           5
#define SW_SCK            4

#define R_SENSE 0.11f

#define STALL_PIN        36

#define fullTurn 1600 // 200 * 8 = 1600

Spool* spool = new Spool(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK, STEP_PIN, DIR_PIN, EN_PIN, STALL_PIN);

bool dir = true;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Starting");
  (*spool).begin(); // Dereference the spool pointer before calling the begin() function
  spool->setCurrent(1000);
  spool->enable();

  Serial.print("DRV_STATUS=0b");
  Serial.println(spool->getDRVStatus(), BIN);
  Serial.print("DRV conn test = ");
  Serial.println(spool->testConnection());

  // spool->setDir(0); // 0 counter-clockwise, 1 clockwise
  // spool->setSpeed(60);
  // spool->setAcceleration(1);
}

void loop() {
  spool->rotateSteps(fullTurn, true);
  delay(5000);
}
