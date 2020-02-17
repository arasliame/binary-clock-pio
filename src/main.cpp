#include <Arduino.h>
#include <clockfuncs.h>
#include <counting.h>



void setup() {
  
  // put your setup code here, to run once:
    
    //time_t now = Rtc.GetDateTime();
    Serial.begin(9600); //debugging
    ledSetup();
    timeSetup();

   
}


void loop() {
  
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


    RtcDateTime now = Rtc.GetDateTime();
    RtcDateTime future = dateEnd(6,now); //specify what unit you want to count to
    unsigned long diffSecs = future-now;
    Serial.print("now:      ");
    printDateTime(now);
    Serial.println();
    Serial.print("future:   ");
    printDateTime(future);
    Serial.println();
    Serial.print("diffSecs: ");
    Serial.print(diffSecs);
    Serial.println();
    Serial.println();

  numToBits(diffSecs, matrix);
  displayBits(matrixHeight, matrix);


}
