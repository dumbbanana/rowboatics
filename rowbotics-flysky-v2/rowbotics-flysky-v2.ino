// Define Input Connections
#define CH1 3
#define CH2 5
#define CH3 6
#define CH4 9

// Integers to represent values from sticks and pots
int ch1Value;
int ch2Value;

int ch3Value;
int ch4Value; 

// Motor A Control Connections
#define pwmA 10
#define in1A 4
#define in2A 7

// Motor B Control Connections
#define pwmB 11
#define in1B 8
#define in2B 12

// Motor Speed Values - Start at zero
int MotorSpeedA = 0;
int MotorSpeedB = 0;

// Motor Direction Values - 0 = backward, 1 = forward
int MotorDirA = 1;
int MotorDirB = 1;

// Control Motor A
void mControlA(int mspeed, int mdir) {

  // Determine direction
  if (mdir == 0) {
    // Motor backward
    digitalWrite(in1A, LOW);
    digitalWrite(in2A, HIGH);
  } else {
    // Motor forward
    digitalWrite(in1A, HIGH);
    digitalWrite(in2A, LOW);
  }

  // Control motor
  analogWrite(pwmA, mspeed);

}

// Control Motor B
void mControlB(int mspeed, int mdir) {

  // Determine direction
  if (mdir == 0) {
    // Motor backward
    digitalWrite(in1B, LOW);
    digitalWrite(in2B, HIGH);
  } else {
    // Motor forward
    digitalWrite(in1B, HIGH);
    digitalWrite(in2B, LOW);
  }

  // Control motor
  analogWrite(pwmB, mspeed);

}

// Read the number of a specified channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Read the switch channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setup(){
  // Set up serial monitor
  Serial.begin(115200);
  
  // Set all pins as inputs
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(in1A, OUTPUT);
  pinMode(in2A, OUTPUT);
  pinMode(in1B, OUTPUT);
  pinMode(in2B, OUTPUT);
}

bool inRange1 (int ch1Value)
{
  return (ch1Value >= -10 && ch1Value <= 10);
}

bool inRange2 (int ch2Value)
{
  return (ch2Value >= -10 && ch2Value <= 10);
}

void loop() {
  
  // Get values for each channel
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch3Value = readChannel(CH3, 0, 155, 0);
  ch4Value = readChannel(CH4, -100, 100, 0);
  
  // Print to Serial Monitor
  Serial.print("Ch1: ");
  Serial.print(ch1Value);
  Serial.print(" | Ch2: ");
  Serial.print(ch2Value);
  Serial.print(" | Ch3: ");
  Serial.print(ch3Value);
  Serial.print(" | Ch4: ");
  Serial.print(ch4Value);

    // Set speeds with channel 3 value
  MotorSpeedA = ch3Value;
  MotorSpeedB = ch3Value;

  if ((inRange1(ch1Value) == 1) && (ch2Value >= 0)) {
        //Forward
        MotorDirA = 1;
        MotorDirB = 1;
        Serial.println("Forward");
      } else if ((inRange1(ch1Value) == 1) && (ch2Value < 0)) {
        //Backward
        MotorDirA = 0;
        MotorDirB = 0;
        Serial.println("Backward");
      } else if ((inRange2(ch2Value) == 1) && (ch1Value < 0)){
       //Right
        MotorDirA = 1;
        MotorDirB = 0;
        Serial.println("Right");  
      } else if ((inRange2(ch2Value) == 1) && (ch1Value > 0)){
        //Left
        MotorDirA = 0;
        MotorDirB = 1;
        Serial.println("Left");
      }

    // Add channel 2 speed
    MotorSpeedA = MotorSpeedA + abs(ch2Value) + abs(ch1Value);
    MotorSpeedB = MotorSpeedB + abs(ch2Value) + abs(ch1Value);

    // Set left/right offset with channel 4 value
    MotorSpeedA = MotorSpeedA - ch4Value;
    MotorSpeedB = MotorSpeedB + ch4Value;

  // Ensure that speeds are between 0 and 255
  MotorSpeedA = constrain(MotorSpeedA, 0, 255);
  MotorSpeedB = constrain(MotorSpeedB, 0, 255);

  //Drive Motors
  mControlA(MotorSpeedA, MotorDirA);
  mControlB(MotorSpeedB, MotorDirB);

  // Print speed values to serial monitor for debugging
  Serial.print("Motor A Speed = ");
  Serial.print(MotorSpeedA);
  Serial.print(" | Motor B Speed = ");
  Serial.println(MotorSpeedB);

  delay(50);
}