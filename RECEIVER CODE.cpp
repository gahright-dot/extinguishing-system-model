#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(9, 10);   
Servo myServo;
const byte address[6] = "00001";
const int relayPin = 5;    
const int servoPin = 6;   

struct DataPacket {
  int servoAngle;    
  bool pumpState;   
};

DataPacket receivedData;

void setup() {
  Serial.begin(9600);
  

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); 
  
  myServo.attach(servoPin);     
  myServo.write(90);           

  if (!radio.begin()) {
    Serial.println("Radio module failed!");
    while (1);
  }
  radio.openReadingPipe(0, address); 
  radio.setPALevel(RF24_PA_LOW);    
  radio.startListening();            
  
  Serial.println("Receiver ready!");
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedData, sizeof(receivedData));  
    
    int angle = constrain(receivedData.servoAngle, 0, 180);  
    myServo.write(angle);
  
    digitalWrite(relayPin, receivedData.pumpState ? HIGH : LOW);
    
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" | Pump: ");
    Serial.println(receivedData.pumpState ? "ON" : "OFF");
    
    
    delay(20);
  }
}
