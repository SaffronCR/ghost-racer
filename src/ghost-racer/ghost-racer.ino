#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include <limits.h>

// Pin configuration.
const int pinIRSensorLine1 = 12;
const int pinIRSensorLine2 = 13;

// IR Sensor status.
int statusSensorLine1 = HIGH;
int statusSensorLine2 = HIGH;
int previousStatusLine1 = HIGH;
int previousStatusLine2 = HIGH;

// Number of laps.
int currentLapLine1 = 0;
int currentLapLine2 = 0;

// Starting time for current lap.
unsigned long startTimeLine1 = 0;
unsigned long startTimeLine2 = 0;

// Best lap time.
unsigned long bestLapTimeLine1 = ULONG_MAX;
unsigned long bestLapTimeLine2 = ULONG_MAX;

// Used to format text.
char buf[3];

// Set the LCD address to 0x27 for a 20 chars and 4 line display.
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() 
{
  Serial.begin(9600);
  
  pinMode(pinIRSensorLine1, INPUT);
  pinMode(pinIRSensorLine2, INPUT);

  // initialize the LCD.
  lcd.begin();
  
  // Turn on the blacklight.
  lcd.backlight();  

  // Clear screen.
  lcd.clear();
}

void loop()
{
  // #TODO: Everything else.

  updateStatusSensor();
}

void updateStatusSensor()
{
  // #TODO: Add sensor for Line1.
  // #TODO: Check millis overflow.

  // Check if car is above the sensor.
  statusSensorLine2 = digitalRead(pinIRSensorLine2);  
  if (statusSensorLine2 == LOW && previousStatusLine2 == HIGH)
  { 
    // Update lap count.
    currentLapLine2++;
        
    Serial.print("Current Lap: ");
    Serial.println(currentLapLine2);

    // Get lap time and check best lap time.
    if (currentLapLine2 > 1)
    {
      // Get lap time.
      unsigned long lapTime = millis() - startTimeLine2;
      unsigned long ms = lapTime;
      unsigned long seconds = lapTime / 1000;
      unsigned long minutes = seconds / 60;     
      ms %= 1000;
      seconds %= 60;
      minutes %= 60;      

      // Format lap time in human readable form.
      String textLapTime;
      sprintf(buf, "%02d", minutes);
      textLapTime += buf;
      textLapTime += ":";
      sprintf(buf, "%02d", seconds);
      textLapTime += buf;
      textLapTime += ".";      
      sprintf(buf, "%03d", ms);
      textLapTime += buf; 
      Serial.println(textLapTime);

      // Show lap time.
      lcd.setCursor (0,0);
      lcd.print("Lap Time:");
      lcd.setCursor (0,1);
      lcd.print(textLapTime);

      // Check best lap time.
      if (lapTime < bestLapTimeLine2)
      {
        bestLapTimeLine2 = lapTime;

        // Show best lap time.
        lcd.setCursor (0,2);
        lcd.print("Best Lap Time:");
        lcd.setCursor (0,3);
        lcd.print(textLapTime);
      }
    }

    // Set start time for next lap.
    startTimeLine2 = millis();
  }

  // Update status.
  previousStatusLine2 = statusSensorLine2;
}
