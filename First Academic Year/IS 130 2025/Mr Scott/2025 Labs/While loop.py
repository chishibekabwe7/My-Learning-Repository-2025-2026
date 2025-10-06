#!/bin/python3



# Program 2
number = int(input("Enter a Number between 1 and 10:"))
while number < 1 or number > 10:
    print("The value of number must be between 1 and 10")
    number = int(input("Enter a number between 1 and 10:"))
if number %2 == 0:
    print("Even")
else:
    print("Odd")
    


# Programe 1: General While loop
count = 10
while count > 0:
    print(count)
    count = count - 1
if count == 0:
    print("Happy New year")


