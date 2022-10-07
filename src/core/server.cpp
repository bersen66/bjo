#include "core/http/server/server.hpp"

#include <boost/asio.hpp>

namespace http {
namespace server {

Server::Server(boost::asio::io_context& ioc, const ConfigPtr& config,
               const RouterPtr& router)
    : io_context_(ioc),
      config_ptr_(config),
      router_ptr_(router),
      listener_(io_context_, {boost::asio::ip::tcp::v4(), config->listen_port},
                config, router) {}

Router::RouterEasyInit Server::RegisterHandlers() {
  return router_ptr_->RegisterHandlers();
}

}  // namespace server
}  // namespace http
