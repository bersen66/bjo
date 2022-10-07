#include "core/http/server/run_server.hpp"

#include <boost/asio.hpp>

#include "core/http/server/listener.hpp"
#include "core/http/server/routes/routes.hpp"

namespace http
{
namespace server
{

void RunServer(const ConfigPtr& config, const RouterPtr& router)
{
  int threads = static_cast<int>(config->threads);
  boost::asio::io_context ioc{threads};

  Listener listener(ioc, {boost::asio::ip::tcp::v4(), config->listen_port}, config, router);

  std::vector<std::thread> thread_pool(threads - 1);

  boost::asio::co_spawn(boost::asio::make_strand(ioc), listener.Listen(), boost::asio::detached);

  for (int i = 0; i < threads - 1; i++)
  {
    thread_pool.emplace_back([&ioc]() { ioc.run(); });
  }
  ioc.run();
}

} // namespace server
} // namespace http
