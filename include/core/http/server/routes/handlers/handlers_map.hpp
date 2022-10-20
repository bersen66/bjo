#pragma once

#include <boost/noncopyable.hpp>
#include <optional>
#include <regex>
#include <set>
#include <unordered_map>

#include "core/http/server/routes/handlers/handlers.hpp"

namespace core
{
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

private:
  std::vector<HandlerHolder> handlers_;
};

} // namespace server
} // namespace http
} // namespace core


