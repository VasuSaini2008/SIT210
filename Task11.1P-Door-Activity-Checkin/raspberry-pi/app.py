"""Door and Activity Check-in System dashboard.

Subscribes to door/system/# on the local Mosquitto broker and serves a
simple Flask dashboard. Development prototype; no authentication.
"""

from __future__ import annotations

import json
import threading
import time
from typing import Any

import paho.mqtt.client as mqtt
from flask import Flask, render_template_string

app = Flask(__name__)
MQTT_HOST = "localhost"
MQTT_PORT = 1883
HEARTBEAT_TIMEOUT_SECONDS = 15

state: dict[str, Any] = {
    "temperature": None,
    "humidity": None,
    "motion": 0,
    "distance": None,
    "help_requested": False,
    "online": False,
    "last_heartbeat": None,
    "last_alert": "No alert received",
}
state_lock = threading.Lock()


def on_connect(client, userdata, flags, reason_code, properties=None):
    if reason_code == 0:
        print("Connected to Mosquitto")
        client.subscribe("door/system/#")
    else:
        print(f"MQTT connection failed: {reason_code}")


def on_message(client, userdata, message):
    topic = message.topic
    payload = message.payload.decode("utf-8", errors="replace")

    if topic == "door/system/heartbeat":
        with state_lock:
            state["last_heartbeat"] = time.time()
            state["online"] = True
        return

    if topic == "door/system/telemetry":
        try:
            readings = json.loads(payload)
        except json.JSONDecodeError:
            print(f"Ignoring invalid telemetry JSON: {payload}")
            return

        allowed = {"temperature", "humidity", "motion", "distance", "help_requested"}
        update = {key: value for key, value in readings.items() if key in allowed}
        with state_lock:
            state.update(update)
        return

    if topic == "door/system/alert":
        with state_lock:
            state["last_alert"] = payload
            if payload == "HELP REQUESTED":
                state["help_requested"] = True
            elif payload == "HELP REQUEST CANCELLED":
                state["help_requested"] = False
        print(f"Alert: {payload}")
        return

    if topic == "door/system/status":
        print(f"Arduino status: {payload}")


def heartbeat_monitor():
    while True:
        with state_lock:
            last = state["last_heartbeat"]
            state["online"] = (
                last is not None and
                (time.time() - last) <= HEARTBEAT_TIMEOUT_SECONDS
            )
        time.sleep(1)


def start_mqtt():
    client = mqtt.Client(
        callback_api_version=mqtt.CallbackAPIVersion.VERSION2,
        client_id="raspberry-dashboard",
    )
    client.on_connect = on_connect
    client.on_message = on_message
    client.reconnect_delay_set(min_delay=1, max_delay=30)
    client.connect(MQTT_HOST, MQTT_PORT, keepalive=60)
    client.loop_start()
    return client


HTML = """
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="refresh" content="3">
  <title>Door and Activity Monitoring System</title>
  <style>
    body { margin:0; padding:24px; background:#101827; color:#f8fafc; font-family:Arial,sans-serif; }
    h1 { text-align:center; color:#38bdf8; }
    .status { text-align:center; font-size:21px; margin:24px; }
    .online { color:#22c55e; } .offline { color:#ef4444; }
    .grid { display:grid; grid-template-columns:repeat(auto-fit,minmax(190px,1fr)); gap:18px; max-width:1000px; margin:auto; }
    .card { background:#1e293b; padding:24px; border-radius:14px; text-align:center; min-height:90px; }
    .value { font-size:27px; color:#38bdf8; margin-top:14px; overflow-wrap:anywhere; }
    .alert { color:#f97316; }
    .note { text-align:center; color:#94a3b8; margin-top:25px; font-size:13px; }
  </style>
</head>
<body>
  <h1>Door and Activity Monitoring System</h1>
  <div class="status">Arduino Status:
    {% if d.online %}<span class="online">ONLINE</span>
    {% else %}<span class="offline">OFFLINE</span>{% endif %}
  </div>
  <main class="grid">
    <section class="card">Temperature<div class="value">{{ d.temperature if d.temperature is not none else "--" }} °C</div></section>
    <section class="card">Humidity<div class="value">{{ d.humidity if d.humidity is not none else "--" }} %</div></section>
    <section class="card">Motion Detection<div class="value">{{ "Motion Detected" if d.motion else "No Motion" }}</div></section>
    <section class="card">Distance<div class="value">{{ d.distance if d.distance is not none else "--" }} cm</div></section>
    <section class="card">Help Request<div class="value alert">{{ "HELP REQUIRED" if d.help_requested else "No Request" }}</div></section>
    <section class="card">Last Heartbeat<div class="value">{% if d.last_heartbeat %}{{ (now - d.last_heartbeat)|round(1) }} seconds ago{% else %}Not received{% endif %}</div></section>
  </main>
  <p class="note">Latest alert: {{ d.last_alert }}</p>
</body>
</html>
"""


@app.route("/")
def dashboard():
    with state_lock:
        snapshot = state.copy()
    return render_template_string(HTML, d=snapshot, now=time.time())


if __name__ == "__main__":
    threading.Thread(target=heartbeat_monitor, daemon=True).start()
    mqtt_client = start_mqtt()
    try:
        app.run(host="0.0.0.0", port=5000, debug=False)
    finally:
        mqtt_client.loop_stop()
        mqtt_client.disconnect()
