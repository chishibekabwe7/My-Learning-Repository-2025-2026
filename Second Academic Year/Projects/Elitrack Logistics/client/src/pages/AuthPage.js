import { faCircleCheck, faCircleXmark } from '@fortawesome/free-solid-svg-icons';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { GoogleLogin } from '@react-oauth/google';
import { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import api from '../api';
import { useAuth } from '../context/AuthContext';

export default function AuthPage() {
  const [mode, setMode] = useState('login');
  const [form, setForm] = useState({ email: '', password: '', phone: '', full_name: '', company: '' });
  const [error, setError] = useState('');
  const [success, setSuccess] = useState('');
  const [loading, setLoading] = useState(false);
  const { login, register } = useAuth();
  const navigate = useNavigate();

  const handle = e => setForm(f => ({ ...f, [e.target.name]: e.target.value }));
  const getErrorMessage = (err, fallback = 'Something went wrong') => {
    if (err?.response?.data?.error) return err.response.data.error;
    if (err?.message === 'Network Error' || String(err?.message || '').toLowerCase().includes('failed to fetch')) {
      return 'Unable to connect to the server. Please try again in a moment.';
    }
    return err?.message || fallback;
  };

  const handleGoogleSuccess = async (credentialResponse) => {
    console.log('[CHECK] Google Login Success - Token received');
    setError('');
    setLoading(true);
    try {
      console.log('[SEND] Sending token to backend...');
      const { data } = await api.post('/auth/google', { token: credentialResponse.credential });
      console.log('[RECV] Backend response received');
      
      if (!data.token || !data.user) {
        console.error('[ERROR] Invalid response structure:', data);
        throw new Error('Invalid response from server');
      }
      
      // Store token and redirect using same keys as AuthContext
      console.log('[CHECK] Token stored, redirecting...');
      localStorage.setItem('tl_token', data.token);
      localStorage.setItem('tl_user', JSON.stringify(data.user));
      navigate(data.user.role === 'admin' ? '/admin' : '/dashboard');
    } catch (err) {
      console.error('[ERROR] Google login error:', err);
      setError(getErrorMessage(err, 'Google login failed. Check browser console for details.'));
    } finally {
      setLoading(false);
    }
  };

  const handleGoogleError = (error) => {
    console.error('[ERROR] Google Authentication Error:', error);
    setError('Google login failed. Please try again.');
  };

  const submit = async e => {
    e.preventDefault();
    setError('');
    setSuccess('');
    setLoading(true);
    try {
      if (mode === 'login') {
        const user = await login(form.email, form.password);
        navigate(user.role === 'admin' ? '/admin' : '/dashboard');
      } else {
        // For register, just create the account without logging in
        await register(form);
        setSuccess('Account created successfully! Please log in with your credentials.');
        setForm({ email: '', password: '', phone: '', full_name: '', company: '' });
        setTimeout(() => {
          setMode('login');
          setSuccess('');
        }, 2000);
      }
    } catch (err) {
      setError(getErrorMessage(err));
    } finally {
      setLoading(false);
    }
  };

  return (
    <div style={{ minHeight: '100vh', background: '#1D2429', display: 'flex', alignItems: 'center', justifyContent: 'center', padding: 20 }}>
      <div style={{ width: '100%', maxWidth: 380 }}>
        <div style={{ textAlign: 'center', marginBottom: 32 }}>
          <h1 style={{ color: '#30BDEC', fontSize: 32, fontWeight: 800, letterSpacing: 4, fontFamily: 'Roboto' }}>ELITRACK</h1>
          <p style={{ color: '#666', fontSize: 11, letterSpacing: 2, marginTop: 6 }}>FLEET MULTI-ASSET PORTAL</p>
        </div>

        <div style={{ background: '#1D2429', borderRadius: 16, border: '1px solid #30BDEC', padding: 32 }}>
          <div style={{ display: 'flex', gap: 8, marginBottom: 28 }}>
            {['login','register'].map(m => (
              <button key={m} onClick={() => setMode(m)} className="btn"
                style={{ flex: 1, background: mode === m ? '#30BDEC' : 'transparent', color: mode === m ? 'white' : '#666', border: '1px solid #333', fontSize: 11, fontFamily: 'Roboto' }}>
                {m === 'login' ? 'Sign In' : 'Register'}
              </button>
            ))}
          </div>

          {mode === 'login' && (
            <div style={{ marginBottom: 24 }}>
              <GoogleLogin
                onSuccess={handleGoogleSuccess}
                onError={handleGoogleError}
              />
            </div>
          )}

          <div style={{ textAlign: 'center', marginBottom: 20, color: '#555', fontSize: 12 }}>
            {mode === 'login' && 'Or continue with email'}
          </div>

          <form onSubmit={submit}>
            {mode === 'register' && (
              <>
                <div className="form-group">
                  <label style={{ color: '#888' }}>Full Name</label>
                  <input name="full_name" placeholder="Your full name" value={form.full_name} onChange={handle}
                    style={{ background: '#1D2429', borderColor: '#333', color: 'white', fontFamily: 'Roboto' }} />
                </div>
                <div className="form-group">
                  <label style={{ color: '#888' }}>Company / Mine</label>
                  <input name="company" placeholder="e.g. Kansanshi Mining" value={form.company} onChange={handle}
                    style={{ background: '#1D2429', borderColor: '#333', color: 'white', fontFamily: 'Roboto' }} />
                </div>
                <div className="form-group">
                  <label style={{ color: '#888' }}>Phone</label>
                  <input name="phone" placeholder="0977 000 000" value={form.phone} onChange={handle} required
                    style={{ background: '#1D2429', borderColor: '#333', color: 'white', fontFamily: 'Roboto' }} />
                </div>
              </>
            )}
            <div className="form-group">
              <label style={{ color: '#888' }}>Email</label>
              <input name="email" type="email" placeholder="you@company.zm" value={form.email} onChange={handle} required
                style={{ background: '#1D2429', borderColor: '#333', color: 'white', fontFamily: 'Roboto' }} />
            </div>
            <div className="form-group">
              <label style={{ color: '#888' }}>Password</label>
              <input name="password" type="password" placeholder="••••••••" value={form.password} onChange={handle} required
                style={{ background: '#1D2429', borderColor: '#333', color: 'white', fontFamily: 'Roboto' }} />
            </div>

            {success && <p style={{ color: '#4ade80', fontSize: 12, marginBottom: 16, textAlign: 'center', fontWeight: 600 }}><FontAwesomeIcon icon={faCircleCheck} style={{ color: '#4ade80', marginRight: 8 }} />{success}</p>}
            {error && <p style={{ color: '#e74c3c', fontSize: 12, marginBottom: 16, textAlign: 'center' }}><FontAwesomeIcon icon={faCircleXmark} style={{ color: '#e74c3c', marginRight: 8 }} />{error}</p>}

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
