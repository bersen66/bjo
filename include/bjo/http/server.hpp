#pragma once

#include "bjo/http/server/config.hpp"
#include "bjo/http/server/listener.hpp"
#include "bjo/http/server/routes/routes.hpp"
#include "bjo/http/server/session/session.hpp"
#include "bjo/task_processor.hpp"

#include <memory> // for std::shared_ptr

#include "bjo/http/server/logger_ptr.hpp"

namespace bjo
{
namespace http
{
namespace server
{

class Server
{

public:
  explicit Server(const ConfigPtr& config = http::server::DefaultConfig(),
                  const RouterPtr& router = http::server::EmptyRouter());

  Router::RouterEasyInit RegisterHandlers();

  void Serve();

private:
  TaskProcessor session_processor_;
  Listener listener_;
  RouterPtr router_ptr_;
  ConfigPtr config_ptr_;
  LoggerPtr info_log_;
  LoggerPtr error_log_;
};

} // namespace server
} // namespace http
} // namespace bjo
