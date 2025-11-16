<div align="center">

<img src="src/plata.svg" width="120" alt="Plata logo">

# Plata

**A lightweight, modern, and open-source PDF viewer**

Built with Qt6 and MuPDF

[![License: AGPL v3](https://img.shields.io/badge/License-AGPL_v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)
![Status](https://img.shields.io/badge/status-early%20development-orange)

</div>

## ✨ Features

- 🖼️ Modern UI and native performance with [Qt6](https://www.qt.io/)
- 📄 Fast and pixel-perfect PDF rendering powered by [MuPDF](https://mupdf.com/)
- 🧠 Focused on simplicity, usability, and responsiveness
- 🪟 Cross-platform support

## 🚀 Installation

There are no official packages yet, so you'll need to compile the project manually.

### Linux/macOS

Make sure you have **Qt6**, **CMake**, and **Make** installed.

```bash
cmake -G "Unix Makefiles" -B build .
cmake --build build
./build/plata
```

### Windows

Install **Visual Studio 2022** and **Qt6**.

> Replace `<Qt6 cmake config path>` with the path to your Qt6 installation
> (usually something like `C:/Qt/6.x.x/msvc2022_64/lib/cmake`).

```bash
cmake -B build -DCMAKE_PREFIX_PATH="<Qt6 cmake config path>" .
cmake --build build
windeployqt .\build\plata
.\build\plata
```

## 🛣️ Development Roadmap

- [x] Basic user interface implementation
- [x] Application icon
- [x] Single PDF page rendering
- [x] Tabs
- [x] Windows compatibility
- [ ] Full PDF rendering
- [ ] Zoom support
- [ ] Packaging
- [ ] Theming
- [ ] Selectable text
- [ ] Text search

## 🤝 Contributing

I welcome all kinds of contributions — whether code, bug reports, or feature
requests! Feel free to open an issue or start a discussion.
