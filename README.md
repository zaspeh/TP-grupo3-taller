**Manual de Usuario - Duck Game**  
**Grupo 3:** Matias Kaled Dib, Joseph Anthony Mamani Tipula, Martina Rey  

---

### **Introducción**  
Este documento describe cómo compilar y ejecutar el trabajo práctico, incluyendo los requisitos del sistema, el proceso de instalación y una guía de uso.

---

### **Requisitos Previos**  
Para compilar y ejecutar el programa, asegúrese de contar con:  
- **Compilador de C++**  
- **Make**  
- **SDL2** y **SDL2_image**  
- **pthread**  

---

### **Instalación de Dependencias**  
En sistemas basados en Debian/Ubuntu, ejecute el siguiente comando en la terminal:  
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev
```

---

### **Compilación**  
1. Abra una terminal en el directorio del proyecto.  
2. Ejecute el siguiente comando:  
   ```bash
   make all
   ```  

Este proceso generará dos ejecutables:  
- **client**  
- **server**  

---

### **Ejecución**  

#### **Iniciar el Servidor**  
Para iniciar el servidor, utilice:  
```bash
./server 8080
```  
Donde `8080` es el puerto en el que escuchará el servidor.  

#### **Conectar Clientes**  
Para conectar un cliente, utilice:  
```bash
./client localhost 8080
```  
Donde:  
- `localhost` es la dirección del servidor.  
- `8080` es el puerto del servidor.  

---

### **Configuración de Jugadores**  

#### **Capacidad**  
El juego admite hasta 4 jugadores simultáneos.  

#### **Asignación de IDs**  
Los IDs deben asignarse siguiendo este orden específico:  
1. Jugador 1: **ID 0**  
2. Jugador 2: **ID 1**  
3. Jugador 3: **ID 2**  
4. Jugador 4: **ID 3**  

**IMPORTANTE:** Los IDs deben ingresarse exactamente en este orden para un correcto funcionamiento del juego.  

---

### **Códigos Especiales**  
Durante el juego, puede utilizar los siguientes códigos especiales:  

| **Código** | **Función**              |  
|------------|--------------------------|  
| **F1**     | Activa balas infinitas   |  
| **F2** + [1-9] | Selecciona un arma específica |  
| **F3**     | Otorga armadura          |  
| **F4**     | Otorga casco             |  

---

### **Especificaciones Técnicas**  
El programa utiliza:  
- **Estándar C++17**  
- Flags de depuración: `-ggdb -DDEBUG -fno-inline`  
- Optimizaciones de compilación  
- Tratamiento estricto de errores  

---

### **Solución de Problemas**  
Si encuentra algún error durante la compilación o ejecución, verifique:  
1. Que todas las dependencias estén instaladas correctamente.  
2. Que el puerto especificado esté disponible.  
3. Que los IDs de los jugadores se ingresen en el orden correcto.  

---

### **Apéndice**  

#### **Estructura de Archivos**  
El proyecto está organizado en:  
- **Código del cliente**  
- **Código del servidor**  
- **Código común**  

#### **Makefile**  
El proyecto incluye un `Makefile` configurado con:  
- Soporte para **C++17**  
- Linkeo con **SDL2** y **SDL2_image**  
- Opciones de depuración  
- Optimizaciones de compilación
