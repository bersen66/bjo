#pragma once

#include <boost/noncopyable.hpp>
#include <list>
#include <optional>
#include <unordered_map>

#include "bjo/http/server/routes/route.hpp"
#include "handler.hpp"

namespace bjo
{
namespace http
{
namespace server
{

class HandlersMap : private boost::noncopyable
{
public:
  HandlersMap() = default;

  const HandlerHolder& operator[](std::string_view route) const;

  void InsertHandler(PatternPtr&& pattern_ptr, HandlerHolder&& handler);

  [[nodiscard]] const HandlerHolder& GetHandler(std::string_view route) const;

private:
  std::unordered_map<PatternPtr, HandlerHolder> handlers_;
};

} // namespace server
} // namespace http
} // namespace bjo
