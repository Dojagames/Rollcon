#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <Espalexa.h>

ESP8266WebServer server(8008);
Espalexa espalexa;

char* ssid = "Neuland";
char* password = "U03uJEI8M69QYcZg";

const int N =  2;
const int LU =  5;
const int RU =  4;
const int LD =  14;
const int RD =  12;
int L_Run = 0;
int R_Run = 0;
int L_Status;
int R_Status;
int con = 0;
int runtime = 15000;
int CurrentTime;

void LChanged(uint8_t height);
void RChanged(uint8_t height);



EspalexaDevice* device1;
EspalexaDevice* device2;

void setup()
{
 WiFi.setSleepMode(WIFI_NONE_SLEEP);
 if (L_Status == 0){L_Status = 1;}
 if (R_Status == 0){R_Status = 1;} 
 pinMode(LU, OUTPUT);
 pinMode(RU, OUTPUT);
 pinMode(LD, OUTPUT);
 pinMode(RD, OUTPUT);
 pinMode(N, OUTPUT);
   
 digitalWrite(LU, LOW);
 digitalWrite(RU, LOW);
 digitalWrite(LD, LOW);
 digitalWrite(RD, LOW);
 digitalWrite(N, LOW); 
  
  SPIFFS.begin();
  WiFi.hostname("rollcon");
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  con = 1;
    device1 = new EspalexaDevice("Rollo Links", LChanged); 
    device2 = new EspalexaDevice("Rollo Rechts", RChanged); 
    espalexa.addDevice(device1);
    espalexa.addDevice(device2); 
     
    if(L_Status == 1) {
      device1->setValue(1);
    } else if(L_Status == 2) {
      device1->setValue(139);
    } else if(L_Status == 3) {
      device1->setValue(255);
    } else { 
      return;
    }
    if(R_Status == 1) {
      device2->setValue(1);
    } else if(R_Status == 2) {
      device2->setValue(139);
    } else if(R_Status == 3) {
      device2->setValue(255);
    } else { 
      return;
    }
    espalexa.begin();

  server.on("/",serveIndexFile);
  server.on("/r1",R_UP);
  server.on("/r2",R_MID);
  server.on("/r3",R_DOWN);
  server.on("/l1",L_UP);
  server.on("/l2",L_MID);
  server.on("/l3",L_DOWN);
  server.on("/1",UP);
  server.on("/3",DOWN);
  server.on("/Lupstart",L_UP_START);
  server.on("/Lupend",L_UP_END);
  server.on("/Rupstart",R_UP_START);
  server.on("/Rupend",R_UP_END);
  server.on("/Ldownstart",L_DOWN_START);
  server.on("/Ldownend",L_DOWN_END);
  server.on("/Rdownstart",R_DOWN_START);
  server.on("/Rdownend",R_DOWN_END);
  server.on("/restart",Reset);
  server.on("/s", getStatus);
  server.begin();
}

void loop()
{
  espalexa.loop();
  server.handleClient();
  delay(1);
}

void serveIndexFile()
{
  File file = SPIFFS.open("/index.html","r");
  server.streamFile(file, "text/html");
  file.close();
}

void getStatus ()
{
  server.send(200,"text/plain", String(L_Status) + " " + String(R_Status));
}

void R_UP(){
  server.send(200,"text/plain", "Done!");
  if (R_Run != 0) {
    return;
  }
  if (R_Status == 1) {
    return;  
  } else if (R_Status == 2) {
    R_Run = 1;
    
    CurrentTime = millis();
    digitalWrite(RU, HIGH);
     while (RU == HIGH){
      if(millis() - CurrentTime >= runtime / 2){
        digitalWrite(RU, LOW); 
        R_Run = 0;
        R_Status = 1;
        device2->setValue(1); 
      }
    } 
  } else if (R_Status == 3) {
    R_Run = 1;
    
    CurrentTime = millis();
    digitalWrite(RU, HIGH);
     while (RU == HIGH){
      if(millis() - CurrentTime >= runtime){
        digitalWrite(RU, LOW); 
        R_Run = 0;
        R_Status = 1;
        device2->setValue(1); 
      }
    }
  } 
}

void L_UP(){
  server.send(200,"text/plain", "200");
  if (L_Run != 0) {
    return;
  }
  if (L_Status == 1) {
    return;  
  } else if (L_Status == 2) {
    L_Run = 1;
    
    CurrentTime = millis();
    digitalWrite(LU, HIGH); 
    while (LU == HIGH){
      if(millis() - CurrentTime >= runtime / 2){
        digitalWrite(LU, LOW); 
        L_Run = 0;
        L_Status = 1;
        device1->setValue(1); 
      }
    } 
  } else if (R_Status == 3) {
    L_Run = 1;
    
     CurrentTime = millis();
    digitalWrite(LU, HIGH); 
    while (LU == HIGH){
      if(millis() - CurrentTime >= runtime){
        digitalWrite(LU, LOW); 
        L_Run = 0;
        L_Status = 1;
        device1->setValue(1);  
      }
    } 
  }
}

void R_MID(){
  server.send(200,"text/plain", "Done!");
  if (R_Run != 0) {
    return;
  }
  if (R_Status == 2) {
    return;
      
  } else if (R_Status == 1) {
    R_Run = 1;
    CurrentTime = millis();
    digitalWrite(RD, HIGH); 
    
    while (RD == HIGH){
      if(millis() - CurrentTime >= runtime / 2){
        digitalWrite(RD, LOW); 
        R_Run = 0;
        R_Status = 2;
        device2->setValue(139);
      }
    }
    
  } else if (R_Status == 3) {
    R_Run = 1;
    CurrentTime = millis();
    digitalWrite(RU, HIGH);
    
     while (RU == HIGH){
      if(millis() - CurrentTime >= runtime / 2){
        digitalWrite(RU, LOW); 
        R_Run = 0;
        R_Status = 2;
        device2->setValue(139); 
        }
     }
  }
}

void L_MID(){
  server.send(200,"text/plain", "200");
  if (L_Run != 0) {
    return;
  }
  if (L_Status == 2) {
    return; 
     
  } else if (L_Status == 1) {
    L_Run = 1;
    CurrentTime = millis();
    digitalWrite(LD, HIGH); 
    
    while (LD == HIGH){
      if(millis() - CurrentTime >= runtime / 2){
        digitalWrite(LD, LOW); 
        L_Run = 0;
        L_Status = 2;
        device1->setValue(139); 
      }
    } 
    
  } else if (R_Status == 3) {
    L_Run = 1; 
    CurrentTime = millis();
    digitalWrite(LU, HIGH);
     
    while (LU == HIGH){
      if(millis() - CurrentTime >= runtime / 2){
        digitalWrite(LU, LOW); 
        L_Run = 0;
        L_Status = 2;
        device1->setValue(139); 
        }
      } 
  }
}

void R_DOWN(){
  server.send(200,"text/plain", "Done!");
  if (R_Run != 0) {
    return;
    
  }
  if (R_Status == 3) {
    return;  
  } else if (R_Status == 2) {
    R_Run = 1;
    CurrentTime = millis();
    digitalWrite(RD, HIGH); 
    
    while (RD == HIGH){
      if(millis() - CurrentTime >= runtime / 2){
        digitalWrite(RD, LOW); 
        R_Run = 0;
        R_Status = 3;
        device2->setValue(255);
      }
    }
    
  } else if (R_Status == 1) {
    R_Run = 1;
    CurrentTime = millis();
    digitalWrite(RD, HIGH); 
    
    while (RD == HIGH){
      if(millis() - CurrentTime >= runtime){
        digitalWrite(RD, LOW); 
        R_Run = 0;
        R_Status = 3;
        device2->setValue(255);
      }
    }
  } 
}

void L_DOWN(){
  server.send(200,"text/plain", "Done!");
  if (L_Run != 0) {
    return;
    
  }
  if (L_Status == 3) {
    return;  
  } else if (L_Status == 2) {
    L_Run = 1;
    CurrentTime = millis();
    digitalWrite(LD, HIGH); 
    
    while (LD == HIGH){
      if(millis() - CurrentTime >= runtime / 2){
        digitalWrite(LD, LOW); 
        L_Run = 0;
        L_Status = 3;
        device1->setValue(255); 
      }
    } 

  } else if (L_Status == 1) {
    L_Run = 1;
    CurrentTime = millis();
    digitalWrite(LD, HIGH); 
    
    while (LD == HIGH){
      if(millis() - CurrentTime >= runtime){
        digitalWrite(LD, LOW); 
        L_Run = 0;
        L_Status = 3;
        device1->setValue(255); 
      }
    } 
  }
}

void DOWN(){
  L_DOWN();
  R_DOWN();
}

void UP(){
  R_UP();
  L_UP();  
}

void L_UP_START(){
  server.send(200,"text/plain", "Done!");
  if(L_Run != 0) {
    return;
  } else if (L_Run == 0){
     L_Run = 1;
     digitalWrite(LU, HIGH);
  }
}

void R_UP_START(){
  server.send(200,"text/plain", "Done!");
  if(R_Run != 0) {
    return;
  } else if (R_Run == 0){
     R_Run = 1;
     digitalWrite(RU, HIGH);
  }
}

void L_DOWN_START(){
  server.send(200,"text/plain", "Done!");
  if(L_Run != 0) {
    return;
  } else if (L_Run == 0){
     L_Run = 1;
     digitalWrite(LD, HIGH);
  }
}

void R_DOWN_START(){
  server.send(200,"text/plain", "Done!");
  if(R_Run != 0) {
    return;
  } else if (R_Run == 0){
     R_Run = 1;
     digitalWrite(RD, HIGH);
  }
}

void L_UP_END(){
  server.send(200,"text/plain", "Done!");
  digitalWrite(LU, LOW);
  L_Run = 0;
}

void R_UP_END(){
  server.send(200,"text/plain", "Done!");
  digitalWrite(RU, LOW);
  R_Run = 0;
}

void L_DOWN_END(){
  server.send(200,"text/plain", "Done!");
  digitalWrite(LD, LOW);
  L_Run = 0;
}

void R_DOWN_END(){
  server.send(200,"text/plain", "Done!");
  digitalWrite(RD, LOW);
  R_Run = 0;
}

void RChanged(uint8_t height) {
 
if (height < 20) {
      R_UP();    
    } else if (height > 20 && height < 235) {
      R_MID();  
    } else if (height > 235) {
      R_DOWN();  
    }
  Serial.println(height);
}
void LChanged(uint8_t height) {

  if (height < 20) {
    L_UP();    
  } else if (height > 20 && height < 235) {
    L_MID();  
  } else if (height > 235) {
    L_DOWN();  
  }
  Serial.println(height);
}    

void Stop(){
  digitalWrite(RD, LOW);
  digitalWrite(LD, LOW);
  digitalWrite(RU, LOW);
  digitalWrite(LU, LOW);
}

void Reset(){
  ESP.restart();
}
