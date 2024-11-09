#!/bin/bash

# Setări și configurări de compilare
SRC_DIR="../src"
BUILD_DIR="../build"
OUTPUT_FILE="ShadowToken.exe"
COMPILER="g++"
CXXFLAGS="-O2 -std=c++17 -Wall"
INCLUDE_DIRS="-I../src -I../src/core -I../src/utils -I../src/evasion -I../src/privilege_escalation -I../src/fileless_injection -I../src/c2_communication -I../src/persistence"

# Verificare dacă directorul de build există
if [ ! -d "$BUILD_DIR" ]; then
    echo "[Info] Crearea directorului de build."
    mkdir -p "$BUILD_DIR"
fi

# Comanda de compilare
echo "[Info] Compilarea fișierelor sursă..."
$COMPILER $CXXFLAGS $INCLUDE_DIRS -o "$BUILD_DIR/$OUTPUT_FILE" $(find $SRC_DIR -type f -name "*.cpp")

# Verificare de erori
if [ $? -eq 0 ]; then
    echo "[Success] Compilare completă. Executabilul se află în $BUILD_DIR/$OUTPUT_FILE"
else
    echo "[Error] Compilare eșuată."
    exit 1
fi
