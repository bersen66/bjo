#include <fmt/ostream.h>
#include <json/json.h>
#include <yaml-cpp/yaml.h>

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <unordered_map>

#include "core/http/server/config/config.hpp"
#include "core/parsing_stuff/parsing_stuff.hpp"

namespace http
{
namespace server
{

enum class FileExtensionId
{
  JSON = 0,
  YAML,
};

const std::unordered_map<std::string, FileExtensionId> extension_ids = {
    {".json", FileExtensionId::JSON},
    {".yml", FileExtensionId::YAML},
};

bool IsValidFileExtension(const std::string& extension)
{
  return extension_ids.contains(extension);
}

FileExtensionId GetFileExtensionId(const std::filesystem::path& extension)
{
  return extension_ids.at(extension);
}

ConfigPtr DefaultConfig()
{
  return std::make_shared<Config>();
}

std::optional<ConfigPtr> ConstructConfigFromJson(Json::Value&& doc)
{
  if (doc.isNull())
  {
    spdlog::error("Parsed json-doc is Null");
    return std::nullopt;
  }

  if (!doc["HTTP"].isNull())
  {
    ConfigPtr result = DefaultConfig();
    const Json::Value& http_node = doc["HTTP"];
    if (!http_node["error_logfile"].isNull())
    {
      result->error_logfile = static_cast<std::filesystem::path>(doc["error_logfile"].asString());
    }
    if (!http_node["info_logfile"].isNull())
    {
      result->error_logfile = http_node["info_logfile"].asString();
    }

    if (!http_node["connection_string"].isNull())
    {
      result->connection_string = http_node["connection_string"].asString();
    }

    if (!http_node["listen_port"].isNull())
    {
      result->listen_port = http_node["listen_port"].asUInt();
    }

    if (!http_node["threads"].isNull())
    {
      result->threads = std::max<unsigned int>(result->threads, http_node["threads"].asUInt());
    }

    if (!http_node["connection_timeout"].isNull())
    {
      result->connection_timeout =
          std::min<unsigned int>(result->connection_timeout, http_node["connection_timeout"].asUInt());
    }

    if (!http_node["request_timeout"].isNull())
    {
      result->request_timeout = std::min<unsigned int>(result->request_timeout, http_node["request_timeout"].asUInt());
    }
    return result;
  }

  return std::nullopt;
}

std::optional<ConfigPtr> GetConfigFromJson(const std::filesystem::path& path_to_config)
{
  try
  {
    std::optional<ConfigPtr> result = ConstructConfigFromJson(std::move(parsers::ParseJson(path_to_config)));
    return result;
  }
  catch (std::exception& exc)
  {
    spdlog::error("Exception while parsing {}\nException message:\n{}", path_to_config, exc.what());
    return std::nullopt;
  }
}

YAML::Node ParseYaml(const std::filesystem::path& path_to_config)
{
  return YAML::LoadFile(path_to_config);
}

std::optional<ConfigPtr> ConstructConfigFromYaml(YAML::Node&& doc)
{
  if (doc.IsNull())
  {
    spdlog::error("Parsed yaml-doc is Null");
    return std::nullopt;
  }

  ConfigPtr res = DefaultConfig();
  if (!doc["error_logfile"].IsNull())
  {
    res->error_logfile = doc["error_logfile"].as<std::string>();
  }
  if (!doc["info_logfile"].IsNull())
  {
    res->error_logfile = doc["info_logfile"].as<std::string>();
  }

  if (!doc["connection_string"].IsNull())
  {
    res->connection_string = doc["connection_string"].as<std::string>();
  }

  if (!doc["listen_port"].IsNull())
  {
    res->listen_port = doc["listen_port"].as<short unsigned int>();
  }

  if (!doc["threads"].IsNull())
  {
    res->threads = std::max<unsigned int>(res->threads, doc["threads"].as<unsigned int>());
  }

  if (!doc["connection_timeout"].IsNull())
  {
    res->connection_timeout =
        std::min<unsigned int>(res->connection_timeout, doc["connection_timeout"].as<unsigned int>());
  }
  if (!doc["request_timeout"].IsNull())
  {
    res->request_timeout = std::min<unsigned int>(res->request_timeout, doc["request_timeout"].as<unsigned int>());
  }

  return res;

}

std::optional<ConfigPtr> GetConfigFromYaml(const std::filesystem::path& path_to_config)
{
  try
  {
    std::optional<ConfigPtr> result = ConstructConfigFromYaml(std::move(ParseYaml(path_to_config)));
    return result;
  }
  catch (const std::exception& exc)
  {
    spdlog::error("Exception while parsing {}\nException message:\n{}", path_to_config, exc.what());
    return std::nullopt;
  }
}

std::optional<ConfigPtr> ParseConfigFile(const std::filesystem::path& config_filepath)
{
  {
    std::error_code ec;
    if (!std::filesystem::exists(config_filepath, ec))
    {
      spdlog::error("Config file: {} does not exists\nError code message: {}", config_filepath, ec.message());
      return std::nullopt;
    }
  }

  const auto& config_extension = config_filepath.extension();
  if (!IsValidFileExtension(config_extension))
  {
    spdlog::error("Config file: {} has invalid file extension.\n", config_filepath);
    return std::nullopt;
  }

  FileExtensionId ext_id = GetFileExtensionId(config_extension);
  switch (ext_id)
  {
  case FileExtensionId::JSON:
    return GetConfigFromJson(config_filepath);
  case FileExtensionId::YAML:
    return GetConfigFromYaml(config_filepath);
  default:
    return std::nullopt;
  }
}

std::ostream& operator<<(std::ostream& os, const Config& config)
{
  os << "{\n"
     << "\terror_logfile: " << config.error_logfile << "\n"
     << "\tinfo_logfile: " << config.info_logfile << "\n"
     << "\tconnection_string: " << config.connection_string << "\n"
     << "\tthreads: " << config.threads << "\n"
     << "\tlisten_port: " << config.listen_port << "\n"
     << "\tconnection_timeout: " << config.connection_timeout << "\n"
     << "\trequest_timeout: " << config.request_timeout << "\n"
     << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const ConfigPtr& config_ptr)
{
  os << "{\n"
     << "\terror_logfile: " << config_ptr->error_logfile << "\n"
     << "\tinfo_logfile: " << config_ptr->info_logfile << "\n"
     << "\tconnection_string: " << config_ptr->connection_string << "\n"
     << "\tthreads: " << config_ptr->threads << "\n"
     << "\tlisten_port: " << config_ptr->listen_port << "\n"
     << "\tconnection_timeout: " << config_ptr->connection_timeout << "\n"
     << "\trequest_timeout: " << config_ptr->request_timeout << "\n"
     << "}";
  return os;
}

} // namespace server
} // namespace http