#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <json/json.h>
#include <yaml-cpp/yaml.h>


namespace core
{
namespace parsers
{

std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(std::string_view line_to_split,
                                                                            std::string_view delimeter = " ");

std::vector<std::string_view> SplitLineIntoTokensViews(std::string_view line, std::string_view delimeter = " ");

std::vector<std::string> SplitLineIntoTokens(std::string_view line, std::string_view delimeter = " ");

std::pair<std::string_view, std::string_view> SplitTwo(std::string_view s, std::string_view delimiter = " ");

std::string_view ReadToken(std::string_view& s, std::string_view delimiter = " ");

std::string GetTextFromStream(std::istream& input);

std::optional<std::string> GetTextFromFile(const std::filesystem::path& path_to_file);

int NextInt(std::string_view& s, std::string_view delimeter = " ");

double NextDouble(std::string_view& s, std::string_view delimeter = " ");

Json::Value ParseJson(const std::filesystem::path& path);

YAML::Node ParseYaml(const std::filesystem::path& path_to_config);


} // namespace parsers
} // namespace core