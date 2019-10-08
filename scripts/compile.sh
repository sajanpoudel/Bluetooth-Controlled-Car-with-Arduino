#!/bin/sh
# Compiles the sketch for an Arduino Uno with arduino-cli (the sketch folder must match the .ino name).
set -e
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TMP="$(mktemp -d)"
mkdir -p "$TMP/bluetoothcontrolledcar"
cp "$ROOT"/*.ino "$ROOT"/*.h "$TMP/bluetoothcontrolledcar/" 2>/dev/null || cp "$ROOT"/*.ino "$TMP/bluetoothcontrolledcar/"
arduino-cli compile --fqbn "${FQBN:-arduino:avr:uno}" "$TMP/bluetoothcontrolledcar" > /dev/null
rm -rf "$TMP"
echo "sketch compiles"
