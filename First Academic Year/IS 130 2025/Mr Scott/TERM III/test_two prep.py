#!/bin/python3
import math
#Problem (3)
        





#Problem (2)
def power(base,exponent):
    return pow(base,exponent)
counter = 0
while counter < 3:
    base = float(input("Enter the base:"))
    exponent = float(input("Enter the value to raise to it to it's power:"))
    anwser = power(base,exponent)
    print(f"The power is {anwser}")
    counter += 1



#Problem (1)
def multiply(a,b):
    return a * b
counter = 0
while counter < 3: 
    a = float(input("Enter a Value:"))
    b = float (input("Enter another value:"))
    result = multiply(a,b)
    print(f"Result is {result}")
    counter += 1
    


    
