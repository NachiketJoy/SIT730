import tkinter as tk
from tkinter import ttk
import RPi.GPIO as GPIO

# -----------------------------
# GPIO SETUP
# -----------------------------

GPIO.setmode(GPIO.BCM)

LIVING_ROOM_PIN = 17
BATHROOM_PIN = 27
CLOSET_PIN = 22

GPIO.setup(LIVING_ROOM_PIN, GPIO.OUT)
GPIO.setup(BATHROOM_PIN, GPIO.OUT)
GPIO.setup(CLOSET_PIN, GPIO.OUT)

GPIO.output(LIVING_ROOM_PIN, GPIO.LOW)
GPIO.output(BATHROOM_PIN, GPIO.LOW)
GPIO.output(CLOSET_PIN, GPIO.LOW)

def turn_off_all():
    GPIO.output(LIVING_ROOM_PIN, GPIO.LOW)
    GPIO.output(BATHROOM_PIN, GPIO.LOW)
    GPIO.output(CLOSET_PIN, GPIO.LOW)

def set_led(selection):
    turn_off_all()
    if selection == "living":
        GPIO.output(LIVING_ROOM_PIN, GPIO.HIGH)
    elif selection == "bathroom":
        GPIO.output(BATHROOM_PIN, GPIO.HIGH)
    elif selection == "closet":
        GPIO.output(CLOSET_PIN, GPIO.HIGH)

def on_exit():
    turn_off_all()
    GPIO.cleanup()
    root.destroy()

# -----------------------------
# TKINTER GUI
# -----------------------------

root = tk.Tk()
root.title("Home Lights Control")
root.geometry("300x200")

title_label = ttk.Label(root, text="Select a room to turn ON its light:")
title_label.pack(pady=10)

selected_room = tk.StringVar(value="none")

def on_radio_change():
    set_led(selected_room.get())

ttk.Radiobutton(root, text="Living Room", value="living",
                variable=selected_room, command=on_radio_change).pack(anchor="w", padx=20, pady=5)

ttk.Radiobutton(root, text="Bathroom", value="bathroom",
                variable=selected_room, command=on_radio_change).pack(anchor="w", padx=20, pady=5)

ttk.Radiobutton(root, text="Closet", value="closet",
                variable=selected_room, command=on_radio_change).pack(anchor="w", padx=20, pady=5)

ttk.Button(root, text="Exit", command=on_exit).pack(pady=15)

root.protocol("WM_DELETE_WINDOW", on_exit)
root.mainloop()