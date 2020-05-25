
#get this to display a list of toppings - or save a list of toppings to CSV.

# Import the flask library
from flask import Flask, render_template, redirect, url_for
from threading import Thread
import serial, time
from serial import SerialException
from time import sleep

# Set up the website
app = Flask(__name__)

#error handling

@app.route('/favicon.ico')
def favicon():
    return redirect(url_for('static', filename='favicon.ico'))

@app.errorhandler(404)
def page_not_found(e):
    # note that we set the 404 status explicitly
    #return render_template('404.html'), 404  #TODO- make 404 page
    return render_template(url_for("error"))

serial_data = []
errormsg = ""
try:
    ser = serial.Serial('/dev/ttyUSB0',115200, timeout =5, bytesize=serial.EIGHTBITS)
    ser.write(b"a")
    ser.flush()
    print("Serial Port Opened")
except SerialException as e:
    print("Serial Port Exception:")
    print(e)
    #redirect(url_for("error"))

#error handling
@app.route("/error/")
def error():
    #return render_template("error.html", message=errormsg)
    return render_template("error.html")

#toppings_set = {}


def get_arduino_stuff():
    cleanData= ""
    
    #create a set for toppings, this means that it will only hold unique values, unlike a list
    toppings_set = {}
    toppings_set = set()
    try:
        ser.write(b"a")
        sleep(1)
        ser.flush()
        timeout = time.time() + 15# 15seconds from now
        while True:
            try:
                data = ser.read(200).decode()
                #data = ser.read(150).decode()
                #ser_bytes = ser.readline() #remove trailing newline
                #data = float(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
                #data = line.decode("utf-8") #not ascii
                #data = decoded_bytes
            except UnicodeDecodeError:
                print("decoding error")
            if "epc" in data:
                
                cleanData = data.split("]")[len(data.split("]"))]#just the text
                serial_data.append(data.split("epc[")[1].replace("]", ""))
                toppings_set.add(cleanData)
            #debug print statements
            if data:
                print("data:")
                print(data)
            if cleanData:
                print("clean:")
                print(cleanData)
            if serial_data:
                print("Serial Data:")
                print(serial_data)
            if toppings_set:
                print(toppings_set)
            
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
        #return redirect(url_for("error"))

serial_thread = Thread(target=get_arduino_stuff)
serial_thread.daemon = True
serial_thread.start()

# This is a website route just using a string
@app.route("/")
def hello():
        return "<h1>Hello</h1> <br/> <a href='/list'><input type='button'>Get Toppings List</input></a>"

@app.route("/list")
def list():
    try:
        get_arduino_stuff()
        data = "<br />".join(serial_data)
        if serial_data:
            return data
        else:
            #TODO could make this cleaner...
            print("Empty data set: no serial data being input")
            return redirect(url_for('error'))
    except Exception as e:
        print("Issue retrieving data:")
        print(type(e))
        print(e)
        return redirect(url_for('error'))

app.run(host='0.0.0.0', port=8080, debug=True)
#app.run( debug=True)

##TODO: test this 
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

#Python Dynamic List>
# x = []
# n = input("enter length")
# for i in range(1, int(n)):
#     k=input("enter value")
#     x.append(k) # push your entered value

# print x