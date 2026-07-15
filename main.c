#include <NewPing.h>
#include <Servo.h>
#define TRIGGER_PIN  PA0
#define ECHO_PIN     PA1
#define MAX_DISTANCE 400
#define LED_PIN PC13 
#define errorpin PA2
#define BUTTON_PIN PA3
TaskHandle_t xServoTaskHandle = NULL; 
QueueHandle_t g_sensorQueue;
TimerHandle_t xWatchdogTimer;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void vSensorTask(void *pvParameters) {
    for(;;) {
        
        int distance = sonar.ping_cm(); 
        
        xQueueSend(g_sensorQueue, &distance, pdMS_TO_TICKS(100));
        xTimerReset(xWatchdogTimer, 0); 
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
 void vServoTask(void *pvParameters){
int sensorValue;
for(;;){
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, 0);

        if(ulNotificationValue > 0) {
            my_Servo.write(90);
            digitalWrite(LED_PIN, HIGH);
            vTaskDelay(pdMS_TO_TICKS(5000)); 
            my_Servo.write(0);
            digitalWrite(LED_PIN, LOW);
        }
        else {
    if(xQueueReceive(g_sensorQueue, &sensorValue, pdMS_TO_TICKS(500))==pdTRUE){
  if(sensorValue == 0) sensorValue = 100;
  if(sensorValue < 10){
    my_Servo.write(90);
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(4000));
    digitalWrite(LED_PIN, LOW);

  }
  else{
    my_Servo.write(0);
  }
    }
}}
void vWatchdogCallback(TimerHandle_t xTimer){
Serial.println("Error Something went wrong!");
digitalWrite(errorpin, HIGH);

}
void IRAM_ATTR ButtonISR(){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xServoTaskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
void setup(){
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(errorpin, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ButtonISR, FALLING);
    g_sensorQueue = xQueueCreate(5, sizeof(int));
    xWatchdogTimer = xTimerCreate("WatchdogTimer", pdMS_TO_TICKS(5000), pdTRUE, NULL, vWatchdogCallback);
    xTaskCreate(vSensorTask, "SensorTask", 2048, NULL, 2, NULL);
    xTaskCreate(vServoTask, "ServoTask", 2048, NULL, 1, &xServoTaskHandle);
    xTimerStart(xWatchdogTimer, 0);
}
    void loop() {
    }
    
