#pragma once
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "bjo/http/server.hpp"
#include "bjo/http/server/config.hpp"
#include "bjo/http/server/logger_ptr.hpp"
#include "bjo/http/server/routes/router.hpp"
#include "bjo/task_processor.hpp"

namespace bjo
{
namespace http
{
namespace server
{

class Listener : private boost::noncopyable
{
public:
  Listener(const boost::asio::ip::tcp::endpoint& ep, TaskProcessor& session_processor,
           const ConfigPtr& config = DefaultConfig(), const RouterPtr& router = EmptyRouter());

  ~Listener();

  void StartListening();

  void AttachErrorLogger(LoggerPtr logger_ptr);
  void AttachInfoLogger(LoggerPtr logger_ptr);

private:
  boost::asio::awaitable<void> Listen();

private:
  TaskProcessor task_processor_;     // where we're accepting new connections
  TaskProcessor& session_processor_; // push sessions there
  boost::asio::ip::tcp::acceptor acceptor_;
  RouterPtr router_ptr_;
  ConfigPtr config_ptr_;
  LoggerPtr infolog_ptr_;
  LoggerPtr errorlog_ptr_;
};

} // namespace server
} // namespace http
} // namespace bjo
