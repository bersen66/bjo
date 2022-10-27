//
// Created by ortur on 27.10.22.
//

#include "profile.hpp"

LogDuration::LogDuration(const std::string& msg)
    : message(msg + ": ")
    , start(std::chrono::steady_clock::now())
{
}

LogDuration::~LogDuration()
{
  auto finish = std::chrono::steady_clock::now();
  auto dur = finish - start;
  std::ostringstream os;
  os << message << std::chrono::duration_cast<std::chrono::microseconds>(dur).count() << " microseconds" << std::endl;
  std::cerr << os.str();
}