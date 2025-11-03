alias b := build
alias c := configure

run: build
    ./build/Plata

build:
    ninja -C build/

configure:
    cmake -B build -G Ninja .

clean:
    rm -rf build
