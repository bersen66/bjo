#include <fmt/core.h>
#include <fmt/ostream.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#define BOOST_STACKTRACE_USE_BACKTRACE
#include <boost/stacktrace.hpp>

#include <utility>

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

#include "core/http/server/config.hpp"
#include "core/http/server/listener.hpp"
#include "core/http/server/session/session.hpp"

namespace core
{
namespace http
{
namespace server
{

Listener::Listener(const boost::asio::ip::tcp::endpoint& ep, TaskProcessor& session_processor, const ConfigPtr& config,
                   const RouterPtr& router)
    : accept_prcocessor_(1)
    , session_processor_(session_processor)
    , acceptor_(accept_prcocessor_.GetIOContext(), ep)
    , config_ptr_(config)
    , router_ptr_(router)
{
  spdlog::info("Construct http::Listener");
}

Listener::~Listener()
{
  spdlog::info("http::Listener destructed!");
}

void Listener::StartListening()
{
  accept_prcocessor_.ProcessTask(Listen(), boost::asio::detached);
  accept_prcocessor_.Start<TaskProcessor::START_MODE::USE_THIS_THREAD>();
}

boost::asio::awaitable<void> Listener::Listen()
{
  spdlog::info("RunServer http listening");
  for (;;)
  {
    try
    {
      auto client_socket = co_await acceptor_.async_accept(boost::asio::use_awaitable);
      session_processor_.ProcessTask(RunSession(std::move(client_socket), config_ptr_, router_ptr_),
                                     boost::asio::detached);
    }
    catch (const std::exception& exc)
    {

      // make async
      spdlog::error("\nException while http listening:\nException message: "
                    "{}\nStacktrace: {}\n",
                    exc.what(), boost::stacktrace::stacktrace());
    }
  }
}

} // namespace server
} // namespace http
} // namespace core
