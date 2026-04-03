#!/usr/bin/env python3
import json
import os


DATA_FILE = os.path.join(os.path.dirname(__file__), "library_books.json")


class Book:
	# Creates a book object with title, author, ID, and availability status.
	def __init__(self, title, author, book_id, available=True):
		self.title = title
		self.author = author
		self.book_id = book_id
		self.available = available

	# Converts a book object into a dictionary for JSON saving.
	def to_dict(self):
		return {
			"title": self.title,
			"author": self.author,
			"book_id": self.book_id,
			"available": self.available,
		}

	# Creates a book object from dictionary data loaded from JSON.
	@staticmethod
	def from_dict(data):
		return Book(
			data.get("title", ""),
			data.get("author", ""),
			data.get("book_id", ""),
			data.get("available", True),
		)


class Library:
	# Creates a library with book and member storage.
	def __init__(self):
		self.books = []
		self.members = {}

	# Loads books from a JSON file when the program starts.
	def load_books(self, filename):
		if not os.path.exists(filename):
			return

		try:
			with open(filename, "r", encoding="utf-8") as file:
				data = json.load(file)
				self.books = [Book.from_dict(item) for item in data]
		except (json.JSONDecodeError, OSError):
			print("Could not load books file. Starting with an empty library.")
			self.books = []

	# Saves all books to a JSON file.
	def save_books(self, filename):
		try:
			with open(filename, "w", encoding="utf-8") as file:
				json.dump([book.to_dict() for book in self.books], file, indent=4)
		except OSError:
			print("Could not save books file.")

	# Adds a new book if the book ID is not already used.
	def add_book_to_library(self, book):
		if self.get_book_by_id(book.book_id) is not None:
			return False

		self.books.append(book)
		return True

	# Registers a member if the member ID is not already used.
	def register_member(self, name, member_id):
		if member_id in self.members:
			return False

		self.members[member_id] = name
		return True

	# Finds a single book by its ID.
	def get_book_by_id(self, book_id):
		for book in self.books:
			if book.book_id == book_id:
				return book
		return None

	# Finds books whose titles contain the search text.
	def search_by_title(self, title):
		matches = []
		search_text = title.lower().strip()

		for book in self.books:
			if search_text in book.title.lower():
				matches.append(book)

		return matches


# Adds a new book using user input.
def add_book(library):
	title = input("Enter book title: ").strip()
	author = input("Enter book author: ").strip()
	book_id = input("Enter book ID: ").strip()

	if not title or not author or not book_id:
		print("All fields are required.")
		return

	new_book = Book(title, author, book_id, True)
	if library.add_book_to_library(new_book):
		library.save_books(DATA_FILE)
		print("Book added successfully.")
	else:
		print("A book with that ID already exists.")


# Registers a new library member using user input.
def register_member(library):
	name = input("Enter member name: ").strip()
	member_id = input("Enter member ID: ").strip()

	if not name or not member_id:
		print("Name and member ID are required.")
		return

	if library.register_member(name, member_id):
		print("Member registered successfully.")
	else:
		print("A member with that ID already exists.")


# Borrows a book by marking it as unavailable.
def borrow_book(library):
	member_id = input("Enter your member ID: ").strip()
	if member_id not in library.members:
		print("Member not found. Please register first.")
		return

	book_id = input("Enter book ID to borrow: ").strip()
	book = library.get_book_by_id(book_id)

	if book is None:
		print("Book not found.")
		return

	if not book.available:
		print("Book is currently unavailable.")
		return

	book.available = False
	library.save_books(DATA_FILE)
	print("Book borrowed successfully.")


# Returns a borrowed book by marking it as available.
def return_book(library):
	book_id = input("Enter book ID to return: ").strip()
	book = library.get_book_by_id(book_id)

	if book is None:
		print("Book not found.")
		return

	if book.available:
		print("This book is already marked as available.")
		return

	book.available = True
	library.save_books(DATA_FILE)
	print("Book returned successfully.")


# Searches for books by title and shows matching results.
def search_book(library):
	title = input("Enter title to search: ").strip()

	if not title:
		print("Please enter a title.")
		return

	matches = library.search_by_title(title)
	if not matches:
		print("No books found with that title.")
		return

	print("\nMatching books:")
	for book in matches:
		status = "Available" if book.available else "Unavailable"
		print(f"- {book.title} by {book.author} | ID: {book.book_id} | {status}")


# Displays the main menu options.
def show_menu():
	print("\nLibrary Management System")
	print("1. Add book")
	print("2. Register member")
	print("3. Borrow book")
	print("4. Return book")
	print("5. Search book by title")
	print("0. Exit")


# Runs the main menu loop until the user chooses to exit.
def main():
	library = Library()
	library.load_books(DATA_FILE)

	while True:
		show_menu()
		choice = input("Choose an option (0-5): ").strip()

		if choice == "1":
			add_book(library)
		elif choice == "2":
			register_member(library)
		elif choice == "3":
			borrow_book(library)
		elif choice == "4":
			return_book(library)
		elif choice == "5":
			search_book(library)
		elif choice == "0":
			library.save_books(DATA_FILE)
			print("Exiting program. Goodbye!")
			break
		else:
			print("Invalid option. Please choose from 0 to 5.")


# Starts the program when this file is run directly.
if __name__ == "__main__":
	main()





