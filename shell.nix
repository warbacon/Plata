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
    qt6.qtbase
    qtcreator
  ];
}
