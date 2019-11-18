# CMakeList.txt: Projekt CMake pro remc2-cmake, sem přidejte logiku zdrojového
# kódu a definic specifickou pro projekt.
#
cmake_minimum_required (VERSION 3.8)

# Přidejte zdrojový kód ke spustitelnému souboru tohoto projektu.
add_executable (remc2-cmake "remc2-cmake.cpp" "remc2-cmake.h")

# TODO: V případě potřeby přidejte testy a cíle instalace.
