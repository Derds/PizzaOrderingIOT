
# Input: serial data from arduino
# Output: Basic flask page showing list of toppings and graph of pizza nutrition from CSV of nutritional data

#
#IMPORT LIBRARIES
#

# Import the flask library
from flask import Flask, render_template, redirect, url_for, send_file

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

# Set up the website
app = Flask(__name__)

#input: CSV files - could use python CSV functions, but pandas handles it nicely
#filepath = "C:\\Users\\catlo\\Desktop\\Dissertation\\PizzaCode\\sample_data.csv"
filepath = "data/sample_data.csv"
nutrients_data = pd.read_csv(filepath, keep_default_na = False)

#initialise nutrient values
pizzaSize = 4 #TODO: change pizza size 
calories = fat = saturates = sugar = salt = 0
#create a setS for toppings, allergens and vitamins - this means that it will only hold unique values, unlike a list
vits_minerals = set()
allergens = set()
toppings_set = set()

# error handling
@app.route('/favicon.ico')
def favicon():
    return redirect(url_for('static', filename='favicon.ico'))

@app.errorhandler(404)
def page_not_found(e):
    # note that we set the 404 status explicitly
    return render_template('404.html') #TODO- make 404 page
    #return render_template(url_for("error"))

serial_data = []
errormsg = ""
#OPEN SERIAL PORT
try:
    ser = serial.Serial('/dev/ttyUSB0',115200, timeout =5)
    ser.write(b"a\n")
    ser.readline()
    #ser.flush()
    print("Serial Port Opened")
except SerialException as e:
    print("Serial Port Exception:")
    print(e)

#error handling
@app.route("/error/")
def error():
    #return render_template("error.html", message=errormsg)
    return render_template("error.html")

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
        timeout = time.time() + 25# 15seconds from now
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
                        serial_data.append(data.split("]")[1].replace("]", ""))

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
                    # if cleanData:
                    #     print("clean:")
                    #     print(cleanData)
                    # if serial_data:
                    #    print("Serial Data:")
                    #    print(serial_data)
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

#create background thread that runs until program ends
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
    df.plot.bar( title="Summary of Nutritional Information")
    plot.xlabel('Nutritional Information')
    plot.ylabel('Calories / grams of each nutrient, per 100g of food')
    return plot

# This is a website route just using a string
@app.route("/")
def hello():
        return "<h1>Hello</h1> <br/> <a href='/top_list'><input type='button'>Get Toppings List</input></a>"

@app.route("/plot")
def simplePlot():
        return render_template("simpleplot.html")

@app.route("/top_list")
def top_list():
    try:
        #data = "<br />".join(serial_data)
        data = "<br />".join(toppings_set)
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
            #return redirect(url_for('error'))
    except Exception as e:
        print("Issue retrieving data:")
        print(type(e))
        print(e)
        return redirect(url_for('error'))

app.run(host='0.0.0.0', port=8080, debug=True)
#app.run( debug=True)