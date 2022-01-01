#include <cmath>
#include <cstring>
#include <string>
#include <filesystem>
#include <iostream>
#include <thread>

// Scripting Language Includes
#include "Embed-Pipeline/Lua/scripting_lua.h"

// Arguments:
// testName [...]
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
	std::string testName = argv[1];
	std::string args = "";
	std::string delimiter = ",";
	for (auto i = 2; i < argc; ++i)
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

	scripting_interface = new Scripting::LuaPipeline();
	auto isGood = scripting_interface->ExecFile((std::filesystem::current_path().string() + "/scripts/example.lua").c_str());
	if (!terminal)
	{
		if (isGood)
		{
			result = scripting_interface->Exec((std::string("return ") + testName + "(" + args + ")").c_str());
		}
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