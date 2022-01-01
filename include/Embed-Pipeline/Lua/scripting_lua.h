#pragma once

#include <cstdint>
#include "global/ScriptingPipeline.h"

namespace Scripting
{
	class LuaPipeline : public ScriptingLanguageInterface
	{
	public:
		LuaPipeline();
		~LuaPipeline() override;
		bool ExecFile(const char *_filePath) override;
		int Exec(const char *_cmdline) override;
	};

}; // namespace Scripting