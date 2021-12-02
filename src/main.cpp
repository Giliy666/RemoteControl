#include <Arduino.h>
#include <FreeRTOS.h>
#include <BleCombo.h>
#include <WiFi.h>

#define BAT_PIN  33

int bat_proc =50;

#define TOUTCH_PIN1 T4
#define TOUTCH_PIN2 T5
#define TOUTCH_PIN3 T6
#define TOUTCH_PIN4 T7
#define TOUTCH_PIN_CLICK T8

int touch_value_click = 0;

int threshold = 30;

bool tach_stat [4]={0,0,0,0};
int tach [4]={0,0,0,0};

bool work_mouse=0;
bool work_key=0;
int a=0;
int u=0;
int u_sum=0;
int sum_array=00000;

int count=0;

TaskHandle_t Task1;
TaskHandle_t Task2;

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

void sort()
{
  
  
}

void setup() 
{
Serial.begin(115200);
Serial.println("Starting BLE work!");

xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, NULL,  0); 
  delay(500); 
xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, NULL,  1); 
  delay(500); 

Keyboard.begin();
Mouse.begin();
}

void loop() 
{
  
}

void Task1code( void * pvParameters )
{
  while (1)
  {
  tach[0]=read_sens(TOUTCH_PIN1);
  tach[1]=read_sens(TOUTCH_PIN2);
  tach[2]=read_sens(TOUTCH_PIN3);
  tach[3]=read_sens(TOUTCH_PIN4);

  touch_value_click=read_sens(TOUTCH_PIN_CLICK);

     if (tach[0]<threshold)
  {
    Keyboard.println("Hello");
    delay(5);
  }
  delay(10);
  
  }
  
}
void Task2code( void * pvParameters )
{
  while (1)
  {
    Serial.println(xPortGetCoreID());
    delay(1000);
 
  }
  
}