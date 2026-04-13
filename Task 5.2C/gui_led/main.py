import tkinter as tk
import RPi.GPIO as GPIO

# -------------------------------
# GPIO SETUP
# -------------------------------
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

LED_LIVING = 17
LED_BATHROOM = 27
LED_CLOSET = 22

GPIO.setup(LED_LIVING, GPIO.OUT)
GPIO.setup(LED_BATHROOM, GPIO.OUT)
GPIO.setup(LED_CLOSET, GPIO.OUT)

# PWM setup
pwm_living = GPIO.PWM(LED_LIVING, 100)
pwm_bathroom = GPIO.PWM(LED_BATHROOM, 100)

pwm_living.start(0)
pwm_bathroom.start(0)


# -------------------------------
# HELPER FUNCTIONS
# -------------------------------

def turn_off_all():
    pwm_living.ChangeDutyCycle(0)
    pwm_bathroom.ChangeDutyCycle(0)
    GPIO.output(LED_CLOSET, GPIO.LOW)


def toggle_led(pwm, state, slider):
    if state.get():
        pwm.ChangeDutyCycle(slider.get())
    else:
        pwm.ChangeDutyCycle(0)


def update_brightness(pwm, value, state):
    """
    Update brightness only if LED is ON
    """
    if state.get():
        pwm.ChangeDutyCycle(int(value))


def start_timer(pwm, state):
    """
    Auto fade brightness using Tkinter timer
    """
    if not state.get():
        return 

    duty = 0

    def fade():
        nonlocal duty
        if not state.get():
            return 

        duty = (duty + 5) % 100
        pwm.ChangeDutyCycle(duty)
        root.after(200, fade)

    fade()


def toggle_closet():
    """Simple ON/OFF for closet"""
    if closet_state.get():
        GPIO.output(LED_CLOSET, GPIO.HIGH)
    else:
        GPIO.output(LED_CLOSET, GPIO.LOW)


def exit_program():
    try:
        pwm_living.stop()
        pwm_bathroom.stop()
    except:
        pass

    GPIO.cleanup()
    root.destroy()


# -------------------------------
# GUI SETUP
# -------------------------------
root = tk.Tk()
root.title("Smart Home Control Panel")
root.geometry("400x520")


# LIVING ROOM
living_frame = tk.LabelFrame(root, text="Living Room", padx=10, pady=10)
living_frame.pack(fill="x", padx=10, pady=5)

living_state = tk.BooleanVar()

living_toggle = tk.Checkbutton(
    living_frame,
    text="ON/OFF",
    variable=living_state,
    command=lambda: toggle_led(pwm_living, living_state, living_slider)
)
living_toggle.pack(anchor="w")

living_slider = tk.Scale(
    living_frame,
    from_=0,
    to=100,
    orient="horizontal",
    label="Brightness",
    command=lambda v: update_brightness(pwm_living, v, living_state)
)
living_slider.set(50)
living_slider.pack(fill="x")

tk.Button(
    living_frame,
    text="Start Timer (Fade)",
    command=lambda: start_timer(pwm_living, living_state)
).pack(pady=5)


# BATHROOM
bathroom_frame = tk.LabelFrame(root, text="Bathroom", padx=10, pady=10)
bathroom_frame.pack(fill="x", padx=10, pady=5)

bathroom_state = tk.BooleanVar()

bathroom_toggle = tk.Checkbutton(
    bathroom_frame,
    text="ON/OFF",
    variable=bathroom_state,
    command=lambda: toggle_led(pwm_bathroom, bathroom_state, bathroom_slider)
)
bathroom_toggle.pack(anchor="w")

bathroom_slider = tk.Scale(
    bathroom_frame,
    from_=0,
    to=100,
    orient="horizontal",
    label="Brightness",
    command=lambda v: update_brightness(pwm_bathroom, v, bathroom_state)
)
bathroom_slider.set(50)
bathroom_slider.pack(fill="x")

tk.Button(
    bathroom_frame,
    text="Start Timer (Fade)",
    command=lambda: start_timer(pwm_bathroom, bathroom_state)
).pack(pady=5)


# CLOSET
closet_frame = tk.LabelFrame(root, text="Closet", padx=10, pady=10)
closet_frame.pack(fill="x", padx=10, pady=5)

closet_state = tk.BooleanVar()

tk.Checkbutton(
    closet_frame,
    text="ON/OFF",
    variable=closet_state,
    command=toggle_closet
).pack(anchor="w")


# EXIT BUTTON
tk.Button(root, text="Exit", command=exit_program).pack(pady=15)


# Run GUI
root.mainloop()
