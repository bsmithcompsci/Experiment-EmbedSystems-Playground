#include "Embed-Pipeline/Javascript/scripting_javascript.h"
#include "global/Utils.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>

#pragma warning(disable : 4100) // We ignore V8s warnings... Trust they have a lot of them...
#pragma warning(disable : 4127)
#include "v8.h"
#include "libplatform/libplatform.h"
#pragma warning(default : 4127)
#pragma warning(default : 4100) 


namespace Scripting
{
    // Thanks: https://github.com/ruby0x1/v8-tutorials
    // Binded function...
    void printLog(const v8::FunctionCallbackInfo<v8::Value> &args)
    {
        std::string output;
        for (auto i = 0; i < args.Length(); i++)
        {
            // Convert the result to an UTF8 string and print it.
            v8::String::Utf8Value utf8(args.GetIsolate(), args[i]->ToString(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
            output += std::string(*utf8) + "\t";
        }
        std::cout << output << "\n";
    }
    void errorLog(const v8::FunctionCallbackInfo<v8::Value> &args)
    {
        std::string output;
        for (auto i = 0; i < args.Length(); i++)
        {
            // Convert the result to an UTF8 string and print it.
            v8::String::Utf8Value utf8(args.GetIsolate(), args[i]->ToString(args.GetIsolate()->GetCurrentContext()).ToLocalChecked());
            output += std::string(*utf8) + "\t";
        }
        std::cerr << output << "\n";
    }
    // Binding Object/Class
    class Math
    {
    public:
        static void _min(const v8::FunctionCallbackInfo<v8::Value> &args)
        {
            double firstArg = args[0]->NumberValue(args.GetIsolate()->GetCurrentContext()).ToChecked();
            double mostMin = firstArg;

            for (auto i = 0; i < args.Length(); i++)
            {
                double val = args[i]->NumberValue(args.GetIsolate()->GetCurrentContext()).ToChecked();
                if (mostMin > val)
                {
                    mostMin = val;
                }
            }

            // return min
            args.GetReturnValue().Set(mostMin);
        }
        static void _max(const v8::FunctionCallbackInfo<v8::Value> &args)
        {
            double firstArg = args[0]->NumberValue(args.GetIsolate()->GetCurrentContext()).ToChecked();
            double mostMax = firstArg;

            for (auto i = 0; i < args.Length(); i++)
            {
                double val = args[i]->NumberValue(args.GetIsolate()->GetCurrentContext()).ToChecked();
                if (mostMax < val)
                {
                    mostMax = val;
                }
            }
            // return max
            args.GetReturnValue().Set(mostMax);
        }
        static void _floor(const v8::FunctionCallbackInfo<v8::Value> &args)
        {
            double firstArg = args[0]->NumberValue(args.GetIsolate()->GetCurrentContext()).ToChecked();

            // Return the floor.
            args.GetReturnValue().Set(floor(firstArg));
        }
        static void _sqrt(const v8::FunctionCallbackInfo<v8::Value> &args)
        {
            double firstArg = args[0]->NumberValue(args.GetIsolate()->GetCurrentContext()).ToChecked();

            // Return the floor.
            args.GetReturnValue().Set(sqrt(firstArg));
        }
    };

    std::string sourceCode;
    std::unique_ptr<v8::Platform> platform;
    v8::Isolate * isolate;
    v8::Isolate::CreateParams create_params;
    JavascriptPipeline::JavascriptPipeline()
    {
        // Initialize V8.
        v8::V8::InitializeICUDefaultLocation(".");
        v8::V8::InitializeExternalStartupData(".");
        platform = v8::platform::NewDefaultPlatform();
        v8::V8::InitializePlatform(platform.get());
        v8::V8::Initialize();

        // Create a new Isolate and make it the current one.
        create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator(); // This will create the default V8 Allocator; if wanted to have a custom allocator, this is the place to do it!
        isolate = v8::Isolate::New(create_params);
	}
	JavascriptPipeline::~JavascriptPipeline() 
    {
        // Dispose the isolate and tear down V8.
        isolate->Dispose();
        v8::V8::Dispose();
        v8::V8::ShutdownPlatform();
        delete create_params.array_buffer_allocator;
    }


	bool JavascriptPipeline::ExecFile(const char *_filePath)
	{
        if (!std::filesystem::exists(_filePath))
        {
            std::cerr << "[JavascriptPipeline::ExecFile] Couldn't open: `" << _filePath << "` (File doesn't exist!)\n";
            return false;
        }
        std::ifstream fHandle;
        fHandle.open(_filePath);
        if (fHandle.is_open())
        {
            std::string line;
            while (std::getline(fHandle, line))
            {
                sourceCode += line + "\n";
            }
            fHandle.close();
            //std::cerr << "Read Source File:\n" << sourceCode << "\n";

            return true;
        }
        std::cerr << "[JavascriptPipeline::ExecFile] Couldn't open: `" << _filePath << "`\n";
		return false;
	}

	int JavascriptPipeline::Exec(const char *_cmdline)
	{
        {
            v8::Isolate::Scope isolate_scope(isolate);

            // Create a stack-allocated handle scope.
            v8::HandleScope handle_scope(isolate);

            v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

            // Binding
            {
                // print("", ...);
                global->Set(v8::String::NewFromUtf8(isolate, "print").ToLocalChecked(), v8::FunctionTemplate::New(isolate, printLog));
                global->Set(v8::String::NewFromUtf8(isolate, "error").ToLocalChecked(), v8::FunctionTemplate::New(isolate, errorLog));
                // math.*
                {
                    v8::Local<v8::ObjectTemplate> math_template = v8::ObjectTemplate::New(isolate);
                    math_template->SetInternalFieldCount(1);
                    math_template->Set(v8::String::NewFromUtf8(isolate, "min").ToLocalChecked(), v8::FunctionTemplate::New(isolate, Math::_min));
                    math_template->Set(v8::String::NewFromUtf8(isolate, "max").ToLocalChecked(), v8::FunctionTemplate::New(isolate, Math::_max));
                    math_template->Set(v8::String::NewFromUtf8(isolate, "floor").ToLocalChecked(), v8::FunctionTemplate::New(isolate, Math::_floor));
                    math_template->Set(v8::String::NewFromUtf8(isolate, "sqrt").ToLocalChecked(), v8::FunctionTemplate::New(isolate, Math::_sqrt));

                    global->Set(v8::String::NewFromUtf8(isolate, "math").ToLocalChecked(), math_template);
                }
            }

            // Create a new context.
            v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global);

            // Enter the context for compiling and running the hello world script.
            v8::Context::Scope context_scope(context);

            // Load the source code...
            {
                // Create a string containing the JavaScript source code.
                v8::MaybeLocal<v8::String> source = v8::String::NewFromUtf8(isolate, sourceCode.c_str(), v8::NewStringType::kNormal, (int)sourceCode.size());

                // Compile the source code.
                v8::Local<v8::Script> script = v8::Script::Compile(context, source.ToLocalChecked()).ToLocalChecked();

                // Run the script to get the result.
                script->Run(context).ToLocalChecked();
            }

            // Execute desired execution.
            {
                // Create a string containing the JavaScript source code.
                v8::MaybeLocal<v8::String> source = v8::String::NewFromUtf8(isolate, _cmdline);

                // Compile the source code.
                v8::Local<v8::Script> script = v8::Script::Compile(context, source.ToLocalChecked()).ToLocalChecked();

                // Run the script to get the result.
                v8::MaybeLocal<v8::Value> maybeResult = script->Run(context);
                if (!maybeResult.IsEmpty())
                {
                    v8::Local<v8::Value> result = maybeResult.ToLocalChecked();
                    if (!result.IsEmpty())
                    {
                        std::string typeName(*v8::String::Utf8Value(isolate, result->TypeOf(isolate)));
                        if (typeName == "number")
                        {
                            v8::String::Utf8Value utf8(isolate, result->ToString(context).ToLocalChecked());
                            std::cout << std::string(*utf8) << "\n";
                            return atoi(*utf8);
                        }
                    }
                }
            }
        }

		return -2;
	}
};
