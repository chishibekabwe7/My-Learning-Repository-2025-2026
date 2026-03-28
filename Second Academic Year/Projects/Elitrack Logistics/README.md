# Elitrack Logistics

**Elitrack Logistics** is a small web app for booking and (simulated) tracking truck/convoy deployments, with a client portal and an admin dashboard for managing bookings and payments.

## What it does

### Client side (Dashboard)

- **Sign in** using email/password or Google sign-in.
- **Book a convoy** by selecting:
  - vehicle type
  - number of units
  - contract days
  - deployment hub (Kitwe / Ndola / Solwezi / Chingola)
  - security tier
- **Creates a booking** and a **pending transaction** for the total amount.
- **Live tracking view** showing a map and “telematics” UI:
  - GPS positions and speed are **simulated** (demo data) to represent live movement.
- **My bookings** list to view previous bookings and their statuses.

### Admin side (Admin Dashboard)

- **Overview stats** (clients, bookings, active convoys, paid vs pending revenue).
- **Manage bookings**:
  - view all bookings
  - update booking status (activate / complete / cancel)
- **Manage transactions**:
  - mark payments as paid (manual)
- **View users** (registered clients list).

## What to improve next

- **Automated testing** for both client and server (unit + integration) to protect key flows like auth, booking, and admin updates.
- **Real payment integration** (instead of manual paid-status updates) with webhook verification and transaction audit trails.
- **Production-grade tracking** by replacing simulated telematics with actual GPS/IoT data ingestion and map playback history.
- **User notifications** (email/SMS/in-app) for booking status changes, payment confirmations, and convoy milestones.
- **Role and permission hardening** (finer-grained admin actions, stronger audit visibility, and safer operational controls).
- **Deployment/DevOps polish** (CI checks, environment validation, and repeatable deploy pipeline for client + server).
