#include <memory>
#include <napi.h>

#include "v8-profiler.h"

using namespace Napi;

Boolean StartProfiling(const CallbackInfo& info) {
  Env env = info.Env();
  return Boolean::New(env, true);
}

Object Init(Env env, Object exports) {
  exports.Set(String::New(env, "StartProfiling"),
              Function::New(env, StartProfiling));
  return exports;
}

NODE_API_MODULE(profiler, Init)
