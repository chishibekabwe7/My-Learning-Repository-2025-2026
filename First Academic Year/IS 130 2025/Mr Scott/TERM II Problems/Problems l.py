#!/bin/python3

#Problem 3
import math
#Write a program that will recieve a student's weight and height as inputs.
#You are to use these inputs to calcucalte a students body mass index(BMI).
Weight = float(input("Enter Weight:"))
height = float(input("Enter height:"))
BMI = Weight/math.sqrt(height)
print(f"Your body mass index is {BMI}")
if BMI < 18.5:
    print("And you are very underweight and malnourished")
else:
    print("And you are very nourished and possibly obesse")

#Problem 2
#Literals Exercise (WYSIWYG)
print("   *   ")
print("  ***  ")
print(" ***** ")
print("*******")

#Problem 1
# 1.Write a Progra that will simplify display what type of programmer you want to be
print("Full Stack Developer")

