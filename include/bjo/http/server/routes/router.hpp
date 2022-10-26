#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include <unordered_map>

#include "bjo/http/server/routes/handlers/handlers_map.hpp"

namespace bjo
{
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

    RouterEasyInit& operator()(METHODS methods, HandlerHolder handler);

  private:
    Router* router_ptr;
  };

public:
  RouterEasyInit RegisterHandlers();

  bool Contains(std::string_view route) const;

  const HandlersMap& operator[](METHODS method) const;

private:
  void InsertHandler(METHODS methods, HandlerHolder&& handler);

private:
  std::unordered_map<METHODS, HandlersMap> method_map;
};

using RouterPtr = std::shared_ptr<Router>;

RouterPtr EmptyRouter();

} // namespace server
} // namespace http
} // namespace bjo
