/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  BootKeyboard example

  Shows that keyboard works even in bios.
  Led indicats if we are in bios.

  See HID Project documentation for more information.
  https://github.com/NicoHood/HID/wiki/Keyboard-API#boot-keyboard
*/

#include "HID-Project.h"

// constants won't change. They're used here to set pin numbers:
const int pinLed = LED_BUILTIN;
const int pinButton = 2;
const int pinLatchOff = 22;
const int pinLatchOn = 23;

// Variables will change:
int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

void setup() {
  // Setup Output Pins
  pinMode(pinLed, OUTPUT);
  pinMode(pinLatchOff, OUTPUT);
  pinMode(pinLatchOn, OUTPUT);  

  // Setup Input Pins
  pinMode(pinButton, INPUT_PULLUP);
  
  // Sends a clean report to the host. This is important on any Arduino type.
  BootKeyboard.begin();

  // set initial LED state
  digitalWrite(pinLed, ledState);
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(pinLed);
//  // Light led if keyboard uses the boot protocol (normally while in bios)
//  // Keep in mind that on a 16u2 and Arduino Micro HIGH and LOW for TX/RX Leds are inverted.
//  if (BootKeyboard.getProtocol() == HID_BOOT_PROTOCOL)
//    digitalWrite(pinLed, LOW);
//  else
//    digitalWrite(pinLed, HIGH);


  // Check for button press
  if (!digitalRead(pinButton)) {

    if (ledState == LOW){
      ledState = HIGH;
      trigger_light(pinLatchOn);
      digitalWrite(pinLed, ledState);
      }
    else {
      ledState = LOW;
      trigger_light(pinLatchOff);
      digitalWrite(pinLed, ledState);
     }

    // Mute / Unmute Channel One on MixPre
    mute_channel_one();

    // Trigger the latch circuit
    // trigger_light(pinLatchOn);

    // Simple debounce
    delay(300);
  }
}

void mute_channel_one(){
    BootKeyboard.press(KEY_LEFT_CTRL);
    BootKeyboard.press(KEY_LEFT_ALT);
    BootKeyboard.press('1');
    BootKeyboard.releaseAll();
}

void trigger_light(int pinLatch){
  // Pulse pinLatch 
  digitalWrite(pinLatch, HIGH); // sets the pin on
  delayMicroseconds(50);      // pauses for 50 microseconds
  digitalWrite(pinLatch, LOW);  // sets the pin off
}
