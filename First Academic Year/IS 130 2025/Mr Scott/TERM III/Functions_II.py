#!/bin/python3
#Python Begginer Friendly Problems (Functions,Arguemnts,Parameters and return values)

#Problem 4
def is_even(num):
    return num % 2 == 0
print(is_even(5))
print(is_even(6))


        

#Problem 3
def check_number(num):
    if num > 0:
        return "Positive"
    elif num < 0:
        return "Negative"
    elif num == 0:
        return "Zero"
print(check_number(8))
        


#Problem 2
#Create a function great with parameter name that takes a name and returns hello + the name argument
def greet(name = "Tobias"):
    print(f"My name is {name}")
greet()
greet("Cj")


#Problem 1
#write a function multiply that takes two parameters a and b and returns their product
def multiply (a,b):
    return a * b
end_product = multiply(3,4)
print(end_product)

def multiply():
    print(3*4)
multiply()



