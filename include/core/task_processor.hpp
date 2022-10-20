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

  enum class START_MODE
  {
    USE_THIS_THREAD,
    ASYNC_START,
  };

public:
  TaskProcessor(int task_in_the_same_time = 1);

  template <START_MODE mode = START_MODE::ASYNC_START> void Start()
  {
    auto& ioc = GetIOContext();
    for (int i = 0; i < thread_num_ - 1; i++)
    {
      thread_pool_.create_thread([&ioc]() {
        WorkGuard guard(ioc.get_executor()); // Prevents finishing of event-loop
        ioc.run();
      });
    }

    if constexpr (mode == START_MODE::USE_THIS_THREAD)
    {
      ioc.run();
    }
    else
    {
      thread_pool_.create_thread([&ioc]() {
        WorkGuard guard(ioc.get_executor()); // Prevents finishing of event-loop
        ioc.run();
      });
    }
  }

  void Join();

  boost::asio::io_context& GetIOContext();

  template <typename Task, typename CompletionToken>
  auto ProcessTask(Task&& task, CompletionToken&& completion_token)
      -> decltype(boost::asio::co_spawn(boost::asio::make_strand(GetIOContext()), std::forward<Task>(task),
                                        std::forward<CompletionToken>(completion_token)))
  {
    return boost::asio::co_spawn(boost::asio::make_strand(GetIOContext()), std::forward<Task>(task),
                                 std::forward<CompletionToken>(completion_token));
  }

private:
  boost::asio::io_context ioc_;
  boost::thread_group thread_pool_;
  int thread_num_;
};

} // namespace core