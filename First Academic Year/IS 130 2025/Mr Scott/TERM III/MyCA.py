#!/bin/python3
#Program 2
def calculateTestCA(testScore,testTotal):
    testCA = (testScore/testTotal)* 20
    return testCA
def calculateTotalCA():
    testCount = 1
    totalCA = 0
    while testCount <= 2:
        testScore = float(input("Enter Test Score: "))
        testTotal = float(input("Enter Test Total: "))
        testCA = calculateTestCA(testScore,testTotal)
        totalCA = totalCA + testCA 
        testCount += 1
    return totalCA
    
grade = input("Enter Course Grade: ")
match(grade):
    case "A+": gradeScore = 86
    case "A":  gradeScore = 76
    case "B+":  gradeScore = 68
    case "B":  gradeScore = 62
    case "C+":  gradeScore = 56
    case "C":  gradeScore = 50
    case "D+":  gradeScore = 40
    case "D":  gradeScore = 0
    case _: gradeScore = "Unknown Grade"

caScore = calculateTotalCA()
examScore = gradeScore - caScore
print(f"Your CA is: {caScore} and minimum Exam Score is: {examScore}") 

#Assignment
#how do I get the CA and grade score for 6 different courses
"""
Course: IS130
Test 1: 32/60
Test 2: 30/60
CA : 22
Exam: 60
Grade: A
"""




# #Program 1
# testScore = float(input("Enter Test Score: "))
# testTotal = float(input("Enter Test Score: "))
# testCA = (testScore/testTotal) * 20
# print(testCA)
