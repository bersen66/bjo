#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/noncopyable.hpp>
#include <memory>
#include <optional>

#include "core/http/messages.hpp"
#include <spdlog/spdlog.h>

namespace core
{
namespace http
{

class Connection : private boost::noncopyable
{
public:
  explicit Connection(boost::asio::ip::tcp::socket&& socket, unsigned int timeout = 3);

  Connection(Connection&&) = default;

  ~Connection();

  template <typename Body = boost::beast::http::string_body, typename Fields = boost::beast::http::fields>
  auto FetchRequest() -> boost::asio::awaitable<std::optional<boost::beast::http::request<Body, Fields>>>
  {
    co_return co_await FetchMessage<MESSAGE::REQUEST, Body, Fields>();
  }

  template <typename Body = boost::beast::http::string_body, typename Fields = boost::beast::http::fields>
  auto FetchResponse() -> boost::asio::awaitable<std::optional<boost::beast::http::response<Body, Fields>>>
  {
    co_return co_await FetchMessage<MESSAGE::RESPONSE, Body, Fields>();
  }

  template <typename Body = boost::beast::http::string_body, typename Fields = boost::beast::http::fields>
  auto SendRequest(boost::beast::http::request<Body, Fields>&& request) -> boost::asio::awaitable<size_t>
  {
    co_return co_await SendMessage<MESSAGE::REQUEST, Body, Fields>(std::move(request));
  }

  template <typename Body = boost::beast::http::string_body, typename Fields = boost::beast::http::fields>
  auto SendResponse(boost::beast::http::response<Body, Fields>&& response) -> boost::asio::awaitable<size_t>
  {
    co_return co_await SendMessage<MESSAGE::RESPONSE, Body, Fields>(std::move(response));
  }

  [[nodiscard]] bool IsAlive() const;

  void Disconnect();

private:
  template <MESSAGE MessageType, typename Body = boost::beast::http::string_body,
            typename Fields = boost::beast::http::fields>
  auto FetchMessage() -> boost::asio::awaitable<
      std::optional<boost::beast::http::message<static_cast<bool>(MessageType), Body, Fields>>>

  {
    if (!IsAlive())
    {
      throw std::runtime_error("Usage (reading) of closed connection!");
    }
    boost::beast::http::message<static_cast<bool>(MessageType), Body, Fields> message = {};
    try
    {
      stream_.expires_after(std::chrono::seconds(timeout_));
      co_await boost::beast::http::async_read(stream_, buffer_, message, boost::asio::use_awaitable);
    }
    catch (std::exception& exc)
    {
      Disconnect();
      spdlog::error("Exception while reading {}: {}\n", (static_cast<bool>(MessageType) ? "request" : "response"),
                    exc.what());
      co_return std::nullopt;
    }
    co_return std::make_optional(std::move(message));
  }

  template <MESSAGE MessageType, typename Body = boost::beast::http::string_body,
            typename Fields = boost::beast::http::fields>
  auto SendMessage(boost::beast::http::message<static_cast<bool>(MessageType), Body, Fields>&& message)
      -> boost::asio::awaitable<size_t>
  {
    if (!IsAlive())
    {
      throw std::runtime_error("Usage (writing) of closed connection!");
    }
    size_t bytes_transferred =
        co_await boost::beast::http::async_write(stream_, std::move(message), boost::asio::use_awaitable);
    co_return bytes_transferred;
  }

private:
  boost::beast::tcp_stream stream_;
  boost::beast::flat_buffer buffer_;
  bool is_alive_;
  unsigned int timeout_;
};

using ConnectionPtr = std::unique_ptr<Connection>;

auto ConnectTo(const std::string& url) -> boost::asio::awaitable<std::optional<ConnectionPtr>>;

auto ConnectTo(std::string&& url) -> boost::asio::awaitable<std::optional<ConnectionPtr>>;

auto ConnectTo(std::string_view url) -> boost::asio::awaitable<std::optional<ConnectionPtr>>;

} // namespace http
} // namespace core
