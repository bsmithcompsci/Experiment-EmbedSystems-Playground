#include <cmath>
#include <cstring>
#include <string>
#include <filesystem>
#include <iostream>
#include <thread>

// Benchmark languages
#include "Embed-Pipeline/Cpp/scripting_cpp.h"
#include "Embed-Pipeline/Python/scripting_python.h"
#include "Embed-Pipeline/Lua/scripting_lua.h"
#include "Embed-Pipeline/LuaJIT/scripting_luajit.h"
#include "Embed-Pipeline/CSharp/scripting_csharp.h"
#include "Embed-Pipeline/Java/scripting_java.h"
#include "Embed-Pipeline/Javascript/scripting_javascript.h"
#include "Embed-Pipeline/Julia/scripting_julia.h"

#define PREVENT_STDOUT_RESULT -2

// Arguments:
// language testName [...]
int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cerr << "Not enough arguments: Ensure you have language & testName (" << argc << ")" << '\n';
		for (auto i = 3; i < argc; ++i)
		{
			std::cerr << "Arg[" << i << "]:" << argv[i] << "\n";
		}
		return EXIT_FAILURE;
	}

	// Arguments
	std::string language = argv[1];
	std::string testName = argv[2];
	std::string args = "";
	std::string delimiter = (language == "cpp" ? " " : ", ");
	for (auto i = 3; i < argc; ++i)
	{
		args += argv[i];
		if (i < argc - 1)
			args += delimiter;
	}

	// Handle the Language pipelines.
	int result = -1;
	bool terminal = (testName == "cli");
	ScriptingLanguageInterface *scripting_interface = nullptr;
	if (!terminal)
	{
		// Do No Operation Time, so psutil can catch this process id (pid).
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	if (language == "cpp")
	{
		Scripting::CppPipeline cppInterface = Scripting::CppPipeline();
		if (!terminal)
		{
			result = cppInterface.Exec((testName + " " + args).c_str());
		}
		else
		{
			std::cerr << "Cpp pipeline cannot have a cli.\n";
			return EXIT_FAILURE;
		}
	}
	else if (language == "python")
	{
		scripting_interface = new Scripting::PythonPipeline(Scripting::PythonPipeline::Version::v3_7_2_x86);
		auto isGood = scripting_interface->ExecFile((std::filesystem::current_path().string() + "/scripts/example.py").c_str());
		if (!terminal)
		{
			if (isGood)
			{
				result = scripting_interface->Exec((testName + "(" + args + ")").c_str());
			}
		}
	}
	else if (language == "lua")
	{
		scripting_interface = new Scripting::LuaPipeline();
		auto isGood = scripting_interface->ExecFile((std::filesystem::current_path().string() + "/scripts/example.lua").c_str());
		if (!terminal)
		{
			if (isGood)
			{
				result = scripting_interface->Exec((std::string("return ") + testName + "(" + args + ")").c_str());
			}
		}
	}
	else if (language == "luajit")
	{
		scripting_interface = new Scripting::LuaJITPipeline();
		auto isGood = scripting_interface->ExecFile((std::filesystem::current_path().string() + "/scripts/example.lua").c_str());
		if (!terminal)
		{
			if (isGood)
			{
				result = scripting_interface->Exec((std::string("return ") + testName + "(" + args + ")").c_str());
			}
		}
	}
	else if (language == "csharp")
	{
		scripting_interface = new Scripting::CSharpPipeline();
		auto isGood = scripting_interface->ExecFile((std::filesystem::current_path().string() + "/scripts/example.cs/bin/Debug/ExampleCS.dll").c_str());
		if (!terminal)
		{
			if (isGood)
			{
				result = scripting_interface->Exec((testName + " " + args).c_str());
			}
		}
	}
	else if (language == "java")
	{
		scripting_interface = new Scripting::JavaPipeline();
		auto isGood = scripting_interface->ExecFile((std::filesystem::current_path().string() + "/scripts/example.js").c_str());
		if (!terminal)
		{
			if (isGood)
			{
				result = scripting_interface->Exec((testName + "(" + args + ")").c_str());
			}
		}
	}
	else if (language == "javascript")
	{
		scripting_interface = new Scripting::JavascriptPipeline();
		auto isGood = scripting_interface->ExecFile((std::filesystem::current_path().string() + "/scripts/example.js").c_str());
		if (!terminal)
		{
			if (isGood)
			{
				result = scripting_interface->Exec((testName + "(" + args + ")").c_str());
			}
		}
	}
	else if (language == "julia")
	{
		scripting_interface = new Scripting::JuliaPipeline();
		auto isGood = scripting_interface->ExecFile((std::filesystem::current_path().string() + "/scripts/example.js").c_str());
		if (!terminal)
		{
			if (isGood)
			{
				result = scripting_interface->Exec((testName + "(" + args + ")").c_str());
			}
		}
	}
	else
	{
		std::cerr << "Unknown language: " << language.c_str() << '\n';
		return EXIT_FAILURE;
	}

	if (scripting_interface != nullptr)
	{
		if (terminal)
		{
			std::cout << "Scripting Terminal Enabled...\n";
			std::string input;
			while (input != "quit()")
			{
				std::cout << "::> ";
				std::getline(std::cin, input);
				scripting_interface->Exec(input.c_str());
			}
		}
		delete scripting_interface;
	}

	return EXIT_SUCCESS;
}