#pragma once
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "config.hpp"
#include "core/http/server/routes/router.hpp"
#include "core/task_processor.hpp"

namespace core
{
namespace http
{
namespace server
{

class Listener : private boost::noncopyable
{
public:
  Listener(const boost::asio::ip::tcp::endpoint& ep,
           TaskProcessor& session_processor,
           const ConfigPtr& config = DefaultConfig(), const RouterPtr& router = EmptyRouter());

  ~Listener();

  void StartListening();

private:
  boost::asio::awaitable<void> Listen();
private:
  TaskProcessor accept_prcocessor_; // where we're accepting new connections
  TaskProcessor& session_processor_; // push sessions there
  boost::asio::ip::tcp::acceptor acceptor_;
  RouterPtr router_ptr_;
  ConfigPtr config_ptr_;
};

} // namespace server
} // namespace http
} // namespace core


