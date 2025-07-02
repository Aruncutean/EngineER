@echo off
setlocal enabledelayedexpansion

set "ZIP_PATH=C:\Program Files\7-Zip\7z.exe"
set "REPO_USER=Aruncutean"
set "REPO_NAME=EngineER"
set "TAG=libs-v1.0.0"

set "LIB_DIR=external"
set "TMP_DIR=downloads"
set "BASE_URL=https://github.com/%REPO_USER%/%REPO_NAME%/releases/download/%TAG%"

set "LIBS=tinyfiledialogs.7z sdl3.7z sdl3-image.7z glad.7z glm.7z imgui.7z bullet3.7z fbx.7z boost.7z"

echo ============================================
echo =  Setup librarii externe din GitHub Release
echo ============================================

if not exist "%LIB_DIR%" mkdir "%LIB_DIR%"
if not exist "%TMP_DIR%" mkdir "%TMP_DIR%"

echo.
echo === Descarcare si extragere librarii ===

for %%F in (%LIBS%) do (
    echo ----------------------------------------
    echo ====>> Descarc: %%F

    curl -L "%BASE_URL%/%%F" -o "%TMP_DIR%\%%F"

    REM Obține dimensiunea fișierului
    for %%S in ("%TMP_DIR%\%%F") do set "SIZE=%%~zS"

    if !SIZE! LSS 100 (
        echo [EROARE] %%F pare corupt sau lipseste de pe GitHub!
        del "%TMP_DIR%\%%F"
    ) else (
        echo ---->> Extrag %%F in %LIB_DIR%\%%~nF
        "%ZIP_PATH%" x "%TMP_DIR%\%%F" -o"%LIB_DIR%" -y >nul
    )
)

echo.
echo === Sterg fisiere temporare ===
rd /s /q "%TMP_DIR%"
