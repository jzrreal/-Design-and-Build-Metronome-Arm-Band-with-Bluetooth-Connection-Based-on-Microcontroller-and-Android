#include "Arduino.h"
#include "stdlib.h"
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

/* ----------------------------------------------------------------------------------------------------------*/

BluetoothSerial SerialBT;

int connect_state = 0;
int MetronomeVibrator_pin = 15;
int BPM = 80;

int delaytimer;
int half_delaytimer;

int metronomeState = 0;
char BTinput;

/* ----------------------------------------------------------------------------------------------------------*/

void mainProcess(void *param);
void metronomeVibrate(void *param);

TaskHandle_t task_mainProcess;
TaskHandle_t task_metronomeVibrate;

/* ----------------------------------------------------------------------------------------------------------*/

void setup() {
  Serial.begin(115200);
  SerialBT.register_callback(callback);

  xTaskCreate(mainProcess,"taskmainProcess",10000,NULL,1,&task_mainProcess);
  xTaskCreate(metronomeVibrate,"taskmetronomeVibrate",10000,NULL,2,&task_metronomeVibrate);
  
  if(!SerialBT.begin("Metronome V2")){
    Serial.println("An error occured initializing");
  }
  else{
    Serial.println("Bluetooth initialized");
  }
  pinMode (LED_BUILTIN, OUTPUT);
  pinMode (MetronomeVibrator_pin, OUTPUT);
}

void loop() {
  
}
/* ----------------------------------------------------------------------------------------------------------*/

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if (event == ESP_SPP_SRV_OPEN_EVT){
    connect_state = 1;
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Client Connected");
  }
}

void mainProcess(void *param){
  while(1){
    BTinput = (char)SerialBT.read();
      if (Serial.available()) {
        SerialBT.write(Serial.read());
      }
      if (SerialBT.available()) {
        SerialBT.print("Received: ");// write on BT app
        SerialBT.println(BTinput);// write on BT app      
        Serial.print ("Received: ");//print on serial monitor
        Serial.println(BTinput);
        if(BTinput == 'p'){
          BPM = BPM + 1;
          Serial.print("BPM: ");
          Serial.println(BPM);
          Serial.println("--------------------------");
        }
        if(BTinput == 'n'){
          BPM = BPM - 1;
          Serial.print("BPM: ");
          Serial.println(BPM);
          Serial.println("--------------------------");
        }
        if(BTinput == 'r'){
          BPM = 80;
          Serial.print("BPM: ");
          Serial.println(BPM);
          Serial.println("--------------------------");
        }
  
        delaytimer = 60000/BPM;
        half_delaytimer = delaytimer / 2;
        
        if(BTinput == '1' ){
          metronomeState = 1;
          Serial.println("Metronome on");
          Serial.println("--------------------------");
        }
        if(BTinput == '0' ){
          metronomeState = 0;
          Serial.println("Metronome off");
          Serial.println("--------------------------");
        }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void metronomeVibrate(void *param){
  while(1){
    if (!metronomeState==1){
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(MetronomeVibrator_pin, LOW);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    else{
      
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(MetronomeVibrator_pin, HIGH);
      
      vTaskDelay(half_delaytimer / portTICK_PERIOD_MS);
      
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(MetronomeVibrator_pin, LOW);
      
      vTaskDelay(half_delaytimer / portTICK_PERIOD_MS);
    }
  }
}
/* ----------------------------------------------------------------------------------------------------------*/
