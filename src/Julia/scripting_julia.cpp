#include "../../include/Embed-Pipeline/Julia/scripting_julia.h"
#include "global/Utils.h"
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

#pragma warning (disable : 4005)
#ifdef _MSC_VER
#include <uv.h>
#include <Windows.h>
#endif
#include "julia.h"
#pragma warning (default : 4005)

namespace Scripting
{
	JuliaPipeline::JuliaPipeline()
	{
		/* required: setup the Julia context */
		jl_init();

		/* run Julia commands */
		//jl_eval_string("print(\"Hello World!\")");
	}
	JuliaPipeline::~JuliaPipeline()
	{
		/* strongly recommended: notify Julia that the
			 program is about to terminate. this allows
			 Julia time to cleanup pending write requests
			 and run all finalizers
		*/
		//jl_atexit_hook(0);
	}
	bool JuliaPipeline::ExecFile(const char *_filePath)
	{
		// We do nothing here.
		// Maybe we could load a dll to test the latency between shared dlls and such... ?
		std::cout << _filePath << "\n";
		return true;
	}

	int JuliaPipeline::Exec(const char *_cmdline)
	{
		std::string cmdline = _cmdline;
		std::vector<std::string> args = splitStrs(cmdline, ' ');
		if (args.size() < 2)
			return -1;

		std::string testName = args[0];
		std::string arrayStr = args[1];
		std::vector<int> arr = splitInts(arrayStr, ',');

		if (args.size() > 2)
		{
			return -1;
		}
		else
		{
			// Output the sorted array
			for (size_t i = 0; i < arr.size(); i++)
			{
				std::cout << arr[i] << ",";
			}
			std::cout << "\n";
		}

		return -2;
	}
};
