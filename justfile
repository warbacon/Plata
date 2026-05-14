alias b := build
alias c := configure

run: build
    ./build/plata

build: configure
    cmake --build build/

configure:
    cmake -B build -G "Unix Makefiles" {{justfile_directory()}}

clean:
    rm -rf build/
