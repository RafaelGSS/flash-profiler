#include <memory>
#include <napi.h>

#include "v8-profiler.h"

using namespace Napi;
using v8::Local;

v8::CpuProfiler* cpuProfiler;

Boolean StartProfiling(const CallbackInfo& info) {
  Env env = info.Env();
  cpuProfiler = v8::CpuProfiler::New(v8::Isolate::GetCurrent());
  Local<v8::String> name = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "prof-test").ToLocalChecked();

  cpuProfiler->SetSamplingInterval(1000);
  cpuProfiler->StartProfiling(name, v8::CpuProfilingMode::kCallerLineNumbers);

  return Boolean::New(env, true);
}

Object ParseDeoptInfo(Env env, std::vector<v8::CpuProfileDeoptInfo> deopts) {
  Object deoptInfo = Object::New(env);
  for (auto deopt : deopts) {
    deoptInfo.Set("reason", deopt.deopt_reason);
  }

  return deoptInfo;
}

Object CreateTimeNode(
    Env env,
    Local<v8::String> name,
    Local<v8::String> scriptName,
    int scriptId,
    int lineNumber,
    int columnNumber,
    int hitCount,
    std::vector<v8::CpuProfileDeoptInfo> deopts,
    Array children
) {
  Object node = Object::New(env);
  node.Set("name", *v8::String::Utf8Value(v8::Isolate::GetCurrent(), name));
  node.Set("scriptName", *v8::String::Utf8Value(v8::Isolate::GetCurrent(), scriptName));
  node.Set("scriptId", scriptId);
  node.Set("lineNumber", lineNumber);
  node.Set("columnNumber", columnNumber);
  node.Set("hitCount", hitCount);
  node.Set("children", children);
  if (deopts.size()) {
    node.Set("deopt", ParseDeoptInfo(env, deopts));
  }

  return node;
}


Object TranslateTimeProfileNode(Env env, const v8::CpuProfileNode* node) {
  int32_t count = node->GetChildrenCount();
  Array children = Array::New(env, count);
  for (int32_t i = 0; i < count; i++) {
    children.Set(i, TranslateTimeProfileNode(env, node->GetChild(i)));
  }

  return CreateTimeNode(
      env,
      node->GetFunctionName(),
      node->GetScriptResourceName(),
      node->GetScriptId(),
      node->GetLineNumber(),
      node->GetColumnNumber(),
      node->GetHitCount(),
      node->GetDeoptInfos(),
      children
  );
}

Object StopProfiling(const CallbackInfo& info) {
  Env env = info.Env();

  if (!cpuProfiler) {
    throw Napi::Error::New(env, "StartProfiling should be called first.");
  }

  Local<v8::String> name = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "prof-test").ToLocalChecked();
  v8::CpuProfile* profile = cpuProfiler->StopProfiling(name);

  Object profilingData = Object::New(env);
  profilingData.Set("title", *v8::String::Utf8Value(v8::Isolate::GetCurrent(), profile->GetTitle()));
  profilingData.Set("startTime", profile->GetStartTime());
  profilingData.Set("endTime", profile->GetEndTime());
  profilingData.Set("topDownRoot", TranslateTimeProfileNode(env, profile->GetTopDownRoot()));

  profile->Delete();
  cpuProfiler->Dispose();
  cpuProfiler = NULL;

  return profilingData;
}

Object Init(Env env, Object exports) {
  exports.Set(String::New(env, "StartProfiling"),
              Function::New(env, StartProfiling));
  exports.Set(String::New(env, "StopProfiling"),
              Function::New(env, StopProfiling));
  return exports;
}

NODE_API_MODULE(profiler, Init)
