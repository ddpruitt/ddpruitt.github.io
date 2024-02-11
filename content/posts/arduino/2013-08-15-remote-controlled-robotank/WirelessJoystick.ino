/*
 
  Editor     : Darren Pruitt
  Date       : 07.24.2013

  Product name: Wireless Joystick v1.1 for Arduino
  Product SKU : DFR0182
  Arduino Duemilanove w/ ATmega328P
  Code Version: 1.0

  Description:
  The sketch for using the gamepad by printing the button states and the analog values of the triggers and joysticks.
 
  Pin Mapping
  =======================
  D2:  X
  D3:  Y
  D5:  A
  D4:  B
  D6:  White
  D7:  Black
  D8:  UP
  D9:  LEFT:
  D10: DOWN
  D11: RIGHT
  D12: Back
  D13: Start
 
  X-A0,Y-A1: Left JOY
  D21: Left JOY_BUTTON
 
  X-A2,Y-A3: Right JOY
  D22: Right JOY_BUTTON
 
  A4: Left Z1
  A5: Right Z2
 
  RST : TURBO
 
*/

// Gamepad Buttons
String buttonName[14] = {"LJS","RJS","Y","X","B","A","WHITE","BLACK","UP","LEFT","DOWN","RIGHT","BACK","START"};

int triggerButton[2];
int currButtonState[14];
int prevButtonState[14];

// Gamepad Joysticks
int joystick[4];

int leftJoyStickXMidPoint, leftJoyStickYMidPoint, rightJoyStickXMidPoint, rightJoyStickYMidPoint;

void setup()
{
  Serial.begin(9600);   //Init the Serial baudrate
  InitIO();             // Initialize the inputs/outputs and the buffers
 
  // These values taken from observations of the Joysticks while
  // they were centered.
  leftJoyStickXMidPoint = 500;
  leftJoyStickYMidPoint = 503;

  rightJoyStickXMidPoint = 501;
  rightJoyStickYMidPoint = 495;
}

void InitIO(){
  for(int i = 2; i < 14; i++)  pinMode(i,INPUT);
  for(int i = 0; i < 14; i++)  prevButtonState[i] = currButtonState[i] = 0;
  for(int i = 0; i < 4; i++)   joystick[i] = 0;
  for(int i = 0; i < 2; i++)   triggerButton[i] = 0;
}

void loop()
{
  DataUpdate();
 
  printButtons();
  printJoySticks();
  printTriggers();
 
  delay(100);
}

void DataUpdate(){
 
  for(int i = 2; i < 14; i++)  currButtonState[i] = digitalRead(i);
  currButtonState[0] = analogRead(6);
  currButtonState[0] = currButtonState[0] > 100 ? 1 : 0;

  currButtonState[1] = analogRead(7);
  currButtonState[1] = currButtonState[1] > 100 ? 1 : 0;

  for(int i = 0; i < 4; i++)  joystick[i] = analogRead(i);
  for(int i = 4; i < 6; i++)  triggerButton[i-4] = analogRead(i);
 
}

void printButtons(){
  for(int i = 0; i < 14; i++)  {
    if(currButtonState[i] == 0 && prevButtonState[i] != currButtonState[i])  {
      Serial.print("$BTN: ");
      Serial.print(buttonName[i]);
      Serial.println(";");
    }
    prevButtonState[i] = currButtonState[i];
  }
}

void printJoySticks() {
  Serial.print("$LJSY: "), Serial.print(mapJoystickValue(joystick[0], leftJoyStickYMidPoint)), Serial.println(";");
  Serial.print("$LJSX: "), Serial.print(-1 * mapJoystickValue(joystick[1], leftJoyStickXMidPoint)), Serial.println(";");

  Serial.print("$RJSY: "), Serial.print(-1 * mapJoystickValue(joystick[2], rightJoyStickYMidPoint)), Serial.println(";");
  Serial.print("$RJSX: "), Serial.print(mapJoystickValue(joystick[3], rightJoyStickXMidPoint)), Serial.println(";");
}

void printTriggers() {
  Serial.print("$LTRG: ");
  Serial.print(map(constrain(triggerButton[0], 0, 800), 0, 800, 500, 0));
  Serial.println(";");
 
  Serial.print("$RTRG: ");
  Serial.print(map(constrain(triggerButton[1], 0, 800), 0, 800, 500, 0));
  Serial.println(";");
}

int mapJoystickValue(int value, int midPoint) {
  int minRange, maxRange, adjustedValue;
  minRange = midPoint - 10;
  maxRange = midPoint + 10;

  //adjustedValue = minRange < value < maxRange ? 510 : constrain(value, 0, 1020);;
  adjustedValue = value > minRange && value < maxRange ? 510 : constrain(value, 0, 1020);
  return map(adjustedValue, 0, 1020, -500, 500);
}