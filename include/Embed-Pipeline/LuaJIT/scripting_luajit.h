#pragma once

#include <cstdint>
#include "global/ScriptingPipeline.h"

namespace Scripting
{
	class LuaJITPipeline : public ScriptingLanguageInterface
	{
	public:
		LuaJITPipeline();
		~LuaJITPipeline() override;
		bool ExecFile(const char *_filePath) override;
		int Exec(const char *_cmdline) override;
	};

}; // namespace Scripting