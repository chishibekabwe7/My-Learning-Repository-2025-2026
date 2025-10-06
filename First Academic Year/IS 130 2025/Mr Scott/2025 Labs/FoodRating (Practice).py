#!/bin/python3
#Question 1
print("          Today's Menu")
print("1.Pizza")
print("2.Burger")
print("3.Hotdog")
print("4.Sandwich")
print("5.Taco")
print("6.Salad")

#Question 2
print(" ")
option = int(input("Pick any item number from our menu:"))

#Question 3
if option == 4:
    food = "Grilled Chicken Sandwich"
elif option == 5:
    food = "Beef Taco"
elif option == 6 :
    food = "Casesar Salar"
else:
    food = 'Unavailable Item'
print(food)

#Question 4
taste = int(input("Enter Taste Rating:"))
presentation = int(input("Enter Presentation Rating:"))
price = int(input("Enter Price Rating:"))

taste_score = (taste / 100) * 50
presentation_score = (presentation / 100) * 30
price_score = (price / 100) * 20
final_rating = taste_score + presentation_score + price_score
print(f"Your overall food rating is {final_rating}%")


#Question 5
if final_rating > 70:
    print("Excellent Choice")
else:
    Acceptable = (taste_score >= 30 and presentation_score < 15) or (60 <=final_rating <=69 and taste_score > 20)
    if Acceptable:
        print("Satisfactory Meal")
    else:
        print("Poor Choice")






