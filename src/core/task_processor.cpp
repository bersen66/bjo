#include "core/task_processor.hpp"

namespace core
{
TaskProcessor::TaskProcessor(int task_in_the_same_time)
    : ioc_(std::max(task_in_the_same_time, 1))
    , thread_num_(std::max(task_in_the_same_time, 1))
{
}

void TaskProcessor::Start()
{
  auto& ioc = GetIOContext();
  for (int i = 0; i < thread_num_; i++)
  {
    thread_pool_.create_thread([&ioc]() {
      WorkGuard guard(ioc.get_executor()); // Prevents finishing of event-loop
      ioc.run();
    });
  }
}

void TaskProcessor::Join()
{
  thread_pool_.join_all();
}

boost::asio::io_context& TaskProcessor::GetIOContext()
{
  return ioc_;
}
} // namespace core
