{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    mupdf-headless
    ninja
    pkg-config
    qt6.full
    qtcreator
  ];
}
