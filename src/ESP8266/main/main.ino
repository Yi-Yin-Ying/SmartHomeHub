#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClientSecure wc;
PubSubClient pc(wc);

const char* ssid = "12337";
const char* password = "12345677";
const int ledPin = D1;
const int mqttLedPin = D2;

const char* mqtt_server = "s6b22c1f.ala.cn-hangzhou.emqxsl.cn";
const uint16_t mqtt_port = 8883;
const char* mqtt_id = "emqx_clouddf6127";
const char* mqtt_user = "ESP8266";
const char* mqtt_pass = "123456";

const char* datatopic="Sensor/Data";

void connectWiFi(const char* wifiName, const char* wifiPass, int timeoutSec) {
    digitalWrite(ledPin, LOW);
    WiFi.disconnect();
    WiFi.begin(wifiName, wifiPass);
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < timeoutSec * 2) {
        delay(500);
        timeout++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        digitalWrite(ledPin, HIGH);
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.printf("Message arrived [%s]: ", topic);
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

bool connectMqtt() {
    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }
    if (pc.connect(mqtt_id, mqtt_user, mqtt_pass)) {
        pc.subscribe("test");
        digitalWrite(mqttLedPin, HIGH);
        return true;
    }
    return false;
}

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(50);  // 缩短串口超时，避免阻塞
    
    pinMode(ledPin, OUTPUT);
    pinMode(mqttLedPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    digitalWrite(mqttLedPin, LOW);
    
    wc.setInsecure();
    pc.setServer(mqtt_server, mqtt_port);
    pc.setCallback(callback);
    
    connectWiFi(ssid, password, 15);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        connectWiFi(ssid, password, 15);
        return;
    }

    if(!pc.connected()){
        digitalWrite(mqttLedPin, LOW);
        connectMqtt();
        return;
    }
    pc.loop();
    // pc.publish("test","123");

    if(Serial.available()){
        String str = Serial.readString();
        // Serial.print(str);
        pc.publish(datatopic,str.c_str());
    }
    // Serial.println("123");
    delay(10);  // 缩短延时，保持MQTT心跳稳定
}
