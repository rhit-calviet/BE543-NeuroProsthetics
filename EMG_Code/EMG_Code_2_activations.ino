const float a0 = 1.0;
const float a1 = -1.6114;
const float a2 = 0.7159;
const float b0 = 0.3580;
const float b1 = 0.0;
const float b2 = -0.3580;

float inputBuffer1[2] = {0.0, 0.0}; 
float outputBuffer1[2] = {0.0, 0.0};
float armaBuffer1[10] = {0.0}; // Buffer for ARMA moving average

float inputBuffer2[2] = {0.0, 0.0}; 
float outputBuffer2[2] = {0.0, 0.0};
float armaBuffer2[10] = {0.0}; // Buffer for ARMA moving average
float armaSum1 = 0.0;
float armaSum2 = 0.0;

const int ledPin1 = 7; // FORWARD
const int ledPin2 = 8; // RIGHT
const int ledPin3 = 9; // LEFT
const float threshold = 0.1; // Adjustable threshold

void setup() {
  Serial.begin(9600); 
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
}

void loop() {
  float inputSignal1 = analogRead(A0) + 256; // RIGHT BICEP
  float inputSignal2 = analogRead(A1) + 256; // LEFT BICEP

  // Apply IIR filter
  float outputSignal1 = b0 * inputSignal1 + b1 * inputBuffer1[0] + b2 * inputBuffer1[1]
                       - a1 * outputBuffer1[0] - a2 * outputBuffer1[1];
  
  float outputSignal2 = b0 * inputSignal2 + b1 * inputBuffer2[0] + b2 * inputBuffer2[1]
                       - a1 * outputBuffer2[0] - a2 * outputBuffer2[1];

  // Update buffers
  updateBuffers(inputBuffer1, outputBuffer1, inputSignal1, outputSignal1);
  updateBuffers(inputBuffer2, outputBuffer2, inputSignal2, outputSignal2);

  // Compute absolute values
  float absOutput1 = fabs(outputSignal1);
  float absOutput2 = fabs(outputSignal2);

  // Update ARMA moving averages efficiently
  updateARMA(armaBuffer1, armaSum1, absOutput1);
  updateARMA(armaBuffer2, armaSum2, absOutput2);

  float armaAverage1 = armaSum1 / 10.0;
  float armaAverage2 = armaSum2 / 10.0;

  // ledPin1 // FORWARD
  // ledPin2 // RIGHT
  // ledPin3 // LEFT
  // armaAverage1 // RIGHT BICEP
  // armaAverage2 // LEFT BICEP

  // LED Control Logic
  if (armaAverage1 > threshold && armaAverage2 > threshold) {
    setLEDs(HIGH, LOW, LOW);
  } else if (armaAverage1 > threshold) {
    setLEDs(HIGH, HIGH, LOW);
  } else if (armaAverage2 > threshold) {
    setLEDs(HIGH, LOW, HIGH);
  } else {
    setLEDs(LOW, LOW, LOW);
  }

  // Print results with timestamp
  Serial.print(millis());
  Serial.print("\t");
  Serial.print(armaAverage1);
  Serial.print("\t");
  Serial.print(armaAverage2);
  Serial.print("\t");
  Serial.println(threshold);

  delay(1);  // 1 ms delay, sampling rate ~1000 Hz
}

// Helper function to update buffers
void updateBuffers(float* inputBuf, float* outputBuf, float inputSignal, float outputSignal) {
  inputBuf[1] = inputBuf[0];
  inputBuf[0] = inputSignal;
  outputBuf[1] = outputBuf[0];
  outputBuf[0] = outputSignal;
}

// Efficient ARMA Update
void updateARMA(float* buffer, float &sum, float newValue) {
  sum -= buffer[9];
  for (int i = 9; i > 0; i--) {
    buffer[i] = buffer[i-1];
  }
  buffer[0] = newValue;
  sum += newValue;
}

// Helper function to control LEDs
void setLEDs(int led1State, int led2State, int led3State) {
  digitalWrite(ledPin1, led1State);
  digitalWrite(ledPin2, led2State);
  digitalWrite(ledPin3, led3State);
}
