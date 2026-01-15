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
  int servoAngle;    //угол сервы (0-180)
  bool pumpState;    //состояние насоса (1-вкл, 0-выкл)
};

DataPacket receivedData;

void setup() {
  Serial.begin(9600);
  

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  //насос выключен
  
  myServo.attach(servoPin);     
  myServo.write(90);            //стартовое положение - середина
  

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
    
    //управляем сервоприводом
    int angle = constrain(receivedData.servoAngle, 0, 180);  // Ограничиваем угол
    myServo.write(angle);
    
    //управляем насосом через реле
    // ВНИМАНИЕ: У реле может быть логика LOW = ВКЛ, HIGH = ВЫКЛ
    // Проверьте ваше реле! Этот код для реле с активным HIGH
    digitalWrite(relayPin, receivedData.pumpState ? HIGH : LOW);
    
    // вывод в монитор порта для отладки
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" | Pump: ");
    Serial.println(receivedData.pumpState ? "ON" : "OFF");
    
    
    delay(20);
  }
}
