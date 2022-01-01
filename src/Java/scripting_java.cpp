#include "../../include/Embed-Pipeline/Java/scripting_java.h"
#include "global/Utils.h"
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#if _WIN32
#include <Windows.h>
#endif


#include "jni.h"

namespace Scripting
{
	JavaVM *jvm;
	JNIEnv *JNIE;
	void *libHandle;
	
	JavaPipeline::JavaPipeline()
	{
		std::string jvmDllPath = (std::filesystem::current_path().string() + "/jre/bin/server/jvm.dll");
		if (!std::filesystem::exists(jvmDllPath))
		{
			std::cerr << "Failed to find JVM: `" << jvmDllPath.c_str() << "`\n";
			return;
		}
#if _WIN32
		libHandle = LoadLibrary(jvmDllPath.c_str());
		if (libHandle == NULL)
		{
			std::cerr << "Failed to load JVM: `" << jvmDllPath.c_str() << "`\n";
			return;
		}
#endif
		// Java options
		JavaVMOption jarPathOption[1] = { };
		std::string scriptPath = std::string("-Djava.class.path=") + std::filesystem::current_path().string() + "/scripts/example.java/example-java.jar";
		jarPathOption[0].optionString = (char *) scriptPath.c_str();
		jarPathOption[0].extraInfo = nullptr;

		// Initialize Java Settings.
		JavaVMInitArgs javaArgs;
		javaArgs.ignoreUnrecognized = false;
		javaArgs.version = JNI_VERSION_1_8;
		javaArgs.nOptions = 1;
		javaArgs.options = jarPathOption;

		// Initializes Java Virtual Machine
		if (JNI_CreateJavaVM(&jvm, (void **)JNIE, &javaArgs) != JNI_OK)
		{
			std::cerr << "JNI Faile to Initialize!\n";
		}

		std::cerr << "JNI was successful!\n";
	}
	JavaPipeline::~JavaPipeline()
	{
#if _WIN32
		FreeLibrary((HMODULE)libHandle);
#endif

		if (jvm)
		{
			jvm->DestroyJavaVM();
			delete jvm;
		}
	}
	bool JavaPipeline::ExecFile(const char *_filePath)
	{
		// We do nothing here.
		// Maybe we could load a dll to test the latency between shared dlls and such... ?
		std::cout << _filePath << "\n";
		return true;
	}

	int JavaPipeline::Exec(const char *_cmdline)
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
		}

		return -2;
	}
};
