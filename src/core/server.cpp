#include "core/http/server.hpp"
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace core
{
namespace http
{
namespace server
{

Server::Server(const ConfigPtr& config, const RouterPtr& router)
    : session_processor_(static_cast<int>(config->threads))
    , config_ptr_(config)
    , router_ptr_(router)
    , listener_( {boost::asio::ip::tcp::v4(), config->listen_port}, session_processor_, config, router)
{
}

Router::RouterEasyInit Server::RegisterHandlers()
{
  return router_ptr_->RegisterHandlers();
}

void Server::Serve()
{
  session_processor_.Start();
  listener_.StartListening();
}

} // namespace server
} // namespace http

} // namespace core
