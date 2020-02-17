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

    Serial.print("now:   ");
    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);
    Serial.println();
    Serial.print("future:");
    RtcDateTime now2 = dateEnd(1,now);
    printDateTime(now2);
    Serial.println();
    unsigned long diffSecs = now2-now;
    Serial.println();
    Serial.print("diff:");
    Serial.print(diffSecs);
    Serial.println();

  resetMatrix(matrixHeight, matrix);
  countDown(5);


}
