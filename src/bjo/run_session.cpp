#include <fmt/core.h>
#include <fmt/ostream.h>

#include <boost/asio.hpp>

#include <boost/beast.hpp>

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

#include "bjo/http/connection.hpp"
#include "bjo/http/server/config.hpp"
#include "bjo/http/server/session/session.hpp"
#include "bjo/http/server.hpp"
#include "bjo/http/server/logger_ptr.hpp"



namespace bjo
{
namespace http
{
namespace server
{

METHODS GetMethod(const Request& req)
{
  //LOG_DURATION("GET METHOD TIME");
  namespace bhttp = boost::beast::http;
  static const std::unordered_map<bhttp::verb, METHODS> resolver = {
      {bhttp::verb::get, METHODS::GET},
      {bhttp::verb::delete_, METHODS::DELETE},
      {bhttp::verb::post, METHODS::POST},
  };

  return resolver.at(req.method());
}

boost::asio::awaitable<void> RunSession(boost::asio::ip::tcp::socket client_socket,
                                        LoggerPtr infolog_ptr,
                                        LoggerPtr errorlog_ptr,
                                        const ConfigPtr& config,
                                        const RouterPtr& routes)
{
  LOG_DURATION("SESSION TIME");
  Connection conn(std::move(client_socket), config->connection_timeout);
  static std::atomic<long long int> session_id = 0;
  long long int id = ++session_id;

  const auto& router_map = *routes;

  infolog_ptr->info("Session id{} started", id);
  try
  {
    for (; conn.IsAlive();)
    {

      auto optional_req = co_await conn.FetchRequest();

      if (!optional_req.has_value())
      {
        break;
      }

      auto& request = optional_req.value();

      infolog_ptr->info("SessionId{}: Fetched request!\n{}\n", id, request);

      const auto& handler = router_map[GetMethod(request)][
          std::string_view(request.target().data(), request.target().size())
      ];
      Response res = co_await handler(request);
      infolog_ptr->info("SessionId{}: Generated response:\n {}\n",id, res);
      co_await conn.SendResponse(std::move(res));
    }
  }
  catch (const std::exception& exc)
  {
    errorlog_ptr->error("SessionId{}: Bad request! Message: {}\n", id, exc.what());
    conn.Disconnect();
  }
}

} // namespace server
} // namespace http
} // namespace bjo
