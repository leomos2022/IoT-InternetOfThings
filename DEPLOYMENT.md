# Despliegue en GitHub Pages

## 🚀 Estado del Despliegue

✅ **Repositorio**: [https://github.com/leomos2022/IoT-InternetOfThings](https://github.com/leomos2022/IoT-InternetOfThings)

✅ **GitHub Pages**: [https://leomos2022.github.io/IoT-InternetOfThings/](https://leomos2022.github.io/IoT-InternetOfThings/)

---

## Configuración Realizada

### 1. GitHub Actions Workflow
- ✅ Archivo: `.github/workflows/deploy.yml`
- ✅ Trigger: Push a branch `main`
- ✅ Despliega desde la raíz del proyecto

### 2. Permisos Configurados
```yaml
permissions:
  contents: read
  pages: write
  id-token: write
```

### 3. Archivos Desplegados
- `index.html` - Aplicación principal
- `styles.css` - Estilos
- `script.js` - Lógica JavaScript
- `documentos/` - Contenido académico
- `practica_arduino/` - Código Arduino
- Imágenes y recursos

---

## Cómo Verificar el Despliegue

### Opción 1: GitHub Actions
1. Ir a: [https://github.com/leomos2022/IoT-InternetOfThings/actions](https://github.com/leomos2022/IoT-InternetOfThings/actions)
2. Verificar que el workflow "Deploy to GitHub Pages" se ejecute correctamente
3. El despliegue toma aproximadamente 1-2 minutos

### Opción 2: GitHub Pages Settings
1. Ir a: Settings → Pages
2. Verificar que Source sea "GitHub Actions"
3. Ver la URL de despliegue

---

## Actualizaciones Futuras

Para actualizar el sitio:

```bash
# 1. Hacer cambios en el código
git add .
git commit -m "Descripción de cambios"
git push origin main

# 2. GitHub Actions desplegará automáticamente
```

---

## Módulos Disponibles en el Sitio

### 🏭 Semana 3: Fábrica Inteligente
Dashboard interactivo con simulación de sistemas IoT industriales

### 🏙️ Semana 5: Futurópolis
Enlace a proyecto 3D de ciudad inteligente

### 🌾 Semana 7: Agricultura Inteligente
Sistema de agricultura de precisión con IoT + IA

---

## Solución de Problemas

### Si el sitio no se actualiza:
1. Verificar que el push se hizo correctamente: `git log`
2. Revisar GitHub Actions: Ver si hay errores en el workflow
3. Limpiar caché del navegador: Ctrl+F5 (Windows) o Cmd+Shift+R (Mac)
4. Esperar 2-3 minutos para que GitHub Pages se actualice

### Si hay error 404:
1. Verificar que `index.html` esté en la raíz del proyecto
2. Comprobar que el workflow use `path: '.'` no `path: './demo_web'`
3. Asegurar que GitHub Pages esté habilitado en Settings

---

**Última actualización**: 19 de Diciembre de 2025  
**Desplegado por**: GitHub Actions
