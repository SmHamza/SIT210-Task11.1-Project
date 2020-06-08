import paho.mqtt.client as mqtt         # Import the MQTT library
import time                 # The time library is useful for delays
from gpiozero import LED
import RPi.GPIO as GPIO
from smbus import SMBus

address = 8
bus = SMBus(1)

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)

#set up the outputs
GPIO.setup(7, GPIO.OUT)
GPIO.setup(11, GPIO.OUT)
GPIO.setup(15, GPIO.OUT)
#set the leds and buzzer to Low
GPIO.output(7, GPIO.LOW)
GPIO.output(11, GPIO.LOW)
GPIO.output(15, GPIO.LOW)
#Naming the pins appropiately
buzzer = 7
alertLed = 11
exhaustLed = 15
# Our "on message" event
def messageFunction (client, userdata, message):
    topic = str(message.topic)
    message = str(message.payload.decode("utf-8"))
    #When alert message is received
    if (message == "alert"):
        print(message)
        GPIO.output(7, GPIO.HIGH)
        GPIO.output(11, GPIO.HIGH)
        bus.write_byte(address, 1)
    #when safe message is received
    if (message == "safe"):
        print(message)
        GPIO.output(7, GPIO.LOW)
        GPIO.output(11, GPIO.LOW)
        bus.write_byte(address, 0)
    #When systemisoff is received
    if (message == "systemisoff"):
        print("System is off")
        GPIO.output(7, GPIO.LOW)
        GPIO.output(11, GPIO.LOW)
        bus.write_byte(address, 0)
    #when TurnExhaustOn
    if (message == "TurnExhaustON"):
        print("Exhaust On")
        bus.write_byte(address, 1)
    #when TurnExhaustOFF message received
    elif (message == "TurnExhaustOFF"):
        bus.write_byte(address, 0)
        print("Exhaust Off")
        
#connecting to the Server
ourClient = mqtt.Client("SIT210Rpi_ProjectFinal")
ourClient.connect("test.mosquitto.org", 1883)
#Subsribe to the Topics
ourClient.subscribe("System")
ourClient.subscribe("ExhaustCode14512")
ourClient.on_message = messageFunction    
ourClient.loop_start()             

# Main program loop
while(1):
    time.sleep(1)
    # Sleep for a second