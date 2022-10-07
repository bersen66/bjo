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
  return GetMatchingPattern(route).has_value();
}

void HandlersMap::Handle(const std::string &route_pattern,
                         const RouteHandler &handler) {

  InsertHandler(route_pattern, handler);
}

void HandlersMap::InsertHandler(const std::string &route_pattern,
                                const RouteHandler &handler) {

  route_patterns.emplace_back(std::regex(route_pattern));
  handlers[route_patterns.size()-1] = handler;
}

const RouteHandler &HandlersMap::GetHandler(const std::string &route) const {

  std::optional<size_t> matching_pattern = GetMatchingPattern(route);
  if (matching_pattern.has_value()) {
    return handlers.at(matching_pattern.value());
  } else {
    throw std::runtime_error("HandlersMap: can't get matching pattern.");
  }
}

bool HandlersMap::Match(std::string_view route, const std::regex& regex) const {
  return std::regex_match(route.begin(), route.end(), regex);
}

std::optional<size_t>
HandlersMap::GetMatchingPattern(const std::string &route) const {

  for (size_t i = 0; i < route_patterns.size(); i++) {
    if (Match(route, route_patterns[i])) {
      return std::make_optional(i);
    }
  }

  return std::nullopt;
}

} // namespace server
} // namespace http
