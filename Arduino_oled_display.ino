#include <Arduino.h>
#include <U8g2lib.h>

// Initialize the OLED display using the U8g2 library
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=/ U8X8_PIN_NONE, / clock=/ A5, / data=*/ A4);

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  // Initialize the display
  u8g2.begin();
}

void loop() {
  if (Serial.available()) {
    int input = Serial.parseInt(); // Read the input from the serial monitor

    // Clear the buffer
    u8g2.clearBuffer();

    // Set the font for the display
    u8g2.setFont(u8g2_font_ncenB08_tr);

    // Check the input and display the corresponding message
    if (input == 1) {
      u8g2.drawStr(0, 20, " Seat is occupied");
    } else if (input == 2) {
      u8g2.drawStr(0, 20, " Go for a walk");
    } else if (input == 3) {
      u8g2.drawStr(0, 20, " Reduce your stress");
    }
    else if (input == 4) {
      u8g2.drawStr(0, 20, "  Analog class at b15");
    }

    // Send the buffer to the display
    u8g2.sendBuffer();

    // Wait a bit before the next read
    delay(1000);
  }
}
