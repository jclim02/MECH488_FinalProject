#include <Servo.h>  // Include the Servo library

#define s0 8
#define s1 9
#define s2 10
#define s3 11
#define out 12

int data = 0;

Servo myServo;  // Create a Servo object

void setup() 
{
   pinMode(s0, OUTPUT);
   pinMode(s1, OUTPUT);
   pinMode(s2, OUTPUT);
   pinMode(s3, OUTPUT);
   pinMode(out, INPUT);

   Serial.begin(9600);
   
   digitalWrite(s0, HIGH);
   digitalWrite(s1, HIGH);

   myServo.attach(6);     // Attach the servo signal wire to pin 6
   myServo.write(90);     // Start centered (90 degrees)
}

void loop()
{
   int redValue, blueValue, greenValue;

   digitalWrite(s2, LOW);
   digitalWrite(s3, LOW);
   Serial.print("Red value= "); 
   redValue = GetData();        // Save red value

   digitalWrite(s2, LOW);
   digitalWrite(s3, HIGH);
   Serial.print("Blue value= ");
   blueValue = GetData();       // Save blue value

   digitalWrite(s2, HIGH);
   digitalWrite(s3, HIGH);
   Serial.print("Green value= ");
   greenValue = GetData();      // Save green value

   Serial.println();

   // === Servo Control directly with conditions ===
   if (redValue < 40 && blueValue < 70) {
     // This is Color A condition , COLOR A is RED: (26,58,62)
     Serial.println("Detected Color Red");
     myServo.write(0);  // Turn left 45 degrees
     delay(500);
     myServo.write(90);  // Return to neutral
   }
   else if (redValue > 40 && blueValue < 70) {
     // This is Color B condition , COLOR B is GREEN: (55,60,55)
     Serial.println("Detected Color Green");
     myServo.write(180);  // Turn right 45 degrees
     delay(500);
     myServo.write(90);  // Return to neutral
   }
   else {
     // Neither color detected
     myServo.write(90);       // Stay centered
   }

   delay(2000);
}

int GetData() {
   data = pulseIn(out, LOW);
   Serial.print(data);
   Serial.print("\t");
   delay(20);
   return data;
}
