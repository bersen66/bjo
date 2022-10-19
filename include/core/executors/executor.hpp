#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

class Executor : private boost::noncopyable
{
public:
  using WorkGuard = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;

  explicit Executor(int thread_num = 1);

  void Run();

  void Join();

  boost::asio::io_context& GetIOService();

  template <typename Task, typename CompletionToken>
  auto Execute(const Task& task, CompletionToken&& completion_token)
      -> decltype(boost::asio::co_spawn(boost::asio::make_strand(GetIOService()), task(),
                                        std::forward<CompletionToken>(completion_token)))
  {
    return boost::asio::co_spawn(boost::asio::make_strand(GetIOService()), task(),
                                 std::forward<CompletionToken>(completion_token));
  }

private:
  boost::asio::io_context ioc_;
  boost::thread_group thread_pool_;
  int thread_num_;
};