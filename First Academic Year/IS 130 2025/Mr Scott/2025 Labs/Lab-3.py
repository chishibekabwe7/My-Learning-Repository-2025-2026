#!/bin/python3

#Ways of incrementing values
x = 5
y = 5
print(f"The value of x is {x}")
print(f"The value of y is {y}")
x+=3 #Compound assignment
y=y + 6
#After Increment
print(f"The value of x after the increment is {x}")
print(f"The value of y after the increment is {y}")

#Calculating the Average
num1 = 0
num2 = 0
average = 0
print("This program calculates and displays the average of two numbers")
num1 = float(input("Enter the value of the first Number:"))
num2 = float(input("Enter the value of the Second Number:"))
average = (num1 + num2)/2
print(f"The Average of {num1} and {num2} is {average}")

#Area of a circle calculation
PI = 3.142
area = 0
radius = 0
print("This Program calculates the area of a circle!")
radius = float(input("Enter the Radius:"))
area = PI * pow(radius,2)
print(f"The Area of the is circle {area}")










