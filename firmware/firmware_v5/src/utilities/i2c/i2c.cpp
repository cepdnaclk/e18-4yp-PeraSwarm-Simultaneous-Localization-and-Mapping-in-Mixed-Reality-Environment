#include "i2c.h"

void i2c_scan()
{
    Serial.println("I2C scanner. Scanning ...");
    byte count = 0;

    Wire.begin();
    Serial.println("...");
    for (byte i = 8; i < 120; i++)
    {
        // Serial.println (i);
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0)
        {
            Serial.print("Found address: ");
            Serial.print(i, DEC);
            Serial.print(" (0x");
            Serial.print(i, HEX);
            Serial.println(")");
            count++;
            delay(1); // maybe unneeded?
        }             // end of good response
    }                 // end of for loop

    Serial.printf("Done.......\nFound %d device(s)\n\n", count);
}