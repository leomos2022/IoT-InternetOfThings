/*
 * Sistema de Mantenimiento Predictivo Avanzado
 * 
 * Descripción:
 * Sistema especializado en detección temprana de fallos en maquinaria industrial
 * mediante análisis de patrones de vibración y temperatura.
 * Implementa algoritmos de detección de anomalías y predicción de fallos.
 * 
 * Conceptos aplicados:
 * - Mantenimiento predictivo (Integrante 1)
 * - Edge computing para decisiones locales (Integrante 2)
 * - Machine learning básico en Arduino (Integrante 3)
 * 
 * Autor: Proyecto Foro IoT
 * Fecha: Noviembre 2025
 * 
 * Hardware requerido:
 * - Arduino Uno R3
 * - LCD 16x2
 * - Potenciómetro para temperatura (A0)
 * - Potenciómetro para vibración (A1)
 * - LED Verde - Máquina saludable (pin 7)
 * - LED Amarillo - Mantenimiento próximo (pin 8)
 * - LED Rojo - Fallo inminente (pin 9)
 * 
 * Referencias:
 * Kumar, P. y Singh Gaba, G. (2020) - Seguridad en IIoT
 * Ramírez Ramírez et al. (2023) - Machine Learning en ciberseguridad
 */

#include <LiquidCrystal.h>

// Configuración LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pines de sensores
const int TEMP_PIN = A0;
const int VIBRATION_PIN = A1;

// Pines de indicadores
const int LED_HEALTHY = 7;
const int LED_MAINTENANCE = 8;
const int LED_FAILURE = 9;

// Buffer para análisis de tendencias
const int BUFFER_SIZE = 20;
float tempBuffer[BUFFER_SIZE];
int vibrationBuffer[BUFFER_SIZE];
int bufferIndex = 0;

// Variables de estado
float currentTemp = 0;
int currentVibration = 0;
float tempTrend = 0;      // Tendencia de temperatura (°C/minuto)
float vibrationTrend = 0;  // Tendencia de vibración

// Umbrales de operación normal (baseline)
const float TEMP_BASELINE = 25.0;
const int VIBRATION_BASELINE = 400;

// Umbrales de desviación aceptable
const float TEMP_DEVIATION_WARNING = 5.0;    // +5°C sobre baseline
const float TEMP_DEVIATION_CRITICAL = 10.0;  // +10°C sobre baseline
const int VIBRATION_DEVIATION_WARNING = 150;
const int VIBRATION_DEVIATION_CRITICAL = 300;

// Umbrales de tendencia (cambio por minuto)
const float TEMP_TREND_WARNING = 0.5;    // +0.5°C/min
const int VIBRATION_TREND_WARNING = 50;   // +50 unidades/min

// Estado de salud de la máquina (0-100%)
int machineHealth = 100;

// Predicción de tiempo hasta fallo (horas)
float timeToFailure = 9999;

// Contador de anomalías consecutivas
int anomalyCounter = 0;

void setup() {
  Serial.begin(9600);
  
  Serial.println(F("================================================"));
  Serial.println(F("Sistema de Mantenimiento Predictivo Avanzado"));
  Serial.println(F("================================================"));
  Serial.println();
  
  lcd.begin(16, 2);
  lcd.print("Mant. Predictivo");
  lcd.setCursor(0, 1);
  lcd.print("Calibrando...");
  
  // Configurar pines de salida
  pinMode(LED_HEALTHY, OUTPUT);
  pinMode(LED_MAINTENANCE, OUTPUT);
  pinMode(LED_FAILURE, OUTPUT);
  
  // Calibración inicial - llenar buffer con valores baseline
  Serial.println(F("Calibracion inicial - Estableciendo baseline..."));
  for (int i = 0; i < BUFFER_SIZE; i++) {
    tempBuffer[i] = TEMP_BASELINE;
    vibrationBuffer[i] = VIBRATION_BASELINE;
  }
  
  delay(2000);
  lcd.clear();
  
  Serial.println(F("Calibracion completada"));
  Serial.println(F("Iniciando monitoreo predictivo..."));
  Serial.println();
}

void loop() {
  // Adquisición de datos
  currentTemp = readTemperature();
  currentVibration = analogRead(VIBRATION_PIN);
  
  // Almacenar en buffer circular
  tempBuffer[bufferIndex] = currentTemp;
  vibrationBuffer[bufferIndex] = currentVibration;
  bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;
  
  // Calcular tendencias
  calculateTrends();
  
  // Calcular salud de la máquina
  calculateMachineHealth();
  
  // Predecir tiempo hasta fallo
  predictTimeToFailure();
  
  // Detectar anomalías
  detectAnomalies();
  
  // Actualizar displays
  updateLCD();
  printDetailedReport();
  
  // Actualizar indicadores
  updateHealthIndicators();
  
  delay(3000);  // Muestreo cada 3 segundos
}

float readTemperature() {
  int reading = analogRead(TEMP_PIN);
  float voltage = reading * (5.0 / 1024.0);
  return voltage * 10.0;
}

/**
 * Calcula tendencias usando regresión lineal simple sobre buffer
 * Referencia: Análisis de tendencias mencionado en Integrante 2
 */
void calculateTrends() {
  // Temperatura: diferencia entre valor actual y promedio del buffer
  float tempSum = 0;
  int vibrationSum = 0;
  
  for (int i = 0; i < BUFFER_SIZE; i++) {
    tempSum += tempBuffer[i];
    vibrationSum += vibrationBuffer[i];
  }
  
  float tempAvg = tempSum / BUFFER_SIZE;
  float vibrationAvg = vibrationSum / BUFFER_SIZE;
  
  // Tendencia simplificada: diferencia actual vs promedio
  // En implementación avanzada, usar regresión lineal verdadera
  tempTrend = currentTemp - tempAvg;
  vibrationTrend = currentVibration - vibrationAvg;
}

/**
 * Calcula salud de la máquina (0-100%)
 * Basado en desviaciones de baseline y tendencias
 */
void calculateMachineHealth() {
  int healthScore = 100;
  
  // Penalización por desviación de temperatura
  float tempDeviation = abs(currentTemp - TEMP_BASELINE);
  if (tempDeviation > TEMP_DEVIATION_CRITICAL) {
    healthScore -= 40;
  } else if (tempDeviation > TEMP_DEVIATION_WARNING) {
    healthScore -= 20;
  }
  
  // Penalización por desviación de vibración
  int vibrationDeviation = abs(currentVibration - VIBRATION_BASELINE);
  if (vibrationDeviation > VIBRATION_DEVIATION_CRITICAL) {
    healthScore -= 40;
  } else if (vibrationDeviation > VIBRATION_DEVIATION_WARNING) {
    healthScore -= 20;
  }
  
  // Penalización por tendencias negativas
  if (tempTrend > TEMP_TREND_WARNING) {
    healthScore -= 10;
  }
  if (vibrationTrend > VIBRATION_TREND_WARNING) {
    healthScore -= 10;
  }
  
  // Asegurar rango 0-100
  machineHealth = constrain(healthScore, 0, 100);
}

/**
 * Predice tiempo estimado hasta fallo
 * Algoritmo simplificado - en producción usar ML avanzado
 * Referencia: Ramírez Ramírez et al. (2023) sobre ML predictivo
 */
void predictTimeToFailure() {
  if (machineHealth >= 80) {
    timeToFailure = 9999;  // Máquina saludable
  } else if (machineHealth >= 60) {
    timeToFailure = 168;   // ~7 días
  } else if (machineHealth >= 40) {
    timeToFailure = 72;    // ~3 días
  } else if (machineHealth >= 20) {
    timeToFailure = 24;    // 1 día
  } else {
    timeToFailure = 8;     // Crítico - 8 horas
  }
}

/**
 * Detecta anomalías mediante análisis de patrones
 * Implementa lógica de seguridad según Kumar & Singh Gaba (2020)
 */
void detectAnomalies() {
  bool anomalyDetected = false;
  
  // Anomalía tipo 1: Incremento súbito de temperatura
  if (tempTrend > TEMP_TREND_WARNING) {
    Serial.println(F("[ANOMALIA] Incremento rapido de temperatura"));
    Serial.print(F("            Tasa: +"));
    Serial.print(tempTrend, 2);
    Serial.println(F(" C/min"));
    Serial.println(F("            Posible fallo de refrigeracion"));
    anomalyDetected = true;
  }
  
  // Anomalía tipo 2: Vibración errática
  if (vibrationTrend > VIBRATION_TREND_WARNING) {
    Serial.println(F("[ANOMALIA] Patron de vibracion anormal"));
    Serial.print(F("            Incremento: +"));
    Serial.print(vibrationTrend);
    Serial.println(F(" unidades/min"));
    Serial.println(F("            Posibles causas:"));
    Serial.println(F("            - Rodamiento desgastado"));
    Serial.println(F("            - Desbalanceo rotor"));
    Serial.println(F("            - Tornillos flojos"));
    anomalyDetected = true;
  }
  
  // Anomalía tipo 3: Correlación anormal temp-vibración
  // Normalmente, alta vibración causa alta temperatura
  if ((currentVibration > VIBRATION_BASELINE + 200) && 
      (currentTemp < TEMP_BASELINE)) {
    Serial.println(F("[ANOMALIA] Correlacion anormal temp-vibracion"));
    Serial.println(F("            Posible fallo de sensor o ataque"));
    Serial.println(F("            Referencia: Kumar & Singh Gaba (2020) - Data injection"));
    anomalyDetected = true;
  }
  
  // Contador de anomalías consecutivas
  if (anomalyDetected) {
    anomalyCounter++;
  } else {
    anomalyCounter = 0;
  }
  
  // Alerta si múltiples anomalías consecutivas
  if (anomalyCounter >= 3) {
    Serial.println(F(""));
    Serial.println(F("****************************************************"));
    Serial.println(F("*** ALERTA: MULTIPLES ANOMALIAS CONSECUTIVAS ***"));
    Serial.println(F("*** ACCION RECOMENDADA: INSPECCION INMEDIATA ***"));
    Serial.println(F("****************************************************"));
    Serial.println(F(""));
  }
}

void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Salud:");
  lcd.print(machineHealth);
  lcd.print("%  ");
  
  lcd.setCursor(0, 1);
  if (timeToFailure > 1000) {
    lcd.print("Estado: OK     ");
  } else {
    lcd.print("Fallo:");
    lcd.print((int)timeToFailure);
    lcd.print("h  ");
  }
}

void printDetailedReport() {
  Serial.println(F("========== REPORTE DE MANTENIMIENTO PREDICTIVO =========="));
  
  Serial.print(F("Temperatura: "));
  Serial.print(currentTemp, 1);
  Serial.print(F(" C (baseline: "));
  Serial.print(TEMP_BASELINE, 1);
  Serial.print(F(" C) | Desviacion: "));
  Serial.print(currentTemp - TEMP_BASELINE, 1);
  Serial.println(F(" C"));
  
  Serial.print(F("Vibracion: "));
  Serial.print(currentVibration);
  Serial.print(F(" (baseline: "));
  Serial.print(VIBRATION_BASELINE);
  Serial.print(F(") | Desviacion: "));
  Serial.println(currentVibration - VIBRATION_BASELINE);
  
  Serial.print(F("Tendencia Temp: "));
  if (tempTrend > 0) Serial.print(F("+"));
  Serial.print(tempTrend, 2);
  Serial.println(F(" C/min"));
  
  Serial.print(F("Tendencia Vibr: "));
  if (vibrationTrend > 0) Serial.print(F("+"));
  Serial.print(vibrationTrend, 0);
  Serial.println(F(" unidades/min"));
  
  Serial.print(F("SALUD DE MAQUINA: "));
  Serial.print(machineHealth);
  Serial.print(F("% - "));
  if (machineHealth >= 80) {
    Serial.println(F("EXCELENTE"));
  } else if (machineHealth >= 60) {
    Serial.println(F("BUENA - Programar mantenimiento"));
  } else if (machineHealth >= 40) {
    Serial.println(F("REGULAR - Mantenimiento prioritario"));
  } else if (machineHealth >= 20) {
    Serial.println(F("MALA - Mantenimiento urgente"));
  } else {
    Serial.println(F("CRITICA - Parada preventiva recomendada"));
  }
  
  Serial.print(F("TIEMPO ESTIMADO HASTA FALLO: "));
  if (timeToFailure > 1000) {
    Serial.println(F(">1000 horas (maquina saludable)"));
  } else {
    Serial.print(timeToFailure, 0);
    Serial.println(F(" horas"));
  }
  
  Serial.println(F("========================================================="));
  Serial.println();
}

void updateHealthIndicators() {
  if (machineHealth >= 70) {
    // Máquina saludable
    digitalWrite(LED_HEALTHY, HIGH);
    digitalWrite(LED_MAINTENANCE, LOW);
    digitalWrite(LED_FAILURE, LOW);
  } else if (machineHealth >= 40) {
    // Mantenimiento próximo
    digitalWrite(LED_HEALTHY, LOW);
    digitalWrite(LED_MAINTENANCE, HIGH);
    digitalWrite(LED_FAILURE, LOW);
  } else {
    // Fallo inminente
    digitalWrite(LED_HEALTHY, LOW);
    digitalWrite(LED_MAINTENANCE, LOW);
    digitalWrite(LED_FAILURE, HIGH);
  }
}

/*
 * NOTAS DE IMPLEMENTACIÓN Y USO:
 * 
 * 1. INTERPRETACIÓN DE RESULTADOS:
 *    - Salud 80-100%: Operación normal, monitoreo rutinario
 *    - Salud 60-79%: Programar mantenimiento en próxima ventana
 *    - Salud 40-59%: Mantenimiento prioritario (dentro de 72h)
 *    - Salud 20-39%: Mantenimiento urgente (dentro de 24h)
 *    - Salud 0-19%: Parada preventiva recomendada
 * 
 * 2. INTEGRACIÓN CON SISTEMAS EMPRESARIALES:
 *    - Conectar a sistema CMMS (Computerized Maintenance Management System)
 *    - Generación automática de órdenes de trabajo según salud
 *    - Alertas automáticas a supervisores vía email/SMS
 * 
 * 3. MEJORAS PARA PRODUCCIÓN:
 *    - Implementar modelos ML entrenados con datos históricos
 *    - Análisis FFT para detección de frecuencias anómalas
 *    - Correlación con datos de producción (carga, velocidad)
 *    - Integración con gemelos digitales (Integrante 4)
 * 
 * 4. VALIDACIÓN DEL SISTEMA:
 *    - Comparar predicciones con fallos reales durante 6 meses
 *    - Ajustar umbrales según curva ROC (tasa VP vs FP)
 *    - Documentar ahorros: reducción de paradas no planificadas
 * 
 * 5. CONSIDERACIONES DE SEGURIDAD:
 *    - Validar integridad de datos de sensores (detectar inyección)
 *    - Autenticar comandos de mantenimiento
 *    - Auditar todos los eventos de salud crítica
 *    - Referencia: Kumar & Singh Gaba (2020) sobre seguridad IIoT
 */
