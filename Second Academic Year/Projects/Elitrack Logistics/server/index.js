require('dotenv').config();
const express = require('express');
const cors = require('cors');
const helmet = require('helmet');
const { initDB } = require('./config/db');
const { authRateLimit, adminRateLimit } = require('./middleware/rateLimit');

if (!process.env.JWT_SECRET) {
  throw new Error('JWT_SECRET is required in server/.env');
}

const app = express();
app.disable('x-powered-by');
app.use(helmet());
app.use(cors({ origin: 'http://localhost:3000', credentials: true }));
app.use(express.json({ limit: '200kb' }));

app.use('/api/auth', authRateLimit, require('./routes/auth'));
app.use('/api/bookings', require('./routes/bookings'));
app.use('/api/admin', adminRateLimit, require('./routes/admin'));

app.get('/', (_, res) => {
  res.json({
    name: 'Elitrack Logistics API',
    status: 'running',
    client_url: 'http://localhost:3000',
    health_url: '/api/health',
  });
});

app.get('/api/health', (_, res) => res.json({ status: 'ok', time: new Date() }));

const PORT = process.env.PORT || 5000;
initDB().then(() => {
  app.listen(PORT, () => console.log(`🚀 Elitrack Logistics API running on port ${PORT}`));
}).catch(err => {
  console.error('DB init failed:', err.message);
  process.exit(1);
});
