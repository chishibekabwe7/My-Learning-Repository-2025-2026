#!/bin/python3
#Lab 4 Exercises
#Question Six
"""
Write a program that asks the user to enter two numbers. The program should use the 
conditional operator to determine which number is the smaller and which is the larger.
"""
num1 = int(input("Enter the first Number:"))
num2 = int(input("Enter the second Number:"))
if num1 > num2:
    print(f"{num1} is larger while {num2} is smaller")
else:
    print(f"{num2} is larger while {num1} is smaller")

#Question Five
"""
Write a program that requests for three numbers from the user and determines the 
greatest number.
"""
Num1 = int(input("Enter the first number:"))
Num2 = int(input("Enter the second number:"))
Num3 = int(input("Enter the third number:"))
if Num1 > Num2 and Num1 > Num3:
    print("The first number is the greatest.")
elif Num2 > Num1 and Num2 > Num3:
    print("The Second number is the greatest.")
elif Num3 > Num1 and Num3 > Num2:
    print("The third number is the greatest.")
else:
    print("The Conditions where met.")





#Question Four
"""
Given that the log in credentials of a system are username = user and password = 
program. Write a program that prints the message “Access Granted” when the user 
enters the correct credentials and “Access Denied” when the credentials are incorrect.
"""
username = "chishibekabwe7"
password = "test4321"
username_input = input("Enter your username:")
password_input = input("Enter your password:")
if username_input == username:
    print("Access Granted!")
elif password_input == password:
    print("Access Denied!")

#Question Three
"""
Write a program that requests for the mark of a student. The program should display 
the message “You have passed” when the mark entered is 50 and above and “You 
have failed” when the mark is below 50.
"""
mark = int(input("Enter Your Test Score:"))
if mark > 50:
    print("You have passed")
else:
    print("You have failed")



#Question Two
"""
Write a program that requests for a number from the user. The program should check 
if the number is negative, positive or zero and display an appropriate message.
"""
Value = int(input("Enter a Number:"))
if Value < 0:
    print(f"{Value} is a negative number")
elif Value >= 1:
    print(f"{Value} is a positive number")
else:
    print(f"{Value} is equivalent to 0")



#Question One
"""
Write a program that prints the message “The number is valid” if the variable grade is 
within the range 0 through 100.
"""
grade = 0
grade = int(input("Enter a Number:"))
if grade <= 100 and grade >= 0:
    print("The number is valid")

































































# #if...elif...else Example
# """
# Write a program that asks for a studen't exam score (0-100) and prints their grade
# """
# Test_Score = 0
# Test_Score = int(input("Enter Your Test Score:"))
# if Test_Score >= 90:
#     print("Your grade is an A")
# elif Test_Score >= 80:
#     print("Your grade is a B")
# elif Test_Score >= 70:
#     print("Your grade is a C")
# elif Test_Score >= 60:
#     print("Your grade is a D")
# elif Test_Score <60:
#     print("Your grade is an F")



# #if...else statement Example
# """
# Write a program that takes a number as input. If the number is even,
# print 'Even'.Otherwise, print 'Odd'.
# """
# Num = 0
# Num = int(input("Enter Value:"))
# if Num % 2 == 0:
#     print(f"{Num} is an even number")
# else:
#     print(f"{Num} is an odd number")

# #if statement Example
# """
# Write a program that asks the user for their age.
# If the age is 18 or older, print 'You are eligible to vote'.
# """
# age = 0
# age = int(input("Enter your age:"))
# if age > 18:
#     print("You are eligible to vote")








