// This #include statement was automatically added by the Particle IDE.
#include <MQTT.h>
#include <Adafruit_DHT.h>
#define DHTPIN 6
#define DHTTYPE DHT11
// Create an MQTT client
MQTT client("test.mosquitto.org", 1883, callback);


DHT dht(DHTPIN, DHTTYPE);
int startupLed = D8;             //If safety system on, this Led Turns on
int sensorPin = D3;              //input pin for PIR sensor
int buzzerPin = D4;              // Buzzer Pin
int pirState = LOW;              //Motion Sensor state
bool motionStat = false;         //Checks the Status of the Motion
bool status = false;             //To check whether Safety system is on or off
bool security = false;           //To check whether the security system is on or off
bool exhaustStat = false;        //To check whether the exhaust is on or off
bool fireStat = false;           //If this is true, notication sent to user
bool notificationStat = false;   //To check whether a notification has been sent or not
int timeValue = 0;               //time delay before sending an alert to make sure its not a false alarm;
int val = 0; 

// This is called when a message is received from the MQTT servers
void callback(char* topic, byte* payload, unsigned int length) 
{
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    if (!strcmp(p, "TurnSystemOFF"))
    {
        Particle.publish("System OFF:", "0", PRIVATE);
        client.publish("System", "systemisoff");
        digitalWrite(startupLed, LOW);
        status = false;
    }
    if (!strcmp(p, "TurnSystemON"))
    {
        Particle.publish("System ON:", "1", PRIVATE);
        status = true;
    }
    if (!strcmp(p, "TurnSecurityON"))
    {
        Particle.publish("Security ON:", "1", PRIVATE);
        security = true;
    }
    if (!strcmp(p, "TurnSecurityOFF"))
    {
        Particle.publish("Security OFF:", "0", PRIVATE);
        digitalWrite(buzzerPin, LOW);
        security = false;
        timeValue = 0;
    }
    delay(1000);
}
void setup() {
    dht.begin();
    client.connect("photonDev12345654321");
    pinMode(startupLed, OUTPUT);     //declare startup led for safety system as output
    pinMode(buzzerPin, OUTPUT);     //declare buzzer as output
    pinMode(sensorPin, INPUT);      // declare Motion Sensor sensor as input
}
//FirstI made funtions for the safety system
//This checks the temperature and returns the values of the temperature
float temp(){
    float t = dht.getTempCelcius();
    return t;
}
//This is called when the temp is above a certain value and if 
void timer()
{
    timeValue+=1;
}
//This functions sends a notification to the user if there is a fire.
void SafetyNotificationAlert(bool status)
{
    if (status)
    {
        Particle.publish("temp", "fireAlertOn", PRIVATE);
        notificationStat = true;
    }
    else
    {
        Particle.publish("temp", "fireAlertOff", PRIVATE);
        notificationStat = false;
    }
}
//Now we make functions for Motion Sensors
void MotionSensor() {
    //we set the value of val variable to whatever motion sensor is reading (0 or 1)
    val = digitalRead(sensorPin);
    Particle.publish("Motion Status", String(val), PRIVATE);
}
//Sounds the buzzer if motion is detected
void setAlarm(int state) {
    digitalWrite(buzzerPin, state);
}

void MotionData() 
{
    //first we check if the value is High or Low
    if (val == HIGH) 
    {
        //if the value is High (which indicates motion)
        //and pirState is Low we change it to High and call the alarm function
        if (pirState == LOW) 
        {
          //Particle.publish("PhotonMotion", "Motion Detected", PRIVATE);
          pirState = HIGH;
          setAlarm(pirState);
        }
    } 
    else 
    {
        if (pirState == HIGH) 
        {
          //Particle.publish("PhotonMotion", "No Motion", PRIVATE);
          pirState = LOW;
          setAlarm(pirState);
        }
    }
    SecurityNotificationAlert(pirState, motionStat);
}
void SecurityNotificationAlert(int status, bool check)
{
    //If value is High and the notification has not been sent
    //This will send a notification
    if (status == 1 && !motionStat)
    {
        Particle.publish("Motion", "Detected", PRIVATE);
        motionStat = true;
        delay(5000);
    }
    //if notification was sent it will again check the status
    //and send a notification when no motion dedected.
    else if (status == 0 && motionStat)
    {
        Particle.publish("Motion", "NotDetected", PRIVATE);
        motionStat = false;
        delay(5000);
    }
}
void loop() {
    if (client.isConnected())
    {
        client.subscribe("TurnSystemfortheProject123");
    }
    if (status)
    {
        Particle.publish("value:", String(temp()), PRIVATE);
        Particle.publish("timer Value", String(timeValue), PRIVATE);
        digitalWrite(startupLed, HIGH);
        //if(temp() > 20 && timer > 5)
        if (temp() > 15)
        {
            timer();
            //Resetting the value so if the values were not collective the time resets.
        }
        if(timeValue > 5) //we are already checking the temp() in the timer function
        {
            // Publish our message to the test server
            //Particle.publish("temp:", "fireAlert", PRIVATE);
            client.publish("System", "alert");
            delay(1000);
            timeValue = 0;
        }
        else if (timeValue < 5 && temp() < 15)
        {
            client.publish("System", "safe");
            delay(1000);
        }
        //if (temp() > 20 && !check)
        if (notificationStat == false && timeValue > 4)
        //if (timeValue > 5)
        {
            fireStat = true;
            SafetyNotificationAlert(fireStat);
        }
        //else if (temp() < 20 && check)
        else if (notificationStat == true && temp() < 15)
        {
            fireStat = false;
            SafetyNotificationAlert(fireStat);
        }
    }
    if (security)
    {
        //if security is ON we start reading Motion Sensor data.
        MotionSensor();
        MotionData();
    }
     // CALL THIS at the end of your loop
    client.loop();
    delay(1000);
}