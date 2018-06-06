#include <ESP8266WiFi.h>
#include <Wire.h>
#include <SD.h>
#include "RTClib.h"

// Pin where is the CS pin of the sd adapter
#define CS_PIN  D8

// Netowrk, Password and Host IP definition 
const char* ssid = "";
const char* password = "";
const char* host = "";

// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x69;

// Select SDA and SCL pins for I2C communication 
const uint8_t scl = D1;
const uint8_t sda = D2;

// Sensitivity scale factor respective to full scale setting provided in datasheet 
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x69;

// Declaration of functions
void wifiSetUp();
void MPU6050_Init();
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress);
void sdSetUp();
void saveDataSD();

// MPU6050 global variables
int16_t AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ;
double Ax, Ay, Az, Gx, Gy, Gz;

// LM35 global variables
int analog;
double Tlm35;

// SD objects
String dataString;
File dataFile;

//RTC char for days of the week
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

//RTC object
RTC_DS3231 rtc;

void setup(){
  Serial.begin(115200);
  delay(10);

  //Wifi start
  wifiSetUp();
  
  //I2C start
  Wire.begin(sda, scl);
  MPU6050_Init();

  //SD card start
  sdSetUp();
}

void loop() {
  
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  
  // divide each with their sensitivity scale factor
  Ax = (double)AccelX/AccelScaleFactor;
  Ay = (double)AccelY/AccelScaleFactor;
  Az = (double)AccelZ/AccelScaleFactor;
  Gx = (double)GyroX/GyroScaleFactor;
  Gy = (double)GyroY/GyroScaleFactor;
  Gz = (double)GyroZ/GyroScaleFactor;
  analog = analogRead(17);
  Tlm35 = analog*0.322265625;

  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  dataString = String(Ax) + "," + String(Ay) + "," + String(Az) + "," + String(Gx) + "," + String(Gy) + "," + String(Gz) + "," + String(Tlm35) 
  + "," + String(now.year(), DEC) + "," + String(now.month(), DEC) + "," + String(now.day(), DEC) + "," + String(daysOfTheWeek[now.dayOfTheWeek()])
  + "," + String(now.hour(), DEC) + "," + String(now.minute(), DEC) + "," + String(now.second(), DEC);

  Serial.print("Conectando com ");
  Serial.println(host);
  
  // Usando a classe WiFiClient para criar a conexão TCP
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Falha na conexão.\n");
    return;
  }
  
    // Criando a URL para as requisições 
    String url = "/salvar.php?";
    url += "Ax=";
    url += Ax;
    url += "&Ay=";
    url += Ay;
    url += "&Az=";
    url += Az;
    url += "&Gx=";
    url += Gx;
    url += "&Gy=";
    url += Gy;
    url += "&Gz=";
    url += Gz;
    url += "&Tlm35=";
    url += Tlm35;
  
  Serial.print("Requisitando URL: ");
  Serial.print(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  // Calcula o tempo que demorou a solicitação
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }  

  // Captura o que retornou do servidor
  while(client.available()){
    String line = client.readStringUntil('\r');
    
    if(line.indexOf("salvo com sucesso!") != -1){
      Serial.println();
      Serial.println("Requisição salva com sucesso!"); 
    }else if(line.indexOf("salvo com sucesso!") != -1){
      Serial.println();
      Serial.println("Erro ao salvar dados no servidor!");
    }
  }
  saveDataSD();
  Serial.println("Conexão fechada.\n");
  //Serial.println();
  delay(1000);  
}

void wifiSetUp(){
  // Starts the connection to the wifi network
  Serial.print("Conectando com ");
  Serial.println(ssid);
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado.");  
}

// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// configure MPU6050
void MPU6050_Init(){
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);// set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}

void sdSetUp(){
  delay(150); 
  pinMode(CS_PIN, OUTPUT);
  if(!SD.begin(CS_PIN)){
    Serial.println("Cartão SD não encontrado.\n");
    return;
  }else{
    Serial.println("Cartão SD inicializado.\n"); 
  }
}

void saveDataSD(){
  delay(150);
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if(dataFile){
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("Dados escritos com sucesso no cartão SD.");
  }else{
    Serial.println("Falha ao abrir o arquivo datalog.csv"); 
  } 
}