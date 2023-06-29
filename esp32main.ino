#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

const char* ssid = "sanjai";
const char* password = "123456789";

#define RXp2 16
#define TXp2 17
#define ID "eoic67"
#define DEVICE_TYPE "assign"
#define DEVICE_ID "12345"
#define TOKEN "18vFYEonfQ99_xzl7K"

char server[] = ID ".messaging.internetofthings.ibmcloud.com";
char publish_Topic1[] = "iot-2/evt/Data1/fmt/json";
char publish_Topic2[] = "iot-2/evt/Data2/fmt/json";
char publish_Topic3[] = "iot-2/evt/Data3/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ID ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(WiFi.localIP());

  if (!client.connected()) {
    Serial.print("Connecting client to ");
    Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    Serial.println("Connected to IBM IoT cloud!");
  }
}

void loop() {
 
  client.loop();

  // read the incoming data from the sensor
  if (Serial2.available() > 0) {
    String data = Serial2.readStringUntil('\n');
    float pHValue = 0.0;
    float turbidityValue = 0.0;
    float tdsValue = 0.0;

    // parse the data from the sensor
    if (data.startsWith("pH value:")) {
      pHValue = data.substring(9).toFloat();
      Serial.print("Received pH value: ");
      Serial.println(pHValue);
    } else if (data.startsWith("Turbidity value:")) {
      turbidityValue = data.substring(17).toFloat();
      Serial.print("Received turbidity value: ");
      Serial.println(turbidityValue);
    } else if (data.startsWith("TDS value:")) {
      tdsValue = data.substring(10).toFloat();
      Serial.print("Received TDS value: ");
      Serial.println(tdsValue);
    }

    // build the payload string and publish it to the cloud
    String payload = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
              payload += ",\"PhValue\":";
              payload += pHValue;
              payload += "}}";
              Serial.print("Sending payload: ");
              Serial.println(payload);
              Serial.println('\n');
    Serial.print("Sending payload: ");
    Serial.println(payload);

    if (client.publish(publish_Topic1, (char*) payload.c_str())) {
      Serial.println("Published successfully");
    } else {
      Serial.println("Failed");
    }
    String payload1 = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
              payload1 += ",\"TurbidityValue\":";
              payload1 += turbidityValue;
              payload1 += "}}";
              Serial.print("Sending payload: ");
              Serial.println(payload1);
              Serial.println('\n');
    Serial.print("Sending payload: ");
    Serial.println(payload1);

    if (client.publish(publish_Topic2, (char*) payload1.c_str())) {
      Serial.println("Published successfully");
    } else {
      Serial.println("Failed");
    }
    String payload2 = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
              payload2 += ",\"TdsValue\":";
              payload2 += tdsValue;
              payload2 += "}}";
              Serial.print("Sending payload: ");
              Serial.println(payload2);
              Serial.println('\n');
    Serial.print("Sending payload: ");
    Serial.println(payload2);

    if (client.publish(publish_Topic3, (char*) payload2.c_str())) {
      Serial.println("Published successfully");
    } else {
      Serial.println("Failed");
    }
    
  
  
  }

  
   
}
