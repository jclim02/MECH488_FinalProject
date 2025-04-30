/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>
#include <Servo.h>

// Variables
int delayTime = 500; // delay time to rotate servos
int restAngle = 90; // neutral position for servo
int redAngle = 0; // servo angle corresponding to red marble
int blueAngle = 180; // servo angle corresponding blue marble 
int debounceTime = 500; 
int solenoidTime = 5000; // Time for solenoid to be pro/retracted

// Keypad
const byte ROWS = 5; //one rows
const byte COLS = 5; //three columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'a','b','c','d','e'},
  {'f','g','h','i','j'},
  {'k','l','m','n','o'},
  {'p','q','r','s','t'},
  {'u','v','w','x','y'}
};
byte rowPins[ROWS] = {52,50,48,46,44}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {53,51,49,47,45}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// Other control buttons
const int goBtnPin = 2;
const int clrBtnPin = 4;

// LED and pattern 
int pattern[ROWS][COLS];
int ledPins[ROWS][COLS] = {
  {42,40,36,38,34},
  {43,41,37,39,35},
  {32,30,26,28,24},
  {33,31,27,29,25},
  {21,20,18,19,17}
};

// Servos
Servo servos[COLS];
int servoPins[COLS] = {9,10,11,12,13}; // make sure they're PWM

// Solenoids
int solenoidPin1 = 7;
int solenoidPin2 = 6;

void setup(){
  Serial.begin(9600);
  
  // Other control buttons
  pinMode(goBtnPin, INPUT_PULLUP);
  pinMode(clrBtnPin, INPUT_PULLUP);

  // LED and pattern
  for (int row = 0; row < ROWS; row++){
    for (int col = 0; col < COLS; col++){
      pinMode(ledPins[row][col], OUTPUT);
      digitalWrite(ledPins[row][col], LOW);
      pattern[row][col] = 0;
    }
  }

  // Servos
  for (int i = 0; i < COLS; i++) {
    servos[i].attach(servoPins[i]);
  }

  // Solenoids
  pinMode(solenoidPin1, OUTPUT);
  pinMode(solenoidPin2, OUTPUT);

  Serial.println("Ready: Press keys to select black spots. GO to activate. CLEAR to reset.");
}
  
void loop(){
  char key = keypad.getKey();
  
  if (key){
    for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
        if (hexaKeys[row][col] == key) {
          pattern[row][col] = 1;
          digitalWrite(ledPins[row][col], HIGH);
          Serial.print("Selected BLACK at [");
          Serial.print(row);
          Serial.print("][");
          Serial.print(col);
          Serial.println("]");
        }
      }
    }
  }

  // if GO button is pressed, display pattern on marbles
  if (digitalRead(goBtnPin) == LOW) {
    Serial.println("GO button pressed. Displaying pattern...");
    displayPattern();
    delay(debounceTime); // debounce
  }

  // if CLEAR button is pressed, reset pattern
  if (digitalRead(clrBtnPin) == LOW) {
    Serial.println("CLEAR button pressed. Resetting...");
    clearPattern();
    delay(debounceTime); // debounce
  }

}

void displayPattern() {
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      int val = pattern[row][col];
      int angle = (val == 1) ? redAngle : blueAngle;
      servos[col].write(angle);
      delay(delayTime); 
      servos[col].write(restAngle); // return to neutral position
      delay(delayTime);
    }
  }
  Serial.println("Done displaying.");
}

void clearPattern() {
  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < 5; col++) {
      pattern[row][col] = 0;
      digitalWrite(ledPins[row][col], LOW);
    }
  }

  // Solenoid 
  digitalWrite(solenoidPin1, HIGH);      //Switch Solenoid ON
  digitalWrite(solenoidPin2, HIGH);      //Switch Solenoid ON
  delay(solenoidTime);                          //Wait 1 Second
  digitalWrite(solenoidPin1, LOW);      //Switch Solenoid ON
  digitalWrite(solenoidPin2, LOW);      //Switch Solenoid ON
  delay(debounceTime);

  Serial.println("Pattern cleared.");
}
