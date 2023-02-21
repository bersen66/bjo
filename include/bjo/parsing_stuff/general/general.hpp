#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <json/json.h>
#include <yaml-cpp/yaml.h>


namespace bjo
{
namespace parsers
{

std::vector<std::string_view> SplitLineIntoTokenViews(std::string_view line, std::string_view delimeter = " ");

std::vector<std::string> SplitLineIntoTokens(std::string_view line, std::string_view delimeter = " ");

std::string_view NextTokenView(std::string_view& s, std::string_view delimiter = " ");

std::string GetTextFromStream(std::istream& input);

std::optional<std::string> GetTextFromFile(const std::filesystem::path& path_to_file);

Json::Value ParseJson(const std::filesystem::path& path);

YAML::Node ParseYaml(const std::filesystem::path& path_to_config);


} // namespace parsers
} // namespace bjo