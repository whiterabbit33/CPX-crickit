/* Infrared_Read.ino Example sketch for IRLib2 and Circuit Playground Express
   Illustrates how to receive an IR signal, decode it and print
   information about it to the serial monitor.
*/
#include <Adafruit_CircuitPlayground.h>

#include "Adafruit_Crickit.h"
#include "seesaw_motor.h"


//IRdecode myDecoder;//Decoder object

Adafruit_Crickit crickit;

seesaw_Motor motor_a(&crickit);
seesaw_Motor motor_b(&crickit);


#if !defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS)
  #error "Infrared support is only for the Circuit Playground Express, it doesn't work with the Classic version"
#endif

/* IR signals consist of a protocol number, a value, and a number of bits.
 *  Store all of these values for future use.
 */
uint8_t IR_protocol;
uint32_t IR_value;
uint16_t IR_bits;




void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
//  while (!Serial);
  Serial.println("Send an IR signal and I will record it.");
  Serial.println("Press the left button and we will retransmit it.");
  

  IR_protocol=0; //  Indicates we've not received a code yet
  
  CircuitPlayground.irReceiver.enableIRIn(); // Start the receiver
  Serial.println("Ready to receive IR signals");


  if (!crickit.begin()) {
    Serial.println("ERROR!");
    while (1);
  }
  else Serial.println("Crickit started");

  //attach motor a
  motor_a.attach(CRICKIT_MOTOR_A1, CRICKIT_MOTOR_A2);

  //attach motor b
  motor_b.attach(CRICKIT_MOTOR_B1, CRICKIT_MOTOR_B2);

   CircuitPlayground.clearPixels();

  CircuitPlayground.setPixelColor(3,   0, 255,   0);

  CircuitPlayground.setPixelColor(4, 0, 255,   0);
  
  delay(1000);

  
}



void loop() {
  

if (! CircuitPlayground.irReceiver.getResults()) {
    return;
  }

  // Did we get any decodable messages?
  if (! CircuitPlayground.irDecoder.decode()) {
    CircuitPlayground.irReceiver.enableIRIn(); // Restart receiver  
    return;
  }

  // We can print out the message if we want...
  CircuitPlayground.irDecoder.dumpResults(false);

  // Did we get any NEC remote messages?
  if (! CircuitPlayground.irDecoder.protocolNum == NEC) {
    CircuitPlayground.irReceiver.enableIRIn(); // Restart receiver  
    return;
  }

  // What message did we get?
  switch(CircuitPlayground.irDecoder.value) {
  case 0x20f002FD: //forward
      Serial.println("Forward");
      motor_a.throttle(-0.5);
      motor_b.throttle(-0.5);
      delay(2000);
      motor_a.throttle(0);
      motor_b.throttle(0);
      delay(10);
  
    break;
    
  case 0x20f022DD: //stop
    Serial.println("stop");
    motor_a.throttle(0);
    motor_b.throttle(0);
    delay(10);
    break;
    
  case 0x20f0827D: //back
    Serial.println("stop");
    motor_a.throttle(0.5);
    motor_b.throttle(0.5);
    delay(2000);
    motor_a.throttle(0);
    motor_b.throttle(0);
    delay(10);
    break;

    case 0x20f0E01F: //left
    Serial.println("left");
    motor_a.throttle(0.5);
    motor_b.throttle(-0.5);
    delay(250);
    motor_a.throttle(0);
    motor_b.throttle(0);
    delay(10);
    break;

    case 0x20f0609F: //right
    Serial.println("back");
    motor_a.throttle(-0.5);
    motor_b.throttle(0.5);
    delay(250);
    motor_a.throttle(0);
    motor_b.throttle(0);
    delay(10);
    break;

    case 0x20f0906F: //full forward
    Serial.println("fullfwd");
    motor_a.throttle(0.5);
    motor_b.throttle(0.5);
    delay(150);
    
    break;
    
 
  }
  //Restart receiver
  CircuitPlayground.irReceiver.enableIRIn();      
}




  

