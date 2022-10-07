#include "core/http/server/routes/router.hpp"

namespace http
{
namespace server
{

Router::RouterEasyInit::RouterEasyInit(Router* router_ptr) : router_ptr(router_ptr)
{
}

Router::RouterEasyInit& Router::RouterEasyInit::operator()(METHODS methods, const std::string& route,
                                                           const RouteHandler& handler)
{
  router_ptr->InsertRoute(methods, route, handler);
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

bool HasGetFlag(METHODS methods) {
  return static_cast<unsigned int>(METHODS::GET) & static_cast<unsigned int>(methods);
}

bool HasPutFlag(METHODS methods) {
  return static_cast<unsigned int>(METHODS::PUT) & static_cast<unsigned int>(methods);
}

bool HasPostFlag(METHODS methods) {
  return static_cast<unsigned int>(METHODS::POST) & static_cast<unsigned int>(methods);
}

bool HasDeleteFlag(METHODS methods) {
  return static_cast<unsigned int>(METHODS::DELETE) & static_cast<unsigned int>(methods);
}

bool HasPatchFlag(METHODS methods) {
  return static_cast<unsigned int>(METHODS::PATCH) & static_cast<unsigned int>(methods);
}

bool HasOptionsFlag(METHODS methods) {
  return static_cast<unsigned int>(METHODS::OPTIONS) & static_cast<unsigned int>(methods);
}


void Router::InsertRoute(METHODS methods, const std::string& route, const RouteHandler& handler)
{
  if (HasGetFlag(methods) ) {
    method_map[METHODS::GET].Handle(route, handler);
  }

  if (HasPostFlag(methods) ) {
    method_map[METHODS::POST].Handle(route, handler);
  }

  if (HasPutFlag(methods) ) {
    method_map[METHODS::PUT].Handle(route, handler);
  }

  if (HasDeleteFlag(methods) ) {
    method_map[METHODS::DELETE].Handle(route, handler);
  }

  if (HasPatchFlag(methods) ) {
    method_map[METHODS::PATCH].Handle(route, handler);
  }

  if (HasOptionsFlag(methods)) {
    method_map[METHODS::OPTIONS].Handle(route, handler);
  }
}

RouterPtr EmptyRouter()
{
  RouterPtr result = std::make_shared<Router>();
  return result;
}

} // namespace server
} // namespace http