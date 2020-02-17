#include <Wire.h>
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

void timeSetup() {
    
    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    //--------RTC SETUP ------------
    // if you are using ESP-01 then uncomment the line below to reset the pins to
    // the available pins for SDA, SCL
    // Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL
    
    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    //printDateTime(compiled);
    Serial.println();

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
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            Serial.println("RTC lost confidence in the DateTime!");

            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            Rtc.SetDateTime(compiled);
        }
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 

}
#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}


RtcDateTime dateAddOne(int datePart, RtcDateTime cur) {
    //return RtcDateTime(cur.year(), cur.month(), cur.dayOfMonth(), cur.hour(), cur.minute(), cur.second());
    
    if (datePart==1) { //add a minute
        return (cur + 60); 
    }
    if (datePart==2) { //add an hour
        return (cur + (60UL*60UL)); 
    }
    if (datePart==3) {
        return (cur + (60UL*60UL*24UL)); //add a day
    }
    if (datePart==4) {
        return (cur + (60UL*60UL*24UL*7UL)); //add a week
    }
    if (datePart==5) {
        int mon = cur.Month()+1;
        if (mon==13) {
            return RtcDateTime(cur.Year()+1, 1, cur.Day(), cur.Hour(), cur.Minute(), cur.Second());
            }
        else {
            return RtcDateTime(cur.Year(), mon, cur.Day(), cur.Hour(), cur.Minute(), cur.Second());
            } //add a month and circle around the year if needed
    }
    if (datePart==6) {
        return RtcDateTime(cur.Year()+1, cur.Month(), cur.Day(), cur.Hour(), cur.Minute(), cur.Second()); //add a month
    }

}


RtcDateTime dateEnd(int datePart, RtcDateTime cur) {
    //return RtcDateTime(cur.year(), cur.month(), cur.dayOfMonth(), cur.hour(), cur.minute(), cur.second());
    RtcDateTime future = dateAddOne(datePart,cur);
    if (datePart==1) { //start of next minute
        return RtcDateTime(future.Year(), future.Month(), future.Day(), future.Hour(), future.Minute(), 0);
    }
    if (datePart==2) { //start of next hour
        return RtcDateTime(future.Year(), future.Month(), future.Day(), future.Hour(), 0, 0); 
    }
    if (datePart==3) { //start of next day
        return RtcDateTime(future.Year(), future.Month(), future.Day(), 0, 0, 0);
    }
    if (datePart==4) { //start of next week, starting Sunday
        future = cur + (60UL*60UL*24UL*(7-cur.DayOfWeek()));
        return RtcDateTime(future.Year(), future.Month(), future.Day(), 0, 0, 0); 
    }
    if (datePart==5) { //beginning of next month
        return RtcDateTime(future.Year(), future.Month(), 1, 0, 0, 0); 
    }
    if (datePart==6) { //beginning of next year
        return RtcDateTime(future.Year(), 1, 1, 0, 0, 0); 
    }
}
