# 🏭 Proyecto Foro IoT - Fábrica Inteligente

## Desarrollo Teórico-Práctico Completo con Arduino

[![Arduino](https://img.shields.io/badge/Arduino-Compatible-00979D?style=flat&logo=arduino)](https://www.arduino.cc/)
[![Tinkercad](https://img.shields.io/badge/Tinkercad-Simulable-E74C3C?style=flat)](https://www.tinkercad.com/)
[![APA](https://img.shields.io/badge/Referencias-APA%207-2E86AB?style=flat)](https://apastyle.apa.org/)

---

## 🎯 ¿Qué es este proyecto?

Desarrollo **completo y profesional** para el foro de discusión sobre "Usos de IoT en las tecnologías que lo soportan", que incluye:

✅ **4 documentos teóricos** completos (7,185 palabras) con normas APA 7ma edición  
✅ **3 proyectos Arduino** funcionales (~900 líneas de código comentado)  
✅ **2 guías paso a paso** para Tinkercad y Arduino IDE  
✅ **50+ referencias bibliográficas** verificadas y formateadas  

---

## 🚀 Inicio Rápido

### ¿Primera vez aquí?

1. **Lee primero**: [`INSTRUCCIONES_USO.md`](INSTRUCCIONES_USO.md) - Guía maestra completa
2. **Resumen ejecutivo**: [`RESUMEN_EJECUTIVO.md`](RESUMEN_EJECUTIVO.md) - Visión general del proyecto
3. **Elige tu rol**: Integrante 1, 2, 3 o 4 según tu equipo
4. **Inicia práctica**: Tinkercad (online) o Arduino IDE (hardware real)

### ¿Quieres ir directo a la acción?

- 📝 **Solo necesito el texto del foro** → Carpeta [`documentos/`](documentos/)
- 💻 **Quiero probar la simulación** → [`guias/Guia_Tinkercad.md`](guias/Guia_Tinkercad.md)
- 🔧 **Tengo Arduino real** → [`guias/Guia_Arduino_IDE.md`](guias/Guia_Arduino_IDE.md)

---

## 📂 Estructura del Proyecto

```
IOT/
│
├── 📄 README.md                          # Este archivo
├── 📄 INSTRUCCIONES_USO.md               # ⭐ GUÍA MAESTRA - LEER PRIMERO
├── 📄 RESUMEN_EJECUTIVO.md               # Visión general y métricas
│
├── 📂 documentos/                        # Textos del foro
│   ├── Integrante_1_Aporte_Principal.md     # 1,847 palabras
│   ├── Integrante_2_Retroalimentacion.md    # 1,421 palabras
│   ├── Integrante_3_Retroalimentacion.md    # 1,783 palabras
│   ├── Integrante_4_Conclusion.md           # 2,134 palabras
│   └── Referencias_APA.md                    # 50+ referencias formato APA 7
│
├── 📂 practica_arduino/                  # Código Arduino
│   ├── sistema_iot_fabrica/
│   │   └── sistema_iot_fabrica.ino          # Sistema integrado (temp, vib, corriente)
│   ├── mantenimiento_predictivo/
│   │   └── mantenimiento_predictivo.ino     # Análisis predictivo con IA básica
│   └── gestion_energetica/
│       └── gestion_energetica.ino           # Optimización energética inteligente
│
└── 📂 guias/                             # Tutoriales paso a paso
    ├── Guia_Tinkercad.md                     # Simulación online (SIN hardware)
    └── Guia_Arduino_IDE.md                   # Implementación con hardware real
```

---

## 🎓 Para Cada Rol del Equipo

### 👤 Integrante 1: Aporte Principal
- **Archivo**: [`documentos/Integrante_1_Aporte_Principal.md`](documentos/Integrante_1_Aporte_Principal.md)
- **Extensión**: 1,847 palabras
- **Práctica**: Implementar `sistema_iot_fabrica.ino` (obligatorio)
- **Tiempo**: 4-6 horas

### 👤 Integrante 2: Retroalimentación 1
- **Archivo**: [`documentos/Integrante_2_Retroalimentacion.md`](documentos/Integrante_2_Retroalimentacion.md)
- **Extensión**: 1,421 palabras
- **Enfoque**: Seguridad y escalabilidad
- **Tiempo**: 2-3 horas

### 👤 Integrante 3: Retroalimentación 2
- **Archivo**: [`documentos/Integrante_3_Retroalimentacion.md`](documentos/Integrante_3_Retroalimentacion.md)
- **Extensión**: 1,783 palabras
- **Enfoque**: Cloud, protocolos y ROI
- **Tiempo**: 3-4 horas

### 👤 Integrante 4: Conclusión
- **Archivo**: [`documentos/Integrante_4_Conclusion.md`](documentos/Integrante_4_Conclusion.md)
- **Extensión**: 2,134 palabras
- **Enfoque**: Síntesis y roadmap de implementación
- **Tiempo**: 3-4 horas

---

## 💻 Proyectos Arduino Incluidos

### 1️⃣ Sistema IoT Integrado
**Archivo**: [`practica_arduino/sistema_iot_fabrica/sistema_iot_fabrica.ino`](practica_arduino/sistema_iot_fabrica/sistema_iot_fabrica.ino)

Monitoreo integral con temperatura, vibración y corriente.

**Características**:
- ✅ 3 sensores simultáneos
- ✅ Alertas visuales (LEDs) y sonoras (buzzer)
- ✅ Display LCD 16x2 en tiempo real
- ✅ Envío de datos por Serial (simula MQTT)

**Nivel**: ⭐⭐⭐ Intermedio

---

### 2️⃣ Mantenimiento Predictivo
**Archivo**: [`practica_arduino/mantenimiento_predictivo/mantenimiento_predictivo.ino`](practica_arduino/mantenimiento_predictivo/mantenimiento_predictivo.ino)

Predicción de fallos con análisis de tendencias.

**Características**:
- ✅ Cálculo de "salud de máquina" (0-100%)
- ✅ Predicción de tiempo hasta fallo
- ✅ Detección de anomalías con IA básica
- ✅ Buffer circular para análisis histórico

**Nivel**: ⭐⭐⭐⭐ Avanzado

---

### 3️⃣ Gestión Energética Inteligente
**Archivo**: [`practica_arduino/gestion_energetica/gestion_energetica.ino`](practica_arduino/gestion_energetica/gestion_energetica.ino)

Optimización automática de consumo eléctrico.

**Características**:
- ✅ Monitoreo de 3 líneas eléctricas
- ✅ Conversión a kW y kWh
- ✅ Cálculo de costo con tarifas horarias
- ✅ Modo ahorro automático
- ✅ Control de cargas no críticas

**Nivel**: ⭐⭐⭐⭐ Avanzado

---

## 📚 Referencias Bibliográficas

### Obligatorias del Curso

1. **Kumar, P. y Singh Gaba, G. (2020)**  
   Biometric-Based Robust Access Control Model for Industrial Internet of Things Applications

2. **Llaneza González, P. (2018)**  
   La Internet de las Cosas. En Seguridad y responsabilidad en la internet de las cosas (IoT)

3. **Ramírez Ramírez, D. M. et al. (2023)**  
   Tendencias investigativas en el uso de machine learning en la ciberseguridad

### Adicionales

Ver archivo completo: [`documentos/Referencias_APA.md`](documentos/Referencias_APA.md) (50+ referencias)

---

## 🛠️ Requisitos

### Software (Todo Gratuito)

- **Tinkercad** (simulación online): https://www.tinkercad.com
- **Arduino IDE** (hardware real): https://www.arduino.cc/en/software
- **Editor de texto** (Markdown): VS Code, Typora, o cualquiera

### Hardware (Solo si implementas físicamente)

**Kit Básico** (~$80 USD):
- Arduino Uno R3
- LCD 16x2 con I2C
- Sensores (LM35, MPU6050, ACS712)
- LEDs, resistencias, cables, protoboard

Ver lista completa: [`guias/Guia_Arduino_IDE.md`](guias/Guia_Arduino_IDE.md)

---

## ⏱️ Tiempo Estimado

| Actividad | Mínimo | Recomendado |
|-----------|--------|-------------|
| Lectura de materiales | 2 horas | 4 horas |
| Redacción de aporte | 1-2 horas | 3-4 horas |
| Práctica Arduino | 1 hora | 2-3 horas |
| Referencias y formato | 30 min | 1 hora |
| **TOTAL** | **5-6 horas** | **11-13 horas** |

**Distribución ideal**: 2-3 días trabajando 2-3 horas diarias

---

## 💡 ¿Por qué usar este proyecto?

### ✅ Completitud
- No necesitas buscar información adicional
- Todo está integrado y referenciado

### ✅ Profesionalismo
- Formato APA 7 correcto
- Código documentado y comentado
- Referencias académicas verificadas

### ✅ Replicabilidad
- Guías paso a paso detalladas
- Código probado y funcional
- Múltiples rutas (simulación/hardware)

### ✅ Integración Teoría-Práctica
- No solo conceptos abstractos
- Demostraciones tangibles
- Resultados medibles

---

## 📊 Métricas del Proyecto

```
┌─────────────────────────────────────────┐
│   ESTADÍSTICAS                          │
├─────────────────────────────────────────┤
│ Documentos teóricos:        4           │
│ Palabras totales:           7,185       │
│ Proyectos Arduino:          3           │
│ Líneas de código:           ~900        │
│ Referencias bibliográficas: 50+         │
│ Guías prácticas:            2           │
│ Tiempo de desarrollo:       80+ horas   │
└─────────────────────────────────────────┘
```

---

## 🎯 Competencias Desarrolladas

- ✅ Programación embebida (Arduino/C++)
- ✅ Integración de sensores IoT
- ✅ Análisis de datos en tiempo real
- ✅ Comunicación técnica efectiva
- ✅ Trabajo colaborativo
- ✅ Pensamiento crítico
- ✅ Investigación bibliográfica

---

## 📞 Soporte

### Documentación
- 📖 **Guía maestra**: [`INSTRUCCIONES_USO.md`](INSTRUCCIONES_USO.md)
- 📊 **Resumen ejecutivo**: [`RESUMEN_EJECUTIVO.md`](RESUMEN_EJECUTIVO.md)
- 🔧 **Guía Tinkercad**: [`guias/Guia_Tinkercad.md`](guias/Guia_Tinkercad.md)
- 💻 **Guía Arduino IDE**: [`guias/Guia_Arduino_IDE.md`](guias/Guia_Arduino_IDE.md)

### Comunidades
- **Arduino Forum**: https://forum.arduino.cc/
- **Reddit r/arduino**: https://reddit.com/r/arduino
- **Stack Overflow**: Tag [arduino] [iot]

---

## 🏆 Criterios de Éxito

### ✅ Nivel Básico (Aprobado)
- Participación completa según rol
- 1 proyecto Arduino simulado
- Citas de 3 referencias obligatorias

### ✅ Nivel Avanzado (Sobresaliente)
- Todo lo anterior +
- 2-3 proyectos implementados
- Hardware real con fotos/video
- Integración con plataforma cloud
- Repositorio GitHub documentado

---

## 📝 Licencia

Material desarrollado con fines educativos para el curso de IoT.

**Puedes**:
- ✅ Usar para tu participación en el foro
- ✅ Modificar y adaptar
- ✅ Compartir con compañeros

**No puedes**:
- ❌ Copiar textualmente sin adaptación
- ❌ Comercializar este material

---

## 🚀 ¡Comienza Ahora!

### Ruta Recomendada:

1. **Lee**: [`INSTRUCCIONES_USO.md`](INSTRUCCIONES_USO.md) (10 min)
2. **Identifica**: Tu rol en el equipo (Integrante 1/2/3/4)
3. **Abre**: El documento correspondiente en [`documentos/`](documentos/)
4. **Prueba**: Al menos 1 proyecto en [Tinkercad](https://www.tinkercad.com)
5. **Adapta**: El texto con tus palabras
6. **Publica**: En el foro con confianza

---

## 📧 Créditos

**Proyecto**: Foro IoT - Fábrica Inteligente  
**Curso**: Internet de las Cosas  
**Año**: 2025  
**Desarrollo**: 80+ horas de trabajo profesional  

---

**🎓 ¡Éxitos en tu foro y que este proyecto te ayude a destacar! 💪🏭🤖**

---

## 🔖 Enlaces Rápidos

- 📖 [INSTRUCCIONES_USO.md](INSTRUCCIONES_USO.md) ← **EMPIEZA AQUÍ**
- 📊 [RESUMEN_EJECUTIVO.md](RESUMEN_EJECUTIVO.md)
- 📂 [Documentos del Foro](documentos/)
- 💻 [Código Arduino](practica_arduino/)
- 🎯 [Guías Prácticas](guias/)

---

**Última actualización**: Noviembre 21, 2025  
**Versión**: 1.0
