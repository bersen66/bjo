#pragma once

#include <boost/asio.hpp>

#include "core/http/server/config/config.hpp"
#include "core/http/server/routes/routes.hpp"

namespace http
{
namespace server
{

boost::asio::awaitable<void> RunSession(boost::asio::ip::tcp::socket client_socket,
                                        const ConfigPtr& config = DefaultConfig(),
                                        const RouterPtr& routes = EmptyRouter());

} // namespace server
} // namespace http
