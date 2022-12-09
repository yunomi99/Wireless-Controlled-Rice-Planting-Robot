#define angkek 111
#define turun  112
#define bukak  113
#define tutuik 114

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

const unsigned int lengan[] = {115, 116, 117, 118, 119};
int arm1, arm2, arm3, arm4, arm5;

unsigned int ID1, ID2, ID3, arm_state;
unsigned int kancang, tunggu=0;
unsigned int penambah[] = {16, 8, 4, 2, 1};
unsigned int pin[] = {0, 1, 2, 3, 4};
unsigned int servo[] = {2, 3, 11, 12, 13, 21, 22, 23, 31, 32, 33, 41, 42, 43};

#define AX_BUS_SERIAL1 1

Dynamixel AX(AX_BUS_SERIAL1);

void setup(){
  AX.begin(3);
  for(int i = 0; i < 5; i++){
    pinMode(pin[i], INPUT_ANALOG);
  }
  for(int i = 0; i < 16; i++){
    AX.jointMode(servo[i]); 
  }
  AX.setPosition(2, 512 + (90*3.41), 1000);
  AX.setPosition(3, 512 - (15*3.41), 1000);
  standby();
  delay(500);
}

void loop(){
  unsigned int signal=bacoperintah();
  SerialUSB.println(signal);
  if(signal == START){standby();}
  else if(signal == R1){move_arm(lengan[1], turun); arm1 = turun;}
  else if(signal == R2){move_arm(lengan[2], turun); arm2 = turun;}
  else if(signal == L1){move_arm(lengan[3], turun); arm3 = turun;}
  else if(signal == L2){move_arm(lengan[4], turun); arm4 = turun;}
  else if(signal == R3){move_arm(lengan[5], turun); arm5 = turun;}
  
  else if(signal == TRIANGLE){
    arm1 = angkek, arm2 =angkek, arm3 =angkek, arm4 =angkek, arm5 =angkek;  
    gerak(tutuik);
    delay(300);
    gerak(angkek);}
  else if(signal == CIRCLE){
    if(arm1 == turun){move_arm(lengan[1], bukak);}
    if(arm2 == turun){move_arm(lengan[2], bukak);}
    if(arm3 == turun){move_arm(lengan[3], bukak);}
    if(arm4 == turun){move_arm(lengan[4], bukak);}
    if(arm5 == turun){move_arm(lengan[5], bukak);}
  }  
  else if(signal == SQUARE){
    if(arm1 == turun){move_arm(lengan[1], tutuik);}
    if(arm2 == turun){move_arm(lengan[2], tutuik);}
    if(arm3 == turun){move_arm(lengan[3], tutuik);}
    if(arm4 == turun){move_arm(lengan[4], tutuik);}
    if(arm5 == turun){move_arm(lengan[5], tutuik);}
  }
  else if(signal == CROSS){katiak();}
}

unsigned int bacoperintah(){
  unsigned int command=0;
  for(int i = 0; i < 5; i++){
    analogRead(pin[i]) == 4095?command+=penambah[i]:command+=0;
  }
  SerialUSB.println(command);
  return command;
}
  
void move_arm(unsigned int arm, unsigned int act){
  
if(arm == lengan[1]){ID1 = 11; ID2 = 12; ID3 = 13;}
else if(arm == lengan[2]){ID1 = 21; ID2 = 22; ID3 = 23;}
else if(arm == lengan[3]){ID1 = 31; ID2 = 32; ID3 = 33;}
else if(arm == lengan[4]){ID1 = 41; ID2 = 42; ID3 = 43;}
else if(arm == lengan[5]){ID2 = 2; ID3 = 3;}
  
if(arm == lengan[1] || arm == lengan[3]){
  if(act == angkek){AX.setPosition(ID2, 512 - (70*3.41), 1000);} 
  else if(act == turun){AX.setPosition(ID2, 512, 1000);}
  else if(act == bukak){AX.setPosition(ID3, 512 + (90*3.41), 1000);}
  else if(act == tutuik){AX.setPosition(ID3, 512, 1000);}  
  }
else if(arm == lengan[2] || arm == lengan[4]){
  if(act == angkek){AX.setPosition(ID2, 512 + (70*3.41), 1000);} 
  else if(act == turun){AX.setPosition(ID2,512, 1000);}
  else if(act == bukak){AX.setPosition(ID3, 512 - (90*3.41), 1000);}
  else if(act == tutuik){AX.setPosition(ID3, 512, 1000);}  
  }
else if(arm == lengan[5]){
  if(act == angkek){AX.setPosition(2, 512 + (80*3.41), 1000);} 
  else if(act == turun){AX.setPosition(2, 480, 1000);}
  else if(act == bukak){AX.setPosition(3, 512 + (70*3.41), 1000);}
  else if(act == tutuik){AX.setPosition(3, 512 - (15*3.41), 1000);}  
  }  
}

void gerak(int jenis){
  move_arm(lengan[1], jenis);
  move_arm(lengan[2], jenis);
  move_arm(lengan[3], jenis);
  move_arm(lengan[4], jenis);
  move_arm(lengan[5], jenis);
}

void katiak(){
  gerak(turun);
  gerak(tutuik);
  AX.setPosition(11, 512, 1000);
  AX.setPosition(21, 512, 1000);
  AX.setPosition(31, 512, 1000);
  AX.setPosition(41, 512, 1000);
}

void standby(){
  gerak(angkek);
  gerak(tutuik);
  AX.setPosition(11, 512 + (90*3.41), 1000);
  AX.setPosition(21, 512 + (90*3.41), 1000);
  AX.setPosition(31, 512 - (90*3.41), 1000);
  AX.setPosition(41, 512 - (90*3.41), 1000);
}

