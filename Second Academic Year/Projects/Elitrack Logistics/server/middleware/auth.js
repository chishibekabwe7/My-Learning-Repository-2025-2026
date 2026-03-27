const jwt = require('jsonwebtoken');
const { pool } = require('../config/db');

const SECRET = process.env.JWT_SECRET;
const JWT_ISSUER = process.env.JWT_ISSUER || 'elitrack-api';
const JWT_AUDIENCE = process.env.JWT_AUDIENCE || 'elitrack-client';

if (!SECRET) {
  throw new Error('JWT_SECRET is required in environment variables.');
}

const authMiddleware = (req, res, next) => {
  const header = req.headers.authorization || '';
  if (!header.startsWith('Bearer ')) return res.status(401).json({ error: 'Unauthorized' });
  const token = header.slice(7);
  if (!token) return res.status(401).json({ error: 'Unauthorized' });
  try {
    req.user = jwt.verify(token, SECRET, { issuer: JWT_ISSUER, audience: JWT_AUDIENCE });
    next();
  } catch {
    res.status(401).json({ error: 'Invalid token' });
  }
};

const adminOnly = async (req, res, next) => {
  if (!req.user?.id) return res.status(403).json({ error: 'Admins only' });
  const [rows] = await pool.query('SELECT role FROM users WHERE id = ? LIMIT 1', [req.user.id]);
  if (!rows.length || rows[0].role !== 'admin') return res.status(403).json({ error: 'Admins only' });
  req.user.role = 'admin';
  next();
};

module.exports = { authMiddleware, adminOnly };
