#pragma once

#include <memory> // for std::shared_ptr
#include <ctre.hpp>


namespace bjo
{
namespace http
{
namespace server
{

struct RouteBase {
  virtual bool Match(std::string_view route) const = 0;
  virtual ~RouteBase() = default;
};

template<ctll::fixed_string pattern>
class RoutePattern : public RouteBase {

  bool Match(std::string_view src) const override {
    return ctre::match<pattern>(src);
  }

};

using PatternPtr = std::shared_ptr<RouteBase>;

template<ctll::fixed_string pattern>
PatternPtr Route() {
  return std::make_shared< RoutePattern<pattern> > ();
}


} // namespace server
} // namespace http
} // namespace bjo


