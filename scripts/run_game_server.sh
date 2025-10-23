#!/usr/bin/env bash
set -e

# --- CONFIG ---
GAME_PATH="$HOME/cpp-projects/FruityEngine/build/Game/Game"
VNC_GEOMETRY="1920x1080"
VNC_DEPTH="24"
# ---------------

# Start GPU X server if not running
if ! pgrep -x Xorg >/dev/null; then
    echo "[+] Starting GPU X server on :0 ..."
    sudo X :0 -noreset -logfile /var/log/Xorg.0.log &
    sleep 2
else
    echo "[=] Xorg already running on :0"
fi

# Start TurboVNC desktop
if ! /opt/TurboVNC/bin/vncserver -list | grep -q ":1"; then
    echo "[+] Starting TurboVNC desktop on :1 ..."
    /opt/TurboVNC/bin/vncserver :1 -geometry ${VNC_GEOMETRY} -depth ${VNC_DEPTH}
else
    echo "[=] TurboVNC already running on :1"
fi

# Launch the game through VirtualGL
echo "[+] Launching game with VirtualGL..."
export DISPLAY=:1
/opt/VirtualGL/bin/vglrun -d :0 "$GAME_PATH"

# When the game exits, optionally stop the servers
echo "[*] Game exited. Do you want to stop VNC and X? (y/n)"
read -r REPLY
if [[ "$REPLY" =~ ^[Yy]$ ]]; then
    echo "[+] Stopping TurboVNC..."
    /opt/TurboVNC/bin/vncserver -kill :1 || true
    echo "[+] Stopping Xorg..."
    sudo pkill Xorg || true
fi

echo "[✓] Done."

