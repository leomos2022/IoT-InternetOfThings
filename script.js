// ========================================
// CONFIGURACIÓN Y CONSTANTES
// ========================================

const CONFIG = {
    updateInterval: 2000, // Actualización cada 2 segundos
    energyMax: 150, // kW máximo
    energyMin: 80, // kW mínimo
    tempNormal: 28, // °C normal
    tempMax: 40, // °C máximo
    vibrationNormal: 600, // unidades normal
    vibrationMax: 1000, // unidades máximo
    currentNormal: 900, // unidades ADC normal
    currentMax: 1023, // unidades ADC máximo
    healthThresholds: {
        excellent: 90,
        good: 75,
        warning: 60,
        critical: 40
    }
};

// ========================================
// ESTADO GLOBAL
// ========================================

let state = {
    energy: {
        current: 0,
        savings: 0,
        history: []
    },
    maintenance: {
        temperature: 0,
        vibration: 0,
        healthScore: 100,
        alerts: []
    },
    sensors: {
        temperature: 0,
        vibration: 0,
        current: 0
    }
};

// ========================================
// GRÁFICOS CON CHART.JS
// ========================================

let energyChart, healthChart;

function initCharts() {
    // Gráfico de Consumo Energético
    const energyCtx = document.getElementById('energyChart').getContext('2d');
    energyChart = new Chart(energyCtx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [{
                label: 'Consumo (kW)',
                data: [],
                borderColor: '#f59e0b',
                backgroundColor: 'rgba(245, 158, 11, 0.1)',
                tension: 0.4,
                fill: true,
                borderWidth: 2,
                pointRadius: 3,
                pointHoverRadius: 5
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                legend: {
                    labels: { 
                        color: '#cbd5e1',
                        font: { size: 12 }
                    }
                }
            },
            scales: {
                y: {
                    beginAtZero: true,
                    max: CONFIG.energyMax,
                    ticks: { 
                        color: '#94a3b8',
                        font: { size: 11 }
                    },
                    grid: { color: 'rgba(148, 163, 184, 0.1)' }
                },
                x: {
                    ticks: { 
                        color: '#94a3b8',
                        font: { size: 10 },
                        maxRotation: 45,
                        minRotation: 45
                    },
                    grid: { color: 'rgba(148, 163, 184, 0.1)' }
                }
            }
        }
    });

    // Gráfico de Salud del Equipo
    const healthCtx = document.getElementById('healthChart').getContext('2d');
    healthChart = new Chart(healthCtx, {
        type: 'doughnut',
        data: {
            labels: ['Salud', 'Riesgo'],
            datasets: [{
                data: [100, 0],
                backgroundColor: ['#10b981', '#ef4444'],
                borderWidth: 0,
                borderRadius: 8
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            cutout: '70%',
            plugins: {
                legend: {
                    position: 'bottom',
                    labels: { 
                        color: '#cbd5e1',
                        font: { size: 12 },
                        padding: 15
                    }
                }
            }
        }
    });
}

// ========================================
// SIMULACIÓN DE DATOS IoT
// ========================================

function generateEnergyData() {
    // Simula consumo energético con variaciones realistas
    const hour = new Date().getHours();
    let baseConsumption = CONFIG.energyMin;
    
    // Mayor consumo durante horas productivas (8-18h)
    if (hour >= 8 && hour <= 18) {
        baseConsumption = CONFIG.energyMax * 0.85;
    } else if (hour >= 6 && hour < 8 || hour > 18 && hour <= 20) {
        baseConsumption = CONFIG.energyMax * 0.6;
    } else {
        baseConsumption = CONFIG.energyMax * 0.4;
    }
    
    // Añade variación aleatoria ±10%
    const variation = (Math.random() - 0.5) * 0.2;
    const current = baseConsumption * (1 + variation);
    
    // Calcula ahorro vs consumo promedio
    const averageConsumption = (CONFIG.energyMax + CONFIG.energyMin) / 2;
    const savings = ((averageConsumption - current) / averageConsumption) * 100;
    
    return {
        current: Math.round(current * 10) / 10,
        savings: Math.max(0, Math.round(savings))
    };
}

function generateMaintenanceData() {
    // Simula temperatura del motor
    const tempBase = CONFIG.tempNormal;
    const tempVariation = (Math.random() - 0.3) * 8;
    const temperature = Math.max(25, Math.min(CONFIG.tempMax, tempBase + tempVariation));
    
    // Simula vibración del equipo
    const vibBase = CONFIG.vibrationNormal;
    const vibVariation = (Math.random() - 0.4) * 200;
    const vibration = Math.max(500, Math.min(CONFIG.vibrationMax, vibBase + vibVariation));
    
    // Calcula score de salud basado en temperatura y vibración
    let healthScore = 100;
    
    // Penalización por temperatura
    if (temperature > 35) {
        healthScore -= (temperature - 35) * 3;
    }
    
    // Penalización por vibración
    if (vibration > 800) {
        healthScore -= (vibration - 800) * 0.1;
    }
    
    healthScore = Math.max(0, Math.min(100, Math.round(healthScore)));
    
    return {
        temperature: Math.round(temperature * 10) / 10,
        vibration: Math.round(vibration),
        healthScore
    };
}

function generateSensorData() {
    // Simula lecturas de sensores
    const tempVariation = (Math.random() - 0.5) * 4;
    const temperature = Math.round((CONFIG.tempNormal + tempVariation) * 10) / 10;
    
    const vibVariation = (Math.random() - 0.5) * 100;
    const vibration = Math.round(CONFIG.vibrationNormal + vibVariation);
    
    const currentVariation = (Math.random() - 0.5) * 100;
    const current = Math.round(CONFIG.currentNormal + currentVariation);
    
    return { temperature, vibration, current };
}

// ========================================
// ACTUALIZACIÓN DE INTERFAZ
// ========================================

function updateEnergyDisplay() {
    const data = generateEnergyData();
    state.energy.current = data.current;
    state.energy.savings = data.savings;
    
    // Actualiza valores en pantalla
    document.getElementById('energy-current').textContent = data.current;
    document.getElementById('energy-savings').textContent = data.savings;
    
    // Actualiza barra de progreso
    const progressPercent = (data.current / CONFIG.energyMax) * 100;
    document.getElementById('energy-progress').style.width = `${progressPercent}%`;
    
    // Actualiza gráfico
    const now = new Date().toLocaleTimeString();
    if (energyChart.data.labels.length > 20) {
        energyChart.data.labels.shift();
        energyChart.data.datasets[0].data.shift();
    }
    energyChart.data.labels.push(now);
    energyChart.data.datasets[0].data.push(data.current);
    energyChart.update('none');
    
    // Log evento
    addLog('success', `Consumo energético: ${data.current} kW (Ahorro: ${data.savings}%)`);
}

function updateMaintenanceDisplay() {
    const data = generateMaintenanceData();
    state.maintenance = { ...state.maintenance, ...data };
    
    // Actualiza valores
    document.getElementById('health-score').textContent = data.healthScore;
    document.getElementById('temp-value').textContent = data.temperature;
    document.getElementById('vibration-value').textContent = data.vibration;
    
    // Actualiza badge según salud
    const badge = document.getElementById('maintenance-badge');
    if (data.healthScore >= CONFIG.healthThresholds.excellent) {
        badge.className = 'badge badge-success';
        badge.textContent = 'Excelente';
    } else if (data.healthScore >= CONFIG.healthThresholds.good) {
        badge.className = 'badge badge-info';
        badge.textContent = 'Bueno';
    } else if (data.healthScore >= CONFIG.healthThresholds.warning) {
        badge.className = 'badge badge-warning';
        badge.textContent = 'Advertencia';
    } else {
        badge.className = 'badge badge-danger';
        badge.textContent = 'Crítico';
    }
    
    // Actualiza gráfico de salud
    healthChart.data.datasets[0].data = [data.healthScore, 100 - data.healthScore];
    healthChart.update('none');
    
    // Maneja alertas
    const alertBox = document.getElementById('maintenance-alert');
    const alertMessage = document.getElementById('alert-message');
    
    if (data.temperature > 35 || data.vibration > 800 || data.healthScore < CONFIG.healthThresholds.warning) {
        let alerts = [];
        
        if (data.temperature > 35) {
            alerts.push(`Temperatura elevada: ${data.temperature}°C`);
        }
        if (data.vibration > 800) {
            alerts.push(`Vibración excesiva: ${data.vibration} unidades`);
        }
        if (data.healthScore < CONFIG.healthThresholds.warning) {
            alerts.push(`Salud crítica: ${data.healthScore}%`);
        }
        
        alertMessage.textContent = alerts.join(' | ');
        alertBox.style.display = 'flex';
        
        addLog('warning', `⚠️ ALERTA: ${alerts[0]}`);
    } else {
        alertBox.style.display = 'none';
    }
}

function updateSensorDisplay() {
    const data = generateSensorData();
    state.sensors = data;
    
    // Actualiza sensores integrados
    document.getElementById('sensor-temp').textContent = `${data.temperature}°C`;
    document.getElementById('sensor-vibration').textContent = data.vibration;
    document.getElementById('sensor-current').textContent = data.current;
}

// ========================================
// SISTEMA DE LOGS
// ========================================

function addLog(type, message) {
    const logsContainer = document.getElementById('logs-container');
    const logEntry = document.createElement('div');
    logEntry.className = `log-entry ${type}`;
    
    const time = new Date().toLocaleTimeString();
    logEntry.innerHTML = `
        <span class="log-time">[${time}]</span>
        <span class="log-message">${message}</span>
    `;
    
    logsContainer.insertBefore(logEntry, logsContainer.firstChild);
    
    // Mantiene solo últimos 50 logs
    while (logsContainer.children.length > 50) {
        logsContainer.removeChild(logsContainer.lastChild);
    }
}

// ========================================
// NAVEGACIÓN
// ========================================

function initNavigation() {
    const navLinks = document.querySelectorAll('.nav-link');
    
    // Observador de intersección para activar nav automáticamente
    const observer = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                const id = entry.target.getAttribute('id');
                navLinks.forEach(link => {
                    link.classList.remove('active');
                    if (link.getAttribute('href') === `#${id}`) {
                        link.classList.add('active');
                    }
                });
            }
        });
    }, { threshold: 0.3 });
    
    // Observa todas las secciones
    document.querySelectorAll('section').forEach(section => {
        observer.observe(section);
    });
    
    // Scroll suave al hacer clic
    navLinks.forEach(link => {
        link.addEventListener('click', (e) => {
            e.preventDefault();
            const targetId = link.getAttribute('href');
            const targetSection = document.querySelector(targetId);
            
            if (targetSection) {
                targetSection.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });
}

// ========================================
// ANIMACIONES DE ENTRADA
// ========================================

function initAnimations() {
    const observerOptions = {
        threshold: 0.1,
        rootMargin: '0px 0px -50px 0px'
    };
    
    const fadeInObserver = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.style.opacity = '1';
                entry.target.style.transform = 'translateY(0)';
            }
        });
    }, observerOptions);
    
    // Aplica animación a cards
    document.querySelectorAll('.overview-card, .system-card, .benefit-card').forEach(card => {
        card.style.opacity = '0';
        card.style.transform = 'translateY(20px)';
        card.style.transition = 'opacity 0.6s ease, transform 0.6s ease';
        fadeInObserver.observe(card);
    });
}

// ========================================
// CICLO PRINCIPAL DE ACTUALIZACIÓN
// ========================================

function startSimulation() {
    // Actualización inicial
    updateEnergyDisplay();
    updateMaintenanceDisplay();
    updateSensorDisplay();
    
    addLog('success', '✓ Sistema IoT iniciado correctamente');
    addLog('info', '📡 Sensores conectados y transmitiendo datos');
    
    // Actualización periódica
    setInterval(() => {
        updateEnergyDisplay();
        updateMaintenanceDisplay();
        updateSensorDisplay();
    }, CONFIG.updateInterval);
}

// ========================================
// INICIALIZACIÓN
// ========================================

document.addEventListener('DOMContentLoaded', () => {
    console.log('🏭 Iniciando Sistema IoT - Fábrica Inteligente');
    
    // Inicializa componentes
    initCharts();
    initNavigation();
    initAnimations();
    
    // Inicia simulación después de un breve delay
    setTimeout(() => {
        startSimulation();
    }, 500);
    
    console.log('✓ Sistema cargado correctamente');
});

// ========================================
// MANEJO DE ERRORES
// ========================================

window.addEventListener('error', (e) => {
    console.error('Error en la aplicación:', e);
    addLog('error', `❌ Error: ${e.message}`);
});

// ========================================
// MODAL FUNCTIONS
// ========================================

function openModal(modalId) {
    const modal = document.getElementById(modalId);
    if (modal) {
        modal.classList.add('active');
        document.body.style.overflow = 'hidden'; // Previene scroll del body
        addLog('info', `📖 Visualizando detalles del circuito`);
    }
}

function closeModal(modalId) {
    const modal = document.getElementById(modalId);
    if (modal) {
        modal.classList.remove('active');
        document.body.style.overflow = 'auto'; // Restaura scroll del body
    }
}

// Cerrar modal al hacer click fuera del contenido
window.addEventListener('click', (e) => {
    if (e.target.classList.contains('modal')) {
        e.target.classList.remove('active');
        document.body.style.overflow = 'auto';
    }
});

// Cerrar modal con tecla ESC
document.addEventListener('keydown', (e) => {
    if (e.key === 'Escape') {
        document.querySelectorAll('.modal.active').forEach(modal => {
            modal.classList.remove('active');
            document.body.style.overflow = 'auto';
        });
    }
});

// ========================================
// EXPORTAR FUNCIONES GLOBALES
// ========================================

window.IoTDashboard = {
    getState: () => state,
    generateReport: () => {
        return {
            timestamp: new Date().toISOString(),
            energy: state.energy,
            maintenance: state.maintenance,
            sensors: state.sensors
        };
    },
    resetSimulation: () => {
        state = {
            energy: { current: 0, savings: 0, history: [] },
            maintenance: { temperature: 0, vibration: 0, healthScore: 100, alerts: [] },
            sensors: { temperature: 0, vibration: 0, current: 0 }
        };
        addLog('info', '🔄 Simulación reiniciada');
    }
};
// ========================================
// SISTEMA DE AGRICULTURA INTELIGENTE
// ========================================

const AGRI_CONFIG = {
    soilMoisture: { min: 20, max: 80, optimal: [40, 70] },
    temperature: { min: 15, max: 35, optimal: [18, 28] },
    light: { min: 200, max: 1000, optimal: [400, 900] },
    pH: { min: 5.0, max: 8.5, optimal: [6.0, 7.5] },
    irrigationCooldown: 30000, // 30 segundos
    updateInterval: 3000 // 3 segundos
};

let agriState = {
    soilMoisture: 50,
    temperature: 22,
    light: 600,
    pH: 6.8,
    irrigating: false,
    lastIrrigation: 0,
    pestDetected: false
};

function initAgricultureSystem() {
    // Verificar si estamos en la página con el módulo de agricultura
    if (!document.getElementById('soil-moisture')) return;
    
    console.log('🌾 Iniciando Sistema de Agricultura Inteligente');
    
    // Iniciar simulación de sensores
    setInterval(updateAgricultureSensors, AGRI_CONFIG.updateInterval);
    
    // Primera actualización inmediata
    updateAgricultureSensors();
}

function updateAgricultureSensors() {
    // Simular lecturas de sensores con variación natural
    agriState.soilMoisture += (Math.random() - 0.5) * 5;
    agriState.soilMoisture = Math.max(AGRI_CONFIG.soilMoisture.min, 
                                       Math.min(AGRI_CONFIG.soilMoisture.max, agriState.soilMoisture));
    
    agriState.temperature += (Math.random() - 0.5) * 2;
    agriState.temperature = Math.max(AGRI_CONFIG.temperature.min, 
                                      Math.min(AGRI_CONFIG.temperature.max, agriState.temperature));
    
    agriState.light += (Math.random() - 0.5) * 50;
    agriState.light = Math.max(AGRI_CONFIG.light.min, 
                                Math.min(AGRI_CONFIG.light.max, agriState.light));
    
    agriState.pH += (Math.random() - 0.5) * 0.2;
    agriState.pH = Math.max(AGRI_CONFIG.pH.min, 
                             Math.min(AGRI_CONFIG.pH.max, agriState.pH));
    
    // Simular detección de plagas (5% de probabilidad)
    if (Math.random() < 0.05 && !agriState.pestDetected) {
        agriState.pestDetected = true;
        setTimeout(() => { agriState.pestDetected = false; }, 15000);
    }
    
    // Actualizar UI
    updateAgricultureUI();
    
    // Verificar necesidad de riego
    checkIrrigationNeeds();
}

function updateAgricultureUI() {
    // Actualizar humedad del suelo
    const soilMoistureEl = document.getElementById('soil-moisture');
    const soilStatusEl = document.getElementById('soil-status');
    const soilBarEl = document.getElementById('soil-bar');
    
    if (soilMoistureEl) {
        soilMoistureEl.textContent = agriState.soilMoisture.toFixed(1);
        soilBarEl.style.width = `${agriState.soilMoisture}%`;
        
        if (agriState.soilMoisture < AGRI_CONFIG.soilMoisture.optimal[0]) {
            soilStatusEl.textContent = '⚠️ Bajo - Requiere riego';
            soilStatusEl.className = 'sensor-status warning';
            soilBarEl.style.background = 'linear-gradient(90deg, #f59e0b, #d97706)';
        } else if (agriState.soilMoisture > AGRI_CONFIG.soilMoisture.optimal[1]) {
            soilStatusEl.textContent = '⚠️ Alto - Riesgo de pudrición';
            soilStatusEl.className = 'sensor-status warning';
            soilBarEl.style.background = 'linear-gradient(90deg, #06b6d4, #0891b2)';
        } else {
            soilStatusEl.textContent = '✓ Óptimo';
            soilStatusEl.className = 'sensor-status';
            soilBarEl.style.background = 'linear-gradient(90deg, #10b981, #059669)';
        }
    }
    
    // Actualizar temperatura
    const tempEl = document.getElementById('agri-temp');
    const tempStatusEl = document.getElementById('temp-status');
    const tempBarEl = document.getElementById('temp-bar');
    
    if (tempEl) {
        tempEl.textContent = agriState.temperature.toFixed(1);
        const tempPercent = ((agriState.temperature - AGRI_CONFIG.temperature.min) / 
                             (AGRI_CONFIG.temperature.max - AGRI_CONFIG.temperature.min)) * 100;
        tempBarEl.style.width = `${tempPercent}%`;
        
        if (agriState.temperature < AGRI_CONFIG.temperature.optimal[0]) {
            tempStatusEl.textContent = '❄️ Frío';
            tempStatusEl.className = 'sensor-status warning';
            tempBarEl.style.background = 'linear-gradient(90deg, #06b6d4, #0891b2)';
        } else if (agriState.temperature > AGRI_CONFIG.temperature.optimal[1]) {
            tempStatusEl.textContent = '🔥 Calor';
            tempStatusEl.className = 'sensor-status danger';
            tempBarEl.style.background = 'linear-gradient(90deg, #ef4444, #dc2626)';
        } else {
            tempStatusEl.textContent = '✓ Óptimo';
            tempStatusEl.className = 'sensor-status';
            tempBarEl.style.background = 'linear-gradient(90deg, #10b981, #059669)';
        }
    }
    
    // Actualizar luz
    const lightEl = document.getElementById('light-level');
    const lightStatusEl = document.getElementById('light-status');
    const lightBarEl = document.getElementById('light-bar');
    
    if (lightEl) {
        lightEl.textContent = Math.round(agriState.light);
        const lightPercent = ((agriState.light - AGRI_CONFIG.light.min) / 
                              (AGRI_CONFIG.light.max - AGRI_CONFIG.light.min)) * 100;
        lightBarEl.style.width = `${lightPercent}%`;
        
        if (agriState.light < AGRI_CONFIG.light.optimal[0]) {
            lightStatusEl.textContent = '🌙 Baja luminosidad';
            lightStatusEl.className = 'sensor-status warning';
        } else if (agriState.light > AGRI_CONFIG.light.optimal[1]) {
            lightStatusEl.textContent = '☀️ Alta luminosidad';
            lightStatusEl.className = 'sensor-status';
        } else {
            lightStatusEl.textContent = '✓ Óptimo';
            lightStatusEl.className = 'sensor-status';
        }
    }
    
    // Actualizar pH
    const phEl = document.getElementById('soil-ph');
    const phStatusEl = document.getElementById('ph-status');
    const phBarEl = document.getElementById('ph-bar');
    
    if (phEl) {
        phEl.textContent = agriState.pH.toFixed(1);
        const phPercent = ((agriState.pH - AGRI_CONFIG.pH.min) / 
                           (AGRI_CONFIG.pH.max - AGRI_CONFIG.pH.min)) * 100;
        phBarEl.style.width = `${phPercent}%`;
        
        if (agriState.pH < AGRI_CONFIG.pH.optimal[0]) {
            phStatusEl.textContent = '🔴 Ácido';
            phStatusEl.className = 'sensor-status warning';
            phBarEl.style.background = 'linear-gradient(90deg, #f59e0b, #d97706)';
        } else if (agriState.pH > AGRI_CONFIG.pH.optimal[1]) {
            phStatusEl.textContent = '🔵 Alcalino';
            phStatusEl.className = 'sensor-status warning';
            phBarEl.style.background = 'linear-gradient(90deg, #3b82f6, #2563eb)';
        } else {
            phStatusEl.textContent = '✓ Neutral';
            phStatusEl.className = 'sensor-status';
            phBarEl.style.background = 'linear-gradient(90deg, #10b981, #059669)';
        }
    }
    
    // Actualizar detección de plagas
    updatePestDetection();
}

function checkIrrigationNeeds() {
    const now = Date.now();
    const irrigationIndicator = document.getElementById('irrigation-indicator');
    const irrigationText = document.getElementById('irrigation-text');
    const irrigationInfo = document.getElementById('irrigation-info');
    
    if (!irrigationIndicator) return;
    
    // Verificar si necesita riego
    const needsIrrigation = agriState.soilMoisture < AGRI_CONFIG.soilMoisture.optimal[0];
    const canIrrigate = (now - agriState.lastIrrigation) > AGRI_CONFIG.irrigationCooldown;
    
    if (agriState.irrigating) {
        // Ya está regando
        irrigationIndicator.querySelector('.indicator-dot').className = 'indicator-dot active';
        irrigationText.textContent = '💧 Riego en Proceso';
        irrigationInfo.textContent = 'Sistema de riego activo - Aplicando agua de precisión';
    } else if (needsIrrigation && canIrrigate) {
        // Activar riego
        activateIrrigation();
    } else if (needsIrrigation && !canIrrigate) {
        // Necesita riego pero está en cooldown
        const remainingTime = Math.ceil((AGRI_CONFIG.irrigationCooldown - (now - agriState.lastIrrigation)) / 1000);
        irrigationIndicator.querySelector('.indicator-dot').className = 'indicator-dot warning';
        irrigationText.textContent = '⏳ En Espera';
        irrigationInfo.textContent = `Próximo riego en ${remainingTime} segundos`;
    } else {
        // No necesita riego
        irrigationIndicator.querySelector('.indicator-dot').className = 'indicator-dot ok';
        irrigationText.textContent = '✓ Sistema en Espera';
        irrigationInfo.textContent = 'Humedad del suelo en niveles óptimos';
    }
}

function activateIrrigation() {
    agriState.irrigating = true;
    agriState.lastIrrigation = Date.now();
    
    const irrigationIndicator = document.getElementById('irrigation-indicator');
    const irrigationText = document.getElementById('irrigation-text');
    const irrigationInfo = document.getElementById('irrigation-info');
    
    irrigationIndicator.querySelector('.indicator-dot').className = 'indicator-dot active';
    irrigationText.textContent = '💧 Riego Activado';
    irrigationInfo.textContent = 'IA detectó baja humedad - Activando riego predictivo';
    
    // Simular duración de riego
    let duration = 5000; // 5 segundos base
    
    // Ajustar duración según temperatura (más calor = más riego)
    if (agriState.temperature > AGRI_CONFIG.temperature.optimal[1]) {
        duration *= 1.5;
        irrigationInfo.textContent = 'IA detectó temperatura alta - Extendiendo tiempo de riego (+50%)';
    }
    
    // Incrementar humedad gradualmente durante el riego
    const incrementPerSecond = 10 / (duration / 1000);
    const irrigationInterval = setInterval(() => {
        agriState.soilMoisture += incrementPerSecond / 3;
    }, 1000);
    
    setTimeout(() => {
        clearInterval(irrigationInterval);
        agriState.irrigating = false;
        irrigationIndicator.querySelector('.indicator-dot').className = 'indicator-dot ok';
        irrigationText.textContent = '✓ Riego Completado';
        irrigationInfo.textContent = 'Sistema en modo de monitoreo continuo';
    }, duration);
}

function updatePestDetection() {
    const pestIndicator = document.getElementById('pest-indicator');
    const pestText = document.getElementById('pest-text');
    const pestInfo = document.getElementById('pest-info');
    
    if (!pestIndicator) return;
    
    if (agriState.pestDetected) {
        pestIndicator.querySelector('.indicator-dot').className = 'indicator-dot danger';
        pestText.textContent = '⚠️ Plagas Detectadas';
        pestInfo.textContent = 'Computer Vision identificó presencia de pulgones - Recomendando aplicación localizada de bioinsecticida';
    } else {
        pestIndicator.querySelector('.indicator-dot').className = 'indicator-dot ok';
        pestText.textContent = '✓ No se detectan plagas';
        pestInfo.textContent = 'Sistema de visión activo - Análisis continuo mediante IA';
    }
}

// Inicializar sistema de agricultura cuando el DOM esté listo
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initAgricultureSystem);
} else {
    initAgricultureSystem();
}

// ========================================
// NAVEGACIÓN Y UX MEJORADA
// ========================================

document.addEventListener('DOMContentLoaded', () => {
    // Smooth scroll mejorado para todos los links de navegación
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function (e) {
            e.preventDefault();
            const target = document.querySelector(this.getAttribute('href'));
            
            if (target) {
                // Actualizar active state en menú
                document.querySelectorAll('.nav-link').forEach(link => {
                    link.classList.remove('active');
                });
                
                if (this.classList.contains('nav-link')) {
                    this.classList.add('active');
                }
                
                // Scroll suave
                target.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });
    
    // Actualizar active state del menú según scroll
    const sections = document.querySelectorAll('section[id]');
    const navLinks = document.querySelectorAll('.nav-link');
    
    window.addEventListener('scroll', () => {
        let current = '';
        
        sections.forEach(section => {
            const sectionTop = section.offsetTop;
            const sectionHeight = section.clientHeight;
            
            if (window.pageYOffset >= sectionTop - 200) {
                current = section.getAttribute('id');
            }
        });
        
        navLinks.forEach(link => {
            link.classList.remove('active');
            if (link.getAttribute('href') === `#${current}`) {
                link.classList.add('active');
            }
        });
    });
});
