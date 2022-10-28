#pragma once

#include "bjo/http/server/config.hpp"
#include "bjo/http/server/listener.hpp"
#include "bjo/http/server/routes.hpp"
#include "bjo/http/server/session/session.hpp"
#include "bjo/task_processor.hpp"

#include <memory> // for std::shared_ptr

#include "bjo/http/server/logger_ptr.hpp"

namespace bjo
{
namespace http
{

class Server
{
public:
  explicit Server(const server::ConfigPtr& config = http::server::DefaultConfig(),
                  const server::RouterPtr& router = http::server::EmptyRouter());

  server::Router::RouterEasyInit RegisterHandlers();

  void Serve();

  void AttachInfoLogger(LoggerPtr logger);
  void AttachErrorLogger(LoggerPtr logger);


private:
  TaskProcessor session_processor_;
  server::Listener listener_;
  server::RouterPtr router_ptr_;
  server::ConfigPtr config_ptr_;
  LoggerPtr info_log_;
  LoggerPtr error_log_;
};

} // namespace http
} // namespace bjo
