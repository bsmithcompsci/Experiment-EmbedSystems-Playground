#pragma once

#include <cstdlib>
#if _WIN32
#define setenv(var, val, override) _putenv_s(var, val)
#endif

/*
    This header creates a cross-bridge between different languages for Native/Host to communicate downwards.
*/

class ScriptingLanguageInterface
{
public:
    ScriptingLanguageInterface() {};
    virtual ~ScriptingLanguageInterface() {};

    // Executes file, usually used for loading a file's definitions so we can run commands on them.
    virtual bool ExecFile(const char *_filePath) = 0;
    // Executes the following code inside the command line.
    virtual int Exec(const char *_cmdline) = 0;
};