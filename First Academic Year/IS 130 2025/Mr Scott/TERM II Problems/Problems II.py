#!/bin/python3
import math
#The BMI Calculator
"""
The BMI has 4 categories, the student can go into the following:
1. if the bmi is less than 18.5 a student is underweight.
2. if the bmi is between 18.5 and 24.9 then your weight is normal.
3. if a student BMI is between 25 and 29.5 then your overweight.
4. if a students BMI is greater than 30 then a student is obesse.
The Program should also recieve the full name,SIN and gender as a single character as inputs.
"""

weight = float(input("Enter weight:"))
height = float(input("Enter height:"))
Full_Name = str(input("Enter Your Name:"))
SIN = int(input("Enter your Student ID:"))
Gender = str(input("Enter your gender (M/F):"))
BMI = weight/pow(height,2.0)
print(f"Your Body Mass index is {BMI}")

if   BMI < 18.5:
    print("Category: Underweight")
elif BMI >= 18.5 and BMI <= 24.9:
    print("Category: Normal")
elif BMI >= 25 and BMI <= 29.9:
    print("Category: Overweight.")
else:
    print("Category: Obesse")

#Next Week Assignment
"""
CBU clinic asks  you to write a program that will be used to display the Waist-to-Height Ratio (WHtR) of a student.
The student will be asked to provide their waist circumference and height as inputs. The program will then calculate the
WHtR as: waist circumference divided by the height. If the WHtR is less than or equal to 0.5 output "Normal" else output "Abnormal"

1.Analysis
Purpose:This Program will display the Waist-to-Height Ratio (WHtR) of a studend.
inputs:Waist Circumference,height
outputs:Normal,Abnormal
Processes: -Waist Circumference /height

2.Psudo Code
L1:Start
L2:Waist Circumference as Real
L3:height as Real
L4:WHtR as Real
L5:output"Enter your Waist Circumference"
L6:input Waist Circumference
L7:output"Enter your height"
L8:input height
L9:WHtR = Waist Cirmumference /height
L10:     output WHtR
L11:End
"""
#3.Python Code
waist_circumference = int(input("Enter your waist circumference:"))
height = int(input("Enter your height:"))
WHtR = waist_circumference/height
print(WHtR)





