#Small Python Flask program to run a website on raspberry pi
# Input: serial data from arduino
# Output: Several flask pages showing 
# -> nutritional information
# -> list of chosen toppings 
# -> graph of pizza nutrition from CSV of nutritional data


#
#IMPORT LIBRARIES
#

# Import the flask library
from flask import Flask, render_template, redirect, url_for, send_file, request

#data vis libraries
from io import BytesIO
import pandas as pd
import matplotlib.pyplot as plot 

#libraries for serial read, sleep and threads
from threading import Thread
import serial, time
from serial import SerialException
from time import sleep

#libraries to match strings and to decode serial data
import re #regex
import codecs


#
#INITIALISE VARS AND FLASK SITE
#   
#set up website application
app = Flask(__name__)

#input: CSV files
filepath = "data/sample_data.csv" #sample data holds a file of nutritional data about each of the topping options
nutrients_data = pd.read_csv(filepath, keep_default_na = False)

#initialise nutrient values
pizzaSize = 4 #TODO: change pizza size 
calories = fat = saturates = sugar = salt = 0
#create a setS for toppings, allergens and vitamins - this means that it will only hold unique values, unlike a list
vits_minerals = set()
allergens = set()
toppings_set = set()
serial_data = False

#
# ERROR HANDLING
#
global errormsg
errormsg = ""
@app.route('/favicon.ico')
def favicon():
    return redirect(url_for('static', filename='favicon.ico'))

@app.route("/error/")
def error():
        return render_template('error.html', message=errormsg)

#
#OPEN SERIAL PORT
#
#arduino will be giving serial read data about the toppings it finds
try:
    #open the serial port at this baud rate
    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout =5)
    #write a to serial to start reading
    ser.write(b"a\n")
    ser.readline()
    #ser.flushInput() #tells the serial port to clear the queue so that data doesn't overlap and create erroneous data points
    print("Serial Port Opened")
except SerialException as e:
    print("Serial Port Exception:")
    print(e)

#
#MAIN FUNCTIONALITY
#

#Function reads arduino serial data - called constantly by thread
def get_arduino_stuff():
    #Declare vars
    cleanData= ""
    global toppings_set, serial_data

    try:
        ser.write(b"a\n")
        sleep(0.1)
        ser.readline()
        #ser.flush()
        #timeout = time.time() + 25# 15seconds from now
        while True:
            try:
                #data = ser.read(200).decode()#reads 200 bytes
                data = ser.readline()
                if data:
                    #ser_bytes = ser.readline() #remove trailing newline
                    #data = decoded_bytes
                    data = data.decode().strip()
                    print("New data found! {}".format(data))
                    if "Module failed to respond. Please check wiring." in data:
                        print(data)
                        raise Exception("Module not wired correctly or power low")

                    #TODO stop from reading several values close beside eachother.
                    if "epc" in data:
                        #TODO tidy this
                        #this is important for later display to work - replace with toppings set later
                        #serial_data.append(data.split("]")[1].replace("]", ""))
                        serial_data = True

                        regex = r"epc\[([0-9A-F\s]*)\]"
                        listOfToppingData = re.findall(regex, data) #finds a list of all the topping byte arrays

                        #debug print
                        print("REGEX Toppings List")
                        print(listOfToppingData)

                        for x in listOfToppingData:
                            cleanData = codecs.decode(x.replace(" ",""), "hex")
                            #if cleanData != "Bad CRC":
                            #add unique to set to keep track of toppings.
                            cleanData = cleanData.decode()
                            toppings_set.add(cleanData)
                        sleep(1)
                    #debug print statements
                    if data:
                        print("data:")
                        print(data)
                    if toppings_set:
                        print("Set:")
                        print(toppings_set)

                    #break out of read loop after timeout
                    # if time.time() > timeout:
                    #     break
            except UnicodeDecodeError:
                print("decoding error")
    except Exception as e:
        print("Issue retrieving data:")
        print(type(e))
        print(e)
        #return redirect(url_for("error"))

#create and maintain a seperate background thread to constantly read serial data that runs until program ends
serial_thread = Thread(target=get_arduino_stuff)
serial_thread.daemon = True
serial_thread.start()

#Get the initial nutrients for the size of the pizza 
#TODO: Call after setting pizza size
def getSliceNutrients():
    #get pizza size * slice
    global calories, fat, saturates, sugar, salt
    row = nutrients_data[nutrients_data["Lookup"]=="Slice"]
    #update nutrients
    calories += (row['Calories'].values[0] * pizzaSize)
    fat += (row['Fat'].values[0] * pizzaSize)
    saturates += (row['Saturates'].values[0] * pizzaSize)
    sugar += (row['Sugar'].values[0] * pizzaSize)
    salt += (row['Salt'].values[0] * pizzaSize)

    vitamin = row['VitaminMineral'].values[0]
    if vitamin != '' :
        list = str(vitamin).replace("'","").replace(" ","")
        list = list.split(",")
        #add each element to the set- will only add items that are unique
        for x in list:
            vits_minerals.add(x)
    allergen = row['Allergens'].values[0]
    if allergen != '':
        list = str(allergen).replace("'","").replace(" ","")
        list = list.split(",")
        #add each element to the set- will only add items that are unique
        for x in list:
            allergens.add(x)

#Call once - or rewrite to call per topping
def calculateNutrients():
    global calories, fat, saturates, sugar, salt
    try:
        for topping in toppings_set:
            #find row in nutrients data where lookup equals the topping
            row = nutrients_data[nutrients_data["Lookup"]==topping]
            # print("row")
            # print(row)
            #if row found
            calories += row['Calories'].values[0]
            # print("row [calories]")
            # print(row['Calories'].values[0])
            #Update the running totals
            fat += row['Fat'].values[0]
            saturates += row['Saturates'].values[0]
            sugar += row['Sugar'].values[0]
            salt += row['Salt'].values[0]

            vitamin = row['VitaminMineral'].values[0]
            allergen = row['Allergens'].values[0]
            # print("allergen")
            # print(row['Allergens'].values[0])
            if not vitamin in vits_minerals and vitamin != '' :

                #strip ' and spaces and then split by , into list
                list = str(vitamin).replace("'","").replace(" ","")
                list = list.split(",")
                #DEBUG print(list)
                #add each element to the set- will only add items that are unique
                for x in list:
                    vits_minerals.add(x)

            if allergen != '' and not allergen in allergens:
                list = str(allergen).replace("'","").replace(" ","")
                list = list.split(",")
                #add each element to the set- will only add items that are unique
                for x in list:
                    allergens.add(x)
    except Exception as e:
        print(e)

def plotBarChart():
    #Make the results into a new data frame
    summary = {'Calories': [calories],
    'Fat': [fat], 'Saturates': [saturates],
    'Sugar': [sugar], 'Salt': [salt]
    }
    df = pd.DataFrame(summary, columns = ['Calories', 'Fat', 'Saturates', 'Sugar', 'Salt' ])
    #print(df)
    # Draw a vertical bar chart
    #TODO add ticks on x axis
    df.plot.bar( title="Summary of Nutritional Information")
    plot.xlabel('Nutritional Information')
    plot.ylabel('Calories / grams of each nutrient, per 100g of food')
    return plot

#
# ROUTING
#
#set 404 page
@app.errorhandler(404)
def page_not_found(e):
    return render_template('404.html') 

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
    message=""
    if request.method == "POST":
        #don't move on until serial data found
        if not serial_data:
            message = "No topping data found"
            return render_template("orderPizza.html", message=message)
        #else take personalisation fields
        name = request.form['username']
        if not name:
            name = "This user"
        age = request.form.get('age')
        sex = request.form.get('optradio')
        if not sex:
            sex = "Female"
        global slice
        slice = request.form.get('slices')
        print(name, age, sex, slice)
        return redirect(url_for("plot", user = name))
    else:
        return render_template("orderPizza.html", message=message)


#Tags page showing just serial data
@app.route("/readTags/<user>")    
def readTags(user):
    #default value if no username is given
    
    #if serial data empty redirect, else display
    #getPizzaToppings()
    try:
        #get_arduino_stuff() might not need to call this? thread might call it?
        if serial_data:
            data = "<br />".join(toppings_set)
            return "<h1>{user}'s Pizza Order</h1><p></p><br />".join(data)
        else:
            print("Empty data set: no serial data being input")
            global errormsg
            errormsg = "No topping data found"
            return redirect(url_for('error'))

    except Exception as e:
        print("Issue retrieving data:")
        print(type(e))
        print(e)
        global errormsg
        errormsg = "No topping data found"
        return redirect(url_for('error'))

@app.route("/plot")
def simplePlot(user):
        return render_template("simpleplot.html", user=user)

@app.route("/toppings_list", methods=['POST'])
def toppings_list():
    try:
        #data = "<br />".join(toppings_set)
        #if serial_data:
        if toppings_set:
            getSliceNutrients() #change the pizza size somewhere
            calculateNutrients() #calculate all the nutrients
            mychart = plotBarChart()
            img = BytesIO()
            mychart.savefig(img)
            img.seek(0)
            #plot.show(block=True)
            #return data
            return send_file(img, mimetype='image/png')
        else:
            #TODO could make this cleaner...
            print("Empty data set: no serial data being input")
            global errormsg
            errormsg = "No topping data found"
            return redirect(url_for('error'))
    except Exception as e:
        print("Issue retrieving data:")
        print(type(e))
        print(e)
        global errormsg
        errormsg = "No topping data found, restart your system and try again."
        return redirect(url_for('error'))

#Code source: https://stackoverflow.com/questions/34066804/disabling-caching-in-flask
#Prevents image caching
@app.after_request
def add_header(r):
    """
    Add headers to both force latest IE rendering engine or Chrome Frame,
    and also to cache the rendered page for 10 minutes.
    """
    r.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    r.headers["Pragma"] = "no-cache"
    r.headers["Expires"] = "0"
    r.headers['Cache-Control'] = 'public, max-age=0'
    return r

#TODO remove debug = true on final
app.run(host='0.0.0.0', port=8080, debug=True)
#app.run(debug=True)