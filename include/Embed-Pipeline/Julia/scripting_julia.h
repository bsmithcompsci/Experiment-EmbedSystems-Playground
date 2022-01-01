#pragma once

#include <cstdint>
#include "global/ScriptingPipeline.h"

namespace Scripting
{
	class JuliaPipeline : public ScriptingLanguageInterface
	{
	public:
		JuliaPipeline() {}
		~JuliaPipeline() override {}
		bool ExecFile(const char *_filePath) override { if (_filePath) {} return false; }
		int Exec(const char *_cmdline) override { if (_cmdline) {} return -1; }
	};

}; // namespace Scripting