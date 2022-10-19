#include <core/executors/executor.hpp>

Executor::Executor(int thread_num)
    : ioc_(std::max(thread_num, 1)),
      thread_num_(std::max(thread_num, 1))
{
}

void Executor::Run()
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

void Executor::Join()
{
  thread_pool_.join_all();
}

boost::asio::io_context& Executor::GetIOService()
{
  return ioc_;
}