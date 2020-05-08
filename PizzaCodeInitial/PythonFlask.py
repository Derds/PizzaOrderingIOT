#Small Python Flask program to run a website on raspberry pi
#

# Import the flask library
from flask import Flask, render_template, redirect, url_for
from threading import Thread
#import serial
from time import sleep

#arduino will be giving serial read data about the toppings it finds
#serial_data = []
#open the serial port at this baud rate
#ser = serial.Serial('/dev/ttyUSB0', 115200)
#write a to serial to start reading
#ser.write(b"a")

#def get_arduino_data():
#        sleep(1)
        #write a to serial to start reading
#        ser.write(b"a")
#        while True:
                #TODO add timeout for readline 
                #https://pyserial.readthedocs.io/en/latest/shortintro.html
#                data = ser.readline().decode()
                #hacky way to display just the bytes
#                if "epc" in data:
#                        serial_data.append(data.split("epc[")[1].replace("]", ""))

#threading stuff
#serial_thread = Thread(target=get_arduino_data)
#serial_thread.daemon = True
#serial_thread.start()

#set up website application
app = Flask(__name__)
#Home page
@app.route("/")
def home():
    return render_template("index.html")

@app.route("/about/")
def about():
    return render_template("about.html")

@app.route("/<name>/")
def user(name):
    return f"Hello {name}!"

#Tags page showing just serial data
@app.route("/readTags/")    
def hello():
    #if serial data empty redirect, else display
    if not serial_data:
        return redirect(url_for("home"))
    #else:
    #    return "<br />".join(serial_data)

#TODO remove debug = true on final
#app.run(host='0.0.0.0', port=8080, debug=True)
app.run(debug=True)