#!/usr/bin/env python3
import sqlite3
from flask import Flask, render_template, request, redirect, url_for, g
from datetime import datetime
import os

app = Flask(__name__)
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATABASE = os.path.join(BASE_DIR, "tasks.db")

def get_db_connection():
    conn = sqlite3.connect(DATABASE)
    conn.row_factory = sqlite3.Row
    return conn

def init_db():
    conn = get_db_connection()
    conn.execute("""
    CREATE TABLE IF NOT EXISTS tasks (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      description TEXT NOT NULL,
      hours REAL NOT NULL,
      created_at TEXT NOT NULL
    );
    """)
    conn.commit()
    conn.close()

@app.route("/")
def index():
    conn = get_db_connection()
    rows = conn.execute("SELECT * FROM tasks ORDER BY created_at DESC").fetchall()
    total = conn.execute("SELECT IFNULL(SUM(hours), 0) as total FROM tasks").fetchone()["total"]
    conn.close()
    # ensure total is a float
    total = float(total) if total is not None else 0.0
    return render_template("index.html", tasks=rows, total_hours=total)

@app.route("/add", methods=["GET", "POST"])
def add():
    if request.method == "POST":
        description = request.form.get("description", "").strip()
        hours_raw = request.form.get("hours", "").strip()
        # basic validation
        if not description or not hours_raw:
            return render_template("add.html", error="Please provide both description and hours.", description=description, hours=hours_raw)
        try:
            hours = float(hours_raw)
        except ValueError:
            return render_template("add.html", error="Hours must be a number (e.g., 1 or 1.5).", description=description, hours=hours_raw)

        conn = get_db_connection()
        conn.execute(
            "INSERT INTO tasks (description, hours, created_at) VALUES (?, ?, ?)",
            (description, hours, datetime.utcnow().isoformat())
        )
        conn.commit()
        conn.close()
        return redirect(url_for("index"))
    else:
        return render_template("add.html")

@app.route("/delete", methods=["POST"])
def delete():
    task_id = request.form.get("id")
    if task_id:
        conn = get_db_connection()
        conn.execute("DELETE FROM tasks WHERE id = ?", (task_id,))
        conn.commit()
        conn.close()
    return redirect(url_for("index"))

if __name__ == "__main__":
    # create DB file + table if missing
    init_db()
    app.run(host="0.0.0.0", port=5000, debug=True)