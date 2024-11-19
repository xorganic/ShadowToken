#!/bin/bash

# Setări și configurări
BUILD_DIR="../build"
OUTPUT_FILE="ShadowToken.exe"
CONFIG_DIR="../config"
DEPLOY_DIR="../deploy"
DEPLOY_ARCHIVE="ShadowToken_v2025.tar.gz"

# Verificare dacă build-ul a fost creat
if [ ! -f "$BUILD_DIR/$OUTPUT_FILE" ]; then
    echo "[Error] Binarul $OUTPUT_FILE nu există. Rulați build.sh înainte de deploy."
    exit 1
fi

# Crearea directorului de deploy dacă nu există
if [ ! -d "$DEPLOY_DIR" ]; then
    echo "[Info] Crearea directorului de deploy."
    mkdir -p "$DEPLOY_DIR"
fi

# Crearea arhivei de deploy
echo "[Info] Crearea arhivei de deploy..."
tar -czvf "$DEPLOY_DIR/$DEPLOY_ARCHIVE" -C "$BUILD_DIR" "$OUTPUT_FILE" -C "$CONFIG_DIR" .

# Verificare de erori
if [ $? -eq 0 ]; then
    echo "[Success] Arhiva de deploy creată în $DEPLOY_DIR/$DEPLOY_ARCHIVE"
else
    echo "[Error] Crearea arhivei de deploy a eșuat."
    exit 1
fi
