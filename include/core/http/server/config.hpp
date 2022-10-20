#pragma once

#include <boost/noncopyable.hpp>
#include <filesystem> // for std::filesystem::path
#include <memory>     // for std::shared_ptr
#include <optional>   // for std::optional
#include <unordered_map>

namespace core
{
namespace http
{
namespace server
{

struct Config : boost::noncopyable
{
  std::filesystem::path error_logfile = "error.log";
  std::filesystem::path info_logfile = "info.log";
  std::string connection_string;
  short unsigned int listen_port = 8080;
  unsigned int threads = 3;
  unsigned int connection_timeout = 2; // in seconds
  unsigned int request_timeout = 3;    // in seconds
};

using ConfigPtr = std::shared_ptr<Config>;

ConfigPtr DefaultConfig();

std::ostream& operator<<(std::ostream& os, const Config& config);
std::ostream& operator<<(std::ostream& os, const ConfigPtr& config_ptr);

std::optional<ConfigPtr> ParseConfigFile(const std::filesystem::path& config_file);

} // namespace server
} // namespace http
} // namespace core
