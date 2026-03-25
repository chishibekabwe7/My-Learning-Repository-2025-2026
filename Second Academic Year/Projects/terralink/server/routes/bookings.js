const router = require('express').Router();
const { pool } = require('../config/db');
const { authMiddleware, adminOnly } = require('../middleware/auth');

// Generate booking reference
const genRef = () => 'TL-' + Date.now().toString(36).toUpperCase();

// Create booking (client)
router.post('/', authMiddleware, async (req, res) => {
  const { truck_type, truck_price_per_day, units, days, hub, security_tier, security_price, total_amount, notes } = req.body;
  try {
    const ref = genRef();
    const [result] = await pool.query(
      `INSERT INTO bookings (user_id, booking_ref, truck_type, truck_price_per_day, units, days, hub, security_tier, security_price, total_amount, notes)
       VALUES (?,?,?,?,?,?,?,?,?,?,?)`,
      [req.user.id, ref, truck_type, truck_price_per_day, units, days, hub, security_tier, security_price, total_amount, notes || '']
    );
    // Create pending transaction
    await pool.query(
      'INSERT INTO transactions (booking_id, user_id, amount) VALUES (?,?,?)',
      [result.insertId, req.user.id, total_amount]
    );
    res.json({ id: result.insertId, booking_ref: ref });
  } catch (e) {
    res.status(500).json({ error: e.message });
  }
});

// Get my bookings (client)
router.get('/mine', authMiddleware, async (req, res) => {
  const [rows] = await pool.query(
    'SELECT * FROM bookings WHERE user_id = ? ORDER BY created_at DESC', [req.user.id]
  );
  res.json(rows);
});

// Get all bookings (admin)
router.get('/all', authMiddleware, adminOnly, async (req, res) => {
  const [rows] = await pool.query(
    `SELECT b.*, u.email, u.full_name, u.company FROM bookings b
     JOIN users u ON b.user_id = u.id ORDER BY b.created_at DESC`
  );
  res.json(rows);
});

// Update booking status (admin)
router.patch('/:id/status', authMiddleware, adminOnly, async (req, res) => {
  const { status } = req.body;
  await pool.query('UPDATE bookings SET status = ? WHERE id = ?', [status, req.params.id]);
  res.json({ success: true });
});

// Update transaction status (admin)
router.patch('/:id/payment', authMiddleware, adminOnly, async (req, res) => {
  const { status, payment_method } = req.body;
  await pool.query(
    'UPDATE transactions SET status = ?, payment_method = ? WHERE booking_id = ?',
    [status, payment_method, req.params.id]
  );
  res.json({ success: true });
});

module.exports = router;
