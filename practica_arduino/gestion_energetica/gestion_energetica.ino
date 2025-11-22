/*
 * Sistema de Gestión Energética Inteligente
 * 
 * Descripción:
 * Sistema IoT para monitoreo y optimización del consumo energético en fábricas.
 * Detecta ineficiencias, identifica equipos con consumo anómalo y optimiza
 * automáticamente el uso de energía según horarios y demanda.
 * 
 * Conceptos aplicados:
 * - Eficiencia energética y sostenibilidad
 * - ROI empresarial (ahorro 25-30% según Integrante 3)
 * - Automatización inteligente con criterio humano supervisorio
 * 
 * Autor: Proyecto Foro IoT
 * Fecha: Noviembre 2025
 * 
 * Hardware requerido:
 * - Arduino Uno R3
 * - LCD 16x2
 * - 3 Potenciómetros simulando sensores de corriente (A0, A1, A2)
 *   Línea 1: Área de producción
 *   Línea 2: Área de oficinas
 *   Línea 3: Sistemas auxiliares (HVAC, iluminación)
 * - LED Verde - Consumo normal (pin 6)
 * - LED Amarillo - Consumo elevado (pin 7)
 * - LED Rojo - Consumo crítico (pin 8)
 * - Relé simulado - Control de carga (pin 10)
 * 
 * Referencias:
 * Llaneza González, P. (2018) - Aplicaciones IoT industriales
 * Integrante 3 - Análisis de ROI en gestión energética
 */

#include <LiquidCrystal.h>

// Configuración LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pines de sensores de corriente (simulados con potenciómetros)
const int CURRENT_PRODUCTION = A0;   // Línea de producción
const int CURRENT_OFFICE = A1;       // Oficinas
const int CURRENT_AUXILIARY = A2;    // Sistemas auxiliares

// Pines de salida
const int LED_GREEN = 6;
const int LED_YELLOW = 7;
const int LED_RED = 8;
const int RELAY_CONTROL = 10;        // Control de cargas no críticas

// Conversión ADC a corriente (Amperios)
// Simulación: 1023 ADC = 100A máximo
const float ADC_TO_AMPS = 100.0 / 1023.0;

// Conversión corriente a potencia (simplificado a 220V monofásico)
const float VOLTAGE = 220.0;

// Umbrales de consumo (Watts)
const float POWER_NORMAL = 15000;      // 15 kW - operación normal
const float POWER_WARNING = 20000;     // 20 kW - consumo elevado
const float POWER_CRITICAL = 25000;    // 25 kW - límite contratado

// Tarifas eléctricas (USD/kWh) - ejemplo Colombia
const float TARIFF_PEAK = 0.18;        // Horario pico (8am-10pm)
const float TARIFF_OFF_PEAK = 0.12;    // Horario valle (10pm-8am)

// Variables de medición
float currentProduction = 0;
float currentOffice = 0;
float currentAuxiliary = 0;
float totalCurrent = 0;
float totalPower = 0;

// Acumuladores de energía (kWh)
float energyConsumed = 0;
float energyCost = 0;

// Control de tiempo
unsigned long previousMillis = 0;
const long SAMPLING_INTERVAL = 2000;   // 2 segundos
unsigned long totalTime = 0;           // Tiempo total en segundos

// Variables para optimización
bool optimizationActive = false;
int savingsPercentage = 0;

// Simulación de hora del día (para tarifa variable)
int currentHour = 8;  // Iniciar en 8am

void setup() {
  Serial.begin(9600);
  
  Serial.println(F("====================================================="));
  Serial.println(F("Sistema de Gestion Energetica Inteligente"));
  Serial.println(F("====================================================="));
  Serial.println();
  
  lcd.begin(16, 2);
  lcd.print("Sistema Energia");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");
  
  // Configurar pines
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(RELAY_CONTROL, OUTPUT);
  
  delay(2000);
  lcd.clear();
  
  Serial.println(F("Sistema iniciado - Monitoreo energetico activo"));
  Serial.println(F("Nota: Consumo simulado con valores escalados"));
  Serial.println();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= SAMPLING_INTERVAL) {
    previousMillis = currentMillis;
    totalTime += (SAMPLING_INTERVAL / 1000);
    
    // Simular avance de hora (1 hora = 60 segundos en simulación)
    currentHour = 8 + (totalTime / 60) % 24;
    
    // Leer sensores de corriente
    readCurrentSensors();
    
    // Calcular potencia total
    calculateTotalPower();
    
    // Acumular energía consumida
    accumulateEnergy();
    
    // Calcular costo
    calculateEnergyCost();
    
    // Analizar consumo y optimizar
    analyzeAndOptimize();
    
    // Actualizar displays
    updateLCD();
    printEnergyReport();
    
    // Actualizar indicadores
    updateIndicators();
  }
}

void readCurrentSensors() {
  int adcProduction = analogRead(CURRENT_PRODUCTION);
  int adcOffice = analogRead(CURRENT_OFFICE);
  int adcAuxiliary = analogRead(CURRENT_AUXILIARY);
  
  currentProduction = adcProduction * ADC_TO_AMPS;
  currentOffice = adcOffice * ADC_TO_AMPS;
  currentAuxiliary = adcAuxiliary * ADC_TO_AMPS;
  
  totalCurrent = currentProduction + currentOffice + currentAuxiliary;
}

void calculateTotalPower() {
  // P = V * I (simplificado, monofásico)
  totalPower = VOLTAGE * totalCurrent;
}

void accumulateEnergy() {
  // Energía = Potencia * Tiempo
  // kWh = kW * h
  float hours = (SAMPLING_INTERVAL / 1000.0) / 3600.0;
  float kW = totalPower / 1000.0;
  energyConsumed += kW * hours;
}

void calculateEnergyCost() {
  float hours = (SAMPLING_INTERVAL / 1000.0) / 3600.0;
  float kWh = (totalPower / 1000.0) * hours;
  
  // Aplicar tarifa según horario
  float currentTariff = isPeakHour() ? TARIFF_PEAK : TARIFF_OFF_PEAK;
  energyCost += kWh * currentTariff;
}

bool isPeakHour() {
  // Horario pico: 8am - 10pm (8-22)
  return (currentHour >= 8 && currentHour < 22);
}

/**
 * Analiza patrones de consumo y aplica optimizaciones
 * Implementa lógica descrita en Integrante 1 sobre automatización inteligente
 */
void analyzeAndOptimize() {
  Serial.println(F("---------- ANALISIS DE CONSUMO ----------"));
  
  // Análisis por línea
  analyzeLine("Produccion", currentProduction);
  analyzeLine("Oficinas", currentOffice);
  analyzeLine("Auxiliares", currentAuxiliary);
  
  // Detección de ineficiencias
  detectInefficiencies();
  
  // Aplicar optimizaciones automáticas
  if (totalPower > POWER_WARNING && !optimizationActive) {
    Serial.println(F("\n[OPTIMIZACION] Consumo elevado detectado"));
    Serial.println(F("              Activando modo ahorro energetico"));
    applyEnergyOptimization();
    optimizationActive = true;
  } else if (totalPower < POWER_NORMAL && optimizationActive) {
    Serial.println(F("\n[OPTIMIZACION] Consumo normalizado"));
    Serial.println(F("              Desactivando modo ahorro"));
    disableEnergyOptimization();
    optimizationActive = false;
  }
  
  Serial.println(F("-----------------------------------------\n"));
}

void analyzeLine(const char* lineName, float current) {
  float power = VOLTAGE * current;
  float powerKW = power / 1000.0;
  
  Serial.print(F("Linea "));
  Serial.print(lineName);
  Serial.print(F(": "));
  Serial.print(current, 2);
  Serial.print(F(" A | "));
  Serial.print(powerKW, 2);
  Serial.println(F(" kW"));
}

/**
 * Detecta patrones de ineficiencia energética
 * Basado en análisis de consumo vs producción
 */
void detectInefficiencies() {
  // Ineficiencia 1: Consumo de oficinas fuera de horario laboral
  if (currentHour < 7 || currentHour > 19) {
    if (currentOffice > 5.0) {  // Más de 5A fuera de horario
      Serial.println(F("\n[INEFICIENCIA] Consumo elevado en oficinas fuera de horario"));
      Serial.print(F("               Hora: "));
      Serial.print(currentHour);
      Serial.println(F(":00"));
      Serial.print(F("               Consumo: "));
      Serial.print(currentOffice, 2);
      Serial.println(F(" A"));
      Serial.println(F("               Recomendacion: Revisar equipos dejados encendidos"));
    }
  }
  
  // Ineficiencia 2: Sistemas auxiliares desproporcionados
  float auxiliaryPercentage = (currentAuxiliary / totalCurrent) * 100;
  if (auxiliaryPercentage > 30) {
    Serial.println(F("\n[INEFICIENCIA] Sistemas auxiliares consumen >30% del total"));
    Serial.print(F("               Porcentaje: "));
    Serial.print(auxiliaryPercentage, 1);
    Serial.println(F("%"));
    Serial.println(F("               Posibles causas:"));
    Serial.println(F("               - HVAC sobredimensionado"));
    Serial.println(F("               - Iluminacion ineficiente"));
    Serial.println(F("               - Fugas de aire comprimido"));
  }
  
  // Ineficiencia 3: Consumo en horario pico
  if (isPeakHour() && totalPower > POWER_WARNING) {
    Serial.println(F("\n[ALERTA TARIFARIA] Consumo elevado en horario pico"));
    Serial.print(F("                   Tarifa actual: $"));
    Serial.print(TARIFF_PEAK, 3);
    Serial.println(F(" /kWh"));
    Serial.print(F("                   Costo actual: $"));
    float costPerHour = (totalPower / 1000.0) * TARIFF_PEAK;
    Serial.print(costPerHour, 2);
    Serial.println(F(" /hora"));
    Serial.println(F("                   Accion: Considerar reprogramar cargas no criticas"));
  }
}

/**
 * Aplica optimizaciones automáticas de energía
 * Implementa balance automatización-supervisión descrito en Integrante 1
 */
void applyEnergyOptimization() {
  Serial.println(F("\n>>> MEDIDAS DE OPTIMIZACION ACTIVADAS:"));
  
  // Medida 1: Reducir cargas no críticas
  Serial.println(F("  1. Reduciendo iluminacion auxiliar a 70%"));
  Serial.println(F("  2. Ajustando HVAC a modo eco (+2C verano / -2C invierno)"));
  
  // Medida 2: Balanceo de carga
  if (isPeakHour()) {
    Serial.println(F("  3. Horario pico detectado - Difiriendo cargas no urgentes"));
    digitalWrite(RELAY_CONTROL, LOW);  // Desactivar cargas diferibles
  }
  
  // Medida 3: Notificar supervisor
  Serial.println(F("  4. Notificacion enviada a supervisor de planta"));
  Serial.println(F("     (En sistema real: email/SMS automatico)"));
  
  // Estimación de ahorro
  savingsPercentage = 15;  // Estimación de 15% de ahorro
  Serial.print(F("\n  Ahorro estimado: "));
  Serial.print(savingsPercentage);
  Serial.println(F("%"));
}

void disableEnergyOptimization() {
  Serial.println(F("\n>>> Reestableciendo configuracion normal"));
  digitalWrite(RELAY_CONTROL, HIGH);  // Reactivar cargas
  savingsPercentage = 0;
}

void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("P:");
  lcd.print(totalPower / 1000.0, 1);
  lcd.print("kW ");
  
  // Indicador de horario tarifario
  if (isPeakHour()) {
    lcd.print("PICO");
  } else {
    lcd.print("VALL");
  }
  lcd.print(" ");
  
  lcd.setCursor(0, 1);
  lcd.print("$");
  lcd.print(energyCost, 2);
  
  if (optimizationActive) {
    lcd.setCursor(11, 1);
    lcd.print("OPT");
  }
}

void printEnergyReport() {
  Serial.println(F("============ REPORTE ENERGETICO ============"));
  
  Serial.print(F("Hora simulada: "));
  Serial.print(currentHour);
  Serial.print(F(":00 | Tarifa: $"));
  Serial.print(isPeakHour() ? TARIFF_PEAK : TARIFF_OFF_PEAK, 3);
  Serial.println(F(" /kWh"));
  
  Serial.print(F("Consumo actual: "));
  Serial.print(totalPower / 1000.0, 2);
  Serial.print(F(" kW ("));
  Serial.print(totalCurrent, 2);
  Serial.println(F(" A)"));
  
  Serial.print(F("Energia acumulada: "));
  Serial.print(energyConsumed, 4);
  Serial.println(F(" kWh"));
  
  Serial.print(F("Costo acumulado: $"));
  Serial.println(energyCost, 2);
  
  if (optimizationActive) {
    float savedPower = totalPower * savingsPercentage / 100.0;
    Serial.print(F("Ahorro activo: "));
    Serial.print(savedPower / 1000.0, 2);
    Serial.print(F(" kW ("));
    Serial.print(savingsPercentage);
    Serial.println(F("%)"));
  }
  
  // Proyección mensual
  float dailyKWh = energyConsumed * (86400.0 / totalTime);  // Extrapolar a 24h
  float monthlyKWh = dailyKWh * 30;
  float monthlyCost = monthlyKWh * ((TARIFF_PEAK + TARIFF_OFF_PEAK) / 2);
  
  Serial.print(F("Proyeccion mensual: "));
  Serial.print(monthlyKWh, 0);
  Serial.print(F(" kWh | $"));
  Serial.println(monthlyCost, 2);
  
  Serial.println(F("============================================\n"));
}

void updateIndicators() {
  if (totalPower >= POWER_CRITICAL) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
  } else if (totalPower >= POWER_WARNING) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
  } else {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
  }
}

/*
 * ANÁLISIS DE ROI - CASO REAL
 * 
 * Basado en análisis del Integrante 3:
 * 
 * INVERSIÓN INICIAL (Fábrica mediana):
 * - 30 sensores de corriente ACS712: $9,000 USD
 * - 3 gateways Arduino/ESP32: $600 USD
 * - Dashboard web (Node-RED + InfluxDB): $2,400 USD
 * - Actuadores para control automático: $5,000 USD
 * - Instalación e integración: $15,000 USD
 * TOTAL: $32,000 USD
 * 
 * AHORROS ANUALES (Fábrica con consumo 200,000 kWh/año):
 * - Reducción consumo 25%: 50,000 kWh ahorrados
 * - @ $0.15/kWh promedio: $7,500 USD/año
 * - Eliminación de penalizaciones por demanda: $3,500 USD/año
 * - Optimización tarifa horaria: $4,000 USD/año
 * - Certificaciones sostenibilidad (intangible): Acceso a nuevos mercados
 * TOTAL AHORRO: ~$15,000 USD/año
 * 
 * ROI: 32,000 / 15,000 = 2.13 años
 * 
 * Nota: Con incentivos gubernamentales de energía renovable,
 * el ROI puede reducirse a 12-18 meses.
 * 
 * BENEFICIOS ADICIONALES:
 * - Reducción de huella de carbono: ~37.5 toneladas CO2/año
 * - Cumplimiento normativas ambientales
 * - Mejora de reputación corporativa
 * - Datos para certificaciones ISO 50001 (gestión energética)
 * 
 * IMPLEMENTACIÓN SUGERIDA:
 * 
 * Fase 1 (Mes 1-2): Auditoría energética
 *   - Medición baseline actual
 *   - Identificación de cargas críticas vs diferibles
 *   - Definición de KPIs (kWh/unidad producida)
 * 
 * Fase 2 (Mes 3-4): Piloto en área crítica
 *   - 5-10 sensores en línea de mayor consumo
 *   - Dashboard básico de monitoreo
 *   - Validación de ahorros vs baseline
 * 
 * Fase 3 (Mes 5-8): Escalamiento
 *   - Despliegue en toda la planta
 *   - Automatizaciones inteligentes
 *   - Integración con sistema SCADA existente
 * 
 * Fase 4 (Mes 9+): Optimización continua
 *   - Machine learning para predicción de demanda
 *   - Integración con pronóstico climático
 *   - Participación en programas de respuesta a demanda
 * 
 * REFERENCIAS:
 * - Llaneza González, P. (2018) - Aplicaciones empresariales de IoT
 * - Integrante 3 - Análisis de ROI cuantitativo
 * - Estándares ISO 50001 para gestión energética
 */
