#include <napi.h>
#include "register.h"

#include <iostream>

Napi::Value RPH(const Napi::CallbackInfo& info) {
  std::string scheme = info[0].ToString().Utf8Value();
  std::string description = info[1].IsEmpty() ? info[1].ToString().Utf8Value() : "";
  std::string command = info[2].IsEmpty() ? info[2].ToString().Utf8Value() : "";

  return Napi::Boolean::New(info.Env(), Register(scheme.c_str(), description.c_str(), command.c_str()));
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports["registerProtocolHandler"] = Napi::Function::New(env, RPH);
  return exports;
}

NODE_API_MODULE(addon, Init)
