#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ostream.h>

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

#define BOOST_STACKTRACE_USE_BACKTRACE
#include <boost/asio.hpp>
#include <boost/stacktrace.hpp>

#include "bjo/http.hpp"

/***
 * Implementation of handlers for default OS signals
 */
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

/***
 * You can implement handlers in a multiple ways.
 *
 * Your handler can be represented as class with overloaded operator()
 */
class AsClass
{
public:
  /**
   * Your class have to overload operator () with following return value and parameters
   */
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

  { // Setting up handlers for default OS signals.
#ifdef SIGINT
    std::signal(SIGINT, SoftQuit); // Close by Ctrl + C
#endif
#ifdef SIGQUIT
    std::signal(SIGQUIT, SoftQuit); // Close by Ctrl + \ or Ctrl + 4 or SysRq
#endif
#ifdef SIGHUP
    std::signal(SIGHUP, SoftQuit); // Close by disconnect
#endif
#ifdef SIGTERM
    std::signal(SIGTERM, SoftQuit);
#endif
#ifdef SIGSEGV
    std::signal(SIGSEGV, HandleSIGSEGV); // Smth bad in memory
#endif
  }

  /**
   * You have to attach your handlers to server components before running server.
   */

  // clang-format off
  bjo::http::Server server(bjo::http::server::DefaultConfig());
  server.RegisterHandlers()
      (bjo::http::METHODS::GET, bjo::http::server::Route<"/">(), AsClass{})
      (bjo::http::METHODS::GET | bjo::http::METHODS::POST, bjo::http::server::Route<"/post">(), AsFunc)
  ;
  // clang-format on
  server.Serve();

  return 0;
}
