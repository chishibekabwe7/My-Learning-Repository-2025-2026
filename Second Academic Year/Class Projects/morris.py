#!/usr/bin/env python3
students = {}

while True:
    print("\n===== Student Management System =====")
    print("1. Add Student")
    print("2. Remove Student")
    print("3. Add Grade")
    print("4. Generate Report")
    print("5. Exit")

    choice = input("Enter choice: ")

    if choice == "1":
        student_id = input("Enter Student ID: ")

        if student_id in students:
            print("Student already exists!")
        else:
            name = input("Enter Student Name: ")
            students[student_id] = {"name": name, "grades": {}}
            print("Student added successfully!")

    elif choice == "2":
        student_id = input("Enter Student ID to remove: ")

        if student_id in students:
            del students[student_id]
            print("Student removed successfully!")
        else:
            print("Student not found!")

    elif choice == "3":
        student_id = input("Enter Student ID: ")

        if student_id not in students:
            print("Student not found!")
        else:
            subject = input("Enter Subject: ")

            try:
                score = float(input("Enter Score (0-100): "))

                if 0 <= score <= 100:
                    students[student_id]["grades"][subject] = score
                    print("Grade added successfully!")
                else:
                    print("Invalid score!")

            except ValueError:
                print("Invalid input!")

    elif choice == "4":
        student_id = input("Enter Student ID: ")

        if student_id not in students:
            print("Student not found!")
        else:
            print("\nStudent ID:", student_id)
            print("Name:", students[student_id]["name"])

            grades = students[student_id]["grades"]

            if len(grades) == 0:
                print("No grades recorded")
                print("GPA: 0")
            else:
                total = sum(grades.values())
                average = total / len(grades)
                gpa = round(average / 25, 2)

                for subject in grades:
                    print(subject, ":", grades[subject])

                print("GPA:", gpa)

    elif choice == "5":
        print("Goodbye!")
        break

    else:
        print("Invalid choice!")