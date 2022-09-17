#pragma once

#include "core/http/server/config/config.hpp"
#include "core/http/server/routes/router.hpp"

namespace http {
namespace server {

void RunServer(const ConfigPtr &config = DefaultConfig(),
               const RouterPtr &router = DefaultRouter());

} // namespace server
} // namespace http