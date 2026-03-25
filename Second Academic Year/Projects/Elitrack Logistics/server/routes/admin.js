const router = require('express').Router();
const { pool } = require('../config/db');
const { authMiddleware, adminOnly } = require('../middleware/auth');

// Dashboard stats
router.get('/stats', authMiddleware, adminOnly, async (req, res) => {
  const [[{ total_users }]] = await pool.query('SELECT COUNT(*) AS total_users FROM users WHERE role="client"');
  const [[{ total_bookings }]] = await pool.query('SELECT COUNT(*) AS total_bookings FROM bookings');
  const [[{ active_bookings }]] = await pool.query('SELECT COUNT(*) AS active_bookings FROM bookings WHERE status="active"');
  const [[{ total_revenue }]] = await pool.query('SELECT COALESCE(SUM(amount),0) AS total_revenue FROM transactions WHERE status="paid"');
  const [[{ pending_revenue }]] = await pool.query('SELECT COALESCE(SUM(amount),0) AS pending_revenue FROM transactions WHERE status="pending"');
  res.json({ total_users, total_bookings, active_bookings, total_revenue, pending_revenue });
});

// All users
router.get('/users', authMiddleware, adminOnly, async (req, res) => {
  const [rows] = await pool.query('SELECT id, email, phone, full_name, company, role, created_at FROM users ORDER BY created_at DESC');
  res.json(rows);
});

// All transactions
router.get('/transactions', authMiddleware, adminOnly, async (req, res) => {
  const [rows] = await pool.query(
    `SELECT t.*, b.booking_ref, b.truck_type, b.hub, u.email, u.full_name
     FROM transactions t
     JOIN bookings b ON t.booking_id = b.id
     JOIN users u ON t.user_id = u.id
     ORDER BY t.created_at DESC`
  );
  res.json(rows);
});

module.exports = router;
