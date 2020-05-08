#Small Python Flask program to run a website on raspberry pi
#

# Import the flask library
from flask import Flask, render_template, redirect, url_for, request
from threading import Thread
import serial
from time import sleep

serial_data = []
#arduino will be giving serial read data about the toppings it finds
#open the serial port at this baud rate
ser = serial.Serial('/dev/ttyUSB0', 115200)
#write a to serial to start reading
ser.write(b"a")

#threading stuff
serial_thread = Thread(target=get_arduino_data)
serial_thread.daemon = True
serial_thread.start()

#set up website application
app = Flask(__name__)
#set up routing
@app.route("/")
def home():
    return render_template("index.html")

@app.route("/about/")
def about():
    return render_template("about.html")

#Order - two methods get and post, if post then the user has clicked "order pizza" button
#If post request then take a username and redirect to read tags page
@app.route("/order/", methods=["POST", "GET"])
def order():
    if request.method == "POST":
        #readTags
        name = request.form["username"]
        return redirect(url_for("readTags", user = name))
    else:
        return render_template("orderPizza.html")


#Tags page showing just serial data
@app.route("/readTags/<user>")    
def readTags(user):
    #TODO set up default value if username not given

    #if serial data empty redirect, else display
    getPizzaToppings()

    if not serial_data:
        return redirect(url_for("/order"))
    else:
        return f"<h1>{user}'s Pizza Order</h1><p></p><br />".join(serial_data)

def getPizzaToppings():
    sleep(1)
    #write a to serial to start reading
    ser.write(b"a")
    while True:
        #TODO add timeout for readline 
        #https://pyserial.readthedocs.io/en/latest/shortintro.html
        data = ser.readline().decode()
        #hacky way to display just the bytes
        if "epc" in data:
                serial_data.append(data.split("epc[")[1].replace("]", ""))

#TODO remove debug = true on final
app.run(host='0.0.0.0', port=8080, debug=True)
#app.run(debug=True)