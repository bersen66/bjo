#include <gtest/gtest.h>

#include <fmt/core.h>
#include <fmt/ostream.h>

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

#define BOOST_STACKTRACE_USE_BACKTRACE
#include <boost/stacktrace.hpp>

#include "boost/asio.hpp"
#include "core/http/server/server.hpp"

#include <ctre.hpp>

void SoftQuit(int signal)
{
  spdlog::info("Closed by signal: {}\n", signal);
  std::exit(EXIT_SUCCESS);
}

void HandleSIGSEGV(int signal)
{
  spdlog::error("\nSIGSEGV OCCURED!\nSIGNAL: {}\nBACKTRACE: {}", signal, boost::stacktrace::stacktrace());
  std::exit(EXIT_FAILURE);
}

class Handler : public http::server::HandlerBase {
public:
  static constexpr ctll::fixed_string route = "/First/[0-9a-z]+";

  bool CanHandle(std::string_view url) const override {
    return ctre::match<route>(url);
  }

  boost::asio::awaitable<http::Response> Handle(const http::Request&) const override {
    http::Response result = {};
    result.body() = "<h1>First<\h1>";
    result.prepare_payload();
    co_return result;
  }
};

class HandlerTwo : public http::server::HandlerBase {
public:
  static constexpr ctll::fixed_string route = "/Second/[0-9a-z]+";

  bool CanHandle(std::string_view url) const override {
    return ctre::match<route>(url);
  }
  boost::asio::awaitable<http::Response> Handle(const http::Request&) const override {
    http::Response result = {};
    result.body() = "<h1>Second<\h1>";
    result.prepare_payload();
    co_return result;
  }
};

class HandleFavicon : public http::server::HandlerBase {
public:
  static constexpr ctll::fixed_string route = "/favicon.ico";

  bool CanHandle(std::string_view url) const override {
    return ctre::match<route>(url);
  }
  boost::asio::awaitable<http::Response> Handle(const http::Request& req) const override {
    http::Response result = {boost::beast::http::status::bad_request, req.version()};
    result.prepare_payload();
    co_return result;
  }
};




int main(int argc, char** argv)
{

  // Setting up handlers for default OS signals.
  std::signal(SIGINT, SoftQuit);  // Close by Ctrl + C
  std::signal(SIGQUIT, SoftQuit); // Close by Ctrl + \ or Ctrl + 4 or SysRq
  std::signal(SIGHUP, SoftQuit);  // Close by disconnect
  std::signal(SIGTERM, SoftQuit);
  std::signal(SIGSEGV, HandleSIGSEGV); // Smth bad in memory

  http::server::Server server(http::server::DefaultConfig());

  server.RegisterHandlers()
      (http::METHODS::GET, std::make_unique<Handler>())
      (http::METHODS::GET, std::make_unique<HandlerTwo>())
      (http::METHODS::GET, std::make_unique<HandleFavicon>())
  ;

  server.Serve();

  return 0;
}
