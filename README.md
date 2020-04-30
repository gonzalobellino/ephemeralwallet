# Ephemeral Wallet


DYI Arduino Ethereum wallet for standalone private key generation. This project it's for fun and in development, anyway the generated keys are good enough to be used 


Tutorials and more information will be made available on [Medium](https://medium.com/@gonzalobellino).

## Why Arduino

Because is fun, cheap to build and we can generate a private key without a computer, internet or third providers.

## Features

 - Private key generation ETH & BTC (Elliptic Curve Cryptography)
 - Wallet Address generation (ETH)
 
## Future features
 - PrivateKey importer: A command line tool por recovering your private key from the Arduino Board (with user agreement)
 - Save PrivateKey in to EEPROM memory
 - Low battery detection
 
## Pending issues
 - Auditing FireFly and Entrophy libraries used in this project.

---

## Materials needed

| Part             | Cost    |
| ---------------- | --------| 
|Arduino board 2560| $40.30|
|LCD 16x2 I2C      | $2.40 |
|IR receiver + RC  | $5 |
|6-cell AA Battery holder | $2.5 |


## Optional enhancements

 - You can replace the LCD 16x2 I2C with LCD 32x2 I2C or another diplay part. This replacement will enhance the user expirience showing in a one page all characters of private key generated (64 characters)


## Supported Boards
- Just tested with Arduino Mega


## Sketch board model


![diagram](https://github.com/gonzalobellino/ephemeralwallet/raw/master/arduino/EphemeralWallet.jpg)

## Usage

Once uploaded if posible view a little menu on lcd screen.
This is a list of remote control keys with functional actions:

 - Key 1: Private Key generation
 - Key 2: View last private key generated
 - Key 3: View ethereum address
 - Key 6: View free memory
 - Key *: Go to main menu
 - Key Arrow down: Show the next 32 characters of private key
 - Key Arrow up: Show the first 32 characters of private key


> Futher information about usage will be found soon in this page [https://medium.com/@gonzalobellino](https://medium.com/@gonzalobellino)


## Installation

### prerequires

Arduino visual editor like this https://create.arduino.cc/editor or a command line client **[arduino-cli](https://github.com/arduino/arduino-cli)**

### Procedure using command-cli
 
 ```
git clone https://github.com/gonzalobellino/ephemeralwallet.git
cd ./arduino

arduino-cli config init
arduino-cli sketch new EphemeralWallet
arduino-cli core update-index
arduino-cli board list
arduino-cli core install arduino:avr
arduino-cli lib install "LiquidCrystal I2C"
arduino-cli lib install IRremote 

arduino-cli compile --fqbn arduino:avr:mega EphemeralWallet.ino --libraries ./libraries --verbose -b arduino:avr:mega

arduino-cli upload -p COM3 --fqbn arduino:avr:mega -t -i EphemeralWallet.ino.arduino.avr.mega.hex

   
 ``` 
> Futher information about usage will be found soon in this page [https://medium.com/@gonzalobellino](https://medium.com/@gonzalobellino)


## Unvaluable libraries

 **This idea was possible with this libraries**
 
* [FireFly](https://github.com/firefly/wallet)
* [micro-ecc](https://github.com/kmackay/micro-ecc)
* [Entropy](https://sites.google.com/site/astudyofentropy/the-team)

---

## Contact
Linkedin: https://www.linkedin.com/in/gonzalo-bellino-1a1877150/
