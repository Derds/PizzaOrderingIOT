
#get this to display a list of toppings - or save a list of toppings to CSV.

# Import the flask library
from flask import Flask, render_template, redirect, url_for
from threading import Thread
import serial
from serial import SerialException
from time import sleep

# Set up the website
app = Flask(__name__)


#error handling
@app.errorhandler(404)
def page_not_found(e):
    # note that we set the 404 status explicitly
    return render_template('404.html'), 404

serial_data = []
errormsg = ""
try:
    ser = serial.Serial('/dev/ttyUSB0', 115200)
    ser.write(b"a")
except SerialException as e:
    global errormsg #tell python to treat error message as global
    errormsg = "Serial Port Exception: {e}"
    print(errormsg)
    #return redirect(url_for("/error", msg = errormsg))

#error handling
@app.route("/error/")
def error():
        return render_template('error.html', message=errormsg)


def get_arduino_stuff():
    try:
        sleep(1)
        ser.write(b"a")
        while True:
                data = ser.readline().decode()
                if "epc" in data:
                        serial_data.append(data.split("epc[")[1].replace("]", ""))
    except Exception as e:
        global errormsg #tell python to treat error message as global
        errormsg = "Issue retrieving data: {{e}}"
        return redirect(url_for("error"))


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
        data = "<br />".join(serial_data)
        if serial_data:
            return data
        else:
            global errormsg #tell python to treat error message as global
            errormsg = "Empty data set: no serial data being input"
            return redirect(url_for("error"))
    except Exception as e:
        global errormsg #tell python to treat error message as global
        errormsg = "Issue retrieving data: {{e}}"
        return redirect(url_for("error"))

app.run(host='0.0.0.0', port=8080, debug=True)