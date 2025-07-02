@echo off
setlocal ENABLEDELAYEDEXPANSION

:: Set path to 7-Zip
set ZIP_PATH="C:\Program Files\7-Zip\7z.exe"

:: Folder containing all your libraries
set LIB_DIR=external

:: Output folder for .7z files
set OUT_DIR=archives

:: Create output folder if it doesn't exist
if not exist %OUT_DIR% (
    mkdir %OUT_DIR%
)

echo =============================
echo Archiving external libraries...
echo =============================

pushd %LIB_DIR%
for /D %%F in (*) do (
    echo Archiving %%F ...
    %ZIP_PATH% a -t7z "..\%OUT_DIR%\%%F.7z" "%%F\"
)
popd

echo =============================
echo All libraries archived into: %OUT_DIR%
echo =============================

pause
