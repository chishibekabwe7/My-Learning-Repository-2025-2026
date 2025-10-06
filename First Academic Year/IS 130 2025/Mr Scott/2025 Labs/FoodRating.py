#!/bin/python3
#Question One
print("Today's Menu")
print("1. Pizza")
print("2. Burger")
print("3. Hotdog")
print("4. Sandwich")
print("5. Taco")
print("6. Salad")

#Question Two
option = int(input("Take your pick from today's menu:"))

#Question Three
if option == 4:
    food = "Grilled Chicken"
elif option == 5:
    food = "Beef Taco"
elif option == 6:
    food = "Caeser Salad" 
else:
    food = "An unavailable Item"
print(f"You've Selected {food}")

print(" ")
print("We also need a review of your food experience here, hope you don't mind")

#Question 4
taste = float(input("Enter Taste rating:"))
presentation = float(input("Enter Presentation rating:"))
price = float(input("Enter Price rating:"))

taste_score = taste / 100 * 50
presentation_score = presentation / 100 * 30
price_score = price / 100 * 20
final_rating = taste_score + presentation_score + price_score
print(f"Your overall food rating is {final_rating}")

#Question 5
if final_rating > 70:
    print("Excellent Choice")
    if (taste_score > 30 and presentation_score < 15) or (final_rating >= 60 and final_rating <= 69):
        if taste_score > 20:
            print("Satisfactory Meal")
        else:
            print("Poor Choice")
