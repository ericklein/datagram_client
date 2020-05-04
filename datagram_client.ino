/*
  Project Name:   datagram_client
  Developer:      Eric Klein Jr. (temp2@ericklein.com)
  Description:    Simple communication between 900mhz radios

  See README.md for target information, revision history, feature requests, etc.
*/

#define DEBUG       // Output to the serial port
//#define EXT_DEBUG   // Output to external LCD display

// i2c LCD connection, default address #0 (A0-A2 not jumpered)
#ifdef EXT_DEBUG
  #include <Adafruit_LiquidCrystal.h>
  #include <Wire.h>
  Adafruit_LiquidCrystal lcd(0);
#endif

// 900mhz radio
#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

// Conditional code compile for radio pinout
#if defined(ARDUINO_SAMD_ZERO)
  #define RFM69_CS      8
  #define RFM69_INT     3
  #define RFM69_RST     4
#endif

#if defined (__AVR_ATmega328P__)
  #define RFM69_INT     3
  #define RFM69_CS      4
  #define RFM69_RST     2
#endif

// server address
#define DEST_ADDRESS   1
// unique addresses for each client, can not be server address
#define MY_ADDRESS     2

// Instantiate radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);
// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);
// 400-[433]-600 for LoRa radio, 850-[868,915]-950 for 900mhz, must match transmitter
#define RF69_FREQ 915.0
// 14-20dbm for RFM69HW power
#define RF69_POWER 20
// packet counter, incremented per transmission
int16_t packetnum = 0;

void setup() 
{
  #ifdef DEBUG
    Serial.begin(115200);
    while (!Serial) 
      {
        delay(1);
      }
  #endif

  #ifdef EXT_DEBUG
    //LCD setup
    lcd.begin(16, 2);
    lcd.setBacklight(HIGH);
  #endif

  // radio setup
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

   // radio reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  if (!rf69_manager.init())
  {
    #ifdef DEBUG
      Serial.println("RFM69 radio init failed");
    #endif
    #ifdef EXT_DEBUG
      displayLCDMessage(0, "Radio init failed");
    #endif
    while (1);
  }
  
  // Default after radio init is 434.0MHz
  if (!rf69.setFrequency(RF69_FREQ))
  {
    #ifdef DEBUG
      Serial.println("RFM69 set frequency failed");
    #endif
    #ifdef EXT_DEBUG
      displayLCDMessage(0, "Can't set frequency");
    #endif
    while (1);
  }
  
  // Defaults after radio init are modulation GFSK_Rb250Fd250, +13dbM (for low power module), no encryption
  // For RFM69HW, 14-20dbm for power, 2nd arg must be true for 69HCW
  rf69.setTxPower(RF69_POWER, true);

  // The encryption key has to match the transmitter
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  
  // radio successfully set up
  #ifdef DEBUG
    Serial.print("RFM69 radio OK @ ");
    Serial.print((int)RF69_FREQ);
    Serial.println("MHz. This node is client.");
  #endif
  #ifdef EXT_DEBUG
    displayLCDMessage(0, "Radio OK, =>client");
    delay(2000);
  #endif
}

// Dont put this on the stack:
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
uint8_t data[] = "  OK";

void loop() {
  delay(1000);  // Wait 1 second between transmits, could also 'sleep' here!

  char radiopacket[20] = "Hello World #";
  itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);
  
  // Send a message to the DESTINATION!
  if (rf69_manager.sendtoWait((uint8_t *)radiopacket, strlen(radiopacket), DEST_ADDRESS)) {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;   
    if (rf69_manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
      buf[len] = 0; // zero out remaining string
      
      Serial.print("Got reply from #"); Serial.print(from);
      Serial.print(" [RSSI :");
      Serial.print(rf69.lastRssi());
      Serial.print("] : ");
      Serial.println((char*)buf);     
    } else {
      Serial.println("No reply, is anyone listening?");
    }
  } else {
    Serial.println("Sending failed (no ack)");
  }
}
#ifdef EXT_DEBUG
  void displayLCDMessage(int row, String message)
  // first row is zero
  {
    lcd.setCursor(0, row);
    lcd.print(message);
  }
#endif