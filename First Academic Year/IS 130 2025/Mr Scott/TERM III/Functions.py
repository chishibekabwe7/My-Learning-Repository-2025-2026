#!/bin/python3
def Addition():
    Num1 = int(input("Enter the first Number:"))
    Num2 = int(input("Enter the second Number:"))
    Sum = Num1 + Num2
    print(Sum)
def Difference():
    Num1 = int(input("Enter the first Number:"))
    Num2 = int(input("Enter the second Number:"))
    Diff = Num1 - Num2
    print(Diff)
def Division():
    Num1 = int(input("Enter the first Number:"))
    Num2 = int(input("Enter the second Number:"))
    Div = Num1 / Num2
    print(Div)
def Multiplication():
    Num1 = int(input("Enter the first Number:"))
    Num2 = int(input("Enter the second Number:"))
    Multi = Num1 - Num2
    print(Multi)
print("1. Addition")
print("2. Subtraction")
print("3. Division")
print("4. Multiplication")
Option = int(input("Enter Your Option: "))
if Option == 1:
    Addition()
elif Option == 2:
    Difference()
elif Option ==3:
    Division()
elif Option ==4:
    Multiplication()
else:
    print("Wrong Option")




    