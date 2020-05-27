# get matplot lib to show a bar graph and a pichart
# load nutritional data from a csv or json file
import pandas as pd
import matplotlib.pyplot as plot 


#input: Json file of nutritional inputs
#foodlist = [food 1: [salt = 6, cal = 4 etc.], food 2 =...., ham = ...]

#input: CSV files - could use python CSV functions, but pandas handles it nicely
filepath = "C:\\Users\\catlo\\Desktop\\Dissertation\\PizzaCode\\sample_data.csv"
nutrients_data = pd.read_csv(filepath, keep_default_na = False)
#nutrients_data = pd.read_csv("data/sample_data.csv") can make path cleaner later #, skiprows = 1 to skip header

#declare variables
#(Energy Kj/)Calories kcal	Fat g	Saturates g	Sugar g	Salt g	Vitamin / Mineral	Allergens
toppings_chosen = ['Ham', 'Extra Cheese', 'YellowPepper']
pizzaSize = 4
calories = fat = saturates = sugar = salt = 0
vits_minerals = set()
allergens = set()

#get pizza size * slice
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

for topping in toppings_chosen:
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

#print(nutrients_data.describe)
# print("columns")
# print(nutrients_data.columns)
# print("data types")
# print(nutrients_data.dtypes)
# print("index")
# print(nutrients_data.index)

#print summary
print("Totals Calculated")
message = (
    f"Calories (kcal): {calories} "	
    f"Fat (g): {fat} " 
    f"Saturates (g):{saturates} "
    f"Sugar (g):{sugar} "
    f"Salt (g):{salt} "
    f"Vitamins / Minerals {vits_minerals} "
    f"Allergens {allergens}")

print(message)

#Make the results into a new data frame
summary = {'Calories': [calories],
'Fat': [fat], 'Saturates': [saturates],
'Sugar': [sugar], 'Salt': [salt]
}
df = pd.DataFrame(summary, columns = ['Calories', 'Fat', 'Saturates', 'Sugar', 'Salt' ])
print(df)
##add food
#if selected food = ham, find ham in list, get nutritional info, add to running total

##remove food
#if selected food = ham , find ham in list, get nutritional info, remove from running total

# Draw a vertical bar chart


df.plot.bar( title="Summary of Nutritional Information")
plot.xlabel('Nutritional Information')
plot.ylabel('Calories / grams of each nutrient, per 100g of food')

plot.show(block=True)

# plt.plot(monthList, profitList, label = 'Month-wise Profit data of last year')
# plt.xlabel('Month number')
# plt.ylabel('Profit in dollar')
# plt.xticks(monthList)
# plt.title('Company profit per month')
# plt.yticks([100000, 200000, 300000, 400000, 500000])
# plt.show()
#could be a better way to manage if bigger lists