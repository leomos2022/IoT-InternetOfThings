# Guía de Implementación con Arduino IDE

## Del Simulador al Hardware Real - Guía Profesional

---

## Índice

1. [Preparación del Entorno](#preparación-del-entorno)
2. [Componentes de Hardware Necesarios](#componentes-de-hardware-necesarios)
3. [Instalación de Bibliotecas](#instalación-de-bibliotecas)
4. [Proyecto 1: Sistema IoT Integrado](#proyecto-1-sistema-iot-integrado-hardware)
5. [Proyecto 2: Mantenimiento Predictivo](#proyecto-2-mantenimiento-predictivo-hardware)
6. [Proyecto 3: Gestión Energética](#proyecto-3-gestión-energética-hardware)
7. [Integración con Plataformas Cloud](#integración-con-plataformas-cloud)
8. [Seguridad y Mejores Prácticas](#seguridad-y-mejores-prácticas)

---

## Preparación del Entorno

### Instalación de Arduino IDE

#### Windows

1. Descargar desde: https://www.arduino.cc/en/software
2. Ejecutar instalador `.exe`
3. Aceptar instalación de drivers USB (importante para comunicación)
4. Completar instalación

#### macOS

1. Descargar `.dmg` desde sitio oficial
2. Arrastrar Arduino a carpeta Aplicaciones
3. Primera ejecución: Sistema → Preferencias → Seguridad → Permitir

#### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install arduino
# O descargar versión más reciente desde sitio oficial
```

### Configuración Inicial

1. Abrir Arduino IDE
2. Herramientas → Placa → Arduino AVR Boards → Arduino Uno
3. Herramientas → Puerto → Seleccionar puerto COM del Arduino
   - Windows: COMx (ej: COM3)
   - macOS: /dev/cu.usbmodem...
   - Linux: /dev/ttyUSB0 o /dev/ttyACM0

4. Verificar conexión:
   - Archivo → Ejemplos → 01.Basics → Blink
   - Subir sketch (flecha →)
   - LED integrado de Arduino debe parpadear

---

## Componentes de Hardware Necesarios

### Lista de Compras para los 3 Proyectos

#### Componentes Comunes

| Componente | Cantidad | Precio Aprox. (USD) | Proveedor Sugerido |
|------------|----------|---------------------|-------------------|
| Arduino Uno R3 | 1 | $25 | Arduino.cc, Amazon |
| LCD 16x2 con I2C | 1 | $5 | AliExpress, Amazon |
| Protoboard 830 puntos | 2 | $6 | Cualquier distribuidor |
| Cables jumper M-M (40 pcs) | 1 pack | $3 | - |
| Cables jumper M-F (40 pcs) | 1 pack | $3 | - |
| Fuente 9V/1A | 1 | $8 | - |

#### Sensores Reales (Reemplazan Potenciómetros)

| Sensor | Proyecto | Precio | Notas |
|--------|----------|--------|-------|
| LM35 Temperatura | Todos | $2 | Precisión ±0.5°C |
| DHT22 Temp/Humedad | Alternativa | $5 | Digital, más preciso |
| MPU6050 Acelerómetro | Mantenimiento | $4 | Incluye giroscopio |
| ACS712 30A Corriente | Gestión Energía | $3 | Versiones 5A, 20A, 30A |

#### Actuadores e Indicadores

| Componente | Cantidad | Precio | Uso |
|------------|----------|--------|-----|
| LEDs (rojo, amarillo, verde) | 5 cada uno | $0.10 c/u | Indicadores |
| Resistencias 220Ω | 10 | $0.05 c/u | LEDs |
| Resistencias 10kΩ | 5 | $0.05 c/u | Pull-up/down |
| Buzzer pasivo | 2 | $1 c/u | Alarmas |
| Módulo Relé 1 canal | 1 | $2 | Control cargas |

#### Conectividad (Opcional pero Recomendado)

| Módulo | Precio | Propósito |
|--------|--------|-----------|
| ESP32 DevKit | $8 | WiFi + Bluetooth + 32 bits |
| ESP8266 NodeMCU | $5 | WiFi (alternativa económica) |
| Ethernet Shield | $12 | Conexión cableada estable |

**TOTAL ESTIMADO**: $80-120 USD (completo con conectividad)

### Dónde Comprar

**Internacional:**
- **Amazon**: Envío rápido, garantía
- **AliExpress**: Económico, envío lento (2-4 semanas)
- **Mouser/DigiKey**: Componentes premium, stock confiable

**Latinoamérica:**
- **MercadoLibre**: Disponibilidad local
- **Vistronica** (Colombia): Componentes electrónicos
- **330ohms** (Perú): Arduino y sensores
- **ElectronicaEstudio** (Argentina): Robótica y IoT

---

## Instalación de Bibliotecas

### Método 1: Gestor de Bibliotecas (Recomendado)

1. Arduino IDE → Herramientas → Administrar Bibliotecas
2. Buscar e instalar:
   - **LiquidCrystal I2C** (by Frank de Brabander)
   - **DHT sensor library** (by Adafruit) - si usa DHT22
   - **Adafruit Unified Sensor** (dependencia de DHT)
   - **MPU6050** (by Electronic Cats) - para vibración
   - **PubSubClient** (by Nick O'Leary) - para MQTT

### Método 2: Instalación Manual

1. Descargar biblioteca (.zip)
2. Sketch → Incluir Biblioteca → Añadir biblioteca .ZIP
3. Seleccionar archivo descargado

### Verificar Instalación

```cpp
// Código de prueba
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Wire.h>

void setup() {
  Serial.println("Bibliotecas cargadas correctamente");
}

void loop() {}
```

Si compila sin errores, bibliotecas están instaladas correctamente.

---

## Proyecto 1: Sistema IoT Integrado (Hardware)

### Diagrama de Conexiones Real

#### Usando LCD con I2C (Recomendado - Solo 4 cables)

```
LCD I2C → Arduino
----------------
VCC → 5V
GND → GND
SDA → A4
SCL → A5
```

#### Sensor LM35 (Temperatura)

```
LM35 → Arduino
--------------
Pin 1 (VCC) → 5V
Pin 2 (OUT) → A0
Pin 3 (GND) → GND
```

#### MPU6050 (Vibración/Acelerómetro)

```
MPU6050 → Arduino
-----------------
VCC → 5V
GND → GND
SDA → A4 (compartido con LCD)
SCL → A5 (compartido con LCD)
```

**Nota**: I2C permite múltiples dispositivos en mismo bus (SDA/SCL).

#### ACS712 (Corriente)

```
ACS712 → Arduino
----------------
VCC → 5V
GND → GND
OUT → A1
```

#### LEDs e Indicadores

```
LED Verde:
  Pin 8 → Resistencia 220Ω → Ánodo LED → Cátodo → GND

LED Amarillo:
  Pin 9 → Resistencia 220Ω → Ánodo LED → Cátodo → GND

LED Rojo:
  Pin 10 → Resistencia 220Ω → Ánodo LED → Cátodo → GND

Buzzer:
  Pin 13 → Buzzer(+) → Buzzer(-) → GND
```

### Código Adaptado para Hardware Real

```cpp
/*
 * Sistema IoT con Hardware Real
 * Adaptación para sensores LM35, MPU6050, ACS712
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MPU6050.h>

// LCD I2C en dirección 0x27 (puede ser 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

MPU6050 mpu;

const int LM35_PIN = A0;
const int ACS712_PIN = A1;

const int LED_GREEN = 8;
const int LED_YELLOW = 9;
const int LED_RED = 10;
const int BUZZER = 13;

float temperature = 0;
int vibrationMagnitude = 0;
float current = 0;

void setup() {
  Serial.begin(9600);
  
  // Inicializar LCD I2C
  lcd.init();
  lcd.backlight();
  lcd.print("Sistema IoT Fab");
  
  // Inicializar MPU6050
  Wire.begin();
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("Error: MPU6050 no detectado");
  }
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  delay(2000);
  lcd.clear();
}

void loop() {
  // Leer temperatura (LM35: 10mV/°C)
  int adcValue = analogRead(LM35_PIN);
  float voltage = adcValue * (5.0 / 1023.0);
  temperature = voltage * 100.0;
  
  // Leer vibración (MPU6050)
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  vibrationMagnitude = sqrt(ax*ax + ay*ay + az*az) / 1000;
  
  // Leer corriente (ACS712-30A: 66mV/A, offset 2.5V)
  adcValue = analogRead(ACS712_PIN);
  voltage = adcValue * (5.0 / 1023.0);
  current = (voltage - 2.5) / 0.066;  // Fórmula para ACS712-30A
  
  // Mostrar en LCD
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C V:");
  lcd.print(vibrationMagnitude);
  lcd.print("  ");
  
  lcd.setCursor(0, 1);
  lcd.print("I:");
  lcd.print(current, 1);
  lcd.print("A     ");
  
  // Análisis y alertas
  analyzeConditions();
  
  // Enviar datos por serial
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" | Vibr: ");
  Serial.print(vibrationMagnitude);
  Serial.print(" | Current: ");
  Serial.println(current);
  
  delay(2000);
}

void analyzeConditions() {
  bool critical = false;
  
  if (temperature > 35.0) {
    Serial.println("[CRITICO] Temperatura alta!");
    critical = true;
  }
  
  if (vibrationMagnitude > 15000) {
    Serial.println("[CRITICO] Vibracion excesiva!");
    critical = true;
  }
  
  if (current > 25.0) {
    Serial.println("[CRITICO] Sobrecarga electrica!");
    critical = true;
  }
  
  if (critical) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    tone(BUZZER, 1000, 500);
  } else {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
  }
}
```

### Calibración de Sensores

#### LM35 (Temperatura)

1. Leer temperatura ambiente con termómetro de referencia
2. Comparar con lectura de Arduino
3. Ajustar fórmula si necesario:
   ```cpp
   temperature = (voltage * 100.0) + OFFSET;  // OFFSET = diferencia medida
   ```

#### ACS712 (Corriente)

**Calibración de CERO:**
```cpp
// Sin carga conectada, leer voltaje
float zeroVoltage = 0;
for(int i=0; i<100; i++) {
  zeroVoltage += analogRead(ACS712_PIN) * (5.0/1023.0);
  delay(10);
}
zeroVoltage /= 100;
Serial.print("Voltaje en cero: ");
Serial.println(zeroVoltage);  // Debe ser ~2.5V
```

#### MPU6050 (Vibración)

**Calibración de baseline:**
```cpp
void calibrateMPU() {
  Serial.println("Calibrando MPU6050... No mover!");
  int16_t ax, ay, az;
  long axSum=0, aySum=0, azSum=0;
  
  for(int i=0; i<200; i++) {
    mpu.getAcceleration(&ax, &ay, &az);
    axSum += ax;
    aySum += ay;
    azSum += az;
    delay(10);
  }
  
  int16_t axOffset = axSum / 200;
  int16_t ayOffset = aySum / 200;
  int16_t azOffset = azSum / 200;
  
  Serial.print("Offsets: ");
  Serial.print(axOffset);
  Serial.print(", ");
  Serial.print(ayOffset);
  Serial.print(", ");
  Serial.println(azOffset);
}
```

---

## Integración con Plataformas Cloud

### Opción 1: MQTT con Mosquitto (Local)

#### Instalar Mosquitto Broker (Servidor local)

**Windows:**
```bash
# Descargar desde: https://mosquitto.org/download/
# Instalar y ejecutar servicio
net start mosquitto
```

**Linux:**
```bash
sudo apt install mosquitto mosquitto-clients
sudo systemctl start mosquitto
```

#### Código Arduino con MQTT

```cpp
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "TU_RED_WIFI";
const char* password = "TU_PASSWORD";
const char* mqtt_server = "192.168.1.100";  // IP de tu PC con Mosquitto

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi conectado");
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Leer sensores (código anterior)
  float temp = readTemperature();
  
  // Publicar datos
  String payload = "{\"temperature\":" + String(temp) + "}";
  client.publish("fabrica/sensores", payload.c_str());
  
  delay(5000);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ArduinoClient")) {
      Serial.println("conectado");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}
```

### Opción 2: AWS IoT Core

1. **Crear cuenta AWS** (capa gratuita incluye IoT Core)

2. **Configurar Thing (Dispositivo)**:
   - AWS Console → IoT Core → Manage → Things → Create
   - Descargar certificados (guardar en lugar seguro)

3. **Instalar bibliotecas**:
   ```bash
   # En Arduino IDE: Administrar Bibliotecas
   # Buscar: "AWS IoT Arduino" by Amazon Web Services
   ```

4. **Código básico AWS IoT**:
   ```cpp
   #include <WiFiClientSecure.h>
   #include <PubSubClient.h>
   
   const char* AWS_ENDPOINT = "xxxxx.iot.us-east-1.amazonaws.com";
   
   // Certificados (incluir en código o cargar de SD)
   const char* AWS_CERT_CA = "-----BEGIN CERTIFICATE-----\n...";
   const char* AWS_CERT_CRT = "-----BEGIN CERTIFICATE-----\n...";
   const char* AWS_CERT_PRIVATE = "-----BEGIN RSA PRIVATE KEY-----\n...";
   
   WiFiClientSecure espClient;
   PubSubClient client(espClient);
   
   void setup() {
     // Configurar certificados
     espClient.setCACert(AWS_CERT_CA);
     espClient.setCertificate(AWS_CERT_CRT);
     espClient.setPrivateKey(AWS_CERT_PRIVATE);
     
     client.setServer(AWS_ENDPOINT, 8883);
   }
   ```

### Opción 3: ThingSpeak (Más Simple para Empezar)

1. **Crear cuenta**: https://thingspeak.com
2. **Crear Channel** con fields: temperature, vibration, current
3. **Obtener Write API Key**

4. **Código Arduino**:
   ```cpp
   #include <WiFi.h>
   
   const char* server = "api.thingspeak.com";
   const String API_KEY = "TU_API_KEY";
   
   WiFiClient client;
   
   void sendToThingSpeak(float temp, int vib, float curr) {
     if (client.connect(server, 80)) {
       String postStr = API_KEY;
       postStr += "&field1=";
       postStr += String(temp);
       postStr += "&field2=";
       postStr += String(vib);
       postStr += "&field3=";
       postStr += String(curr);
       
       client.print("POST /update HTTP/1.1\n");
       client.print("Host: api.thingspeak.com\n");
       client.print("Connection: close\n");
       client.print("Content-Type: application/x-www-form-urlencoded\n");
       client.print("Content-Length: ");
       client.print(postStr.length());
       client.print("\n\n");
       client.print(postStr);
       
       client.stop();
     }
   }
   ```

---

## Seguridad y Mejores Prácticas

### Seguridad según Kumar & Singh Gaba (2020)

#### 1. Encriptación de Comunicaciones

```cpp
// Usar WiFiClientSecure en lugar de WiFiClient
WiFiClientSecure client;

// Configurar TLS 1.2 mínimo
client.setInsecure();  // Solo para desarrollo
// En producción, usar certificados:
client.setCACert(root_ca);
```

#### 2. Autenticación de Dispositivos

```cpp
// Incluir identificador único de dispositivo
String deviceID = WiFi.macAddress();

// Incluir en cada mensaje
String payload = "{\"device\":\"" + deviceID + "\",\"temp\":" + String(temp) + "}";
```

#### 3. Validación de Datos

```cpp
// Rechazar lecturas anómalas (posible ataque de inyección)
if (temperature < -40 || temperature > 125) {  // Rango físico LM35
  Serial.println("[SEGURIDAD] Lectura temperatura fuera de rango físico");
  return;  // No enviar dato
}
```

#### 4. Actualización OTA Segura

```cpp
#include <ArduinoOTA.h>

void setupOTA() {
  ArduinoOTA.setPassword("PASSWORD_SEGURO_AQUI");
  
  ArduinoOTA.onStart([]() {
    Serial.println("Iniciando actualización OTA");
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
  });
  
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();  // Procesar actualizaciones
  // ... resto del código
}
```

### Mejores Prácticas de Código

#### Gestión de Memoria

```cpp
// Evitar uso excesivo de String (fragmenta memoria)
// MALO:
String mensaje = "Temp: " + String(temp) + " C";

// BUENO:
char mensaje[32];
snprintf(mensaje, sizeof(mensaje), "Temp: %.1f C", temp);
```

#### Watchdog Timer (Reinicio ante fallo)

```cpp
#include <avr/wdt.h>

void setup() {
  wdt_enable(WDTO_8S);  // Watchdog de 8 segundos
}

void loop() {
  wdt_reset();  // Resetear watchdog (indicar que sistema responde)
  
  // ... código principal
  
  // Si loop() se cuelga >8s, Arduino se reiniciará automáticamente
}
```

#### Manejo de Errores

```cpp
bool readSensor(float &value) {
  int attempts = 0;
  while (attempts < 3) {
    value = analogRead(SENSOR_PIN);
    if (value >= 0 && value <= 1023) {
      return true;  // Lectura válida
    }
    attempts++;
    delay(100);
  }
  Serial.println("Error: Fallo lectura sensor tras 3 intentos");
  return false;  // Fallo
}

void loop() {
  float temp;
  if (readSensor(temp)) {
    // Procesar dato
  } else {
    // Manejo de error
  }
}
```

---

## Troubleshooting Hardware

### Problema: LCD no muestra nada

**Diagnóstico:**
1. Verificar contraste (potenciómetro en VO)
2. Verificar alimentación (VDD=5V, VSS=GND)
3. Probar dirección I2C:
   ```cpp
   #include <Wire.h>
   
   void setup() {
     Wire.begin();
     Serial.begin(9600);
     Serial.println("Escaneando I2C...");
     
     for(byte addr=1; addr<127; addr++) {
       Wire.beginTransmission(addr);
       if (Wire.endTransmission() == 0) {
         Serial.print("Dispositivo en 0x");
         Serial.println(addr, HEX);
       }
     }
   }
   ```

### Problema: Lecturas de sensores erráticas

**Soluciones:**
1. **Agregar capacitor**: 100nF entre VCC y GND del sensor (filtra ruido)
2. **Promediar lecturas**:
   ```cpp
   float readAveraged(int pin, int samples = 10) {
     float sum = 0;
     for(int i=0; i<samples; i++) {
       sum += analogRead(pin);
       delay(10);
     }
     return sum / samples;
   }
   ```
3. **Usar referencia externa**: `analogReference(EXTERNAL)` con referencia 2.56V o 1.1V

### Problema: Arduino se reinicia aleatoriamente

**Causas comunes:**
1. **Alimentación insuficiente**: Usar fuente externa 9V/1A, no USB
2. **Consumo excesivo**: Relés/motores deben usar fuente separada
3. **Ruido eléctrico**: Separar fuentes analógicas/digitales

---

## Recursos Adicionales

### Libros Recomendados

- **"Arduino Cookbook"** - Michael Margolis (O'Reilly)
- **"Getting Started with Arduino"** - Massimo Banzi
- **"Programming Arduino"** - Simon Monk

### Cursos Online

- **Arduino Official Courses**: https://www.arduino.cc/education/courses/
- **Coursera - IoT Specialization**: Universidad de California Irvine
- **edX - IoT Programming**: Microsoft

### Comunidades

- **Arduino Forum**: https://forum.arduino.cc/
- **Reddit r/arduino**: Soporte comunitario
- **Discord - Arduino**: Servidor oficial

---

**Autor**: Proyecto Foro IoT - 2025  
**Versión**: 1.0  
**Última actualización**: Noviembre 2025
