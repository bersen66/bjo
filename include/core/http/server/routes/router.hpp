#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include <unordered_map>

#include "core/http/server/routes/handlers/handlers_map.hpp"

namespace http
{
namespace server
{

class Router : boost::noncopyable
{
public:
  class RouterEasyInit : boost::noncopyable
  {
  public:
    explicit RouterEasyInit(Router* router_ptr);

    RouterEasyInit& operator()(METHODS methods, const std::string& route, RouteHandler handler);

  private:
    Router* router_ptr;
  };

public:
  RouterEasyInit RegisterHandlers();

  bool Contains(const std::string& route);

  const HandlersMap& operator[](METHODS method) const;

private:
  void InsertRoute(METHODS methods, const std::string& route, const RouteHandler& handler);

private:
  std::unordered_map<METHODS, HandlersMap> method_map;
};

using RouterPtr = std::shared_ptr<Router>;

RouterPtr EmptyRouter();

} // namespace server
} // namespace http