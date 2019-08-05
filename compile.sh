mkdir -p build/dev
mkdir -p build/release

g++ -g -rdynamic main.cpp src/*.cpp -o build/dev/gameoflife -lSDL2
chmod +x build/dev/gameoflife
