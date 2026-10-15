```python
import tkinter as tk
import RPi.GPIO as GPIO


# Set the GPIO pin for each room's LED
ROOM_PINS = {
    "Living Room": 14,
    "Bathroom": 15,
    "Closet": 18,
}

# Set the PWM speed for controlling LED brightness
PWM_FREQUENCY = 1000

# Use BCM numbering for GPIO pins
GPIO.setmode(GPIO.BCM)


# Set up a PWM controller for each room's LED
room_pwm = {}

for room, pin in ROOM_PINS.items():
    GPIO.setup(pin, GPIO.OUT)  # Set the pin as an output

    # Create a PWM controller for the LED
    controller = GPIO.PWM(pin, PWM_FREQUENCY)

    # Start the LED with 0% brightness
    controller.start(0)

    # Store the controller for later use
    room_pwm[room] = controller


# Change the brightness of the selected room's LED
def set_brightness(room, value):

    # Apply the brightness value to the LED
    room_pwm[room].ChangeDutyCycle(float(value))


# Turn off all lights by resetting the sliders
def switch_all_off():

    # Set every slider to zero brightness
    for slider in sliders.values():
        slider.set(0)


# Stop the lights and close the application
def close_window():

    # Stop PWM control for all LEDs
    for controller in room_pwm.values():
        controller.stop()

    # Reset the GPIO pins
    GPIO.cleanup()

    # Close the application window
    app.destroy()


# Create the main application window
app = tk.Tk()

# Set the window title and size
app.title("Home Light Control")
app.geometry("500x520")


# Add the main heading
tk.Label(
    app,
    text="Home Light Control",
    font=("Arial", 20, "bold"),
).pack(pady=15)


# Store the brightness sliders for each room
sliders = {}


# Create a brightness slider for each room
for room in ROOM_PINS:

    # Show the room name
    tk.Label(
        app,
        text=room,
        font=("Arial", 14, "bold"),
    ).pack()

    # Create a slider to control brightness from 0 to 100
    slider = tk.Scale(
        app,
        from_=0,
        to=100,
        orient=tk.HORIZONTAL,
        length=350,
        label="Brightness",

        # Update the LED brightness when the slider moves
        command=lambda value, selected_room=room: set_brightness(
            selected_room, value
        ),
    )

    # Display the slider
    slider.pack(pady=10)

    # Save the slider for the selected room
    sliders[room] = slider


# Add a button to turn off all lights
tk.Button(
    app,
    text="TURN OFF ALL",
    width=20,
    command=switch_all_off,
).pack(pady=15)


# Add a button to close the application
tk.Button(
    app,
    text="EXIT",
    width=20,
    command=close_window,
).pack(pady=5)


# Clean up the GPIO pins when the window is closed
app.protocol("WM_DELETE_WINDOW", close_window)


# Keep the application running
app.mainloop()
```
