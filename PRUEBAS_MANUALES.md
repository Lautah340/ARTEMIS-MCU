# Guia de pruebas interactivas

Los casos equivalentes se verificaron con entradas automatizadas el 9 de noviembre de 2026. Esta guia permite repetirlos manualmente con `ejecutar.bat`. El fin de entrada se verifico mediante cierre del flujo; la combinacion de teclas Ctrl+Z queda para comprobacion manual.

1. Elegir 7. Verificar v=7792.948363 m/s, T=5291.520391 s,
   ac=9.253397 m/s^2, desviacion=25.000000 m y delta_v=5.000000 m/s.
2. En el menu escribir `hola`, una linea vacia, `1.5`, `9` y un entero
   de cien digitos. Debe pedir nuevamente una opcion sin cerrarse.
3. Pegar una linea de mas de 255 caracteres. Debe rechazarla completa;
   el siguiente ingreso valido debe funcionar sin procesar restos de la linea.
4. Elegir 5. Para altitud probar `nan`, `inf`, `1e999`, `0x10`, `192000abc`,
   `192.000,0`, `99999` y `2000001`. Debe rechazarlos.
5. Completar el ensayo con `192000`, `33000`, `0,50`, `10`.
   Elegir 7: debe reproducir el caso nominal y mostrar dos ensayos.
6. Elegir 5 y completar `192000`, `33000`, `0`, `10`. Elegir 4:
   desviacion y delta_v deben ser cero; debe indicar ausencia de perturbacion.
7. Elegir 6 doce veces, luego 7. El historial debe contener solo diez ensayos.
8. Elegir 8, ingresar `ss`: debe rechazarlo. Ingresar `n`: no exporta.
   Volver a elegir 8 e ingresar `S`: debe guardar `reporte_artemis.txt` en esta
   carpeta. Abrirlo y comprobar formulas, unidades e historial.
9. Cerrar la entrada con Ctrl+Z y Enter en una consola de Windows, tambien
   durante la configuracion de un ensayo. Debe terminar limpiamente sin
   reintentos infinitos ni guardar el ensayo incompleto.
10. Volver a iniciar y elegir 0: debe finalizar normalmente.

