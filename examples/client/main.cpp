#include "bjo/http.hpp"
#include "bjo/task_processor.hpp"
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <boost/asio.hpp>

///// DOESN'T WORK NOW

namespace asio = boost::asio;

bjo::TaskProcessor tp;

auto Get(std::string_view target) -> asio::awaitable<bjo::http::Response>
{

  auto connection_opt = co_await bjo::http::ConnectTo(target);
  if (!connection_opt.has_value())
  {
    throw std::runtime_error(fmt::format("Can't connect to {}", target));
  }

  auto& conn_ptr = connection_opt.value();

  {
    bjo::http::Request request = {};
    co_await conn_ptr->SendRequest(std::move(request));
  }

  bjo::http::Response result = (co_await conn_ptr->FetchResponse()).value();

  co_return result;
}

boost::asio::awaitable<void> AsyncMain() {
  bjo::http::Response res = co_await tp.ProcessTask(Get("0.0.0.0"), asio::use_awaitable);
  fmt::print("Result:\n{}", res);
  co_return;
}

int main()
{
  tp.ProcessTask(AsyncMain(), asio::detached);
  tp.Start<bjo::TaskProcessor::START_MODE::USE_THIS_THREAD>();
}