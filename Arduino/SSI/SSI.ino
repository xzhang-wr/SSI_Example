#define ClockPin 13
#define DataPin1 2
#define DataPin2 3
#define DataPin3 4
#define DataPin4 5
#define DataPin5 6
#define DataPin6 7
//Connect the 1st encoder to pin 2, then 2nd to pin 3......connect the 6th encoder to pin 7.


#define tc       3 //Pulse time 
#define EncoderNumber 1// The total amount of encoders (An int less than 7)
unsigned short    Pos[6] =    {0};//Position read  
byte            TempData[12] = {0};
unsigned int    mask      ;//byte mask
int     a; // Program indicator
float   d[EncoderNumber] = {0}; //Position in Degree
int     i;
int     j;
bool    EncoderCheck[EncoderNumber] = {0}; //Encoder Initiation Checker
bool    probIn = false; //Problem Indicator;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DDRD = DDRD & B00000011; //Set Pin 2~7 as INPUT, leaving Pin 0 and Pin 1 as default.
  pinMode(ClockPin,OUTPUT);
  
  digitalWrite(ClockPin, HIGH);
  delay(1000);
  for (i = 0; i<EncoderNumber; i++) 
  {
    if(!(PIND & 1<<(2+i)))
    {
      EncoderCheck[i] = true;
      probIn = true;
    }
  }
  while(probIn)
  {
    Serial.println("The following encoders problem:");
    for (i = 0; i<EncoderNumber; i++) 
    {
      if(EncoderCheck[i])
      Serial.print(i);
      Serial.print(" ");
    }
    Serial.println(".");
  }
  Serial.println("Setting done, communication begins!");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //while(!digitalRead(DataPin));
  delayMicroseconds(50);
  noInterrupts();
  PORTB &= B11011111; //Set ClockPin LOW
  delayMicroseconds(tc);

  for(i=0; i<12;i++)
  {
    PORTB |= B00100000; //Set ClockPin HIGH
    delayMicroseconds(tc);
    PORTB &= B11011111; //Set ClockPin LOW
    TempData[i] = PIND;
    delayMicroseconds(tc);
  }
  PORTB |= B00100000; //Set ClockPin HIGH

  //------Data Processing:------
  j = 0;
  for(mask = 2048; mask>0; mask >>= 1)
  {
    for(i = 0; i<EncoderNumber; i++)
    {
      if(TempData[j] & 1<<(2+i))
      Pos[i] = Pos[i] | mask;
    }
    j++;
  }

  interrupts();

  for (i = 0; i<EncoderNumber; i++) 
  {
    d[i] = Pos[i]*360/4096;
    Pos[i] = 0;
  }
}
