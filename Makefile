.PHONY: build config clean nuke
export CMAKE_BUILD_TYPE ?= Debug

build: build/CMakeCache.txt
	cmake --build build/

config: clean build/CMakeCache.txt

clean:
	rm -f build/CMakeCache.txt

nuke:
	rm -rf build/

build/CMakeCache.txt:
	mkdir -p build/
	cmake -B build/
