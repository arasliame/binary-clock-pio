#include <Arduino.h>
#include <clockfuncs.h>
#include <ledfuncs.h>



void setup() {

    Serial.begin(9600); //debugging
    ledSetup();
    timeSetup();
   
}


void loop() {
  
//confirm RTC is still good
  if (!Rtc.IsDateTimeValid()) 
    {
        if (Rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) the battery on the device is low or even missing and the power line was disconnected
            Serial.println("RTC lost confidence in the DateTime!");
        }
    }

//calculate dates
    RtcDateTime now = Rtc.GetDateTime(); //calculate time now
    RtcDateTime future = dateEnd(6,now); //calculate date that you want to count down to
    unsigned long diffSecs = future-now; //find difference in secs

//print some stuff
    Serial.print("now:      ");
    printDateTime(now);
    Serial.println();
    Serial.print("countTo:  ");
    printDateTime(future);
    Serial.println();
    Serial.print("diffSecs: ");
    Serial.print(diffSecs);
    Serial.println();
    Serial.println();

//display to the LED matrix
  numToBits(diffSecs, matrix);
  displayBits(matrixHeight, matrix);

}
