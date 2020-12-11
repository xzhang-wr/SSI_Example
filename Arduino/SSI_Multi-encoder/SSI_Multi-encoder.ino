#define ClockPin 7
#define DataPin1 8
#define DataPin2 9
#define DataPin3 10
#define DataPin4 11
#define DataPin5 12
#define DataPin6 13
//Connect the 1st encoder to pin 2, then 2nd to pin 3......connect the 6th encoder to pin 7.


#define tc       3 //Pulse time 
#define EncoderNumber 6// The total amount of encoders (An int less than 7)
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
  DDRB = DDRB & B11000000; //Set Pin 2~7 as INPUT, leaving Pin 0 and Pin 1 as default.
  pinMode(ClockPin,OUTPUT);
  
  digitalWrite(ClockPin, HIGH);
  delay(1000);

  //-------Encoder Problem Checker-----------
  /*  
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
  */
  Serial.println("Setting done, communication begins!");
  
}

void loop() {

  delayMicroseconds(50);
  noInterrupts();
  PORTD &= B01111111; //Set ClockPin LOW
  delayMicroseconds(tc);

  for(i=0; i<12;i++)
  {
    PORTD |= B10000000; //Set ClockPin HIGH
    delayMicroseconds(tc);
    PORTD &= B01111111; //Set ClockPin LOW
    TempData[i] = PINB;
    delayMicroseconds(tc);
  }
  PORTD |= B10000000; //Set ClockPin HIGH

  //------Data Processing:------
  
  for(i = 0; i<EncoderNumber; i++)
  {
      for(mask = 2048, j = 0; j<12; mask >>= 1, j++)
      {
        if(TempData[j] & 1<<i)
          Pos[i] = Pos[i] | mask;
       }
  }
  TempData[12] = {0};

  interrupts();
  
  for (i = 0; i<EncoderNumber; i++)
  {
    Serial.print(Pos[i]);
    Serial.print("  |  ");
  }
  
  Serial.println();
  for (i = 0; i<EncoderNumber; i++) 
  {
    d[i] = Pos[i]*360/4096;
    Pos[i] = 0;
  } 
  
  


}


