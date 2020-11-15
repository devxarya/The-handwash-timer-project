#include "pitches.h"
#define TRUE 1
#define FALSE 0
#define HANDWASHTIME 30  
#define L1SEC 10000      //DEFINE LED1 TIME HERE (10) SEC
#define L2SEC 15000
#define L3SEC 20000      
#define L4SEC 30000      //DEFINE LED4 TIME HERE (30) SEC
#define L5SEC 31000     
#define T1SEC  1000
#define PIR_LATENCY 2000

#define GREEN   3
#define YELLOW1 4
#define YELLOW2 5
#define RED1    6
#define RED2    7
#define PIR      2
#define BUZZER  9



const int death[] = {11,                                                  // Array for Death sound effect & song
   NOTE_B3, 8, NOTE_F4, 8, NOTE_H, 8, NOTE_F4, 8, NOTE_F4, 6, 
  NOTE_E4, 6, NOTE_D4, 6, NOTE_C4, 8, NOTE_E3, 8, NOTE_H, 8, NOTE_E3, 8, NOTE_C3, 8};
  
  const int flagpole[] = {8,                                               // Array for Flag pole sound effect & song
  NOTE_G2, 10, NOTE_C3, 10, NOTE_E3, 10, NOTE_G3, 10, NOTE_C4, 10, NOTE_E4, 10, NOTE_G4, 3, NOTE_E4, 2};

  const int theme[] = {11,                                                  // Array for Theme song
  NOTE_E4, 8, NOTE_E4, 8, NOTE_H, 8, NOTE_E4, 8, NOTE_H, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_H, 8, NOTE_G4, 8, NOTE_H, 3, NOTE_G3, 8};



void setup()
{
   pinMode(PIR,INPUT);
   pinMode(GREEN,OUTPUT);
   pinMode(YELLOW1,OUTPUT);
   pinMode(YELLOW2,OUTPUT);
   pinMode(RED1,OUTPUT);
   pinMode(RED2,OUTPUT);
   pinMode(BUZZER,OUTPUT);
   attachInterrupt(digitalPinToInterrupt(PIR), PIR_ISR, RISING);
   Serial.begin(9600);
}

int u16_MotionOn;
unsigned long u32_StartTime;
unsigned long u32_PreseTime;
unsigned long u32_DebounceTime;
unsigned long u32_HandWshTime;
bool b1_HandWashOn;
bool b1_motionDetect;

void PIR_ISR (void)
{
  b1_motionDetect=HIGH;
}

void loop()
{
  
  u16_MotionOn=digitalRead(PIR);
  u32_DebounceTime= millis();
  u32_DebounceTime-=u32_PreseTime;
  
  if(b1_motionDetect) 
  { 
    u32_StartTime= millis();
    b1_HandWashOn=TRUE;
    b1_motionDetect=FALSE;
    StartSound();
    while(b1_HandWashOn)
    {
      
      u32_PreseTime=millis();
      u32_HandWshTime=u32_PreseTime-u32_StartTime;
      
      
      if((0<u32_HandWshTime)&&
        (L1SEC>u32_HandWshTime))
      {
        LED_handler( RED2, RED1,0);
      }
      else if((L1SEC<u32_HandWshTime)&&
        (L2SEC>u32_HandWshTime))
      {
         LED_handler( RED1, YELLOW2,RED2);
      }
      else if((L2SEC<u32_HandWshTime)&&
        (L3SEC>u32_HandWshTime))
      {
        LED_handler( YELLOW2, YELLOW1,RED1 );
      }
       else if((L3SEC<u32_HandWshTime)&&
        (L4SEC>u32_HandWshTime))
      {
         LED_handler( YELLOW1, GREEN, YELLOW2);
      }
      else if((L4SEC<u32_HandWshTime)&&
        (L5SEC>u32_HandWshTime))
      {
         digitalWrite(GREEN,HIGH);
         digitalWrite(YELLOW1,HIGH);
          
      }
      else if(L5SEC<u32_HandWshTime )
      {
         b1_HandWashOn=FALSE;
        StopSound();
        digitalWrite(YELLOW1,LOW);
        digitalWrite(YELLOW2,LOW);
        digitalWrite(GREEN,LOW);
        digitalWrite(RED2,LOW);
        digitalWrite(RED1,LOW);
      }
      else
     {
       
     }
  
      if(b1_motionDetect)
      { 
        b1_HandWashOn=FALSE;
        b1_motionDetect=FALSE;

        HaltSound();
      }
      
    }
  
    

  delay(10); // Delay a little bit to improve simulation performance
  
  }
}

   


void LED_handler( int ON, int Blink,int OFF)
{
   static unsigned long u32_CurTime;
   static unsigned long u32_prevTime;
   static  bool b1_toggle=FALSE;
  digitalWrite(ON,HIGH);
 digitalWrite(OFF,LOW);

  u32_CurTime=millis();
//  Serial.println((u32_CurTime-u32_prevTime));

   if((u32_CurTime-u32_prevTime)>500)
   {
      Serial.println((u32_CurTime-u32_prevTime));
       digitalWrite(Blink, !digitalRead(Blink));
     u32_prevTime=u32_CurTime;
   }
 
}


bool PIRRead (void)
{
   static unsigned long u32_CurTime;
   static unsigned long u32_prevTime;
   bool b1_repsonse; 
  
   u32_CurTime=millis();
  
   if(u32_CurTime-u32_prevTime>1000)
   b1_repsonse=digitalRead(PIR);
  
   return(b1_repsonse);

}


void StartSound(void) {
     for (int thisNote = 1; thisNote < (theme[0] * 2 + 1); thisNote = thisNote + 2) 
  { // Run through the notes one at a time
      tone(BUZZER, theme[thisNote], (1000 / theme[thisNote + 1]));     // Play the single note
      delay((1000 / theme[thisNote + 1]) * 1.10);                          // Delay for the specified time
      noTone(BUZZER);                                                 // Silence the note that was playing
    }
}

void HaltSound(void){

for (int thisNote = 1; thisNote < (death[0] * 2 + 1); thisNote = thisNote + 2) { // Run through the notes one at a time
      tone(BUZZER, death[thisNote], (1000/death[thisNote + 1]));      // Play the single note
      delay((1000/death[thisNote + 1]) * 1.30);                           // Delay for the specified time
      noTone(BUZZER);                                                 // Silence the note that was playing
    }
}

void StopSound(void){
  for (int thisNote = 1; thisNote < (flagpole[0] * 2 + 1); thisNote = thisNote + 2) { // Run through the notes one at a time
      tone(BUZZER, flagpole[thisNote], (1000/flagpole[thisNote + 1]));// Play the single note
      delay((1000/flagpole[thisNote + 1]) * 1.30);                        // Delay for the specified time
      noTone(BUZZER);                                                 // Silence the note that was playing
    }
        digitalWrite(YELLOW1,LOW);
        digitalWrite(YELLOW2,LOW);
        digitalWrite(GREEN,LOW);
        
        digitalWrite(RED2,HIGH);
        digitalWrite(RED1,HIGH);
        delay(200);
        digitalWrite(RED2,LOW);
        digitalWrite(RED1,LOW);
        delay(100);
        digitalWrite(RED2,HIGH);
        digitalWrite(RED1,HIGH);
        delay(200);
        digitalWrite(RED2,LOW);
        digitalWrite(RED1,LOW);
}

