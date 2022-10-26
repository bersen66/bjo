#include "bjo/http/server.hpp"
#include <boost/asio.hpp>

namespace bjo
{
namespace http
{
namespace server
{

Server::Server(const ConfigPtr& config, const RouterPtr& router)
    : session_processor_(static_cast<int>(config->threads))
    , config_ptr_(config)
    , router_ptr_(router)
    , error_log_(bjo::InitLogger("server_err", config->error_logfile))
    , info_log_(bjo::InitLogger("server_info", config->info_logfile))
    , listener_({boost::asio::ip::tcp::v4(), config->listen_port}, session_processor_, config, router)

{
  listener_.AttachErrorLogger(error_log_);
  listener_.AttachInfoLogger(info_log_);
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
} // namespace bjo
