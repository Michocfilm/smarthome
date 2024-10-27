#include <Servo.h>
Servo myServo;  // สร้างอ็อบเจ็กต์สำหรับ Servo
int sensorPin1 = 2;  // ขาเชื่อมต่อเซ็นเซอร์ (ในที่นี้ใช้ขา Digital Pin 2)
int sensorPin2 = 3;
int sensorLight = A0; // พินเชื่อมต่อเซ็นเซอร์วัดแสง (ควรเป็น Analog Pin เช่น A0)
int sensorValue1;     // ตัวแปรสำหรับเก็บค่าจากเซ็นเซอร์ D2
int sensorValue2;     // ตัวแปรสำหรับเก็บค่าจากเซ็นเซอร์ D3
const int lightPin = 7; // พินสำหรับไฟที่ต้องการเปิดเมื่อแสงมืด
const int Rpin = 4;  // LED RED D4
const int Ypin = 5;  // LED YELLOW D5
const int Gpin = 6;  // LED GREEN D6
unsigned long previousMillis = 0;  // เก็บเวลาสำหรับการเช็คไฟ LED
unsigned long servoMillis = 0;     // เก็บเวลาสำหรับการเช็คการทำงานของ Servo
const long intervalLED = 3000;     // ระยะเวลาไฟแต่ละดวง
const long servoDelay = 4000;      // เวลาที่ Servo ทำงานเมื่อมีการตรวจจับ
const int lightThreshold = 500;    // ค่าความสว่างที่เป็นเกณฑ์สำหรับแสงน้อย (ปรับตามความเหมาะสม)
bool servoActivated = false;

void setup() {
  Serial.begin(9600); 
  myServo.attach(8);  // เชื่อมต่อ Servo ที่ขา Digital Pin 9 หรือ D9
  pinMode(sensorPin1, INPUT);  
  pinMode(sensorPin2, INPUT); 
  pinMode(sensorLight, INPUT); // ตั้งพินเซ็นเซอร์วัดแสงเป็น INPUT
  myServo.write(0);  // กำหนดค่า servo เป็น 0 ํ
  pinMode(Rpin, OUTPUT);
  pinMode(Ypin, OUTPUT);
  pinMode(Gpin, OUTPUT);
  pinMode(lightPin, OUTPUT); // ตั้งพินไฟเป็น OUTPUT
  
  delay(800);
  digitalWrite(Rpin, LOW); 
  digitalWrite(Ypin, LOW);
  digitalWrite(Gpin, HIGH);
  delay(800);
  digitalWrite(Rpin, LOW); 
  digitalWrite(Ypin, HIGH);
  digitalWrite(Gpin, HIGH);
  delay(800);
  digitalWrite(Rpin, HIGH); 
  digitalWrite(Ypin, HIGH);
  digitalWrite(Gpin, HIGH);
  delay(1000);
  digitalWrite(Rpin, LOW); 
  digitalWrite(Ypin, LOW);
  digitalWrite(Gpin, LOW);
  delay(1500);
}

void loop() {
  sensorValue1 = digitalRead(sensorPin1);  // อ่านค่าสถานะจากเซ็นเซอร์
  sensorValue2 = digitalRead(sensorPin2);  // อ่านค่าสถานะจากเซ็นเซอร์
  int lightValue = analogRead(sensorLight); // อ่านค่าความสว่างจากเซ็นเซอร์แสง (ค่าช่วง 0-1023)
  unsigned long currentMillis = millis();  // เก็บค่าเวลาปัจจุบัน

  // ตรวจสอบเซ็นเซอร์แสง: ถ้าค่าความสว่างน้อยกว่าเกณฑ์ (มืด) ให้เปิดไฟ
  if (lightValue < lightThreshold) {
    digitalWrite(lightPin, HIGH);  // เปิดไฟที่พิน 7 เมื่อแสงน้อย
  } else {
    digitalWrite(lightPin, LOW);   // ปิดไฟที่พิน 7 เมื่อแสงสว่าง
  }

  // ตรวจสอบเซ็นเซอร์ Servo
  if ((sensorValue1 == HIGH || sensorValue2 == HIGH) && !servoActivated) {
    myServo.write(180);  // หมุน Servo ไปที่ 180 องศา
    servoMillis = currentMillis;  // บันทึกเวลาที่ Servo เริ่มทำงาน
    servoActivated = true;
  }

  // เช็คเวลาการทำงาน Servo
  if (servoActivated && (currentMillis - servoMillis >= servoDelay)) {
    myServo.write(0);  // ตั้ง Servo กลับที่เดิม
    servoActivated = false;
  }

  // สลับไฟ LED ตามเวลา
  if (currentMillis - previousMillis >= intervalLED) {
    previousMillis = currentMillis;
    
    if (digitalRead(Gpin) == HIGH) {
      digitalWrite(Gpin, LOW);
      digitalWrite(Ypin, HIGH);
    } 
    else if (digitalRead(Ypin) == HIGH) {
      digitalWrite(Ypin, LOW);
      digitalWrite(Rpin, HIGH);
    } 
    else {
      digitalWrite(Rpin, LOW);
      digitalWrite(Gpin, HIGH);
    }
  }
}
