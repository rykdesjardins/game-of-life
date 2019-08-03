mkdir -p build/dev
mkdir -p build/release

g++ main.cpp src/*.cpp -o build/dev/gameoflife
chmod +x build/dev/gameoflife
