#include <utility>

#include "bjo/http/messages.hpp"
#include "bjo/http/server/routes/handlers/handler.hpp"
#include "bjo/http/server/routes/router.hpp"

namespace bjo
{
namespace http
{
namespace server
{

Router::RouterEasyInit::RouterEasyInit(Router* router_ptr)
    : router_ptr(router_ptr)
{
}

Router::RouterEasyInit& Router::RouterEasyInit::operator()(METHODS methods, PatternPtr&& route, HandlerHolder handler)
{
  router_ptr->InsertHandler(methods, std::move(route), std::move(handler));
  return *this;
}

Router::RouterEasyInit Router::RegisterHandlers()
{
  return Router::RouterEasyInit(this);
}

const HandlersMap& Router::operator[](METHODS method) const
{
  return method_map.at(method);
}

bool HasGetFlag(METHODS methods)
{
  return static_cast<unsigned int>(METHODS::GET) & static_cast<unsigned int>(methods);
}

bool HasPutFlag(METHODS methods)
{
  return static_cast<unsigned int>(METHODS::PUT) & static_cast<unsigned int>(methods);
}

bool HasPostFlag(METHODS methods)
{
  return static_cast<unsigned int>(METHODS::POST) & static_cast<unsigned int>(methods);
}

bool HasDeleteFlag(METHODS methods)
{
  return static_cast<unsigned int>(METHODS::DELETE) & static_cast<unsigned int>(methods);
}

bool HasPatchFlag(METHODS methods)
{
  return static_cast<unsigned int>(METHODS::PATCH) & static_cast<unsigned int>(methods);
}

bool HasOptionsFlag(METHODS methods)
{
  return static_cast<unsigned int>(METHODS::OPTIONS) & static_cast<unsigned int>(methods);
}

void Router::InsertHandler(METHODS methods, PatternPtr&& pattern, HandlerHolder&& handler)
{
  if (HasGetFlag(methods))
  {
    method_map[METHODS::GET].InsertHandler(std::move(pattern),std::move(handler));
  }

  if (HasPostFlag(methods))
  {
    method_map[METHODS::POST].InsertHandler(std::move(pattern),std::move(handler));
  }

  if (HasPutFlag(methods))
  {
    method_map[METHODS::PUT].InsertHandler(std::move(pattern),std::move(handler));
  }

  if (HasDeleteFlag(methods))
  {
    method_map[METHODS::DELETE].InsertHandler(std::move(pattern),std::move(handler));
  }

  if (HasPatchFlag(methods))
  {
    method_map[METHODS::PATCH].InsertHandler(std::move(pattern),std::move(handler));
  }

  if (HasOptionsFlag(methods))
  {
    method_map[METHODS::OPTIONS].InsertHandler(std::move(pattern),std::move(handler));
  }
}

RouterPtr EmptyRouter()
{
  RouterPtr result = std::make_shared<Router>();
  return result;
}

} // namespace server
} // namespace http
} // namespace bjo
