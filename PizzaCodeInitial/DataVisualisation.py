# get matplot lib to show a bar graph and a pichart
# load nutritional data from a csv or json file
import pandas as pd
import matplotlib.pyplot as plt 

#input: Json file of nutritional inputs
#foodlist = [food 1: [salt = 6, cal = 4 etc.], food 2 =...., ham = ...]

#input: CSV files
data = pd.read_csv("C:\\Users\\catlo\\Desktop\\Dissertation\\sample_data.csv")
nutrientsList = df ['nutrients'].tolist() #eg fat, protein, etc
#or should nutrients be 3,5,6,   ,23,45 with each being the nutrients so the below isnt 2d?
foodsList  = df ['food'].tolist() #2d array of tomato: 3g,5g, etc. , ham: 3g, 12g, etc


#initi
for food in foodsList:
    
else:
    pass

##add food
#if selected food = ham, find ham in list, get nutritional info, add to running total

##remove food
#if selected food = ham , find ham in list, get nutritional info, remove from running total

##print output
#on print
 

monthList  = df ['month_number'].tolist()
plt.plot(monthList, profitList, label = 'Month-wise Profit data of last year')
plt.xlabel('Month number')
plt.ylabel('Profit in dollar')
plt.xticks(monthList)
plt.title('Company profit per month')
plt.yticks([100000, 200000, 300000, 400000, 500000])
plt.show()
#could be a better way to manage if bigger lists