#pragma once

#include <boost/noncopyable.hpp>
#include <optional>
#include <regex>
#include <set>
#include <unordered_map>

#include "core/http/server/routes/handlers/handlers.hpp"

namespace http {
namespace server {

class HandlersMap : boost::noncopyable {
public:
  HandlersMap() = default;

  const RouteHandler &operator[](const std::string &route) const;

  bool CanHandle(const std::string &route) const;

  void Handle(const std::string &route_pattern, const RouteHandler &handler);

private:
  void InsertHandler(const std::string &route_pattern,
                     const RouteHandler &handler);

  const RouteHandler &GetHandler(const std::string &route) const;

  bool Match(std::string_view route, std::string_view pattern) const;

  std::optional<std::string> GetMatchingPattern(const std::string &route) const;

private:
  struct Sorter {
    bool operator()(const std::string &lhs, const std::string &rhs) const {
      return lhs > rhs;
    }
  };

private:
  std::set<std::string, Sorter> route_patterns;
  std::unordered_map<std::string, RouteHandler> handlers; // {pattern, handler}
};

} // namespace server
} // namespace http
