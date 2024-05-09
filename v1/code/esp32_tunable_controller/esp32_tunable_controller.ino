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


// TODO : stepper motor control
// TODO : dynamics handling (speedup, slowdown, ...)
// TODO : homing/retract sequence
// TODO : state machine
// TODO : web interface
// TODO : CAT protocol

#include <TMCStepper.h>

#define EN_PIN           15
#define DIR_PIN          32
#define STEP_PIN         33
#define CS_PIN           17
#define SW_MOSI          14
#define SW_MISO           5
#define SW_SCK            4

#define R_SENSE 0.11f

#define STALL_PIN        36

#define fullTurn 51200 // 200 * 256 = 51200

TMC2130Stepper driver = TMC2130Stepper(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);

bool dir = true;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Starting");
  driver.begin();
  driver.rms_current(1000); 
  driver.en_pwm_mode(1);


  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);

  Serial.print("DRV_STATUS=0b");
  Serial.println(driver.DRV_STATUS(), BIN);
  Serial.print("DRV conn test = ");
  Serial.println(driver.test_connection());

  driver.shaft(0); // 0 counter-clockwise, 1 clockwise
}

void loop() {
  for (int i = 0; i < fullTurn; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(10);
  }
  delay(1000);
}
