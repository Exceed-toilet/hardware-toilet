int ldr1 = 32 ,ldr2 = 33, ldr3 = 34 ;
int led1 = 5 ,led2 = 2, led3 = 0;

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* ssid = "vvieww";
const char* password = "qwerty555";
//const char* url_post = "https://ecourse.cpe.ku.ac.th/exceed49/api/payload?gid=16";
const char* url_post = "https://ecourse.cpe.ku.ac.th/exceed16/api/toilet";
//const char* url_post = "http://127.0.0.1:8000/toilet";

const int _size1 = 2*JSON_OBJECT_SIZE(3);
StaticJsonDocument<_size1> JSONPost;

char str[50];

//status room 0:empty
void setup() {
  Serial.begin(115200);
  pinMode(ldr1,INPUT);
  pinMode(ldr2,INPUT);
  pinMode(ldr3,INPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  digitalWrite(led1,LOW);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
  Wifi_connect();
  xTaskCreatePinnedToCore(
    room1,
    "room 1",
    1024*32,
    NULL,
    1,
    NULL,
    0
    );

    xTaskCreatePinnedToCore(
    room2,
    "room 2",
    1024*32,
    NULL,
    1,
    NULL,
    0
    );

    xTaskCreatePinnedToCore(
    room3,
    "room 3",
    1024*32,
    NULL,
    1,
    NULL,
    0
    );
}



void loop() {
//  Serial.print("room1:");
//  Serial.println(analogRead(ldr1));
//  Serial.print("room2:");
//  Serial.println(analogRead(ldr2));
//  Serial.print("room3:");
//  Serial.println(analogRead(ldr3));
//  delay(1000);
}

void room1(void * parameter){
  int status1 = 0;
  while(1){
    int bright = analogRead(ldr1);
    if(status1 == 0  && bright >= 2000){
      status1 = 1;
      digitalWrite(led1,HIGH);
      post(1,status1);
    }else if(status1 == 1 && bright < 2000){
      status1=0;
      digitalWrite(led1,LOW);
      post(1,status1);
    }
    vTaskDelay(50/portTICK_PERIOD_MS);
  }
}
void post(int room,int status1){
  if(WiFi.status()==WL_CONNECTED){
    HTTPClient http;
    http.begin(url_post);
    http.addHeader("Content-Type","application/json");
//    JSONPost["ldr"]=room;
//    JSONPost["led1"]=status1;
//    JSONPost["led2"]=5;
    JSONPost["room_num"]=room;
    JSONPost["use_status"]=status1;
    serializeJson(JSONPost,str);
    int httpcode = http.POST(str);
//    Serial.println(str);
    if(httpcode == HTTP_CODE_OK){
      String payload = http.getString();
      Serial.println(httpcode);
      Serial.println(payload);
    }else{
      Serial.println(httpcode);
      Serial.println("ERROR");
    }
  }else{
    Wifi_connect();
  }
  vTaskDelay(50/portTICK_PERIOD_MS);
}


void room2(void * parameter){
  int status1 = 0;
  while(1){
    int bright = analogRead(ldr2);
    if(status1 == 0  && bright >= 3500){
      status1 = 1;
      digitalWrite(led2,LOW);//red
      post(2,status1);
    }else if(status1 == 1 && bright < 3500){
      status1=0;
      digitalWrite(led2,HIGH);
      post(2,status1);
    }
    vTaskDelay(50/portTICK_PERIOD_MS);
  }
}

void room3(void * parameter){
  int status1 = 0;
  while(1){
    int bright = analogRead(ldr3);
    if(status1 == 0  && bright >= 1500){
      status1 = 1;
      digitalWrite(led3,LOW);//red
      post(3,status1);
    }else if(status1 == 1 && bright < 1500){
      status1=0;
      digitalWrite(led3,HIGH);
      post(3,status1);
    }
    vTaskDelay(50/portTICK_PERIOD_MS);
  }
}

void Wifi_connect(){
  WiFi.disconnect();
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
   delay(1000);
   Serial.println("connecting to Wifi..."); 
  }
  Serial.println("Connected to wifi...");
  Serial.print("ip : ");
  Serial.println(WiFi.localIP());
}
