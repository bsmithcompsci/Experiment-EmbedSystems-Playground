#pragma once

#include <cstdint>
#include "global/ScriptingPipeline.h"

namespace Scripting
{
	class JavaPipeline : public ScriptingLanguageInterface
	{
	public:
		JavaPipeline() {}
		~JavaPipeline() override {}
		bool ExecFile(const char *_filePath) override { if (_filePath != nullptr) {} return false; }
		int Exec(const char *_cmdline) override { if (_cmdline != nullptr) {} return -1; }
	};

}; // namespace Scripting