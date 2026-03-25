const mysql = require('mysql2/promise');

const pool = mysql.createPool({
  host: process.env.DB_HOST || 'localhost',
  user: process.env.DB_USER || 'root',
  password: process.env.DB_PASSWORD || '',
  database: process.env.DB_NAME || 'terralink_db',
  waitForConnections: true,
  connectionLimit: 10,
});

const initDB = async () => {
  const conn = await pool.getConnection();
  try {
    await conn.query(`CREATE DATABASE IF NOT EXISTS terralink_db`);
    await conn.query(`USE terralink_db`);

    await conn.query(`
      CREATE TABLE IF NOT EXISTS users (
        id INT AUTO_INCREMENT PRIMARY KEY,
        email VARCHAR(255) UNIQUE NOT NULL,
        phone VARCHAR(50) NOT NULL,
        password_hash VARCHAR(255) NOT NULL,
        role ENUM('client','admin') DEFAULT 'client',
        full_name VARCHAR(255),
        company VARCHAR(255),
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
      )
    `);

    await conn.query(`
      CREATE TABLE IF NOT EXISTS bookings (
        id INT AUTO_INCREMENT PRIMARY KEY,
        user_id INT NOT NULL,
        booking_ref VARCHAR(20) UNIQUE NOT NULL,
        truck_type VARCHAR(100) NOT NULL,
        truck_price_per_day INT NOT NULL,
        units INT NOT NULL DEFAULT 1,
        days INT NOT NULL DEFAULT 1,
        hub VARCHAR(100) NOT NULL,
        security_tier VARCHAR(100) NOT NULL,
        security_price INT NOT NULL DEFAULT 0,
        total_amount INT NOT NULL,
        status ENUM('pending','active','completed','cancelled') DEFAULT 'pending',
        notes TEXT,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
        FOREIGN KEY (user_id) REFERENCES users(id)
      )
    `);

    await conn.query(`
      CREATE TABLE IF NOT EXISTS transactions (
        id INT AUTO_INCREMENT PRIMARY KEY,
        booking_id INT NOT NULL,
        user_id INT NOT NULL,
        amount INT NOT NULL,
        currency VARCHAR(10) DEFAULT 'ZMW',
        payment_method VARCHAR(50) DEFAULT 'pending',
        status ENUM('pending','paid','failed','refunded') DEFAULT 'pending',
        reference VARCHAR(100),
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (booking_id) REFERENCES bookings(id),
        FOREIGN KEY (user_id) REFERENCES users(id)
      )
    `);

    await conn.query(`
      CREATE TABLE IF NOT EXISTS fleet_telemetry (
        id INT AUTO_INCREMENT PRIMARY KEY,
        booking_id INT NOT NULL,
        truck_id VARCHAR(20) NOT NULL,
        latitude DECIMAL(10,7),
        longitude DECIMAL(10,7),
        speed INT DEFAULT 0,
        status VARCHAR(50) DEFAULT 'TRACKING',
        recorded_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (booking_id) REFERENCES bookings(id)
      )
    `);

    // Seed admin user if not exists
    const bcrypt = require('bcryptjs');
    const hash = await bcrypt.hash('admin123', 10);
    await conn.query(`
      INSERT IGNORE INTO users (email, phone, password_hash, role, full_name, company)
      VALUES ('admin@terralink.zm', '0973930287', ?, 'admin', 'Elijah Mufwambi', 'Terralink')
    `, [hash]);

    console.log('✅ Database initialized');
  } finally {
    conn.release();
  }
};

module.exports = { pool, initDB };
