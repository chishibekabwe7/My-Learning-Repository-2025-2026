#!/bin/python3
# Conditional Ternary operator tutorial
# mark = int(input("Enter your marks:"))
# comment = "passed" if mark >=50 else comment = "failed"
# print(comment)

#Lab 5 Exercise
#Question Four
"""
Write a program that allows the user to enter a wrong username or password 3 times 
only. When this limit is exceeded, the program should display a message System 
Locked. However, if the user enters the correct username and password before the 
limit, the program should display the message Access Granted. Note that the 
condition should check if the entered username and password equals ‘user’ and 
‘python’ respectively.
"""
correct_username = '24248580'
correct_password = 'test1234'

#initialize counter in the variable name attempts
attempts = 3
while attempts > 0:
    username = input("Enter Username:")
    password = input("Enter Password:")
    #Check credentials
    if username == correct_username and password == correct_password:
        print("Access Granted")
        break
    else:
        attempts -= 1
        if attempts > 0:
            print(f"Invalid Credentials. {attempts} attempts remaining.")
        else:
            print("System Locked")

correct_username = '24148580'
correct_password = 'test1234'
attempts = 3
while attempts > 0:
    username = input("Enter Username:")
    password = input("Enter your password:")
    if (correct_username == username) and (correct_password == password):
        print("Access Granted!")
        break
    else:
        attempts -= 1
        if attempts > 0:
            print(f"Please Try again {attempts} remaining.")
        else:
            print("Locked!")



    
    



#Question Three
"""
Write a program that asks the user to enter how many numbers they have. The 
program should then request for these numbers using a loop, calculate the 
total/average and print the results.
Analysis (How it works)
1. It asks how many numbers you have
2. Then uses a for loop to ask for each number one by one.
3. It keeps adding them up.
4. Then it calculates the average and prints both the total and average.
"""
Number = int(input("How many number do you have?:"))
Total = 0
for i in range(Number):
    Total += int(input("Enter a Number?"))
    Average = Total/Number
print(f"The Total is {Total}")
print(f"The Average is {Average}")
#Question Two
"""
Write a program that requests for your name and the number of times you want the 
name printed. The program should print your name on separate lines equivalent to the 
number you specified.
"""
name = input("Enter your Full Names:")
times = int(input("Enter the number of times you want your name to displayed:"))
for i in range(0,times,1):
    print(name)


#Question One
"""
Write a for loop that displays the following set of numbers: 0, 10, 20, 30, 40, 50 …
1000.
"""
# for loop in range(0, 1010,10):
#     print(loop)


