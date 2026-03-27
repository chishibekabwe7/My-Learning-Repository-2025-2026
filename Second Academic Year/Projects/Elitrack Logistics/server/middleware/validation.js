const isEmail = (value) => /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(String(value || ''));

const validateRegister = (req, res, next) => {
  const { email, phone, password, full_name, company } = req.body;
  if (!email || !phone || !password) {
    return res.status(400).json({ error: 'email, phone and password are required.' });
  }
  if (!isEmail(email)) return res.status(400).json({ error: 'Invalid email format.' });
  if (String(password).length < 8) return res.status(400).json({ error: 'Password must be at least 8 characters.' });
  if (String(phone).length < 7 || String(phone).length > 20) {
    return res.status(400).json({ error: 'Invalid phone number length.' });
  }
  if (full_name && String(full_name).length > 120) return res.status(400).json({ error: 'full_name too long.' });
  if (company && String(company).length > 120) return res.status(400).json({ error: 'company too long.' });
  next();
};

const validateLogin = (req, res, next) => {
  const { email, password } = req.body;
  if (!email || !password) return res.status(400).json({ error: 'email and password are required.' });
  if (!isEmail(email)) return res.status(400).json({ error: 'Invalid email format.' });
  next();
};

const validateGoogleAuth = (req, res, next) => {
  const { token } = req.body;
  if (!token || String(token).length < 20) return res.status(400).json({ error: 'Valid Google token is required.' });
  next();
};

const validateBookingCreate = (req, res, next) => {
  const {
    truck_type, truck_price_per_day, units, days, hub,
    security_tier, security_price, total_amount,
  } = req.body;
  if (!truck_type || !hub || !security_tier) return res.status(400).json({ error: 'Missing required booking fields.' });
  if ([truck_price_per_day, units, days, security_price, total_amount].some((v) => Number.isNaN(Number(v)))) {
    return res.status(400).json({ error: 'Booking numeric fields must be valid numbers.' });
  }
  if (Number(units) < 1 || Number(days) < 1) return res.status(400).json({ error: 'units and days must be >= 1.' });
  next();
};

module.exports = {
  validateRegister,
  validateLogin,
  validateGoogleAuth,
  validateBookingCreate,
};
