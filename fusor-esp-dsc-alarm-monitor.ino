/*
 *  Monitoring DSC based alarm system - see open zones, status and etc.
 *  Needs direct access to serial data bus
 * 
 *  Based on Fusor framework
 *  
 *  Board tested: WeMos D1 R2 & Mini
 *  
 *  For more info look in readme.md
 */

// #define SM_DEBUGGER

#include <Arduino.h>

#include <StateMachine.h>
#include <StateMachineDebug.h>
#include <DSCAlarmSystemPlugin.h>
#include <NodeConnector.h>

const int clockPin = D6;
const int signalPin = D5;

void sleepFunction(unsigned long ms)
{
  delay(ms);
}

unsigned long getTime()
{
  return millis();
}

void debugPrinter(const char *message)
{
  Serial.print(message);
}

NodeConnector nodeConnector;
StateMachineController sm = StateMachineController("alm", sleepFunction, getTime);
DSCAlarmSystemPlugin dcaAlarmSystemPlugin = DSCAlarmSystemPlugin("dsc", clockPin, signalPin);

void setup() {
  Serial.begin(57600);

  delay(100);
  pinMode(D5, INPUT); // digital input for control button

  // D5 - control button for node setup
  nodeConnector.setup(D5, false);

  if (nodeConnector.isSmdLoaded) Serial.println(F("SMD loaded"));
  else {
    Serial.println(F("No SMD. Halt."));
    while(true) delay(100000);
  }


  nodeConnector.initSM(&sm);
  Serial.println(F("State Machine created"));
  sm.registerPlugin(&dcaAlarmSystemPlugin);

  Serial.println(F("Actions and plugins registered"));

  sm.init();
  Serial.println(F("State machine initialized"));
}


void loop() {
  sm.cycle();
  nodeConnector.loop();
}
