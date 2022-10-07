#include <fmt/core.h>
#include <fmt/ostream.h>

#include <boost/asio.hpp>

#include <boost/beast.hpp>

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

#include "core/http/connection.hpp"

#include "core/http/server/session/session.hpp"

#include "core/http/server/config/config.hpp"

namespace http {
namespace server {

METHODS GetMethod(const Request &req) {
  namespace bhttp = boost::beast::http;
  static const std::unordered_map<bhttp::verb, METHODS> resolver = {
      {bhttp::verb::get, METHODS::GET},
      {bhttp::verb::delete_, METHODS::DELETE},
      {bhttp::verb::post, METHODS::POST},
  };

  return resolver.at(req.method());
}

std::string GetRoute(const Request &req) {
  const auto &target = req.target();
  return {target.begin(), target.end()};
}

boost::asio::awaitable<void>
RunSession(boost::asio::ip::tcp::socket client_socket, const ConfigPtr &config,
           const RouterPtr &routes) {
  Connection conn(std::move(client_socket), config->connection_timeout);
  const auto &router_map = *routes;
  for (; conn.IsAlive();) {
    auto optional_req = co_await conn.FetchRequest();

    if (!optional_req.has_value()) {
      break;
    }

    auto &request = optional_req.value();

    spdlog::info("Fetched request!\n{}\n", request);
    try {
      auto handler = router_map[GetMethod(request)][GetRoute(request)];
      Response res = co_await handler(request);
      spdlog::info("Generated response:\n {}\n", res);
      co_await conn.SendResponse(std::move(res));
    } catch (const std::exception &exc) {
      spdlog::error("Bad request! Message: {}\n", exc.what());
      conn.Disconnect();
    }
  }
}

} // namespace server
} // namespace http
