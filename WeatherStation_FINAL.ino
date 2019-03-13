/*####################################*/
/*######    EL562  GROUP 18     ######*/
/*######    WEATHER STATION     ######*/
/*######    BEN ZENG ZXBZ2      ######*/
/*######   LIAM O'BRIEN LJO26   ######*/
/*######  RAWAZ GARMIANI RG292  ######*/
/*###### SAMUAL ADEKUNLE SOA26  ######*/
/*####################################*/
#include <string>
#include <SFE_BMP180.h> 
#include <DHT.h>
#include <Wire.h>

const int WindDirPin     = A0;
const int WindSpeedPin   = A1;
const int DHTPin         = 2;
const int RainPin        = A2;
//BMP180 sensor declarations
SFE_BMP180 pressure;
#define ALTITUDE 1655.0 
//DHT11 sensor declarations
#define DHTTYPE DHT11
DHT dht(DHTPin, DHTTYPE);

unsigned long StartTimer;
unsigned long EndTimer;
unsigned long Interval = 1000; // the repeat interval
int RainCount = 0;
int RainOld = 0;
String StringOut;
String WindDirString;
String WindSpeedString;
String RainString;
String PressureString;
String TempString;
String HumidityString;

//Setup function
void setup() 
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pressure.begin();
  dht.begin();
  pinMode (RainPin, INPUT_PULLUP);
}

void loop()
{
  GetWindDir();
  GetRainWindSpeed();
  GetPressure();
  GetHumidity();
  PrintSerial();
}

void GetHumidity()
{
  int H = dht.readHumidity();
  HumidityString = String(H, DEC);
  HumidityString.remove(3);
}

void GetPressure()
{
  char status;
  double T,P;

  status = pressure.startTemperature();
  if (status != 0)
  {

    delay(status);

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      TempString = String(T, DEC);
      TempString.remove(5);
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          PressureString = String(P, DEC);
          PressureString.remove(7);
        }
      }
    }
  }
}


void GetWindDir()
{
  int WindDirRaw    = analogRead(WindDirPin);            //wind direction reed switch is read in
  int WindDirMap    = map(WindDirRaw, 395, 990, 0, 8);

  if (WindDirRaw < 960 && WindDirRaw > 956)
    WindDirString = "315 - North West";
  else
    switch (WindDirMap) 
    {
    case 0:    // 
      WindDirString = "090 - East      ";
      break;
    case 1:    // 
      WindDirString = "135 - South East";
      break;
    case 2:    // 
      WindDirString = "180 - South     ";
      break;
    case 4:    // 
      WindDirString = "045 - North East";
      break;
    case 5:    // 
      WindDirString = "225 - South West";
      break;
    case 6:    // 
      WindDirString = "000 - North     ";
      break;
    case 7:    // 
      WindDirString = "270 - West      ";
      break;
    } 
}

void GetRainWindSpeed()
{
  int WindSpeedCount = 0;
  StartTimer = millis(); 
  EndTimer = StartTimer;
  while ((EndTimer - StartTimer) <=1000)
  {
    int WindSpeedRaw1  = analogRead(WindSpeedPin);
    if (WindSpeedRaw1 > 0)
      WindSpeedRaw1 = 1;
    int WindSpeedRaw2  = analogRead(WindSpeedPin);
    if (WindSpeedRaw2 > 0)
      WindSpeedRaw2 = 1;
    if (WindSpeedRaw1 != WindSpeedRaw2)
      WindSpeedCount++;


    int RainRaw  = digitalRead(RainPin);
    if((RainRaw == LOW) && (RainOld != RainRaw))
    {
      RainOld = RainRaw;
      RainCount++;
    }
    else
      RainOld = RainRaw;
    EndTimer = millis();
  }
  WindSpeedCount = WindSpeedCount * 1.492;
  WindSpeedString = String(WindSpeedCount, DEC);
 // RainCount = RainCount * 0.2794;
  RainString = String(RainCount * 0.2794, DEC);
  WindSpeedString.remove(2);
  RainString.remove(3);

}


void PrintSerial()
{
  StringOut = WindDirString + ", " + WindSpeedString  + "mph, " + RainString + "mm, "+ TempString + "oC, " + PressureString + "mb, " + HumidityString + "%"; // + " "   StartTimer + " " + EndTimer ;
  Serial.println(StringOut);
}



















