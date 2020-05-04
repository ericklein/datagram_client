# datagram_client
## What is datagram_client? 
Reliable datagram communication between 900mhz radios. client is half of the solution, the other half is in the server application.

### Purpose
Validate package transmission and reception for inclusion in other solutions

### Contributors
This code extends from the Radiohead rf69 client and server example code.

### Software Dependencies
  - uses Radiohead http://www.airspayce.com/mikem/arduino/RadioHead/RadioHead-1.100.zip
    - adfruit maintains a GitHub branch at https://github.com/adafruit/RadioHead
  - uses Adafruit_LiquidCrystal https://github.com/adafruit/Adafruit_LiquidCrystal

### BOM
  - 1X each: Adafruit Arduino Pro Mini 328 - 5V (part 2378) + Adafruit FRM69HCW Radio Breakout (part 3070) + Adafruit FTDI Friend (part 284)
  - 1X: Adafruit Feather M0 RFM69HCW Packet Radio (part 3176)
  - [optional]1X: 16x2 LCD panel
  - [optional]1X: Adafruit i2c character LCD backpack (part 292)
  - 1 protoboard
  - wire

### Pinouts
  - see Fritzing diagram
  - Pro A4 (SDA) for LCD backpack
  - Pro A5 (SLC) for LCD backpack

### Information Sources
    - Adafruit RFM69HCW (SPI): https://learn.adafruit.com/adafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts?view=all
    - Sparkfun RFM69HCW: https://learn.sparkfun.com/tutorials/rfm69hcw-hookup-guide/all
    - Adafruit i2c/SPI LCD Backpack: https://learn.adafruit.com/i2c-spi-lcd-backpack/
    - https://learn.adafruit.com/adafruit-feather-m0-radio-with-rfm69-packet-radio
    - Radiohead main page: https://www.airspayce.com/mikem/arduino/RadioHead/index.html
    - 16x2 LCD panels: http://oomlout.com/parts/LCDD-01-guide.pdf
    - https://community.particle.io/t/my-attempt-at-rf95-lora/40573/3
  - https://groups.google.com/forum/#!forum/radiohead-arduino
  - https://learn.adafruit.com/adafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts?view=all

### Issues
  - 041719: code will shit a brick after 7 digit packet #
  - 040620: displayMessage will leave visual artifacts if message string decreases in size

### Questions
      - surprised iota isn't having a problem with arg3
      - do I buffer overrun or truncate when inbound packet is larger than defined buffer len?
      - Why are we constantly resetting buf, len in loop?
      - how do you determine server and client without hardcoding pairs?
      - code looks like it could send packets to multiple clients based on form of client_address declare?

### Learnings

### Feature Requests
 
### Revisions
    - 050320: first version, adapted from adapted from receiver_RX_RAW and Adafruit RadioHead69_AddrDemo_TX