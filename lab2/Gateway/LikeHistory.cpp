#include "LikeHistory.h"
#include "../Shared/HttpAssist.h"
using namespace HttpAssist;

History::History(Model* _model)
    : Base(_model)
{
}

void History::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (request.method() == "OPTIONS") { //will be delete after debug
        response.setStatus(200);
        return;
    }
    if (request.method() != "GET") {
        WriteResponse(response, 403);
        return;
    }
    const Wt::Http::Message msg = model->history(request.headers(), request.queryString());
    writeOutput(msg, response);
}
