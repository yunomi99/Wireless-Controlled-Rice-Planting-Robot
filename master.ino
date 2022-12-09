#include <Wire.h>
#include <PS2X_lib.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

#define PS2_DAT        11  //12   
#define PS2_CMD        12  //11
#define PS2_SEL        10  //10
#define PS2_CLK        13  //13

#define pressures   false
#define rumble      false

#define dontcare          31

#define TRIANGLE    12
#define CIRCLE      13
#define CROSS       14
#define SQUARE      15

#define R1    16
#define R2    17
#define L1    18
#define L2    19
#define R3    20
#define L3    21
#define START 22

int pin_logic[] = {7, 6, 5, 4, 3};
bool logic;

int error = 0;
byte type = 0;
byte vibrate = 0;
PS2X ps2x; // create PS2 Controller Class

int     SV1 = 2;
bool    V1=1;

void setup() {
  for(int i = 0; i < 5; i++){
    pinMode(pin_logic[i], OUTPUT);
    }
  pinMode(SV1, OUTPUT);  digitalWrite(SV1,LOW);
  Serial.begin(57600);
  delay(200);  //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
  case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
  
  Wire.begin(0x40);  //=========================================== SLAVE MOTOR 1
  Wire.begin(0x51);  //=========================================== SLAVE MOTOR 2
  Wire.begin(0x42); //============================================ SLAVE MOTOR 3
  Wire.begin(0x43); //============================================ SLAVE MOTOR 4
}

void loop(){ 
  lcd.setCursor(0,0);
  lcd.print("ROBOTIKA&OTOMASI");

  lcd.setCursor(0,1);
  lcd.print("UNP");

  lcd.setCursor(0,5);
  lcd.print(millis());
  
  if(error == 1) //skip loop if no controller found
    return; 
  if(type == 2) // not use
     return;
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    if(ps2x.Button(PSB_PAD_UP)) {Serial.println("UP"); maju();}
    else if(ps2x.Button(PSB_PAD_LEFT)) {Serial.println("LEFT"); kiri();}      
    else if(ps2x.Button(PSB_PAD_RIGHT)) {Serial.println("RIGHT"); kanan();}      
    else if(ps2x.Button(PSB_PAD_DOWN)) {Serial.println("DOWN"); mundur();}          

    else if(ps2x.Button(PSB_R1)) {Serial.println("R1"); kirim(R1);}
    else if(ps2x.Button(PSB_R2)) {Serial.println("R2"); kirim(R2);}
    else if(ps2x.Button(PSB_L1)) {Serial.println("L1"); kirim(L1);}
    else if(ps2x.Button(PSB_L2)) {Serial.println("L2"); kirim(L2);}
    else if(ps2x.Button(PSB_R3)) {Serial.println("R3"); kirim(R3);}
    else if(ps2x.Button(PSB_L3)) {Serial.println("L3"); digitalWrite(SV1, HIGH); delay(2000); digitalWrite(SV1, LOW);}
      
    else if(ps2x.Button(PSB_TRIANGLE)) {Serial.println("TRIAGLE"); kirim(TRIANGLE);}
    else if(ps2x.Button(PSB_CIRCLE)) {Serial.println("CIRCLE"); kirim(CIRCLE);}
    else if(ps2x.Button(PSB_CROSS)) {Serial.println("CROSS"); kirim(CROSS);}
    else if(ps2x.Button(PSB_SQUARE)) {Serial.println("SQUARE"); kirim(SQUARE);}

    else if(ps2x.Button(PSB_START)){Serial.print("START"); kirim(START);}

    else {
      kirim(dontcare);
      diam();
      }  
     }  
      delay(15);
  }

void kanan(){ 
    kirim(0, 200, 0x40); 
    kirim(200, 0, 0x51); 
    kirim(200, 0, 0x42); 
    kirim(0, 200, 0x43);  //kanan
  }

void kiri(){ 
    kirim(200, 0, 0x40); 
    kirim(0, 200, 0x51); 
    kirim(0, 200, 0x42);
    kirim(200, 0, 0x43);  //kiri
  }

void maju(){ 
    kirim(0, 200, 0x40); 
    kirim(200, 0, 0x51); 
    kirim(0, 200, 0x42); 
    kirim(200, 0, 0x43);  //maju
  }

void mundur(){ 
    kirim(200, 0, 0x40); 
    kirim(0, 200, 0x51); 
    kirim(2000, 0, 0x42); 
    kirim(0, 200, 0x43);  //mundur
  }

void diam(){ 
    kirim(0, 0, 0x40); 
    kirim(0, 0, 0x51); 
    kirim(0, 0, 0x42); 
    kirim(0, 0, 0x43);  //diam
  }

void relay(bool V1){
    digitalWrite(SV1,V1);
}

void kirim(int L_PWM, int R_PWM, int alamat){
 Wire.beginTransmission(alamat);   // Transmit to device store on variable alamat
 Wire.write(L_PWM);                // Sends one byte
 Wire.endTransmission();           // End transmition
 delayMicroseconds(50);
 Wire.beginTransmission(alamat);   // Transmit to device store on variable alamat
 Wire.write(R_PWM);                // Sends one byte
 Wire.endTransmission();           // End transmition
 delayMicroseconds(50);(50);
}

void kirim(int data){
for(int i = 0; i < 5; i++){
  if(data%2 == 1){logic = 1;} else{logic = 0;} 
  data /= 2;
  digitalWrite(pin_logic[i], logic);
    }
} 
