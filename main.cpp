#include <cpr/cpr.h>
#include "crow/include/crow.h"
#include "MyMiddleware.h"
#include "Helpers.h"

static std::string
    authUrl{"https://www.googleapis.com/identitytoolkit/v3/relyingparty/"};
const std::string signUpEndpoint{"signupNewUser"};
const std::string loginEndpoint{"verifyPassword"};
const std::string authApiKey{"AIzaSyAD0toTbFtPT5Rjp8tuf1JnFWgHxX88nRM"};

const std::string firebaseUrl{"https://recipebook-f88d0.firebaseio.com/"};
const std::string recipesEndpoint{"recipes.json"};
const std::string shoppingListEndpoint{"shopping-list.json"};

crow::response handleAuth(const crow::request &req,
                          const std::string &endPoint) {
  auto payload = crow::json::load(req.body);

  if (!payload) return crow::response(400, "Cannot decode request body.");

  std::string email, password;
  try {
    email = payload["email"].s();
    password = payload["password"].s();
  }
  catch (const std::runtime_error &error) {
    std::ostringstream oss;
    oss << error.what();
    if (oss.str() == "value is not string") {
      return crow::response(400, "Bad request body.");
    }
    return crow::response(400, "Unknown bad request.");
  }

  crow::json::wvalue reqBody;
  reqBody["email"] = email;
  reqBody["password"] = password;
  reqBody["returnSecureToken"] = true;

  auto r = cpr::Post(
      cpr::Url{authUrl + endPoint},
      cpr::Parameters{{"key", authApiKey}},
      cpr::Body{crow::json::dump(reqBody)},
      cpr::Header{{"Content-Type", "application/json"}}
  );

  return crow::response(r.status_code, crow::json::load(r.text));
}

int main() {
  crow::App<middleware::MyMiddleware> app;

  // Dummy health check endpoint.
  CROW_ROUTE(app, "/health")(
      []() {
        return crow::response(200, "CROW running...");
      }
  );

  CROW_ROUTE(app, "/signup").methods("POST"_method)(
      [](const crow::request &req) {
        return handleAuth(req, signUpEndpoint);
      }
  );

  CROW_ROUTE(app, "/login").methods("POST"_method)(
      [](const crow::request &req) {
        return handleAuth(req, loginEndpoint);
      }
  );

  // Get all recipes.
  CROW_ROUTE(app, "/get-recipes").methods("GET"_method)(
      [&app](const crow::request &req) {
        auto ctx = app.get_context<middleware::MyMiddleware>(req);
        CROW_LOG_INFO << "user token: " + ctx.userIdToken;
        auto r = cpr::Get(
            cpr::Url{firebaseUrl + recipesEndpoint},
            cpr::Parameters{{"auth", ctx.userIdToken}}
        );
        CROW_LOG_INFO << r.status_code;
        CROW_LOG_INFO << r.text;
        return crow::response(r.status_code, crow::json::load(r.text));
      }
  );

  // Add or update a single recipe.
  CROW_ROUTE(app, "/recipe").methods("POST"_method)(
      []() {
        return "Update or add recipe.";
      }
  );

  // Get all shopping list items.
  CROW_ROUTE(app, "/get-shopping-list").methods("GET"_method)(
      []() {
        return "all shopping list items";
      }
  );

  // Add or update a single recipe.
  CROW_ROUTE(app, "/shopping-list-item").methods("POST"_method)(
      []() {
        return "Update or add a recipe.";
      }
  );

  app.port(12345).concurrency(2).run();
}