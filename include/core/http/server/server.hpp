#pragma once

#include "core/http/server/config/config.hpp"
#include "core/http/server/listener.hpp"
#include "core/http/server/routes/routes.hpp"
#include "core/http/server/run_server.hpp"
#include "core/http/server/session/session.hpp"

namespace http {
namespace server {

class Server {
public:
  explicit Server(boost::asio::io_context& ioc,
                  const ConfigPtr& config,
                  const RouterPtr& router);

  Router::RouterEasyInit RegisterHandlers();

private:
  boost::asio::io_context& io_context_;
  Listener listener_;
  RouterPtr router_ptr_;
  ConfigPtr config_ptr_;
};

}  // namespace server
}  // namespace http
