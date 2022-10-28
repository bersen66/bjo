#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ostream.h>

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

#define BOOST_STACKTRACE_USE_BACKTRACE
#include <boost/asio.hpp>
#include <boost/stacktrace.hpp>

#include "bjo/http.hpp"

#include <profile.hpp>

void SoftQuit(int signal)
{
  fmt::print(fmt::fg(fmt::color::green), "FINISHED BY SIGNAL {}.", signal);
  std::exit(EXIT_SUCCESS);
}

void HandleSIGSEGV(int signal)
{
  // Show stacktrace if SIGSEGV
  spdlog::error("\nSIGSEGV OCCURED!\nSIGNAL: {}\nBACKTRACE: {}", signal, boost::stacktrace::stacktrace());
  std::exit(EXIT_FAILURE);
}

class AsClass
{
public:
  boost::asio::awaitable<bjo::http::Response> operator()(const bjo::http::Request& request)
  {

    spdlog::info("Handling {}", request.target());
    bjo::http::Response result = {};
    result.body() = R"(Hello world!)";
    result.prepare_payload();
    co_return result;
  }
};

boost::asio::awaitable<bjo::http::Response> AsFunc(const bjo::http::Request& request)
{
  spdlog::info("Handling {}", request.target());
  bjo::http::Response result = {};
  result.body() = "From func\n";
  result.prepare_payload();
  co_return result;
}

int main(int argc, char** argv)
{

  spdlog::set_pattern("[%H:%M:%S %z] [thread %t] %v");
  spdlog::info("app started");

  // Setting up handlers for default OS signals.
  std::signal(SIGINT, SoftQuit);  // Close by Ctrl + C
  std::signal(SIGQUIT, SoftQuit); // Close by Ctrl + \ or Ctrl + 4 or SysRq
  std::signal(SIGHUP, SoftQuit);  // Close by disconnect
  std::signal(SIGTERM, SoftQuit);
  std::signal(SIGSEGV, HandleSIGSEGV); // Smth bad in memory

  bjo::http::Server server(bjo::http::server::DefaultConfig());
  server.RegisterHandlers()
      (bjo::http::METHODS::GET, bjo::http::server::Route<"/">(), AsClass{})
      (bjo::http::METHODS::GET | bjo::http::METHODS::POST, bjo::http::server::Route<"/post">(), AsFunc)
  ;
  server.Serve();

  return 0;
}
