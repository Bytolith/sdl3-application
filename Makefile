all:
	cmake --build build
	./build/app
setup:
	cmake -S . -B build -G Ninja
