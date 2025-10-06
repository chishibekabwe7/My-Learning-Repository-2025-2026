#!/bin/python3 
#For Loops
# A for loop is a pre-test loop
# Bounded Naturally
# for i in range(stop)
print("For loop Example One")
for k in range(10):
    print(k)
# for i in range (start,stop)
print("For loop Example Two")
for j in range(4,10):
    print(j)
# for i in range (start,stop,step)
print("For loop Example Three")
for i in range(10,20,2):
    print(i)

"""
Lab Assignment
*#*#*
#*#*#
*#*#*
#*#*#
*#*#*
Using Nested loops
ANALYSIS
  purpose:print a 5-row alternating pattern
  inputs:None
  outputs:A lines alternating between # and *
  processes:-Outer loop iterate 5 times(rows)
            -inner loop: alternate * and # based on row parity (even/old)
"""

for row in range(5):
    for col in range(5):
        if (row + col) % 2 == 0:
            print('*',end='')
        else:
            print('#',end='')
    print()



"""
Additional tutorial of end='' keyword/statement.
print() add a newline automatically,while end='' lets you print it in the same line and modify it's endings.
"""
print("end=''", end=" ")
print("Tutorial")


