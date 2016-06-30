/*=======================
  Wiring:
  DHT 11: Connected to Pin D2
  BPM 180: SDA to A4, SCL to A5
  Light : Connected to A0
  Sound : Connected to A1
  ========================*/
#include <DHT.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <SoftwareSerial.h>
/*======================
  PIN and VARIABLE definition begin
=======================*/
#define DHTTYPE DHT11
#define DHTPIN 2
#define LIGHTPIN 0
#define SOUNDPIN 1
#define SERVERURL "URL"
#define APIURL "/params?"
#define PORT 3030
#define SSID "SSID"
#define PASSWORD "PASS"
float temperature, humidity, hic;
double pressure;
int light, sound;

/*======================
  PIN and VARIABLE definition end
=======================*/
/*======================
  Object definition begin
=======================*/
DHT dht(DHTPIN, DHTTYPE);
SFE_BMP180 bmp;
SoftwareSerial esp8266(6,5);
/*======================
  Object definition end
=======================*/
void setup(){
  Serial.begin(9600);
  initializeModules();
  //Connecting to WiFi
  connectWiFi();
}

void loop(){
  delay(2000);
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  hic = dht.computeHeatIndex(temperature, humidity, false);
  light = analogRead(LIGHTPIN);
  sound = analogRead(SOUNDPIN);
  /*Serial.print("Temp: ");
  Serial.println(temperature);
  Serial.print("Hum: ");
  Serial.println(humidity);
  Serial.print("Heat Index: ");
  Serial.println(hic);*/
  pressure = getPressure();
  /*Serial.print("Pres: ");
  Serial.println(pressure, 2);
  Serial.print("Light : ");
  Serial.println(light);
  Serial.print("Sound : ");
  Serial.println(sound);*/
  sendData();
}

void sendData(){
  //Connection with server
  char buffer[10];
  String command = "AT+CIPSTART=\"TCP\",\"";
  command+= SERVERURL;
  command+= "\",";
  command+= PORT;
  executeESPCommand(command, 2000);
  String request = "GET ";
  request+= APIURL;
  request+= "temp=";
  request+= dtostrf(temperature, 4, 1, buffer);
  request+= "&hum=";
  request+= dtostrf(humidity, 4, 1, buffer);
  request+= "&hic=";
  request+= dtostrf(hic, 4, 1, buffer);
  request+= "&pres=";
  request+= dtostrf(pressure, 4, 1, buffer);
  request+= "&lig=";
  request+= itoa(light, buffer, 10);
  request+= "&sou=";
  request+= itoa(sound, buffer, 10);
  request+= "\r\n\n\n\n\n\n\n\n\n\n";
  esp8266.print("AT+CIPSEND=");
  esp8266.println(request.length() - 7);
  if(esp8266.available()) // check if the esp is sending a message
  {
    while(esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      Serial.write(c);
    }
   }
  delay(500);
  esp8266.print(request);
  if(esp8266.available()){
    while(esp8266.available()){
       char c = esp8266.read();
       Serial.write(c);
    }
  }
  delay(2000);

}

void initializeModules(){
 dht.begin();
 bmp.begin();
 esp8266.begin(9600);
}

double getPressure(){
 double T, P;
 char status;
  status = bmp.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = bmp.getTemperature(T);
    if (status != 0)
    {
      status = bmp.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = bmp.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          //Serial.print("absolute pressure: ");
          //Serial.print(P,2);
          //Serial.print(" mb, ");
          //Serial.print(P*0.0295333727,2);
          //Serial.println(" inHg");
          return P;
        }
        else return -1;
      }
      else return -1;
    }
    else return -1;
  }
  else return -1;
}

void connectWiFi(){
  executeESPCommand("AT+CWMODE=1", 500);
  //String connection = "AT+CWJAP=\"" + SSID + "\",\"" + PASSWORD + "\"";
  String connection = "AT+CWJAP=\"";
  connection += SSID;
  connection += "\",\"";
  connection += PASSWORD;
  connection += "\"";
  executeESPCommand(connection, 5000);
}


void executeESPCommand(String command, int timeDelay){
  esp8266.println(command);
  if(esp8266.available()) // check if the esp is sending a message
   {
     while(esp8266.available())
      {
        // The esp has data so display its output to the serial window
        char c = esp8266.read(); // read the next character.
        Serial.write(c);
       }
    }
   delay(timeDelay);
}
