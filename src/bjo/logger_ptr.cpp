#include "bjo/http/server/logger_ptr.hpp"

#include <memory>
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>

namespace bjo
{

LoggerPtr InitLogger(const std::string& loggername, const std::string& filename)
{
  auto max_size = 1048576 * 5;
  auto max_files = 3;
  LoggerPtr res = spdlog::rotating_logger_mt<spdlog::async_factory>(loggername, filename, max_size, max_files);
  res->set_pattern("[%H:%M:%S %z] [thread %t] %v");
  return res;
}

} // namespace bjo