#include <gtest/gtest.h>

#include <fmt/core.h>
#include <fmt/ostream.h>

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

#define BOOST_STACKTRACE_USE_BACKTRACE
#include <boost/stacktrace.hpp>

#include "boost/asio.hpp"
#include "core/http/server/server.hpp"



void SoftQuit(int signal) {
  spdlog::info("Closed by signal: {}\n", signal);
  std::exit(EXIT_SUCCESS);
}

void HandleSIGSEGV(int signal) {
  spdlog::error("\nSIGSEGV OCCURED!\nSIGNAL: {}\nBACKTRACE: {}", signal,
                boost::stacktrace::stacktrace());
  std::exit(EXIT_FAILURE);
}


struct HandlerOne{
  boost::asio::awaitable<http::Response> operator()(const http::Request) {
    http::Response res = {};
    co_return res;
  }
};

int main(int argc, char** argv) {
  // Setting up handlers for default OS signals.
  std::signal(SIGINT, SoftQuit);   // Close by Ctrl + C
  std::signal(SIGQUIT, SoftQuit);  // Close by Ctrl + \ or Ctrl + 4 or SysRq
  std::signal(SIGHUP, SoftQuit);   // Close by disconnect
  std::signal(SIGTERM, SoftQuit);
  std::signal(SIGSEGV, HandleSIGSEGV);  // Smth bad in memory



  http::server::Server server(
      http::server::DefaultConfig(),
      http::server::EmptyRouter());


  server.RegisterHandlers()
      (http::METHODS::GET, "^/includes/[0-9]+/$", HandlerOne{})
      (http::METHODS::GET, "^/$", HandlerOne{})
  ;

  server.Serve();
}
