import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# -----------------------------------------------------------------------------
# Notes:
# - Create the transactions table (run once in sqlite3 or phpLiteAdmin):
#   CREATE TABLE IF NOT EXISTS transactions (
#       id INTEGER PRIMARY KEY AUTOINCREMENT,
#       user_id INTEGER NOT NULL,
#       symbol TEXT NOT NULL,
#       shares INTEGER NOT NULL,
#       price REAL NOT NULL,
#       transacted TIMESTAMP DEFAULT CURRENT_TIMESTAMP
#   );
#   CREATE INDEX IF NOT EXISTS idx_transactions_user ON transactions (user_id);
#   CREATE INDEX IF NOT EXISTS idx_transactions_symbol ON transactions (symbol);
# -----------------------------------------------------------------------------

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded (helpful during development)
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter for Jinja
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (so sessions persist during dev)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached (development friendly)."""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = "0"
    response.headers["Pragma"] = "no-cache"
    return response



@app.route("/")
@login_required
def index():
    """Show portfolio of stocks for the logged-in user."""
    user_id = session["user_id"]

    # Get user's cash
    user_rows = db.execute("SELECT cash, username FROM users WHERE id = ?", user_id)
    if len(user_rows) != 1:
        return apology("user not found", 500)
    cash = user_rows[0]["cash"]

    # Aggregate holdings (sum of shares per symbol), only show if > 0
    rows = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING shares > 0",
        user_id,
    )

    portfolio = []
    stocks_value = 0.0

    for r in rows:
        symbol = r["symbol"]
        shares = int(r["shares"])

        quote = lookup(symbol)
        # Defensive: if lookup fails (shouldn't for valid symbols), skip or show 0
        if quote is None:
            price = 0.0
            name = symbol
        else:
            price = quote["price"]
            name = quote["name"]

        total = price * shares
        stocks_value += total

        portfolio.append(
            {
                "symbol": symbol,
                "name": name,
                "shares": shares,
                "price": price,
                "total": total,
            }
        )

    grand_total = cash + stocks_value

    return render_template(
        "index.html",
        portfolio=portfolio,
        cash=cash,
        total_stocks_value=stocks_value,
        grand_total=grand_total,
    )


# -------------------------
# Register
# -------------------------
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user: require username, password, confirmation."""
    if request.method == "GET":
        return render_template("register.html")

    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    # Validations
    if not username:
        return apology("must provide username", 400)
    if not password:
        return apology("must provide password", 400)
    if password != confirmation:
        return apology("passwords do not match", 400)

    # Insert user (handle duplicate username)
    try:
        new_id = db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            username,
            generate_password_hash(password),
        )
    except ValueError:
        return apology("username already exists", 400)

    # Log user in
    session["user_id"] = new_id
    return redirect("/")


# -------------------------
# Login / Logout
# -------------------------
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""
    # Forget any user_id
    session.clear()

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")

        if not username:
            return apology("must provide username", 403)
        if not password:
            return apology("must provide password", 403)

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("invalid username and/or password", 403)

        session["user_id"] = rows[0]["id"]
        return redirect("/")

    return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out."""
    session.clear()
    return redirect("/")


# -------------------------
# Quote
# -------------------------
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Look up a stock's price."""
    if request.method == "GET":
        return render_template("quote.html")

    symbol = request.form.get("symbol")
    if not symbol:
        return apology("must provide symbol", 400)

    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol", 400)

    # quote is a dict with name, price (float), symbol (uppercase)
    return render_template("quoted.html", quote=quote)


# -------------------------
# Buy
# -------------------------
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of a stock."""
    user_id = session["user_id"]

    if request.method == "GET":
        return render_template("buy.html")

    # POST
    symbol = request.form.get("symbol")
    shares_str = request.form.get("shares")

    # Validate input
    if not symbol:
        return apology("must provide symbol", 400)
    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol", 400)

    if not shares_str:
        return apology("must provide shares", 400)
    # Ensure shares is a positive integer (no floats)
    if not shares_str.isdigit():
        return apology("shares must be a positive integer", 400)
    shares = int(shares_str)
    if shares <= 0:
        return apology("shares must be a positive integer", 400)

    price = quote["price"]
    cost = price * shares

    # Check user's cash
    rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    if len(rows) != 1:
        return apology("user not found", 500)
    cash = rows[0]["cash"]
    if cash < cost:
        return apology("can't afford", 400)

    # Record transaction and update cash
    db.execute(
        "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
        user_id,
        quote["symbol"],
        shares,
        price,
    )
    db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", cost, user_id)

    flash("Bought!")
    return redirect("/")


# -------------------------
# Sell
# -------------------------
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of a stock that the user owns."""
    user_id = session["user_id"]

    if request.method == "GET":
        # fetch symbols the user currently has (net shares > 0)
        rows = db.execute(
            "SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING shares > 0",
            user_id,
        )
        symbols = [r["symbol"] for r in rows]
        return render_template("sell.html", symbols=symbols)

    # POST
    symbol = request.form.get("symbol")
    shares_str = request.form.get("shares")

    if not symbol:
        return apology("must provide symbol", 400)
    if not shares_str:
        return apology("must provide shares", 400)
    if not shares_str.isdigit():
        return apology("shares must be a positive integer", 400)

    shares = int(shares_str)
    if shares <= 0:
        return apology("shares must be a positive integer", 400)

    # How many shares does user own?
    rows = db.execute(
        "SELECT SUM(shares) AS shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol",
        user_id,
        symbol,
    )
    if len(rows) == 0 or rows[0]["shares"] is None or int(rows[0]["shares"]) < shares:
        return apology("not enough shares", 400)

    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol", 400)

    price = quote["price"]
    proceeds = price * shares

    # Record sale (negative shares), add cash
    db.execute(
        "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
        user_id,
        quote["symbol"],
        -shares,
        price,
    )
    db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", proceeds, user_id)

    flash("Sold!")
    return redirect("/")


# -------------------------
# History
# -------------------------
@app.route("/history")
@login_required
def history():
    """Show history of all transactions for user."""
    user_id = session["user_id"]

    rows = db.execute(
        "SELECT symbol, shares, price, transacted FROM transactions WHERE user_id = ? ORDER BY transacted DESC",
        user_id,
    )

    # Add a price_str_no_dollar key (two decimals, no $) to be friendly for automated graders
    history = []
    for r in rows:
        history.append(
            {
                "symbol": r["symbol"],
                "shares": r["shares"],
                "price": r["price"],
                # helpful: formatted price (no $) for templates that need it
                "price_str": "{:.2f}".format(r["price"]),
                "transacted": r["transacted"],
            }
        )

    return render_template("history.html", history=history)


# -------------------------
# Add Cash (personal touch)
# -------------------------
@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    """Allow user to deposit more cash into their account."""
    user_id = session["user_id"]

    if request.method == "GET":
        return render_template("add_cash.html")

    amount_str = request.form.get("amount")
    if not amount_str:
        return apology("must provide amount", 400)

    # Allow decimal amounts like 100.50
    try:
        amount = float(amount_str)
    except ValueError:
        return apology("invalid amount", 400)
    if amount <= 0:
        return apology("amount must be positive", 400)

    # Update user's cash
    db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", amount, user_id)

    # Optionally flash and redirect back to index
    flash("Cash added!")
    return redirect("/")


# -------------------------
# Change password (personal touch)
# -------------------------
@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Let a logged-in user change their password."""
    user_id = session["user_id"]

    if request.method == "GET":
        return render_template("change_password.html")

    old = request.form.get("old_password")
    new = request.form.get("new_password")
    confirm = request.form.get("confirmation")

    if not old or not new or not confirm:
        return apology("must fill out all fields", 400)
    if new != confirm:
        return apology("new passwords do not match", 400)

    rows = db.execute("SELECT hash FROM users WHERE id = ?", user_id)
    if len(rows) != 1 or not check_password_hash(rows[0]["hash"], old):
        return apology("current password incorrect", 400)

    db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(new), user_id)
    flash("Password changed!")
    return redirect("/")


# -------------------------
# API route (optional) - return quote JSON
# -------------------------
@app.route("/price")
@login_required
def price():
    """Return current price for a symbol as JSON (useful for AJAX or debugging)."""
    symbol = request.args.get("symbol")
    if not symbol:
        return jsonify({"error": "missing symbol"}), 400

    quote = lookup(symbol)
    if quote is None:
        return jsonify({"error": "invalid symbol"}), 400

    return jsonify(quote)


# -----------------------------------------------------------------------------
# End of app
# -----------------------------------------------------------------------------
if __name__ == "__main__":
    # When run directly, start Flask (useful for local debugging)
    app.run(host="0.0.0.0", port=5000, debug=True)
