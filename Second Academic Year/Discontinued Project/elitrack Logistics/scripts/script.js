  const hubs = {
        "kitwe": [-12.8167, 28.2000], "ndola": [-12.9667, 28.6333],
        "solwezi": [-12.1833, 26.4000], "chingola": [-12.5333, 27.8500]
    };

    let map; 
    let fleet = {}; 
    let activeTruckId = null;
    let trackingInterval;

    function login() {
        if(document.getElementById('userEmail').value && document.getElementById('userPhone').value) {
            document.getElementById('auth-overlay').style.display = 'none';
            document.getElementById('app').style.display = 'block';
        } else {
            alert("Enter valid credentials.");
        }
    }

    function calc() {
        let truckPrice = parseInt(document.getElementById('truck').value);
        let units = parseInt(document.getElementById('units').value) || 1;
        let days = parseInt(document.getElementById('days').value) || 1;
        let secPrice = parseInt(document.getElementById('sec_tier').value);
        let grandTotal = (truckPrice * units * days) + secPrice;
        document.getElementById('total').innerText = "K" + grandTotal.toLocaleString();
    }

    function processBooking() {
        const cityKey = document.getElementById('loc').value;
        const truckType = document.getElementById('truck').options[document.getElementById('truck').selectedIndex].text.split(' (')[0];
        const unitCount = parseInt(document.getElementById('units').value) || 1;
        
        document.getElementById('track-section').style.display = 'block';
        document.getElementById('map').style.display = 'block';
        document.getElementById('fleet-list-section').style.display = 'block';
        document.getElementById('live-telematics').style.display = 'block';

        if (!map) {
            map = L.map('map').setView(hubs[cityKey], 12);
            L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png').addTo(map);
        }

        // Reset Fleet
        for (let id in fleet) { map.removeLayer(fleet[id].marker); }
        fleet = {};
        document.getElementById('fleet-container').innerHTML = "";

        // Generate Multiple Units
        for(let i = 1; i <= unitCount; i++) {
            let tId = `TL-${100 + i}`;
            let offset = i * 0.005;
            let pos = [hubs[cityKey][0] + offset, hubs[cityKey][1] + offset];
            
            let m = L.marker(pos).addTo(map).bindPopup(`<b>${tId}</b>: ${truckType}`);
            
            fleet[tId] = { id: tId, type: truckType, marker: m };

            // Add to UI List
            let div = document.createElement('div');
            div.className = 'fleet-item';
            div.id = `item-${tId}`;
            div.innerHTML = `<span><strong>${tId}</strong></span> <span style="font-size:10px; color:var(--success)">● TRACKING</span>`;
            div.onclick = () => selectTruck(tId);
            document.getElementById('fleet-container').appendChild(div);
        }

        selectTruck(`TL-101`);
        startGlobalMovement();
        alert(`Convoy of ${unitCount} deployed from ${cityKey.toUpperCase()}`);
    }

    function selectTruck(id) {
        activeTruckId = id;
        
        // Update UI styling
        document.querySelectorAll('.fleet-item').forEach(el => el.classList.remove('active'));
        document.getElementById(`item-${id}`).classList.add('active');
        
        const t = fleet[id];
        document.getElementById('active-asset-id').innerText = id;
        map.flyTo(t.marker.getLatLng(), 15);
        t.marker.openPopup();

        // Simulate Camera Feed Change
        document.getElementById('video-content').innerHTML = "<p style='color:#gold'>Connecting to 24H LTE Cam...</p>";
        setTimeout(() => {
            document.getElementById('video-content').innerHTML = `
                <img src="https://images.unsplash.com/photo-1519003722824-192d992a605b?auto=format&fit=crop&w=500&q=60" style="width:100%; height:100%; object-fit:cover; opacity: 0.5;">
                <div style="position:absolute; color:white; font-size:11px; font-weight:bold; background:rgba(0,0,0,0.5); padding: 5px;">STREAM: ${id} - PRIMARY</div>`;
        }, 1200);
    }

    function startGlobalMovement() {
        if (trackingInterval) clearInterval(trackingInterval);
        trackingInterval = setInterval(() => {
            for (let id in fleet) {
                let t = fleet[id];
                let lat = t.marker.getLatLng().lat + (Math.random() - 0.5) * 0.001;
                let lng = t.marker.getLatLng().lng + (Math.random() - 0.5) * 0.001;
                t.marker.setLatLng([lat, lng]);

                if (id === activeTruckId) {
                    document.getElementById('live-speed').innerText = Math.floor(Math.random() * 20 + 45) + " km/h";
                }
            }
        }, 4000);
    }