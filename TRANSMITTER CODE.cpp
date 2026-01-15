#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>



RF24 radio(9, 10);   
const byte address[6] = "00001";
const int joystickPin = A0;    
const int buttonPin = 2;     

struct DataPacket {
  int servoAngle;    
  bool pumpState;  
};

DataPacket sentData;

bool lastButtonState = HIGH;
bool currentButtonState = HIGH;
bool pumpState = false;

void setup() {
  Serial.begin(9600);
  pinMode(joystickPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);  
 
  if (!radio.begin()) {
    Serial.println("Radio module failed!");
    while (1);
  }
  radio.openWritingPipe(address);    
  radio.setPALevel(RF24_PA_LOW);    
  radio.stopListening();           
  
  Serial.println("Transmitter ready!");
}

void loop() {
  int joystickValue = analogRead(joystickPin);
  sentData.servoAngle = map(joystickValue, 0, 1023, 0, 180);
  currentButtonState = digitalRead(buttonPin);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    pumpState = !pumpState;  
    delay(50);             
  }
  lastButtonState = currentButtonState;
  
  sentData.pumpState = pumpState;
  

  bool result = radio.write(&sentData, sizeof(sentData));
  
 
  Serial.print("Sent | Angle: ");
  Serial.print(sentData.servoAngle);
  Serial.print(" | Pump: ");
  Serial.print(pumpState ? "ON" : "OFF");
  Serial.print(" | Radio: ");
  Serial.println(result ? "OK" : "FAIL");
  
  delay(50);
}
