import paho.mqtt.client as mqtt
import json
import pprint
import time
import random as rnd

broker_address="192.168.0.161" 
client = mqtt.Client("python")
client.connect(broker_address, 11883, 30) 
for i in range(0, 100):
   print(i)
   temp = 111.5
   hum = rnd.uniform(0, 100)
   room_num = rnd.randint(1, 2)
   msg = "temperature,humidity,pressure\n{},{},{}".format(temp, hum, 10.0)
   client.publish("room{}".format(room_num), msg)
        
   time.sleep(15)

client.disconnect()