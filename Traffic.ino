#define PIR D7
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>

#define WIFI_AP "SSID"//ENTER YOUR SSID HERE
#define WIFI_PASSWORD "PASSWORD" // ENTER YOUR PASSWORD HERE
String apiserver = "http://smartrackapi.herokuapp.com/api/traffic/000003"; //Change This To Refelect Rack Number


WiFiClient wifiClient;

HTTPClient http;

int status = WL_IDLE_STATUS;
int pirState = LOW;
int val = 0;
int time_buffer = 0;
bool timer_started = false;
int stable_interval = 20000;
bool real_started = false;
int real_timer = 0;
int detection_interval = 2000;
int time_elapsed = 0;
int real_elapsed = 0;
void InitWiFi()
{
  Serial.print("Connecting to AP ");
  Serial.print(WIFI_AP);
  Serial.println(" With Password ");
  Serial.print(WIFI_PASSWORD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
  }

void setup() {
Serial.begin(115200);
  pinMode(PIR, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  InitWiFi();
 delay(100);
   checkValue();
}
void startTimer(int now) {
  if(!timer_started){
    timer_started = true;
    time_buffer = now;
    Serial.println("Wait Timer Started");
  }
}
void resetTimer (int now) {
  timer_started = false;
  time_buffer = now;
}

void realTimer(int now) {
    if(!real_started){
    real_started = true;
    real_timer = now;
    Serial.println("Actual Motion Timer Started");
  }
}
void resetRealTimer (int now) {
  real_started = false;
  real_timer = now;
}

void  checkValue()
{
  int now = millis();
  time_elapsed = now - time_buffer;
  val = digitalRead(PIR);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(BUILTIN_LED, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion Detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
      resetTimer(now);
      realTimer(now);
    }
  } else {
    digitalWrite(BUILTIN_LED, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion Ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
      startTimer(now);
    }
      if(time_elapsed > stable_interval && timer_started) {
        resetTimer(now);
        Serial.print("Sending Data: ");
        sendData();
      }
  }
}

void sendData()
{
  int now = millis();
 // Prepare a JSON payload string
 real_elapsed = now - real_timer;
 int time_in_sec = ((real_elapsed - stable_interval) / 1000);
 Serial.println(real_elapsed);
  String payload = "{";
  payload += "\"time\":"; payload += time_in_sec;
  payload += "}";
  Serial.println(payload);
   if ( WiFi.status() == WL_CONNECTED) {
    http.begin(apiserver);
    http.addHeader("Content-type","application/json");
    int httpCode = http.POST(payload);
    String response = http.getString();
    Serial.println(httpCode);
    Serial.println(response);
    http.end();
   }
   delay(5000);
   resetRealTimer(now);
}

void loop() {
// fromhere
   delay(100);
   checkValue();
  }
