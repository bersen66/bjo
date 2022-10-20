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
    : io_context_(boost::asio::io_context{static_cast<int>(config->threads)})
    , config_ptr_(config)
    , router_ptr_(router)
    , listener_(io_context_, {boost::asio::ip::tcp::v4(), config->listen_port}, config, router)
{
}

Router::RouterEasyInit Server::RegisterHandlers()
{
  return router_ptr_->RegisterHandlers();
}

void Server::Serve()
{
  /*
  int threads = static_cast<int>(config_ptr_->threads);
  std::vector<std::thread> thread_pool(threads - 1);

  boost::asio::co_spawn(boost::asio::make_strand(io_context_), listener_.Listen(), boost::asio::detached);

  auto& ioc = io_context_;

  for (int i = 0; i < threads - 1; i++)
  {
    thread_pool.emplace_back([&ioc]() { ioc.run(); });
  }
  io_context_.run();
   */
  auto& ioc = io_context_;
  int threads = static_cast<int>(config_ptr_->threads - 1);
  boost::thread_group pool;

  boost::asio::co_spawn(boost::asio::make_strand(io_context_), listener_.Listen(), boost::asio::detached);
  for (int i = 0; i < threads; i++)
  {
    pool.create_thread([&ioc]() { ioc.run(); });
  }
  io_context_.run();
}

} // namespace server
} // namespace http

} // namespace core
