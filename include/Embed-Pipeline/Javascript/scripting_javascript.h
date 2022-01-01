#pragma once

#include <cstdint>
#include "global/ScriptingPipeline.h"

namespace Scripting
{
	class JavascriptPipeline : public ScriptingLanguageInterface
	{
	public:
		JavascriptPipeline();
		~JavascriptPipeline() override;
		bool ExecFile(const char *_filePath) override;
		int Exec(const char *_cmdline) override;
	};

}; // namespace Scripting