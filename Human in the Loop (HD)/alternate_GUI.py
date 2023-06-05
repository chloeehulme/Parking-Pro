'''
   Generates a GUI that users can interact with to select a parling type for
   the rover. Sends the selected parking type via bluetooth to the rover.
   Uses:  Tinter, Pyserial
   Author: Chloe Hulme
'''

import serial
from tkinter import *
import tkinter.font

global parking_type


# Initialise serial comms
port = serial.Serial('/dev/cu.HC-05', 9600, timeout=1)
# port.flushInput() 

print("Connected")


# GUI definitions
win = Tk()
win.title("Parking Types")
myFont = tkinter.font.Font(family='Helvetica', size=15, weight="bold")


# Sends the parking type (chosen in GUI) via bluetooth
def set_forward_park():
    parking_type = "forward"
    port.write(str.encode('1'))
    print(parking_type)

def set_reverse_park():
    parking_type = "reverse"
    port.write(str.encode(str(parking_type)))
    print(parking_type)

def set_parallel_park():
    parking_type = "parallel"
    port.write(str.encode(str(parking_type)))
    print(parking_type)

# Closes window
def close():
    win.destroy()

# Create widgets
Forward = Button(win, text='Forward Park', font=myFont, command=set_forward_park, bg='green', height=3, width=15)
Reverse = Button(win, text='Reverse Park', font=myFont, command=set_reverse_park, bg='orange', height=3, width=15)
Parallel = Button(win, text='Parallel Park', font=myFont, command=set_parallel_park, bg='red', height=3, width=15)

# Positions widgets
Forward.grid(row=0, column=1)
Reverse.grid(row=1, column=1)
Parallel.grid(row=2, column=1)

# Create additional exit button
exitButton = Button(win, text='Exit', font=myFont, command=close, bg='seashell1', height=1, width=6)
exitButton.grid(row=3, column=4)

# Link top RHS x to close function
win.protocol("WM_DELETE_WINDOW", close)

win.mainloop()
