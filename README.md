# Simulador de Misiones Artemis: Orion en LEO

## Ejecucion en Windows

1. Abra `ejecutar.bat` con doble clic. Mantenga los archivos en esta carpeta.
2. Elija una opcion del menu e ingrese los valores pedidos.
3. Use la opcion 7 para ver el reporte completo y la opcion 8 para guardarlo.
4. Use la opcion 0 para salir.

Se incluye `artemis.exe`, compilado para Windows x64. No necesita Python,
un entorno de desarrollo ni un compilador para ejecutarlo. El script fija esta
carpeta como directorio de trabajo: `reporte_artemis.txt` se genera aqui al
elegir la opcion 8 y confirmar. El reporte no existe hasta que se exporta.

**Estado de verificacion:** compilacion C11 con Wall, Wextra, Wpedantic y Werror mediante Clang de Zig 0.13.0, sin advertencias. El 9 de noviembre de 2026 se ejecutaron correctamente las pruebas del caso nominal, entradas invalidas, lineas largas, NUL, NaN/inf, desbordamientos, coma decimal, fin de entrada, pulso nulo, historial y exportacion de reportes. Un bloqueo inicial de Windows impidio las primeras pruebas; posteriormente el programa pudo ejecutarse. Los scripts no modifican las protecciones de Windows. No requiere ejecutarse como administrador.

### Si Windows bloquea el programa (Device Guard)

Al abrir `ejecutar.bat`, Windows puede mostrar un mensaje como **“artemis.exe ha sido bloqueado por la directiva de Device Guard de su organización”**. Ese mensaje puede ocurrir en cualquier dispositivo.

**Para lo sucedido realice las siguientes acciones:**

1. Abrir **Inicio → Seguridad de Windows**.
2. Entrar en **Control de aplicaciones y navegador**.
3. Abrir **Configuración de Control inteligente de aplicaciones**.
4. En caso de que este  **Activado** vas a seleccionar **Desactivado**.
5. Volver a a brir `ejecutar.bat`.

**Este cambio afecta a todas las aplicaciones, no solamente a Artemis.** Control inteligente de aplicaciones es una protección adicional y no reemplaza al antivirus; Microsoft Defender puede seguir activo. Microsoft indica que esta función no ofrece una excepción para permitir una sola aplicación. Luego de probar la aplicacion , la activa devuelta

## Recompilar despues de modificar el codigo

Ejecute `compilar.bat`. Detecta GCC, Clang o Zig disponibles en PATH. Para
recompilar se necesita uno de esos compiladores, instalado por separado con su
entorno de enlace para Windows. El compilador no esta incluido en esta carpeta.
Si no modifica el codigo, utilice directamente el ejecutable incluido.

Comando equivalente con GCC:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror main.c simulador.c validaciones.c -o artemis.exe -lm
.\artemis.exe
```

## Archivos y uso

- `main.c`: inicializacion y llamada al menu.
- `simulador.h`: constantes, estructuras y funciones publicas.
- `simulador.c`: calculos fisicos, menu, historial y reportes.
- `validaciones.h` y `validaciones.c`: ingreso seguro de datos.
- `artemis.exe`: programa compilado.
- `ejecutar.bat`: inicio con ventana que permanece abierta al finalizar.
- `compilar.bat`: compilacion y actualizacion del ejecutable si tiene exito.
- `PRUEBAS_MANUALES.md`: pasos y resultados esperados para verificar la interfaz.

Los decimales admiten punto o coma, sin separadores de miles: escriba `192000`,
no `192.000,0`. El caso inicial usa h=192000 m, masa=33000 kg, a=0.50 m/s^2
y pulso=10 s. La opcion 5 permite ensayos independientes; la opcion 6 restaura
el caso nominal. Se conservan los diez ultimos ensayos en memoria hasta salir.

## Modelo físico

Se considera una Tierra esférica, gravedad newtoniana y una órbita circular ideal, sin resistencia atmosférica ni otras perturbaciones. Todas las fórmulas usan unidades del Sistema Internacional.

### Datos del caso nominal

| Símbolo | Magnitud | Valor |
|---|---|---|
| $G$ | Constante de gravitación universal | $6{,}674 \times 10^{-11}\ \mathrm{N\,m^2/kg^2}$ |
| $M_T$ | Masa de la Tierra | $5{,}972 \times 10^{24}\ \mathrm{kg}$ |
| $R_T$ | Radio terrestre medio | $6\,371\,000\ \mathrm{m}$ |
| $h$ | Altitud de Orion | $192\,000\ \mathrm{m}$ |
| $m$ | Masa de Orion | $33\,000\ \mathrm{kg}$ |

El radio orbital se mide desde el centro de la Tierra:

$$
r = R_T + h = 6\,371\,000 + 192\,000 = 6\,563\,000\ \mathrm{m}
$$

### A. Fuerza gravitatoria y velocidad orbital

**Menú: opción 1.**

La fuerza gravitatoria apunta hacia el centro de la Tierra y es perpendicular a la velocidad tangencial. En una órbita circular ideal, proporciona la fuerza centrípeta necesaria para cambiar la dirección de la velocidad. No son dos fuerzas distintas que deban sumarse.

$$
F_g = G\frac{M_T m}{r^2}
\qquad
F_c = m\frac{v^2}{r}
$$

Aplicando la segunda ley de Newton en la dirección radial:

$$
G\frac{M_T m}{r^2} = m\frac{v^2}{r}
\quad\Longrightarrow\quad
v^2 = \frac{GM_T}{r}
\quad\Longrightarrow\quad
\boxed{v = \sqrt{\frac{GM_T}{r}}}
$$

La masa de Orion se cancela: la rapidez circular a ese radio no depende de la masa de la nave.

$$
v = \sqrt{\frac{(6{,}674 \times 10^{-11})(5{,}972 \times 10^{24})}{6\,563\,000}}
\approx \boxed{7\,792{,}948363\ \mathrm{m/s}}
$$

La igualdad de los módulos de las fuerzas se verifica numéricamente:

$$
F_g = F_c \approx \boxed{305\,362{,}099371\ \mathrm{N}}
$$

### B. Período orbital

**Menú: opción 2.**

Una vuelta completa tiene una longitud igual a la circunferencia de la órbita. Como la rapidez es constante, el período es distancia dividida por rapidez:

$$
\boxed{T = \frac{2\pi r}{v}}
$$

$$
T = \frac{2\pi(6\,563\,000)}{7\,792{,}948363}
\approx \boxed{5\,291{,}520391\ \mathrm{s}}
$$

$$
T_{\mathrm{min}} = \frac{T}{60}
\approx \boxed{88{,}192007\ \mathrm{min}}
$$

Los cálculos del programa conservan la precisión interna; el redondeo se aplica al mostrar los resultados.

### C. Aceleración centrípeta

**Menú: opción 3.**

La aceleración centrípeta es la aceleración gravitatoria a la altitud de la nave:

$$
\boxed{a_c = \frac{v^2}{r} = \frac{GM_T}{r^2}}
$$

$$
a_c = \frac{(6{,}674 \times 10^{-11})(5{,}972 \times 10^{24})}{(6\,563\,000)^2}
\approx \boxed{9{,}253397\ \mathrm{m/s^2}}
$$

Esta aceleración apunta al centro terrestre. Los astronautas y la cápsula están en caída libre conjunta: ese valor no significa que sientan una carga de apoyo equivalente dentro de la nave.

### D. Pulso del propulsor lateral

**Menú: opción 4.**

Se aplica una aceleración lateral de módulo constante durante un intervalo breve:

$$
a_{\mathrm{lat}} = 0{,}50\ \mathrm{m/s^2}
\qquad
\Delta t = 10{,}0\ \mathrm{s}
$$

Para calcular la desviación adicional respecto del movimiento sin propulsor, se utiliza una **aproximación local**: eje longitudinal en la dirección de la velocidad orbital inicial, eje lateral perpendicular y velocidad lateral relativa inicial nula.

$$
x(t) \approx vt
\qquad
y(t) \approx \frac{1}{2}a_{\mathrm{lat}}t^2
$$

Al eliminar el tiempo, la perturbación local tiene forma parabólica durante el encendido:

$$
\boxed{y(x) \approx \frac{a_{\mathrm{lat}}}{2v^2}x^2}
$$

La desviación lateral al finalizar el pulso es:

$$
d_{\mathrm{lat}} \approx \frac{1}{2}a_{\mathrm{lat}}(\Delta t)^2
= \frac{1}{2}(0{,}50)(10{,}0)^2
= \boxed{25{,}0\ \mathrm{m}}
$$

El cambio de velocidad lateral en esa misma aproximación es:

$$
\Delta v_{\mathrm{lat}} \approx a_{\mathrm{lat}}\Delta t
= (0{,}50)(10{,}0)
= \boxed{5{,}0\ \mathrm{m/s}}
$$

**Alcance del resultado:** los 25 m representan una desviación adicional aproximada respecto del movimiento sin propulsor, no una altitud final ni una solución exacta de la trayectoria geocéntrica. La nave deja de seguir el MCU original; al finalizar el empuje, su posición y velocidad determinan la nueva trayectoria bajo gravedad. Para imponer perpendicularidad exacta a la velocidad instantánea, hay que especificar la dirección del empuje —radial o fuera del plano orbital— e integrar las ecuaciones de movimiento.

### Consulta de resultados

Las opciones **1 a 4** muestran cada inciso por separado. La opción **7** reúne los cuatro análisis y el historial; la opción **8** los exporta a un archivo de texto. La opción **6** restablece los datos nominales del enunciado.

## Registro del desarrollo
1.-Utilizamos Gpt(6.1 Sol) y Gemini(3.1 Pro= como apoyo para generar la primera versión del código, organizar los cinco archivos, implementar validaciones y preparar pruebas y documentación. Mas que nada para guiarnos en la utilizacion de  funciones, estructuras, arreglos y punteros. La solución se verificó mediante compilación y pruebas de cálculos, entradas incorrectas, historial y reportes. Cada integrante debe estudiar y comprender el código para explicar sus fórmulas, funciones y decisiones de programación. Queda modificar (Estetica , errores de repeticion , limpieza de menu , etc)



