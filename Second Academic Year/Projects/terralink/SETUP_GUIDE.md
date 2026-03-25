# Terralink.fleets — Full Stack Setup Guide

## Project Architecture

```
terralink/
├── package.json            ← root (runs both servers)
├── server/                 ← Express + MySQL API
│   ├── index.js
│   ├── .env                ← YOUR DB credentials go here
│   ├── config/db.js
│   ├── middleware/auth.js
│   └── routes/
│       ├── auth.js
│       ├── bookings.js
│       └── admin.js
└── client/                 ← React frontend
    ├── public/index.html
    └── src/
        ├── App.js
        ├── index.js
        ├── index.css
        ├── api/index.js
        ├── context/AuthContext.js
        ├── components/ProtectedRoute.js
        └── pages/
            ├── AuthPage.js
            ├── Dashboard.js
            └── AdminDashboard.js
```

---

## Step 1 — Prerequisites

Install these before anything else:

| Tool | Download |
|------|----------|
| Node.js (v18+) | https://nodejs.org |
| MySQL (v8+) | https://dev.mysql.com/downloads/mysql/ |
| VS Code | https://code.visualstudio.com |
| Git (optional) | https://git-scm.com |

Verify installations in your terminal:

```bash
node -v        # should show v18.x or higher
npm -v         # should show 9.x or higher
mysql --version
```

---

## Step 2 — Open in VS Code

1. Open VS Code
2. Go to **File → Open Folder** and select the `terralink` folder
3. For the best experience, open the workspace file instead:
   - **File → Open Workspace from File** → select `terralink.code-workspace`
4. Open the integrated terminal: **Terminal → New Terminal** (or `Ctrl + `` ` ``)

---

## Step 3 — Configure MySQL

### Option A: MySQL already installed

Open MySQL Workbench or your terminal and run:

```sql
CREATE DATABASE terralink_db;
CREATE USER 'terralink_user'@'localhost' IDENTIFIED BY 'your_password';
GRANT ALL PRIVILEGES ON terralink_db.* TO 'terralink_user'@'localhost';
FLUSH PRIVILEGES;
```

### Option B: Fresh MySQL install

During installation, set a root password. Then use root credentials in the `.env` file.

---

## Step 4 — Configure Environment Variables

Open `server/.env` and fill in your MySQL details:

```env
PORT=5000
DB_HOST=localhost
DB_USER=root
DB_PASSWORD=your_mysql_root_password_here
DB_NAME=terralink_db
JWT_SECRET=terralink_ultra_secret_2026
```

> The database tables are created **automatically** when the server starts — you do not need to run any SQL files manually.

---

## Step 5 — Install All Dependencies

In the VS Code terminal, from the `terralink` root folder:

```bash
# Install root dependencies (concurrently)
npm install

# Install server dependencies
cd server
npm install
cd ..

# Install client dependencies
cd client
npm install
cd ..
```

Or run them all at once:

```bash
npm run install:all
```

---

## Step 6 — Run the App

From the `terralink` root folder, run both servers simultaneously:

```bash
npm run dev
```

This starts:
- **API server** → http://localhost:5000
- **React client** → http://localhost:3000

Your browser will open automatically at `http://localhost:3000`.

---

## Step 7 — First Login

| Role | Email | Password |
|------|-------|----------|
| Admin | admin@terralink.zm | admin123 |
| Client | Register a new account | — |

> The admin account is seeded automatically on first server start.

---

## API Endpoints Reference

### Auth
| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | /api/auth/register | Register new client |
| POST | /api/auth/login | Login (returns JWT token) |

### Bookings (requires auth token)
| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | /api/bookings | Create a new booking |
| GET | /api/bookings/mine | Get current user's bookings |
| GET | /api/bookings/all | Get ALL bookings (admin only) |
| PATCH | /api/bookings/:id/status | Update booking status (admin only) |
| PATCH | /api/bookings/:id/payment | Mark payment as paid (admin only) |

### Admin (requires admin JWT)
| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | /api/admin/stats | Dashboard stats |
| GET | /api/admin/users | All registered users |
| GET | /api/admin/transactions | All transactions |

---

## Database Tables

### `users`
Stores all registered users. Role is either `client` or `admin`.

### `bookings`
Every convoy booking with truck type, hub, security tier, units, days, and total cost.

### `transactions`
One transaction record per booking. Admin can mark as `paid`, `pending`, or `refunded`.

### `fleet_telemetry`
GPS position logs per truck per booking. Ready for IoT device integration.

---

## VS Code Recommended Extensions

Install these from the Extensions panel (`Ctrl+Shift+X`):

- **ESLint** — `dbaeumer.vscode-eslint`
- **Prettier** — `esbenp.prettier-vscode`
- **Auto Rename Tag** — `formulahendry.auto-rename-tag`
- **Material Icon Theme** — `PKief.material-icon-theme`
- **SQLTools** — `mtxr.sqltools`
- **SQLTools MySQL Driver** — `mtxr.sqltools-driver-mysql`

---

## Common Errors & Fixes

### `Error: connect ECONNREFUSED 127.0.0.1:3306`
MySQL is not running. Start it:
- **Windows**: Open Services → Start MySQL80
- **Mac**: `brew services start mysql`
- **Linux**: `sudo systemctl start mysql`

### `Access denied for user 'root'@'localhost'`
Wrong password in `server/.env`. Double-check `DB_PASSWORD`.

### `npm ERR! code ENOENT`
You're in the wrong directory. Make sure you `cd server` before `npm install` in the server folder.

### `Port 3000 already in use`
Another app is using port 3000. Kill it:
```bash
# Windows
netstat -ano | findstr :3000
taskkill /PID <pid> /F

# Mac/Linux
lsof -ti:3000 | xargs kill
```

### React shows blank screen
Open browser DevTools (F12) → Console tab to see the error. Usually a missing `.env` or server not running.

---

## Deploying to Production

When ready to deploy:

1. Build the React app: `cd client && npm run build`
2. Serve the `client/build` folder from Express by adding to `server/index.js`:
```js
app.use(express.static(path.join(__dirname, '../client/build')));
app.get('*', (req, res) => res.sendFile(path.join(__dirname, '../client/build/index.html')));
```
3. Use a process manager like **PM2**: `pm2 start server/index.js --name terralink`
4. Point your domain/server to port 5000

---

## Support

**Founder:** Elijah Mufwambi  
**Dispatch:** 0973930287  
**System:** Terralink.fleets © 2026
