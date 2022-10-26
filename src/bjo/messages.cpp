#include "bjo/http/messages.hpp"
namespace bjo
{
namespace http
{

METHODS operator|(const METHODS& lhs, const METHODS& rhs)
{
  return static_cast<METHODS>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
}

} // namespace http
} // namespace bjo