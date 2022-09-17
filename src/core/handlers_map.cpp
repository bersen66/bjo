#include "core/http/server/routes/handlers/handlers_map.hpp"
#include "core/http/server/routes/handlers/handlers.hpp"

#include <algorithm>
#include <optional>

namespace http {
namespace server {

const RouteHandler &HandlersMap::operator[](const std::string &route) const {
  return GetHandler(route);
}

bool HandlersMap::CanHandle(const std::string &route) const {
  return handlers.contains(route);
}

void HandlersMap::Handle(const std::string &route_pattern,
                         const RouteHandler &handler) {

  InsertHandler(route_pattern, handler);
}

void HandlersMap::InsertHandler(const std::string &route_pattern,
                                const RouteHandler &handler) {

  route_patterns.insert(route_pattern);
  handlers[route_pattern] = handler;
}

const RouteHandler &HandlersMap::GetHandler(const std::string &route) const {

  std::optional<std::string> matching_pattern = GetMatchingPattern(route);
  if (matching_pattern.has_value()) {
    return handlers.at(matching_pattern.value());
  } else {
    throw std::runtime_error("HandlersMap: can't get matching pattern.");
  }
}

bool HandlersMap::Match(std::string_view route,
                        std::string_view pattern) const {

  return route.starts_with(pattern);
}

std::optional<std::string>
HandlersMap::GetMatchingPattern(const std::string &route) const {

  for (const auto &pattern : route_patterns) {
    if (Match(route, pattern)) {
      return std::make_optional(pattern);
    }
  }
  return std::nullopt;
}

} // namespace server
} // namespace http
