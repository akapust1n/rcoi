#include "News.h"
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"

using namespace HttpAssist;

GetTitles::GetTitles(Model* _model)
    : Base(_model)
{
}

void GetTitles::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
    response.addHeader("Access-Control-Allow-Origin", "*");
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    if (request.method() != "GET") {
        WriteResponse(response, 403);
        return;
    }
    const Wt::Http::Message msg = model->getTitles(request.headers(), request.queryString());
    writeOutput(msg, response);
}

CreateNews::CreateNews(Model* _model)
    : Base(_model)
{
}

void CreateNews::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
    response.addHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    response.addHeader("Access-Control-Allow-Origin", "*");
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    if (request.method() != "POST") {
        WriteResponse(response, 403);
        return;
    }
    uint32_t userId = 0;
    if (!model->checkAuth(request.headers(), userId, Actions_createNews)) {
        WriteResponse(response, 400);
        return;
    }
    const std::string body = getRequestBody(request);
    const Wt::Http::Message& msg = model->createNews(request.headers(), body);
    writeOutput(msg, response);
}

OneNews::OneNews(Model* _model)
    : Base(_model)
{
}

void OneNews::handleRequest(const Http::Request& request, Http::Response& response)
{
    response.addHeader("Access-Control-Allow-Origin", "*");
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    if (request.method() != "GET") {
        WriteResponse(response, 403);
        return;
    }
    const Wt::Http::Message msg = model->getOneNews(request.headers(), request.queryString());
    writeOutput(msg, response);
}

#ifdef IS_TEST_BUILD

Clear::Clear(Model* _model)
    : Base(_model)
{
}

void Clear::handleRequest(const Http::Request& request, Http::Response& response)
{
    std::cout << std::endl
              << "HEY";
    const Wt::Http::Message msg = model->clear(request.headers());
    writeOutput(msg, response);
}
#endif
