#Small Python Flask program to run a website on raspberry pi
#

# Import the flask library
from flask import Flask, render_template, redirect, url_for, request
from threading import Thread
import serial, time
from time import sleep
from serial import SerialException

#json files
import json
#csv files
import time
import csv

    
#set up website application
app = Flask(__name__)

#arduino will be giving serial read data about the toppings it finds
serial_data = []
try:
    #open the serial port at this baud rate
    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout =5)
    #write a to serial to start reading
    ser.write(b"a")
    ser.flushInput() #tells the serial port to clear the queue so that data doesn't overlap and create erroneous data points
    print("Serial Port Opened")
except SerialException as e:
    print("Serial Port Exception:")
    print(e)
    redirect(url_for('error'))

errormsg = ""
#error handling
@app.route("/error/")
def error():
        return render_template('error.html', message=errormsg)

@app.errorhandler(404)
def page_not_found(e):
    # note that we set the 404 status explicitly
    return render_template('404.html')  #todo- make 404 page
    #return render_template(url_for('error'))

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
    #default value if no username is given
    if not user:
        user = "This User"
    #if serial data empty redirect, else display
    #getPizzaToppings()
    try:
        #get_arduino_stuff() might not need to call this? thread might call it?
        data = "<br />".join(serial_data)
        if serial_data:
            return "<h1>{user}'s Pizza Order</h1><p></p><br />".join(data)
        else:
            print("Empty data set: no serial data being input")
            return redirect(url_for('error'))

    except Exception as e:
        print("Issue retrieving data:")
        print(type(e))
        print(e)
        return redirect(url_for('error'))

#TODO save to csv or json --->
# def getPizzaToppingsDifferently():
#     decoded_bytes = float(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
#     print(decoded_bytes)
#     with open("test_data.csv","a") as f:
#         writer = csv.writer(f,delimiter=",")
#         writer.writerow([time.time(),decoded_bytes])


# def getPizzaToppings():
#     sleep(1)
#     #write a to serial to start reading
#     ser.write(b"a")
#     while True:
#         try:
#             #TODO add timeout for readline 
#             #serial = serial.Serial("/dev/ttyUSB0", 9600, timeout=1) #timeout for if it cant find within that time.
#             #https://pyserial.readthedocs.io/en/latest/shortintro.html
#             ser_bytes = ser.readline()
#             decoded_bytes = ser_bytes.decode()
#             print(decoded_bytes)
#             with open("toppings_data.csv","a") as f:  #find 
#                 writer = csv.writer(f,delimiter=",")
#                 writer.writerow([time.time(),decoded_bytes])
#             #hacky way to display just the bytes
#             #if "epc" in data:
#             #    serial_data.append(data.split("epc[")[1].replace("]", ""))
#         except:
#             print("Keyboard Interrupt")
#             break

def get_arduino_stuff():
    try:
        #sleep(1)
        ser.write(b"a")
        timeout = time.time() + 15# 15seconds from now
        while True:
            data = ser.readline().decode()
            if "epc" in data:
                serial_data.append(data.split("epc[")[1].replace("]", ""))
                #save data to csv file 
            #debug print statements
            if data:
                print("data")
                print(data)
            if serial_data:
                print("Serial Data:")
                print(serial_data)
            
            if "Module failed to respond. Please check wiring." in data:
                print(data)
                raise Exception("Module not wired correctly or power low")
            #break out of read loop after timeout
            if time.time() > timeout:
                break
            
    except Exception as e:
        print("Issue retrieving data:")
        print(type(e))
        print(e)
        return redirect(url_for('error'))

#threading stuff
serial_thread = Thread(target=get_arduino_stuff)
serial_thread.daemon = True
serial_thread.start()


#TODO remove debug = true on final
app.run(host='0.0.0.0', port=8080, debug=True)
#app.run(debug=True)