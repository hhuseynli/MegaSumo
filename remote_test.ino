#include <IRremote.hpp>

#define IR_RECEIVE_PIN 11 // Pin connected to the IR receiver

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start receiver
}

void loop() {
  if (IrReceiver.decode()) {
    uint32_t irCode = IrReceiver.decodedIRData.decodedRawData;

    switch (irCode) {
      case 0xBA45FF00: 
        Serial.println("Release");
        break;
      case 0xB946FF00:
        Serial.println("Start");
        break;
      case 0xB847FF00:
        Serial.println("Stop");
        break;
      default:
        Serial.print("Received code: 0x");
        Serial.println(irCode, HEX);
        break;
    }

    IrReceiver.resume(); // Prepare to receive next
  }
}
