# Guía de Implementación en Tinkercad

## Sistema IoT para Fábrica Inteligente - Tutorial Paso a Paso

---

## Índice

1. [Introducción a Tinkercad](#introducción-a-tinkercad)
2. [Proyecto 1: Sistema IoT Integrado](#proyecto-1-sistema-iot-integrado)
3. [Proyecto 2: Mantenimiento Predictivo](#proyecto-2-mantenimiento-predictivo)
4. [Proyecto 3: Gestión Energética](#proyecto-3-gestión-energética)
5. [Pruebas y Validación](#pruebas-y-validación)
6. [Exportación y Documentación](#exportación-y-documentación)

---

## Introducción a Tinkercad

### ¿Qué es Tinkercad?

Tinkercad es una plataforma gratuita de Autodesk que permite simular circuitos electrónicos, incluyendo Arduino, sin necesidad de hardware físico.

### Acceso a Tinkercad

1. Visitar: https://www.tinkercad.com
2. Crear cuenta gratuita (puede usar Google/Microsoft para login rápido)
3. Hacer clic en "Circuits" en el menú principal
4. Seleccionar "Create new Circuit"

---

## Proyecto 1: Sistema IoT Integrado

### Objetivo

Construir un sistema de monitoreo que integre temperatura, vibración y corriente con alertas visuales y sonoras.

### Paso 1: Crear Nuevo Circuito

1. En Tinkercad, hacer clic en **"Create new Circuit"**
2. Nombrar el proyecto: `Sistema_IoT_Fabrica_Inteligente`

### Paso 2: Agregar Componentes

#### Componente 1: Arduino Uno

1. En el panel de componentes (derecha), buscar **"Arduino Uno R3"**
2. Arrastrar al área de trabajo
3. Posicionar en el centro-izquierda del área

#### Componente 2: LCD 16x2

1. Buscar **"LCD 16x2"** en componentes
2. Arrastrar a la derecha del Arduino
3. Rotar si es necesario (clic derecho → Rotate)

#### Componente 3: Potenciómetros (Sensores Simulados)

1. Buscar **"Potentiometer"**
2. Arrastrar **3 potenciómetros** al área de trabajo
3. Ubicarlos debajo del Arduino
4. Etiquetar cada uno:
   - Clic en el potenciómetro
   - Cambiar nombre en panel de propiedades:
     - Potenciómetro 1: "Sensor Temperatura"
     - Potenciómetro 2: "Sensor Vibración"
     - Potenciómetro 3: "Sensor Corriente"

#### Componente 4: LEDs Indicadores

1. Buscar **"LED"**
2. Agregar **3 LEDs**:
   - LED Verde (operación normal)
   - LED Amarillo (advertencia)
   - LED Rojo (crítico)

#### Componente 5: Resistencias para LEDs

1. Buscar **"Resistor"**
2. Agregar **3 resistencias de 220Ω**
3. Cambiar valor:
   - Clic en resistencia → Panel propiedades → Resistance: 220Ω

#### Componente 6: Buzzer

1. Buscar **"Piezo"**
2. Arrastrar un buzzer al área de trabajo

#### Componente 7: Protoboard (Opcional pero recomendado)

1. Buscar **"Breadboard Small"**
2. Agregar para organizar conexiones

### Paso 3: Realizar Conexiones

#### Conexiones del LCD 16x2

```
LCD Pin → Arduino Pin
-------------------
VSS    → GND
VDD    → 5V
V0     → Potenciómetro de contraste (terminal central)
RS     → Pin 12
RW     → GND
E      → Pin 11
D4     → Pin 5
D5     → Pin 4
D6     → Pin 3
D7     → Pin 2
A      → 5V (con resistencia 220Ω)
K      → GND
```

**Nota sobre V0 (contraste):**
- Agregar un potenciómetro pequeño solo para contraste
- Terminal izquierda → 5V
- Terminal central → V0 del LCD
- Terminal derecha → GND

#### Conexiones de Potenciómetros (Sensores)

```
Sensor Temperatura (A0):
  - Terminal izquierda → GND
  - Terminal central → A0
  - Terminal derecha → 5V

Sensor Vibración (A1):
  - Terminal izquierda → GND
  - Terminal central → A1
  - Terminal derecha → 5V

Sensor Corriente (A2):
  - Terminal izquierda → GND
  - Terminal central → A2
  - Terminal derecha → 5V
```

#### Conexiones de LEDs

```
LED Verde (Pin 8):
  - Ánodo (+, pata larga) → Pin 8
  - Cátodo (-, pata corta) → Resistencia 220Ω → GND

LED Amarillo (Pin 9):
  - Ánodo → Pin 9
  - Cátodo → Resistencia 220Ω → GND

LED Rojo (Pin 10):
  - Ánodo → Pin 10
  - Cátodo → Resistencia 220Ω → GND
```

#### Conexión del Buzzer

```
Buzzer (Pin 13):
  - Terminal positivo → Pin 13
  - Terminal negativo → GND
```

### Paso 4: Cargar el Código

1. Hacer clic en el botón **"Code"** (esquina superior derecha)
2. Seleccionar **"Text"** (no Blocks)
3. Eliminar el código de ejemplo
4. Copiar y pegar el código completo de `sistema_iot_fabrica.ino`
5. Hacer clic en **"Done Editing"**

### Paso 5: Verificar y Simular

1. Hacer clic en **"Start Simulation"** (botón verde)
2. Observar:
   - LCD muestra "Sistema IoT Fab" durante inicialización
   - Test de LEDs (parpadean secuencialmente)
   - LEDs se apagan excepto el verde (estado normal)
   - LCD muestra datos de sensores

### Paso 6: Probar Alertas

#### Prueba de Temperatura

1. Hacer clic en el potenciómetro "Sensor Temperatura"
2. Girar hacia la derecha (incrementar valor)
3. Observar:
   - A ~60% → LED amarillo (advertencia)
   - A ~70% → LED rojo + buzzer (crítico)
   - Monitor Serial muestra alertas

#### Prueba de Vibración

1. Hacer clic en el potenciómetro "Sensor Vibración"
2. Girar hacia la derecha
3. Observar alertas similares

#### Prueba de Corriente

1. Hacer clic en el potenciómetro "Sensor Corriente"
2. Girar hacia la derecha
3. Observar alertas de sobrecarga

### Paso 7: Monitor Serial

1. Hacer clic en **"Serial Monitor"** (parte inferior)
2. Verificar que muestra:
   - Mensajes de inicialización
   - Datos de sensores cada 2 segundos
   - Alertas cuando se exceden umbrales

---

## Proyecto 2: Mantenimiento Predictivo

### Objetivo

Sistema especializado que predice fallos en maquinaria mediante análisis de tendencias.

### Configuración Rápida

#### Diferencias con Proyecto 1

- Solo 2 potenciómetros (temperatura y vibración)
- Solo 3 LEDs (saludable, mantenimiento, fallo)
- Sin buzzer
- Código diferente (análisis de tendencias)

### Pasos Específicos

1. **Crear nuevo circuito**: `Mantenimiento_Predictivo`

2. **Componentes necesarios**:
   - Arduino Uno R3
   - LCD 16x2
   - 2 Potenciómetros (temperatura A0, vibración A1)
   - 3 LEDs (verde pin 7, amarillo pin 8, rojo pin 9)
   - 3 Resistencias 220Ω
   - 1 Potenciómetro para contraste LCD

3. **Conexiones**:
   - Igual que Proyecto 1, pero solo sensores A0 y A1
   - LEDs en pines 7, 8, 9

4. **Cargar código**: `mantenimiento_predictivo.ino`

5. **Prueba de predicción**:
   - Incrementar gradualmente temperatura
   - Observar cómo "Salud" disminuye de 100% a valores bajos
   - Monitor Serial muestra "TIEMPO ESTIMADO HASTA FALLO"

---

## Proyecto 3: Gestión Energética

### Objetivo

Monitorear consumo energético y optimizar automáticamente para reducir costos.

### Configuración Específica

#### Componentes Únicos

- **Relé** (simulado): Representa control de cargas no críticas
- **3 Potenciómetros**: Simulan corrientes de diferentes áreas

### Pasos

1. **Crear circuito**: `Gestion_Energetica_Inteligente`

2. **Componentes**:
   - Arduino Uno R3
   - LCD 16x2
   - 3 Potenciómetros (A0: Producción, A1: Oficinas, A2: Auxiliares)
   - 3 LEDs (verde pin 6, amarillo pin 7, rojo pin 8)
   - 1 LED azul (pin 10, simula relé)
   - 4 Resistencias 220Ω

3. **Conexiones**:
   ```
   LED Azul (Relé, Pin 10):
     - Ánodo → Pin 10
     - Cátodo → Resistencia 220Ω → GND
   ```

4. **Cargar código**: `gestion_energetica.ino`

5. **Prueba de optimización**:
   - Incrementar los 3 potenciómetros para simular alto consumo
   - Observar mensaje "MEDIDAS DE OPTIMIZACION ACTIVADAS"
   - LED azul se apaga (cargas no críticas desconectadas)
   - LCD muestra "OPT" indicando modo optimización

---

## Pruebas y Validación

### Checklist de Validación para Cada Proyecto

#### ✅ Sistema IoT Integrado

- [ ] LCD muestra valores en ambas líneas
- [ ] Potenciómetros modifican valores en LCD
- [ ] LED verde enciende en estado normal
- [ ] LED amarillo enciende al girar potenciómetro ~60%
- [ ] LED rojo + buzzer activan al girar potenciómetro ~70%
- [ ] Monitor Serial muestra mensajes de alerta

#### ✅ Mantenimiento Predictivo

- [ ] LCD muestra "Salud: XX%"
- [ ] Salud disminuye al incrementar sensores
- [ ] LED verde = Salud >70%
- [ ] LED amarillo = Salud 40-70%
- [ ] LED rojo = Salud <40%
- [ ] Monitor Serial predice "Tiempo hasta fallo"

#### ✅ Gestión Energética

- [ ] LCD muestra potencia en kW y costo
- [ ] LCD alterna "PICO" y "VALL" según hora simulada
- [ ] Al incrementar consumo, aparece "OPT" en LCD
- [ ] LED azul (relé) se apaga en modo optimización
- [ ] Monitor Serial reporta proyección mensual

---

## Exportación y Documentación

### Capturas de Pantalla

1. **Captura del circuito completo**:
   - Hacer clic derecho en área de trabajo → "Download"
   - Seleccionar formato PNG o PDF

2. **Captura del Monitor Serial**:
   - Capturar pantalla con herramientas del sistema
   - Windows: Win + Shift + S
   - macOS: Cmd + Shift + 4

### Compartir Proyecto

1. Hacer clic en botón **"Share"** (esquina superior derecha)
2. Configurar privacidad:
   - **Público**: Cualquiera puede ver
   - **Privado**: Solo tú
   - **Link**: Solo quienes tengan el enlace
3. Copiar URL para incluir en documentos del foro

### Video de Demostración

1. Usar grabador de pantalla (OBS Studio, Loom, etc.)
2. Grabar secuencia:
   - Mostrar circuito completo
   - Iniciar simulación
   - Demostrar cada tipo de alerta
   - Mostrar Monitor Serial con datos
3. Duración recomendada: 2-3 minutos por proyecto

---

## Solución de Problemas Comunes

### Problema: LCD muestra cuadros negros

**Solución:**
- Verificar potenciómetro de contraste (V0)
- Ajustar valor del potenciómetro en simulación

### Problema: LEDs no encienden

**Solución:**
- Verificar resistencias de 220Ω están conectadas
- Revisar polaridad del LED (ánodo a pin digital, cátodo a GND)

### Problema: Código no compila

**Solución:**
- Verificar que se seleccionó modo "Text" (no Blocks)
- Revisar que se copió el código completo
- Asegurar incluir `#include <LiquidCrystal.h>` al inicio

### Problema: Monitor Serial no muestra datos

**Solución:**
- Verificar que simulación esté corriendo (botón verde)
- Hacer clic en botón "Serial Monitor" en parte inferior
- Verificar baud rate está en 9600 (configurable en Monitor Serial)

### Problema: Simulación muy lenta

**Solución:**
- Tinkercad puede ser lento en navegadores antiguos
- Probar con Chrome o Firefox actualizados
- Cerrar otras pestañas del navegador
- Reducir complejidad del circuito para pruebas iniciales

---

## Recursos Adicionales

### Tutoriales de Tinkercad

- **Oficial**: https://www.tinkercad.com/learn/circuits
- **YouTube**: Buscar "Tinkercad Arduino tutorial"

### Documentación Arduino

- **Referencia oficial**: https://www.arduino.cc/reference/en/
- **LiquidCrystal Library**: https://www.arduino.cc/en/Reference/LiquidCrystal

### Comunidad

- **Foro Arduino**: https://forum.arduino.cc/
- **Reddit r/arduino**: https://www.reddit.com/r/arduino/
- **Stack Overflow**: Tag [arduino]

---

## Próximos Pasos

Después de dominar las simulaciones en Tinkercad:

1. **Exportar código** para uso en Arduino IDE
2. **Implementar con hardware real** (ver `Guia_Arduino_IDE.md`)
3. **Agregar conectividad WiFi** (ESP32/ESP8266)
4. **Integrar con plataformas cloud** (AWS IoT, Azure IoT Hub)
5. **Desarrollar dashboard web** personalizado

---

**Autor**: Proyecto Foro IoT - 2025  
**Versión**: 1.0  
**Última actualización**: Noviembre 2025
