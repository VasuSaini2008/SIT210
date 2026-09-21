import tkinter as tk
import RPi.GPIO as GPIO

# GPIO pins for each room
LIVING_ROOM_PIN = 18
BATHROOM_PIN = 23
CLOSET_PIN = 24

# Set GPIO numbering and LED pins
GPIO.setmode(GPIO.BCM)
GPIO.setup(LIVING_ROOM_PIN, GPIO.OUT)
GPIO.setup(BATHROOM_PIN, GPIO.OUT)
GPIO.setup(CLOSET_PIN, GPIO.OUT)

# Set up PWM for living room brightness control
living_pwm = GPIO.PWM(LIVING_ROOM_PIN, 1000)
living_pwm.start(0)


# Turn on the living room LED
def living_room():
    GPIO.output(BATHROOM_PIN, GPIO.LOW)
    GPIO.output(CLOSET_PIN, GPIO.LOW)
    living_pwm.ChangeDutyCycle(brightness.get())


# Turn on the bathroom LED
def bathroom():
    living_pwm.ChangeDutyCycle(0)
    GPIO.output(BATHROOM_PIN, GPIO.HIGH)
    GPIO.output(CLOSET_PIN, GPIO.LOW)


# Turn on the closet LED
def closet():
    living_pwm.ChangeDutyCycle(0)
    GPIO.output(BATHROOM_PIN, GPIO.LOW)
    GPIO.output(CLOSET_PIN, GPIO.HIGH)


# Change living room brightness using PWM
def change_brightness(value):
    if room.get() == "living":
        living_pwm.ChangeDutyCycle(float(value))


# Clean up GPIO and close the GUI
def close_program():
    living_pwm.stop()
    GPIO.cleanup()
    window.destroy()


# Create the main GUI window
window = tk.Tk()
window.title("Smart Home Lighting")
window.geometry("450x450")

room = tk.StringVar(value="living")
brightness = tk.IntVar(value=50)

# GUI title
title = tk.Label(
    window,
    text="Smart Home Lighting",
    font=("Arial", 20, "bold")
)
title.pack(pady=20)

# Room selection
tk.Label(
    window,
    text="Select Room",
    font=("Arial", 14)
).pack(pady=5)

living_button = tk.Radiobutton(
    window,
    text="Living Room",
    variable=room,
    value="living",
    command=living_room,
    font=("Arial", 12)
)
living_button.pack()

bathroom_button = tk.Radiobutton(
    window,
    text="Bathroom",
    variable=room,
    value="bathroom",
    command=bathroom,
    font=("Arial", 12)
)
bathroom_button.pack()

closet_button = tk.Radiobutton(
    window,
    text="Closet",
    variable=room,
    value="closet",
    command=closet,
    font=("Arial", 12)
)
closet_button.pack()

# Brightness slider for living room
tk.Label(
    window,
    text="Living Room Brightness",
    font=("Arial", 14)
).pack(pady=(30, 5))

brightness_slider = tk.Scale(
    window,
    from_=0,
    to=100,
    orient=tk.HORIZONTAL,
    length=300,
    variable=brightness,
    command=change_brightness
)
brightness_slider.pack()

# Exit button
exit_button = tk.Button(
    window,
    text="Exit",
    command=close_program,
    width=12
)
exit_button.pack(pady=30)

# Start the GUI
window.mainloop()

