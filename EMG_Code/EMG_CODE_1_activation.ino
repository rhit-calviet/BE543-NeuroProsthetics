const float a0 = 1.0;
const float a1 = -1.6114;
const float a2 = 0.7159;
const float b0 = 0.3580;
const float b1 = 0.0;
const float b2 = -0.3580;

float inputBuffer[2] = {0.0, 0.0}; 
float outputBuffer[2] = {0.0, 0.0};
float armaBuffer[10] = {0.0}; // Buffer for ARMA moving average

const int ledPin = 7; // LED pin (external LED connected to pin 7)
const float threshold = 0.1; // Adjustable threshold

void setup() {
  Serial.begin(9600); 
  pinMode(ledPin, OUTPUT);
}

void loop() {
  float inputSignal = analogRead(A0) + 256; 

  // Apply IIR filter
  float outputSignal = b0 * inputSignal + b1 * inputBuffer[0] + b2 * inputBuffer[1]
                       - a1 * outputBuffer[0] - a2 * outputBuffer[1];

  // Update buffers
  inputBuffer[1] = inputBuffer[0];
  inputBuffer[0] = inputSignal;

  outputBuffer[1] = outputBuffer[0];
  outputBuffer[0] = outputSignal;

  // Compute absolute value
  float absOutput = fabs(outputSignal);

  // Compute ARMA moving average (10-sample window)
  for (int i = 9; i > 0; i--) {
    armaBuffer[i] = armaBuffer[i-1];
  }
  armaBuffer[0] = absOutput;

  float armaAverage = 0.0;
  for (int i = 0; i < 10; i++) {
    armaAverage += armaBuffer[i];
  }
  armaAverage /= 10.0;

  // Control LED based on threshold
  if (armaAverage > threshold) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Print results for Serial Plotter
  Serial.print(armaAverage);
  Serial.print("\t");
  Serial.println(threshold);

  delay(1);  // 1 ms delay, sampling rate ~1000 Hz
}
