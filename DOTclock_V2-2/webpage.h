
#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>DOTclock</title>
    <link rel="stylesheet" href="styles.css">
</head>
<body>
    <div class="container">
        <h1 class="title">DOTclock</h1>
        
        <!-- Text Display -->
        <div class = "text">
            <div class="card">
                <h2 class="section-title text-color">Custom Text</h2>
                <div class="text-container">
                    <input type="text" id="textInput" placeholder="Opsional :v" />
                </div>
                <button class="submit-btn" onclick="saveCustomText()">Kirim</button>
            </div>
        </div>

        <!-- Sensor Display -->
        <div class="grid-2">
            <div class="card">
                <svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" color="#F33535">
                    <path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"/>
                </svg>
                <div class="label temp-color">Temp</div>
                <div class="dht-container">
                    <span class="value temp-color" id="sensor1">-</span>
                </div>
            </div>

            <div class="card">
                <svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" color = "#0C74DA">
                    <path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"/>
                </svg>
                <div class="label humidity-color">Humidity</div>
                <div class="dht-container">
                    <span class="value humidity-color" id="sensor2">-</span>
                </div>
            </div>
        </div>

        <!-- Waktu & Kalender -->
        <div class="card">
            <h2 class="section-title">Setting Waktu</h2>
            <div class="grid-2 mb-4">
                <button class="icon-button" onclick="openModal('timeModal')">
                    <svg class="icon-lg" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" color="#F4991A">
                        <circle cx="12" cy="12" r="10"/>
                        <polyline points="12 6 12 12 16 14"/>
                    </svg>
                    <span class="clock-color">Jam</span>
                </button>

                <button class="icon-button" onclick="openModal('dateModal')">
                    <svg class="icon-lg" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" color="#004030">
                        <rect x="3" y="4" width="18" height="18" rx="2" ry="2"/>
                        <line x1="16" y1="2" x2="16" y2="6"/>
                        <line x1="8" y1="2" x2="8" y2="6"/>
                        <line x1="3" y1="10" x2="21" y2="10"/>
                    </svg>
                    <span class="calendar-color">Kalender</span>
                </button>
            </div>

            <!-- Tombol Kirim Data Waktu -->
            <button class="submit-btn" onclick="saveDateTime()">Kirim</button>
        </div>

        <!-- Alarm -->
        <div class="card setting-alarm-card">
            <h2 class="section-title">Setting Alarm</h2>
            <div class="alarm-container">
                <button class="icon-button alarm-btn" onclick="openModal('alarmModal')">
                    <svg class="icon-lg" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" color="#B6771D">
                        <circle cx="12" cy="13" r="8"/>
                        <path d="M12 9v4l2 2"/>
                        <path d="M5 3L2 6"/>
                        <path d="M22 6l-3-3"/>
                        <path d="M6 19l-2 2"/>
                        <path d="M18 19l2 2"/>
                    </svg>
                    <span class="alarm-color">Alarm</span>
                </button>

                <div class="alarm-list" id="alarmList"></div>
            </div>

            <!-- Tombol Kirim Data Alarm -->
            <button class="submit-btn" onclick="saveAlarms()">Kirim</button>
        </div>
    </div>

    <!-- Modal Jam -->
    <div id="timeModal" class="modal">
        <div class="modal-content">
            <button class="close-btn" onclick="closeModal('timeModal')">&times;</button>
            <h2 class="modal-title">Atur Jam</h2>
            <form onsubmit="submitTime(event)">
                <input type="time" id="timeInput" class="time-input" required>
                <button type="submit" class="submit-btn">Simpan</button>
            </form>
        </div>
    </div>

    <!-- Modal Tanggal -->
    <div id="dateModal" class="modal">
        <div class="modal-content">
            <button class="close-btn" onclick="closeModal('dateModal')">&times;</button>
            <h2 class="modal-title">Atur Tanggal</h2>
            <form onsubmit="submitDate(event)">
                <input type="date" id="dateInput" class="time-input" required>
                <button type="submit" class="submit-btn">Simpan</button>
            </form>
        </div>
    </div>

    <!-- Modal Alarm -->
    <div id="alarmModal" class="modal">
        <div class="modal-content">
            <button class="close-btn" onclick="closeModal('alarmModal')">&times;</button>
            <h2 class="modal-title" id="alarmModalTitle">Tambah Alarm</h2>
            <form onsubmit="submitAlarm(event)">
                <input type="time" id="alarmInput" class="time-input" required>
                <button type="submit" class="submit-btn">Simpan</button>
            </form>
        </div>
    </div>

    <!-- Tanpa Supabase -->
    <script src="suhu.js"></script>
    <script src="app.js"></script>
</body>
</html>
)rawliteral";

const char STYLES_CSS[] PROGMEM = R"rawliteral(
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    font-family: system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
}

body {
    min-height: 100vh;
    background: #F0F0F0;
    display: flex;
    align-items: center;
    justify-content: center;
    padding: 1rem;
}

.container {
    width: 100%;
    max-width: 28rem;
}

.title {
    color: #171717;
    font-size: 4rem;
    text-align: center;
    margin-bottom: 2rem;
}

.grid-2 {
    display: grid;
    grid-template-columns: repeat(2, 1fr);
    gap: 1rem;
    margin-bottom: 1.5rem;
}

.card {
    background: #F0F0F0;
    border-radius: 1rem;
    padding: 1.5rem;
    border: 8px solid #423F3E;
}

.icon {
    width: 2rem;
    height: 2rem;
    margin: 0 auto 0.5rem;
    display: block;
}

.icon-lg {
    width: 2.5rem;
    height: 2.5rem;
    margin-bottom: 0.5rem;
}

.label {
    font-size: 2rem;
    font-weight:bold;
    text-align: center;
    margin-bottom: 0.25rem;
}

.value {
    font-size: 3rem;
    text-align: center;
}

.temp-color {
    color: #F33535;
}

.text-color{
    color:#34656D
}

.humidity-color {
    color: #0C74DA;
}

.clock-color {
    color: #F4991A;
}

.calendar-color {
    color: #004030;
}

.alarm-color {
    color: #B6771D;
}

.section-title {
    color: #171717;
    font-size: 1.5rem;
    text-align: center;
    margin-bottom: 1rem;
}

.mb-4 {
    margin-bottom: 1rem;
}

.icon-button {
    background: #F0F0F0;
    border: 6px solid #423F3E;
    border-radius: 0.75rem;
    padding: 1rem;
    cursor: pointer;
    transition: background 0.3s;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
}

.icon-button:hover {
    background: #4b5563;
}

.icon-button span {
    font-size: 1.125rem;
}

.submit-btn {
    width: 100%;
    background: #423F3E;
    color: #F9F5F0;
    border: none;
    border-radius: 0.75rem;
    padding: 0.75rem;
    font-size: 1.125rem;
    cursor: pointer;
    transition: background 0.3s;
}

.submit-btn:hover {
    background: #4b5563;
}
.setting-alarm-card {
    margin-top: 1.5rem;
}
.dht-container{
    display: flex;
    align-items: center;
    justify-content: center;
}
.text{
    margin-bottom: 1.5rem;
    color:#34656D;
}

.text-container{
    padding: 10px;
    display:flex;
    gap: 1rem;
    align-items: center;
    justify-content: center;
    margin-bottom: 1rem;
}
#textInput {
    font-size: 15px;
    border-radius: 10px;     
    padding: 12px 16px;   
    height: 50px;         
}

.alarm-container {
    display: flex;
    gap: 1rem;
    align-items: flex-start;
    margin-bottom: 1rem;
}

.alarm-btn {
    flex-shrink: 0;
}

.alarm-list {
    flex: 1;
    max-height: 10rem;
    overflow-y: auto;
}

.alarm-item {
    display: flex;
    align-items: center;
    justify-content: space-between;
    background: #423F3E;
    border-radius: 0.5rem;
    padding: 0.5rem 1rem;
    margin-bottom: 0.5rem;
}

.alarm-time {
    color: #F9F5F0;
    background: none;
    border: none;
    cursor: pointer;
    font-size: 0.875rem;
    font-family: system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
    transition: color 0.3s;
}

.alarm-time:hover {
    color: #F4CE14;
}

.delete-btn {
    background: none;
    border: none;
    color: #9ca3af;
    cursor: pointer;
    padding: 0.25rem;
    transition: color 0.3s;
}

.delete-btn:hover {
    color: #ef4444;
}

.modal {
    display: none;
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background: rgba(0, 0, 0, 0.5);
    align-items: center;
    justify-content: center;
    z-index: 1000;
    padding: 1rem;
}

.modal.active {
    display: flex;
}

.modal-content {
    background: #2B2B2B;
    border-radius: 1rem;
    padding: 1.5rem;
    width: 100%;
    max-width: 24rem;
    position: relative;
    border: 2px solid #423F3E;
}

.close-btn {
    position: absolute;
    top: 1rem;
    right: 1rem;
    background: none;
    border: none;
    color: #F9F5F0;
    font-size: 2rem;
    cursor: pointer;
    line-height: 1;
    transition: color 0.3s;
}

.close-btn:hover {
    color: #F9F5F0;
}

.modal-title {
    color: #F9F5F0;
    font-size: 1.5rem;
    text-align: center;
    margin-bottom: 1.5rem;
}

.time-input {
    width: 100%;
    background: #2B2B2B;
    color: #F9F5F0;
    border: 2px solid #423F3E;
    border-radius: 0.75rem;
    padding: 1rem;
    font-size: 1.25rem;
    text-align: center;
    margin-bottom: 1.5rem;
    font-family: system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
}

.time-input:focus {
    outline: none;
    border-color: #F9F5F0;
}

.alarm-list::-webkit-scrollbar {
    width: 0.5rem;
}

.alarm-list::-webkit-scrollbar-track {
    background: #1f2937;
    border-radius: 0.25rem;
}

.alarm-list::-webkit-scrollbar-thumb {
    background: #4b5563;
    border-radius: 0.25rem;
}

.alarm-list::-webkit-scrollbar-thumb:hover {
    background: #6b7280;
}

@media (max-width: 640px) {
    .title {
        font-size: 2.5rem;
    }

    .value {
        font-size: 2rem;
    }
}

@media (max-width: 400px) {
    .title {
        font-size: 2rem;
    }

    .value {
        font-size: 1.75rem;
    }

    .label {
        font-size: 1rem;
    }

    .icon-button span {
        font-size: 1rem;
    }
}
)rawliteral";

const char SUHU_JS[] PROGMEM = R"rawliteral(
// suhu.js - polling sensor tiap 1s
const refresh_time = 5000;

async function load_sensor() {
  try {
    const response = await fetch('/getsensor');
    if (!response.ok) throw new Error('Network error!');
    const data = await response.json();

    const mapping = {
      sensor1: (data.temp!=null)? (parseFloat(data.temp).toFixed(1) + ' °C') : '-',
      sensor2: (data.hum!=null)? (parseFloat(data.hum).toFixed(0) + ' %') : '-'
    };

    Object.entries(mapping).forEach(([id, value]) => {
      const element = document.getElementById(id);
      if (element) element.textContent = value ?? '-';
    });
  } catch (error) {
    console.error('Error loading sensors:', error);
  }
}

document.addEventListener('DOMContentLoaded', function () {
  load_sensor();
  setInterval(load_sensor, refresh_time);
});
)rawliteral";

const char APP_JS[] PROGMEM = R"rawliteral(
// app.js - UI handling, localStorage, send time, text & alarms
const refreshMs = 1000; // polling interval (ms)

function saveData(key, value) {
  try { localStorage.setItem(key, JSON.stringify(value)); } catch(e){}
}
function loadData(key, fallback = null) {
  try { const s = localStorage.getItem(key); return s? JSON.parse(s) : fallback; } catch(e){ return fallback; }
}

function openModal(id){ 
  const m = document.getElementById(id); 
  if(!m) return; 
  m.classList.add('active'); 
  const now=new Date();
  if(id==='timeModal') document.getElementById('timeInput').value = now.toTimeString().slice(0,5);
  if(id==='dateModal') document.getElementById('dateInput').value = now.toISOString().slice(0,10);
  if(id==='alarmModal'){ 
    document.getElementById('alarmInput').value = now.toTimeString().slice(0,5);
    document.getElementById('alarmModalTitle').textContent='Tambah Alarm';
  }
}
function closeModal(id){ 
  const m=document.getElementById(id); 
  if(!m) return; 
  m.classList.remove('active'); 
}

function submitTime(e){ 
  e.preventDefault(); 
  const t=document.getElementById('timeInput').value; 
  let d=loadData('time_settings')||{}; 
  d.custom_time=t; 
  d.updated_at=new Date().toISOString(); 
  saveData('time_settings', d); 
  closeModal('timeModal'); 
}
function submitDate(e){ 
  e.preventDefault(); 
  const dt=document.getElementById('dateInput').value; 
  let d=loadData('time_settings')||{}; 
  d.custom_date=dt; 
  d.updated_at=new Date().toISOString(); 
  saveData('time_settings', d); 
  closeModal('dateModal'); 
}

let editingAlarmId = null;
function submitAlarm(e){
  e.preventDefault();
  const t=document.getElementById('alarmInput').value;
  let alarms = loadData('alarms')||[];
  if(editingAlarmId){
    alarms = alarms.map(a => a.id===editingAlarmId ? {...a, alarm_time:t, updated_at:new Date().toISOString()} : a);
  } else {
    alarms.push({ id: Date.now(), alarm_time: t, is_active: true, created_at: new Date().toISOString() });
  }
  saveData('alarms', alarms);
  editingAlarmId=null;
  closeModal('alarmModal');
  loadAlarms();
}

function loadAlarms(){
  const alarms = loadData('alarms') || [];
  const el = document.getElementById('alarmList');
  if(!el) return;
  el.innerHTML = '';
  alarms.sort((a,b)=>a.alarm_time.localeCompare(b.alarm_time)).forEach(alarm => {
    const item = document.createElement('div'); item.className='alarm-item';
    const btn = document.createElement('button'); btn.className='alarm-time'; btn.textContent = alarm.alarm_time.slice(0,5);
    btn.onclick = ()=> editAlarm(alarm.id, alarm.alarm_time);
    const del = document.createElement('button'); del.className='delete-btn'; del.innerHTML='🗑';
    del.onclick = ()=> { deleteAlarm(alarm.id); };
    item.appendChild(btn); item.appendChild(del);
    el.appendChild(item);
  });
}

function editAlarm(id, time){ editingAlarmId = id; document.getElementById('alarmInput').value = time.slice(0,5); openModal('alarmModal'); }
function deleteAlarm(id){ let alarms = loadData('alarms')||[]; alarms = alarms.filter(a=>a.id!==id); saveData('alarms', alarms); loadAlarms(); }

async function sendTimeToESP(payload){
  try{
    const r = await fetch('/set_time', { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify(payload) });
    if(!r.ok) throw new Error('HTTP '+r.status);
    return true;
  }catch(e){ console.error('sendTimeToESP', e); return false; }
}

async function sendAlarmsToESP(alarms){
  try{
    const r = await fetch('/set_alarms', { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify({alarms}) });
    if(!r.ok) throw new Error('HTTP '+r.status);
    return true;
  }catch(e){ console.error('sendAlarmsToESP', e); return false; }
}

async function sendTextToESP(payload){
  try{
    const r = await fetch('/set_text', { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify(payload) });
    if(!r.ok) throw new Error('HTTP '+r.status);
    return true;
  }catch(e){ console.error('sendTextToESP', e); return false; }
}

async function saveDateTime(){
  const now = new Date();
  const customTime = document.getElementById("timeInput").value || now.toTimeString().slice(0,5);
  const customDate = document.getElementById("dateInput").value || now.toISOString().slice(0,10);
  const payload = { custom_time: customTime, custom_date: customDate, updated_at: now.toISOString() };
  saveData("time_settings", payload);
  const ok = await sendTimeToESP(payload);
  alert(ok? 'Waktu dikirim ke DOTclock ;)' : 'Gagal kirim waktu :(');
}

async function saveAlarms(){
  const alarms = loadData('alarms') || [];
  saveData('alarms', alarms);
  const ok = await sendAlarmsToESP(alarms);
  alert(ok? 'Alarm dikirim ke DOTclock ;)' : 'Gagal kirim alarm :(');
}

async function saveCustomText(){
  const now = new Date();
  const inputEl = document.getElementById('textInput');
  if(!inputEl) { console.error('textInput not found'); return; }
  // ambil value, jika kosong -> simpan string kosong (opsional)
  const textValue = (inputEl.value||'').trim(); // kosong jika tidak diisi
  const payload = { custom_text: textValue, updated_at: now.toISOString() };
  saveData('text_settings', payload);
  const ok = await sendTextToESP(payload);
  alert(ok? 'Teks dikirim ke DOTclock ;)' : 'Teks tersimpan lokal, gagal kirim ke DOTclock :(');
}

document.addEventListener('DOMContentLoaded', ()=>{
  const ts = loadData('text_settings') || {};
  const textInputEl = document.getElementById('textInput');
  if(textInputEl && ts.custom_text !== undefined){
    textInputEl.value = ts.custom_text || '';
  }

  loadAlarms();
  
  setInterval(()=>{ /* optional: fetch('/getsensor') every 5s if you want */ }, refreshMs);
});
)rawliteral";

#endif 
