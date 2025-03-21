const float a0 = 1.0;
const float a1 = -1.6114;
const float a2 = 0.7159;
const float b0 = 0.3580;
const float b1 = 0.0;
const float b2 = -0.3580;

float inputBuffer[2] = {0.0, 0.0}; 
float outputBuffer[2] = {0.0, 0.0};

void setup() {
  Serial.begin(9600); 
}

void loop() {

  float inputSignal = analogRead(A0) + 256; 

 
  float outputSignal = b0 * inputSignal + b1 * inputBuffer[0] + b2 * inputBuffer[1]
                       - a1 * outputBuffer[0] - a2 * outputBuffer[1];

  inputBuffer[1] = inputBuffer[0];
  inputBuffer[0] = inputSignal;

  outputBuffer[1] = outputBuffer[0];
  outputBuffer[0] = outputSignal;

  Serial.println(outputSignal);

  delay(1);  // 1 ms delay, sampling rate ~1000 Hz
}
