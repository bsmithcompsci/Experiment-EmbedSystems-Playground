#include "../../include/Embed-Pipeline/Cpp/scripting_cpp.h"
#include "global/Utils.h"
#include "example.hpp"
#include <string>
#include <vector>
#include <iostream>


namespace Scripting
{
	CppPipeline::CppPipeline()
	{
	}
	CppPipeline::~CppPipeline()
	{
	}
	bool CppPipeline::ExecFile(const char *_filePath)
	{
		// We do nothing here.
		// Maybe we could load a dll to test the latency between shared dlls and such... ?
		std::cout << _filePath << "\n";
		return true;
	}

	int CppPipeline::Exec(const char *_cmdline)
	{
		std::string cmdline = _cmdline;
		std::vector<std::string> args = splitStrs(cmdline, ' ');
		if (args.size() < 2)
			return -1;

		std::string testName = args[0];
		std::string arrayStr = args[1];
		std::vector<int> arr = splitInts(arrayStr, ',');

		// Arrays
		// Searches
		if (args.size() > 2)
		{
			int result = -1;
			std::string testNeedleStr = args[2];
			int testNeedle = atoi(testNeedleStr.c_str());
			if (testName == "Array_Search_Linear")
			{
				result = Array_Search_Linear((const int *)arr.data(), (int)arr.size(), testNeedle);
			}
			else if (testName == "Array_Search_Binary")
			{
				result = Array_Search_Binary((const int *)arr.data(), (int)arr.size(), testNeedle, 0, (int)arr.size() - 1);
			}
			else if (testName == "Array_Search_Jump")
			{
				result = Array_Search_Jump((const int *)arr.data(), (int)arr.size(), testNeedle);
			}
			else if (testName == "Array_Interpolation_Search")
			{
				result = Array_Interpolation_Search((const int *)arr.data(), (int)arr.size(), testNeedle);
			}
			else if (testName == "Array_Exponential_Search")
			{
				result = Array_Exponential_Search((const int *)arr.data(), (int)arr.size(), testNeedle);
			}
			else
			{
				std::cerr << "Unsupported Test Function[Cpp]: " << testName << "\n";
				exit(EXIT_FAILURE);
			}

			std::cout << result << "\n";
			return result;
		}
		else
		{
			// Sorts
			if (testName == "Array_selectionSort")
			{
				Array_selectionSort(arr.data(), (int)arr.size());
			}
			else if (testName == "Array_bubbleSort")
			{
				Array_bubbleSort(arr.data(), (int)arr.size());
			}
			else if (testName == "Array_insertionSort")
			{
				Array_insertionSort(arr.data(), (int)arr.size());
			}
			else if (testName == "Array_quickSort")
			{
				Array_quickSort(arr.data(), 0, (int)arr.size());
			}
			else if (testName == "Array_radixSort")
			{
				Array_radixSort(arr.data(), (int)arr.size());
			}
			else
			{
				std::cerr << "Unsupported Test Function[Cpp]: " << testName << "\n";
				exit(EXIT_FAILURE);
			}

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
