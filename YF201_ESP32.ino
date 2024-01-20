
/*
  Application:
  - Interface water flow sensor with ESP32 board.
  
  Board:
  - ESP32 Dev Module
    https://my.cytron.io/p-node32-lite-wifi-and-bluetooth-development-kit
  Sensor:
  - G 1/2 Water Flow Sensor
    https://my.cytron.io/p-g-1-2-water-flow-sensor
 */

#include <WiFi.h>
#include <HTTPClient.h>
String URL = "http://192.168.0.78/fuel_monitoring_system/flow.php?";

const char* ssid = "ZTE_41ECC3";
const char* password = "2EF7AJ28XZ";

#define LED_BUILTIN 2
#define SENSOR  27
#define SENSOR2 26

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate1 ;
unsigned int flowMilliLitres1;
float totalMilliLitres1;
long currentMillis2 = 0;
long previousMillis2 = 0;
float calibrationFactor2 = 4.5;
volatile byte pulseCount2;
byte pulse1Sec2 ;
float flowRate2 ;
unsigned int flowMilliLitres2;
float totalMilliLitres2;

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}
void IRAM_ATTR pulseCounter2()
{
  pulseCount2++;
}

void setup()
{
  Serial.begin(115200);
  connectWiFi();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(14, OUTPUT);
  digitalWrite(14,HIGH);
  

  pulseCount = 0;
  flowRate1 = 0.0;
  flowMilliLitres1 = 0;
  totalMilliLitres1 = 0;
  previousMillis = 0;
  pulseCount2 = 0;
  flowRate2 = 0.0;
  flowMilliLitres2 = 0;
  totalMilliLitres2 = 0;
  previousMillis2 = 0;

  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(SENSOR2), pulseCounter2, FALLING);
}

void loop()
{
  if(WiFi.status() != WL_CONNECTED){
    connectWiFi();
  }
  String postData = URL+"flowRate1=" + String(flowRate1) + "&totalMilliLitres1=" + String(totalMilliLitres1) + "&flowRate2=" + String(flowRate2) + "&totalMilliLitres2=" +String(totalMilliLitres2);
  HTTPClient http;
  http.begin(postData.c_str());
  int httpCode = http.GET();
  String payload = http.getString();
  http.addHeader("Connect-Type" , "application/x-www-form-urlencoded");
  Serial.println("URL : "); Serial.println(URL);
  Serial.println("Data: "); Serial.println(postData);
  Serial.println("httpCode: "); Serial.println(httpCode);
  Serial.println("payload:"); Serial.println(payload);
  currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;
    pulse1Sec2 = pulseCount2;
    pulseCount2 = 0;


    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate1 = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    flowRate2 = ((1000.0 / (millis() - previousMillis2)) * pulse1Sec2) / calibrationFactor2;
    previousMillis2 = millis();

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres1 = (flowRate1 / 60) * 1000;
    flowMilliLitres2 = (flowRate2 / 60) * 1000;

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres1 += flowMilliLitres1;
    totalMilliLitres2 += flowMilliLitres2;
    
    // Print the flow rate for this second in litres / minute
    
    
  }
  



}
void connectWiFi(){
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wifi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println("Connected to : "); Serial.println(ssid);
  Serial.println("IP adsrees"); Serial.println(WiFi.localIP());

}