/* @file AppleiiiScanKBD.ino
|| @version 1.0
|| @author Stephen Gillespie
||
|| @description
|| | This emulated the Apple iii keyboard encoder
|| #
// pin connections
// arduino pin 22 -> H14 Encoder socket 7 (ASCII 7 - Always set)
// arduino pin 23 -> H14 Encoder socket 8 (ASCII 6)
// arduino pin 24 -> H14 Encoder socket 9 (ASCII 5)
// arduino pin 25 -> H14 Encoder socket 10 (ASCII 4)
// arduino pin 26 -> H14 Encoder socket 11 (ASCII 3)
// arduino pin 27 -> H14 Encoder socket 12 (ASCII 2)
// arduino pin 28 -> H14 Encoder socket 13 (ASCII 1)
// arduino pin 29 -> H14 Encoder socket 14 (ASCII 0)
// arduino pin 30 -> H14 Encoder socket 16 (DTRDY - Strobe)
// arduino pin 31 -> H14 Encoder socket 4 (Anykey)
// arduino pin GND -> H14 Encoder socket 15 (GND)
// arduino pin A15 -> H14 Encoder socket 29 (SHIFT)
// arduino pin A14 -> H14 Encoder socket 28 (CONTROL)
*/

#include <Keypad.h>

const int SHIFT=A15;
const int CONTROL=A14;

const int PIN_ASCII0=29;
const int PIN_ASCII1=28;
const int PIN_ASCII2=27;
const int PIN_ASCII3=26;
const int PIN_ASCII4=25;
const int PIN_ASCII5=24;
const int PIN_ASCII6=23;
const int PIN_ASCII7=22;

const int PIN_OUT_ANYKEY=31;
const int PIN_OUT_DTRDY=30;

const byte ROWS = 10; //four rows
const byte COLS = 8; //three columns
char keys_old[ROWS][COLS] = {
{157,137,'a','z','~','~','~','~'},
{'1','q','s','x','9','6','3','~'},
{'2','w','d','c','~','~','.',141},
{'3','e','f','v','8','5','2',' '},
{'4','r','h','b','~','~','0','~'},
{'5','t','g','n','7','4','1','-'},
{'6','y','j','m','\\','`',141,'~'},
{'7','u','k',',','=',']',139,138},
{'8','i',';','.','0','p','~',136},
{'9','o','l','/','-','[','\'',149}
};

char keys[ROWS][COLS] = {
{1,2,3,4,5,6,7,8},
{9,10,11,12,13,14,15,16},
{17,18,19,20,21,22,23,24},
{25,26,27,28,29,30,31,32},
{33,34,35,36,37,38,39,40},
{41,42,43,44,45,46,47,48},
{49,50,51,52,53,54,55,56},
{57,58,59,60,61,62,63,64},
{65,66,67,68,69,70,71,72},
{73,74,75,76,77,78,79,80}
};

char a3_ascii[81] = {
  0,157,137,193,218,0,0,0,0,177,209,211,216,185,182,179,0,178,215,196,195,0,0,174,141,179,197,198,214,184,181,178,160,180,210,200,194,0,0,176,0,181,212,199,206,183,180,177,0,182,217,202,205,220,224,141,0,183,213,203,167,189,221,139,138,184,201,186,174,176,208,0,136,185,207,204,175,173,219,167,149
};

char adtpro[288] = {
  193,176,176,176,186,193,196,160,196,198,160,198,198,160,176,185,160,198,178,160,184,196,160,196,198,160,198,198,141,
  193,176,176,184,186,193,185,160,176,194,160,184,196,160,198,178,160,195,176,160,193,185,160,177,176,160,184,196,141,
  193,176,177,176,186,198,179,160,195,176,160,193,185,160,176,176,160,193,184,160,184,181,160,183,197,160,193,185,141,
  193,176,177,184,186,193,177,160,184,181,160,183,198,160,193,178,160,180,184,160,184,197,160,178,180,160,176,180,141,
  193,176,178,176,186,197,184,160,184,197,160,178,181,160,176,180,160,178,176,160,180,176,160,193,176,160,195,185,141,
  193,176,178,184,186,180,183,160,196,176,160,198,185,160,193,178,160,176,179,160,178,176,160,180,176,160,193,176,141,
  193,176,179,176,186,185,177,160,183,197,160,184,196,160,178,183,160,176,180,160,195,184,160,196,176,160,198,181,141,
  193,176,179,184,186,197,182,160,183,198,160,195,193,160,196,176,160,198,176,160,180,195,160,176,176,160,193,177,141,
  193,176,180,176,186,193,196,160,198,177,160,195,176,160,178,185,160,182,184,160,195,185,160,176,184,160,196,176,141,
  193,176,180,184,186,198,183,160,193,196,160,198,176,160,196,176,160,182,176,141
  };

byte rowPins[ROWS] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {9, 8, 7, 6, 5, 4, 3, 2}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned long loopCount;
unsigned long startTime;
String msg;

char debugstr[100];

void setup() {
    Serial.begin(9600);
    loopCount = 0;
    startTime = millis();
    msg = "";

    pinMode(SHIFT, INPUT); 
    pinMode(CONTROL, INPUT); 

    pinMode(PIN_OUT_ANYKEY, OUTPUT); // kbd out ANYKEY
    pinMode(PIN_OUT_DTRDY, OUTPUT); // kbd out DTRDY
  
    digitalWrite(PIN_OUT_ANYKEY,0);  
    digitalWrite(PIN_OUT_DTRDY,0);  
    
    pinMode(PIN_ASCII0, OUTPUT); 
    digitalWrite(PIN_ASCII0,0);   
    pinMode(PIN_ASCII1, OUTPUT); 
    digitalWrite(PIN_ASCII0,0);   
    pinMode(PIN_ASCII1, OUTPUT); 
    digitalWrite(PIN_ASCII0,0);   
    pinMode(PIN_ASCII2, OUTPUT); 
    digitalWrite(PIN_ASCII2,0);   
    pinMode(PIN_ASCII3, OUTPUT); 
    digitalWrite(PIN_ASCII3,0);   
    pinMode(PIN_ASCII4, OUTPUT); 
    digitalWrite(PIN_ASCII4,0);   
    pinMode(PIN_ASCII5, OUTPUT); 
    digitalWrite(PIN_ASCII5,0);   
    pinMode(PIN_ASCII6, OUTPUT); 
    digitalWrite(PIN_ASCII6,0);    
    pinMode(PIN_ASCII7, OUTPUT); 
    digitalWrite(PIN_ASCII7,0);      
}


void loop() {
    loopCount++;
    if ( (millis()-startTime)>5000 ) {
        Serial.print("Average loops per second = ");
        Serial.println(loopCount/5);
        startTime = millis();
        loopCount = 0;
    }

    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                    msg = " PRESSED.";
                    if (kpd.key[i].kchar == 53) 
                        {adtprocode();} 
                    else
                        {send_char(a3_ascii[kpd.key[i].kchar]);}
                break;
                    case HOLD:
                    msg = " HOLD.";
                    send_char(a3_ascii[kpd.key[i].kchar]);
                break;
                    case RELEASED:
                    msg = " RELEASED.";
                break;
                    case IDLE:
                    msg = " IDLE.";
                }
                Serial.print("Key ");
                Serial.print(kpd.key[i].kchar);
                Serial.print(kpd.key[i].kcode);
                Serial.print(a3_ascii[kpd.key[i].kchar]);
                Serial.println(msg);
                if(!digitalRead(SHIFT)) Serial.println("SHIFT");
                if(!digitalRead(CONTROL)) Serial.println("CONTROL");
                
            }
        }
    }
}  // End loop



void send_char(char c) {

    Serial.println(c);
    // you don't actually need this most sig bit but sending it works, so why not
    digitalWrite(PIN_ASCII7,HIGH); // bit
  
    digitalWrite(PIN_ASCII6,bitRead(c,6));
    digitalWrite(PIN_ASCII5,bitRead(c,5));
    digitalWrite(PIN_ASCII4,bitRead(c,4));
    digitalWrite(PIN_ASCII3,bitRead(c,3));
    digitalWrite(PIN_ASCII2,bitRead(c,2));
    digitalWrite(PIN_ASCII1,bitRead(c,1));
    digitalWrite(PIN_ASCII0,bitRead(c,0));


    digitalWrite(PIN_OUT_ANYKEY,HIGH);
    digitalWrite(PIN_OUT_DTRDY,HIGH);
   
    delay(10);

    digitalWrite(PIN_ASCII7,LOW); // bit
  
    digitalWrite(PIN_ASCII6,LOW);
    digitalWrite(PIN_ASCII5,LOW);
    digitalWrite(PIN_ASCII4,LOW);
    digitalWrite(PIN_ASCII3,LOW);
    digitalWrite(PIN_ASCII2,LOW);
    digitalWrite(PIN_ASCII1,LOW);
    digitalWrite(PIN_ASCII0,LOW);

    digitalWrite(PIN_OUT_ANYKEY,LOW);
    digitalWrite(PIN_OUT_DTRDY,LOW);
      
    //sprintf(inChar, "send=%d c=%d", cnt++, c);
    Serial.println(bitRead(c,7));
    Serial.println(bitRead(c,6));
    Serial.println(bitRead(c,5));
    Serial.println(bitRead(c,4));
    Serial.println(bitRead(c,3));
    Serial.println(bitRead(c,2));
    Serial.println(bitRead(c,1));
    Serial.println(bitRead(c,0));
}

void adtprocode()
{
for (int i=0; i<288; i++)   // Scan the adtpro code array.
{
     send_char(adtpro[i]);
}

}
