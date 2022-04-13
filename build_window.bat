@echo off
mkdir build
cd build
set /P OPT=<cmake_option.txt
cmake -S %~dp0 %OPT% -B ./
pause