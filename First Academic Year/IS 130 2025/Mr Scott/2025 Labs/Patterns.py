#!/bin/python3
#Pattern I
"""
* # * # * 
# * # * # 
* # * # * 
# * # * # 
* # * # * 
"""
for col in range(5):
    for rol in range(5):
        if (col + rol) % 2 == 0:
            print('*',end=" ")
        else:
            print('#',end=" ")
    print() #moves to the next line after each for


#Pattern II

size = 5
#Top Half
for i in range(size):
    print(' ' * (size - i - 1), end='')
    print('*' * (2*i+1))
#Bottom Half
for i in range(size -2, -1, -1):
    print(' '* (size -i-1),end='')
    print('*'*(2*i+1))
