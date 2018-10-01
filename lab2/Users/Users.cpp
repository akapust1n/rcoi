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
    json userAuthJson = json::parse(getRequestBody(request));
    UserAuth userAuth;
    if (request.method() != "POST" or !from_json(userAuthJson, userAuth)) {
        response.setStatus(403);
        return;
    }

    json newsResponse = model->login(userAuth.name, userAuth.password);
    if (newsResponse.empty()) {
        response.setStatus(500);
        return;
    }
    response.out() << newsResponse.dump();
}

Reg::Reg(Model* _model)
    : Base(_model)
{
}

void Reg::handleRequest(const Http::Request& request, Http::Response& response)
{
    json userAuthJson = json::parse(getRequestBody(request));
    UserAuth userAuth;
    if (request.method() != "POST" or !from_json(userAuthJson, userAuth)) {
        response.setStatus(403);
        return;
    }

    json newsResponse = model->reg(userAuth.name, userAuth.password);
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
    json userIdJson = json::parse(getRequestBody(request));
    UserAuth userAuth;
    auto userIdIt = userIdJson.find("userId");
    if (request.method() != "DELETE" or userIdIt == userIdJson.end()) {
        response.setStatus(403);
        return;
    }

    json newsResponse = model->del(userIdIt.value().get<int32_t>());
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
    json userIdJson = json::parse(getRequestBody(request));
    UserAuth userAuth;
    auto userIdIt = userIdJson.find("userId");
    if (request.method() != "POST" or userIdIt == userIdJson.end()) {
        response.setStatus(403);
        return;
    }

    json newsResponse = model->incRating(userIdIt.value().get<int32_t>());
    if (newsResponse.empty()) {
        response.setStatus(500);
        return;
    }
    response.out() << newsResponse.dump();
}
