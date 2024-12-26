#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

const char* ssid = "ESP32-AP";
const char* password = "12345678";

// 设置静态IP地址
IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

const int ledPin = 2; // ESP32板载LED通常连接到GPIO 2
const int motorPin = 4; // 电机控制引脚
const int fanPin = 5; // 风扇控制引脚

bool ledState = false;
bool motorState = false;
bool fanState = false;

void handleRoot() {
  server.serveStatic("/", SPIFFS, "/index.html");
}

void handleLEDToggle() {
  ledState = !ledState;
  digitalWrite(ledPin, ledState ? HIGH : LOW);
  String response = "{\"status\":\"" + String(ledState ? "已点亮" : "已熄灭") + "\",";
  response += "\"image\":\"" + String(ledState ? "/led_on.png" : "/led_off.png") + "\",";
  response += "\"buttonText\":\"" + String(ledState ? "熄灭LED" : "点亮LED") + "\"}";
  server.send(200, "application/json", response);
}

void handleMotorToggle() {
  motorState = !motorState;
  digitalWrite(motorPin, motorState ? HIGH : LOW);
  String response = "{\"status\":\"" + String(motorState ? "已启动" : "已停止") + "\",";
  response += "\"image\":\"" + String(motorState ? "/motor_on.png" : "/motor_off.png") + "\",";
  response += "\"buttonText\":\"" + String(motorState ? "停止电机" : "启动电机") + "\"}";
  server.send(200, "application/json", response);
}

void handleFanToggle() {
  fanState = !fanState;
  digitalWrite(fanPin, fanState ? HIGH : LOW);
  String response = "{\"status\":\"" + String(fanState ? "已启动" : "已停止") + "\",";
  response += "\"image\":\"" + String(fanState ? "/fan_on.png" : "/fan_off.png") + "\",";
  response += "\"buttonText\":\"" + String(fanState ? "停止风扇" : "启动风扇") + "\"}";
  server.send(200, "application/json", response);
}

void handleSceneWakeup() {
  // 实现起床模式的逻辑
  ledState = true;
  motorState = true;
  fanState = true;
  digitalWrite(ledPin, HIGH);
  digitalWrite(motorPin, HIGH);
  digitalWrite(fanPin, HIGH);
  server.send(200, "application/json", "{\"status\":\"起床模式已激活\"}");
}

void handleSceneHome() {
  // 实现回家模式的逻辑
  ledState = true;
  motorState = false;
  fanState = true;
  digitalWrite(ledPin, HIGH);
  digitalWrite(motorPin, LOW);
  digitalWrite(fanPin, HIGH);
  server.send(200, "application/json", "{\"status\":\"回家模式已激活\"}");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(motorPin, LOW);
  digitalWrite(fanPin, LOW);

  // 初始化SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS 初始化失败");
    return;
  }

  // 设置ESP32为AP模式，并设置静态IP地址
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  Serial.println("AP 模式已启动");
  Serial.print("IP 地址: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/LED_TOGGLE", handleLEDToggle);
  server.on("/MOTOR_TOGGLE", handleMotorToggle);
  server.on("/FAN_TOGGLE", handleFanToggle);
  server.on("/SCENE_WAKEUP", handleSceneWakeup);
  server.on("/SCENE_HOME", handleSceneHome);

  // 提供图标文件
  server.serveStatic("/led_on.png", SPIFFS, "/led_on.png");
  server.serveStatic("/led_off.png", SPIFFS, "/led_off.png");
  server.serveStatic("/motor_on.png", SPIFFS, "/motor_on.png");
  server.serveStatic("/motor_off.png", SPIFFS, "/motor_off.png");
  server.serveStatic("/fan_on.png", SPIFFS, "/fan_on.png");
  server.serveStatic("/fan_off.png", SPIFFS, "/fan_off.png");

  server.begin();
  Serial.println("HTTP 服务器已启动");
}

void loop() {
  server.handleClient();
}