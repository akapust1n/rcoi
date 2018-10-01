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
    const Wt::Http::Message msg = model->getTitles(request.headers(), request.queryString());
    writeOutput(msg, response);
}

CreateNews::CreateNews(Model* _model)
    : Base(_model)
{
}

void CreateNews::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
    const std::string body = getRequestBody(request);
    const Wt::Http::Message& msg = model->createNews(request.headers(), body);
    writeOutput(msg, response);
}
