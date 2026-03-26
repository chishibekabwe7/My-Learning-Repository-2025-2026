import { faBox, faChartBar, faHourglassEnd, faMoneyBill, faTruck, faUsers } from '@fortawesome/free-solid-svg-icons';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { useEffect, useState } from 'react';
import api from '../api';
import { useAuth } from '../context/AuthContext';

export default function AdminDashboard() {
  const { user, logout } = useAuth();
  const [tab, setTab] = useState('overview');
  const [stats, setStats] = useState(null);
  const [bookings, setBookings] = useState([]);
  const [users, setUsers] = useState([]);
  const [transactions, setTransactions] = useState([]);
  const [loading, setLoading] = useState(false);

  useEffect(() => { loadStats(); }, []);
  useEffect(() => {
    if (tab === 'bookings') loadBookings();
    if (tab === 'users') loadUsers();
    if (tab === 'transactions') loadTransactions();
  }, [tab]);

  const loadStats = async () => {
    const { data } = await api.get('/admin/stats');
    setStats(data);
  };

  const loadBookings = async () => {
    setLoading(true);
    try { const { data } = await api.get('/bookings/all'); setBookings(data); }
    finally { setLoading(false); }
  };

  const loadUsers = async () => {
    setLoading(true);
    try { const { data } = await api.get('/admin/users'); setUsers(data); }
    finally { setLoading(false); }
  };

  const loadTransactions = async () => {
    setLoading(true);
    try { const { data } = await api.get('/admin/transactions'); setTransactions(data); }
    finally { setLoading(false); }
  };

  const updateBookingStatus = async (id, status) => {
    await api.patch(`/bookings/${id}/status`, { status });
    loadBookings(); loadStats();
  };

  const updatePayment = async (id, status) => {
    await api.patch(`/bookings/${id}/payment`, { status, payment_method: 'Manual' });
    loadTransactions(); loadStats();
  };

  const TABS = [['overview','Overview'],['bookings','Bookings'],['transactions','Transactions'],['users','Users']];

  return (
    <div style={{ minHeight: '100vh', background: '#1D2429', color: 'white' }}>
      {/* Header */}
      <header style={{ background: '#1D2429', borderBottom: '3px solid #30BDEC', padding: '18px 28px', display: 'flex', justifyContent: 'space-between', alignItems: 'center', fontFamily: 'Roboto' }}>
        <div>
          <h1 style={{ color: '#30BDEC', fontSize: 20, fontWeight: 800, letterSpacing: 3, fontFamily: 'Roboto' }}>ELITRACK</h1>
          <p style={{ color: '#555', fontSize: 9, letterSpacing: 2 }}>ADMIN CONTROL CENTER</p>
        </div>
        <div style={{ display: 'flex', gap: 12, alignItems: 'center' }}>
          <span style={{ fontSize: 11, color: '#888' }}>{user?.full_name || user?.email}</span>
          <span style={{ background: '#30BDEC', color: 'white', padding: '2px 8px', borderRadius: 4, fontSize: 9, fontWeight: 700, fontFamily: 'Roboto' }}>ADMIN</span>
          <button className="btn btn-dark btn-sm" onClick={logout}>Logout</button>
        </div>
      </header>

      {/* Tabs */}
      <div style={{ background: '#131313', padding: '0 28px', display: 'flex', gap: 4, borderBottom: '1px solid #222', fontFamily: 'Roboto' }}>
        {TABS.map(([k,v]) => (
          <button key={k} onClick={() => setTab(k)} style={{
            padding: '14px 20px', background: 'none', border: 'none', cursor: 'pointer',
            fontSize: 12, fontFamily: 'Roboto', fontWeight: 700,
            color: tab === k ? '#30BDEC' : '#444',
            borderBottom: tab === k ? '2px solid #30BDEC' : '2px solid transparent', letterSpacing: 1
          }}>{k === 'overview' ? <><FontAwesomeIcon icon={faChartBar} style={{color: '#30BDEC', marginRight: 8}}/>{v}</> : k === 'bookings' ? <><FontAwesomeIcon icon={faBox} style={{color: '#30BDEC', marginRight: 8}}/>{v}</> : k === 'transactions' ? <><FontAwesomeIcon icon={faMoneyBill} style={{color: '#30BDEC', marginRight: 8}}/>{v}</> : <><FontAwesomeIcon icon={faUsers} style={{color: '#30BDEC', marginRight: 8}}/>{v}</>}</button>
        ))}
      </div>

      <div style={{ padding: '28px', maxWidth: 1100, margin: '0 auto' }}>
        {/* Overview */}
        {tab === 'overview' && (
          <div className="fade-up">
            <h2 style={{ color: '#30BDEC', marginBottom: 24, fontSize: 14, letterSpacing: 2, textTransform: 'uppercase', fontFamily: 'Roboto' }}>Dashboard Overview</h2>
            {stats ? (
              <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(180px, 1fr))', gap: 16, marginBottom: 32 }}>
                {[
                  { label: 'Total Clients', value: stats.total_users, icon: faUsers },
                  { label: 'Total Bookings', value: stats.total_bookings, icon: faBox },
                  { label: 'Active Convoys', value: stats.active_bookings, icon: faTruck },
                  { label: 'Revenue (Paid)', value: `K${parseInt(stats.total_revenue).toLocaleString()}`, icon: faMoneyBill },
                  { label: 'Pending Revenue', value: `K${parseInt(stats.pending_revenue).toLocaleString()}`, icon: faHourglassEnd },
                ].map(s => (
                  <div className="stat-card" key={s.label}>
                    <div style={{ fontSize: 24, marginBottom: 8, color: '#30BDEC' }}><FontAwesomeIcon icon={s.icon} /></div>
                    <div className="stat-value">{s.value}</div>
                    <div className="stat-label">{s.label}</div>
                  </div>
                ))}
              </div>
            ) : <div className="spinner" />}

              <div style={{ background: '#1D2429', borderRadius: 12, border: '1px solid #333', padding: 20, fontFamily: 'Roboto' }}>
              <p className="section-label">Quick Actions</p>
              <div style={{ display: 'flex', gap: 12, flexWrap: 'wrap' }}>
                <button className="btn btn-gold btn-sm" onClick={() => setTab('bookings')}>Manage Bookings</button>
                <button className="btn btn-dark btn-sm" onClick={() => setTab('transactions')}>View Transactions</button>
                <button className="btn btn-dark btn-sm" onClick={() => setTab('users')}>View Clients</button>
              </div>
            </div>
          </div>
        )}

        {/* Bookings */}
        {tab === 'bookings' && (
          <div className="fade-up">
            <h2 style={{ color: '#30BDEC', marginBottom: 20, fontSize: 14, letterSpacing: 2, textTransform: 'uppercase', fontFamily: 'Roboto' }}>All Bookings</h2>
            {loading ? <div className="spinner" /> : (
              <div style={{ background: '#1a1a1a', borderRadius: 12, border: '1px solid #333', overflow: 'hidden' }}>
                <div className="table-wrap">
                  <table style={{ color: '#ddd' }}>
                    <thead>
                      <tr><th>Ref</th><th>Client</th><th>Asset</th><th>Hub</th><th>Units</th><th>Days</th><th>Total</th><th>Status</th><th>Actions</th></tr>
                    </thead>
                    <tbody>
                      {bookings.map(b => (
                        <tr key={b.id} style={{ borderBottom: '1px solid #222' }}>
                          <td className="mono" style={{ color: '#30BDEC', fontSize: 11 }}>{b.booking_ref}</td>
                          <td>
                            <div style={{ fontSize: 12, fontWeight: 700 }}>{b.full_name || '—'}</div>
                            <div style={{ fontSize: 10, color: '#666' }}>{b.email}</div>
                          </td>
                          <td style={{ fontSize: 11 }}>{b.truck_type}</td>
                          <td style={{ textTransform: 'capitalize', fontSize: 11 }}>{b.hub}</td>
                          <td style={{ textAlign: 'center' }}>{b.units}</td>
                          <td style={{ textAlign: 'center' }}>{b.days}</td>
                          <td className="mono" style={{ fontWeight: 700, color: '#30BDEC' }}>K{parseInt(b.total_amount).toLocaleString()}</td>
                          <td><span className={`badge badge-${b.status}`}>{b.status}</span></td>
                          <td>
                            <div style={{ display: 'flex', gap: 6, flexWrap: 'wrap' }}>
                              {b.status === 'pending' && <button className="btn btn-success btn-sm" onClick={() => updateBookingStatus(b.id, 'active')}>Activate</button>}
                              {b.status === 'active' && <button className="btn btn-sm" style={{ background: '#3498db', color: 'white', fontSize: 11 }} onClick={() => updateBookingStatus(b.id, 'completed')}>Complete</button>}
                              {['pending','active'].includes(b.status) && <button className="btn btn-danger btn-sm" onClick={() => updateBookingStatus(b.id, 'cancelled')}>Cancel</button>}
                            </div>
                          </td>
                        </tr>
                      ))}
                    </tbody>
                  </table>
                </div>
                {bookings.length === 0 && <p style={{ textAlign: 'center', padding: '30px', color: '#555' }}>No bookings yet.</p>}
              </div>
            )}
          </div>
        )}

        {/* Transactions */}
        {tab === 'transactions' && (
          <div className="fade-up">
            <h2 style={{ color: '#30BDEC', marginBottom: 20, fontSize: 14, letterSpacing: 2, textTransform: 'uppercase', fontFamily: 'Roboto' }}>All Transactions</h2>
            {loading ? <div className="spinner" /> : (
              <div style={{ background: '#1a1a1a', borderRadius: 12, border: '1px solid #333', overflow: 'hidden' }}>
                <div className="table-wrap">
                  <table style={{ color: '#ddd' }}>
                    <thead>
                      <tr><th>Booking Ref</th><th>Client</th><th>Asset</th><th>Amount</th><th>Payment</th><th>Status</th><th>Actions</th></tr>
                    </thead>
                    <tbody>
                      {transactions.map(t => (
                        <tr key={t.id} style={{ borderBottom: '1px solid #222' }}>
                          <td className="mono" style={{ color: '#30BDEC', fontSize: 11 }}>{t.booking_ref}</td>
                          <td style={{ fontSize: 12 }}>{t.full_name || t.email}</td>
                          <td style={{ fontSize: 11 }}>{t.truck_type}</td>
                          <td className="mono" style={{ fontWeight: 700, color: '#27ae60' }}>K{parseInt(t.amount).toLocaleString()}</td>
                          <td style={{ fontSize: 11 }}>{t.payment_method}</td>
                          <td><span className={`badge badge-${t.status}`}>{t.status}</span></td>
                          <td>
                            {t.status === 'pending' && (
                              <button className="btn btn-success btn-sm" onClick={() => updatePayment(t.booking_id, 'paid')}>Mark Paid</button>
                            )}
                          </td>
                        </tr>
                      ))}
                    </tbody>
                  </table>
                </div>
                {transactions.length === 0 && <p style={{ textAlign: 'center', padding: '30px', color: '#555' }}>No transactions yet.</p>}
              </div>
            )}
          </div>
        )}

        {/* Users */}
        {tab === 'users' && (
          <div className="fade-up">
            <h2 style={{ color: '#30BDEC', marginBottom: 20, fontSize: 14, letterSpacing: 2, textTransform: 'uppercase', fontFamily: 'Roboto' }}>Registered Clients</h2>
            {loading ? <div className="spinner" /> : (
              <div style={{ background: '#1a1a1a', borderRadius: 12, border: '1px solid #333', overflow: 'hidden' }}>
                <div className="table-wrap">
                  <table style={{ color: '#ddd' }}>
                    <thead>
                      <tr><th>Name</th><th>Email</th><th>Phone</th><th>Company</th><th>Role</th><th>Joined</th></tr>
                    </thead>
                    <tbody>
                      {users.map(u => (
                        <tr key={u.id} style={{ borderBottom: '1px solid #222' }}>
                          <td style={{ fontWeight: 700, fontSize: 13 }}>{u.full_name || '—'}</td>
                          <td style={{ fontSize: 12 }}>{u.email}</td>
                          <td className="mono" style={{ fontSize: 11 }}>{u.phone}</td>
                          <td style={{ fontSize: 12 }}>{u.company || '—'}</td>
                          <td><span style={{ background: u.role === 'admin' ? '#30BDEC' : '#333', color: u.role === 'admin' ? 'white' : '#888', padding: '2px 8px', borderRadius: 4, fontSize: 9, fontWeight: 700, fontFamily: 'Roboto' }}>{u.role.toUpperCase()}</span></td>
                          <td style={{ fontSize: 11, color: '#666' }}>{new Date(u.created_at).toLocaleDateString()}</td>
                        </tr>
                      ))}
                    </tbody>
                  </table>
                </div>
              </div>
            )}
          </div>
        )}
      </div>
    </div>
  );
}
