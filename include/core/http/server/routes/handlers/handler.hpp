#pragma once

#include <boost/asio.hpp>

#include "core/http/messages/messages.hpp"

namespace http
{
namespace server
{

using RouteHandler = std::function<boost::asio::awaitable<Response>(const Request&)>;

struct HandlerBase {
  [[nodiscard]] virtual bool CanHandle(std::string_view) const = 0;
  [[nodiscard]] virtual boost::asio::awaitable<http::Response> Handle(const http::Request&) const = 0;
  virtual ~HandlerBase() = default;
};

using HandlerHolder = std::unique_ptr<HandlerBase>;

} // namespace server
} // namespace http