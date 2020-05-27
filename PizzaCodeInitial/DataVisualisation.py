# get matplot lib to show a bar graph and a pichart
# load nutritional data from a csv or json file
import pandas as pd

#import matplotlib.pyplot as plt 


#input: Json file of nutritional inputs
#foodlist = [food 1: [salt = 6, cal = 4 etc.], food 2 =...., ham = ...]

#input: CSV files - could use python CSV functions, but pandas handles it nicely
nutrients_data = pd.read_csv("C:\\Users\\catlo\\Desktop\\Dissertation\\PizzaCode\\sample_data.csv")
#nutrients_data = pd.read_csv("data/sample_data.csv") can make path cleaner later #, skiprows = 1 to skip header

#declare variables
#(Energy Kj/)Calories kcal	Fat g	Saturates g	Sugar g	Salt g	Vitamin / Mineral	Allergens
toppings_chosen = ['Ham', 'Extra Cheese', 'YellowPepper']
pizzaSize = 0
calories = fat = saturates = sugar = salt = 0
vits_minerals = []
allergens = []

#get pizza size * slice

for topping in toppings_chosen:
    #find row in nutrients data where lookup equals the topping
    row = nutrients_data[nutrients_data["Lookup"]==topping]
    # print("row")
    # print(row)
    #if row found
    calories += row['Calories'].values[0]
    print("row [calories]")
    print(row['Calories'].values[0])
    #Update the running totals
    #calories += nutrients_data.at(topping, 'Calories')
    # fat += nutrients_data.get_value(topping, 'Fat')
    # saturates += nutrients_data.get_value(topping, 'Saturates')
    # sugar += nutrients_data.get_value(topping, 'Sugar')
    # salt += nutrients_data.get_value(topping, 'Salt')
    # vitamin = nutrients_data.get_value(topping, 'Vitamin or Mineral')
    # allergen = nutrients_data.get_value(topping, 'Allergens')
    # if not vitamin in vits_minerals and vitamin != "" :
    #     vits_minerals.append(vitamin)
    # if not allergen in allergens:
    #     allergens.append(allergen)

#print(nutrients_data.describe)
# print("columns")
# print(nutrients_data.columns)
# print("data types")
# print(nutrients_data.dtypes)
# print("index")
# print(nutrients_data.index)
print("Totals Calculated")
message = (
    f"Calories (kcal): {calories} "	
    f"Fat (g): {fat} "
    f"Saturates (g):{saturates}	"
    f"Sugar (g):{sugar} "
    f"Salt (g):{salt} " 
    f"Vitamins / Minerals {vits_minerals} "
    f"Allergens {allergens}")

print(message)

##add food
#if selected food = ham, find ham in list, get nutritional info, add to running total

##remove food
#if selected food = ham , find ham in list, get nutritional info, remove from running total



# monthList  = df ['month_number'].tolist()
# plt.plot(monthList, profitList, label = 'Month-wise Profit data of last year')
# plt.xlabel('Month number')
# plt.ylabel('Profit in dollar')
# plt.xticks(monthList)
# plt.title('Company profit per month')
# plt.yticks([100000, 200000, 300000, 400000, 500000])
# plt.show()
#could be a better way to manage if bigger lists