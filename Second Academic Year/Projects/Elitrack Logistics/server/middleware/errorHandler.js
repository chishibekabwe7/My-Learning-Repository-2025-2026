const logger = require('../utils/logger');

const notFoundHandler = (req, res, next) => {
  const error = new Error(`Route not found: ${req.method} ${req.originalUrl}`);
  error.status = 400;
  next(error);
};

const errorHandler = (err, req, res, _next) => {
  const status = err.status && err.status >= 400 && err.status < 500 ? 400 : 500;
  const message = status === 400 ? err.message || 'Bad request' : 'Internal server error';

  if (status === 500) {
    logger.error('Unhandled server error', {
      path: req.originalUrl,
      method: req.method,
      message: err.message,
      stack: err.stack,
    });
  } else {
    logger.warn('Client request error', {
      path: req.originalUrl,
      method: req.method,
      message,
    });
  }

  res.status(status).json({ error: true, message });
};

module.exports = { notFoundHandler, errorHandler };
