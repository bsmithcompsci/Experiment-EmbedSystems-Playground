#include "../../include/Embed-Pipeline/CSharp/scripting_csharp.h"
#include "global/Utils.h"
#include <iostream>
#include <filesystem>

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/debug-helpers.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/mono-config.h"

namespace Scripting
{
	MonoDomain *domain = nullptr;
	MonoAssembly *assembly = nullptr;

	CSharpPipeline::CSharpPipeline()
	{
		// https://stackoverflow.com/questions/9582365/debugging-w-embedded-mono-how-to-set-breakpoints-in-c-sharp-code
		//mono_jit_parse_options()
		mono_debug_init(MONO_DEBUG_FORMAT_MONO);
		mono_config_parse(NULL);

		auto currentPath = std::filesystem::current_path().string();
		mono_set_dirs(currentPath.c_str(), currentPath.c_str());
		domain = mono_jit_init("CSharp-Domain");

		if (!domain)
		{
			std::cerr << "Mono Domain could not load..." << "\n";
		}

		// We can manually control/manage the allocation vtable; this can be extremely useful, if we wanted to keep records of allocation or handle the allocations ourself.
		// Note: by default the allocations go through standard library of C++ (malloc, free).
		//mono_set_allocator_vtable()
	}
	CSharpPipeline::~CSharpPipeline()
	{
		if (domain)
		{
			mono_jit_cleanup(domain);
		}
	}
	bool CSharpPipeline::ExecFile(const char *_filePath)
	{
		if (!domain)
			return false;
		// Load the assembly from a dll;
		// --
		// Note: this is loading one at a time, ideally we would want to do memory tracking of this.
		//			Though jit cleanup is handling all the requests we are opening, and getting information about mono; thus when jit cleanup is called, all of these pointers will become nullptr.
		assembly = mono_domain_assembly_open(domain, _filePath);
		if (assembly)
		{
			return true;
		}
		return false;
	}
	
	// Thanks: https://stackoverflow.com/a/47510808
	std::list<MonoClass *> GetAssemblyClassList(MonoImage *image)
	{
		std::list<MonoClass *> class_list;

		const MonoTableInfo *table_info = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);

		int rows = mono_table_info_get_rows(table_info);

		/* For each row, get some of its values */
		for (int i = 0; i < rows; i++)
		{
			MonoClass *_class = nullptr;
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
			const char *name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			const char *name_space = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			_class = mono_class_from_name(image, name_space, name);
			class_list.push_back(_class);
		}
		return class_list;
	}

	int CSharpPipeline::Exec(const char *_cmdline)
	{
		if (!assembly)
			return -1;

		// Load the Assembly Image, essentially gets all the CLR structs/classes/functions.
		MonoImage *localAssemblyImage = mono_assembly_get_image(assembly);
		
		if (!localAssemblyImage)
			return -1;


		// Lookup the class that has the `Native.IEmbed` to find the main class we will run.
		MonoClass *IEmbedClass = mono_class_from_name(localAssemblyImage, "Native", "IEmbed");
		MonoClass *MainClass = nullptr;
		// Search through all the assembly classes, then we find the one that implements the IEmbed interface. The first one is considered the main class...
		//	Note: If there are multiple classes that implement the IEmbed interface, this will pick the first alphabetically.
		for each (MonoClass *monoClass in GetAssemblyClassList(localAssemblyImage))
		{
			if (mono_class_implements_interface(monoClass, IEmbedClass) && monoClass != IEmbedClass)
			{
				MainClass = monoClass;
				break;
			}
		}

		if (!IEmbedClass || !MainClass)
			return -1;

		std::string cmdline = _cmdline;
		std::vector<std::string> args = splitStrs(cmdline, ' ');
		if (args.size() < 2)
			return -1;

		std::string testName = args[0];
		std::string arrayStr = args[1];
		std::vector<int> arr = splitInts(arrayStr, ',');

		std::string mainClassName = mono_class_get_name(MainClass);
		std::string methodDescript = std::string(".") + mainClassName + ":" + testName;
		MonoMethodDesc * methodDesc = mono_method_desc_new(methodDescript.c_str(), false);
		int result = -1;
		if (methodDesc)
		{
			MonoMethod *method = mono_method_desc_search_in_class(methodDesc, MainClass);
			if (method)
			{
				// Construct a Mono Integer array, so we can pass it over the pipeline.
				MonoArray *monoArr = mono_array_new(domain, MainClass, arr.size());
				for (size_t i = 0; i < arr.size(); i++)
				{
					mono_array_set(monoArr, int, i, arr[i]);
				}

				void **inputArgs = nullptr;
				if (args.size() > 2)
				{
					std::string testNeedleStr = args[2];
					int testNeedle = atoi(testNeedleStr.c_str());
					inputArgs = new void * [] {
						monoArr,
						&testNeedle
					};
				}
				else
				{
					inputArgs = new void *[] {
						monoArr
					};
				}
				
				// Create our exception object, so we can catch runtime errors; and invoke the function that we are targeting.
				MonoObject *monoExceptObj = nullptr;
				auto retExecution = mono_runtime_invoke(method, MainClass, inputArgs, &monoExceptObj);
				if (retExecution)
				{
					// We don't need to check for datatype; since we contracted the MainClass to IEmbed; we know what type we will get back.
					result = *(int *)mono_object_unbox(retExecution);
					std::cout << result << "\n";
				}

				// Error Handling...
				if (monoExceptObj)
				{
					MonoString *exString = mono_object_to_string(monoExceptObj, nullptr);
					std::cerr << "Scripting Exception[CSharp-Mono] ::> " << mono_string_to_utf8(exString) << "\n";
				}
				
				// Cleanup the input args.
				delete[] inputArgs;
			}
		}

		// Successfully Cleanup...
		mono_method_desc_free(methodDesc);

		return result;
	}
};
