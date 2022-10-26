#pragma once
#include <boost/beast.hpp>


namespace bjo
{
namespace http
{

enum class MESSAGE : bool
{
  REQUEST = true,
  RESPONSE = false,
};

enum class METHODS : unsigned int
{
  GET = 1 << 0,
  POST = 1 << 1,
  PUT = 1 << 2,
  PATCH = 1 << 3,
  DELETE = 1 << 4,
  OPTIONS = 1 << 5,
};

METHODS operator|(const METHODS& lhs, const METHODS& rhs);


using Request = boost::beast::http::request<boost::beast::http::string_body>;
using Response = boost::beast::http::response<boost::beast::http::string_body>;

} // namespace http
} // namespace bjo

