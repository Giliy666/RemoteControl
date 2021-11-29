#include <map>
#include <Arduino.h>
#include <FreeRTOS.h>
#include <Preferences.h>
#define USE_NIMBLE
#include <BleKeyboard.h>
#include <Preferences.h>
#include <WiFi.h>

#define BAT_PIN  33

int bat_proc =50;

#define TOUTCH_PIN1 T4
#define TOUTCH_PIN2 T5
#define TOUTCH_PIN3 T6
#define TOUTCH_PIN4 T7
int touch_value_1 = 0;
int touch_value_2 = 0;
int touch_value_3 = 0;
int touch_value_4 = 0;

bool tach_stat [4]={0,0,0,0};

MediaKeyReport comand [14]=
{
0xB5,0xB6,0xCD,0xE9,0xEA,0xB7,0xC2, //Next Track,Previous Track,(Play/Pause),(Volume Increment),(Volume Decrement),(Stop)F1,
0xD8,0xDA,0xD9,0xD7,//под замену
0x80,0x81,0x82, //веремнно для тестов
};

int u=0;
int u_sum=0;
int sum_array=0000;


Preferences preferences;

TaskHandle_t Task1;
TaskHandle_t Task2;

const char* ssid     = "MALOMU";
const char* password = "1";
WiFiServer server(80);
char lineBuf[80];
int charCount = 0;

BleKeyboard bleKeyboard("MALOMU","Samopal",bat_proc);

void Task1code( void * pvParameters );
void Task2code( void * pvParameters );

int read_sens(uint8_t pin)
{
  for(int i=0;i<5;i++)
  {
  u = touchRead(pin);  
  u_sum = u_sum + u;
  }
  u = u_sum/5;
  u_sum=0;  
  return(u);
}

void setup() 
{
Serial.begin(115200);
Serial.println("Starting BLE work!");

preferences.begin("tehnolog-cat", false);
preferences.clear();

bleKeyboard.begin();

WiFi.softAP(ssid, password);

Serial.println();
Serial.print("IP address: ");
Serial.println(WiFi.softAPIP());

xTaskCreatePinnedToCore(Task1code,"Task1",10000,NULL,1,&Task1,0); 
  delay(5); 
xTaskCreatePinnedToCore(Task2code,"Task2",10000,NULL,1,&Task2,1); 
  delay(5);   
}
void loop() 
{
  
}
void Task1code( void * pvParameters )
{
  while (1)
  {
    touch_value_1 = read_sens(TOUTCH_PIN1);       
    touch_value_2 = read_sens(TOUTCH_PIN2); 
    touch_value_3 = read_sens(TOUTCH_PIN3);    
    touch_value_4 = read_sens(TOUTCH_PIN4);  

    if (touch_value_1<30)
    {
      tach_stat[0]=1;
    }
    else
    {
      tach_stat[0]=0;
    }
    if (touch_value_2<30)
    {
      tach_stat[1]=1;
    }
    else
    {
      tach_stat[1]=0;
    }
    if (touch_value_3<30)
    {
      tach_stat[2]=1;
    }
    else
    {
      tach_stat[2]=0;
    }
    if (touch_value_4<30)
    {
      tach_stat[3]=1;
    }
    else
    {
      tach_stat[3]=0;
    }
    sum_array=(tach_stat[0]*1000)+(tach_stat[1]*100)+(tach_stat[2]*10)+(tach_stat[3]*1);
    
    Serial.print(sum_array);
    
    Serial.print('\n');    

    switch (sum_array)
    {
      case 1:
      bleKeyboard.write(comand[0]);
      break;
      case 10:
      bleKeyboard.write(comand[1]);
      break;
      case 11:
      bleKeyboard.write(comand[2]);
      break;
      case 100:
      bleKeyboard.write(comand[3]);
      break;
      case 101:
      bleKeyboard.write(comand[4]);
      break;
      case 110:
      bleKeyboard.write(comand[5]);
      break;
      case 111:
      bleKeyboard.write(comand[6]);
      break;
      case 1000:
      bleKeyboard.write(comand[7]);
      break;
      case 1001:
      bleKeyboard.write(comand[8]);
      break;
      case 1010:
      bleKeyboard.write(comand[9]);
      break;
      case 1011:
      bleKeyboard.write(comand[10]);
      break;
      case 1100:
      bleKeyboard.write(comand[11]);
      break;
      case 1101:
      bleKeyboard.write(comand[12]);
      break;
      case 1110:
      bleKeyboard.write(comand[13]);
      break;
      case 1111:
      bleKeyboard.write(comand[14]);
      break;    
    default:
      delay(100);
      break;
    }    

  } 
  
}

void Task2code( void * pvParameters )
{ 
  while (1)
  {  
    WiFiClient client = server.available(); // прослушка входящих клиентов
  if (client) {
    Serial.println("New client");
    memset(lineBuf, 0, sizeof(lineBuf));
    charCount = 0;
    // HTTP-запрос заканчивается пустой строкой
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
	  
      // формируем веб-страницу
      String webPage = "<!DOCTYPE HTML>";
      webPage += "<html>";
      webPage += "<head>";
      webPage += "<meta name=\"viewport\" content=\"width=device-width,";
      webPage += "initial-scale=1\">";
      webPage += "</head>";
      webPage += "<body>";
      webPage += "<h1>ESP32 - Web Server</h1>";
      webPage += "<p>AnalogPin 34 = ";
      webPage +=    analogRead(34);
      webPage += "</p>";
      webPage += "<p>AnalogPin 36 = ";
      webPage +=    analogRead(36);
      webPage += "</p>";
      webPage += "<p>AnalogPin 39 = ";
      webPage +=    analogRead(39);
      webPage += "</p>";
      webPage += "</body></html>";
      client.println(webPage);
      break;
    }
    // Даём веб-браузеру время для получения данных
    delay(1);
    // Закрываем соединение
    client.stop();
    Serial.println("client disconnected");
  }
  }
  
 
}