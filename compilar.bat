@echo off
setlocal
cd /d "%~dp0"
title Compilar Simulador Artemis
where gcc >nul 2>nul
if not errorlevel 1 goto gcc
where clang >nul 2>nul
if not errorlevel 1 goto clang
where zig >nul 2>nul
if not errorlevel 1 goto zig
echo No se encontro GCC, Clang ni Zig en PATH.
echo El ejecutable incluido puede usarse sin instalar un compilador.
echo Para modificar y recompilar el codigo, instale un compilador C y agreguelo a PATH.
pause
exit /b 1
:gcc
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror main.c simulador.c validaciones.c -o artemis_nuevo.exe -lm
goto verificar
:clang
clang -std=c11 -Wall -Wextra -Wpedantic -Werror main.c simulador.c validaciones.c -o artemis_nuevo.exe -lm
goto verificar
:zig
zig cc -std=c11 -Wall -Wextra -Wpedantic -Werror main.c simulador.c validaciones.c -o artemis_nuevo.exe -lm
:verificar
if errorlevel 1 (
    echo La compilacion fallo. Se conserva el ejecutable anterior.
    pause
    exit /b 1
)
move /y "artemis_nuevo.exe" "artemis.exe" >nul
if errorlevel 1 (
    echo No se pudo actualizar artemis.exe. Cierre el simulador y reintente.
    pause
    exit /b 1
)
echo Compilacion correcta. Abra ejecutar.bat para iniciar.
pause
exit /b 0
