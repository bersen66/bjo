#pragma once

#include <boost/noncopyable.hpp>
#include <optional>
#include <list>
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

  //[[nodiscard]] bool CanHandle(std::string_view route) const;

  void InsertHandler(PatternPtr&& pattern_ptr, HandlerHolder&& handler);

  [[nodiscard]] const HandlerHolder& GetHandler(std::string_view route) const;


private:
 // std::optional<size_t> GetMatchingPattern(const std::string &route) const;


private:

//  std::list<PatternPtr> patterns_;
  std::unordered_map<PatternPtr, HandlerHolder> handlers_;

};

} // namespace server
} // namespace http
} // namespace bjo


