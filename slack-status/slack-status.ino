// import slack configurations
#import "keyConfigEmpty.h"

#include <WiFi.h>
#include <WiFiMulti.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

WiFiMulti wifiMulti;

void setup() {
  Serial.begin(115200);

  // Wait for setup
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  
  // conenct to wifi network
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    // build http url
    String url = "https://slack.com/api/users.getPresence?";
    url = url + "token=" + SLACK_BOT_TOKEN;
    url = url + "&user=" + SLACK_USER;
    http.begin(url);

    // get http status code
    int httpCode = http.GET();

    // if valid status code, continue
    if (httpCode > 0) {
      
      // print out current response code, expected 200 OK
      Serial.printf("[HTTP] GET code: %d\n", httpCode);

      // get JSON for successful 200 OK response
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        handlePayload(payload);
      }
    } else { // otherwise, -1 if error
      Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(5000);
}

/**
  Called to handle the payload -- formats string to json and retreives 
  active status

  For this project we only want the presence to turn the lights on and off
*/
String handlePayload(String payload) {

  // use ArduinoJson library to deserialize string into json
  StaticJsonDocument<500> json;
  deserializeJson(json, payload);

  // print for debugging purposes
  Serial.printf(json["presence"]);
  Serial.println();
  
  return json["presence"];
}
