#include "core/task_processor.hpp"

TaskProcessor::TaskProcessor(int thread_num)
    : ioc_(std::max(thread_num, 1))
    , thread_num_(std::max(thread_num, 1))
{
}

void TaskProcessor::Start()
{
  auto& ioc = GetIOService();
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

boost::asio::io_context& TaskProcessor::GetIOService()
{
  return ioc_;
}