{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  name = "plata";

  packages = with pkgs; [
    cmake
    just
    mupdf-headless
    ninja
    pkg-config
    qt6.qtbase
    qt6.qtsvg
    qt6.qttools
  ];

  shellHook = ''
    export QT_PLUGIN_PATH="${pkgs.qt6.qtbase}/lib/qt-6/plugins:${pkgs.qt6.qtsvg}/lib/qt-6/plugins"
  '';
}
