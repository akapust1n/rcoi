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
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    const std::string body = getRequestBody(request);
    const Wt::Http::Message msg = model->createComment(request.headers(), body);
    writeOutput(msg, response);
}

Like::Like(Model* _model)
    : Base(_model)
{
}

void Like::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    const std::string body = getRequestBody(request);
    const Wt::Http::Message& msg = model->like(request.headers(), body);
    writeOutput(msg, response);
}
