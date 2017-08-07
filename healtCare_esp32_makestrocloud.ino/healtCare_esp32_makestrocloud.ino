/* 

author : Joel Murphy & Yury Gitman 
        2013

        Hisyam Kamil
        Agustus 2017
*/


#include <Wire.h> 

#define OLED_SSD1306_DISPLAY    1
#define USE_CLOUD               1

#if OLED_SSD1306_DISPLAY
#include "UIService.h"
UIService uiSvc;
#endif


#if USE_CLOUD
  #include <PubSubClient.h>
  #include "WiFi.h"
  #include <ArduinoJson.h>

  
  const char* ssid     = "DyWare-AP2";
  const char* password = "957DaegCen";


  //MQTT IP address
  const char* mqtt_server = "cloud.makestro.com";
  const char* topic = "Hisyam_Kamil/HealtCare/data";
  const char* sub = "Hisyam_Kamil/HealtCare/control";

  long lastMsg = 0;
  char msg[50];
  int value = 0;
  long lastReconnectAttempt = 0;

  WiFiClient client;
  PubSubClient mqtt_client(client);

#endif


int fadePin = 12;                 // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin


// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.




volatile int rate[10];                    // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0; // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;  // used to find IBI
volatile int P =512;                      // used to find peak in pulse wave, seeded
volatile int T = 512;                     // used to find trough in pulse wave, seeded
volatile int thresh = 512;                // used to find instant moment of heart beat, seeded
volatile int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = false;      // used to seed rate array so we startup with reasonable BPM


hw_timer_t * timer = NULL;


  
void setup(){
  Wire.begin(21,22);
  Serial.begin(115200);             // we agree to talk fast!

  #if OLED_SSD1306_DISPLAY
    uiSvc.begin();
  #endif 

  #if USE_CLOUD
   
    WiFi.begin(ssid, password);
   
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    set_mqtt_server();
    //check_mqtt_connect(fabrick_usr, fabrick_pass);

  #endif
  
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
}



void loop(){
  #if USE_CLOUD
  if (!mqtt_client.connected()) {
    reconnect();
  }
  
  mqtt_client.loop();
  #endif


  #if OLED_SSD1306_DISPLAY
        uiSvc.loop();
  #endif

  sendDataToProcessing('S', Signal);     // send Processing the raw Pulse Sensor data
  if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
        fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
        sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
        sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
        QS = false;                      // reset the Quantified Self flag for next time    
        #if OLED_SSD1306_DISPLAY
        uiSvc.setBPMValue(BPM);
        #endif

        #if USE_CLOUD
       
        publishKeyValue("BPM",BPM);
        Serial.println("publish cloud");
        #endif
     }

  

}


void sendDataToProcessing(char symbol, int data ){
    Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
    Serial.println(data);                // the data to send culminating in a carriage return
  }

  void interruptSetup(){     
  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin(0, 80, true);
  
  // Initializes Timer to run the ISR to sample every 2mS as per original Sketch.
  // Attach ISRTr function to our timer.
  timerAttachInterrupt(timer, &ISRTr, true);


  // Set alarm to call isr function every 2 milliseconds (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, 2000, true);

  // Start an alarm
  timerAlarmEnable(timer);
   
} 


// THIS IS THE HW-TIMER INTERRUPT SERVICE ROUTINE. 
// Timer makes sure that we take a reading every 2 miliseconds
void ISRTr(){                                 // triggered when timer fires....
  Signal = analogRead(34);                    // read the Pulse Sensor on pin 34 3.3v sensor power......default ADC setup........
  Signal = map(Signal, 0, 4095, 0, 1023);     // Map the value back to original sketch range......
  sampleCounter += 2;                         // keep track of the time in mS with this variable
  int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

    //  find the peak and trough of the pulse wave
  if(Signal < thresh && N > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal < T){                        // T is the trough
      T = Signal;                         // keep track of lowest point in pulse wave 
    }
  }

  if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
    P = Signal;                             // P is the peak
  }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (N > 250){                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){        
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
      //digitalWrite(LED_BUILTIN,HIGH);                // turn on pin 13 LED
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat = false;                  // clear secondBeat flag
        for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;                      
        }
      }

      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
        sei();                               // enable interrupts again
        return;                              // IBI value is unreliable so discard it
      }   


      // keep a running total of the last 10 IBI values
      word runningTotal = 0;                  // clear the runningTotal variable    

      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value 
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = true;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over
    //digitalWrite(LED_BUILTIN,LOW);            // turn off pin 13 LED
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  if (N > 2500){                           // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
  }


}// end isr

#if USE_CLOUD

void reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect("Hisyam_Kamil-HealtCare-default","Hisyam_Kamil","CjIzfO689VdBxb0O5krsgLk7zdQWEDlwhz49eA0AaZ7b6rW2ZMAryQiguDIqeyE0")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void publishKeyValue(const char* key, char Valueval) {
    const int bufferSize = JSON_OBJECT_SIZE(2);
    StaticJsonBuffer<bufferSize> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root[key] =  Valueval;

    String jsonString;
    root.printTo(jsonString);
    publishData(jsonString);
  }

void publishData(String payload) {
   publish(topic, payload);
}

void publish(String topic, String payload) {
mqtt_client.publish(topic.c_str(), payload.c_str());
}


//****************************************
// Set MQTT Server
//****************************************
void set_mqtt_server(){  
  // Set MQTT server
  mqtt_client.setServer(mqtt_server, 1883);
  mqtt_client.setCallback(callback);  
}

//****************************************
// Callback MQTT
//****************************************
void callback(char* topic, byte* payload, unsigned int length) {
  // Nothing here because we don't subscribe to any topics 
}

#endif