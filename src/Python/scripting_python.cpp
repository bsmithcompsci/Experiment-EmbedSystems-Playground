#include "../../include/Embed-Pipeline/Python/scripting_python.h"
#include <iostream>
#include <filesystem>
#include "pybind11/pybind11.h"
#include "pybind11/embed.h" // everything needed for embedding
namespace py = pybind11;

namespace Scripting
{
	PythonPipeline::PythonPipeline(const Version &_version)
	{
		std::wstring pythonLibraryPath = std::filesystem::current_path().wstring();
		//std::wcerr << pythonLibraryPath.c_str() << '\n';
		switch (_version)
		{
		case Version::v3_7_2_x86:
			pythonLibraryPath += L"/Python/Lib";
			break;
		default:
			break;
		}
		setenv("PYTHONPATH", (const char*)pythonLibraryPath.c_str(), true);
		Py_SetPath(pythonLibraryPath.c_str());
		py::initialize_interpreter(); // start the interpreter and keep it alive
	}
	PythonPipeline::~PythonPipeline()
	{
		py::finalize_interpreter();
	}

	bool PythonPipeline::ExecFile(const char *_filePath)
	{
		try
		{
			py::eval_file(_filePath);
			return true;
		}
		catch (py::error_already_set const &pythonErr)
		{
			std::cerr << pythonErr.what() << "\n";
		}
		return false;
	}

	int PythonPipeline::Exec(const char *_cmdline)
	{
		try
		{
			auto obj = py::eval(_cmdline);
			if (py::isinstance<py::int_>(obj))
			{
				int val = obj.cast<int>();
				std::cout << val << "\n";
				return val;
			}
			return -1;
		}
		catch (py::error_already_set const &pythonErr)
		{
			std::cerr << "[Embed-Systems] ::> " << pythonErr.what() << "\n";
		}
		return -1;
	}

}; // namespace Scripting
