#include "Comments.h"
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"
#include "Model.h"

using namespace ns;
using namespace HttpAssist;

CreateComment::CreateComment(Model* _model)
    : Base(_model)
{
}

void CreateComment::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
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
    if (!model->checkAuth(request.headers(), userId)) {
        WriteResponse(response, 400);
        return;
    }

    std::string body = getRequestBody(request);
    HttpAssist::addUserId(body, userId);
    const Wt::Http::Message msg = model->createComment({}, body);
    writeOutput(msg, response);
}

Like::Like(Model* _model)
    : Base(_model)
{
}

void Like::handleRequest(const Http::Request& request, Http::Response& response)
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
    if (!model->checkAuth(request.headers(), userId)) {
        WriteResponse(response, 400);
        return;
    }

    std::string body = getRequestBody(request);
    HttpAssist::addUserId(body, userId);
    const Wt::Http::Message& msg = model->like({}, body);
    writeOutput(msg, response);
}
