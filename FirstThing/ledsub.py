import time
import paho.mqtt.client as paho
import random
import serial

broker = "broker.emqx.io"

port = "COM5"
connection = serial.Serial(port, timeout=1)

def send_command(cmd: str, 
                 response_len: int) -> str:
    connection.write(cmd.encode())
    if response_len !=0:
        #connection.in_waiting
        resp = connection.read(response_len)
        str_resp = resp.decode()
    return str_resp

def on_message(client, userdata, message):
    time.sleep(1)
    data = str(message.payload.decode("utf-8"))
    print("received message =", data)
    if data == "led on":
        resp = send_command("u", 6)
    elif data == "led off":
        resp = send_command("d", 7)
    

client = paho.Client("NikitaBagulov2")
client.on_message = on_message

print("Connecting to broker", broker)
client.connect(broker)
client.loop_start()
print("Subscribing")
client.subscribe("house/led")

time.sleep(300)
client.disconnect()
client.loop_stop()