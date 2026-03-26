#!/bin/bash

# Percorso della cartella contenente il tuo file .ino (dentro il container)
SKETCH_DIR="/root/ros2_ws/src/fra2mo_real/src/fra2mo_hardware/firmware"

echo "🛠️ Compilazione del firmware per Raspberry Pi Pico in corso..."

# Compila lo sketch
arduino-cli compile --fqbn rp2040:rp2040:rpipico "$SKETCH_DIR"

if [ $? -eq 0 ]; then
    echo "✅ Compilazione completata!"
    echo "⚡ Inizio il flashing sul Pico (Assicurati che sia collegato a /dev/ttyACM0)..."

    arduino-cli upload -p /dev/ttyACM0 --fqbn rp2040:rp2040:rpipico "$SKETCH_DIR"

    if [ $? -eq 0 ]; then
        echo "🚀 Firmware caricato con successo!"
    else
        echo "❌ Errore durante il flashing."
    fi
else
    echo "❌ Errore di compilazione."
fi