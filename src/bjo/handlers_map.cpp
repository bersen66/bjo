
#include <optional>


#include "bjo/http/server/routes/handlers/handler.hpp"
#include "bjo/http/server/routes/handlers/handlers_map.hpp"
#include "bjo/http/server/routes/route.hpp"


namespace bjo
{
namespace http
{
namespace server
{

const HandlerHolder& HandlersMap::operator[](std::string_view route) const
{
  return GetHandler(route);
}

void HandlersMap::InsertHandler(PatternPtr&& pattern_ptr, HandlerHolder&& handler)
{
  handlers_[std::move(pattern_ptr)] = std::move(handler);
}

const HandlerHolder& HandlersMap::GetHandler(std::string_view route) const
{
  for (const auto& [pattern_ptr, handler] : handlers_)
  {
    if (pattern_ptr->Match(route))
    {
      return handler;
    }
  }
  throw std::runtime_error("HandlersMap: no handlers for this route.");
}

} // namespace server
} // namespace http
} // namespace bjo