#pragma once

#include <boost/noncopyable.hpp>
#include <optional>
#include <regex>
#include <set>
#include <unordered_map>

#include "core/http/server/routes/handlers/handlers.hpp"

namespace http
{
namespace server
{

class HandlersMap : private boost::noncopyable
{
public:
  HandlersMap() = default;

  const RouteHandler& operator[](const std::string& route) const;

  bool CanHandle(const std::string& route) const;

  void Handle(const std::string& route_pattern, const RouteHandler& handler);

private:
  void InsertHandler(const std::string& route_pattern, const RouteHandler& handler);

  const RouteHandler& GetHandler(const std::string& route) const;

  bool Match(std::string_view route, const std::regex& pattern) const;

  std::optional<size_t> GetMatchingPattern(const std::string& route) const;

private:
private:
  std::vector<std::regex> route_patterns;
  std::unordered_map<size_t, RouteHandler> handlers; // {pattern_idx, handler}
};

} // namespace server
} // namespace http
