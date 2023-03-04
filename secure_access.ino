#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo servo; 
LiquidCrystal_I2C lcd(0x27,16,2);

#define SS_PIN 4
#define RST_PIN 3

const int red = 10 ; 
const int green = 9 ; 
const int buzzer = 11 ;  
 int var = 0 ; 


MFRC522 rfid(SS_PIN, RST_PIN); 

// Tab ID
byte nuidPICC[4];
byte code[4]= {53 ,  58  , 21 ,  173} ; 

void setup() 
{ 
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  SPI.begin(); 

  rfid.PCD_Init(); 
 
  pinMode(red , OUTPUT);
  pinMode(green , OUTPUT);
  pinMode(buzzer , OUTPUT);
  servo.attach(8) ;
}
 
void loop() 
{
 

  if ( !rfid.PICC_IsNewCardPresent())
    return;

  // Verify the presence of new cards
  if ( !rfid.PICC_ReadCardSerial())
    return;
  
  //  saving the ID of the card 
  for (byte i = 0; i < 4; i++) 
  {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  
  // Affichage de l'ID 
  Serial.println("Un badge est détecté");
  Serial.println(" L'UID du tag est:");
  for (byte i = 0; i < 4; i++) 
  {
    Serial.print(nuidPICC[i], HEX);
    Serial.print(":");
  }
  Serial.println();

  rfid.PICC_HaltA(); 
  rfid.PCD_StopCrypto1(); 
  bool pass = true  ; 
  int i = 0 ; 
  while(pass && i < 4) 
  { 
    if(code[i] != nuidPICC[i])
    {
      pass = false ; 
    }
    i++ ; 
  }
  
  if(pass)
  {
    lcd.clear();
    lcd.print("Hello Master"); 
      digitalWrite(red , LOW) ;
      digitalWrite(green , HIGH) ;
     if( var == 100 ) 
     {
       servo.write(0) ;
       var = 0 ; 
     }
     else 
     {
       servo.write(100);
       var = 100 ; 
     }
       
       
      tone(buzzer , 100) ; 

  }
  else
  {
    lcd.clear();
    lcd.print("ACCESS DENIED !"); 
    digitalWrite(green , LOW) ;
    digitalWrite(red , HIGH) ;
    tone(buzzer , 2000) ; 

  }
  delay(1000);
  lcd.clear();
  digitalWrite(green , LOW) ;
  digitalWrite(red , LOW) ;
  noTone(buzzer)  ;
}    