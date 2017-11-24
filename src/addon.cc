#include <napi.h>
#include "register.h"

Napi::Value RPH(const Napi::CallbackInfo& info) {
  const char* scheme = info[0].As<Napi::String>().Utf8Value().c_str();
  const char* command = info[0].As<Napi::String>().Utf8Value().c_str();

  Register(scheme, command);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("registerProtocolHandler", Napi::Function::New(env, RPH));
  return exports;
}

NODE_API_MODULE(addon, Init)
