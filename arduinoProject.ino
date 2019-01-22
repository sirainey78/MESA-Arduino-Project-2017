#include <LiquidCrystal.h>
#include <Servo.h>  // servo library
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte F[8] = {
  0b00000,
  0b00001,
  0b00001,
  0b00111,
  0b00001,
  0b01111,
  0b00000,
  0b00000
};

byte locked[8] = {
  0b00000,
  0b10001,
  0b01010,
  0b00100,
  0b01010,
  0b10001,
  0b00000,
  0b00000
};

byte unlocked[8] = {
  0b00000,
  0b00000,
  0b00010,
  0b00101,
  0b01000,
  0b10000,
  0b00000,
  0b00000
};

byte A[8] = {
  0b00000,
  0b10001,
  0b10001,
  0b01110,
  0b01010,
  0b00100,
  0b00000,
  0b00000
};

byte I[8] = {
  0b00000,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b00000,
  0b00000
};

byte L[8] = {
  0b00000,
  0b01111,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00000,
  0b00000
};

byte P[8] = {
  0b00000,
  0b00001,
  0b00001,
  0b01111,
  0b01001,
  0b01111,
  0b00000,
  0b00000
};

byte S[8] = {
  0b00000,
  0b01111,
  0b01000,
  0b01111,
  0b00001,
  0b01111,
  0b00000,
  0b00000
};
Servo servo1;  // servo control object
int servoPin = 13;
int blue = 6;
int green = 7;
int red = 8;
int redB = 19;
int blueB = 18;
int greenB = 17;
int yellowB = 16;
const int buzzerPin = 9;
int songLength = 8;
char notes[] = "12345678"; // a space represents a rest
char notes2[] = "1       "; // a space represents a rest
int beats[] = {1,1,1,1,1,1,1,3};
int beats2[] = {8,1,1,1,1,1,1,1};
int tempo = 150;  
int trigPin = 14;
int echoPin = 15;
boolean test = true;
void setup() {
  Serial.begin(9600);
  servo1.attach(servoPin);
  lcd.clear();
  lcd.createChar(0, F);
  lcd.createChar(1, unlocked);
  lcd.createChar(2, locked);
  lcd.createChar(3, A);
  lcd.createChar(4, I);
  lcd.createChar(5, L);
  lcd.createChar(6, P);
  lcd.createChar(7, S);
  lcd.begin(16, 2);
  lcd.write(byte(5));
  lcd.write(byte(4));
  lcd.write(byte(3));
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(3));
  lcd.write(byte(6));
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(redB, INPUT);
  pinMode(blueB, INPUT);
  pinMode(greenB, INPUT);
  pinMode(yellowB, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  int i, duration;
  long durationUS = 0; 
  long distance = 0;
  int len = 4;
  char state[len] = "    ";
  char password[len] = "rbgy";
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
  distance = getDistance();
  Serial.println(distance);
  digitalWrite(blue, LOW); 
  boolean in = true;
  //servo1.write(0);   // Tell servo to go to 180 degrees
  //delay(1000);         // Pause to get it time to move
  while(distance > 6 && distance < 14 && in){
    digitalWrite(blue, HIGH);
    for(int i = 0; i<len; i++){
      while(state[i] == ' '){
        if(digitalRead(redB) == LOW)
          state[i] = 'r';
        else if(digitalRead(blueB) == LOW)
          state[i] = 'b';        
        else if(digitalRead(greenB) == LOW)
          state[i] = 'g';
        else if(digitalRead(yellowB) == LOW)
          state[i] = 'y';       
        for(int j = 0; j<len; j++)
          Serial.print(state[j]);  
        Serial.println(); 
      }
      delay(250);    
    }
    boolean check = true;
    for(int i = 0; i<len; i++){
      if(state[i] != password[i])
        check = false;
    }
    if(check){
      digitalWrite(green, HIGH);
      digitalWrite(blue, LOW);
       lcd.write(byte(7));
       lcd.write(byte(7));
       lcd.write(byte(3));
       lcd.write(byte(6));  
    }
    else{
      digitalWrite(red, HIGH);
      digitalWrite(blue, LOW);
      lcd.write(byte(5));
      lcd.write(byte(4));
      lcd.write(byte(3));
      lcd.write(byte(0));
    }
    if(check){
      for (i = 0; i < songLength; i++) // step through the song arrays
    {
      duration = beats[i] * tempo;  // length of note/rest in ms
  
      if (notes[i] == ' ')          // is this a rest? 
      {
        delay(duration);            // then pause for a moment
      }
      else                          // otherwise, play the note
      {
        tone(buzzerPin, frequency(notes[i]), duration);
        delay(duration);            // wait for tone to finish
      }
      delay(tempo/10);              // brief pause between notes
    }
    servo1.write(180);   // Tell servo to go to 180 degrees
    delay(1000);         // Pause to get it time to move
   } 
      else{
      for (i = 0; i < songLength; i++) // step through the song arrays
    {
      duration = beats2[i] * tempo;  // length of note/rest in ms
  
      if (notes2[i] == ' ')          // is this a rest? 
      {
        delay(duration);            // then pause for a moment
      }
      else                          // otherwise, play the note
      {
        tone(buzzerPin, frequency(notes[i]), duration);
        delay(duration);            // wait for tone to finish
      }
      delay(tempo/10);              // brief pause between notes
    }
   } 
   in = false;
  }//end of while      
  lcd.setCursor(0,1);
  // Turn off the display:
  lcd.display();
  lcd.clear();
  delay(300);
}
int frequency(char note) 
{
  // This function takes a note character (a-g), and returns the
  // corresponding frequency in Hz for the tone() function.

  int i;
  const int numNotes = 8;  // number of notes we're storing

  // The following arrays hold the note characters and their
  // corresponding frequencies. The last "C" note is uppercase
  // to separate it from the first lowercase "c". If you want to
  // add more notes, you'll need to use unique characters.

  // For the "char" (character) type, we put single characters
  // in single quotes.

  char names[] = { '1','2','3','4','5','6','7','8'}; //original
  int frequencies[] = {2*392,2*370,2*311,2*220,2*208,2*330,2*415,2*523}; //original

  // Now we'll search through the letters in the array, and if
  // we find it, we'll return the frequency for that note.

  for (i = 0; i < numNotes; i++)  // Step through the notes
  {
    if (names[i] == note)         // Is this the one?
    {
      return(frequencies[i]);     // Yes! Return the frequency
    }
  }
  return(0);  // We looked through everything and didn't find it,
              // but we still need to return a value, so return 0.
}
long getDistance()
{
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long distance = pulseIn(echoPin, HIGH)/2/29.1;   
    return distance;
}

