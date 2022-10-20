#include "core/http/server/routes/handlers/handlers_map.hpp"

#include <algorithm>
#include <optional>

#include "core/http/server/routes/handlers/handlers.hpp"

namespace core
{
namespace http
{
namespace server
{

const HandlerHolder& HandlersMap::operator[](std::string_view route) const
{
  return GetHandler(route);
}

bool HandlersMap::CanHandle(std::string_view route) const
{
  for (const auto& handler_ptr : handlers_)
  {
    if (handler_ptr->CanHandle(route))
    {
      return true;
    }
  }
  return false;
}

void HandlersMap::InsertHandler(HandlerHolder&& handler)
{
  handlers_.emplace_back(std::move(handler));
}

const HandlerHolder& HandlersMap::GetHandler(std::string_view route) const
{
  for (const auto& handler_ptr : handlers_)
  {
    if (handler_ptr->CanHandle(route))
    {
      return handler_ptr;
    }
  }
  throw std::runtime_error("HandlersMap: no handlers for this route.");
}

} // namespace server
} // namespace http
} // namespace core