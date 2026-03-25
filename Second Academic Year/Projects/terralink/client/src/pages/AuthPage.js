import { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { useAuth } from '../context/AuthContext';

export default function AuthPage() {
  const [mode, setMode] = useState('login');
  const [form, setForm] = useState({ email: '', password: '', phone: '', full_name: '', company: '' });
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);
  const { login, register } = useAuth();
  const navigate = useNavigate();

  const handle = e => setForm(f => ({ ...f, [e.target.name]: e.target.value }));

  const submit = async e => {
    e.preventDefault();
    setError(''); setLoading(true);
    try {
      const user = mode === 'login'
        ? await login(form.email, form.password)
        : await register(form);
      navigate(user.role === 'admin' ? '/admin' : '/dashboard');
    } catch (err) {
      setError(err.response?.data?.error || 'Something went wrong');
    } finally { setLoading(false); }
  };

  return (
    <div style={{ minHeight: '100vh', background: '#0e0e0e', display: 'flex', alignItems: 'center', justifyContent: 'center', padding: 20 }}>
      <div style={{ width: '100%', maxWidth: 380 }}>
        <div style={{ textAlign: 'center', marginBottom: 32 }}>
          <h1 style={{ color: '#d4af37', fontSize: 32, fontWeight: 800, letterSpacing: 4 }}>ELITRACK</h1>
          <p style={{ color: '#666', fontSize: 11, letterSpacing: 2, marginTop: 6 }}>FLEET MULTI-ASSET PORTAL</p>
        </div>

        <div style={{ background: '#1a1a1a', borderRadius: 16, border: '1px solid #d4af37', padding: 32 }}>
          <div style={{ display: 'flex', gap: 8, marginBottom: 28 }}>
            {['login','register'].map(m => (
              <button key={m} onClick={() => setMode(m)} className="btn"
                style={{ flex: 1, background: mode === m ? '#d4af37' : 'transparent', color: mode === m ? '#0e0e0e' : '#666', border: '1px solid #333', fontSize: 11 }}>
                {m === 'login' ? 'Sign In' : 'Register'}
              </button>
            ))}
          </div>

          <form onSubmit={submit}>
            {mode === 'register' && (
              <>
                <div className="form-group">
                  <label style={{ color: '#888' }}>Full Name</label>
                  <input name="full_name" placeholder="Your full name" value={form.full_name} onChange={handle}
                    style={{ background: '#242424', borderColor: '#333', color: 'white' }} />
                </div>
                <div className="form-group">
                  <label style={{ color: '#888' }}>Company / Mine</label>
                  <input name="company" placeholder="e.g. Kansanshi Mining" value={form.company} onChange={handle}
                    style={{ background: '#242424', borderColor: '#333', color: 'white' }} />
                </div>
                <div className="form-group">
                  <label style={{ color: '#888' }}>Phone</label>
                  <input name="phone" placeholder="0977 000 000" value={form.phone} onChange={handle} required
                    style={{ background: '#242424', borderColor: '#333', color: 'white' }} />
                </div>
              </>
            )}
            <div className="form-group">
              <label style={{ color: '#888' }}>Email</label>
              <input name="email" type="email" placeholder="you@company.zm" value={form.email} onChange={handle} required
                style={{ background: '#242424', borderColor: '#333', color: 'white' }} />
            </div>
            <div className="form-group">
              <label style={{ color: '#888' }}>Password</label>
              <input name="password" type="password" placeholder="••••••••" value={form.password} onChange={handle} required
                style={{ background: '#242424', borderColor: '#333', color: 'white' }} />
            </div>

            {error && <p style={{ color: '#e74c3c', fontSize: 12, marginBottom: 16, textAlign: 'center' }}>{error}</p>}

            <button type="submit" className="btn btn-gold btn-full" disabled={loading}>
              {loading ? 'Please wait...' : mode === 'login' ? 'Access Fleet Portal' : 'Create Account'}
            </button>
          </form>

          <p style={{ textAlign: 'center', marginTop: 20, fontSize: 11, color: '#555' }}>
            Admin: admin@elictrack.zm / admin123
          </p>
        </div>
      </div>
    </div>
  );
}
