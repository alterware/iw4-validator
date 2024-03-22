![license](https://img.shields.io/github/license/alterware/iw4-validator.svg)
[![build](https://github.com/alterware/iw4-validator/workflows/Build/badge.svg)](https://github.com/alterware/iw4-validator/actions)

# AlterWare: iw4-validator
You may use this tool to validate "client effects" GSC files for the IW4 engine and files containing data from `sv_mapRotation` dvar.

## Build
- Install [Premake5][premake5-link] and add it to your system PATH
- Clone this repository using [Git][git-link]
- Update the submodules using ``git submodule update --init --recursive``
- Run Premake with either of these two options ``premake5 vs2022`` (Windows) or ``premake5 gmake2`` (Linux/macOS)

#### Command line arguments

- ```-createfx```
  - Parses a "create fx" file
    - Do not include a trailing backslash in the path
- ```-map-rotation```
  - Parses a "map rotation" file
    - Do not include a trailing backslash in the path

[premake5-link]:          https://premake.github.io
[git-link]:               https://git-scm.com
