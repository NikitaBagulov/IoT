import time
import paho.mqtt.client as paho
import random
responses = {'d':7,
            'u':6}

broker = "broker.emqx.io"

client = paho.Client("NikitaBagulov")

print("Connection to broker", broker)
client.connect(broker)
client.loop_start()
print("Publishing")
i=0
for _ in range(100):
    if i==0:
        state = "led off"
        print(f"state is {state}")
        i+=1
    else:
        state = "led on"
        print(f"state is {state}")
        i-=1
    client.publish("house/led", state)
    time.sleep(1)

client.disconnect()
client.loop_stop()