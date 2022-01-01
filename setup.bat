@echo off

:main
:: Environment Variables
set CMAKE=%~dp0\ThirdParty\CMake\bin\cmake.exe
set INTERMEDIATE_DIR=%~dp0.Intermediate\Windows
:: Setting Variables
set CONFIGURATIONS="Debug;Release;Shipping"
set Python=%~dp0\ThirdParty\Python37-32\windows\python.exe

if NOT EXIST %INTERMEDIATE_DIR% (
    mkdir %INTERMEDIATE_DIR%
)

:: Create a Virtual Environment for our Python
if NOT EXIST %~dp0\ThirdParty\Python37-32\VENV\Windows (
    %Python% -m venv %~dp0\ThirdParty\Python37-32\VENV\Windows
)
set Python=%~dp0\ThirdParty\Python37-32\VENV\Windows\Scripts\python.exe
:: Update our Virtual Environment Python
%Python% -m pip install -r %~dp0\ThirdParty\Python37-32\requirements.txt

pushd %~dp0
:: Now we can try building a sln project.
pushd .Intermediate\Windows
%CMAKE%  -G "Visual Studio 16 2019" -A x64 -DCMAKE_INSTALL_PREFIX:PATH="%INTERMEDIATE_DIR%\install" -DCMAKE_CONFIGURATION_TYPES:STRING=%CONFIGURATIONS% -DCMAKE_USE_OPENSSL:BOOL="False" -DCMAKE_USE_SCHANNEL:BOOL="True" "%~dp0"
popd
if %ERRORLEVEL% == 0 (
    echo [CMake] Success!
::  Create a Windows Shortcut from the intermediate folder's solution to the local directory.
    pushd %INTERMEDIATE_DIR%
    for %%f in (*.sln) do (
        if "%%~xf"==".sln" (
            powershell "$s=(New-Object -COM WScript.Shell).CreateShortcut('%~dp0/%%f.lnk');$s.TargetPath='%INTERMEDIATE_DIR%/%%f';$s.Save()"
        )
    )
    popd
) else (
    echo [CMake] Failure to construct CMake properly, please contact builds/tools team to assist you!
    pause
)
popd
