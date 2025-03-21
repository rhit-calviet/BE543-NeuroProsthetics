// Pin assignments
const int rBicepPin = 2;
const int lBicepPin = 3;
const int rQuadPin = 4;
const int lQuadPin = 5;

const int forwardLEDPin = 6;
const int backLEDPin = 7;
const int rightLEDPin = 8;
const int leftLEDPin = 9;

void setup() {
  // Set input pins
  pinMode(rBicepPin, INPUT);
  pinMode(lBicepPin, INPUT);
  pinMode(rQuadPin, INPUT);
  pinMode(lQuadPin, INPUT);

  // Set output pins
  pinMode(forwardLEDPin, OUTPUT);
  pinMode(backLEDPin, OUTPUT);
  pinMode(rightLEDPin, OUTPUT);
  pinMode(leftLEDPin, OUTPUT);

  // Ensure LEDs are off at start
  digitalWrite(forwardLEDPin, LOW);
  digitalWrite(backLEDPin, LOW);
  digitalWrite(rightLEDPin, LOW);
  digitalWrite(leftLEDPin, LOW);

  Serial.begin(9600); // For debugging
}

void loop() {
  // Read input values
  int rBicep = digitalRead(rBicepPin);
  int lBicep = digitalRead(lBicepPin);
  int rQuad = digitalRead(rQuadPin);
  int lQuad = digitalRead(lQuadPin);

  // Logic for Forward = RB OR LB
  bool forward = (rBicep || lBicep);
  
  // Logic for Back = RQ OR LQ
  bool back = (rQuad || lQuad);

  // Logic for Right = (RB AND NOT LB) OR (RQ AND NOT LQ) OR (RB AND RQ)
  bool right = (rBicep && !lBicep) || (rQuad && !lQuad) || (rBicep && rQuad);

  // Logic for Left = (LB AND NOT RB) OR (LQ AND NOT RQ) OR (LB AND LQ)
  bool left = (lBicep && !rBicep) || (lQuad && !rQuad) || (lBicep && lQuad);

  // Control the LEDs based on logic
  digitalWrite(forwardLEDPin, forward ? HIGH : LOW);
  digitalWrite(backLEDPin, back ? HIGH : LOW);
  digitalWrite(rightLEDPin, right ? HIGH : LOW);
  digitalWrite(leftLEDPin, left ? HIGH : LOW);

  // Debugging Output
  Serial.print("RB: "); Serial.print(rBicep);
  Serial.print(" LB: "); Serial.print(lBicep);
  Serial.print(" RQ: "); Serial.print(rQuad);
  Serial.print(" LQ: "); Serial.print(lQuad);
  Serial.print(" | F: "); Serial.print(forward);
  Serial.print(" B: "); Serial.print(back);
  Serial.print(" R: "); Serial.print(right);
  Serial.print(" L: "); Serial.println(left);

  delay(100); // Stability delay
}
