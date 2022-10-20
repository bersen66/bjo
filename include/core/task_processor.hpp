#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
namespace core
{

class TaskProcessor : private boost::noncopyable
{
public:
  using WorkGuard = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;

  explicit TaskProcessor(int task_in_the_same_time = 1);

  void Start();

  void Join();

  boost::asio::io_context& GetIOContext();

  template <typename Task, typename CompletionToken>
  auto ProcessTask(Task&& task, CompletionToken&& completion_token)
      -> decltype(boost::asio::co_spawn(boost::asio::make_strand(GetIOContext()), std::move(task),
                                        std::forward<CompletionToken>(completion_token)))
  {
    return boost::asio::co_spawn(boost::asio::make_strand(GetIOContext()), std::move(task),
                                 std::forward<CompletionToken>(completion_token));
  }

private:
  boost::asio::io_context ioc_;
  boost::thread_group thread_pool_;
  int thread_num_;
};

} // namespace core