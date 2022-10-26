#pragma once

#include <boost/asio.hpp>

#include "bjo/http/server/logger_ptr.hpp"
#include "bjo/http/server.hpp"
#include "bjo/http/server/config.hpp"
#include "bjo/http/server/routes/routes.hpp"


namespace bjo
{
namespace http
{
namespace server
{

boost::asio::awaitable<void> RunSession(boost::asio::ip::tcp::socket client_socket,
                                        LoggerPtr infolog_ptr,
                                        LoggerPtr errorlog_ptr,
                                        const ConfigPtr& config = DefaultConfig(),
                                        const RouterPtr& routes = EmptyRouter());

} // namespace server
} // namespace http
} // namespace bjo