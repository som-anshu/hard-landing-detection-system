#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin assignments for the Ground Proximity Sensor
const int trigPin = 9;
const int echoPin = 8;

// Pin assignments for Tilt, Flex, and Force sensors
const int tiltPitchPin = 2;
const int tiltRollPin = 3;
const int flexPin = A2;
const int forcePin = A3;

// Decision threshold values
const int thresholdForce = 245;
const int thresholdFlex = 27;
const long proximityLimit = 100; // Activation distance threshold in cm

// System status tracking flags
bool hardLanding = false;
bool unstableApproach = false;

// Initialize the 16x2 LCD at I2C address 0x20
LiquidCrystal_I2C lcd(0x20, 16, 2);

void setup() {
    Serial.begin(9600);
    
    // Configure input and output pins
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(tiltPitchPin, INPUT);
    pinMode(tiltRollPin, INPUT);
    
    // Initialize LCD screen parameters
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("System Ready");
    delay(1500);
    lcd.clear();
}

void loop() {
    long duration, distance;
    
    // Trigger a clean pulse from the Ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Read the reflection echo duration time window
    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) / 29.1; // Transform time value to centimeters
    
    // Fetch raw analog readings from physical impact pads
    int forceVal = analogRead(forcePin);
    int flexVal = analogRead(flexPin);
    
    // Fetch state indications from digital tilt switches
    int pitchState = digitalRead(tiltPitchPin);
    int rollState = digitalRead(tiltRollPin);

    // Assess orientation stability profiles during flight phase
    if (distance > proximityLimit) {
        // Reset states while in the air to allow fresh landing evaluations
        hardLanding = false; 
        
        if (pitchState == HIGH || rollState == HIGH) {
            unstableApproach = true;
            lcd.setCursor(0, 0);
            lcd.print("Warning: Unstable");
            lcd.setCursor(0, 1);
            lcd.print("Approach Angle  ");
        } else {
            unstableApproach = false;
            lcd.setCursor(0, 0);
            lcd.print("Status: Airborne");
            lcd.setCursor(0, 1);
            lcd.print("Flight Stable   ");
        }
    } 
    // Execute critical impact parsing logic upon ground proximity entry
    else if (distance <= proximityLimit && !hardLanding) {
        if (flexVal <= thresholdFlex && forceVal >= thresholdForce) {
            hardLanding = true;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("CRITICAL IMPACT!");
            lcd.setCursor(0, 1);
            lcd.print("Hard Landing!   ");
            Serial.println("ALERT: Hard landing event registered!");
        } else {
            lcd.setCursor(0, 0);
            lcd.print("Status: Landed  ");
            lcd.setCursor(0, 1);
            lcd.print("Touchdown Safe  ");
        }
    }
    
    delay(200); // Master control cycle iteration delay window
}
