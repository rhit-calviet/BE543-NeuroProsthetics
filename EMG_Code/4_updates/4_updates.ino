#include <Servo.h>

// === Filter coefficients ===
float a0 = 1.0;
float a1 = -1.6114;
float a2 = 0.7159;
float b0 = 0.3580;
float b1 = 0.0;
float b2 = -0.3580;

// === EMG 0 ===
float inputBuffer0[2] = {0.0, 0.0};
float outputBuffer0[2] = {0.0, 0.0};
float rmsBuffer0[10] = {0.0};
int activationCounter0 = 0;

// === EMG 1 ===
float inputBuffer1[2] = {0.0, 0.0};
float outputBuffer1[2] = {0.0, 0.0};
float rmsBuffer1[10] = {0.0};
int activationCounter1 = 0;

// === EMG 2 ===
float inputBuffer2[2] = {0.0, 0.0};
float outputBuffer2[2] = {0.0, 0.0};
float rmsBuffer2[10] = {0.0};
int activationCounter2 = 0;

// === EMG 3 ===
float inputBuffer3[2] = {0.0, 0.0};
float outputBuffer3[2] = {0.0, 0.0};
float rmsBuffer3[10] = {0.0};
int activationCounter3 = 0;

// === Servo and LED ===
Servo myservo0;
Servo myservo1;
Servo myservo2;
Servo myservo3;

const int ledPin0 = 5;
const int ledPin1 = 6;
const int ledPin2 = 7;
const int ledPin3 = 3;

const int debounceDuration = 3;

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

  myservo0.attach(9);
  myservo1.attach(10);
  myservo2.attach(11);
  myservo3.attach(12);

  myservo0.write(0);
  myservo1.write(0);
  myservo2.write(0);
  myservo3.write(0);
}

void loop() {
  float input0 = analogRead(A0) + 256;
  float output0 = b0 * input0 + b1 * inputBuffer0[0] + b2 * inputBuffer0[1] - a1 * outputBuffer0[0] - a2 * outputBuffer0[1];
  inputBuffer0[1] = inputBuffer0[0];
  inputBuffer0[0] = input0;
  outputBuffer0[1] = outputBuffer0[0];
  outputBuffer0[0] = output0;

  float rms0 = updateRMS(rmsBuffer0, output0);
  float threshold0 = adaptiveThreshold(rmsBuffer0);
  bool activated0 = isActivated(rms0, threshold0, activationCounter0, debounceDuration);
  if (activated0) {
    digitalWrite(ledPin0, HIGH);
    myservo0.write(90);
  } else {
    digitalWrite(ledPin0, LOW);
    myservo0.write(0);
  }

  float input1 = analogRead(A1) + 256;
  float output1 = b0 * input1 + b1 * inputBuffer1[0] + b2 * inputBuffer1[1] - 
  a1 * outputBuffer1[0] - a2 * outputBuffer1[1];
  inputBuffer1[1] = inputBuffer1[0];
  inputBuffer1[0] = input1;
  outputBuffer1[1] = outputBuffer1[0];
  outputBuffer1[0] = output1;

  float rms1 = updateRMS(rmsBuffer1, output1);
  float threshold1 = adaptiveThreshold(rmsBuffer1);
  bool activated1 = isActivated(rms1, threshold1, activationCounter1, debounceDuration);
  if (activated1) {
    digitalWrite(ledPin1, HIGH);
    myservo1.write(90);
  } else {
    digitalWrite(ledPin1, LOW);
    myservo1.write(0);
  }

  float input2 = analogRead(A2) + 256;
  float output2 = b0 * input2 + b1 * inputBuffer2[0] + b2 * inputBuffer2[1] - a1 * outputBuffer2[0] - a2 * outputBuffer2[1];
  inputBuffer2[1] = inputBuffer2[0];
  inputBuffer2[0] = input2;
  outputBuffer2[1] = outputBuffer2[0];
  outputBuffer2[0] = output2;

  float rms2 = updateRMS(rmsBuffer2, output2);
  float threshold2 = adaptiveThreshold(rmsBuffer2);
  bool activated2 = isActivated(rms2, threshold2, activationCounter2, debounceDuration);
  if (activated2) {
    digitalWrite(ledPin2, HIGH);
    myservo2.write(90);
  } else {
    digitalWrite(ledPin2, LOW);
    myservo2.write(0);
  }

  float input3 = analogRead(A3) + 256;
  float output3 = b0 * input3 + b1 * inputBuffer3[0] + b2 * inputBuffer3[1] - a1 * outputBuffer3[0] - a2 * outputBuffer3[1];
  inputBuffer3[1] = inputBuffer3[0];
  inputBuffer3[0] = input3;
  outputBuffer3[1] = outputBuffer3[0];
  outputBuffer3[0] = output3;

  float rms3 = updateRMS(rmsBuffer3, output3);
  float threshold3 = adaptiveThreshold(rmsBuffer3);
  bool activated3 = isActivated(rms3, threshold3, activationCounter3, debounceDuration);
  if (activated3) {
    digitalWrite(ledPin3, HIGH);
    myservo3.write(90);
  } else {
    digitalWrite(ledPin3, LOW);
    myservo3.write(0);
  }

  Serial.print(rms0); Serial.print("\t");
  Serial.print(rms1); Serial.print("\t");
  Serial.print(rms2); Serial.print("\t");
  Serial.print(rms3); Serial.print("\n");

  delay(10);
}

// === RMS Calculation ===
float updateRMS(float buffer[], float newValue) {
  for (int i = 9; i > 0; i--) {
    buffer[i] = buffer[i - 1];
  }
  buffer[0] = newValue * newValue;

  float sum = 0.0;
  for (int i = 0; i < 10; i++) {
    sum += buffer[i];
  }

  return sqrt(sum / 10.0);
}

// === Adaptive Threshold ===
float adaptiveThreshold(float buffer[], float sensitivity = 1.5) {
  float mean = 0.0;
  for (int i = 0; i < 10; i++) {
    mean += buffer[i];
  }
  mean /= 10.0;

  float variance = 0.0;
  for (int i = 0; i < 10; i++) {
    variance += pow(buffer[i] - mean, 2);
  }
  variance /= 10.0;

  return mean + sensitivity * sqrt(variance);
}

// === Debounce Activation ===
bool isActivated(float value, float threshold, int &counter, int limit) {
  if (value > threshold) {
    counter++;
    if (counter >= limit) return true;
  } else {
    counter = 0;
  }
  return false;
}
