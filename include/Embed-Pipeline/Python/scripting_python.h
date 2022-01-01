#pragma once
#include "global/ScriptingPipeline.h"
#include <cstdint>

namespace Scripting
{
	class PythonPipeline : public ScriptingLanguageInterface
	{
	public:

		enum class Version : uint8_t
		{
			v3_7_2_x86
		};

		PythonPipeline(const Version &_version);
		~PythonPipeline() override;
		bool ExecFile(const char *_filePath) override;
		int Exec(const char *_cmdline) override;
	};
}; // namespace Scripting