#pragma once

#include <cstdint>
#include "global/ScriptingPipeline.h"

namespace Scripting
{
	class CSharpPipeline : public ScriptingLanguageInterface
	{
	public:
		CSharpPipeline();
		~CSharpPipeline() override;
		bool ExecFile(const char *_filePath) override;
		int Exec(const char *_cmdline) override;
	};

}; // namespace Scripting