{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    mupdf-headless
    ninja
    qt6.full
    qtcreator
  ];
}
