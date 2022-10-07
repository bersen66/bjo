#pragma once

#include <boost/asio.hpp>

#include "core/http/messages/messages.hpp"

namespace http {
namespace server {

using RouteHandler =
   std::function<boost::asio::awaitable<Response>(const Request &)>;

} // namespace server
} // namespace http