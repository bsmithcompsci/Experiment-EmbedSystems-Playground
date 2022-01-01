@echo off
set CMAKE=%~dp0\..\..\CMake\bin\cmake.exe
set INTERMEDIATE_DIR=%~dp0.Intermediate\Windows
set CONFIGURATIONS="Debug;Release;"

rd /s /q %INTERMEDIATE_DIR%
if NOT EXIST %INTERMEDIATE_DIR% (
    mkdir %INTERMEDIATE_DIR%
)

@del src\*.obj src\*.manifest src\minilua.exe src\buildvm.exe
@del src\host\buildvm_arch.h
@del src\lj_bcdef.h src\lj_ffdef.h src\lj_libdef.h src\lj_recdef.h src\lj_folddef.h

pushd %~dp0
:: Now we can try building a sln project.
pushd .Intermediate\Windows
%CMAKE%  -G "Visual Studio 16 2019" -A x64 -DCMAKE_INSTALL_PREFIX:PATH="%INTERMEDIATE_DIR%\install" -DCMAKE_CONFIGURATION_TYPES:STRING=%CONFIGURATIONS% "%~dp0"
popd
if %ERRORLEVEL% == 0 (
    echo [CMake] Success!
::  Create a Windows Shortcut from the intermediate folder's solution to the local directory.
    pushd %INTERMEDIATE_DIR%
    %CMAKE% --build . --config Release
    popd
) else (
    echo [CMake] Failure to construct CMake properly, please contact builds/tools team to assist you!
    pause
)
popd
