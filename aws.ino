#include <WiFiClientSecure.h>
#include <MQTTClient.h>   


const char *ssid = "XXXXXXXXXX";
const char *pass = "XXXXXXXXXXXXX";

const char *awsEndPoint = "xxxxxxxxxxxxxxx-ats.iot.us-west-2.amazonaws.com";
const char *subscribeTopic = "intopic"; // Can be changed to shadow topics...
const char *publishTopic = "outtopic";

WiFiClientSecure net;
MQTTClient client;

unsigned long lastMillis = 0;

long lastMsg = 0;
char msg[50];
int value = 0;



const char* rootCABuff = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE-----\n";

// Fill with your certificate.pem.crt with LINE ENDING
const char* certificateBuff = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcN\n" \
"-----END CERTIFICATE-----\n";


// Fill with your private.pem.key with LINE ENDING
const char* privateKeyBuff = \
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"MIIDWTCCAkGgAwIBAgIUJWvukrXOlu3upOZ5NWCtzrcZH88wDQYJKoZIhvcNAQEL\n" \
"-----END RSA PRIVATE KEY-----\n";


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  net.setCACert(rootCABuff);
  net.setCertificate(certificateBuff);
  net.setPrivateKey(privateKeyBuff);

  client.begin(awsEndPoint, 8883, net);

  connect();
}

void connect() {
  Serial.print("Checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("ESP32")) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nconnected!");

  client.subscribe(subscribeTopic);
  // client.unsubscribe(subscribeTopic);
}

void loop() {

  if (!client.connected()) {
    connect();
  }
  client.loop();
  delay(100);

  if (millis() - lastMillis > 2000) {   //2 seconds non blocking delay
    lastMillis = millis();
    ++value;
    snprintf (msg, 75, "Hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(publishTopic, msg);
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}




