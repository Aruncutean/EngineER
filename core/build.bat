@echo off
setlocal enabledelayedexpansion

:: ========== CONFIGURARE ==========

:: Lista de ABI-uri pentru care vrei să compilezi
set ABI_LIST=x86_64

:: Platforma Android minimă
set PLATFORM=android-21

:: Directorul proiectului Android Studio
set ANDROID_LIB_ROOT=..\runtime-android\app\jni\myLib\lib

:: Numele folderului de build
set BUILD_ROOT=build

:: ========== LOOP PE ABI-URI ==========

for %%A in (%ABI_LIST%) do (
    set ABI=%%A
    echo ===============================
    echo ==== Compiling for !ABI!
    echo ===============================

    :: Setări CMake
    cmake -S . -B %BUILD_ROOT%/!ABI! ^
        -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK_HOME%\build\cmake\android.toolchain.cmake ^
        -DANDROID_ABI=!ABI! ^
        -DANDROID_PLATFORM=%PLATFORM% ^
        -DCMAKE_BUILD_TYPE=Release ^
        -G "Ninja"

    if errorlevel 1 (
        echo [!] CMake configuration failed for !ABI!
        exit /b 1
    )

    :: Build
    cmake --build %BUILD_ROOT%/!ABI! --config Release
    if errorlevel 1 (
        echo [!] Build failed for !ABI!
        exit /b 1
    )

      :: Copiere în folderul Android
      set DST=!ANDROID_LIB_ROOT!\!ABI!
      if not exist "!DST!" mkdir "!DST!"

      :: Copiază libCore.a
      if exist "%BUILD_ROOT%\!ABI!\libCore.a" (
          copy /Y "%BUILD_ROOT%\!ABI!\libCore.a" "!DST!\" >nul
          echo     [+] Copiat: libCore.a → !DST!
      )

:: Copiază header-ele (include)
echo ===============================
echo ==== Copiere include files
echo ===============================

set INCLUDE_SRC=include
set INCLUDE_DST=..\runtime-android\app\jni\myLib\include

if not exist "!INCLUDE_DST!" mkdir "!INCLUDE_DST!"

xcopy /E /Y /I "!INCLUDE_SRC!" "!INCLUDE_DST!\"
echo     [+] Include files copiate în !INCLUDE_DST!
)

endlocal
