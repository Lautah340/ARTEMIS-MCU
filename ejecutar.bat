@echo off
setlocal
cd /d "%~dp0"
title Simulador de Misiones Artemis
if not exist "artemis.exe" (
    echo No se encontro artemis.exe. Primero ejecute compilar.bat.
    pause
    exit /b 1
)
"%~dp0artemis.exe"
set "resultado=%errorlevel%"
if not "%resultado%"=="0" (
    echo.
    echo No se pudo ejecutar correctamente. Codigo: %resultado%
    echo Si Windows informa un bloqueo, consulte las instrucciones de README.md.
)
echo.
pause
exit /b %resultado%
