#include "core/parsing_stuff/parsing_stuff.hpp"

#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <json/json.h>

namespace parsers
{

std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(std::string_view line_to_split,
                                                                            std::string_view delimeter)
{
  const size_t pos = line_to_split.find(delimeter);
  if (pos == std::string_view::npos)
  {
    return {line_to_split, std::nullopt};
  }
  else
  {
    return {line_to_split.substr(0, pos), line_to_split.substr(pos + delimeter.length())};
  }
}

std::vector<std::string_view> SplitLineIntoTokensViews(std::string_view line, std::string_view delimeter)
{
  std::vector<std::string_view> result;

  do
  {
    auto [l, r] = SplitTwoStrict(line, delimeter);
    if (!l.empty())
    {
      result.push_back(std::string(l));
      line = r.value_or(std::string_view(""));
    }
  } while (!line.empty());

  return result;
}

std::vector<std::string> SplitLineIntoTokens(std::string_view line, std::string_view delimeter)
{
  std::vector<std::string> result;

  do
  {
    auto [l, r] = SplitTwoStrict(line, delimeter);
    if (!l.empty())
    {
      result.push_back(move(std::string(l)));
      line = r.value_or("");
    }
  } while (!line.empty());

  return result;
}

std::pair<std::string_view, std::string_view> SplitTwo(std::string_view s, std::string_view delimiter)
{
  const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
  return {lhs, rhs_opt.value_or("")};
}

std::string_view ReadToken(std::string_view& s, std::string_view delimiter)
{
  const auto [lhs, rhs] = SplitTwo(s, delimiter);
  s = rhs;
  return lhs;
}

std::string GetTextFromStream(std::istream& input)
{
  return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
std::optional<std::string> GetTextFromFile(const std::filesystem::path& path_to_file)
{
  if (!std::filesystem::exists(path_to_file))
  {
    return std::nullopt;
  }

  std::ifstream file(path_to_file);
  if (file.is_open())
  {
    return std::make_optional<std::string>(GetTextFromStream(file));
  }
  else
  {
    return std::nullopt;
  }
}

int NextInt(std::string_view& s, std::string_view delimeter)
{
  std::string_view token = ReadToken(s, delimeter);
  return boost::lexical_cast<int>(token);
}

double NextDouble(std::string_view& s, std::string_view delimeter)
{
  std::string_view token = ReadToken(s, delimeter);
  return boost::lexical_cast<double>(token);
}

Json::Value ParseJson(const std::filesystem::path& path)
{
  Json::Value result;
  std::ifstream config_file(path);
  config_file >> result;
  return result;
}

} // namespace parsers
