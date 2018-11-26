#include "Users.h"
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"
#include "Model.h"

using namespace ns;
using namespace HttpAssist;

Login::Login(Model* _model)
    : Base(_model)
{
}

void Login::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
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
