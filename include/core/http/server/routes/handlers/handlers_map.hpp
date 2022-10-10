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

  const HandlerHolder& operator[](std::string_view route) const;

  [[nodiscard]] bool CanHandle(std::string_view route) const;

  void InsertHandler(HandlerHolder&& handler);

private:

  [[nodiscard]] const HandlerHolder& GetHandler(std::string_view route) const;

//  void InsertHandler(const std::string& route_pattern, const RouteHandler& handler);
//
//  const RouteHandler& GetHandler(const std::string& route) const;
//
//  bool Match(std::string_view route, const std::regex& pattern) const;
//
//  std::optional<size_t> GetMatchingPattern(const std::string& route) const;

private:
//  std::vector<std::regex> route_patterns;
//  std::unordered_map<size_t, RouteHandler> handlers; // {pattern_idx, handler}

  std::vector<HandlerHolder> handlers_;
};

} // namespace server
} // namespace http
