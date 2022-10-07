#include "core/http/server/routes/router.hpp"

namespace http
{
namespace server
{

Router::RouterEasyInit::RouterEasyInit(Router* router_ptr) : router_ptr(router_ptr)
{
}

Router::RouterEasyInit& Router::RouterEasyInit::operator()(METHODS method, const std::string& route,
                                                           RouteHandler handler)
{
  router_ptr->InsertRoute(method, route, handler);
  return *this;
}

Router::RouterEasyInit Router::RegisterHandlers()
{
  return Router::RouterEasyInit(this);
}

bool Router::Contains(const std::string& route)
{
  for (const auto& [method, handlers_map] : method_map)
  {
    if (handlers_map.CanHandle(route))
    {
      return true;
    }
  }
  return false;
}

const HandlersMap& Router::operator[](METHODS method) const
{
  return method_map.at(method);
}

void Router::InsertRoute(METHODS method, const std::string& route, const RouteHandler& handler)
{
  method_map[method].Handle(route, handler);
}

RouterPtr EmptyRouter()
{
  RouterPtr result = std::make_shared<Router>();
  return result;
}

} // namespace server
} // namespace http