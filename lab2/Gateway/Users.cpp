#include "Users.h"
#include "../Shared/HttpAssist.h"
using namespace HttpAssist;

Login::Login(Model* _model)
    : Base(_model)
{
}

void Login::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    const std::string body = getRequestBody(request);
    const Wt::Http::Message& msg = model->login(request.headers(), body);
    writeOutput(msg, response);
}

Register::Register(Model* _model)
    : Base(_model)
{
}

void Register::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    const std::string body = getRequestBody(request);
    const Wt::Http::Message msg = model->reg(request.headers(), body);
    writeOutput(msg, response);
}

DelUser::DelUser(Model* _model)
    : Base(_model)
{
}

void DelUser::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    const std::string body = getRequestBody(request);
    const Wt::Http::Message msg = model->del(request.headers(), body);
    writeOutput(msg, response);
}

GetAuthCode::GetAuthCode(Model* _model)
    : Base(_model)
{
}

void GetAuthCode::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    const Wt::Http::Message msg = model->getAuthCode(request.headers(), request.queryString());
    writeOutput(msg, response);
}

GetToken::GetToken(Model* _model)
    : Base(_model)
{
}

void GetToken::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    const Wt::Http::Message msg = model->getToken(request.headers(), request.queryString());
    writeOutput(msg, response);
}

RefreshToken::RefreshToken(Model* _model)
    : Base(_model)
{
}

void RefreshToken::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    const std::string body = getRequestBody(request);
    const Wt::Http::Message msg = model->refreshToken(request.headers(), body);
    writeOutput(msg, response);
}
