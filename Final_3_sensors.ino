#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <HX711.h>

#define REPORTING_PERIOD_MS 1000

// PulseOximeter
PulseOximeter pox;
uint32_t tsLastReport = 0;

// HC-SR05 Ultrasonic Sensor Pins
#define TRIG_PIN 12
#define ECHO_PIN 14

// HX711 Load Cell Pins
const int LOADCELL_DOUT_PIN = 25;
const int LOADCELL_SCK_PIN = 26;
HX711 scale;

// Load Cell Calibration Factor (Adjust according to your load cell)
const float CALIBRATION_FACTOR = 2280.0; // This value is obtained from the calibration sketch

void onBeatDetected() {
    Serial.println("Beat Detected!");
}

void setup() {
    Serial.begin(115200);

    // Initialize the Pulse Oximeter
    Serial.print("Initializing pulse oximeter..");
    if (!pox.begin()) {
        Serial.println("FAILED");
        for (;;);  // Infinite loop on failure
    } else {
        Serial.println("SUCCESS");
        pox.setOnBeatDetectedCallback(onBeatDetected);
    }

    // Setup the HC-SR05 pins
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Initialize HX711 Load Cell
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(CALIBRATION_FACTOR); // Adjust to this calibration factor
    scale.tare();  // Reset the scale to 0

    Serial.println("HX711 Load Cell Initialized");
}

void loop() {
    // Update the PulseOximeter
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate: ");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2: ");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        tsLastReport = millis();
    }

    // Trigger the ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure the response from the ultrasonic sensor
    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.034 / 2;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Read Load from Load Cell
    float weight = scale.get_units(10); // Average of 10 readings
    if (weight < 0) weight = 0;  // Prevent negative readings
    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" kg");

    delay(1000); // Delay for 1 second between readings
}
