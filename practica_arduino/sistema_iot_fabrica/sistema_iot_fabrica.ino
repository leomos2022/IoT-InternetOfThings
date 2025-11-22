/*
 * Sistema IoT Integrado para Fábrica Inteligente
 * 
 * Descripción:
 * Este programa simula un sistema de monitoreo integral para una fábrica inteligente
 * integrando múltiples sensores para:
 * - Monitoreo de temperatura (mantenimiento predictivo)
 * - Detección de vibración (condición de máquinas)
 * - Medición de consumo de corriente (gestión energética)
 * 
 * Autor: Leonardo Mosquera Rodriguez
 * NRC-3315-ELECTIVA CPC: Internet de las cosas
 * Fecha: Noviembre 2025
 * 
 * Hardware requerido (Tinkercad):
 * - Arduino Uno R3
 * - LCD 16x2 (pines: 12, 11, 5, 4, 3, 2)
 * - 3 Potenciómetros (simulando sensores en A0, A1, A2)
 * - LED Verde (pin 8) - Operación normal
 * - LED Amarillo (pin 9) - Advertencia
 * - LED Rojo (pin 10) - Alerta crítica
 * - Buzzer (pin 13) - Alarma sonora
 * 
 * Referencias bibliográficas:
 * Kumar, P. y Singh Gaba, G. (2020). Biometric-Based Robust Access Control Model
 * Llaneza González, P. (2018). La Internet de las Cosas
 */

#include <LiquidCrystal.h>

// Configuración del LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pines de sensores analógicos
const int TEMP_SENSOR_PIN = A0;      // Sensor de temperatura (simula LM35)
const int VIBRATION_SENSOR_PIN = A1;  // Sensor de vibración (simula MPU6050)
const int CURRENT_SENSOR_PIN = A2;    // Sensor de corriente (simula ACS712)

// Pines de indicadores visuales y sonoros
const int LED_GREEN = 8;    // Sistema operando normalmente
const int LED_YELLOW = 9;   // Advertencia - requiere atención
const int LED_RED = 10;     // Alerta crítica - acción inmediata
const int BUZZER = 13;      // Alarma sonora

// Umbrales de alerta (basados en estándares industriales)
const float TEMP_WARNING = 30.0;      // °C - Temperatura de advertencia
const float TEMP_CRITICAL = 35.0;     // °C - Temperatura crítica
const int VIBRATION_WARNING = 600;    // Unidades ADC - Vibración elevada
const int VIBRATION_CRITICAL = 800;   // Unidades ADC - Vibración crítica
const int CURRENT_WARNING = 900;      // Unidades ADC - Consumo elevado
const int CURRENT_CRITICAL = 1020;    // Unidades ADC - Sobrecarga

// Variables para almacenar lecturas
float temperature = 0.0;
int vibration = 0;
int current = 0;

// Variables de control de tiempo
unsigned long previousMillis = 0;
const long SAMPLING_INTERVAL = 2000;  // Intervalo de muestreo: 2 segundos

// Estados del sistema
enum SystemState {
  NORMAL,
  WARNING,
  CRITICAL
};
SystemState currentState = NORMAL;

// Contadores para estadísticas
unsigned long totalSamples = 0;
unsigned int warningCount = 0;
unsigned int criticalCount = 0;

void setup() {
  // Inicialización del monitor serial
  Serial.begin(9600);
  Serial.println(F("==========================================="));
  Serial.println(F("Sistema IoT para Fabrica Inteligente v1.0"));
  Serial.println(F("==========================================="));
  Serial.println();
  
  // Inicialización del LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Sistema IoT Fab");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");
  
  // Configuración de pines de salida
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  // Secuencia de inicio (test de LEDs)
  testLEDs();
  
  delay(2000);
  lcd.clear();
  
  Serial.println(F("Sistema iniciado correctamente"));
  Serial.println(F("Iniciando monitoreo continuo..."));
  Serial.println();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Lectura periódica de sensores según intervalo definido
  if (currentMillis - previousMillis >= SAMPLING_INTERVAL) {
    previousMillis = currentMillis;
    
    // Lectura de sensores
    temperature = readTemperature();
    vibration = analogRead(VIBRATION_SENSOR_PIN);
    current = analogRead(CURRENT_SENSOR_PIN);
    
    // Incrementar contador de muestras
    totalSamples++;
    
    // Actualizar display LCD
    updateLCD();
    
    // Enviar datos por serial (simulando protocolo MQTT)
    sendDataToCloud();
    
    // Análisis de condiciones y generación de alertas
    analyzeConditions();
    
    // Actualizar indicadores visuales
    updateIndicators();
  }
}

/**
 * Función: readTemperature
 * Propósito: Lee y convierte señal del sensor de temperatura
 * Retorna: Temperatura en grados Celsius
 * 
 * Nota: En implementación real con LM35:
 * - Cada 10mV = 1°C
 * - A 25°C, salida = 250mV
 */
float readTemperature() {
  int reading = analogRead(TEMP_SENSOR_PIN);
  
  // Conversión ADC a voltaje (0-1023 -> 0-5V)
  float voltage = reading * (5.0 / 1024.0);
  
  // Conversión voltaje a temperatura (LM35: 10mV/°C)
  // Ajustado para potenciómetro: escala de 0-50°C
  float temp = voltage * 10.0;
  
  return temp;
}

/**
 * Función: updateLCD
 * Propósito: Actualiza información en pantalla LCD
 * Formato:
 *   Línea 1: T:XX.XC V:XXX
 *   Línea 2: I:XXX S:XXX
 */
void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C V:");
  lcd.print(vibration);
  lcd.print("   "); // Limpiar caracteres residuales
  
  lcd.setCursor(0, 1);
  lcd.print("I:");
  lcd.print(current);
  lcd.print(" S:");
  lcd.print(totalSamples);
  lcd.print("   ");
}

/**
 * Función: sendDataToCloud
 * Propósito: Simula envío de datos a plataforma cloud vía MQTT
 * 
 * En implementación real, esto sería similar a:
 * client.publish("fabrica/linea1/sensores", jsonPayload);
 * 
 * Referencia: Protocolo MQTT mencionado en Integrante 3
 */
void sendDataToCloud() {
  Serial.print(F("[DATA] Muestra #"));
  Serial.print(totalSamples);
  Serial.print(F(" | Temp: "));
  Serial.print(temperature, 1);
  Serial.print(F("C | Vibr: "));
  Serial.print(vibration);
  Serial.print(F(" | Curr: "));
  Serial.println(current);
}

/**
 * Función: analyzeConditions
 * Propósito: Analiza lecturas de sensores y determina estado del sistema
 * Implementa lógica de mantenimiento predictivo según Kumar & Singh Gaba (2020)
 */
void analyzeConditions() {
  bool warningDetected = false;
  bool criticalDetected = false;
  
  // Análisis de temperatura
  if (temperature >= TEMP_CRITICAL) {
    Serial.println(F("[CRITICO] Temperatura fuera de limites seguros!"));
    Serial.print(F("          Temp actual: "));
    Serial.print(temperature, 1);
    Serial.print(F("C | Limite: "));
    Serial.print(TEMP_CRITICAL, 1);
    Serial.println(F("C"));
    Serial.println(F("          Accion: Activar sistema de ventilacion"));
    criticalDetected = true;
    criticalCount++;
  } else if (temperature >= TEMP_WARNING) {
    Serial.println(F("[ADVERTENCIA] Temperatura elevada"));
    Serial.print(F("              Temp: "));
    Serial.print(temperature, 1);
    Serial.println(F("C"));
    warningDetected = true;
    warningCount++;
  }
  
  // Análisis de vibración (mantenimiento predictivo)
  if (vibration >= VIBRATION_CRITICAL) {
    Serial.println(F("[CRITICO] Vibracion excesiva detectada!"));
    Serial.print(F("          Valor: "));
    Serial.print(vibration);
    Serial.print(F(" | Limite: "));
    Serial.println(VIBRATION_CRITICAL);
    Serial.println(F("          Posibles causas:"));
    Serial.println(F("          - Desbalanceo de rotor"));
    Serial.println(F("          - Desgaste de rodamientos"));
    Serial.println(F("          - Desalineacion de ejes"));
    Serial.println(F("          Accion: Parada preventiva programada"));
    criticalDetected = true;
    criticalCount++;
  } else if (vibration >= VIBRATION_WARNING) {
    Serial.println(F("[ADVERTENCIA] Vibracion por encima de lo normal"));
    Serial.print(F("              Valor: "));
    Serial.println(vibration);
    Serial.println(F("              Accion: Inspeccionar en proximo mantenimiento"));
    warningDetected = true;
    warningCount++;
  }
  
  // Análisis de consumo eléctrico (gestión energética)
  if (current >= CURRENT_CRITICAL) {
    Serial.println(F("[CRITICO] Sobrecarga electrica!"));
    Serial.print(F("          Consumo: "));
    Serial.print(current);
    Serial.print(F(" | Limite: "));
    Serial.println(CURRENT_CRITICAL);
    Serial.println(F("          Accion: Reducir carga automaticamente"));
    criticalDetected = true;
    criticalCount++;
  } else if (current >= CURRENT_WARNING) {
    Serial.println(F("[ADVERTENCIA] Consumo electrico elevado"));
    Serial.print(F("              Valor: "));
    Serial.println(current);
    warningDetected = true;
    warningCount++;
  }
  
  // Actualizar estado del sistema
  if (criticalDetected) {
    currentState = CRITICAL;
  } else if (warningDetected) {
    currentState = WARNING;
  } else {
    currentState = NORMAL;
  }
  
  // Línea separadora para claridad en log
  if (warningDetected || criticalDetected) {
    Serial.println(F("-------------------------------------------"));
  }
}

/**
 * Función: updateIndicators
 * Propósito: Actualiza LEDs y buzzer según estado del sistema
 */
void updateIndicators() {
  switch (currentState) {
    case NORMAL:
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, LOW);
      noTone(BUZZER);
      break;
      
    case WARNING:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_RED, LOW);
      noTone(BUZZER);
      break;
      
    case CRITICAL:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, HIGH);
      // Alarma intermitente (1 segundo encendido, 1 apagado)
      tone(BUZZER, 1000, 500);
      break;
  }
}

/**
 * Función: testLEDs
 * Propósito: Secuencia de prueba de LEDs al inicio
 */
void testLEDs() {
  Serial.println(F("Ejecutando test de indicadores..."));
  
  digitalWrite(LED_GREEN, HIGH);
  delay(300);
  digitalWrite(LED_GREEN, LOW);
  
  digitalWrite(LED_YELLOW, HIGH);
  delay(300);
  digitalWrite(LED_YELLOW, LOW);
  
  digitalWrite(LED_RED, HIGH);
  tone(BUZZER, 1000, 200);
  delay(300);
  digitalWrite(LED_RED, LOW);
  
  Serial.println(F("Test completado exitosamente"));
}

/*
 * NOTAS DE IMPLEMENTACIÓN:
 * 
 * 1. SIMULACIÓN EN TINKERCAD:
 *    - Usar potenciómetros para simular variaciones de sensores
 *    - Girar potenciómetros para provocar alertas
 *    - Observar cambios en LCD, LEDs y monitor serial
 * 
 * 2. IMPLEMENTACIÓN REAL:
 *    - Reemplazar potenciómetros por sensores reales:
 *      * LM35 o DHT22 para temperatura
 *      * MPU6050 o ADXL345 para vibración
 *      * ACS712 para corriente
 *    - Agregar módulo WiFi (ESP32) o Ethernet Shield para conectividad
 *    - Implementar cliente MQTT para envío de datos a cloud
 * 
 * 3. MEJORAS SUGERIDAS (Integrante 2):
 *    - Agregar procesamiento edge con filtros de media móvil
 *    - Implementar algoritmo de detección de tendencias
 *    - Añadir almacenamiento local (SD card) para buffer offline
 * 
 * 4. SEGURIDAD (Kumar & Singh Gaba, 2020):
 *    - En producción, usar TLS 1.3 para comunicaciones
 *    - Implementar autenticación de dispositivo con certificados
 *    - Encriptar datos sensibles antes de transmitir
 * 
 * 5. ESCALABILIDAD (Integrante 3):
 *    - Este código representa un nodo en red IoT
 *    - Asignar DEVICE_ID único a cada Arduino
 *    - Centralizar gestión en broker MQTT o plataforma cloud
 */
