#pragma once
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "core/http/server/config/config.hpp"
#include "core/http/server/routes/router.hpp"

namespace http {
namespace server {

class Listener : private boost::noncopyable {
public:
  Listener(boost::asio::io_context& ioc,
           const boost::asio::ip::tcp::endpoint& ep,
           const ConfigPtr& config = DefaultConfig(),
           const RouterPtr& router = DefaultRouter());

  ~Listener();
  boost::asio::awaitable<void> Listen();

private:
  boost::asio::io_context& ioc_;
  boost::asio::ip::tcp::acceptor acceptor_;
  RouterPtr routes;
  ConfigPtr config;
};

}  // namespace server
}  // namespace http
