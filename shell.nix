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
    qt6.full
    qtcreator
  ];

  shellHook = ''
    export C_INCLUDE_PATH=${pkgs.mupdf}/include/mupdf
    export CPLUS_INCLUDE_PATH=${pkgs.mupdf}/include/mupdf
    export LIBRARY_PATH=${pkgs.mupdf}/lib
  '';
}
