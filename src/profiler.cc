#include <memory>
#include <napi.h>

#include "v8-profiler.h"

using namespace Napi;
using v8::Local;

v8::CpuProfiler* cpuProfiler;

Boolean StartProfiling(const CallbackInfo& info) {
  Env env = info.Env();
  cpuProfiler = v8::CpuProfiler::New(v8::Isolate::GetCurrent());
  Local<v8::String> name = v8::MaybeLocal<v8::String>("prof-test");

  cpuProfiler->SetSamplingInterval(1000);
  cpuProfiler->StartProfiling(name, v8::CpuProfilingMode::kCallerLineNumbers);

  return Boolean::New(env, true);
}

Object StopProfiling(const CallbackInfo& info) {
  Env env = info.Env();

  if (!cpuProfiler) {
    throw Napi::Error::New(env, "StartProfiling should be called first.");
  }

  v8::CpuProfile* profile = cpuProfiler->StopProfiling("prof-test");
  v8::Local<String> functionName = profile->GetTopDownRoot()->GetFunctionName();

  Object profilingData = Object::New(env);
  profilingData.Set("functionName", functionName);

  profile->Delete();
  cpuProfiler->Dispose();
  cpuProfiler = NULL;

  return profilingData
}

Object Init(Env env, Object exports) {
  exports.Set(String::New(env, "StartProfiling"),
              Function::New(env, StartProfiling));
  exports.Set(String::New(env, "StopProfiling"),
              Function::New(env, StopProfiling));
  return exports;
}

NODE_API_MODULE(profiler, Init)
