#include <SD.h>

#define CS_PIN  D8

double Tlm35;
int analog;
String dataString;
File dataFile;

void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializando o cartão SD...");
  pinMode(CS_PIN, OUTPUT);

  if (!SD.begin(CS_PIN)) {
    Serial.println("Falha, verifique se o cartão está conectado.");
    return;
  }
  Serial.println("Cartão inicializado.");
  delay(100);
}
 
void loop()
{
  analog = analogRead(17);
  Tlm35 = analog*0.322265625;
  dataString = String(Tlm35) + "," + String(Tlm35);

  saveData();
  delay(3000);
}

void saveData(){
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("Os dados foram escritos com sucesso!");
  } else {
    Serial.println("Falha ao abrir o arquivo datalog.csv");
  }
}
