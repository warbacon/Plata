{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  name = "plata";
  buildInputs = with pkgs; [
    cmake
    mupdf-headless
    ninja
    pkg-config
    freetype
    gumbo
    harfbuzz
    jbig2dec
    libjpeg
    openjpeg
    brotli
    qt6.full
    qtcreator
  ];
}
