#pragma once

#include <memory>
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

namespace bjo
{
using LoggerPtr = std::shared_ptr<spdlog::logger>;

LoggerPtr InitLogger(const std::string& loggername, const std::string& filename);

} // namespace bjo