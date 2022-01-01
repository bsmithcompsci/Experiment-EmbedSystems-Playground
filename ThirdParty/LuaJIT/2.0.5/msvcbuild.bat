@rem Script to build LuaJIT with MSVC.
@rem Copyright (C) 2005-2017 Mike Pall. See Copyright Notice in luajit.h
@rem
@rem Either open a "Visual Studio .NET Command Prompt"
@rem (Note that the Express Edition does not contain an x64 compiler)
@rem -or-
@rem Open a "Windows SDK Command Shell" and set the compiler environment:
@rem     setenv /release /x86
@rem   -or-
@rem     setenv /release /x64
@rem
@rem Then cd to this directory and run this script.
@echo off
@if not defined INCLUDE goto :TRYVSVAR
:TRYVSVAR
@if EXIST "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars64.bat" call "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars64.bat"
@if EXIST "C:\Program Files(x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" call "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars64.bat"
@if not defined INCLUDE goto :FAIL

@setlocal
@set OUTDIR=%~dp0Windows\x64
@set SRCDIR=%~dp0src
@set LJCOMPILE=cl /nologo /c /O2 /W3 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_STDIO_INLINE=__declspec(dllexport)__inline
@set LJLINK=link /nologo
@set LJMT=mt /nologo
@set LJLIB=lib /nologo /nodefaultlib
@set DASMDIR=%~dp0dynasm
@set DASM=%DASMDIR%\dynasm.lua
@set LJDLLNAME=lua51.dll
@set LJLIBNAME=lua51.lib
@set ALL_LIB=%SRCDIR%\lib_base.c %SRCDIR%\lib_math.c %SRCDIR%\lib_bit.c %SRCDIR%\lib_string.c %SRCDIR%\lib_table.c %SRCDIR%\lib_io.c %SRCDIR%\lib_os.c %SRCDIR%\lib_package.c %SRCDIR%\lib_debug.c %SRCDIR%\lib_jit.c %SRCDIR%\lib_ffi.c

@echo Cleaning up

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@del /s /Q %OUTDIR%\*.obj %OUTDIR%\*.manifest %OUTDIR%\minilua.exe %OUTDIR%\buildvm.exe
@del /s /Q %SRCDIR%\host\buildvm_arch.h
@del /s /Q %SRCDIR%\lj_bcdef.h %SRCDIR%\lj_ffdef.h %SRCDIR%\lj_libdef.h %SRCDIR%\lj_recdef.h %SRCDIR%\lj_folddef.h

@del /s /Q %OUTDIR%\bin
@del /s /Q %OUTDIR%\lib

mkdir %OUTDIR%\bin
mkdir %OUTDIR%\lib

pushd %OUTDIR%

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@echo Compiling minilua
echo %LJCOMPILE% %SRCDIR%\host\minilua.c
%LJCOMPILE% %SRCDIR%\host\minilua.c
if %errorlevel% neq 0 goto :BAD
echo %LJLINK% /out:%OUTDIR%\minilua.exe %OUTDIR%\minilua.obj
%LJLINK% /out:%OUTDIR%\minilua.exe %OUTDIR%\minilua.obj
if %errorlevel% neq 0 goto :BAD
if exist %OUTDIR%\minilua.exe.manifest^
  %LJMT% -manifest %OUTDIR%\minilua.exe.manifest -outputresource:%OUTDIR%\minilua.exe

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@echo Generating with minilua
@set DASMFLAGS=-D WIN -D JIT -D FFI -D P64
@set LJARCH=x64
echo minilua
@minilua
@if errorlevel 8 goto :X64
@set DASMFLAGS=-D WIN -D JIT -D FFI
@set LJARCH=x86
:X64
echo minilua %DASM% -LN %DASMFLAGS% -o %SRCDIR%\host\buildvm_arch.h %SRCDIR%\vm_x86.dasc
minilua %DASM% -LN %DASMFLAGS% -o %SRCDIR%\host\buildvm_arch.h %SRCDIR%\vm_x86.dasc
if %errorlevel% neq 0 goto :BAD

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@echo Compiling buildVM
echo %LJCOMPILE% /I "%SRCDIR%" /I %DASMDIR% %SRCDIR%\host\buildvm*.c
%LJCOMPILE% /I "%SRCDIR%" /I %DASMDIR% %SRCDIR%\host\buildvm*.c
if %errorlevel% neq 0 goto :BAD
echo %LJLINK% /out:%OUTDIR%\buildvm.exe %OUTDIR%\buildvm*.obj
%LJLINK% /out:%OUTDIR%\buildvm.exe %OUTDIR%\buildvm*.obj
if %errorlevel% neq 0 goto :BAD
if exist %OUTDIR%\buildvm.exe.manifest^
  %LJMT% -manifest %OUTDIR%\buildvm.exe.manifest -outputresource:%OUTDIR%\buildvm.exe

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@echo Generating with buildVM
echo buildvm -m peobj -o %OUTDIR%\lj_vm.obj
buildvm -m peobj -o %OUTDIR%\lj_vm.obj
if %errorlevel% neq 0 goto :BAD
echo buildvm -m bcdef -o %SRCDIR%\lj_bcdef.h %ALL_LIB%
buildvm -m bcdef -o %SRCDIR%\lj_bcdef.h %ALL_LIB%
if %errorlevel% neq 0 goto :BAD
echo buildvm -m ffdef -o %SRCDIR%\lj_ffdef.h %ALL_LIB%
buildvm -m ffdef -o %SRCDIR%\lj_ffdef.h %ALL_LIB%
if %errorlevel% neq 0 goto :BAD
echo buildvm -m libdef -o %SRCDIR%\lj_libdef.h %ALL_LIB%
buildvm -m libdef -o %SRCDIR%\lj_libdef.h %ALL_LIB%
if %errorlevel% neq 0 goto :BAD
echo buildvm -m recdef -o %SRCDIR%\lj_recdef.h %ALL_LIB%
buildvm -m recdef -o %SRCDIR%\lj_recdef.h %ALL_LIB%
if %errorlevel% neq 0 goto :BAD
echo buildvm -m vmdef -o %SRCDIR%\jit\vmdef.lua %ALL_LIB%
buildvm -m vmdef -o %SRCDIR%\jit\vmdef.lua %ALL_LIB%
if %errorlevel% neq 0 goto :BAD
echo buildvm -m folddef -o %SRCDIR%\lj_folddef.h %SRCDIR%\lj_opt_fold.c
buildvm -m folddef -o %SRCDIR%\lj_folddef.h %SRCDIR%\lj_opt_fold.c
if %errorlevel% neq 0 goto :BAD


:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@echo Compiling Lua

@if "%1" neq "debug" goto :NODEBUG
@shift
@set LJCOMPILE=%LJCOMPILE% /Zi
@set LJLINK=%LJLINK% /debug
:NODEBUG
@if "%1"=="amalg" goto :AMALGDLL
@if "%1"=="static" goto :STATIC

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Dynamic (SHARED)

echo %LJCOMPILE% /MD /DLUA_BUILD_AS_DLL %SRCDIR%\lj_*.c %SRCDIR%\lib_*.c
%LJCOMPILE% /MD /DLUA_BUILD_AS_DLL %SRCDIR%\lj_*.c %SRCDIR%\lib_*.c
if %errorlevel% neq 0 goto :BAD
echo %LJLINK% /DLL /out:%LJDLLNAME% %OUTDIR%\lj_*.obj %OUTDIR%\lib_*.obj
%LJLINK% /DLL /out:%LJDLLNAME% %OUTDIR%\lj_*.obj %OUTDIR%\lib_*.obj
if %errorlevel% neq 0 goto :BAD
@goto :MTDLL

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:STATIC
echo %LJCOMPILE% %SRCDIR%\lj_*.c %SRCDIR%\lib_*.c
%LJCOMPILE% %SRCDIR%\lj_*.c %SRCDIR%\lib_*.c
if %errorlevel% neq 0 goto :BAD
echo %LJLIB% /out:%OUTDIR%\%LJLIBNAME% %SRCDIR%\lj_*.obj %SRCDIR%\lib_*.obj
%LJLIB% /out:%OUTDIR%\%LJLIBNAME% %OUTDIR%\lj_*.obj %OUTDIR%\lib_*.obj
if %errorlevel% neq 0 goto :BAD
@goto :MTDLL

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:AMALGDLL
echo %LJCOMPILE% /MD /DLUA_BUILD_AS_DLL %SRCDIR%\ljamalg.c
%LJCOMPILE% /MD /DLUA_BUILD_AS_DLL %SRCDIR%\ljamalg.c
if %errorlevel% neq 0 goto :BAD
echo %LJLINK% /DLL /out:%LJDLLNAME% %OUTDIR%\ljamalg.obj %OUTDIR%\lj_vm.obj
%LJLINK% /DLL /out:%LJDLLNAME% %OUTDIR%\ljamalg.obj %OUTDIR%\lj_vm.obj
if %errorlevel% neq 0 goto :BAD

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:MTDLL
if exist %OUTDIR%\%LJDLLNAME%.manifest^
  %LJMT% -manifest %OUTDIR%\%LJDLLNAME%.manifest -outputresource:%OUTDIR%\%LJDLLNAME%;2

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@echo Compiling LuaJIT
echo %LJCOMPILE% %SRCDIR%\luajit.c
%LJCOMPILE% %SRCDIR%\luajit.c
if %errorlevel% neq 0 goto :BAD
echo %LJLINK% /out:%OUTDIR%\luajit.exe %OUTDIR%\luajit.obj %OUTDIR%\%LJLIBNAME%
%LJLINK% /out:%OUTDIR%\luajit.exe %OUTDIR%\luajit.obj %OUTDIR%\%LJLIBNAME%
if %errorlevel% neq 0 goto :BAD
if exist %OUTDIR%\luajit.exe.manifest^
  %LJMT% -manifest %OUTDIR%\luajit.exe.manifest -outputresource:%OUTDIR%\luajit.exe

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

echo Wrapping up packages

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

xcopy %OUTDIR%\*.lib %OUTDIR%\lib
xcopy %OUTDIR%\*.exp %OUTDIR%\lib
xcopy %OUTDIR%\*.ilk %OUTDIR%\lib
xcopy %OUTDIR%\*.pdb %OUTDIR%\lib
xcopy %OUTDIR%\*.exe %OUTDIR%\bin
xcopy %OUTDIR%\*.dll %OUTDIR%\bin

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@del /Q %OUTDIR%\*.lib %OUTDIR%\*.exp %OUTDIR%\*.ilk %OUTDIR%\*.pdb %OUTDIR%\*.exe %OUTDIR%\*.dll %OUTDIR%\*.obj
@del /Q %OUTDIR%\bin\buildvm.exe %OUTDIR%\bin\minilua.exe %OUTDIR%\lib\buildvm.* %OUTDIR%\lib\minilua.*

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@echo.
@echo === Successfully built LuaJIT for Windows/%LJARCH% ===

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@goto :END
:BAD
@echo.
@echo *******************************************************
@echo *** Build FAILED -- Please check the error messages ***
@echo *******************************************************
@goto :END
:FAIL
@echo You must open a "Visual Studio .NET Command Prompt" to run this script
:END
popd