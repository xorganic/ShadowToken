#!/bin/bash

# Setări și directoare de curățat
BUILD_DIR="../build"
DEPLOY_DIR="../deploy"
OUTPUT_FILE="ShadowToken.exe"
DEPLOY_ARCHIVE="ShadowToken_v2025.tar.gz"

# Ștergerea binarului din directorul de build
if [ -f "$BUILD_DIR/$OUTPUT_FILE" ]; then
    echo "[Info] Ștergerea binarului $OUTPUT_FILE din $BUILD_DIR."
    rm "$BUILD_DIR/$OUTPUT_FILE"
fi

# Ștergerea arhivei de deploy
if [ -f "$DEPLOY_DIR/$DEPLOY_ARCHIVE" ]; then
    echo "[Info] Ștergerea arhivei $DEPLOY_ARCHIVE din $DEPLOY_DIR."
    rm "$DEPLOY_DIR/$DEPLOY_ARCHIVE"
fi

# Confirmare curățare
echo "[Success] Curățarea completă a fost efectuată."
