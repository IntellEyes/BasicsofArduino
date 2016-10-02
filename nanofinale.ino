#include <Wire.h>
#include <EEPROMex.h>
#include <Keypad.h>
#include "RTClib.h"

#define buzzer A0
#define redled 10
#define greenled 11
#define powerled 9
#define relayswitch 12

RTC_DS3231 rtc;
int c_id = 4;
int date_id;
long passcalc;
long passcalc1;
long passcalc2;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

String inputkeys,k1,g1,f1;
String aa,bb,cc,dd,ee,ff,gg,hh,ii;
String mm,nn,oo;
char customkey = 0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

long readLong()
{
    long cv = EEPROM.readLong(0);
    mm = String(cv);
//    Serial.println("Reading Custom Data that Written at Address 0");
//    Serial.println(mm);
    aa = mm;
    long cb = EEPROM.readLong(255);
    nn = String(cb);
//    Serial.println("Reading Custom Data that Written at Address 255");
//    Serial.println(nn);
    bb = nn;
    long cn = EEPROM.readLong(511);
    oo = String(cn);
//    Serial.println("Reading Custom Data that Written at Address 511");
//    Serial.println(cn);
    cc = oo;
}

void setup(){
  pinMode(powerled,OUTPUT);
  pinMode(relayswitch,OUTPUT);
  digitalWrite(relayswitch,LOW);
  pinMode(redled,OUTPUT);
  pinMode(greenled,OUTPUT);
  pinMode(buzzer,OUTPUT);
  //Power Led Turns On Forever
  digitalWrite(powerled,HIGH);
  Serial.begin(9600);
  // Use the Below Loop to Clear the EEPROM (Not Much Required)
/*  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }*/ 
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    //If RTC is Not Running Power LED Turns OFF
    digitalWrite(powerled,LOW);
    while (1);
  }
//    if (! rtc.isrunning()) {
//    Serial.println("RTC is NOT running!");
//    //If RTC is Not Running Power LED Turns OFF
//    digitalWrite(powerled,LOW);
//    // following line sets the RTC to the date & time this sketch was compiled
//    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//    // This line sets the RTC with an explicit date & time, for example to set
//    // January 21, 2014 at 3am you would call:
//   //rtc.adjust(DateTime(2016, 9, 07, 21, 12, 33));
//  }
   //rtc.adjust(DateTime(2016, 7, 1, 2, 51, 0));
   // Current Date and Time has been set with this Format YYYY/MM/DD HH/MM/SS
   // Uncomment above Line rtc.adjust While Setting Date and Time to the DS1307 RTC
   //DateTime dt0 (YYYY, MM, DD, HH, MM, SS);
    DateTime start_date (2016, 6, 1, 0, 0, 0);
  readLong();
}
//-----------------------------------------------------------------PASSWORD GENERATION START--------------------------------------------------------------------//
long pass_gen(){
  DateTime start_date (2016, 6, 1, 0, 0, 0);
  DateTime date_today (rtc.now());
  TimeSpan delta = date_today - start_date;
//  delay(1000);
  long date_id = delta.days();
  long local_hour = date_today.hour();
  long local_min = date_today.minute();
  long gen_hour = local_hour * 12;
  long gen_min = local_min / 5;
  long tempTimeID = gen_hour + gen_min;
  long tempTimeID1 = tempTimeID - 1;
  long tempTimeID2 = tempTimeID - 2;
  long passcalc = 100000L + ((((((date_id - 1) * 288) + tempTimeID) * 123) - 123) + 1) + (c_id * 7);
  Serial.println(passcalc);
  Serial.println();
  return passcalc;
}
long pass_gen1(){
  DateTime start_date (2016, 6, 1, 0, 0, 0);
  DateTime date_today (rtc.now());
  TimeSpan delta = date_today - start_date;
//  delay(1000);
  long date_id = delta.days();
  long local_hour = date_today.hour();
  long local_min = date_today.minute();
  long gen_hour = local_hour * 12;
  long gen_min = local_min / 5;
  long tempTimeID = gen_hour + gen_min;
  long tempTimeID1 = tempTimeID - 1;
  long tempTimeID2 = tempTimeID - 2;
  long passcalc1 = 100000L + ((((((date_id - 1) * 288) + tempTimeID1) * 123) - 123) + 1) + (c_id * 7);
  Serial.println(passcalc1);
  Serial.println();
  return passcalc1;
}
long pass_gen2(){
  DateTime start_date (2016, 6, 1, 0, 0, 0);
  DateTime date_today (rtc.now());
  TimeSpan delta = date_today - start_date;
//  delay(1000);
  long date_id = delta.days();
  long local_hour = date_today.hour();
  long local_min = date_today.minute();
  long gen_hour = local_hour * 12;
  long gen_min = local_min / 5;
  long tempTimeID = gen_hour + gen_min;
  long tempTimeID1 = tempTimeID - 1;
  long tempTimeID2 = tempTimeID - 2;
  long passcalc2 = 100000L + ((((((date_id - 1) * 288) + tempTimeID2) * 123) - 123) + 1) + (c_id * 7);
  Serial.println(passcalc2);
  Serial.println();
  return passcalc2;
}
 //-----------------------------------------------------------------PASSWORD GENERATION CLOSE--------------------------------------------------------------------//
void loop(){
  customkey = customKeypad.getKey();
  if(customkey){
    inputkeys += customkey;
    buzz();
    if(inputkeys.endsWith("#"))
    {
      int a =inputkeys.length();
      inputkeys.remove(a-1);
      checkitout();
    }   
  }
}

long checkitout(){
  Serial.println("Checked it Out");
  Serial.println(inputkeys);
  real_time();
  long k = pass_gen();
  k1 = String(k);
  long g = pass_gen1();
  g1 = String(g);
  long f = pass_gen2();
  f1 = String(f);
  if(inputkeys == aa || inputkeys == bb || inputkeys == cc || inputkeys == dd || inputkeys == ee || inputkeys == ff || inputkeys == gg || inputkeys == ii){
//    Serial.println("Already Used Keys");
    failurebuzz();
    //RedLed Blink Once;
    digitalWrite(redled,HIGH);
    delay(1000);
    digitalWrite(redled,LOW);
  }
  else if (inputkeys == k1){
//    Serial.println("Key's are Equal to Passcalc");
    successbuzz();
    ii = "";
    ii = hh;
    hh = gg;
    gg = ff;
    ff = ee;
    ee = dd;
    dd = cc;
    cc = bb;
    bb = aa;
    aa = k1;
    EEPROM.writeLong(0,0);
    EEPROM.writeLong(0,k);
    digitalWrite(relayswitch,HIGH);
    digitalWrite(greenled,HIGH);
    delay(3000);
    digitalWrite(greenled,LOW);
    digitalWrite(relayswitch,LOW);
  }
  else if (inputkeys == g1){
//    Serial.println("Key's are Equal to Passcalc1");
    successbuzz();
    ii = "";
    ii = hh;
    hh = gg;
    gg = ff;
    ff = ee;
    ee = dd;
    dd = cc;
    cc = bb;
    bb = aa;
    aa = g1;
    EEPROM.writeLong(255,0);
    EEPROM.writeLong(255,g);
    digitalWrite(relayswitch,HIGH);
    digitalWrite(greenled,HIGH);
    delay(3000);
    digitalWrite(greenled,LOW);
    digitalWrite(relayswitch,LOW);
  }
  else if (inputkeys == f1){
//    Serial.println("Key's are Equal to Passcalc2");
    successbuzz();
    ii = "";
    ii = hh;
    hh = gg;
    gg = ff;
    ff = ee;
    ee = dd;
    dd = cc;
    cc = bb;
    bb = aa;
    aa = f1;
    EEPROM.writeLong(511,0);
    EEPROM.writeLong(511,f);
    digitalWrite(relayswitch,HIGH);
    digitalWrite(greenled,HIGH);
    delay(3000);
    digitalWrite(greenled,LOW);
    digitalWrite(relayswitch,LOW);
  }
  else
  {
//    Serial.println("Key's Doesn't Matched with any of Password Calculations");
    failurebuzz();
    digitalWrite(redled,HIGH);
    delay(1000);
    digitalWrite(redled,LOW);
  }
  inputkeys ="";
}
void real_time() {
    DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.println();
    delay(1000);
}
void buzz(){
  digitalWrite(buzzer,HIGH);
  delay(30);
  digitalWrite(buzzer,LOW);
}
void successbuzz(){
  digitalWrite(buzzer,HIGH);
  delay(450);
  digitalWrite(buzzer,LOW);
}
void failurebuzz(){
  digitalWrite(buzzer,HIGH);
  delay(200);
  digitalWrite(buzzer,LOW);
  delay(150);
  digitalWrite(buzzer,HIGH);
  delay(200);
  digitalWrite(buzzer,LOW);
}
