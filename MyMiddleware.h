//
// Created by William Liu on 2019-07-21.
//

#pragma once

#include "crow/include/crow.h"
#include "Helpers.h"

namespace middleware {
class MyMiddleware {
 public:
  class context {
   public:
    std::string userIdToken;
  };
  void before_handle(crow::request &req, crow::response &resp, context &ctx);
  void after_handle(crow::request &req, crow::response &resp, context &ctx);
 private:
  static void parseCookies(const crow::request &req,
                           middleware::MyMiddleware::context &ctx);
};
}
