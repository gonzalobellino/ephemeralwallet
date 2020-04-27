#include <ethers.h>
#include <ethers.h>
#include <uECC.h>
#include <Entropy.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

// The following addresses a problem in version 1.0.5 and earlier of the
// Arduino IDE that prevents randomSeed from working properly.
//   https://github.com/arduino/Arduino/issues/575
#define randomSeed(s) srandom(s)

const unsigned long IR_ARROW_UP_KEY = 0x00FF629D;
const unsigned long IR_OK_KEY = 0x00FF02FD;
const unsigned long IR_ARROW_DOWN_KEY = 0x00FFA857;
const unsigned long IR_1_KEY = 0x00FF6897;
const unsigned long IR_2_KEY = 0x00FF9867;
const unsigned long IR_3_KEY = 0x0FFB04F;
const unsigned long IR_4_KEY = 0x00FF30CF;
const unsigned long IR_5_KEY = 0x00FF18E7;
const unsigned long IR_6_KEY = 0x00FF7A85;
const unsigned long IR_7_KEY = 0x00FF10EF;
const unsigned long IR_AST_KEY = 0x00FF42BD;

const int RECV_PIN = 11;
const struct uECC_Curve_t * curve = uECC_secp256k1();  

unsigned long optionSelected = IR_AST_KEY;
String walletAddress = "N/A";
String privateKey = "N/A";
decode_results results;   
IRrecv irrecv(RECV_PIN);

//more addresses can be defined, for example  0x3F. Use I2C scanner if none of us work 
LiquidCrystal_I2C lcd(0x27,16,2);

//Overwritted RNG function for enhance random generation using Entropy library
static int RNG(uint8_t *dest, unsigned size) {
    uint8_t seed_value;
    while (size) {
      // The random method returns an unsigned 32-bit value, which can be cast as a
      // signed value if needed.  The function will wait until sufficient entropy is
      // available to return, which could cause delays of up to approximately 500ms
      seed_value = Entropy.randomByte();
      *dest = seed_value;
      ++dest;
      --size;
    }
    return 1;
  } 

//Calculate free memory available
int freeRAM() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


String convertBytesKeyToHexKey(uint8_t bytesKey[], int size) {
  String str = String();

  for (byte i = 0; i < size; i = i + 1) {
    String int2Hex = String(bytesKey[i], HEX);
    if (int2Hex.length() ==  1) {
      int2Hex = "0" + int2Hex;
    }
    str.concat(int2Hex);
  }

  return str;
}

void showAtScreen(String msg, int line) {
  lcd.clear();
  lcd.setCursor(0,line);

  if (msg.length() > 16 && line == 0) {
    lcd.print(msg.substring(0,16));
    lcd.setCursor(0,1);    
    lcd.print(msg.substring(16));
  } else {
      lcd.print(msg);
  }
}


//@todo show all options
void lcdPrincipalMenu() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1. Generate Priv");
  lcd.setCursor(0,1);
  lcd.print("6. Show free mem");  
}

void generateWallet() {
  uint8_t pk[32];
  uint8_t pub[64];
  uint8_t *address = (uint8_t*)malloc(20);  

  randomSeed(Entropy.random());    
  uECC_make_key(pub, pk, curve);
  
  uint8_t hashed[32];
  ethers_keccak256(pub, 64, hashed);
  memcpy(address, &hashed[12], 20);
  walletAddress =  "0x" + String( convertBytesKeyToHexKey(address, 20));
  privateKey = String(convertBytesKeyToHexKey(pk, 32));

  //Serial.println(privateKey);
  //Serial.println(walletAddress);
  //Serial.println("&" + privateKey + "!"); 
  //Serial.println("#" + wallet + "$");
  //logSerial(wallet.c_str());
  //Serial.println("#" + wallet + "$");
  //Serial.end() Serial.begin() controlar desde opciones del control remoto  
}

void setup() {
  //@todo change Serial to disabled by default
  Serial.begin(115200);
  irrecv.enableIRIn(); 
  lcd.init();
  lcd.backlight();
  uECC_set_rng(&RNG);
  Entropy.initialize();
  // By using the Entropy library to seed the normal pseudo-random number generator which
  // ensures that the standard libraries random number generator will provide different starting
  // values each time the sketch is run.  This performs much better than the normal
  // randomSeed(analogRead(0)).
  randomSeed(Entropy.random());  

  lcdPrincipalMenu();
}

void loop() {
 if (irrecv.decode(&results)) {
    switch(results.value) {
      //1 Generate new privateKey
      //2 Show private key
      //3 Show wallet address
      //4 Remove new privateKey
      //5 send private key throw serial
      //6 save private key in EEPROM
      //7 show free ram
      //Arrow Up> LCD previus page
      //Arrow Down> LCD Next page
      //Ok key> Confirmation
      //code list for Sony receiver --> https://naylampmechatronics.com/blog/36_Tutorial-Arduino-y-control-remoto-Infrarrojo.html
      case IR_ARROW_UP_KEY: Serial.println("Arrow up"); 
        if (optionSelected == IR_1_KEY or optionSelected == IR_2_KEY) {
          showAtScreen(privateKey.substring(0,32),0);
        }
        break;
      case IR_OK_KEY: Serial.println("OK key");    
        break;
      case IR_ARROW_DOWN_KEY: Serial.println("Arrow down"); 
        if (optionSelected == IR_1_KEY or optionSelected == IR_2_KEY) {
          showAtScreen(privateKey.substring(32),0);
        }
        break;
      case IR_1_KEY: Serial.println("Key 1: Generate pk");    
        optionSelected = IR_1_KEY;
        lcd.blink_on();
        showAtScreen("Generating Private Key...",0);      
        generateWallet();
        lcd.blink_off();
        showAtScreen(privateKey.substring(0,32),0);
         break;
      case IR_2_KEY: Serial.println("Key 2: Show Private key");
        optionSelected = IR_2_KEY;  
        showAtScreen(privateKey.substring(0,32),0);      
        break;
      case IR_3_KEY: Serial.println("Key 3: Show wallet address");
        optionSelected = IR_3_KEY;
        showAtScreen(walletAddress,0);      
        break;
      case IR_4_KEY: Serial.println("Key 4: Send PK throw default Serial port");    
        optionSelected = IR_4_KEY;
        break;
      case IR_5_KEY: Serial.println("Key 5: Save PK in EEPROM memory");
        optionSelected = IR_5_KEY;
        break;
      case IR_6_KEY: Serial.println("Key 6: Show free memory");
        optionSelected = IR_6_KEY;
        showAtScreen("Free memory: " + String(freeRAM()),0);
        break;
      case IR_7_KEY: Serial.println("Key 7: Clean memory");
        optionSelected = IR_7_KEY;
        showAtScreen("Not implemented",0);
        break;                       
      case IR_AST_KEY: Serial.println("Key *: Goto main menu");
        optionSelected = IR_AST_KEY;
        lcdPrincipalMenu();
        break;                        
    }

    irrecv.resume();
  } 
}