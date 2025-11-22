# 🌐 Demo Web Interactiva - Sistema IoT Fábrica Inteligente

## 📋 Descripción

Dashboard web interactivo que simula en tiempo real la implementación de sistemas IoT en una fábrica inteligente. Incluye tres módulos principales:

1. **Gestión Energética**: Monitoreo de consumo en kW con gráficos históricos
2. **Mantenimiento Predictivo**: Scoring de salud 0-100% con alertas automáticas
3. **Sistema IoT Integrado**: Lecturas en tiempo real de temperatura, vibración y corriente

## 🚀 Cómo Usar

### Método 1: Abrir directamente en el navegador

1. Navega a la carpeta `demo_web/`
2. Haz doble clic en `index.html`
3. El dashboard se abrirá automáticamente en tu navegador predeterminado

### Método 2: Servidor local (recomendado)

```bash
cd /Users/leomos/Desktop/IOT/demo_web
python3 -m http.server 8000
```

Luego abre en tu navegador: `http://localhost:8000`

### Método 3: Live Server (VS Code)

1. Instala la extensión "Live Server" en VS Code
2. Click derecho en `index.html` → "Open with Live Server"

## 📁 Estructura de Archivos

```
demo_web/
├── index.html          # Estructura HTML principal
├── styles.css          # Estilos y diseño responsive
├── script.js           # Lógica de simulación y gráficos
└── README.md           # Esta documentación
```

## ✨ Características

- ✅ **Simulación en tiempo real** con actualización cada 2 segundos
- ✅ **Gráficos interactivos** usando Chart.js
- ✅ **Alertas automáticas** cuando se detectan anomalías
- ✅ **Registro de eventos** con timestamps
- ✅ **Diseño responsive** funciona en desktop, tablet y móvil
- ✅ **Sin dependencias externas** (excepto Chart.js desde CDN)
- ✅ **Navegación suave** entre secciones
- ✅ **Animaciones fluidas** al hacer scroll

## 🎨 Secciones del Dashboard

### 1. Introducción
- Contexto teórico sobre IoT industrial
- Estadísticas clave (30% ahorro energético, 70% reducción paradas)

### 2. Dashboard en Tiempo Real
- **Cards de sistema**: 3 tarjetas con métricas principales
- **Gráficos**: Consumo energético (línea) y salud del equipo (dona)
- **Logs en vivo**: Registro cronológico de eventos

### 3. Sistemas Implementados
- Descripción detallada de cada sistema IoT
- Ventajas y características técnicas

### 4. Beneficios Empresariales
- 6 beneficios clave con explicación

### 5. Referencias Académicas
- Citas en formato APA 7ª Edición
- Información del proyecto

## 🔧 Personalización

### Modificar intervalos de actualización

En `script.js`, línea 8:
```javascript
updateInterval: 2000, // Cambiar a 5000 para actualizar cada 5 seg
```

### Ajustar umbrales de alerta

En `script.js`, líneas 12-21:
```javascript
tempMax: 40,          // Temperatura máxima antes de alerta
vibrationMax: 1000,   // Vibración máxima antes de alerta
healthThresholds: {
    excellent: 90,    // >90% = Excelente
    good: 75,         // 75-90% = Bueno
    warning: 60,      // 60-75% = Advertencia
    critical: 40      // <60% = Crítico
}
```

### Cambiar colores del tema

En `styles.css`, líneas 7-14:
```css
--primary-color: #2563eb;    /* Azul principal */
--secondary-color: #7c3aed;  /* Púrpura secundario */
--success-color: #10b981;    /* Verde éxito */
--warning-color: #f59e0b;    /* Amarillo advertencia */
```

## 📊 Datos Simulados

La aplicación genera datos realistas basados en:

- **Consumo energético**: Varía según hora del día (mayor 8-18h)
- **Temperatura**: Base 28°C con variaciones ±8°C
- **Vibración**: Base 600 unidades con variaciones ±200
- **Salud del equipo**: Calculada automáticamente según temperatura y vibración

Los algoritmos incluyen:
```javascript
// Penalización por temperatura elevada
if (temperature > 35°C) {
    healthScore -= (temperature - 35) * 3
}

// Penalización por vibración excesiva
if (vibration > 800) {
    healthScore -= (vibration - 800) * 0.1
}
```

## 🎯 Uso Académico

Este dashboard es ideal para:

- ✅ Presentaciones de proyectos IoT
- ✅ Demostraciones en vivo de sistemas industriales
- ✅ Material complementario para documentación técnica
- ✅ Evidencia visual de implementación práctica
- ✅ Compartir con profesores y compañeros (no requiere instalación)

## 🌐 Compartir la Demo

### Opción 1: Archivo ZIP
Comprimir la carpeta `demo_web/` y compartir por email o plataforma educativa

### Opción 2: GitHub Pages
1. Subir archivos a repositorio GitHub
2. Activar GitHub Pages en Settings
3. Compartir URL pública: `https://tuusuario.github.io/proyecto-iot`

### Opción 3: Netlify Drop
1. Ir a https://app.netlify.com/drop
2. Arrastrar carpeta `demo_web/`
3. Obtener URL pública instantánea

## 🔍 Navegación por Consola

Abre la consola del navegador (F12) y ejecuta:

```javascript
// Ver estado actual del sistema
IoTDashboard.getState()

// Generar reporte completo
IoTDashboard.generateReport()

// Reiniciar simulación
IoTDashboard.resetSimulation()
```

## 📱 Compatibilidad

- ✅ Chrome 90+
- ✅ Firefox 88+
- ✅ Safari 14+
- ✅ Edge 90+
- ✅ Dispositivos móviles (iOS/Android)

## 🐛 Solución de Problemas

### Los gráficos no aparecen
- **Causa**: Chart.js no cargó desde CDN
- **Solución**: Verifica conexión a internet o descarga Chart.js localmente

### Los datos no se actualizan
- **Causa**: JavaScript bloqueado o error en consola
- **Solución**: Abre consola (F12) y verifica mensajes de error

### El diseño se ve mal en móvil
- **Causa**: Viewport no configurado
- **Solución**: Verifica que `index.html` tenga: `<meta name="viewport" content="width=device-width, initial-scale=1.0">`

## 📚 Recursos Adicionales

- **Chart.js Docs**: https://www.chartjs.org/docs/
- **MDN Web Docs**: https://developer.mozilla.org/
- **Can I Use**: https://caniuse.com/ (compatibilidad navegadores)

## 📧 Soporte

Para preguntas sobre la implementación o personalización, consulta:
- Código fuente comentado en `script.js`
- Documentación CSS en `styles.css`
- Estructura HTML en `index.html`

---

**Nota**: Esta es una simulación educativa. Los datos mostrados son generados algorítmicamente y no representan lecturas reales de sensores IoT.

**Proyecto**: Sistema IoT - Fábrica Inteligente  
**Fecha**: Noviembre 2025  
**Tecnologías**: HTML5, CSS3, JavaScript ES6, Chart.js
