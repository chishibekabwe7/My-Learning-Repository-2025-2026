#!/usr/bin/env python3
import json
import os


DATA_FILE = os.path.join(os.path.dirname(__file__), "bank_accounts.json")


# Represents a basic bank account with balance and transaction history.
class Account:
	# Creates a new account object.
	# Inputs: name (str), account_number (str), balance (float), transactions (list or None)
	# Output: None
	def __init__(self, name, account_number, balance=0.0, transactions=None):
		self.name = name
		self.account_number = account_number
		self.balance = float(balance)
		self.transactions = transactions if transactions is not None else []

	# Adds money to the account.
	# Inputs: amount (float)
	# Output: tuple(bool, str) -> success status and message
	def deposit(self, amount):
		if amount <= 0:
			return False, "Deposit amount must be greater than zero."

		self.balance += amount
		self.transactions.append(f"Deposit: +{amount:.2f}")
		return True, f"Deposit successful. New balance: {self.balance:.2f}"

	# Removes money from the account if enough balance exists.
	# Inputs: amount (float)
	# Output: tuple(bool, str) -> success status and message
	def withdraw(self, amount):
		if amount <= 0:
			return False, "Withdrawal amount must be greater than zero."

		if amount > self.balance:
			return False, "Insufficient balance."

		self.balance -= amount
		self.transactions.append(f"Withdraw: -{amount:.2f}")
		return True, f"Withdrawal successful. New balance: {self.balance:.2f}"

	# Returns interest amount for this account type.
	# Inputs: None
	# Output: float
	def calculate_interest(self):
		return 0.0

	# Returns account transactions.
	# Inputs: None
	# Output: list[str]
	def view_transactions(self):
		return self.transactions

	# Returns account details in dictionary form for display.
	# Inputs: None
	# Output: dict
	def display_account_details(self):
		return {
			"type": "Account",
			"name": self.name,
			"account_number": self.account_number,
			"balance": self.balance,
		}

	# Converts account object into dictionary format for JSON storage.
	# Inputs: None
	# Output: dict
	def to_dict(self):
		return {
			"type": "Account",
			"name": self.name,
			"account_number": self.account_number,
			"balance": self.balance,
			"transactions": self.transactions,
		}


# Represents a savings account with a simple fixed interest rate.
class SavingsAccount(Account):
	# Creates a new savings account object.
	# Inputs: name (str), account_number (str), balance (float), interest_rate (float), transactions (list or None)
	# Output: None
	def __init__(self, name, account_number, balance=0.0, interest_rate=0.03, transactions=None):
		super().__init__(name, account_number, balance, transactions)
		self.interest_rate = float(interest_rate)

	# Calculates interest for savings account.
	# Inputs: None
	# Output: float
	def calculate_interest(self):
		return self.balance * self.interest_rate

	# Returns savings account details in dictionary form for display.
	# Inputs: None
	# Output: dict
	def display_account_details(self):
		details = super().display_account_details()
		details["type"] = "SavingsAccount"
		details["interest_rate"] = self.interest_rate
		details["estimated_interest"] = self.calculate_interest()
		return details

	# Converts savings account object into dictionary format for JSON storage.
	# Inputs: None
	# Output: dict
	def to_dict(self):
		data = super().to_dict()
		data["type"] = "SavingsAccount"
		data["interest_rate"] = self.interest_rate
		return data


# Manages all account operations, including loading and saving data.
class Bank:
	# Creates a new bank object.
	# Inputs: None
	# Output: None
	def __init__(self):
		self.accounts = {}

	# Creates and stores a new account if input is valid.
	# Inputs: name (str), account_number (str), balance (float), account_type (str)
	# Output: tuple(bool, str) -> success status and message
	def create_account(self, name, account_number, balance, account_type="normal"):
		if not name.strip():
			return False, "Name cannot be empty."

		if not account_number.isdigit() or len(account_number) < 4:
			return False, "Account number must be numeric and at least 4 digits."

		if account_number in self.accounts:
			return False, "Account number already exists."

		if balance < 0:
			return False, "Starting balance cannot be negative."

		if account_type == "savings":
			account = SavingsAccount(name, account_number, balance)
		else:
			account = Account(name, account_number, balance)

		account.transactions.append(f"Account created with balance {balance:.2f}")
		self.accounts[account_number] = account
		self.save_data(DATA_FILE)
		return True, "Account created successfully."

	# Deposits money into a selected account.
	# Inputs: account_number (str), amount (float)
	# Output: tuple(bool, str) -> success status and message
	def deposit(self, account_number, amount):
		account = self.accounts.get(account_number)
		if account is None:
			return False, "Account not found."

		success, message = account.deposit(amount)
		if success:
			self.save_data(DATA_FILE)
		return success, message

	# Withdraws money from a selected account.
	# Inputs: account_number (str), amount (float)
	# Output: tuple(bool, str) -> success status and message
	def withdraw(self, account_number, amount):
		account = self.accounts.get(account_number)
		if account is None:
			return False, "Account not found."

		success, message = account.withdraw(amount)
		if success:
			self.save_data(DATA_FILE)
		return success, message

	# Returns an account object by account number.
	# Inputs: account_number (str)
	# Output: Account or None
	def get_account(self, account_number):
		return self.accounts.get(account_number)

	# Loads accounts from JSON file when the program starts.
	# Inputs: filename (str)
	# Output: None
	def load_data(self, filename):
		if not os.path.exists(filename):
			return

		try:
			with open(filename, "r", encoding="utf-8") as file:
				data = json.load(file)

			self.accounts = {}
			for item in data:
				account_type = item.get("type", "Account")
				if account_type == "SavingsAccount":
					account = SavingsAccount(
						item.get("name", ""),
						item.get("account_number", ""),
						item.get("balance", 0.0),
						item.get("interest_rate", 0.03),
						item.get("transactions", []),
					)
				else:
					account = Account(
						item.get("name", ""),
						item.get("account_number", ""),
						item.get("balance", 0.0),
						item.get("transactions", []),
					)

				self.accounts[account.account_number] = account
		except (json.JSONDecodeError, OSError):
			print("Could not load account data. Starting with empty bank records.")
			self.accounts = {}

	# Saves all accounts into JSON file after updates.
	# Inputs: filename (str)
	# Output: None
	def save_data(self, filename):
		try:
			with open(filename, "w", encoding="utf-8") as file:
				json.dump([account.to_dict() for account in self.accounts.values()], file, indent=4)
		except OSError:
			print("Could not save account data.")


# Creates an account using user input.
# Inputs: bank (Bank)
# Output: None
def create_account(bank):
	name = input("Enter account holder name: ").strip()
	account_number = input("Enter account number: ").strip()

	try:
		balance = float(input("Enter starting balance: ").strip())
	except ValueError:
		print("Invalid amount. Please enter a number.")
		return

	account_choice = input("Account type (1 = Normal, 2 = Savings): ").strip()
	account_type = "savings" if account_choice == "2" else "normal"

	success, message = bank.create_account(name, account_number, balance, account_type)
	print(message)


# Deposits money using user input.
# Inputs: bank (Bank)
# Output: None
def deposit(bank):
	account_number = input("Enter account number: ").strip()

	try:
		amount = float(input("Enter deposit amount: ").strip())
	except ValueError:
		print("Invalid amount. Please enter a number.")
		return

	success, message = bank.deposit(account_number, amount)
	print(message)


# Withdraws money using user input.
# Inputs: bank (Bank)
# Output: None
def withdraw(bank):
	account_number = input("Enter account number: ").strip()

	try:
		amount = float(input("Enter withdrawal amount: ").strip())
	except ValueError:
		print("Invalid amount. Please enter a number.")
		return

	success, message = bank.withdraw(account_number, amount)
	print(message)


# Shows account balance and transaction history.
# Inputs: bank (Bank)
# Output: None
def show_balance(bank):
	account_number = input("Enter account number: ").strip()
	account = bank.get_account(account_number)

	if account is None:
		print("Account not found.")
		return

	details = account.display_account_details()
	print("\nAccount Details")
	print(f"Name: {details['name']}")
	print(f"Account Number: {details['account_number']}")
	print(f"Account Type: {details['type']}")
	print(f"Balance: {details['balance']:.2f}")

	if details["type"] == "SavingsAccount":
		print(f"Interest Rate: {details['interest_rate'] * 100:.2f}%")
		print(f"Estimated Interest: {details['estimated_interest']:.2f}")

	print("Transactions:")
	if not account.view_transactions():
		print("- No transactions yet.")
	else:
		for item in account.view_transactions():
			print(f"- {item}")


# Displays the main menu choices.
# Inputs: None
# Output: None
def show_menu():
	print("\nBanking System Menu")
	print("1. Create account")
	print("2. Deposit")
	print("3. Withdraw")
	print("4. Check balance")
	print("5. Exit")


# Runs the menu loop until the user exits.
# Inputs: None
# Output: None
def main():
	bank = Bank()
	bank.load_data(DATA_FILE)

	while True:
		show_menu()
		choice = input("Choose an option (1-5): ").strip()

		if choice == "1":
			create_account(bank)
		elif choice == "2":
			deposit(bank)
		elif choice == "3":
			withdraw(bank)
		elif choice == "4":
			show_balance(bank)
		elif choice == "5":
			bank.save_data(DATA_FILE)
			print("Exiting program. Goodbye!")
			break
		else:
			print("Invalid option. Please choose a number from 1 to 5.")


# Starts the program when this file is run directly.
if __name__ == "__main__":
	main()
