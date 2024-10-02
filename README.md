# ParseIntPy
 A Python binding for a fast integer parsing function in C++.

## Build Instructions
1. Install pybind11 via vcpkg or equivalent.
2. Install pybind11 via pip.
3. Enter the main folder of this repo and enter `cmake -S . -B ./Build -DCMAKE_BUILD_TYPE=Release_OR_Debug` then `cmake --build ./Build --config=Release_OR_Debug`.
4. Modify the Test.py file to point at the location of the generated .pyd file. Import from it parse_integer or parse_integers, and use them.