//
// Created by William Liu on 2019-07-21.
//
#pragma once

#include "MyMiddleware.h"

void middleware::MyMiddleware::after_handle(crow::request &req,
                                                crow::response &resp,
                                                middleware::MyMiddleware::context &ctx) {
  resp.set_header("Access-Control-Allow-Origin", "http://localhost:4200");
  resp.set_header("Access-Control-Allow-Credentials", "true");
}

void middleware::MyMiddleware::before_handle(crow::request &req,
                                                 crow::response &resp,
                                                 middleware::MyMiddleware::context &ctx) {
  int count = req.headers.count("Cookie");
  CROW_LOG_INFO << "count: " << count;
  if (count == 0) {
    return;
  }
  if (count > 1) {
    resp.code = 400;
    resp.end();
    return;
  }
  parseCookies(req, ctx);
  if (ctx.userIdToken.empty()) {
    resp.end();
  }
}

void middleware::MyMiddleware::parseCookies(const crow::request &req,
                                                middleware::MyMiddleware::context &ctx) {
  std::string cookiesString = req.get_header_value("Cookie");
  CROW_LOG_INFO << "raw cookie string: " + cookiesString;
  int curr = 0;
  while (curr < cookiesString.length()) {
    int eqPos = cookiesString.find_first_of('=', curr);
    if (eqPos == std::string::npos) break;

    std::string name = cookiesString.substr(curr, eqPos - curr);
    helpers::trim(name);
    CROW_LOG_INFO << "cookie name: " + name;

    curr = eqPos + 1;
    int semiColonPos = cookiesString.find_first_of(';', curr);

    std::string value = cookiesString.substr(curr, semiColonPos - curr);
    helpers::trim(value);
    CROW_LOG_INFO << "cookie value: " + value;

    if (name == "user") {
      ctx.userIdToken = crow::json::load(helpers::urlDecode(value))["idToken"].s();
      break;
    }

    // check this at the end because the Cookie header might be "x=1" without
    // an trailing semi colon.
    if (semiColonPos == std::string::npos) break;

    curr = semiColonPos + 1;
  }
}