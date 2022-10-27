

#include <optional>

#include "bjo/http/server/handler.hpp"
#include "bjo/http/server/routes/handlers/handlers.hpp"
#include "bjo/http/server/routes/handlers/handlers_map.hpp"
#include "profile.hpp"

namespace bjo
{
namespace http
{
namespace server
{

const HandlerHolder& HandlersMap::operator[](std::string_view route) const
{
  //LOG_DURATION("GET HANDLER TIME");
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
  //LOG_DURATION("INSERT HANDLER TIME");
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
} // namespace bjo