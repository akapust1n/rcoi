#include "News.h"
#include "../Shared/HttpAssist.h"
using namespace HttpAssist;

GetNews::~GetNews()
{
    beingDeleted();
}

GetNews::GetNews(Model* _model)
    : Base(_model)
{
}

//todo: pagination
void GetNews::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
    const Wt::Http::Message& msg = model->getfromNews(request.headers(), request.pathInfo(), "news");
    writeHeaders(response, msg.headers());
    response.out() << msg.body();
}
