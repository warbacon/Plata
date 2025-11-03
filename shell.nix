{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  name = "plata";
  packages = with pkgs; [
    cmake
    just
    ninja
    pkg-config
    qt6.qtbase
    qt6.qtsvg
    qt6.qttools
  ];
}
