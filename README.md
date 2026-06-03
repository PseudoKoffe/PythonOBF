Compilation using the MSYS2 UCRT64 terminal and the GCC compiler

< Compiling DLL >
g++ -static -shared -o libprotect.dll libprotect.cpp -Wl,--kill-at

< Compiling the obfuscator >
g++ -static -std=c++11 -o obfuscator.exe obfuscator.cpp
