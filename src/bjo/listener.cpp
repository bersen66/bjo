#include <fmt/core.h>
#include <fmt/ostream.h>

#include <boost/asio.hpp>

#include <boost/stacktrace.hpp>

#include <utility>

#include "bjo/http/server.hpp"
#include "bjo/http/server/config.hpp"
#include "bjo/http/server/listener.hpp"

namespace bjo
{
namespace http
{
namespace server
{

Listener::Listener(const boost::asio::ip::tcp::endpoint& ep, TaskProcessor& session_processor, const ConfigPtr& config,
                   const RouterPtr& router)
    : task_processor_(1)
    , session_processor_(session_processor)
    , acceptor_(task_processor_.GetIOContext(), ep)
    , config_ptr_(config)
    , router_ptr_(router)

{
  if (infolog_ptr_)
    infolog_ptr_->debug("Construct http::Listener");
}

Listener::~Listener()
{
  if (infolog_ptr_)
    infolog_ptr_->debug("http::Listener destructed!");
}

void Listener::StartListening()
{
  task_processor_.ProcessTask(Listen(), boost::asio::detached); // posting listen task to event loop
  task_processor_.Start<TaskProcessor::START_MODE::USE_THIS_THREAD>();
}

boost::asio::awaitable<void> Listener::Listen()
{

  if (infolog_ptr_)
    infolog_ptr_->info("Start accepting.");

  for (;;)
  {
    try
    {
      if (infolog_ptr_)
        infolog_ptr_->debug("Doing accept");
      auto client_socket = co_await acceptor_.async_accept(boost::asio::use_awaitable);

      if (infolog_ptr_)
        infolog_ptr_->info("Connection opened");
      session_processor_.ProcessTask(
          RunSession(std::move(client_socket), infolog_ptr_, errorlog_ptr_, config_ptr_, router_ptr_),
          boost::asio::detached);
    }
    catch (const std::exception& exc)
    {
      if (errorlog_ptr_)
        errorlog_ptr_->error("\nException while http listening:\nException message: "
                             "{}\nStacktrace: {}\n",
                             exc.what(), boost::stacktrace::stacktrace());
    }
  }
}

void Listener::AttachErrorLogger(LoggerPtr logger_ptr)
{
  errorlog_ptr_ = logger_ptr;
}

void Listener::AttachInfoLogger(LoggerPtr logger_ptr)
{
  infolog_ptr_ = logger_ptr;
}

} // namespace server
} // namespace http
} // namespace bjo
