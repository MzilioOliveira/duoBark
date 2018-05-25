#include <ESP8266WiFi.h>
#include <Wire.h>
#include <SD.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

//Pin where is the CS pin of the sd adapter
#define CS_PIN  D8

//Netowrk, Password and Host IP definition 
const char ssid[] = "FARMIN";
const char pass[] = "3x4m3.farmin";
const char host[] = "18.191.120.124";

//NTP Server
static const char ntpServerName[] = "pool.ntp.br";

//Set the time zone
const int timeZone = -3;

//MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

//Select SDA and SCL pins for I2C communication 
const uint8_t scl = D1;
const uint8_t sda = D2;

//Sensitivity scale factor respective to full scale setting provided in datasheet 
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

//MPU6050 few configuration register addresses
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
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

//Declaration of functions
void wifiSetUp();
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress);
void MPU6050_Init();
void sdSetUp();
void saveDataSD();
void piscaLed();
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

//MPU6050 variables
int16_t AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ;
double Ax, Ay, Az, Gx, Gy, Gz;

//LM35 variables
int analog;
double Tlm35;

//SD variables
String dataString;
File dataFile;

//Udp variable
WiFiUDP Udp;

//NTP time is in the first 48 bytes of message
const int NTP_PACKET_SIZE = 48;

//Buffer to hold incoming & outgoing packets
byte packetBuffer[NTP_PACKET_SIZE];

// local port to listen for UDP packets
unsigned int localPort = 8888;

void setup(){
  //OUTPUT Led
  pinMode(LED_BUILTIN, OUTPUT);
  
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
  
  //Divide each with their sensitivity scale factor
  Ax = (double)AccelX/AccelScaleFactor;
  Ay = (double)AccelY/AccelScaleFactor;
  Az = (double)AccelZ/AccelScaleFactor;
  Gx = (double)GyroX/GyroScaleFactor;
  Gy = (double)GyroY/GyroScaleFactor;
  Gz = (double)GyroZ/GyroScaleFactor;
  analog = analogRead(17);
  Tlm35 = analog*0.322265625;

  dataString = String(Ax) + "," + String(Ay) + "," + String(Az) + "," + String(Gx) + "," + String(Gy) + "," + String(Gz) + "," + String(Tlm35)
  + "," + String(hour()) + "," + String(minute()) + "," + String(second()) + "," + String(year()) + "," + String(month()) + "," + String(day());

  Serial.print("Conectando com ");
  Serial.println(host);
  
  //Using the WiFiClient class to create the TCP connection
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Falha na conexão\n");
    return;
  }
  
  //Creating the URL for the requisitions 
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
  
  //This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  //Calculates the time it took for the order
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }  

  //Captures what returned from the server
  while(client.available()){
    String line = client.readStringUntil('\r');
    piscaLed();
    if(line.indexOf("salvo com sucesso!") != -1){
      Serial.println();
      Serial.println("Requisição salva com sucesso!"); 
      piscaLed();
    }else if(line.indexOf("salvo com sucesso!") != -1){
      Serial.println();
      Serial.println("Erro ao salvar dados no servidor!\n");
    }
    
  }
  saveDataSD();
  Serial.println("Conexão fechada.\n");
  
  delay(1000);  
}

void wifiSetUp(){
  //Starts the connection to the wifi network
  Serial.print("Conectando com ");
  Serial.println(ssid);
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado");  
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
}

//Read all 14 register
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

//Configure MPU6050
void MPU6050_Init(){
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}

void sdSetUp(){ 
  pinMode(CS_PIN, OUTPUT);
  if(!SD.begin(CS_PIN)){
    Serial.println("Falha, cartão SD não encontrado.");
    return;
  }else{
    Serial.println("Cartão inicializado."); 
  }
}

void saveDataSD(){
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if(dataFile){
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("Dados escritos com sucesso no cartão SD!");
  }else{
    Serial.println("Falha ao abrir o arquivo datalog.csv."); 
  }
}

time_t getNtpTime(){
  //NTP server's ip address
  IPAddress ntpServerIP; 

  //Discard any previously received packets
  while (Udp.parsePacket() > 0) ;
  //Get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);//Read packet into the buffer
      unsigned long secsSince1900;
      //Convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0;//Return 0 if unable to get the time
}

//Send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address){
  //Set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  //Initialize values needed to form NTP request
  //(See URL above for details on the packets)
  packetBuffer[0] = 0b11100011;//LI, Version, Mode
  packetBuffer[1] = 0;//Stratum, or type of clock
  packetBuffer[2] = 6;//Polling Interval
  packetBuffer[3] = 0xEC;//Peer Clock Precision
  //8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  //All NTP fields have been given values, now
  //You can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void piscaLed(){
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
}