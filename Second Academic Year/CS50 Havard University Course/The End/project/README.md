# Task & Time Tracker
#### Video Demo: https://drive.google.com/file/d/1ymtSU5GvD3G6iHL2_2zUGEK56kpRZrDa/view?usp=sharing 
#### Description:

Task & Time Tracker is a web-based productivity application built using Python and Flask. The purpose of this project is to allow users to create tasks, log the number of hours spent on each task, and track their overall productivity. The application demonstrates backend development, database design, and frontend templating using technologies learned throughout CS50x.

This project was developed as my final project for CS50x. It integrates Flask routing, SQLite database management, HTML templating with Jinja, and basic CSS styling. The goal of this application is to provide a simple but functional task tracking system that showcases my understanding of full-stack web development concepts introduced in the course.

When the application is executed using `python app.py`, Flask initializes the server and connects to a SQLite database file named `tasks.db`. Users access the application through their web browser at `localhost:5000`. The homepage displays all stored tasks and the total number of hours logged. Users can add new tasks through a form and delete existing tasks directly from the homepage.

The application implements basic CRUD functionality:
- Create: Add new tasks with a title and number of hours.
- Read: Display all stored tasks.
- Delete: Remove tasks from the database.

The database consists of a single table named `tasks` with the following schema:

- id (INTEGER PRIMARY KEY AUTOINCREMENT)
- title (TEXT NOT NULL)
- hours (REAL NOT NULL)

The `id` uniquely identifies each task. The `title` stores the description of the task, while `hours` stores the amount of time spent working on that task. I chose to use SQLite because it is lightweight, easy to integrate with Flask, and appropriate for small-scale applications.

The project directory is structured as follows:

- app.py: Contains all Flask routes, database connection logic, and server configuration.
- templates/layout.html: Base template that provides consistent structure and layout across all pages.
- templates/index.html: Displays the list of tasks and total hours worked.
- templates/add.html: Contains the form for adding new tasks.
- static/style.css: Provides styling for the user interface.
- tasks.db: SQLite database file.

One important design decision I made was to keep the application simple and focused on core functionality. Instead of implementing authentication or user accounts, I prioritized stability and clarity. I wanted to ensure that the core task management features worked reliably before considering additional complexity.

Another design choice was to calculate the total number of hours dynamically by summing values from the database each time the homepage loads. This prevents inconsistencies that could arise if totals were stored separately.

During development, I encountered several challenges. One challenge involved debugging template loading errors in Flask. I resolved this by ensuring that all HTML files were correctly placed inside the `templates` directory and that file names matched exactly, as Flask is case-sensitive. Another challenge was validating user input to prevent empty tasks or invalid hour values from being inserted into the database.

If I had more time to expand this project, I would consider adding the following features:

- Task editing functionality
- Task categories or tags
- User authentication
- Graphical productivity statistics
- Deployment to a public hosting service

Overall, this project represents my understanding of web development concepts learned in CS50x, including backend programming, database management, and templating. It reflects my ability to design, implement, debug, and document a complete web application from scratch.
