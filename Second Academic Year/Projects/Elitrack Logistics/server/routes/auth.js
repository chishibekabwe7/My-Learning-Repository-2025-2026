const router = require('express').Router();
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');
const { OAuth2Client } = require('google-auth-library');
const { pool } = require('../config/db');
const { validateRegister, validateLogin, validateGoogleAuth } = require('../middleware/validation');
const SECRET = process.env.JWT_SECRET;
const JWT_ISSUER = process.env.JWT_ISSUER || 'elitrack-api';
const JWT_AUDIENCE = process.env.JWT_AUDIENCE || 'elitrack-client';
const JWT_EXPIRES_IN = process.env.JWT_EXPIRES_IN || '12h';

if (!SECRET) {
  throw new Error('JWT_SECRET is required in environment variables.');
}

const signAuthToken = (user) => jwt.sign(
  { id: user.id, email: user.email, role: user.role },
  SECRET,
  { expiresIn: JWT_EXPIRES_IN, issuer: JWT_ISSUER, audience: JWT_AUDIENCE }
);

// Register
router.post('/register', validateRegister, async (req, res) => {
  const { email, phone, password, full_name, company } = req.body;
  if (!email || !phone || !password) return res.status(400).json({ error: 'Missing fields' });
  try {
    const hash = await bcrypt.hash(password, 10);
    const [result] = await pool.query(
      'INSERT INTO users (email, phone, password_hash, full_name, company) VALUES (?,?,?,?,?)',
      [email, phone, hash, full_name || '', company || '']
    );
    res.json({ success: true, message: 'Account created successfully. Please log in.' });
  } catch (e) {
    if (e.code === 'ER_DUP_ENTRY') return res.status(409).json({ error: 'Email already registered' });
    res.status(500).json({ error: e.message });
  }
});

// Login
router.post('/login', validateLogin, async (req, res) => {
  const { email, password } = req.body;
  try {
    const [rows] = await pool.query('SELECT * FROM users WHERE email = ?', [email]);
    if (!rows.length) return res.status(401).json({ error: 'Invalid credentials' });
    const user = rows[0];
    const match = await bcrypt.compare(password, user.password_hash);
    if (!match) return res.status(401).json({ error: 'Invalid credentials' });
    const token = signAuthToken(user);
    res.json({ token, user: { id: user.id, email: user.email, role: user.role, full_name: user.full_name, company: user.company } });
  } catch (e) {
    res.status(500).json({ error: e.message });
  }
});

// Google OAuth
router.post('/google', validateGoogleAuth, async (req, res) => {
  const { token } = req.body;
  try {
    if (!process.env.GOOGLE_CLIENT_ID) {
      return res.status(500).json({ error: 'GOOGLE_CLIENT_ID is not configured.' });
    }
    const client = new OAuth2Client(process.env.GOOGLE_CLIENT_ID);
    const ticket = await client.verifyIdToken({
      idToken: token,
      audience: process.env.GOOGLE_CLIENT_ID
    });
    const payload = ticket.getPayload();
    const { email, name } = payload;

    // Check if user exists, create if not
    const [rows] = await pool.query('SELECT * FROM users WHERE email = ?', [email]);
    let user;

    if (rows.length === 0) {
      const [result] = await pool.query(
        'INSERT INTO users (email, full_name, phone, password_hash) VALUES (?,?,?,?)',
        [email, name || '', 'N/A', await bcrypt.hash('oauth_user', 10)]
      );
      user = { id: result.insertId, email, full_name: name, role: 'client' };
    } else {
      user = rows[0];
    }

    const jwtToken = signAuthToken(user);
    res.json({ token: jwtToken, user: { id: user.id, email: user.email, role: user.role, full_name: user.full_name, company: user.company } });
  } catch (err) {
    console.error('Google auth error:', err.message);
    res.status(401).json({ error: 'Google authentication failed: ' + err.message });
  }
});

module.exports = router;
