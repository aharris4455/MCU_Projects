#include <Arduino_FreeRTOS.h>
#include <semphr.h>

void TaskBlink(void *pvParameters);
void TaskAnalogRead(void *pvParameters);
void TaskFanCtrl(void *pvParameters); 
QueueHandle_t xQueue;

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ;
  }
  xQueue = xQueueCreate(5, sizeof(int));
  xTaskCreate(
    TaskBlink
    , "Blink" // Name in human readable form
    , 128   // Stack size, can be cchecked and adjusted using Stack Highwater
    , NULL  
    , 2     // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    , NULL);

  xTaskCreate(
    TaskAnalogRead
    ,  "AnalogRead"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );
  xTaskCreate(
    TaskFanCtrl
    ,  "StartFan"
    ,  128  // Stack size
    ,  NULL
    ,  2  // Priority
    ,  NULL );

  // Task scheduler takes over control
}

void loop() {
//No need
}
void TaskBlink(void *pvParameters){
  // Blin onboard LED every second
  (void) pvParameters;
  pinMode(LED_BUILTIN, OUTPUT);
  for(;;){
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN, LOW); 
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void TaskAnalogRead(void *pvParameters){
  //Read pointometer
  (void) pvParameters;
  for (;;){
    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);
    // print out the value you read:
    //Serial.println(sensorValue);
    xQueueSend(xQueue, &sensorValue, portMAX_DELAY);
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskFanCtrl(void *pvParameters){
  // Control fan 
  (void) pvParameters;
  int rxVal;
  pinMode(7, OUTPUT);
  for(;;){
    if(xQueueReceive(xQueue, &rxVal, portMAX_DELAY)){
      if(rxVal >= 500){
        Serial.println("Fan on");
        Serial.println(rxVal);
        digitalWrite(7,HIGH);
        vTaskDelay(1);
      }
      else{
        digitalWrite(7,LOW);
        vTaskDelay(1);
      }
    }
  }
}



