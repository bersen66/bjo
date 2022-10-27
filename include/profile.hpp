#pragma once

#include <chrono>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

class LogDuration
{
public:
  explicit LogDuration(const std::string& msg = "");

  ~LogDuration();

private:
  std::string message;
  std::chrono::steady_clock::time_point start;
};

#ifndef UNIQ_ID
#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)
#endif

#define LOG_DURATION(message) LogDuration UNIQ_ID(__LINE__){message};