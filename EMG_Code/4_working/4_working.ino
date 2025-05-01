#include <Servo.h> // Include the Servo library

const float a0 = 1.0;
const float a1 = -1.6114;
const float a2 = 0.7159;
const float b0 = 0.3580;
const float b1 = 0.0;
const float b2 = -0.3580;

float inputBuffer0[2] = {0.0, 0.0}; 
float outputBuffer0[2] = {0.0, 0.0};
float armaBuffer0[10] = {0.0};

float inputBuffer1[2] = {0.0, 0.0}; 
float outputBuffer1[2] = {0.0, 0.0};
float armaBuffer1[10] = {0.0};

float inputBuffer2[2] = {0.0, 0.0}; 
float outputBuffer2[2] = {0.0, 0.0};
float armaBuffer2[10] = {0.0};

float inputBuffer3[2] = {0.0, 0.0}; 
float outputBuffer3[2] = {0.0, 0.0};
float armaBuffer3[10] = {0.0};

const int ledPin0 = 5; 
const int ledPin1 = 6; 
const int ledPin2 = 7; 
const int ledPin3 = 3; 

const int servoPin0 = 9;
const int servoPin1 = 10;
const int servoPin2 = 11;
const int servoPin3 = 12;

const float threshold0 = 100;
const float threshold1 = 100;
const float threshold2 = 10;
const float threshold3 = 10;

Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin0, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  digitalWrite(ledPin0, LOW);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);

  servo0.attach(servoPin0);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);

  servo0.write(0);
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
}

void loop() {
  // === Signal 0 ===
  float inputSignal0 = analogRead(A0) + 256; 
  float outputSignal0 = b0 * inputSignal0 + b1 * inputBuffer0[0] + b2 * inputBuffer0[1]
                        - a1 * outputBuffer0[0] - a2 * outputBuffer0[1];

  inputBuffer0[1] = inputBuffer0[0];
  inputBuffer0[0] = inputSignal0;

  outputBuffer0[1] = outputBuffer0[0];
  outputBuffer0[0] = outputSignal0;

  float armaAverage0 = updateARMA(armaBuffer0, fabs(outputSignal0));
  armaAverage0 *= armaAverage0;

  if (armaAverage0 > threshold0) {
    digitalWrite(ledPin0, HIGH);
    servo0.write(90);
  } else {
    digitalWrite(ledPin0, LOW);
    servo0.write(0);
  }

  // === Signal 1 ===
  float inputSignal1 = analogRead(A1) + 256; 
  float outputSignal1 = b0 * inputSignal1 + b1 * inputBuffer1[0] + b2 * inputBuffer1[1]
                        - a1 * outputBuffer1[0] - a2 * outputBuffer1[1];

  inputBuffer1[1] = inputBuffer1[0];
  inputBuffer1[0] = inputSignal1;

  outputBuffer1[1] = outputBuffer1[0];
  outputBuffer1[0] = outputSignal1;

  float armaAverage1 = updateARMA(armaBuffer1, fabs(outputSignal1));

  if (armaAverage1 > threshold1) {
    digitalWrite(ledPin1, HIGH);
    servo1.write(90);
  } else {
    digitalWrite(ledPin1, LOW);
    servo1.write(0);
  }

  // === Signal 2 ===
  float inputSignal2 = analogRead(A2) + 256; 
  float outputSignal2 = b0 * inputSignal2 + b1 * inputBuffer2[0] + b2 * inputBuffer2[1]
                        - a1 * outputBuffer2[0] - a2 * outputBuffer2[1];

  inputBuffer2[1] = inputBuffer2[0];
  inputBuffer2[0] = inputSignal2;

  outputBuffer2[1] = outputBuffer2[0];
  outputBuffer2[0] = outputSignal2;

  float armaAverage2 = updateARMA(armaBuffer2, fabs(outputSignal2));

  if (armaAverage2 > threshold2) {
    digitalWrite(ledPin2, HIGH);
    servo2.write(90);
  } else {
    digitalWrite(ledPin2, LOW);
    servo2.write(0);
  }

  // === Signal 3 ===
  float inputSignal3 = analogRead(A3) + 256; 
  float outputSignal3 = b0 * inputSignal3 + b1 * inputBuffer3[0] + b2 * inputBuffer3[1]
                        - a1 * outputBuffer3[0] - a2 * outputBuffer3[1];

  inputBuffer3[1] = inputBuffer3[0];
  inputBuffer3[0] = inputSignal3;

  outputBuffer3[1] = outputBuffer3[0];
  outputBuffer3[0] = outputSignal3;

  float armaAverage3 = updateARMA(armaBuffer3, fabs(outputSignal3));

  if (armaAverage3 > threshold3) {
    digitalWrite(ledPin3, HIGH);
    servo3.write(90);
  } else {
    digitalWrite(ledPin3, LOW);
    servo3.write(0);
  }

  Serial.print(armaAverage0);
  Serial.print("\t");
  Serial.print(armaAverage1);
  Serial.print("\t");
  Serial.print(armaAverage2);
  Serial.print("\t");
  Serial.println(armaAverage3);

  delay(10); // Sampling rate ~1000 Hz
}

// Function to update ARMA moving average
float updateARMA(float buffer[], float newValue) {
  for (int i = 9; i > 0; i--) {
    buffer[i] = buffer[i - 1];
  }
  buffer[0] = newValue;

  float average = 0.0;
  for (int i = 0; i < 10; i++) {
    average += buffer[i];
  }
  return average / 10.0;
}