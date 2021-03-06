#include <ArduinoJson.h>
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

#define PORT 666

BridgeServer server(PORT);
DynamicJsonBuffer jsonBuffer;

void setup() {
  Serial.begin(9600);
  Bridge.begin();
  server.noListenOnLocalhost();
  server.begin();
}

void loop() {

  BridgeClient client = server.accept();
  
  if(client.connected()){
    Serial.println("Connected with Client!");
    String response;
    while(client.connected()){
      if(client.available()){
        char cmd = client.read();
        if(cmd == '\n' ||  cmd == '\0'){
          break;
        } else {
          response += String(cmd);
        }
      }
    }
    client.stop();
    Serial.println(response);
    JsonObject& root = jsonBuffer.parseObject(response);
    if (root.success()) {
      int numColors = root["color"]["count"];
      int r[numColors];
      int g[numColors];
      int b[numColors];
      int i;
      for (i = 0; i < numColors; i++) {
        r[i] = root["color"]["colors"][i]["r"];
        g[i] = root["color"]["colors"][i]["g"];
        b[i] = root["color"]["colors"][i]["b"];
      }

      // Print values.
      //Serial.println(numColors);
      //for (i = 0; i < numColors; i++) {
      //  Serial.println(r[i]);
      //}
    } else {
      Serial.println("parseObject() failed");
      return;
    }
  }

  delay(1000);
}
