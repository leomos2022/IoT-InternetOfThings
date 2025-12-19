/*
 * =======================================================================
 * SISTEMA IOT DE AGRICULTURA INTELIGENTE
 * =======================================================================
 * 
 * Descripción:
 * Sistema integrado de monitoreo agrícola que simula la implementación
 * de IoT e Inteligencia Artificial en una granja moderna. Monitorea
 * condiciones ambientales críticas y activa sistemas de riego automático.
 * 
 * Componentes Simulados:
 * - Sensor DHT22 (Temperatura y Humedad Ambiental)
 * - Sensor de Humedad del Suelo (Capacitivo)
 * - Sensor de Luz (LDR - Fotoresistor)
 * - Sensor de pH del Suelo (Potenciómetro)
 * - Bomba de Riego (Relay/Motor)
 * - Sistema de Alertas (LEDs)
 * 
 * Autor: Sistema de Agricultura Inteligente
 * Curso: Foro Semana 7 - Integración IoT e IA
 * Fecha: Diciembre 2025
 * =======================================================================
 */

// ========== LIBRERÍAS ==========
#include <DHT.h>

// ========== DEFINICIÓN DE PINES ==========
#define DHT_PIN 2              // Pin digital para sensor DHT22
#define DHT_TYPE DHT22         // Tipo de sensor DHT
#define SOIL_MOISTURE_PIN A0   // Pin analógico para sensor de humedad del suelo
#define LIGHT_SENSOR_PIN A1    // Pin analógico para fotoresistor (LDR)
#define PH_SENSOR_PIN A2       // Pin analógico para sensor de pH (simulado)
#define WATER_PUMP_PIN 7       // Pin digital para relay de bomba de agua
#define LED_ALERT_PIN 13       // LED de alerta (condiciones críticas)
#define LED_RIEGO_PIN 12       // LED indicador de riego activo
#define LED_OK_PIN 11          // LED indicador de condiciones óptimas

// ========== PARÁMETROS DE CULTIVO ==========
// Valores óptimos para cultivo genérico (ajustables según tipo de planta)
const float TEMP_MIN = 18.0;           // Temperatura mínima óptima (°C)
const float TEMP_MAX = 28.0;           // Temperatura máxima óptima (°C)
const float HUMIDITY_AIR_MIN = 40.0;   // Humedad ambiental mínima (%)
const float HUMIDITY_AIR_MAX = 70.0;   // Humedad ambiental máxima (%)
const int SOIL_MOISTURE_MIN = 300;     // Humedad suelo mínima (valor ADC)
const int SOIL_MOISTURE_MAX = 700;     // Humedad suelo óptima (valor ADC)
const int LIGHT_MIN = 300;             // Luz mínima necesaria (valor ADC)
const float PH_MIN = 6.0;              // pH mínimo del suelo
const float PH_MAX = 7.5;              // pH máximo del suelo

// ========== PARÁMETROS DE RIEGO ==========
const int RIEGO_DURACION = 5000;       // Duración del riego (ms)
const unsigned long RIEGO_COOLDOWN = 30000; // Tiempo mínimo entre riegos (ms)

// ========== VARIABLES GLOBALES ==========
DHT dht(DHT_PIN, DHT_TYPE);
unsigned long ultimoRiego = 0;
unsigned long ultimaLectura = 0;
const unsigned long INTERVALO_LECTURA = 2000; // Leer sensores cada 2 segundos

// Variables para algoritmo predictivo simple
float tempPromedio = 0;
float humedadSueloPromedio = 0;
int contadorLecturas = 0;

// ========== SETUP ==========
void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600);
  Serial.println("===========================================");
  Serial.println("  SISTEMA DE AGRICULTURA INTELIGENTE");
  Serial.println("  Integracion IoT e IA");
  Serial.println("===========================================");
  Serial.println();
  
  // Inicializar sensor DHT
  dht.begin();
  
  // Configurar pines de salida
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(LED_ALERT_PIN, OUTPUT);
  pinMode(LED_RIEGO_PIN, OUTPUT);
  pinMode(LED_OK_PIN, OUTPUT);
  
  // Estado inicial: todos apagados
  digitalWrite(WATER_PUMP_PIN, LOW);
  digitalWrite(LED_ALERT_PIN, LOW);
  digitalWrite(LED_RIEGO_PIN, LOW);
  digitalWrite(LED_OK_PIN, HIGH); // Sistema OK al inicio
  
  Serial.println("Sistema inicializado correctamente");
  Serial.println("Esperando estabilizacion de sensores...");
  delay(2000);
  Serial.println();
}

// ========== LOOP PRINCIPAL ==========
void loop() {
  unsigned long tiempoActual = millis();
  
  // Leer sensores en intervalos definidos
  if (tiempoActual - ultimaLectura >= INTERVALO_LECTURA) {
    ultimaLectura = tiempoActual;
    
    // ===== LECTURA DE SENSORES =====
    float temperatura = dht.readTemperature();
    float humedadAire = dht.readHumidity();
    int humedadSuelo = analogRead(SOIL_MOISTURE_PIN);
    int luz = analogRead(LIGHT_SENSOR_PIN);
    float pH = leerPH();
    
    // Validar lecturas del DHT
    if (isnan(temperatura) || isnan(humedadAire)) {
      Serial.println("ERROR: Fallo en lectura del sensor DHT");
      digitalWrite(LED_ALERT_PIN, HIGH);
      delay(500);
      digitalWrite(LED_ALERT_PIN, LOW);
      return;
    }
    
    // Actualizar promedios para algoritmo predictivo
    actualizarPromedios(temperatura, humedadSuelo);
    
    // ===== MOSTRAR DATOS EN MONITOR SERIAL =====
    mostrarDatosSensores(temperatura, humedadAire, humedadSuelo, luz, pH);
    
    // ===== ANÁLISIS DE CONDICIONES =====
    bool condicionesOptimas = true;
    
    // Análisis de temperatura
    if (temperatura < TEMP_MIN) {
      Serial.println("[ALERTA] Temperatura BAJA - Riesgo de estres termico");
      condicionesOptimas = false;
    } else if (temperatura > TEMP_MAX) {
      Serial.println("[ALERTA] Temperatura ALTA - Aumentar ventilacion");
      condicionesOptimas = false;
    }
    
    // Análisis de humedad ambiental
    if (humedadAire < HUMIDITY_AIR_MIN) {
      Serial.println("[ALERTA] Humedad ambiental BAJA - Riesgo de deshidratacion");
      condicionesOptimas = false;
    } else if (humedadAire > HUMIDITY_AIR_MAX) {
      Serial.println("[ALERTA] Humedad ambiental ALTA - Riesgo de hongos");
      condicionesOptimas = false;
    }
    
    // Análisis de humedad del suelo (CRÍTICO para riego)
    if (humedadSuelo < SOIL_MOISTURE_MIN) {
      Serial.println("[CRITICO] Humedad del suelo BAJA - Activando riego automatico");
      activarRiego(temperatura);
      condicionesOptimas = false;
    } else if (humedadSuelo > SOIL_MOISTURE_MAX) {
      Serial.println("[ADVERTENCIA] Humedad del suelo ALTA - Riesgo de pudricion raices");
      condicionesOptimas = false;
    }
    
    // Análisis de luz
    if (luz < LIGHT_MIN) {
      Serial.println("[ALERTA] Iluminacion INSUFICIENTE - Fotosintesis limitada");
      condicionesOptimas = false;
    }
    
    // Análisis de pH
    if (pH < PH_MIN) {
      Serial.println("[ALERTA] pH ACIDO - Considerar aplicacion de cal");
      condicionesOptimas = false;
    } else if (pH > PH_MAX) {
      Serial.println("[ALERTA] pH ALCALINO - Considerar acidificacion");
      condicionesOptimas = false;
    }
    
    // ===== PREDICCIÓN SIMPLE (Simulación de IA) =====
    if (contadorLecturas >= 10) {
      predecirNecesidadRiego(temperatura, humedadSuelo);
    }
    
    // ===== ACTUALIZAR INDICADORES LED =====
    if (condicionesOptimas) {
      digitalWrite(LED_OK_PIN, HIGH);
      digitalWrite(LED_ALERT_PIN, LOW);
      Serial.println("[INFO] Todas las condiciones dentro de parametros optimos");
    } else {
      digitalWrite(LED_OK_PIN, LOW);
      digitalWrite(LED_ALERT_PIN, HIGH);
    }
    
    Serial.println("-------------------------------------------");
    Serial.println();
  }
}

// ========== FUNCIONES AUXILIARES ==========

/**
 * Lee y convierte el valor del sensor de pH
 * En implementación real, requiere calibración específica
 */
float leerPH() {
  int lecturaAnalogica = analogRead(PH_SENSOR_PIN);
  // Conversión simplificada: mapear 0-1023 a rango pH 4.0-9.0
  float pH = map(lecturaAnalogica, 0, 1023, 40, 90) / 10.0;
  return pH;
}

/**
 * Activa el sistema de riego automático
 * Considera temperatura para ajustar duración
 */
void activarRiego(float temperatura) {
  unsigned long tiempoActual = millis();
  
  // Verificar cooldown (evitar riego excesivo)
  if (tiempoActual - ultimoRiego < RIEGO_COOLDOWN) {
    unsigned long tiempoRestante = (RIEGO_COOLDOWN - (tiempoActual - ultimoRiego)) / 1000;
    Serial.print("[INFO] Riego en cooldown. Tiempo restante: ");
    Serial.print(tiempoRestante);
    Serial.println(" segundos");
    return;
  }
  
  // Calcular duración ajustada según temperatura (mayor temp = más riego)
  int duracionAjustada = RIEGO_DURACION;
  if (temperatura > TEMP_MAX) {
    duracionAjustada = RIEGO_DURACION * 1.5; // 50% más tiempo si hace calor
    Serial.println("[IA] Temperatura alta detectada - Extendiendo tiempo de riego");
  }
  
  // Activar bomba de agua
  Serial.print("[ACCION] Activando sistema de riego por ");
  Serial.print(duracionAjustada / 1000);
  Serial.println(" segundos");
  
  digitalWrite(WATER_PUMP_PIN, HIGH);
  digitalWrite(LED_RIEGO_PIN, HIGH);
  
  delay(duracionAjustada);
  
  digitalWrite(WATER_PUMP_PIN, LOW);
  digitalWrite(LED_RIEGO_PIN, LOW);
  
  ultimoRiego = millis();
  Serial.println("[INFO] Riego completado");
}

/**
 * Actualiza promedios móviles para predicciones
 */
void actualizarPromedios(float temp, int humedadSuelo) {
  contadorLecturas++;
  tempPromedio = ((tempPromedio * (contadorLecturas - 1)) + temp) / contadorLecturas;
  humedadSueloPromedio = ((humedadSueloPromedio * (contadorLecturas - 1)) + humedadSuelo) / contadorLecturas;
  
  // Resetear contador cada 50 lecturas para adaptarse a cambios
  if (contadorLecturas > 50) {
    contadorLecturas = 25; // Mantener algo de histórico
  }
}

/**
 * Predicción simple de necesidad de riego
 * Simula algoritmo de ML considerando tendencias
 */
void predecirNecesidadRiego(float tempActual, int humedadActual) {
  // Calcular tasa de pérdida de humedad
  float tasaEvapotranspiracion = (tempActual - 20) * 0.05; // Simplificado
  
  // Estimar humedad en próximas horas
  float humedadPredicha = humedadActual - (tasaEvapotranspiracion * 10);
  
  if (humedadPredicha < SOIL_MOISTURE_MIN && humedadActual > SOIL_MOISTURE_MIN) {
    Serial.println("========================================");
    Serial.println("[IA PREDICTIVA] Analisis de tendencias:");
    Serial.print("  - Humedad actual: ");
    Serial.println(humedadActual);
    Serial.print("  - Humedad predicha (2h): ");
    Serial.println(humedadPredicha);
    Serial.println("  - RECOMENDACION: Riego preventivo sugerido");
    Serial.println("    para evitar estres hidrico inminente");
    Serial.println("========================================");
  }
}

/**
 * Muestra lecturas de sensores en formato organizado
 */
void mostrarDatosSensores(float temp, float humAire, int humSuelo, int luz, float pH) {
  Serial.println("┌─────────────────────────────────────────┐");
  Serial.println("│      LECTURAS DE SENSORES IoT           │");
  Serial.println("├─────────────────────────────────────────┤");
  
  Serial.print("│ Temperatura:        ");
  Serial.print(temp, 1);
  Serial.println(" °C          │");
  
  Serial.print("│ Humedad Ambiental:  ");
  Serial.print(humAire, 1);
  Serial.println(" %           │");
  
  Serial.print("│ Humedad Suelo:      ");
  Serial.print(humSuelo);
  Serial.print(" (");
  Serial.print(map(humSuelo, 0, 1023, 0, 100));
  Serial.println("%)      │");
  
  Serial.print("│ Luminosidad:        ");
  Serial.print(luz);
  Serial.print(" (");
  Serial.print(map(luz, 0, 1023, 0, 100));
  Serial.println("%)      │");
  
  Serial.print("│ pH del Suelo:       ");
  Serial.print(pH, 1);
  Serial.println("              │");
  
  Serial.println("└─────────────────────────────────────────┘");
}

/*
 * =======================================================================
 * NOTAS DE IMPLEMENTACIÓN EN TINKERCAD:
 * =======================================================================
 * 
 * Para simular este sistema en Tinkercad:
 * 
 * 1. SENSOR DHT22:
 *    - Usar componente "Temperature Sensor" (DHT22)
 *    - Conectar VCC a 5V, GND a GND, DATA al pin 2
 *    - Ajustar temperatura con el deslizador (simular día/noche)
 * 
 * 2. SENSOR DE HUMEDAD DEL SUELO:
 *    - Usar POTENCIÓMETRO conectado a A0
 *    - Girar para simular diferentes niveles de humedad
 *    - Bajo valor = suelo seco, Alto valor = suelo húmedo
 * 
 * 3. SENSOR DE LUZ (LDR):
 *    - Usar POTENCIÓMETRO o FOTORESISTOR conectado a A1
 *    - Simula diferentes niveles de iluminación
 * 
 * 4. SENSOR DE pH:
 *    - Usar POTENCIÓMETRO conectado a A2
 *    - Simula diferentes niveles de acidez/alcalinidad
 * 
 * 5. BOMBA DE AGUA:
 *    - Usar MOTOR DC con RELAY/TRANSISTOR en pin 7
 *    - O simplemente un LED para visualizar activación
 * 
 * 6. LEDs INDICADORES:
 *    - LED ROJO (pin 13): Alerta general
 *    - LED AZUL (pin 12): Riego activo
 *    - LED VERDE (pin 11): Condiciones óptimas
 * 
 * =======================================================================
 * ESCENARIOS DE PRUEBA:
 * =======================================================================
 * 
 * ESCENARIO 1: Día caluroso y seco
 * - Temperatura DHT: 32°C
 * - Humedad suelo (A0): <300 (potenciómetro girado a bajo)
 * - Resultado esperado: Sistema activa riego automático extendido
 * 
 * ESCENARIO 2: Noche fría y húmeda
 * - Temperatura DHT: 15°C
 * - Humedad suelo (A0): >700
 * - Resultado esperado: Alertas de temperatura baja y exceso de humedad
 * 
 * ESCENARIO 3: Condiciones óptimas
 * - Temperatura: 22-25°C
 * - Humedad aire: 50-60%
 * - Humedad suelo: 400-600
 * - Luz: >300
 * - pH: 6.5-7.0
 * - Resultado esperado: LED verde encendido, sin alertas
 * 
 * ESCENARIO 4: Suelo ácido
 * - pH (A2): <200 (equivale a pH ~6.0)
 * - Resultado esperado: Alerta de pH bajo, sugerir aplicar cal
 * 
 * =======================================================================
 */
