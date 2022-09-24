#pragma once

#include <boost/asio.hpp>

#include "core/http/messages/messages.hpp"

namespace http {
namespace server {

using RouteHandler =
   std::function<boost::asio::awaitable<Response>(const Request &)>;


struct HandlerBase {
  virtual boost::asio::awaitable<Response> operator()(const Request&) const = 0;
  virtual ~HandlerBase() = default;
};

} // namespace server
} // namespace http