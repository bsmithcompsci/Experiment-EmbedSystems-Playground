#pragma once

#include <cstdint>
#include "global/ScriptingPipeline.h"

namespace Scripting
{
	class CppPipeline : public ScriptingLanguageInterface
	{
	public:
		CppPipeline();
		~CppPipeline() override;
		bool ExecFile(const char *_filePath) override;
		int Exec(const char *_cmdline) override;
	};

}; // namespace Scripting