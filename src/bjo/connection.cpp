#include <optional>

#define SPDLOG_FMT_EXTERNAL
#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <memory>

#include "bjo/http/connection.hpp"

namespace bjo
{
namespace http
{

Connection::Connection(boost::asio::ip::tcp::socket&& socket, unsigned int timeout)
    : stream_(std::move(socket))
    , is_alive_(true)
    , timeout_(timeout)
{
  stream_.expires_after(std::chrono::seconds(timeout_));
  spdlog::info("Connection constructed");
}

Connection::~Connection()
{
  spdlog::info("Connection destructed");
}

bool Connection::IsAlive() const
{
  return is_alive_;
}

void Connection::Disconnect()
{
  is_alive_ = false;
}


auto ConnectTo(std::string_view url) -> boost::asio::awaitable<std::optional<ConnectionPtr>>
{
  auto executor = co_await boost::asio::this_coro::executor;
  boost::asio::ip::tcp::resolver resolver(executor);
  try
  {
    boost::asio::ip::tcp::resolver::query query(url.data(), "80");
    auto enabled_ips = co_await resolver.async_resolve(query, boost::asio::use_awaitable);

    boost::asio::ip::tcp::socket socket(executor);

    co_await boost::asio::async_connect(socket, enabled_ips, boost::asio::use_awaitable);

    co_return std::make_optional<ConnectionPtr>(std::make_unique<Connection>(std::move(socket)));
  }
  catch (const std::exception& exc)
  {
    spdlog::error("Exception when trying to connect to {}\n Exception message: {}", url, exc.what());
    co_return std::nullopt;
  }

}

auto ConnectTo(const std::string& url) -> boost::asio::awaitable<std::optional<ConnectionPtr>>
{
  co_return co_await ConnectTo(std::string_view(url));
}

auto ConnectTo(std::string&& url) -> boost::asio::awaitable<std::optional<ConnectionPtr>>
{
  co_return co_await ConnectTo(std::string_view(url));
}
} // namespace http
} // namespace bjo