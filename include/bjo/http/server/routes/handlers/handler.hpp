#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <functional>
#include "bjo/http/messages.hpp"

namespace bjo
{
namespace http
{
namespace server
{

//struct HandlerBase : private boost::noncopyable
//{
//  [[nodiscard]] virtual bool CanHandle(std::string_view) const = 0;
//  [[nodiscard]] virtual boost::asio::awaitable<http::Response> Handle(const http::Request&) const = 0;
//  virtual ~HandlerBase() = default;
//};

using HandlerHolder = std::function<boost::asio::awaitable<bjo::http::Response>(const bjo::http::Request&)>;

} // namespace server
} // namespace http
} // namespace bjo
