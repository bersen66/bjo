#pragma once
#include <boost/beast.hpp>

namespace http {

enum class MESSAGE : bool {
  REQUEST = true,
  RESPONSE = false,
};

enum class METHOD {
  GET,
  POST,
  PUT,
  PATCH,
  DELETE,
  OPTIONS,
};

using Request = boost::beast::http::request<boost::beast::http::string_body>;
using Response = boost::beast::http::response<boost::beast::http::string_body>;

} // namespace http
