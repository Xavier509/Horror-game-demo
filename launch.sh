#!/bin/bash
pkill -9 Xvfb x11vnc websockify 2>/dev/null
sleep 1
export DISPLAY=:99
Xvfb :99 -screen 0 1280x720x24 +extension GLX &
sleep 3
x11vnc -display :99 -forever -shared -rfbport 5900 -nopw -quiet &
sleep 2
cd /usr/share/novnc && websockify --web=/usr/share/novnc 6080 localhost:5900 &
sleep 2
echo "Open: http://localhost:6080/vnc_lite.html"
cd /workspaces/Horror-game-demo/build
SDL_VIDEODRIVER=x11 DISPLAY=:99 ./MansionHorror
pkill -9 websockify x11vnc Xvfb
