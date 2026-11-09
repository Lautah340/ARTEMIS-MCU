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

## Modelo fisico

Tierra esferica, gravedad newtoniana y orbita circular ideal sin atmosfera.
Fg=G*M*m/r^2 proporciona la fuerza centripeta m*v^2/r; por ello
v=sqrt(G*M/r), T=2*pi*r/v y ac=G*M/r^2.

Resultados nominales: v=7792.948363 m/s; T=5291.520391 s (88.192007 min);
ac=9.253397 m/s^2; Fg=305362.099371 N.

El pulso lateral usa la aproximacion local y=0.5*a*t^2 y delta_v=a*t:
25 m y 5 m/s en el caso nominal. Es una perturbacion respecto al movimiento
sin propulsor en ejes locales, no una integracion exacta de la orbita ni una
altitud final. La perpendicularidad exacta a la velocidad instantanea necesita
especificar la direccion del empuje e integrar las ecuaciones dinamicas.

## Registro del desarrollo
1.-Utilizamos Gpt(6.1 Sol) y Gemini(3.1 Pro= como apoyo para generar la primera versión del código, organizar los cinco archivos, implementar validaciones y preparar pruebas y documentación. Mas que nada para guiarnos en la utilizacion de  funciones, estructuras, arreglos y punteros. La solución se verificó mediante compilación y pruebas de cálculos, entradas incorrectas, historial y reportes. Cada integrante debe estudiar y comprender el código para explicar sus fórmulas, funciones y decisiones de programación. Queda modificar (Estetica , errores de repeticion , limpieza de menu , etc)



