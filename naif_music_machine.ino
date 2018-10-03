#define MUSIC_STATES 5 
#define SERVOMIN 44
#define SERVOMAX 156
extern volatile unsigned long timer0_millis;

/*************************************************
 * Note play Constants
 *************************************************/

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0




#define BUTTON_PORT  PORTL
#define BUTTON_PORT2 PORTE
#define BUTTON_PORT3 PORTB
#define BUTTON_PIN   PINL
#define BUTTON_PIN2  PINE
#define BUTTON_PIN3  PINB


#define BUTTON_BIT0 PL1
#define BUTTON_BIT1 PD1
#define BUTTON_BIT2 PD2
#define BUTTON_BIT3 PD3
#define BUTTON_BIT4 PE4
#define BUTTON_BIT5 PE5
#define BUTTON_BIT6 PB4
#define BUTTON_BIT7 PB5


int axelf[] = {
  NOTE_D4, 0, NOTE_F4, NOTE_D4, 0, NOTE_D4, NOTE_G4, NOTE_D4, NOTE_C4,
  NOTE_D4, 0, NOTE_A4, NOTE_D4, 0, NOTE_D4, NOTE_AS4, NOTE_A4, NOTE_F4,
  NOTE_D4, NOTE_A4, NOTE_D5, NOTE_D4, NOTE_C4, 0, NOTE_C4, NOTE_A3, NOTE_E4, NOTE_D4,
  0};

int axelf_durations[] = {
  8, 8, 6, 16, 16, 16, 8, 8, 8, 
  8, 8, 6, 16, 16, 16, 8, 8, 8,
  8, 8, 8, 16, 16, 16, 16, 8, 8, 2,
  2};
// ref: https://github.com/nseidle/AxelF_DoorBell/wiki/How-to-convert-sheet-music-into-an-Arduino-Sketch


// notes in the melody:
int Mario_music[] = {
  NOTE_E4, NOTE_E4, REST, NOTE_E4, 
  REST, NOTE_C4, NOTE_E4, REST,
  NOTE_G4, REST, REST, NOTE_G3, REST,
  
  NOTE_C4, REST, REST, NOTE_G3,
  REST, NOTE_E3, REST,
  REST, NOTE_A3, REST, NOTE_B3,   
  REST, NOTE_AS3, NOTE_A3, REST,
  
  NOTE_G3, NOTE_E4, NOTE_G4,
  NOTE_A4, REST, NOTE_F4, NOTE_G4, 
  REST, NOTE_E4, REST, NOTE_C4, 
  NOTE_D4, NOTE_B3, REST
};

int Mario_durations[] = {
    4, 4, 4, 4,
    4, 4, 4, 4,
    4, 2, 4, 2, 2,
    
    4, 4, 4, 4,
    2, 4, 4,
    4, 4, 4, 4,  
    4, 4, 4, 4,
    
    4, 2, 4,
    4, 4, 4, 4,
    4, 4, 4, 4, 
    4, 4, 2
  };


// Mary Had a Little Lamb
int Mary_music[] = {NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4,
                NOTE_E4, NOTE_E4, NOTE_E4, 
                NOTE_D4, NOTE_D4, NOTE_D4, 
                NOTE_E4, NOTE_G4, NOTE_G4,
                NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4,
                NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
                NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4,
                NOTE_C4};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int Mary_durations[] =  {4,4,4,4,
                        4,4,2,
                        4,4,2,
                        4,4,2,
                        4,4,4,4,
                        4,4,4,4,
                        4,4,4,4,
                        1};
// ref: https://github.com/dgbrahle/Arduino/blob/master/mini_Project/miniproj1.pde

// London Bridge is Falling Down:
int London_music[] = {NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
                NOTE_E4, NOTE_F4, NOTE_G4,
                NOTE_D4, NOTE_E4, NOTE_F4,
                NOTE_E4, NOTE_F4, NOTE_G4,
                NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
                NOTE_E4, NOTE_F4, NOTE_G4,
                NOTE_D4, NOTE_G4,
                NOTE_E4, NOTE_C4};
                
int London_durations[] = {4,4,4,4,
                       4,4,2,
                       4,4,2,
                       4,4,2,
                       4,4,4,4,
                       4,4,2,
                       2,2,
                       2,2}; 


// Old MacDonald Had A Farm
int Oldmac_music[] = {NOTE_G4, NOTE_G4, NOTE_G4, NOTE_D4,
                NOTE_E4, NOTE_E4, NOTE_D4,
                NOTE_B4, NOTE_B4, NOTE_A4, NOTE_A4,
                NOTE_G4, REST, NOTE_D4,
                NOTE_G4, NOTE_G4, NOTE_G4, NOTE_D4,
                NOTE_E4, NOTE_E4, NOTE_D4,
                NOTE_B4, NOTE_B4, NOTE_A4, NOTE_A4,
                NOTE_G4};
                
int Oldmac_durations[] = {4,4,4,4,
                       4,4,2,
                       4,4,4,4,
                       2,4,4,
                       4,4,4,4,
                       4,4,2,
                       4,4,4,4,
                       2}; 


int state = 0;     
int music_state = 0;       // switches between music
int mstate = 0;            // internal loop state, switches between initial, play, rest
int servostate = 0;        // internal loop state, tries to sync with mstate

int mToneIdx;
int servomToneIdx = 0;

volatile int button = 0; //initialize button and other variables (initial value 0)
bool drums = 1, buzzer = 1, bargraph=1;
int tune_select = 1; //instruments and music selection variables

long last_millis = 0;   // deliberately use signed long to track last_millis read value
long mTimeout = 0;      // track duration to play note
long stateTimeout = 0;  // track duration before we can allow change state
long servoTimeout = 0;  // track duration before we change servo
int servoValue=80;
int servoCount = 0;

void setup() {

    // On the Arduino Mega we have 6 timers and 15 PWM outputs:
    // Pins 4 and 13: controlled by timer0 8-bit
    // Pins 11 and 12: controlled by timer1
    // Pins 10 and 9: controlled by timer2 8-bit
    // Pin 5, 2, 3: controlled by timer 3
    // Pin 6, 7 and 8: controlled by timer 4
    // Pin 44, 45, 46: controlled by timer 5
    //
    // OCRnx controls duty cycle 
    // ICRx controls timer frequency
    //
    // COMn[ABC]1=1 and COMn[ABC]0=0 sets clear on compare match
    // http://sphinx.mythic-beasts.com/~markt/ATmega-timers.html


    // buzzer pin 13
    //   <-- good. best pin to use  .
    // Servo connect to pin 7 and 8
    //   <-- yes, hopefully pin 7 and 8 but he seems to be connecting to 49,51,53
    //   <-- we alternatively use pin 5 to drive servo
    
    // Push buttons connect from pin 18 to 21
    //   <-- pin 21: PD0   20 PD1   19 PD2   18   PD3
    //       pin 2 : PE4    3 PE5
    //       In this arrangement we cover INT0-INT5    
    // 
    //   <-- button port defined as portc. but pins 18 to 21 are PD0-PD3?
    //   <-- per schema he is connecting to pin PA3,PA5,PA7
    //       uppernibble: PE4 PE5 PB4 PB5 : pin2,3,10,11    
    
    // 10 led bargraph connect from pin 0 and 9 in the left side of Arduino
    
    
    
    //S E R V O - S E T U P    <-- pin 7 and 8
    DDRH |= (1<<DDH4) | (1<<DDH5);  //setting pin#7 and pin#8 or PH4 and PH5 as timmer output for 2 servos
                                    // <-- OC4C and OC4B
    TCCR4A = 0;
    TCCR4B = 0;
    TCCR4A = (1<<COM4C1) | (1<<COM4B1)|(1<<WGM41); // setting the timmer register as it to clear OC1x on compare match
    TCCR4B = (1<<WGM43)|(1<<WGM42)|(1<<CS42); //setting FAST PWM mode/(mode#14) and prescalar set to clk/256 
    //  <-- timer4
    OCR4B = 93;  //initial duty cycle for timer4 channel B
    OCR4C = 93;  //initial duty cycle for timer4 channel C
    ICR4 = 1250; //TOP value set to 20ms or 50Hz of the timmer
    TCNT4 = 0;   //BOTTOM value set to 0 of the timmer
    // --> tmr 4,ch [BC], pin 7-8  pinout oc4b ph4 pin 7; oc4c ph5 pin 8

    
    // <-- driving pin 5. duplicate pin 8
    /*
    DDRE |= (1<<DDE3);                // Make PE3 (Pin 5of uino) output pin     
    TCCR3A = 0;
    TCCR3B = 0;
    TCCR3A = (1<<COM3A1)|(1<<WGM31); // Fast PWM mode 14, Output Compare Mode 2 for channel C
    TCCR3B = (1<<WGM33)|(1<<WGM32)|(1<<CS32); //Fast PWM mode 14, prescaler clkio/256  
    */
   
    OCR3A = 93;  // Drive SERVO1  to Neutral mid-point
    ICR3 = 1250; // Load 1250 for TOP
    TCNT3 = 0;   // Load 0 for BOT
    // --> tm3,ch A, pin 5. pinout oc3a pe3 pin 5
    // --> we can do extra also on oc4a
    //--E N D--

    //Buzzer pin 13   --> tmr 1,ch c, pin 13  pinout oc1c, pb7 pin 13
    DDRB |= (1<<DDB7);                //setting pin#13(arduino) or PB7 as timmer output for Buzzer
    TCCR1A = (1<<COM1C1)|(1<<WGM11); // setting the timer register as it to clear OC1x on compare match
    TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS12); //setting FAST PWM mode/(mode#14) and prescalar set to clk/256 
    ICR1=0;OCR1C=0;TCNT1=0;
    // --> tmr 1,ch c, pin 13  pinout oc1c, pb7 pin 13
    
    
    // PUSHBUTTON
    //PORTL &= 0xF0;
    DDRL &= 0xF0;  // set PL0, PL1 as input
    
}

int button_state = 0;
long buttonTimeout = 0;
#define BUTTONWAIT 100

int button_is_pressed(){
  if (button_state == 0) {
    if ((long)timer0_millis-servoTimeout>=0) {
      if(bit_is_set(BUTTON_PIN, BUTTON_BIT0)) { //Test twice to try and eliminate false press
        button_state = 1;
        buttonTimeout = (long)timer0_millis + BUTTONWAIT;
      } 
      else {
        button_state = 0;
        buttonTimeout = (long)timer0_millis + BUTTONWAIT;
      }      
    }    
  
  } else if (button_state == 1) {  // wait for next button check
  
    if((long)timer0_millis-servoTimeout>=0) {

      if(bit_is_set(BUTTON_PIN, BUTTON_BIT0)) {
        button_state = 2;
        buttonTimeout = (long)timer0_millis + BUTTONWAIT;
      }
      else {
        button_state = 0;
        buttonTimeout = (long)timer0_millis + BUTTONWAIT;      
      }

    }
  } else if (button_state == 2) {  // wait for next button check
  
    if((long)timer0_millis-servoTimeout>=0) {

      if(bit_is_set(BUTTON_PIN, BUTTON_BIT0)) {
        button_state = 3;
        buttonTimeout = (long)timer0_millis + BUTTONWAIT;
      }
      else {
        button_state = 0;
        buttonTimeout = (long)timer0_millis + BUTTONWAIT;      
      }

    }
  } else if (button_state == 3) {  // wait for next button check
  
    if((long)timer0_millis-servoTimeout>=0) {

      if(bit_is_set(BUTTON_PIN, BUTTON_BIT0)) {
        button_state = 0;
        buttonTimeout = (long)timer0_millis + BUTTONWAIT;
        return 1;
      }
      else {
        button_state = 0;
        buttonTimeout = (long)timer0_millis + BUTTONWAIT;      
      }

    }
  }
  return 0;
}


void move_servo_100() {
  OCR3A = 44;
  OCR4B = 44;
  OCR4C = 44;
}
void move_servo_180() {
  OCR3A = 156;
  OCR4B = 156;
  OCR4C = 156;
}

void move_servo_120() {
  OCR3A = 75;
  OCR4B = 75;
  OCR4C = 75;
}
void move_servo_160() {
  OCR3A = 125;
  OCR4B = 125;
  OCR4C = 125;
}



void moveServo(int servo_idx )
{
  int *muArray;
  int *muDurations = 0;
  int muLen;
  int min1=44,max1=156;
  
  switch (servo_idx) {
    case 0: muArray = axelf; muLen=sizeof(axelf)/sizeof(int);
      muDurations = axelf_durations;
    break;
    case 1: muArray = Mario_music; muLen=sizeof(Mario_music)/sizeof(int);
      muDurations = Mario_durations;
    break;
    case 2: muArray = Mary_music; muLen=sizeof(Mary_music)/sizeof(int); 
      muDurations = Mary_durations;
    break;
    case 3: muArray = London_music; muLen=sizeof(London_music)/sizeof(int); 
      muDurations = London_durations;
    break;    
    case 4: muArray = Oldmac_music; muLen=sizeof(Oldmac_music)/sizeof(int); 
      muDurations = Oldmac_durations;
    break;
  }
  
  int noteDuration = 1.0/muDurations[mToneIdx];
  
  
  if (drums == 1 || buzzer == 1 || bargraph == 1) // if any of instrument selected run following
  {
    if (servostate == 0) {
      servostate = 1;
      servoTimeout = (long)timer0_millis + 1000;
      servomToneIdx = mToneIdx;  // try to keep up with playing tone
      
      if (drums == 1) 
      {
          //ICR4 = 1250; //TOP value set to 20ms or 50Hz of the timer
          switch(music_state) {
            case 0:
              OCR4B = SERVOMAX;
              OCR4C = SERVOMAX;
              OCR3A = SERVOMAX;
              break;
            case 1:
              OCR4B = SERVOMAX;
              OCR4C = 0;
              OCR3A = 0;
              break;
            case 2:
              OCR4B = 0;
              OCR4C = SERVOMAX;              
              OCR3A = SERVOMAX;
              break;
            case 3:
              OCR4B = 0;
              OCR4C = 0;
              OCR3A = 0;
              break;
          }
          
        
      } else {
        OCR4B=0; OCR4C=0; OCR3A = 0;
        servostate = 0;
      }

      if (bargraph == 1) // if bargraph LEDs are selected, play bargraph LEDs.
      {
         // play_LED(1); // run LED animation 1
         int z;
      }
      
    } else if (servostate == 1) {
      servostate = 1; // wait for drum to hit
  

      if((long)timer0_millis-servoTimeout>=0) {
          servostate = 2;
          servoTimeout = (long)timer0_millis + 1000;
          
          switch(music_state) {
            case 0:
              OCR4B = SERVOMIN;
              OCR4C = SERVOMIN;
              OCR3A = 0;
              break;
            case 1:
              OCR4B = SERVOMIN;
              OCR4C = 0;
              OCR3A = 0;
              break;
            case 2:
              OCR4B = 0;
              OCR4C = SERVOMIN;
              OCR3A = SERVOMIN;
              break;
            case 3:
              OCR4B = 0;
              OCR4C = 0;
              OCR3A = 0;
              break;
          }
      }
    } // endservostate 1
    
    else if (servostate == 2) {
      servostate = 2; // wait for drumstick to retract
  
      if((long)timer0_millis-servoTimeout>=0) {

          if (servomToneIdx == mToneIdx) {
          
            //rest time 
            servoTimeout = mTimeout;  // sync with music
            OCR4B = 0;
            OCR4C = 0;
            OCR3A = 0;
            
          }
            
          else {
            servomToneIdx = mToneIdx;
            servoTimeout = (long)timer0_millis + 500;  
            servostate = 1;
            switch(music_state) {
              case 0:
                OCR4B = SERVOMAX;
                OCR4C = SERVOMAX;
                OCR3A = SERVOMAX;
                break;
              case 1:
                OCR4B = SERVOMAX;
                OCR4C = 0;
                OCR3A = 0;
                break;
              case 2:
                OCR4B = 0;
                OCR4C = SERVOMAX;
                OCR3A = SERVOMAX;
                break;
              case 3:
                OCR4B = 0;
                OCR4C = 0;
                OCR3A = 0;
                break;
            }           
            
          }
          
          
      }
    
    } // endservostate 2

    
    
    
    
  }

}


void loop() {

  if((long)timer0_millis-stateTimeout>=0) {
    button = button_is_pressed();
    if (button == 1) {
      state = (state+1) % 4;
      music_state = (music_state+1) % MUSIC_STATES;
      stateTimeout = (long)timer0_millis + 2000;
      
    } 
  }
  moveServo(music_state);
  play_music(drums,buzzer,bargraph, music_state);    
  
}

/*
ref: https://www.faludi.com/2007/12/18/arduino-millis-rollover-handling/
long eventTimeout=(long)timer0_millis+1000;
if((long)timer0_millis-eventTimeout>=0) {
eventTimeout=(long)timer0_millis+1000;
*/




void drum_music1()
{
  int z;
}




void play_music(bool drums_play, bool buzzer_play, bool bargraph_play, int music_idx )
{
  int *muArray;
  int *muDurations = 0;
  int muLen;
  
  switch (music_idx) {
    case 0: muArray = axelf; muLen=sizeof(axelf)/sizeof(int);
      muDurations = axelf_durations;
    break;
    case 1: muArray = Mario_music; muLen=sizeof(Mario_music)/sizeof(int);
      muDurations = Mario_durations;
    break;
    case 2: muArray = Mary_music; muLen=sizeof(Mary_music)/sizeof(int); 
      muDurations = Mary_durations;
    break;
    case 3: muArray = London_music; muLen=sizeof(London_music)/sizeof(int); 
      muDurations = London_durations;
    break;    
    case 4: muArray = Oldmac_music; muLen=sizeof(Oldmac_music)/sizeof(int); 
      muDurations = Oldmac_durations;
    break;
  }
  
  int noteDuration = 500.0/muDurations[mToneIdx];
  
  
  if (drums_play == 0 && buzzer_play == 0 && bargraph_play == 0) //make all enable if user has not selected any of instrument
  {
     drums_play == 1; buzzer_play == 1; bargraph_play == 1; //enable all
  }
  if (drums_play == 1 || buzzer_play == 1 || bargraph_play == 1) // if any of instrument selected run following
  {
    if (mstate == 0) {
      mstate = 1;
      mToneIdx = 0;
      mTimeout = (long)timer0_millis + noteDuration;
      
      
      if (buzzer_play == 1) // if buzzer is selected, play buzzer
      {
          ICR1 =muArray[ mToneIdx ];
          OCR1C=muArray[ mToneIdx ]/2;
          TCNT1=0;
        
      } else {
        ICR1=0;OCR1C=0;TCNT1=0;        
        mstate = 0;
      }
     
      
      

      if (drums_play == 1) // if drums are selected, play drums
      {
         //drum_music1(); // run drum sequence 1
         int z;
      }
      if (bargraph_play == 1) // if bargraph LEDs are selected, play bargraph LEDs.
      {
         // play_LED(1); // run LED animation 1
         int z;
      }
      
    } else if (mstate == 1) {
      mstate = 1; // wait for note play to finish
  

      if((long)timer0_millis-mTimeout>=0) {
        if (mToneIdx >= muLen) {
          mToneIdx = 0;
        } 
        ICR1=0;OCR1C=0;TCNT1=0;        
        mstate = 2;
        
        /*
        if (buzzer_play == 1) { // if buzzer is selected, play buzzer
          ICR1 =muArray[ mToneIdx ];
          OCR1C=muArray[ mToneIdx ]/2;
          TCNT1=0;          
          
        }
        */
        
        mTimeout = (long)timer0_millis + noteDuration*1.3;
        mToneIdx ++;
        
      }
      


      if (buzzer_play != 1) { // if buzzer is not selected, turn off buzzer
        // ICR1=0;OCR1C=0;TCNT1=0;        
        mstate = 0;
      }
     
      
      

      if (drums_play == 1) // if drums are selected, play drums
      {
         //drum_music1(); // run drum sequence 1
         int z;
      }
      if (bargraph_play == 1) // if bargraph LEDs are selected, play bargraph LEDs.
      {
          // play_LED(1); // run LED animation 1
          int z;
      }
    
    } // endmstate 1
    
    else if (mstate == 2) {
      mstate = 2; // wait for rest to finish
  

      if((long)timer0_millis-mTimeout>=0) {
        if (mToneIdx >= muLen) {
          mToneIdx = 0;
        } 
        
        if (buzzer_play == 1) { // if buzzer is selected, play buzzer
          ICR1 =muArray[ mToneIdx ];
          OCR1C=muArray[ mToneIdx ]/2;
          TCNT1=0;                    
        }
        mstate = 1;
        
        mTimeout = (long)timer0_millis + noteDuration*1.3;
        
      }
      

      if (buzzer_play != 1) { // if buzzer is not selected, turn off buzzer
        // ICR1=0;OCR1C=0;TCNT1=0;        
        mstate = 0;
      }
      

      if (drums_play == 1)  // if drums are selected, play drums
      {
         //drum_music1(); // run drum sequence 1
         int z;
      }
      if (bargraph_play == 1) // if bargraph LEDs are selected, play bargraph LEDs.
      {
          // play_LED(1); // run LED animation 1
          int z;
      }
    
    } // endmstate 2

  }

}


