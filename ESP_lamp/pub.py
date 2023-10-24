import paho.mqtt.client as paho
from datetime import datetime

broker = "broker.emqx.io"

client = paho.Client("client-isu-153")

print("Connection to broker", broker)
client.connect(broker)
client.loop_start()
print("Publishing")

topic = input("Enter topic: ")

current_time = datetime.now()
last_min = current_time.minute
last_sec = current_time.second

led_on = False

min_sec = 20
max_sec = 40

while True:
    current_time = datetime.now()
    current_min = current_time.minute
    current_sec = current_time.second

    if (current_min != last_min):
        if (max_sec != 35):
            max_sec -= 1
        elif min_sec != 25:
            min_sec += 1
        else:
            min_sec = 20
            max_sec = 40
    elif not led_on and current_sec >= min_sec and current_sec <= max_sec: #ещё не отправили, чтобы лампочка горела
        state = 'd'
        client.publish(topic, state)
        print(f"state is {state}")
        led_on = True
    elif led_on and (current_sec < min_sec or current_sec > max_sec):
        state = 'u'
        client.publish(topic, state)
        print(f"state is {state}")
        led_on = False

    last_min = current_min
    last_sec = current_sec
