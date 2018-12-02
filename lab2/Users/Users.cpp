#include "Users.h"
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"
#include "Model.h"

using namespace ns;
using namespace HttpAssist;
const int32_t hardCodedClientSecret = 123;

Login::Login(Model* _model)
    : Base(_model)
{
}

void Login::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        response.setStatus(401);
        return;
    }

    json_t userAuthjson = tryParsejson(getRequestBody(request));
    UserAuth userAuth;
    if (request.method() != "POST" or !from_json(userAuthjson, userAuth)) {
        response.setStatus(403);
        return;
    }

    json_t loginResponse = model->login(userAuth.name, userAuth.password);
    if (loginResponse.empty()) {
        response.setStatus(500);
        return;
    }
    response.out() << loginResponse.dump();
}

Reg::Reg(Model* _model)
    : Base(_model)
{
}

void Reg::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        response.setStatus(401);
        return;
    }
    json_t userAuthjson = tryParsejson(getRequestBody(request));
    UserAuth userAuth;
    if (request.method() != "POST" or !from_json(userAuthjson, userAuth)) {
        response.setStatus(403);
        return;
    }
    if (userAuth.name.size() < 1 or !userAuth.password.size()) {
        response.setStatus(403);
        return;
    }
    json_t newsResponse = model->reg(userAuth.name, userAuth.password);
    if (newsResponse.empty()) {
        response.setStatus(500);
        return;
    }
    response.out() << newsResponse.dump();
}

Del::Del(Model* _model)
    : Base(_model)
{
}

void Del::handleRequest(const Http::Request& request, Http::Response& response)
{
    json_t userIdjson = tryParsejson(getRequestBody(request));
    auto userIdIt = userIdjson.find("userId");
    if (request.method() != "DELETE" or userIdIt == userIdjson.end()) {
        response.setStatus(403);
        return;
    }

    json_t newsResponse = model->del(userIdIt.value().get<int32_t>());
    if (newsResponse.empty()) {
        response.setStatus(500);
        return;
    }
    response.out() << newsResponse.dump();
}

IncRating::IncRating(Model* _model)
    : Base(_model)
{
}

void IncRating::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        response.setStatus(401);
        return;
    }
    json_t userIdjson = tryParsejson(getRequestBody(request));
    auto userIdIt = userIdjson.find("userId");
    if (request.method() != "POST" or userIdIt == userIdjson.end()) {
        response.setStatus(403);
        return;
    }

    json_t newsResponse = model->incRating(userIdIt.value().get<int32_t>());
    if (newsResponse.empty()) {
        response.setStatus(500);
        return;
    }
    response.out() << newsResponse.dump();
}

GetUsernames::GetUsernames(Model* _model)
    : Base(_model)
{
}

void GetUsernames::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        response.setStatus(401);
        return;
    }
    auto ids = request.getParameterValues("id");
    if (request.method() != "GET") {
        response.setStatus(403);
        return;
    }
    std::vector<int32_t> idsInt;
    for (auto id : ids) {
        idsInt.push_back(atoi(id.c_str()));
    }
    json_t comments = model->getNames(idsInt);
    if (comments.empty()) {
        response.out() << "Cant find names comments";
        return;
    }
    response.out() << comments.dump();
}

#ifdef IS_TEST_BUILD

Clear::Clear(Model* _model)
    : Base(_model)
{
}

void Clear::handleRequest(const Http::Request& request, Http::Response& response)
{
    json result = model->clear();
    response.out() << result.dump();
}
#endif

CheckAuth::CheckAuth(Model* _model)
    : Base(_model)
{
}

void CheckAuth::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        response.setStatus(401);
        return;
    }
    auto token = request.getParameter("token");
    if (request.method() != "GET" or token == nullptr or ((*token).empty())) {
        response.setStatus(403);
        return;
    }
    //can throw
    try {
        auto dec_obj = jwt::decode(*token, algorithms({ "hs256" }), secret(key));
        uint32_t userId = static_cast<uint32_t>(std::stoi(dec_obj.payload().get_claim_value<std::string>("userId")));
        json_t result;
        result["userId"] = userId;
        response.setStatus(model->checkAuth(userId, *token));
        response.out() << result;
    } catch (...) {
        response.setStatus(401);
    }
}

GetAuthCode::GetAuthCode(Model* _model)
    : Base(_model)
{
}

void GetAuthCode::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        response.setStatus(401);
        return;
    }
    const int32_t clientId = extractPositiveParam(request, "client_id");
    const std::string callbackUrl = extractStringParam(request, "callback_url");
    if (clientId == -1 or callbackUrl.empty()) {
        response.setStatus(400);
        return;
    }
    int32_t authCode = model->getAuthCode(clientId);
    if (authCode > 0) {
        const std::string location = callbackUrl + "?code=" + std::to_string(authCode);
        std::cout << "LOCATION " << location << std::endl;
        response.addHeader("Location", location);
        response.setStatus(302);
    } else {
        response.setStatus(400);
    }
}
GetToken::GetToken(Model* _model)
    : Base(_model)
{
}
void GetToken::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        response.setStatus(401);
        return;
    }
    const int32_t clientId = extractPositiveParam(request, "client_id");
    const int32_t authCode = extractPositiveParam(request, "code");

    const int32_t clientSecret = extractPositiveParam(request, "client_secret");
    const std::string callbackUrl = extractStringParam(request, "callback_url");
    if (clientId == -1 or callbackUrl.empty() or clientSecret != 123) {
        response.setStatus(400);
        return;
    }
    const json_t tokens = model->getToken(clientId, authCode);
    if (!tokens.empty()) {
        response.out() << tokens.dump();
        response.addHeader("Location", callbackUrl);
        response.setStatus(200);
    } else {
        response.setStatus(401);
    }
}

RefreshToken::RefreshToken(Model* _model)
    : Base(_model)
{
}

void RefreshToken::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        response.setStatus(401);
        return;
    }
    json_t refreshTokenJson = tryParsejson(getRequestBody(request));
    auto refreshTokenIt = refreshTokenJson.find("refreshToken");
    if (request.method() != "POST" or refreshTokenIt == refreshTokenJson.end()) {
        response.setStatus(403);
        return;
    }
    const std::string refreshToken = refreshTokenIt.value().get<std::string>();
    json_t result = model->refreshToken(refreshToken);
    if (!result.empty()) {
        response.out() << result.dump();
        response.setStatus(200);
    } else {
        response.setStatus(401);
    }
}
