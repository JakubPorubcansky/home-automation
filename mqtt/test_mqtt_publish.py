import paho.mqtt.client as mqtt
import json
import pprint
import time

# dict_msg = {
#     "contact": True,
#     "something": 1.5,
#     "temperature": 20.5,
#     "my_field": "awesome",
# }
# msg = json.dumps(dict_msg)
msg = "measurement,cpu,time_user,time_system,time_idle,time\ncpu,cpu0,42,42,42,2018-09-13T13:03:28Z"
# msg = "19.2"

pprint.pprint(msg)

MQTT_HOST = "192.168.0.161"
MQTT_PORT = 11883
MQTT_KEEPALIVE_INTERVAL = 45

count = 0


def on_publish(client, userdata, mid):
    print("Message Published to MQTT ...")


mqttc = mqtt.Client("python_client")
mqttc.on_publish = on_publish
# mqttc.username_pw_set("esp1","esp1")
mqttc.connect(MQTT_HOST, MQTT_PORT, MQTT_KEEPALIVE_INTERVAL)

mqttc.publish("temperature", msg)

mqttc.disconnect()
time.sleep(2)



msg = "measurement,cpu,time_user,time_system,time_idle,time\ncpu,cpu0,100,42,42,2018-09-13T13:03:28Z"
broker_address="192.168.0.161" 
client = mqtt.Client("python") #create new instance
client.connect(broker_address, 11883) #connect to broker
# client.subscribe("house/bulbs/bulb1")
client.publish("temperature", msg)
client.disconnect()