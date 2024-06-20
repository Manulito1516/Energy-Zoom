g++ src\**.cpp src\**\**.cpp -IC:\MinGW\include\SDL2 -LC:\MinGW\lib -w -W -isystem windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o build\EZ.exe
pause
cd build
EZ.exe
pause
