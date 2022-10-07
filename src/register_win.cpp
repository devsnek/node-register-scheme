/*
 * MinGW defaults to WINNT 5.1 (aka XP), however some of functions used here
 * require WINNT >= 6.0 APIs, which are only visible when WINVER and
 * _WIN32_WINNT defines are set properly before including any system headers.
 * Such API is e.g. RegSetKeyValueW.
 */
#ifdef __MINGW32__
// 0x0600 == vista
#define WINVER 0x0600
#define _WIN32_WINNT 0x0600
#endif // __MINGW32__

#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#define NOMCX
#define NOSERVICE
#define NOIME
#include <windows.h>
#include <psapi.h>
#include <strsafe.h>

#include <iostream>

bool RegisterW(const wchar_t* scheme, const wchar_t* description, const wchar_t* command) {
  // https://msdn.microsoft.com/en-us/library/aa767914(v=vs.85).aspx
  // Update the HKEY_CURRENT_USER, because it doesn't seem to require special permissions.

  wchar_t exeFilePath[MAX_PATH];
  DWORD exeLen = GetModuleFileNameExW(GetCurrentProcess(), nullptr, exeFilePath, MAX_PATH);

  wchar_t protocolName[64];
  StringCbCopyW(protocolName, sizeof(protocolName), scheme);

  wchar_t openCommand[1024];
  if (wcslen(command) != 0) {
    StringCbCopyW(openCommand, sizeof(openCommand), command);
  } else {
    StringCbCopyW(openCommand, sizeof(openCommand), exeFilePath);
  }

  wchar_t protocolDescription[128];
  if (wcslen(description) != 0) {
    StringCbCopyW(protocolDescription, sizeof(protocolDescription), description);
  } else {
    StringCbPrintfW(protocolDescription, sizeof(protocolDescription), L"URL: %s", scheme);
  }

  wchar_t urlProtocol = 0;

  wchar_t keyName[256];
  StringCbPrintfW(keyName, sizeof(keyName), L"Software\\Classes\\%s", protocolName);
  HKEY key;
  auto status =
    RegCreateKeyExW(HKEY_CURRENT_USER, keyName, 0, nullptr, 0, KEY_WRITE, nullptr, &key, nullptr);
  if (status != ERROR_SUCCESS) {
    return false;
  }

  DWORD len;
  LSTATUS result;
  len = (DWORD)lstrlenW(protocolDescription) + 1;
  result =
    RegSetKeyValueW(key, nullptr, nullptr, REG_SZ, protocolDescription, len * sizeof(wchar_t));
  if (FAILED(result))
    return false;

  len = (DWORD)lstrlenW(protocolDescription) + 1;
  result = RegSetKeyValueW(key, nullptr, L"URL Protocol", REG_SZ, &urlProtocol, sizeof(wchar_t));
  if (FAILED(result))
    return false;

  result = RegSetKeyValueW(
    key, L"DefaultIcon", nullptr, REG_SZ, exeFilePath, (exeLen + 1) * sizeof(wchar_t));
  if (FAILED(result))
    return false;

  len = (DWORD)lstrlenW(openCommand) + 1;
  result = RegSetKeyValueW(
    key, L"shell\\open\\command", nullptr, REG_SZ, openCommand, len * sizeof(wchar_t));
  if (FAILED(result))
      return false;

  RegCloseKey(key);

  return true;
}

extern "C" bool Register(const char* scheme, const char* description, const char* command) {
  wchar_t wscheme[32];
  MultiByteToWideChar(CP_UTF8, 0, scheme, 32, wscheme, 32);

  wchar_t wdescription[128];
  MultiByteToWideChar(CP_UTF8, 0, description, 128, wdescription, 128);

  wchar_t wcommand[1024];
  MultiByteToWideChar(CP_UTF8, 0, command, 1024, wcommand, 1024);

  return RegisterW(wscheme, wdescription, wcommand);
}

