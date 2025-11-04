alias b := build
alias c := configure

run: build
    ./build/plata

build:
    cmake --build build/

configure:
    cmake -B build -G Ninja {{justfile_directory()}}

clean:
    rm -rf build/
